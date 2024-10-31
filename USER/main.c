#include "main.h"
void fun_init();
u8 at24_addr=0x00;
uint32_t w25q_addr=0x00; 

int main(){
		
	main_init();	
	while(1){
		if(kaiji_flag){//开机
			printf_oled_kaiji();
			light=getadcaverage1(4);
			DHT11_WORK(&humi,&temp);
			printf_oled();//打印温湿度光照电压
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
		if(!kaiji_flag){//关机
			printf_oled_guanji();
			
		}

	}

//	return 0;
	
}


void main_init(){
	extern u8 buf[5];
	delay_init(168);//延时函数初始化
	beep_init();//蜂鸣器
	led_init();//LED灯
	key_init();//按键123
	fun_init();//风扇
	
	NVIC_init();
	exti_1234key_init();

	usart1_init();//串口1
	usart2_init();//串口2wifi
	NVIC_usart1_init();//串口1发送数据中断
	NVIC_usart2_init();//wifi发送数据中断
	det11_init();//温湿度
	adc_init();//电压，光照
	
//	NVIC_tim4_init();
//	Timx_4_init();
//	
	wifi_firstcmd();//wifi启动代码
	iic_init();//AT24C002存储芯片
	spi_init();//w25q128存储芯片

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
