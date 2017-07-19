/*
********************************************************************************
*                              COPYRIGHT NOTICE
*                             Copyright (c) 2016
*                             All rights reserved
*
*  @FileName       : mbrtu.c
*  @Author         : scm 351721714@qq.com
*  @Create         : 2016/11/23 09:51:51
*  @Last Modified  : 2017/01/25 16:28:15
********************************************************************************
*/

#ifdef MB_MODBUS_RTU

#include <stddef.h>
#include "mbconfig.h"
#include "mbchecksum.h"
#include "mberror.h"
#include "mbformat.h"
#include "mbserial.h"

extern mbu8 mbRecBuff[MB_RECEIVE_BUFFER_LENGTH];
extern mbu8 mbSendBuff[MB_SEND_BUFFER_LENGTH];
extern volatile mbu16 mbRecIndex;
extern void mbClearBuff(void);

static void mbSerialAnalyze(void);
static int  mbCheckCRC(mbu8 *frame, mbu16 length);
static void mbAppendCRC(mbu8 *buff, mbu16 length);

static mbFormat_t mbSerialFormat = {
    /* .frame = */                  mbRecBuff,
    /* .frameLength = */            0,
    /* .sendBuff =  */              mbSendBuff,
    /* .buffCapacity =  */          MB_SEND_BUFFER_LENGTH, 
    /* .chksumLength =  */          2,
    /* .checkChksumHandle =  */     /* mbCheckCRC, */
    /* .appendChksumHandle =  */    mbAppendCRC,
    /* .sendHandle =  */            mbSerialSend,
    /* .clearHandle =  */           mbClearBuff,
};

/*
********************************************************************************
*                               mbSerialReceive 
*
* Description : 用户程序在串口中断函数中把接收到的数据通过mbSerialReceive函数传给
*               modbus模块进行数据处理。
*
* Arguments   : recvalue: 串口接收到的数据。
*
* Returns     : none
*
* Note(s)     : none
********************************************************************************
*/
void mbSerialReceive(mbu8 recvalue)
{
    mbDisableTimer();

    if(mbRecIndex < MB_RECEIVE_BUFFER_LENGTH-1)
    {
        mbRecBuff[mbRecIndex++] = recvalue;
        mbEnableTimer();
    }
    else
    {
        mbSerialAnalyze();
    }
}

/*
********************************************************************************
*                               mbOnTimer 
*
* Description : 用户程序在定时器中断函数中中调用这个函数。 
*
* Arguments   : none
*
* Returns     : none
*
* Note(s)     : modbus rtu在这个函数中判断一个帧的结束。
*               modbus ascii如果定时器中断，则认为是通信超时。
********************************************************************************
*/
void mbOnTimer(void)
{
    mbDisableTimer();
    mbSerialAnalyze();
}

/*
********************************************************************************
*                               mbSerialAnalyze 
*
* Description : 初步分析一个modbus帧，查看功能码是否合法，并根据功能码调用相应的处理函数。
*
* Arguments   : none
*
* Returns     : none
*
* Note(s)     : none
* 
********************************************************************************
*/
static void mbSerialAnalyze(void)
{
    mbu16 devid, funcode; 

    /* 计算设备地址和功能码 */
    if(mbRecIndex < 4)    /* modbus RTU上位机的报文最小长度为4 */
        goto clear;
    devid = mbRecBuff[0];
    funcode = mbRecBuff[1];

    /* 如果接收到的设备地址和本设备地址不相符，或者接收到的设备地址不是广播地址，则不做处理 */
    if(devid != MB_SLAVE_DEVID && devid != 0)
        goto clear;

    if(funcode <= MB_FUNCTION_FIELD && mbFunArray[funcode] != NULL)
    {
        /* 检查校验和 */
        if(!mbCheckCRC(mbRecBuff, mbRecIndex))
        {
            mberror(&mbSerialFormat, MBERR_CHECKSUM);
        }
        else
        {
            mbSerialFormat.frameLength = mbRecIndex;
            mbFunArray[funcode](&mbSerialFormat);
        }
    }
    else
    {
        mberror(&mbSerialFormat, MBERR_CODE);
    }

clear:
    mbClearBuff();
}

/*
********************************************************************************
*                               mbCheckCRC 
*
* Description : 检查rtu帧的CRC校验和
*
* Arguments   : fram:rtu数据帧首地址。length:rtu帧的长度。
*
* Returns     : 返回1:校验成功。返回0校验失败。
*
* Note(s)     : none
********************************************************************************
*/
static int mbCheckCRC(mbu8 *frame, mbu16 length)
{
    mbu16 crc = frame[length-2]<<8 | frame[length-1];
    return (mbComputeCRC(frame, length-2) == crc) ? 1 : 0;
}

/*
********************************************************************************
*                               mbAppendCRC 
*
* Description : 计算buff[0]到buff[length-3]之间的CRC校验和
*               并把crc高字节放在buff[length-2]里，把crc低字节放在buff[length-1]里
*
* Arguments   : buff: 要计算CRC校验和的数据
*               length: buff的长度
*
* Returns     : none
*
* Note(s)     : 计算的CRC校验和以网络字节序（大端字节序）的方式加在buff尾部
********************************************************************************
*/
static void mbAppendCRC(mbu8 *buff, mbu16 length)
{
    mbu16 crc = mbComputeCRC(buff, length-2);
#ifdef MB_BIG_ENDIAN
    *(mbu16*)(buff+length-2) = crc;
#else
    *(mbu16*)(buff+length-2) = crc>>8 | crc<<8;
#endif
}

#endif /* #ifdef MB_MODBUS_RTU */
