1.�����51��Ƭ����Ҫ�ڱ������ж����DEVICE_C51�������豸��ò�Ҫ��������ꡣ

2.����Ǵ���ֽ���Ҫ�ڱ������ж����BIG_ENDIAN����������Ĭ����С���ֽ���

3.���Ҫ����ModbusRTU,��Ҫ�ڱ������ж����MB_MODBUS_RTU�����Ҫ����ModbusASCII
��Ҫ�ڱ������ж����MB_MODBUS_ASCII�����Ҫ����ModbusTCP,��Ҫ�ڱ������ж����
MB_MODBUS_TCP��

4.����Ǵ���ͨ�ţ��û�Ҫ�ṩһ����ʱ���������ModbusRTU��ʱ���������һ��
RTU���ĵĽ�������ʱʱ��Ϊ3.5T�������ModbusASCII��ʱ���������ͨ�ų�ʱ����
��ʱ�����жϺ����е���mbOnTimer()������

5.����Ǵ���ͨ���û�Ҫʵ������������
    ��1��mbDisableTimer(������������������йرն�ʱ����
    ��2��mbEnableTimer(������������������д򿪶�ʱ����
    ��3��mbSerialSend(unsigned char *sendData, size_t length)����������������а�����
         ͨ�����ڷ��ͳ�ȥ��
 ���������ͨ����ֻ��Ҫʵ��mbSerialSend(unsigned char *sendData, size_t length)������

6.���ڻ������ڽ��յ����ݺ�Ҫ�����ݴ���mbSerialReceive(unsigned char recValue)������