#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "STC12C5A60S2.h"
#include "mbConfig.h"
#include "mbSerial.h"
#include "mbData.h"

/*����ʱҪѡ��
 *�洢ģʽ��Large:������XDATA
 *����ROM��С��64K���򣨻��ߴ���ROM��С��Compact: 2K������64K����
 */

//���ڴ�ӡ����
//�����ӡ����Ҫǿת��int�ͣ���uprintf("%d\n", (int)5);
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

//������ʱ
void delay_ms(unsigned int ms)
{
	unsigned short i;
	unsigned char j, k;	
	for(i = 0; i < ms; ++i)
		for(j = 0; j < 9; ++j)
			for(k = 0; k < 254; ++k);
}

//-----------------------------------------------
// mbTimerDisable��mbTimerEnable��mbSend ��������������OpenModbus����õ�δʵ�ֵĺ���
void mbDisableTimer(void)
{
	ET0 = 0;	
	TR0 = 0;
}

void mbEnableTimer(void)
{
	TH0=(65536-11111)/256;  //��װT0
 	TL0=(65536-11111)%256;  //T0��λ
	ET0 = 1;	
	TR0 = 1;
}

void mbSerialSend(mbu8 *sendData, size_t length)
{													
	int i;
	ES = 0;					//�رմ����ж�
	for(i = 0; i < length; ++i)
	{
		SBUF = sendData[i];
		while(!TI)
			;
		TI = 0;
	}
	ES = 1;					//�򿪴����ж�
}

//Ҫ�ڴ����жϺͶ�ʱ���ж��е���mbSerialReceive������mbOnTimer����

void ComRec() interrupt 4				//�����жϺ����������жϵ������4
{
	if(RI == 1)
	{
		RI = 0;							//���������ж�ʱRI��Ӳ����1���ڴ����жϷ����������Ҫ�������0��
		mbSerialReceive(SBUF);			    //�����ж��е���OpenModbus�е�mbSerialReceive����
	}
} 


//modbus rtu ��3.5��T��ʱ��˵��һ��rtu���������ɣ�����OpenModubs��mbOnTimer����
void onTimer() interrupt 1				//��ʱ��T0�жϷ������
{
 	static unsigned int i = 0;

 	TH0=(65536-11111)/256;				//��װT0
 	TL0=(65536-11111)%256;				//T0��λ

	if(!(i = ++i%4))
		mbOnTimer();		
}

 //��ʼ�����ںͶ�ʱ��
 void init()
{
	//115200�����ʣ�18.432MHz����
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
 
	REN=1;      //���д��ڽ������ݣ�REN=1�����ڽ��գ�REN=0��ֹ���ڽ�������
	SM0=0;      //���ô��ڹ�����ʽΪ1����10λ�첽ͨ�ţ�1��ʼλ��8����λ��1ֹͣλ
	SM1=1;      //ͬ�ϣ�REN��SM0��SM1ͬ����SCON�Ĵ���������SCON�Ĵ�������λѰַ
	EA=1;       //�����ж�
	ES=1;       //�������ж�

 	TH0=(65536-11111)/256;	//װT0��λ��ֵ
 	TL0=(65536-11111)%256;	//T0��λ��ֵ
	ET0=1;					//����ʱ���ж�
	TR0=0;					//������T0��ʱ��
	
	P1=0xff;            	//��λ�������                   
}

//������
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






