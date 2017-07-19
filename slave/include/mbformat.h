/*
********************************************************************************
*                              COPYRIGHT NOTICE
*                             Copyright (c) 2016
*                             All rights reserved
*
*  @FileName       : mbformat.h
*  @Author         : scm 351721714@qq.com
*  @Create         : 2016/11/20 13:28:40
*  @Last Modified  : 2017/01/25 16:17:31
********************************************************************************
*/
#ifndef MBFORMAT_H
#define MBFORMAT_H

#include "mbconfig.h"

#ifdef __cplusplus
extern "C" {
#endif

struct _mbFormat_t;
typedef void (* mbFormatHandle_t)(struct _mbFormat_t *);
extern mbFormatHandle_t mbFunArray[];

/* typedef int  (* mbCheckChksumHandle_t)(mbu8 *, mbu16); */
typedef void (* mbAppendCheckSumHandle_t)(mbu8 *, mbu16);
typedef void (* mbSendHandle_t)(mbu8 *, mbu16);
typedef void (* mbClearHandle_t)(void);

typedef struct _mbFormat_t {
    mbu8 *frame;
    mbu16 frameLength;
    mbu8 *sendBuff;
    mbu16 buffCapacity;
    mbu8 chksumLength;
    /* mbCheckChksumHandle_t checkChksumHandle; */
    mbAppendCheckSumHandle_t appendChksumHandle;
    mbSendHandle_t sendHandle;
    mbClearHandle_t clearHandle;
} mbFormat_t;

int mb_add_function(mbu8 funcode, mbFormatHandle_t handle);

void mbformat01(mbFormat_t *format);
void mbformat02(mbFormat_t *format);
void mbformat03(mbFormat_t *format);
void mbformat04(mbFormat_t *format);
void mbformat05(mbFormat_t *format);
void mbformat06(mbFormat_t *format);
void mbformat16(mbFormat_t *format);

#ifdef __cplusplus
    } //extern "C" {
#endif

#endif /* end of include guard: MBFORMAT_H */
