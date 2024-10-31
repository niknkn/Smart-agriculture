#include "stm32f4xx.h"
#include "led.h"
#include "delay.h"

//RCC基地址
#define rcc_addr 0x40023800
//GPIOE在AHB1总线的地址
#define ahb1_addr (unsigned int *) (rcc_addr+0x30)

#define GPIOE_addr 0x40021000
#define GPIOE_MODE_addr (unsigned int *) (GPIOE_addr+0x00)
#define GPIOE_OPYTER_addr (unsigned int *) (GPIOE_addr+0x04)
#define GPIOE_FASTER_addr (unsigned int *) (GPIOE_addr+0x08)
#define GPIOE_ODR_addr (unsigned int *) (GPIOE_addr+0x14)

GPIO_InitTypeDef gpioled;

void led_init(){
	//1.使能时钟， led3 位于PE8 GPIOE端的8号引脚 手册可知挂载在AHB1总线上	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	//gpio结构体配置
	
	gpioled.GPIO_Mode=GPIO_Mode_OUT;//输出模式
	gpioled.GPIO_OType=GPIO_OType_PP;//输出类型 推挽输出
	gpioled.GPIO_Pin=GPIO_Pin_10|GPIO_Pin_9|GPIO_Pin_8;//引脚
	gpioled.GPIO_Speed=GPIO_Speed_2MHz;//频率
	
	GPIO_Init(GPIOE,&gpioled);//启用gpio配置
	GPIO_WriteBit(GPIOE,GPIO_Pin_10|GPIO_Pin_9|GPIO_Pin_8,Bit_SET);//向引脚写1(原理图),灯灭
}

void led_alllight(){
	GPIO_WriteBit(GPIOE,GPIO_Pin_10|GPIO_Pin_9|GPIO_Pin_8,Bit_RESET);//向引脚写0(原理图),灯亮
}

void led_allclose(){
	GPIO_WriteBit(GPIOE,GPIO_Pin_10|GPIO_Pin_9|GPIO_Pin_8,Bit_SET);//向引脚写1(原理图),灯灭
}


void led_waterled(){

	unsigned int *led_addr=ahb1_addr;
	//总线时钟使能
	*led_addr &= ~(1<<2);
	*led_addr |= (1<<2);
	
	//选灯，输出模式	
	unsigned int *mode=GPIOE_MODE_addr;
	*mode &= ~(63<<16);//11 1111 左移取反 置零
	*mode |= (21<<16); //01 0101 
	
	//输出模式，推挽输出
	unsigned int *opyter=GPIOE_OPYTER_addr;	
	*opyter &= ~(7<<8);
	
	//频率2mhz
	unsigned int *faster=GPIOE_FASTER_addr;
	//*faster=0x00;
	*faster &= ~(63<<16);
	
	//赋0亮灯
	unsigned int *odr=GPIOE_ODR_addr;
	
	int a=0;	
	while(1){

		//赋0亮灯
		*odr |= (7<<8);//全灭	
		
		*odr &= ~(4<<8);//3亮
		delay_ms(200);	
		*odr |= (7<<8);	
		
		*odr &= ~(2<<8);//2亮
		delay_ms(200);
		*odr |= (7<<8);
		
		*odr &= ~(1<<8);//1亮
		delay_ms(200);		
		*odr |= (7<<8);		
					
		if(a==4){
			break;
		}
		a++;
	}

}

void led_3(){
	GPIO_ToggleBits(GPIOE,GPIO_Pin_10);
}
