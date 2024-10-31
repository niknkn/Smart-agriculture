#include "stm32f4xx.h"
#include "nvic.h"
#include "wifi.h"
#include "oled.h"
#include "delay.h"
void NVIC_init(void){
	//配置中断分组
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	//配置中断结构体
	NVIC_InitTypeDef nvic1;	
	nvic1.NVIC_IRQChannel=EXTI4_IRQn;//中断通道
	nvic1.NVIC_IRQChannelCmd=ENABLE;//使能
	nvic1.NVIC_IRQChannelPreemptionPriority=0;//主优先级
	nvic1.NVIC_IRQChannelSubPriority=0;//从优先级	
	NVIC_Init(&nvic1);
	
	//配置中断结构体
	NVIC_InitTypeDef nvic2;	
	nvic2.NVIC_IRQChannel=EXTI9_5_IRQn;//中断通道
	nvic2.NVIC_IRQChannelCmd=ENABLE;//使能
	nvic2.NVIC_IRQChannelPreemptionPriority=0;//主优先级
	nvic2.NVIC_IRQChannelSubPriority=1;//从优先级	
	NVIC_Init(&nvic2);	
	
	//配置中断结构体
	NVIC_InitTypeDef nvic3;	
	nvic3.NVIC_IRQChannel=EXTI15_10_IRQn;//中断通道
	nvic3.NVIC_IRQChannelCmd=ENABLE;//使能
	nvic3.NVIC_IRQChannelPreemptionPriority=0;//主优先级
	nvic3.NVIC_IRQChannelSubPriority=2;//从优先级	
	NVIC_Init(&nvic3);	
}


void NVIC_tim6_init(){//计时器
	NVIC_InitTypeDef tim6;
	tim6.NVIC_IRQChannel=TIM6_DAC_IRQn;
	tim6.NVIC_IRQChannelCmd=ENABLE;
	tim6.NVIC_IRQChannelPreemptionPriority=0;
	tim6.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&tim6);
}


void NVIC_tim4_init(){//软件消抖
	NVIC_InitTypeDef tim4;
	tim4.NVIC_IRQChannel=TIM4_IRQn;
	tim4.NVIC_IRQChannelCmd=ENABLE;
	tim4.NVIC_IRQChannelPreemptionPriority=0;
	tim4.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&tim4);
}


void NVIC_usart1_init(){
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitTypeDef usart1;
	usart1.NVIC_IRQChannel=USART1_IRQn;
	usart1.NVIC_IRQChannelCmd=ENABLE;
	usart1.NVIC_IRQChannelPreemptionPriority=0;
	usart1.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&usart1);
}


void NVIC_usart2_init(){
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitTypeDef usart2;
	usart2.NVIC_IRQChannel=USART2_IRQn;
	usart2.NVIC_IRQChannelCmd=ENABLE;
	usart2.NVIC_IRQChannelPreemptionPriority=0;
	usart2.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&usart2);
}


void exti_1234key_init(void){	
	//1 外部中断时钟总线-系统时钟。  42MHZ
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
	
    //2.中断线映射函数 PE4配置成 外部中断4； 
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,EXTI_PinSource4);
	EXTI_InitTypeDef exti1;
	exti1.EXTI_Line=EXTI_Line4;//四号引脚
	exti1.EXTI_LineCmd=ENABLE;//使能
	exti1.EXTI_Mode=EXTI_Mode_Interrupt;//中断
	exti1.EXTI_Trigger=EXTI_Trigger_Falling;//下降沿	
	EXTI_Init(&exti1);
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,EXTI_PinSource5);	
	EXTI_InitTypeDef exti2;
	exti2.EXTI_Line=EXTI_Line5;//5号引脚
	exti2.EXTI_LineCmd=ENABLE;//使能
	exti2.EXTI_Mode=EXTI_Mode_Interrupt;//中断
	exti2.EXTI_Trigger=EXTI_Trigger_Falling;//下降沿
	EXTI_Init(&exti2);
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,EXTI_PinSource6);	
	EXTI_InitTypeDef exti3;
	exti3.EXTI_Line=EXTI_Line6;//6号引脚
	exti3.EXTI_LineCmd=ENABLE;//使能
	exti3.EXTI_Mode=EXTI_Mode_Interrupt;//中断
	exti3.EXTI_Trigger=EXTI_Trigger_Falling;//下降沿
	EXTI_Init(&exti3);
	
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC,EXTI_PinSource13);	
//	EXTI_InitTypeDef exti4;
//	exti4.EXTI_Line=EXTI_Line13;//13号引脚
//	exti4.EXTI_LineCmd=ENABLE;//使能
//	exti4.EXTI_Mode=EXTI_Mode_Interrupt;//中断
//	exti4.EXTI_Trigger=EXTI_Trigger_Falling;//下降沿
//	EXTI_Init(&exti4);
}


void EXTI4_IRQHandler(void){
    if(EXTI_GetITStatus(EXTI_Line4)){//有中断响应    	
		//GPIO_ToggleBits(GPIOE,GPIO_Pin_8);//引脚翻转--LED1
		kaiji_flag=1;
		//清除中断标志位
		EXTI_ClearITPendingBit(EXTI_Line4);		      
    }
}


void EXTI9_5_IRQHandler(void){	
	if(EXTI_GetITStatus(EXTI_Line5)){//有中断响应            
        //GPIO_ToggleBits(GPIOE,GPIO_Pin_9);//引脚翻转--LED2
		if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_5)==0){	
			while(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_5)==0);//松手
			kaiji_flag=0;			
		}			
		
        //清除中断标志位
        EXTI_ClearITPendingBit(EXTI_Line5);
    }
		else if(EXTI_GetITStatus(EXTI_Line6)){//有中断响应           
        //GPIO_ToggleBits(GPIOE,GPIO_Pin_10);//引脚翻转--LED3
        //清除中断标志位s
        EXTI_ClearITPendingBit(EXTI_Line6);
		}				
}


void EXTI15_10_IRQHandler(void){
//	if(EXTI_GetITStatus(EXTI_Line13)){//有中断响应            
//        GPIO_ToggleBits(GPIOB,GPIO_Pin_10);//引脚翻转--BEEP
//        //清除中断标志位
//        EXTI_ClearITPendingBit(EXTI_Line13);
//    }
}







