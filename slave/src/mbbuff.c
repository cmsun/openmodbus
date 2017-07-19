/*
********************************************************************************
*                              COPYRIGHT NOTICE
*                             Copyright (c) 2016
*                             All rights reserved
*
*  @FileName       : mbbuff.c
*  @Author         : scm 351721714@qq.com
*  @Create         : 2016/11/23 11:15:51
*  @Last Modified  : 2017/01/25 16:16:54
********************************************************************************
*/

#include "mbconfig.h"

mbu8 mbRecBuff[MB_RECEIVE_BUFFER_LENGTH] = {0};
mbu8 mbSendBuff[MB_SEND_BUFFER_LENGTH] = {0};
volatile mbu16 mbRecIndex = 0;

void mbClearBuff(void)
{
    mbRecIndex = 0;
}

