/*
********************************************************************************
*                              COPYRIGHT NOTICE
*                             Copyright (c) 2016
*                             All rights reserved
*
*  @FileName       : mbserial.h
*  @Author         : scm 351721714@qq.com
*  @Create         : 2016/11/20 14:45:38
*  @Last Modified  : 2017/01/25 15:55:22
********************************************************************************
*/

#include "mbconfig.h"

#ifndef MBSERIAL_H
#define MBSERIAL_H

#ifdef __cplusplus
extern "C" {
#endif

void mbOnTimer(void);
void mbEnableTimer(void);
void mbDisableTimer(void);
void mbSerialReceive(mbu8 recvalue);
void mbSerialSend(mbu8 *senddata, mbu16 length);

#ifdef __cplusplus
    } //extern "C" {
#endif

#endif /* end of include guard: MBSERIAL_H */

