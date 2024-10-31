#include "stm32f4xx.h"
#include "beep.h"

GPIO_InitTypeDef gpiobeep;	

void beep_init(){
	//beep时钟使能
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	//结构体赋值
	
	gpiobeep.GPIO_Mode=GPIO_Mode_OUT;//输出模式
	gpiobeep.GPIO_OType=GPIO_OType_PP;//推挽输出
	gpiobeep.GPIO_Pin=GPIO_Pin_10;//接口
	gpiobeep.GPIO_Speed=GPIO_Speed_2MHz;//频率
	
	GPIO_Init(GPIOB,&gpiobeep);
	
}
void beep_out(){
	GPIO_WriteBit(GPIOB,GPIO_Pin_10,1);//写1
}

void beep_stop(){
	GPIO_WriteBit(GPIOB,GPIO_Pin_10,0);//写0
}
