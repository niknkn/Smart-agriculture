#include "stm32f4xx.h"
#include "iic.h"
#include "delay.h"
#include "stdio.h"
void iic_init(){
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef iic;
	iic.GPIO_Mode=GPIO_Mode_OUT;
	iic.GPIO_OType=GPIO_OType_OD;//开漏
	iic.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7;
	iic.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOB,&iic);
}


void SDA_out(){
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef sdaout;
	sdaout.GPIO_Mode=GPIO_Mode_OUT;
	sdaout.GPIO_OType=GPIO_OType_OD;//开漏
	sdaout.GPIO_Pin=IIC_SDA_PIN;
	sdaout.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOB,&sdaout);
}


void SDA_in(){
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef sdain;
	sdain.GPIO_Mode=GPIO_Mode_IN;
	sdain.GPIO_PuPd=GPIO_PuPd_UP;//上拉
	sdain.GPIO_Pin=IIC_SDA_PIN;
	sdain.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOB,&sdain);
}


//起始信号
void start_signal(){
	SDA_out();// SAD输出模式
	SCL_Write(1);
	SDA_Write(1);
	delay_us(2);
	SDA_Write(0);
	delay_us(2);
	SCL_Write(0);
}


//停止信号
void stop_signal(){
	SDA_out();// SAD输出模式
	SCL_Write(0);
	SDA_Write(0);
	delay_us(2);
	SCL_Write(1);
	delay_us(2);
	SDA_Write(1);
}


//等待从机应答：0应答，1不应答
u8 IIC_get_Ask(void){
	//SCL 拉低-------数据无效
	SCL_Write(0);
	SDA_in();//输入模式
	SCL_Write(1);
	delay_us(5);
	
	if(GPIO_ReadInputDataBit(IIC_PORT,IIC_SDA_PIN)==RESET){
		//应答
		SCL_Write(0);
		delay_us(5);
		return 0;
	}
	return 1;	
}


//主机应答
void IIC_send_Ask(u8 ask){
	//SCL拉低-----数据无效
	SCL_Write(0);
	//输出模式
	SDA_out();
	SCL_Write(0);
	delay_us(5);
	//SDA准备数据
	if(ask){
		SDA_Write(1);
	}else{
		SDA_Write(0);
	}
	//拉高SCL-----发送数据
	SCL_Write(1);
	delay_us(5);
	//拉低
	SCL_Write(0);
	delay_us(5);
}


//发送数据
void IIC_send_Byte(u8 data){
	int i;
	SCL_Write(0);
	SDA_out();
	SCL_Write(0);
	delay_us(2);
	//发送数据
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


//IIC读字节数据
u8 IIC_read_Byte(void){
	u8 data = 0;
	SCL_Write(0);
	SDA_in();
	SCL_Write(0);
	delay_us(2);
	//读数据
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
	//返回读到的数据
	return data;
}


//向AT24C02指定地址写入一个字节数据
void IIC_AT24C02_write_data(u8 addr,u8 data){
	//1.起始信号
	start_signal();
	//2.发送从器件地址(写方式)
	IIC_send_Byte(0xA0);
	//3.等待响应
	if(IIC_get_Ask()) //返回1是未应答
	{
		printf("Device addr no ask\r\n");
		stop_signal();
		return ;
	}
	//4.写字地址
	IIC_send_Byte(addr);
	//5.等待应答
	if(IIC_get_Ask()) // 1 未应答
	{
		printf("Word addr no ask\r\n");
		stop_signal();
		return ;
	}
	//6.写数据
	IIC_send_Byte(data);
	//7.等待应答
	if(IIC_get_Ask()) // 1 未应答
	{
		printf("Data no ask\r\n");
		stop_signal();
		return ;
	}
	//8.停止信号
	stop_signal();
	//间隔
	delay_ms(20);
}

//随机读
u8 AT24C02_read_data(u8 addr){
	//1.起始信号
	start_signal();
	//2.发送从器件地址 写
	IIC_send_Byte(0xA0);
	//3.等待应答
	if(IIC_get_Ask()) // 1 未应答
	{
		printf("Read device addr no ask\r\n");
		stop_signal();
	}
	//4.发送字地址
	IIC_send_Byte(addr);
	//5.等待应答
	if(IIC_get_Ask()) // 1 未应答
	{
		printf("Read word addr no ask\r\n");
		stop_signal();
	}
	//6.起始信号
	start_signal();
	//7.发送从器件地址 读
	IIC_send_Byte(0xA1);
	//8.等待应答
	if(IIC_get_Ask()) // 1 未应答
	{
		printf("Read device_2 addrno ask\r\n");
		stop_signal();
	}
	//9.读数据
	u8 data = IIC_read_Byte();
	//10.主机发送 不应答
	IIC_send_Ask(1);
	//11.停止信号
	stop_signal();
	return data;
}

