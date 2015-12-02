//#include <REGX52.H>
#include "STC15F2K60S2.h"
#include "IIC.h"
#include "delay.h"
#include "modbus.h"
#include "E2PROM.h"

void main()
{
	i2cinit();
	delay_ms(20);

	write_add(0x01,0x20);
	delay_ms(20);
	write_add(0x03,110);
	delay_ms(20);

	Init_SerialPort(9600);
	Init_timer2();
	EA=1;                                   //�����ж�
	Recenable=0;                            //485��������

	MyAddress=IapReadByte(0x1300);			//��EEPROM��ַ0x1300��ȡվ��
	delay_ms(50);
//	SBUF=MyAddress;
//	delay_ms(50);

	while(1)
	{
		if(Exflag==1)
		{
			Exdata=read_add(0x01); //��ʼI2C�����ж�����
			delay_ms(20);
			if(Exdata==0x20)	   //������յ���������
			{
				led1=1;
				led2=1;			   //�ؼ̵���
				Exflag=0;		   //����I2Cͨ�ű�־λ
				read_add(0x02);    //����I2Cͨ��
			}
		}
	}

}