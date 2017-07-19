/*
********************************************************************************
*                              COPYRIGHT NOTICE
*                             Copyright (c) 2016
*                             All rights reserved
*
*  @FileName       : mbformat.c
*  @Author         : scm 351721714@qq.com
*  @Create         : 2016/11/20 13:29:07
*  @Last Modified  : 2017/01/25 16:26:50
********************************************************************************
*/
#include <stddef.h>
#include "mbconfig.h"
#include "mbchecksum.h"
#include "mberror.h"
#include "mbproc.h"
#include "mbformat.h"

mbFormatHandle_t mbFunArray[MB_FUNCTION_FIELD+1] = {
    NULL,
    mbformat01,
    mbformat02,
    mbformat03,
    mbformat04,
    mbformat05,
    mbformat06,
    NULL, NULL, NULL,
    NULL, NULL, NULL,
    NULL, NULL, NULL,
    mbformat16,
};

/*
********************************************************************************
*                               mb_add_function
*
* Description : 扩展modbus功能
*
* Arguments   : funcode: 要添加的功能码。
*               handle: 相应功能码的处理函数。
*
* Returns     : 0: 扩展失败。
*               1：扩展成功。
*
* Note(s)     : 22以内的功能码已经被modbus标准定义，不允许修改。
*               127以上的功能码作为异常码，不允许使用。
********************************************************************************
*/
int mb_add_function(mbu8 funcode, mbFormatHandle_t handle)
{
    if(funcode < 22  || funcode > 127 || funcode > MB_FUNCTION_FIELD)
        return 0;
    mbFunArray[funcode] = handle;
    return 1;
}

void mbformat01(mbFormat_t *format)
{
    uint8_t *frame, *sendbuff, chksumlen;
    mbu16 framelen, addr, count, sendlen;

    frame = format->frame;
    sendbuff = format->sendBuff;
    framelen = format->frameLength;
    chksumlen = format->chksumLength;

    /* 对于ASCII: dataLength和buffCapacity是除去起始符、终止符之后，再把ASCII字符串转成BYTE后的数据长度 */
    /* 对于TCP：dataLength和buffCapacity是除去TCP/IP头部之后的长度 */
    if(framelen != 6U + chksumlen)
    {
        mberror(format, MBERR_FORMAT);
        return;
    }

    /* 检查modbus地址是否超出范围 */
    addr = frame[2]<<8 | frame[3];
    count = frame[4]<<8 | frame[5];
    if(addr >= MB_COIL_COUNT || (addr+count) > MB_COIL_COUNT)
    {
        mberror(format, MBERR_ADDR);
        return;
    }
    
    /* 检查是否有足够的回复空间 */
    sendlen = 3 + (count%8 ? count/8+1 : count/8) + chksumlen;
    if(sendlen > format->buffCapacity)
    {
        mberror(format, MBERR_FAULT);
        return;
    }

    /* 如果接收的数据报文格式无误，则进行数据处理 */
    mbproc01(frame, sendbuff);
    /* 添加校验码 */
    format->appendChksumHandle(sendbuff, sendlen);
    if(frame[0] != 0) format->sendHandle(sendbuff, sendlen);
}

void mbformat02(mbFormat_t *format) 
{
    uint8_t *frame, *sendbuff, chksumlen;
    mbu16 framelen, addr, count, sendlen;

    frame = format->frame;
    sendbuff = format->sendBuff;
    framelen = format->frameLength;
    chksumlen = format->chksumLength;

    if(framelen != 6U + chksumlen)
    {
        mberror(format, MBERR_FORMAT);
        return;
    }

    /* 检查modbus地址是否超出范围 */
    addr = frame[2]<<8 | frame[3];
    count = frame[4]<<8 | frame[5];
    if(addr >= MB_DIGITAL_COUNT || (addr+count) > MB_DIGITAL_COUNT)
    {
        mberror(format, MBERR_ADDR);
        return;
    }

    /* 检查是否有足够的回复空间 */
    sendlen = 3 + (count%8 ? count/8+1 : count/8) + chksumlen;
    if(sendlen > format->buffCapacity)
    {
        mberror(format, MBERR_FAULT);
        return;
    }

    /* 如果接收的数据报文格式无误，则进行数据处理 */
    mbproc02(frame, sendbuff);
    format->appendChksumHandle(sendbuff, sendlen);
    if(frame[0] != 0) format->sendHandle(sendbuff, sendlen);
}

void mbformat03(mbFormat_t *format)
{
    uint8_t *frame, *sendbuff, chksumlen;
    mbu16 framelen, addr, count, sendlen;

    frame = format->frame;
    sendbuff = format->sendBuff;
    framelen = format->frameLength;
    chksumlen = format->chksumLength;

    if(framelen != 6U + chksumlen)
    {
        mberror(format, MBERR_FORMAT);
        return;
    }

    /* 检查modbus地址是否超出范围 */
    addr = frame[2]<<8 | frame[3];
    count = frame[4]<<8 | frame[5];
    if(addr >= MB_REGISTER_COUNT || (addr+count) > MB_REGISTER_COUNT)
    {
        mberror(format, MBERR_ADDR);
        return;
    }
    
    /* 检查是否有足够的回复空间 */
    sendlen = 3 + count*2 + chksumlen;
    if(sendlen > format->buffCapacity)
    {
        mberror(format, MBERR_FAULT);
        return;
    }

    /* 如果接收的数据报文格式无误，则进行数据处理 */
    mbproc03(frame, sendbuff);
    format->appendChksumHandle(sendbuff, sendlen);
    if(frame[0] != 0) format->sendHandle(sendbuff, sendlen);
}

void mbformat04(mbFormat_t *format)
{
    uint8_t *frame, *sendbuff, chksumlen;
    mbu16 framelen, addr, count, sendlen;

    frame = format->frame;
    sendbuff = format->sendBuff;
    framelen = format->frameLength;
    chksumlen = format->chksumLength;

    if(framelen != 6U + chksumlen)
    {
        mberror(format, MBERR_FORMAT);
        return;
    }

    /* 检查modbus地址是否超出范围 */
    addr = frame[2]<<8 | frame[3];
    count = frame[4]<<8 | frame[5];
    if(addr >= MB_ANALOGIN_COUNT*4 || (addr+count) > MB_ANALOGIN_COUNT*4)
    {
        mberror(format, MBERR_ADDR);
        return;
    }
    
    /* 检查是否有足够的回复空间 */
    sendlen = 3 + count*2 + chksumlen;
    if(sendlen > format->buffCapacity)
    {
        mberror(format, MBERR_FAULT);
        return;
    }

    /* 如果接收的数据报文格式无误，则进行数据处理 */
    mbproc04(frame, sendbuff);
    format->appendChksumHandle(sendbuff, sendlen);
    if(frame[0] != 0) format->sendHandle(sendbuff, sendlen);
}

void mbformat05(mbFormat_t *format)
{
    uint8_t *frame, *sendbuff, chksumlen;
    mbu16 framelen, addr;

    frame = format->frame;
    sendbuff = format->sendBuff;
    framelen = format->frameLength;
    chksumlen = format->chksumLength;

    if(framelen != 6U + chksumlen)
    {
        mberror(format, MBERR_FORMAT);
        return;
    }

    /* 检查modbus地址是否超出范围 */
    addr = frame[2]<<8 | frame[3];
    if(addr >= MB_COIL_COUNT)
    {
        mberror(format, MBERR_ADDR);
        return;
    }

    /* 如果接收的数据报文格式无误，则进行数据处理 */
    mbproc05(frame, sendbuff);
    format->appendChksumHandle(sendbuff, 6 + chksumlen);
    if(frame[0] != 0) format->sendHandle(sendbuff, 6 + chksumlen);   
}

void mbformat06(mbFormat_t *format)
{
    uint8_t *frame, *sendbuff, chksumlen;
    mbu16 framelen, addr;

    frame = format->frame;
    sendbuff = format->sendBuff;
    framelen = format->frameLength;
    chksumlen = format->chksumLength;

    if(framelen != 6U + chksumlen)
    {
        mberror(format, MBERR_FORMAT);
        return;
    }

    /* 检查modbus地址是否超出范围 */
    addr = frame[2]<<8 | frame[3];
    if(addr >= MB_REGISTER_COUNT)
    {
        mberror(format, MBERR_ADDR);
        return;
    }

    /* 如果接收的数据报文格式无误，则进行数据处理 */
    mbproc06(frame, sendbuff);
    format->appendChksumHandle(sendbuff, 6 + chksumlen);
    if(frame[0] != 0) format->sendHandle(sendbuff, 6 + chksumlen);
}

void mbformat16(mbFormat_t *format)
{
    uint8_t *frame, *sendbuff, chksumlen;
    mbu16 framelen, addr;

    frame = format->frame;
    sendbuff = format->sendBuff;
    framelen = format->frameLength;
    chksumlen = format->chksumLength;

    /* 检查报文格式 */
    if(frame[5]*2 != frame[6] || 
       framelen != frame[6]+7U + chksumlen)
    {
        mberror(format, MBERR_FORMAT);
        return;
    }   

    /* 检查modbus地址是否超出范围 */
    addr = frame[2]<<8 | frame[3];
    if(addr >= MB_REGISTER_COUNT || (addr+frame[5]) > MB_REGISTER_COUNT)
    {
        mberror(format, MBERR_ADDR);
        return;
    }

    /* 如果接收的数据报文格式无误，则进行数据处理 */
    mbproc16(frame, sendbuff);
    format->appendChksumHandle(sendbuff, 6 + chksumlen);
    if(frame[0] != 0) format->sendHandle(sendbuff, 6 + chksumlen);
}
