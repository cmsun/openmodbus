/*
********************************************************************************
*                              COPYRIGHT NOTICE
*                             Copyright (c) 2016
*                             All rights reserved
*
*  @FileName       : mbchecksum.h
*  @Author         : scm 351721714@qq.com
*  @Create         : 2016/11/20 13:31:17
*  @Last Modified  : 2017/01/25 16:14:12
********************************************************************************
*/
#ifndef MBCHECKSUMH
#define MBCHECKSUMH

#include "mbconfig.h"

#ifdef __cplusplus
extern "C" {
#endif

mbu16 mbComputeCRC(mbu8 *pdata, mbu16 length);
mbu8 mbComputeLRC(mbu8 *pdata, mbu16 length);

#ifdef __cplusplus
    } //extern "C" {
#endif

#endif
