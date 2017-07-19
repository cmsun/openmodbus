1.如果是51单片机，要在编译器中定义宏DEVICE_C51。其它设备最好不要定义这个宏。

2.如果是大端字节序，要在编译器中定义宏BIG_ENDIAN，不定义则默认是小端字节序。

3.如果要编译ModbusRTU,则要在编译器中定义宏MB_MODBUS_RTU。如果要编译ModbusASCII
则要在编译器中定义宏MB_MODBUS_ASCII。如果要编译ModbusTCP,则要在编译器中定义宏
MB_MODBUS_TCP。

4.如果是串口通信，用户要提供一个定时器。如果是ModbusRTU则定时器用来检测一个
RTU报文的结束，定时时间为3.5T。如果是ModbusASCII则定时器用来检测通信超时。在
定时器的中断函数中调用mbOnTimer()函数。

5.如果是串口通信用户要实现三个函数：
    （1）mbDisableTimer(）函数。在这个函数中关闭定时器。
    （2）mbEnableTimer(）函数。在这个函数中打开定时器。
    （3）mbSerialSend(unsigned char *sendData, size_t length)函数。在这个函数中把数据
         通过串口发送出去。
 如果是网口通信则只需要实现mbSerialSend(unsigned char *sendData, size_t length)函数。

6.串口或者网口接收到数据后，要把数据传给mbSerialReceive(unsigned char recValue)函数。