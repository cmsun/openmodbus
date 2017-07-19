#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "STC12C5A60S2.h"
#include "mbConfig.h"
#include "mbSerial.h"
#include "mbData.h"

/*编译时要选择：
 *存储模式：Large:变量在XDATA
 *代码ROM大小：64K程序（或者代码ROM大小：Compact: 2K函数，64K程序）
 */

//串口打印函数
//如果打印数字要强转成int型，如uprintf("%d\n", (int)5);
void uprintf(const char *format, ...)	
{
	char len, i;
	char buff[64];
	va_list pfmt;

	va_start(pfmt, format);
	vsprintf(buff, format, pfmt);
	buff[63] = '\0';
   	len = strlen(buff);
	ES = 0;
	for(i = 0; i <= len; ++i)
	{
		SBUF = buff[i];
		while(!TI)
			;
		TI = 0;
	}
	ES = 1;	
}

//毫秒延时
void delay_ms(unsigned int ms)
{
	unsigned short i;
	unsigned char j, k;	
	for(i = 0; i < ms; ++i)
		for(j = 0; j < 9; ++j)
			for(k = 0; k < 254; ++k);
}

//-----------------------------------------------
// mbTimerDisable、mbTimerEnable、mbSend 这三个函数是在OpenModbus里调用但未实现的函数
void mbDisableTimer(void)
{
	ET0 = 0;	
	TR0 = 0;
}

void mbEnableTimer(void)
{
	TH0=(65536-11111)/256;  //重装T0
 	TL0=(65536-11111)%256;  //T0低位
	ET0 = 1;	
	TR0 = 1;
}

void mbSerialSend(mbu8 *sendData, size_t length)
{													
	int i;
	ES = 0;					//关闭串口中断
	for(i = 0; i < length; ++i)
	{
		SBUF = sendData[i];
		while(!TI)
			;
		TI = 0;
	}
	ES = 1;					//打开串口中断
}

//要在串口中断和定时器中断中调用mbSerialReceive函数和mbOnTimer函数

void ComRec() interrupt 4				//串口中断函数，串口中断的序号是4
{
	if(RI == 1)
	{
		RI = 0;							//产生串口中断时RI被硬件置1，在串口中断服务程序中需要用软件清0；
		mbSerialReceive(SBUF);			    //串口中断中调用OpenModbus中的mbSerialReceive函数
	}
} 


//modbus rtu 的3.5个T超时后，说明一个rtu报告接收完成，调用OpenModubs的mbOnTimer函数
void onTimer() interrupt 1				//定时器T0中断服务程序
{
 	static unsigned int i = 0;

 	TH0=(65536-11111)/256;				//重装T0
 	TL0=(65536-11111)%256;				//T0低位

	if(!(i = ++i%4))
		mbOnTimer();		
}

 //初始化串口和定时器
 void init()
{
	//115200波特率，18.432MHz晶振
	PCON &= 0x7F;		
	SCON = 0x50;		
	AUXR |= 0x04;	
	BRT = 0xFB;		
	AUXR |= 0x01;		
	AUXR |= 0x10;
	
	//19200bps@18.432MHz
	/*PCON &= 0x7F;		
	SCON = 0x50;		
	AUXR |= 0x04;		
	BRT = 0xE2;	
	AUXR |= 0x01;		
	AUXR |= 0x10;*/							
 
	REN=1;      //运行串口接收数据，REN=1允许串口接收，REN=0禁止串口接收数据
	SM0=0;      //设置串口工作方式为1，即10位异步通信，1起始位，8数据位，1停止位
	SM1=1;      //同上，REN、SM0、SM1同属于SCON寄存器，但是SCON寄存器允许位寻址
	EA=1;       //开总中断
	ES=1;       //开串口中断

 	TH0=(65536-11111)/256;	//装T0高位初值
 	TL0=(65536-11111)%256;	//T0低位初值
	ET0=1;					//开定时器中断
	TR0=0;					//不启动T0定时器
	
	P1=0xff;            	//复位所有输出                   
}

//主函数
void main()
{
	static char i = 0;
	unsigned char h = 0, m = 0, s = 0;

	init();

	for(i = 0; i < (MB_DIGITAL_COUNT+7)/8; ++i)
		mbDigitIn[i] = i+1;

	for(i = 0; i < MB_ANALOGIN_COUNT; ++i)
		((float *)mbAnalogIn)[i] = i + 0.123;
                   
 	while(1)                 
	{
		//P0 = ~P0;

		delay_ms(5000);
		delay_ms(500);
		if(!(i = ++i%5))
		{
			if((m %= 60) == 59 && s == 55)
				h += 1;
			s += 5;
			if(!(s %= 60))
				m += 1;

			uprintf("Program Running: %2d:%2d:%2d\n", (int)h, (int)m, (int)s);
		}
	}
}






