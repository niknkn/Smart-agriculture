#include "stm32f4xx.h"
#include "beep.h"

GPIO_InitTypeDef gpiobeep;	

void beep_init(){
	//beepʱ��ʹ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	//�ṹ�帳ֵ
	
	gpiobeep.GPIO_Mode=GPIO_Mode_OUT;//���ģʽ
	gpiobeep.GPIO_OType=GPIO_OType_PP;//�������
	gpiobeep.GPIO_Pin=GPIO_Pin_10;//�ӿ�
	gpiobeep.GPIO_Speed=GPIO_Speed_2MHz;//Ƶ��
	
	GPIO_Init(GPIOB,&gpiobeep);
	
}
void beep_out(){
	GPIO_WriteBit(GPIOB,GPIO_Pin_10,1);//д1
}

void beep_stop(){
	GPIO_WriteBit(GPIOB,GPIO_Pin_10,0);//д0
}
