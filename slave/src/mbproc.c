/*
********************************************************************************
*                              COPYRIGHT NOTICE
*                             Copyright (c) 2016
*                             All rights reserved
*
*  @FileName       : mbproc.c
*  @Author         : scm 351721714@qq.com
*  @Create         : 2016/11/20 13:30:06
*  @Last Modified  : 2016/11/22 22:31:19
********************************************************************************
*/
#include <string.h>
#include "mbconfig.h"
#include "mbdata.h"

void mbproc01(mbu8 *frame, mbu8 *sendbuff)
{
    mbu8 movBit, remainBit;
    mbu16 i, modbusAddr, startAddr, endAddr, bitCount, byteCount;

    //modbus通信协议是大端字节序
    //把modbus数据读到本地内存时，不管运行代码的设备是大端还是小端，用移位的方法得到的数值大小是一样的
    //所以其实只需要modbusAddr = frame[2]<<8 | frame[3]这个代码就可以在大小端下通用
    modbusAddr = frame[2]<<8 | frame[3];            //modbus起始地址
    movBit = modbusAddr % 8;                            //bmCoil中要读取的位所在字节要右移的位数 
    startAddr = modbusAddr / 8;                         //要读取的mbCoil起始地址
    bitCount = frame[4]<<8 | frame[5];              //总共要读取的位数
    endAddr = (modbusAddr+bitCount-1) / 8;              //读取的最后一位所在的mbCoil的地址
    byteCount = bitCount%8 ? bitCount/8+1 : bitCount/8; //要返回的字节数
    remainBit = 7-(modbusAddr+bitCount-1)%8;            //要读取的最后一个位所在字节剩下的位数

    for(i = 0; i < byteCount; ++i)
    {
        sendbuff[3+i] = 0;
        if((modbusAddr+i*8)/8 < endAddr) //如果当前读取的mbCoil所在的字节小于endAddr所在字节 
        {
            sendbuff[3+i] |= mbCoil[startAddr+i] >> movBit;
            sendbuff[3+i] |= mbCoil[startAddr+i+1] << (8-movBit);
            if(i == byteCount-1)
            {
                sendbuff[3+i] <<= remainBit-(8-movBit);         //左移，把不读取的位清0
                sendbuff[3+i] >>= remainBit-(8-movBit);         //右移回去
            }
        }
        else                                                //如果当前读取的mbCoil所在的字节等于endAddr所在字节 
        {
            sendbuff[3+i] = mbCoil[startAddr+i] << remainBit;  //右移，把不读取的位清0
            sendbuff[3+i] >>= movBit + remainBit;              //左移回去
        }
    }
    
    sendbuff[0] = frame[0];
    sendbuff[1] = frame[1];
    sendbuff[2] = byteCount;
}

void mbproc02(mbu8 *frame, mbu8 *sendbuff)
{
    mbu8 movBit, remainBit;
    mbu16 i, modbusAddr, startAddr, endAddr, bitCount, byteCount;

    modbusAddr = frame[2]<<8 | frame[3];            //modbus起始地址
    movBit = modbusAddr % 8;                            //bmCoil中要读取的位所在字节要左移的位数 
    startAddr = modbusAddr / 8;                         //要读取的mbDigitIn起始地址
    bitCount = frame[4]<<8 | frame[5];              //总共要读取的位数
    endAddr = (modbusAddr+bitCount-1) / 8;              //读取的最后一位所在的mbCoil的地址
    byteCount = bitCount%8 ? bitCount/8+1 : bitCount/8; //要返回的字节数
    remainBit = 7-(modbusAddr+bitCount-1)%8;            //要读取的最后一个位所在字节剩下的位数

    for(i = 0; i < byteCount; ++i)
    {
        sendbuff[3+i] = 0;
        if((modbusAddr+i*8)/8 < endAddr) //如果当前读取的mbDigitIn所在的字节小于endAddr所在字节 
        {
            sendbuff[3+i] |= mbDigitIn[startAddr+i] >> movBit;
            sendbuff[3+i] |= mbDigitIn[startAddr+i+1] << (8-movBit);
            if(i == byteCount-1)
            {
                sendbuff[3+i] <<= remainBit-(8-movBit);           //左移，把不读取的位清0
                sendbuff[3+i] >>= remainBit-(8-movBit);           //右移回去
            }
        }
        else                                                    //如果当前读取的mbCoil所在的字节等于endAddr所在字节 
        {
            sendbuff[3+i] = mbDigitIn[startAddr+i] << remainBit;   //右移，把不读取的位清0
            sendbuff[3+i] >>= movBit + remainBit;                  //左移回去
        }
    }

    sendbuff[0] = frame[0];
    sendbuff[1] = frame[1];
    sendbuff[2] = byteCount;
}

void mbproc03(mbu8 *frame, mbu8 *sendbuff)
{
    mbu16 i, modbusAddr, registerCount;

    modbusAddr = frame[2]<<8 | frame[3];
    registerCount = frame[4]<<8 | frame[5];

    for(i = 0; i < registerCount; ++i)
    {
#ifdef BIG_ENDIAN
        ((mbu16*)(sendbuff+3))[i] = mbHoldReg[modbusAddr+i];
#else
        ((mbu16*)(sendbuff+3))[i] = mbHoldReg[modbusAddr+i]>>8 | mbHoldReg[modbusAddr+i]<<8;
#endif
    }

    sendbuff[0] = frame[0];
    sendbuff[1] = frame[1];
    sendbuff[2] = registerCount * 2;
}

void mbproc04(mbu8 *frame, mbu8 *sendbuff)
{
    mbu16 i, modbusAddr, registerCount;
    
    modbusAddr = frame[2]<<8 | frame[3];
    registerCount = frame[4]<<8 | frame[5];

    for(i = 0; i < registerCount; ++i)
    {
#ifdef BIG_ENDIAN
        ((mbu16*)(sendbuff+3))[i] = mbAnalogIn[modbusAddr+i];
#else
        ((mbu16*)(sendbuff+3))[i] = mbAnalogIn[modbusAddr+i]>>8 | mbAnalogIn[modbusAddr+i]<<8;
#endif
    }

    sendbuff[0] = frame[0];
    sendbuff[1] = frame[1];
    sendbuff[2] = registerCount * 2;
}

void mbproc05(mbu8 *frame, mbu8 *sendbuff)
{
    mbu8 movBit;
    mbu16 modbusAddr, byteAddr;

    modbusAddr = frame[2]<<8 | frame[3];
    movBit = modbusAddr%8;
    byteAddr = modbusAddr/8;
    frame[4] ? (mbCoil[byteAddr] |= 1<<movBit) : (mbCoil[byteAddr] &= ~(1<<movBit));
    memcpy(sendbuff, frame, 6);
}

void mbproc06(mbu8 *frame, mbu8 *sendbuff)
{
    mbu16 modbusAddr;

    modbusAddr = frame[2]<<8 | frame[3];
    mbHoldReg[modbusAddr] = frame[4]<<8 | frame[5];
    memcpy(sendbuff, frame, 6);
}

void mbproc16(mbu8 *frame, mbu8 *sendbuff)
{
    mbu16 i, modbusAddr, registerCount;

    modbusAddr = frame[2]<<8 | frame[3];
    registerCount = frame[4]<<8 | frame[5];

    for(i = 0; i < registerCount; ++i)
    {
        mbHoldReg[modbusAddr+i] = frame[7+i*2]<<8 | frame[8+i*2];
    }
    memcpy(sendbuff, frame, 6);
}

