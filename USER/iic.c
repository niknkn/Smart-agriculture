#include "stm32f4xx.h"
#include "iic.h"
#include "delay.h"
#include "stdio.h"
void iic_init(){
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef iic;
	iic.GPIO_Mode=GPIO_Mode_OUT;
	iic.GPIO_OType=GPIO_OType_OD;//��©
	iic.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7;
	iic.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOB,&iic);
}


void SDA_out(){
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef sdaout;
	sdaout.GPIO_Mode=GPIO_Mode_OUT;
	sdaout.GPIO_OType=GPIO_OType_OD;//��©
	sdaout.GPIO_Pin=IIC_SDA_PIN;
	sdaout.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOB,&sdaout);
}


void SDA_in(){
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef sdain;
	sdain.GPIO_Mode=GPIO_Mode_IN;
	sdain.GPIO_PuPd=GPIO_PuPd_UP;//����
	sdain.GPIO_Pin=IIC_SDA_PIN;
	sdain.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOB,&sdain);
}


//��ʼ�ź�
void start_signal(){
	SDA_out();// SAD���ģʽ
	SCL_Write(1);
	SDA_Write(1);
	delay_us(2);
	SDA_Write(0);
	delay_us(2);
	SCL_Write(0);
}


//ֹͣ�ź�
void stop_signal(){
	SDA_out();// SAD���ģʽ
	SCL_Write(0);
	SDA_Write(0);
	delay_us(2);
	SCL_Write(1);
	delay_us(2);
	SDA_Write(1);
}


//�ȴ��ӻ�Ӧ��0Ӧ��1��Ӧ��
u8 IIC_get_Ask(void){
	//SCL ����-------������Ч
	SCL_Write(0);
	SDA_in();//����ģʽ
	SCL_Write(1);
	delay_us(5);
	
	if(GPIO_ReadInputDataBit(IIC_PORT,IIC_SDA_PIN)==RESET){
		//Ӧ��
		SCL_Write(0);
		delay_us(5);
		return 0;
	}
	return 1;	
}


//����Ӧ��
void IIC_send_Ask(u8 ask){
	//SCL����-----������Ч
	SCL_Write(0);
	//���ģʽ
	SDA_out();
	SCL_Write(0);
	delay_us(5);
	//SDA׼������
	if(ask){
		SDA_Write(1);
	}else{
		SDA_Write(0);
	}
	//����SCL-----��������
	SCL_Write(1);
	delay_us(5);
	//����
	SCL_Write(0);
	delay_us(5);
}


//��������
void IIC_send_Byte(u8 data){
	int i;
	SCL_Write(0);
	SDA_out();
	SCL_Write(0);
	delay_us(2);
	//��������
	for(int i=7;i>=0;i--){
		if(data & (0x01<<i)){
			SDA_Write(1);
		}else{
			SDA_Write(0);
		}
		delay_us(2);
		SCL_Write(1);
		delay_us(2);
		SCL_Write(0);
		delay_us(2);
	}
}


//IIC���ֽ�����
u8 IIC_read_Byte(void){
	u8 data = 0;
	SCL_Write(0);
	SDA_in();
	SCL_Write(0);
	delay_us(2);
	//������
	for(int i=7;i>=0;i--)
	{
		SCL_Write(1);
		delay_us(2);
		if(GPIO_ReadInputDataBit(IIC_PORT,IIC_SDA_PIN) == SET)
		{
			data |= 1<<i;
		}
		SCL_Write(0);
		delay_us(2);
	}
	SCL_Write(1);
	delay_us(2);
	//���ض���������
	return data;
}


//��AT24C02ָ����ַд��һ���ֽ�����
void IIC_AT24C02_write_data(u8 addr,u8 data){
	//1.��ʼ�ź�
	start_signal();
	//2.���ʹ�������ַ(д��ʽ)
	IIC_send_Byte(0xA0);
	//3.�ȴ���Ӧ
	if(IIC_get_Ask()) //����1��δӦ��
	{
		printf("Device addr no ask\r\n");
		stop_signal();
		return ;
	}
	//4.д�ֵ�ַ
	IIC_send_Byte(addr);
	//5.�ȴ�Ӧ��
	if(IIC_get_Ask()) // 1 δӦ��
	{
		printf("Word addr no ask\r\n");
		stop_signal();
		return ;
	}
	//6.д����
	IIC_send_Byte(data);
	//7.�ȴ�Ӧ��
	if(IIC_get_Ask()) // 1 δӦ��
	{
		printf("Data no ask\r\n");
		stop_signal();
		return ;
	}
	//8.ֹͣ�ź�
	stop_signal();
	//���
	delay_ms(20);
}

//�����
u8 AT24C02_read_data(u8 addr){
	//1.��ʼ�ź�
	start_signal();
	//2.���ʹ�������ַ д
	IIC_send_Byte(0xA0);
	//3.�ȴ�Ӧ��
	if(IIC_get_Ask()) // 1 δӦ��
	{
		printf("Read device addr no ask\r\n");
		stop_signal();
	}
	//4.�����ֵ�ַ
	IIC_send_Byte(addr);
	//5.�ȴ�Ӧ��
	if(IIC_get_Ask()) // 1 δӦ��
	{
		printf("Read word addr no ask\r\n");
		stop_signal();
	}
	//6.��ʼ�ź�
	start_signal();
	//7.���ʹ�������ַ ��
	IIC_send_Byte(0xA1);
	//8.�ȴ�Ӧ��
	if(IIC_get_Ask()) // 1 δӦ��
	{
		printf("Read device_2 addrno ask\r\n");
		stop_signal();
	}
	//9.������
	u8 data = IIC_read_Byte();
	//10.�������� ��Ӧ��
	IIC_send_Ask(1);
	//11.ֹͣ�ź�
	stop_signal();
	return data;
}

