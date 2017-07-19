/*
********************************************************************************
*                              COPYRIGHT NOTICE
*                             Copyright (c) 2016
*                             All rights reserved
*
*  @FileName       : mbproc.h
*  @Author         : scm 351721714@qq.com
*  @Create         : 2016/11/20 13:32:23
*  @Last Modified  : 2016/11/21 15:58:06
********************************************************************************
*/
#ifndef MBSLAVEH
#define MBSLAVEH

#include "mbconfig.h"

#ifdef __cplusplus
extern "C" {
#endif

void mbproc01(mbu8 *frame, mbu8 *sendbuff);
void mbproc02(mbu8 *frame, mbu8 *sendbuff);
void mbproc03(mbu8 *frame, mbu8 *sendbuff);
void mbproc04(mbu8 *frame, mbu8 *sendbuff);
void mbproc05(mbu8 *frame, mbu8 *sendbuff);
void mbproc06(mbu8 *frame, mbu8 *sendbuff);
void mbproc16(mbu8 *frame, mbu8 *sendbuff);

#ifdef __cplusplus
    } //extern "C" {
#endif

#endif

