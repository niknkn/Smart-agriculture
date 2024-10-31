#include "stm32f4xx.h"
#include "key.h"
#include "delay.h"
#include "beep.h"
#include "led.h"

#define rcc_addr 0x40023800
//AHB1���ߵĵ�ַ
#define AHB1_addr (unsigned int *) (rcc_addr+0x30)
#define GPIOC_addr 0x40020800
#define GPIOC_mode (unsigned int *)(GPIOC_addr+0x00)//ģʽ
#define GPIOC_pupdr (unsigned int *)(GPIOC_addr+0x0C)//������
#define GPIOC_speed (unsigned int *)(GPIOC_addr+0x08)//����	

void key_init(){
	//ʱ��ʹ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	
	GPIO_InitTypeDef gpiokey;
	gpiokey.GPIO_Mode=GPIO_Mode_IN;//����ģʽ	
	gpiokey.GPIO_Pin=GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;//4�ſ�
	gpiokey.GPIO_PuPd=GPIO_PuPd_UP;//��������
	gpiokey.GPIO_Speed=GPIO_Speed_2MHz;//Ƶ��
	
	GPIO_Init(GPIOE,&gpiokey);
	
}


void key_init_jcq(){
	unsigned int * gpioc=AHB1_addr;
	//����ʱ��ʹ��
	*gpioc &= ~(1<<2);
	*gpioc |= (1<<2);
	//����ģʽ
	unsigned int * key_mode=GPIOC_mode;
	* key_mode &= ~(3<<26);
	
	//��������
	unsigned int * key_pupdr=GPIOC_pupdr;
	*key_pupdr &= ~(1<<26);
	*key_pupdr |= (1<<26);
	//����
	unsigned int * key_speed=GPIOC_speed;
	* key_speed &= ~(3<<26);
	
//	//ʱ��ʹ��
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
//	
//	GPIO_InitTypeDef gpiokey;
//	gpiokey.GPIO_Mode=GPIO_Mode_IN;//����ģʽ	
//	gpiokey.GPIO_Pin=GPIO_Pin_13;//4�ſ�
//	gpiokey.GPIO_PuPd=GPIO_PuPd_UP;//��������
//	gpiokey.GPIO_Speed=GPIO_Speed_2MHz;//Ƶ��
//	
//	GPIO_Init(GPIOC,&gpiokey);
		
}


void key_in(){
	
	if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)==0){//����
		delay_ms(2);
		if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)==0){			
			while(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)==0);//����
			led_waterled();				
		}
	}

	if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_5)==0){//����
		
		delay_ms(2);
		if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_5)==0){			
			while(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_5)==0);//����
			beep_out();			
		}			
	}

	if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_6)==0){//����
		delay_ms(2);
		if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_6)==0){			
			while(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_6)==0);//����
			beep_stop();		
		}			
	}

	if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)==0){//����
		delay_ms(2);
		if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)==0){			
			while(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)==0);//����
			beep_out();		
		}			
	}


}
	
