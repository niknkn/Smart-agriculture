#include "stm32f4xx.h"
#include "led.h"
#include "delay.h"

//RCC����ַ
#define rcc_addr 0x40023800
//GPIOE��AHB1���ߵĵ�ַ
#define ahb1_addr (unsigned int *) (rcc_addr+0x30)

#define GPIOE_addr 0x40021000
#define GPIOE_MODE_addr (unsigned int *) (GPIOE_addr+0x00)
#define GPIOE_OPYTER_addr (unsigned int *) (GPIOE_addr+0x04)
#define GPIOE_FASTER_addr (unsigned int *) (GPIOE_addr+0x08)
#define GPIOE_ODR_addr (unsigned int *) (GPIOE_addr+0x14)

GPIO_InitTypeDef gpioled;

void led_init(){
	//1.ʹ��ʱ�ӣ� led3 λ��PE8 GPIOE�˵�8������ �ֲ��֪������AHB1������	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	//gpio�ṹ������
	
	gpioled.GPIO_Mode=GPIO_Mode_OUT;//���ģʽ
	gpioled.GPIO_OType=GPIO_OType_PP;//������� �������
	gpioled.GPIO_Pin=GPIO_Pin_10|GPIO_Pin_9|GPIO_Pin_8;//����
	gpioled.GPIO_Speed=GPIO_Speed_2MHz;//Ƶ��
	
	GPIO_Init(GPIOE,&gpioled);//����gpio����
	GPIO_WriteBit(GPIOE,GPIO_Pin_10|GPIO_Pin_9|GPIO_Pin_8,Bit_SET);//������д1(ԭ��ͼ),����
}

void led_alllight(){
	GPIO_WriteBit(GPIOE,GPIO_Pin_10|GPIO_Pin_9|GPIO_Pin_8,Bit_RESET);//������д0(ԭ��ͼ),����
}

void led_allclose(){
	GPIO_WriteBit(GPIOE,GPIO_Pin_10|GPIO_Pin_9|GPIO_Pin_8,Bit_SET);//������д1(ԭ��ͼ),����
}


void led_waterled(){

	unsigned int *led_addr=ahb1_addr;
	//����ʱ��ʹ��
	*led_addr &= ~(1<<2);
	*led_addr |= (1<<2);
	
	//ѡ�ƣ����ģʽ	
	unsigned int *mode=GPIOE_MODE_addr;
	*mode &= ~(63<<16);//11 1111 ����ȡ�� ����
	*mode |= (21<<16); //01 0101 
	
	//���ģʽ���������
	unsigned int *opyter=GPIOE_OPYTER_addr;	
	*opyter &= ~(7<<8);
	
	//Ƶ��2mhz
	unsigned int *faster=GPIOE_FASTER_addr;
	//*faster=0x00;
	*faster &= ~(63<<16);
	
	//��0����
	unsigned int *odr=GPIOE_ODR_addr;
	
	int a=0;	
	while(1){

		//��0����
		*odr |= (7<<8);//ȫ��	
		
		*odr &= ~(4<<8);//3��
		delay_ms(200);	
		*odr |= (7<<8);	
		
		*odr &= ~(2<<8);//2��
		delay_ms(200);
		*odr |= (7<<8);
		
		*odr &= ~(1<<8);//1��
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
