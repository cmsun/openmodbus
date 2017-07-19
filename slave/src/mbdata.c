/*
********************************************************************************
*                              COPYRIGHT NOTICE
*                             Copyright (c) 2016
*                             All rights reserved
*
*  @FileName       : mbdata.c
*  @Author         : scm 351721714@qq.com
*  @Create         : 2016/11/20 13:30:39
*  @Last Modified  : 2016/11/21 14:38:02
********************************************************************************
*/
#include "mbconfig.h"

/*
 * modbus通信协议中规定：一个逻辑线圈或者一个继电器或者一个数字量输入占用一个位。
 * modbus通信协议中规定：所有的寄存器（保持型寄存器和模拟量输入寄存器）都是双字节的。
 */

mbu8  mbCoil    [(MB_COIL_COUNT+7)/8]       = {0};  /* 线圈和继电器(一个位对应一个逻辑线圈) */
mbu8  mbDigitIn [(MB_DIGITAL_COUNT+7)/8]    = {0};  /* 数字量输入(一个位对应一个数字量输入) */
mbu16 mbHoldReg [MB_REGISTER_COUNT]         = {0};  /* 保持型寄存器 */
mbu16 mbAnalogIn[MB_ANALOGIN_COUNT*2]       = {0};  /* 模拟量输入寄存器(模拟量为浮点数，要占2个寄存器4个字节) */

