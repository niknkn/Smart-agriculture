#include "main.h"
void fun_init();
u8 at24_addr=0x00;
uint32_t w25q_addr=0x00; 

int main(){
		
	main_init();	
	while(1){
		if(kaiji_flag){//����
			printf_oled_kaiji();
			light=getadcaverage1(4);
			DHT11_WORK(&humi,&temp);
			printf_oled();//��ӡ��ʪ�ȹ��յ�ѹ
			if(temp>=30){
				GPIO_ToggleBits(GPIOB,GPIO_Pin_15);
				for(int i=0;i<3;i++){
					beep_out();
					delay_ms(500);	
					beep_stop();
					delay_ms(500);	
				}	
				GPIO_ToggleBits(GPIOB,GPIO_Pin_15);
			}
			
			IIC_AT24C02_write_data(at24_addr++,temp);
			IIC_AT24C02_write_data(at24_addr++,humi);
			IIC_AT24C02_write_data(at24_addr++,light);
			if(at24_addr>255) at24_addr=0x00;
			W25Q128_PageProgram(w25q_addr++,&temp,sizeof(temp));
			W25Q128_PageProgram(w25q_addr++,&humi,sizeof(humi));
			W25Q128_PageProgram(w25q_addr++,(uint8_t *)&light,sizeof(light));
			if(w25q_addr>255) w25q_addr=0x00;
			delay_ms(1000);		
			
		}
		if(!kaiji_flag){//�ػ�
			printf_oled_guanji();
			
		}

	}

//	return 0;
	
}


void main_init(){
	extern u8 buf[5];
	delay_init(168);//��ʱ������ʼ��
	beep_init();//������
	led_init();//LED��
	key_init();//����123
	fun_init();//����
	
	NVIC_init();
	exti_1234key_init();

	usart1_init();//����1
	usart2_init();//����2wifi
	NVIC_usart1_init();//����1���������ж�
	NVIC_usart2_init();//wifi���������ж�
	det11_init();//��ʪ��
	adc_init();//��ѹ������
	
//	NVIC_tim4_init();
//	Timx_4_init();
//	
	wifi_firstcmd();//wifi��������
	iic_init();//AT24C002�洢оƬ
	spi_init();//w25q128�洢оƬ

}


void fun_init(){
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef fun;
	fun.GPIO_Mode=GPIO_Mode_OUT;
	fun.GPIO_OType=GPIO_OType_PP;
	fun.GPIO_Pin=GPIO_Pin_15;
	fun.GPIO_PuPd=GPIO_PuPd_UP;
	fun.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&fun);
	
}
