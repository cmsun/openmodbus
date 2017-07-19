/*
********************************************************************************
*                              COPYRIGHT NOTICE
*                             Copyright (c) 2016
*                             All rights reserved
*
*  @FileName       : mbconfig.h
*  @Author         : scm 351721714@qq.com
*  @Create         : 2016/11/20 13:31:49
*  @Last Modified  : 2017/01/25 16:20:40
********************************************************************************
*/
#ifndef MBCONFIGH
#define MBCONFIGH

/*
 * 如果是51单片机请在编译器中设置宏定义DEVICE_C51，其它设备不要设置这个宏
 * #define MB_DEVICE_C51                 
 * 
 * 如果为大端字节序则在编译器中定义这个宏，否则小端字节序不要定义
 * (keil c51是大端，stm32为小端，arm为小端)
 * #define MB_BIG_ENDIAN                
 */

#ifdef MB_DEVICE_C51
typedef unsigned long mbu32;
typedef unsigned int mbu16;
typedef unsigned char mbu8;
typedef signed long mbs32;
typedef signed int mbs16;
typedef signed char mbs8;
#else
#include <stdint.h>
typedef uint32_t mbu32;
typedef uint16_t mbu16;
typedef uint8_t mbu8;
typedef int32_t mbs32;
typedef int16_t mbs16;
typedef int8_t mbs8;
#endif

#ifdef MB_DEVICE_C51
    /*
     * C51声明可重入函数。
     * 当C51调用参数超过3个以上的函数指针时，也要声明reentrant，
     * 目的是为了使C51的参数以入栈出栈的方式传递，而不是以寄存器传递。
     */
    #define REENTRANT reentrant 
#else
    #define REENTRANT
#endif

/*----------------------------------------------------------------------------*/

#if !defined(MB_MODBUS_RTU) && !defined(MB_MODBUS_ASCII) && !defined(MODBUS_TCP)
    #error  "如果是ModbusRTU则定义MB_MODBUS_RTU宏"
    #error  "如果是ModbusASCII则定义MB_MODBUS_ASCII宏"
    #error  "如果是ModbusTCP则定义MODBUS_TPC宏"
#endif

#if defined(MB_MODBUS_RTU) && defined(MB_MODBUS_ASCII)
    #error "modbusRTU 和 modbusASCII 不能同时使用！"
#endif

/*----------------------------------------------------------------------------*/

#define MB_SLAVE_DEVID  1   //下位机设备地址
#if MB_SLAVE_DEVID == 0
    #error "设备地址0是广播地址！"
#endif

#define MB_FUNCTION_FIELD          127  //modbus功能码范围。(0-127是可以使用的功能码，127以上的异常码。)

#define MB_RECEIVE_BUFFER_LENGTH    64  //数据接收缓冲区大小
#define MB_SEND_BUFFER_LENGTH       64  //数据发送缓冲区大小

#define MB_COIL_COUNT               32  //逻辑线圈数量（一个逻辑线圈占一个bit）
#define MB_DIGITAL_COUNT            32  //数字量输入数量（一个数字量输入占一个bit）
#define MB_REGISTER_COUNT           32  //保持型寄存器数量（一个保持型寄存器占一个unsgined short大小）
#define MB_ANALOGIN_COUNT           3   //模拟量输入寄存器数量（一个模拟量输入寄存器为两个unsigned short大小）

#if MB_RECEIVE_BUFFER_LENGTH < 11 || MB_SEND_BUFFER_LENGTH < 8
    #error "接收缓存空间至少11个字节，发送缓存空间到少8个字节！"
#endif

#endif
