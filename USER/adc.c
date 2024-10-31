#include "stm32f4xx.h"
#include "delay.h"


void adc_init(){
	//总线时钟使能
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	
	GPIO_InitTypeDef gpioa;
	gpioa.GPIO_Mode=GPIO_Mode_AN;//模拟输入模式
	gpioa.GPIO_Pin=GPIO_Pin_0;
	gpioa.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&gpioa);
	//ADC外设配置以及初始化
	ADC_CommonInitTypeDef adc1;
	adc1.ADC_DMAAccessMode=ADC_DMAAccessMode_Disabled;//失能
	adc1.ADC_Mode=ADC_Mode_Independent;//独立模式
	adc1.ADC_Prescaler=ADC_Prescaler_Div2;//2分频
	adc1.ADC_TwoSamplingDelay=ADC_TwoSamplingDelay_5Cycles;//两个采样阶段之间的延迟5个时钟，这里可以注释，和DMA有关
	ADC_CommonInit(&adc1);
	//adc结构体配置
	ADC_InitTypeDef adc;
	adc.ADC_Resolution=ADC_Resolution_12b;
	adc.ADC_ContinuousConvMode=DISABLE;//关闭扫描源
	adc.ADC_DataAlign=ADC_DataAlign_Right;//数据右对齐
	adc.ADC_ExternalTrigConvEdge=ADC_ExternalTrigConvEdge_None;//不使用外部触发源
	adc.ADC_NbrOfConversion=1;//单次转换
	ADC_Init(ADC1,&adc);
	//使能adc通道
	ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_3Cycles);
	//使能adc
	ADC_Cmd(ADC1,ENABLE);
	
}


//获取adc的值
u16 get_adc_val(){
	//设置规则通道组
	//adc1，通道0，一个序列，480个机器周期，提高采样时间可以提高精度，这是采样时间
	ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_480Cycles);
	//开始转换
	ADC_SoftwareStartConv(ADC1);
	//等待转换结束
	while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));
	//获取结果
	return ADC_GetConversionValue(ADC1);//*3300/4096得到mv毫伏
}


//获取通道ch的转换值，取times次取平均值
unsigned short int getadcaverage1(unsigned char times){
	unsigned short int temp_val=0;
	for(unsigned char t=0;t<times;t++){
		temp_val+=get_adc_val();
		delay_ms(5);
	}
	return temp_val/times;
}