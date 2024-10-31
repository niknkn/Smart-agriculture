#include "stm32f4xx.h"
#include "nvic.h"
#include "wifi.h"
#include "oled.h"
#include "delay.h"
void NVIC_init(void){
	//�����жϷ���
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	//�����жϽṹ��
	NVIC_InitTypeDef nvic1;	
	nvic1.NVIC_IRQChannel=EXTI4_IRQn;//�ж�ͨ��
	nvic1.NVIC_IRQChannelCmd=ENABLE;//ʹ��
	nvic1.NVIC_IRQChannelPreemptionPriority=0;//�����ȼ�
	nvic1.NVIC_IRQChannelSubPriority=0;//�����ȼ�	
	NVIC_Init(&nvic1);
	
	//�����жϽṹ��
	NVIC_InitTypeDef nvic2;	
	nvic2.NVIC_IRQChannel=EXTI9_5_IRQn;//�ж�ͨ��
	nvic2.NVIC_IRQChannelCmd=ENABLE;//ʹ��
	nvic2.NVIC_IRQChannelPreemptionPriority=0;//�����ȼ�
	nvic2.NVIC_IRQChannelSubPriority=1;//�����ȼ�	
	NVIC_Init(&nvic2);	
	
	//�����жϽṹ��
	NVIC_InitTypeDef nvic3;	
	nvic3.NVIC_IRQChannel=EXTI15_10_IRQn;//�ж�ͨ��
	nvic3.NVIC_IRQChannelCmd=ENABLE;//ʹ��
	nvic3.NVIC_IRQChannelPreemptionPriority=0;//�����ȼ�
	nvic3.NVIC_IRQChannelSubPriority=2;//�����ȼ�	
	NVIC_Init(&nvic3);	
}


void NVIC_tim6_init(){//��ʱ��
	NVIC_InitTypeDef tim6;
	tim6.NVIC_IRQChannel=TIM6_DAC_IRQn;
	tim6.NVIC_IRQChannelCmd=ENABLE;
	tim6.NVIC_IRQChannelPreemptionPriority=0;
	tim6.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&tim6);
}


void NVIC_tim4_init(){//�������
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
	//1 �ⲿ�ж�ʱ������-ϵͳʱ�ӡ�  42MHZ
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
	
    //2.�ж���ӳ�亯�� PE4���ó� �ⲿ�ж�4�� 
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,EXTI_PinSource4);
	EXTI_InitTypeDef exti1;
	exti1.EXTI_Line=EXTI_Line4;//�ĺ�����
	exti1.EXTI_LineCmd=ENABLE;//ʹ��
	exti1.EXTI_Mode=EXTI_Mode_Interrupt;//�ж�
	exti1.EXTI_Trigger=EXTI_Trigger_Falling;//�½���	
	EXTI_Init(&exti1);
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,EXTI_PinSource5);	
	EXTI_InitTypeDef exti2;
	exti2.EXTI_Line=EXTI_Line5;//5������
	exti2.EXTI_LineCmd=ENABLE;//ʹ��
	exti2.EXTI_Mode=EXTI_Mode_Interrupt;//�ж�
	exti2.EXTI_Trigger=EXTI_Trigger_Falling;//�½���
	EXTI_Init(&exti2);
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,EXTI_PinSource6);	
	EXTI_InitTypeDef exti3;
	exti3.EXTI_Line=EXTI_Line6;//6������
	exti3.EXTI_LineCmd=ENABLE;//ʹ��
	exti3.EXTI_Mode=EXTI_Mode_Interrupt;//�ж�
	exti3.EXTI_Trigger=EXTI_Trigger_Falling;//�½���
	EXTI_Init(&exti3);
	
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC,EXTI_PinSource13);	
//	EXTI_InitTypeDef exti4;
//	exti4.EXTI_Line=EXTI_Line13;//13������
//	exti4.EXTI_LineCmd=ENABLE;//ʹ��
//	exti4.EXTI_Mode=EXTI_Mode_Interrupt;//�ж�
//	exti4.EXTI_Trigger=EXTI_Trigger_Falling;//�½���
//	EXTI_Init(&exti4);
}


void EXTI4_IRQHandler(void){
    if(EXTI_GetITStatus(EXTI_Line4)){//���ж���Ӧ    	
		//GPIO_ToggleBits(GPIOE,GPIO_Pin_8);//���ŷ�ת--LED1
		kaiji_flag=1;
		//����жϱ�־λ
		EXTI_ClearITPendingBit(EXTI_Line4);		      
    }
}


void EXTI9_5_IRQHandler(void){	
	if(EXTI_GetITStatus(EXTI_Line5)){//���ж���Ӧ            
        //GPIO_ToggleBits(GPIOE,GPIO_Pin_9);//���ŷ�ת--LED2
		if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_5)==0){	
			while(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_5)==0);//����
			kaiji_flag=0;			
		}			
		
        //����жϱ�־λ
        EXTI_ClearITPendingBit(EXTI_Line5);
    }
		else if(EXTI_GetITStatus(EXTI_Line6)){//���ж���Ӧ           
        //GPIO_ToggleBits(GPIOE,GPIO_Pin_10);//���ŷ�ת--LED3
        //����жϱ�־λs
        EXTI_ClearITPendingBit(EXTI_Line6);
		}				
}


void EXTI15_10_IRQHandler(void){
//	if(EXTI_GetITStatus(EXTI_Line13)){//���ж���Ӧ            
//        GPIO_ToggleBits(GPIOB,GPIO_Pin_10);//���ŷ�ת--BEEP
//        //����жϱ�־λ
//        EXTI_ClearITPendingBit(EXTI_Line13);
//    }
}







