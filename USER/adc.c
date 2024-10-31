#include "stm32f4xx.h"
#include "delay.h"


void adc_init(){
	//����ʱ��ʹ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	
	GPIO_InitTypeDef gpioa;
	gpioa.GPIO_Mode=GPIO_Mode_AN;//ģ������ģʽ
	gpioa.GPIO_Pin=GPIO_Pin_0;
	gpioa.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&gpioa);
	//ADC���������Լ���ʼ��
	ADC_CommonInitTypeDef adc1;
	adc1.ADC_DMAAccessMode=ADC_DMAAccessMode_Disabled;//ʧ��
	adc1.ADC_Mode=ADC_Mode_Independent;//����ģʽ
	adc1.ADC_Prescaler=ADC_Prescaler_Div2;//2��Ƶ
	adc1.ADC_TwoSamplingDelay=ADC_TwoSamplingDelay_5Cycles;//���������׶�֮����ӳ�5��ʱ�ӣ��������ע�ͣ���DMA�й�
	ADC_CommonInit(&adc1);
	//adc�ṹ������
	ADC_InitTypeDef adc;
	adc.ADC_Resolution=ADC_Resolution_12b;
	adc.ADC_ContinuousConvMode=DISABLE;//�ر�ɨ��Դ
	adc.ADC_DataAlign=ADC_DataAlign_Right;//�����Ҷ���
	adc.ADC_ExternalTrigConvEdge=ADC_ExternalTrigConvEdge_None;//��ʹ���ⲿ����Դ
	adc.ADC_NbrOfConversion=1;//����ת��
	ADC_Init(ADC1,&adc);
	//ʹ��adcͨ��
	ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_3Cycles);
	//ʹ��adc
	ADC_Cmd(ADC1,ENABLE);
	
}


//��ȡadc��ֵ
u16 get_adc_val(){
	//���ù���ͨ����
	//adc1��ͨ��0��һ�����У�480���������ڣ���߲���ʱ�������߾��ȣ����ǲ���ʱ��
	ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_480Cycles);
	//��ʼת��
	ADC_SoftwareStartConv(ADC1);
	//�ȴ�ת������
	while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));
	//��ȡ���
	return ADC_GetConversionValue(ADC1);//*3300/4096�õ�mv����
}


//��ȡͨ��ch��ת��ֵ��ȡtimes��ȡƽ��ֵ
unsigned short int getadcaverage1(unsigned char times){
	unsigned short int temp_val=0;
	for(unsigned char t=0;t<times;t++){
		temp_val+=get_adc_val();
		delay_ms(5);
	}
	return temp_val/times;
}