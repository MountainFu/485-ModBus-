#include "STC15F2K60S2.h"
#include<intrins.h>

#include "delay.h"
#include "modbus.h"
#include "E2PROM.h"
#include "IIC.h"
char TimeInterval;
char data DI[6]={1,2,3,4,5,6}; 
char data DO[6]={6,7,8,9,10,11}; 
char data AI[6]={11,12,13,14,15,16};
char data AO[6]={16,17,18,19,20,21};    
unsigned char data ReceiveData[14];
unsigned char data  countnumber;

char MyAddress;
unsigned char Weight;
typedef unsigned char BYTE;
typedef unsigned int WORD;

#define FOSC 18432000L          //ϵͳƵ��
#define BAUD 9600             //���ڲ�����

#define S1_S0 0x40              //P_SW1.6
#define S1_S1 0x80              //P_SW1.7

/*---------------------------------------------------------------------------
����˵����CRC ��λ�ֽ�ֵ��
---------------------------------------------------------------------------*/
static unsigned char code auchCRCHi[] = {
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
                                                              
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
} ;
/*---------------------------------------------------------------------------
����˵����CRC��λ�ֽ�ֵ��
---------------------------------------------------------------------------*/
static unsigned char code auchCRCLo[] = {
0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,
0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,
0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,
0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,
0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,
0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,
0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,
0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,
0x43, 0x83, 0x41, 0x81, 0x80, 0x40
};
/*---------------------------------------------------------------------------
���÷�ʽ��unsigned int CRC16(unsigned char *puchMsg, unsigned int usDataLen)
����˵����CRCУ��
---------------------------------------------------------------------------*/
unsigned int CRC16(unsigned char *puchMsg, unsigned int usDataLen)
{
  unsigned char uchCRCHi = 0xFF ;    // ��CRC�ֽڳ�ʼ�� 
  unsigned char uchCRCLo = 0xFF ;    // ��CRC �ֽڳ�ʼ��
  unsigned uIndex ;                  // CRCѭ���е����� 
  while (usDataLen--)                // ������Ϣ������  
  {
    uIndex = uchCRCHi ^ *puchMsg++ ; // ����CRC         
    uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex] ;
    uchCRCLo = auchCRCLo[uIndex] ;
  }
  return (uchCRCHi << 8 | uchCRCLo) ;
}
/*---------------------------------------------------------------------------
���÷�ʽ��unsigned char getbit(unsigned int  address,unsigned char  function)
����˵����ȡ��������ַ��λֵ
---------------------------------------------------------------------------*/
unsigned char getbit(unsigned int  address,unsigned char  function)
{
  unsigned char data Offset;
  unsigned char data temp;
  unsigned char data *Myaddress;  
  Offset=(char)address&0x07;
  switch (function)                 //���ݹ����벻ͬ������Ӧ����
  {
   case 2:
           Myaddress=DI;
           break;
   case 1:
           Myaddress=DO;             
           break;
   default:
           return 2;
  }
  temp=(char)(address>>3);
  temp=*(Myaddress+temp);
  temp>>=(Offset);
  if (temp&0x01)
    return 1;
  else
    return 0;
}
/*---------------------------------------------------------------------------
���÷�ʽ��void SendData(unsigned char  *output,unsigned char  Outlength)
����˵������������������
---------------------------------------------------------------------------*/
void SendData(unsigned char  *output,unsigned char  Outlength)
{  
  ES=0;
  while(Outlength--)
  {  
    TI=0;
    ACC=*output;
    TB8=P;
    SBUF=*(output++);
    while(!TI);
    TI=0;   
  }  
  ES=1;
}
/*---------------------------------------------------------------------------
���÷�ʽ��void Function12(unsigned char address,unsigned char len)
����˵����������1��2����
---------------------------------------------------------------------------*/
void Function12(unsigned char address,unsigned char len)
{
	unsigned int data i;
	unsigned char data j;
	unsigned char data length;
	unsigned char data *send;
	unsigned int data temp;
	unsigned char data function;

	if((address==0x01)&&(len==0x02))	//���̵���1
	{
		Exdata=read_add(0x01); //��ʼI2C�����ж�����
		delay_ms(20);
		if(Exdata==0x20)	   //������յ��������ݹرռ̵���
		{
			led1=1;
			Exflag=0;
		}
		else
		{
			led1=0;
			Exflag=1;
		}
	}
	if((address==0x02)&&(len==0x01))	//�ؼ̵���1
	{
		led1=1;
		Exflag=0;	
	}
	if((address==0x03)&&(len==0x02))	//���̵���2
	{
		Exdata=read_add(0x01); //��ʼI2C�����ж�����
		delay_ms(20);
		if(Exdata==0x20)	   //������յ��������ݹرռ̵���
		{
			led2=1;
			Exflag=0;
		}
		else
		{
			led2=0;
			Exflag=1;
		}

	}
	if((address==0x04)&&(len==0x01))	//�ؼ̵���1
	{
		led2=1;
		Exflag=0;	
	}

	if((address==0x05)&&(len==0x02))   //ͬʱ���̵���1��2
	{
		Exdata=read_add(0x01); //��ʼI2C�����ж�����
		delay_ms(20);
		if(Exdata==0x20)	   //������յ��������ݹرռ̵���
		{
			led1=1;
			led2=1;
			Exflag=0;
		}
		else
		{
			led1=0;
			led2=0;
			Exflag=1;
		}
		
	}

	if((address==0x06)&&(len==0x01))   //ͬʱ�ؼ̵���1��2
	{
		led1=1;
		led2=1;
		Exflag=0;	
	}

	length=0;
	send=ReceiveData;
	function=send[1];
	for (i=1;i<=len;i++)
	{
		length++;
		*(send+2+length)=0;
		for (j=0;j<8;j++)
		{
		  *(send+2+length)=*(send+2+length)|getbit(address,function)<<j;
		  address++;
		  i++;
		if(i>len)break;
	}
	i--;
	}
	*(send+2)=length;                        //���ݳ���
	temp=CRC16(send,length+3);               //DI״̬����
	*(send+3+length)=(char)(temp>>8);        //CRCУ���
	*(send+4+length)=(char)temp;             //CRCУ���
	SendData(send,length+5);                 //����A���ͳ���
}
/*---------------------------------------------------------------------------
���÷�ʽ��void Function3(unsigned char  address,unsigned char  len)
����˵����������3����-----��ȡ�Ĵ���
---------------------------------------------------------------------------*/
void Function3(unsigned char  address,unsigned char  len)
{
	unsigned char data i;
	unsigned char data *send;
	unsigned int data temp;	

	Weight=read_add(0x03); 					//��ʼI2C�ɼ�����
	IapEraseSector(0x0001);					
	delay_ms(10);
	IapEraseSector(0x0000);					
	delay_ms(10);
	IapProgramByte(0x0000,0x00);
	delay_ms(10);
	IapProgramByte(0x0001,Weight);			//����������д��EEPROM
	delay_ms(10);

	send=ReceiveData;
	*(send+2)=2*len;                         //���ݳ��ȸ�
	address=2*address;

  for(i=0;i<len;i++)                       //ȡ����Ĵ�������
  {
//    *(send+3+2*i)=AO[address++];
//    *(send+4+2*i)=AO[address++];

	*(send+3+2*i)=IapReadByte(address++);
    *(send+4+2*i)=IapReadByte(address++);	 //����ָ���ȡEEPROM����������
  }
  temp=CRC16(send,2*len+3);                //CRCУ��
  *(send+3+2*len)=(char)(temp>>8);
  *(send+4+2*len)=(char)temp;
  SendData(send,2*len+5);                  //����A���ͳ���
}
/*---------------------------------------------------------------------------
���÷�ʽ��void SendError(char ErrorCode)
����˵����
---------------------------------------------------------------------------*/
void SendError(char ErrorCode)
{
  unsigned char data *send;
  unsigned int data temp;
  send=ReceiveData;
  *(send+1)=ReceiveData[1]|0x01;//���λ��1
  *(send+2)=ErrorCode;
  temp=CRC16(send,3);
  *(send+3)=(char)(temp>>8);
  *(send+4)=(char)temp;
  SendData(send,countnumber);              //���÷��ͳ���,���ش������
}
/*---------------------------------------------------------------------------
���÷�ʽ��void Deal()
����˵�����������ݴ���
---------------------------------------------------------------------------*/
void Deal()
{
  unsigned int data temp;
  unsigned char data address;
  unsigned char data length;
  unsigned char data counter;
  unsigned char data *Pointer;
  Pointer=ReceiveData;
  counter=countnumber;
  if(MyAddress==Pointer[0])  //	�Ƿ��뱾��վ����ͬ
  {
	  if (counter<=3 ) return;
	  temp=Pointer[counter-2]<<8;   
	  temp= temp|Pointer[counter-1];
	  if( temp==CRC16(Pointer,counter-2))      //CRC������ȷ
	  { 
	    address=Pointer[3];
	    length=Pointer[5];
	    if(address<48)
	    {
	      switch (Pointer[1])                  //���ݹ����벻ͬ������Ӧ����*/
	   {
	    case 1:
	            Function12(address,length);
	            break;
	    case 2:
	            Function12(address,length);
	            break;
	    case 3:
				if(length>9)
				break;
				else
				{
	            	address=address-1;
	            	Function3(address,length);
				}
	            break;
	    default:
	               SendError(0x81);
	               break;
	      }
	    }
	    else
	    { SendError(0x82);}
	  }
	}
  countnumber=0;
}

/*---------------------------------------------------------------------------
���÷�ʽ��void Init_timer2(unsigned int baudrate)
����˵������ʱ��2�ĳ�ʼ��
---------------------------------------------------------------------------*/
void Init_timer2()
{
  	TL0  = 0xff;
	TH0  = 0x48;
	EA=1;
	ET0=0;
	TR0=1;
}
/*---------------------------------------------------------------------------
���÷�ʽ��void Init_SerialPort(unsigned int baudrate)
����˵�������ڳ�ʼ��
---------------------------------------------------------------------------*/
void Init_SerialPort(unsigned int baudrate)
{
    ACC = P_SW1;
    ACC &= ~(S1_S0 | S1_S1);    //S1_S0=0 S1_S1=0
    P_SW1 = ACC;                //(P3.0/RxD, P3.1/TxD)
 
    SCON = 0x50;                //8λ�ɱ䲨����
    T2L = (65536 - (FOSC/4/BAUD));   //���ò�������װֵ
    T2H = (65536 - (FOSC/4/BAUD))>>8;
    AUXR = 0x14;                //T2Ϊ1Tģʽ, ��������ʱ��2
    AUXR |= 0x01;               //ѡ��ʱ��2Ϊ����1�Ĳ����ʷ�����
    ES = 1;                     //ʹ�ܴ���1�ж�
    EA = 1;	
}

/*---------------------------------------------------------------------------
���÷�ʽ��void SeiralA() interrupt 4 using 0
����˵����ͨѶ�ж�
---------------------------------------------------------------------------*/
void SeiralA() interrupt 4 using 0
{
  if(RI) 
  {
    if((TimeInterval>2)&&(TimeInterval<=4)) 
    { countnumber=0;}
    if(countnumber>20) 
    { countnumber=0;}                     
    ReceiveData[countnumber++]=SBUF;   //�����յ������ݵ���������
    TimeInterval=0;
    RI=0;
    ET0=1;                                //�����յ����ݺ�,����ʱ��0����
  }                
}

/*---------------------------------------------------------------------------
���÷�ʽ��void SerialPortTime() interrupt 1 using 0
����˵������ʱ��0�жϳ���
---------------------------------------------------------------------------*/
void SerialPortTime() interrupt 1 using 0
{
  	TL0  = 0xff;
	TH0  = 0x48;
  ET0=0;                                  //��ʱ��0�ж��������                   
  TimeInterval=TimeInterval+1;
  if((TimeInterval>4) && countnumber) 
  {
    Recenable=1;           //����
     Deal();                             //�����յ������ݽ��д���
     Recenable=0;        //����
    countnumber=0;
  }
  else
  { ET0=1;}                               //����ʱ��0��
  TF0=0;          //��ʱ��0�����־,�����0
}