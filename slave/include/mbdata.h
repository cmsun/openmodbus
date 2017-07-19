/*
********************************************************************************
*                              COPYRIGHT NOTICE
*                             Copyright (c) 2016
*                             All rights reserved
*
*  @FileName       : mbdata.h
*  @Author         : scm 351721714@qq.com
*  @Create         : 2016/11/20 13:31:56
*  @Last Modified  : 2016/11/21 14:36:33
********************************************************************************
*/
#ifndef MBDATAH
#define MBDATAH

#include "mbconfig.h"

#ifdef __cplusplus
    extern "C" {
#endif

extern  mbu8     mbCoil[];       /* 逻辑线圈 */
extern  mbu8     mbDigitIn[];    /* 数字量输入 */
extern  mbu16    mbHoldReg[];    /* 寄存器 */
extern  mbu16    mbAnalogIn[];   /* 模拟量输入 */

#ifdef __cplusplus
    }   /* extern "C" { */
#endif

#endif
