#include "STC15F2K60S2.h"
#include "IIC.h"
#include "delay.h"
#include "modbus.h"
unsigned char Exflag;
unsigned char Exdata;

void i2cinit()//���߳�ʼ��
{
 SDA = 1;
 delay();
 SCL = 1;
 delay();
}
void start()//�����ź�
{
 SDA = 1;
 SCL = 1;
 delay();
 SDA = 0;
 delay();
}
void stop()//ֹͣ�ź�
{
 SDA = 0;
 delay();
 SCL = 1;
 delay();
 SDA = 1;
 delay();
}
void respons()//Ӧ���ź�
{
 unsigned char i = 0;
 SCL = 1;
 delay();
 while(SDA == 1 && i < 255)//�ȴ�Ӧ�𣬹�һ��ʱ�䲻Ӧ���˳�ѭ��
  i++;
 SCL = 0;
 delay();
}
void writebyte(unsigned char date)//дһ���ֽ�
{
 unsigned char i,temp;
 temp = date;
 for(i = 0; i < 8; i++)
 {
  temp <<= 1;//temp����һλ���λ��CY
  SCL = 0;
  delay();
  SDA = CY;
  delay();
  SCL = 1;
  delay();
 } 
 SCL = 0;//Ӧ���ź���SCL = 1����������Ҫ��0
 delay();
 SDA = 1;//����Ҫ�ͷ���������
 delay();
}
unsigned char readbyte()//��һ���ֽ� 
{
 unsigned char i,k;
 SCL = 0;
 delay();
 SDA = 1;
 for(i = 0; i < 8; i++)
 {
  SCL = 1; 
  delay();
  k = (k << 1) | SDA; //�����λ��һλλ�͵�K
  SCL = 0;
  delay();
 }
 delay();
 return k;
}
void write_add(unsigned char address,unsigned char date)//����ư巢������
{
 start();
 writebyte(0x01);//���ַΪ01�Ŀ��ư巢������
 respons();
 writebyte(address); //���ư��ڲ��ݴ�����ַ
 respons();
 writebyte(date);
 respons();
 stop();
}
unsigned char read_add(unsigned char address)//�����ư��ڲ��Ĵ�������
{
 unsigned char date;
 start();
 writebyte(0x01);// ��ַ01
 respons();
 writebyte(address); //���ư��ڲ��ݴ�����ַ 
 respons();
 start();
 writebyte(0x02);//��ַ+1��У��
 respons();
 date = readbyte();	//��ʼ������
 stop();			//��ȡ���ֹͣ
 return date;
}