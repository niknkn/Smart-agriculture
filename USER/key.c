#include "stm32f4xx.h"
#include "key.h"
#include "delay.h"
#include "beep.h"
#include "led.h"

#define rcc_addr 0x40023800
//AHB1总线的地址
#define AHB1_addr (unsigned int *) (rcc_addr+0x30)
#define GPIOC_addr 0x40020800
#define GPIOC_mode (unsigned int *)(GPIOC_addr+0x00)//模式
#define GPIOC_pupdr (unsigned int *)(GPIOC_addr+0x0C)//上下拉
#define GPIOC_speed (unsigned int *)(GPIOC_addr+0x08)//速率	

void key_init(){
	//时钟使能
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	
	GPIO_InitTypeDef gpiokey;
	gpiokey.GPIO_Mode=GPIO_Mode_IN;//输入模式	
	gpiokey.GPIO_Pin=GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;//4号口
	gpiokey.GPIO_PuPd=GPIO_PuPd_UP;//上拉电阻
	gpiokey.GPIO_Speed=GPIO_Speed_2MHz;//频率
	
	GPIO_Init(GPIOE,&gpiokey);
	
}


void key_init_jcq(){
	unsigned int * gpioc=AHB1_addr;
	//总线时钟使能
	*gpioc &= ~(1<<2);
	*gpioc |= (1<<2);
	//输入模式
	unsigned int * key_mode=GPIOC_mode;
	* key_mode &= ~(3<<26);
	
	//上拉输入
	unsigned int * key_pupdr=GPIOC_pupdr;
	*key_pupdr &= ~(1<<26);
	*key_pupdr |= (1<<26);
	//低速
	unsigned int * key_speed=GPIOC_speed;
	* key_speed &= ~(3<<26);
	
//	//时钟使能
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
//	
//	GPIO_InitTypeDef gpiokey;
//	gpiokey.GPIO_Mode=GPIO_Mode_IN;//输入模式	
//	gpiokey.GPIO_Pin=GPIO_Pin_13;//4号口
//	gpiokey.GPIO_PuPd=GPIO_PuPd_UP;//上拉电阻
//	gpiokey.GPIO_Speed=GPIO_Speed_2MHz;//频率
//	
//	GPIO_Init(GPIOC,&gpiokey);
		
}


void key_in(){
	
	if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)==0){//按下
		delay_ms(2);
		if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)==0){			
			while(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)==0);//松手
			led_waterled();				
		}
	}

	if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_5)==0){//按下
		
		delay_ms(2);
		if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_5)==0){			
			while(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_5)==0);//松手
			beep_out();			
		}			
	}

	if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_6)==0){//按下
		delay_ms(2);
		if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_6)==0){			
			while(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_6)==0);//松手
			beep_stop();		
		}			
	}

	if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)==0){//按下
		delay_ms(2);
		if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)==0){			
			while(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)==0);//松手
			beep_out();		
		}			
	}


}
	
