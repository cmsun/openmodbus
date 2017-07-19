/*
********************************************************************************
*                              COPYRIGHT NOTICE
*                             Copyright (c) 2016
*                             All rights reserved
*
*  @FileName       : mbError.c
*  @Author         : scm 351721714@qq.com
*  @Create         : 2016/11/20 13:30:20
*  @Last Modified  : 2016/12/22 15:32:46
********************************************************************************
*/
#include "mbconfig.h"
#include "mbserial.h"
#include "mberror.h"

void mberror(mbFormat_t *format, mbu8 errcode) REENTRANT
{
    if(format->frame[0] == 0) /* 不回复广播 */
        return;

    format->sendBuff[0] = format->frame[0];
    format->sendBuff[1] = format->frame[1] | 0x80; /* 功能码最高位为1说明下位机回复的是一个异常码 */
    format->sendBuff[2] = errcode;
    format->appendChksumHandle(format->sendBuff, 5);
    format->sendHandle(format->sendBuff, 5);
    format->clearHandle();
}

