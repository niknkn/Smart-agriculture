#include "stm32f4xx.h"
#include "dht11.h"
#include "delay.h"
#include "stdio.h"
void det11_init(){
	//1，配置时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
    //2.配置引脚
    GPIO_InitTypeDef DHT11;
    //填写引脚
    DHT11.GPIO_Pin = GPIO_Pin_3;
    //填写速度
    DHT11.GPIO_Speed = GPIO_Speed_50MHz;
    DHT11.GPIO_PuPd = GPIO_PuPd_NOPULL ;
    //填写电阻
    DHT11.GPIO_Mode = GPIO_Mode_OUT;
    //即可以输出高电平也可以输出低电平
    DHT11.GPIO_OType = GPIO_OType_OD;
    GPIO_Init(GPIOA,&DHT11);
    //把引脚拉高。
    GPIO_SetBits(GPIOA,GPIO_Pin_3);  

}


void DHT11_InMode(){//输入模式函数
	GPIO_InitTypeDef dht11;
	dht11.GPIO_Mode=GPIO_Mode_IN;//输入模式
	dht11.GPIO_Pin=GPIO_Pin_3;
	dht11.GPIO_PuPd=GPIO_PuPd_NOPULL;
	dht11.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&dht11);	
}


void DHT11_outMode(){//输出模式函数
	GPIO_InitTypeDef dht11;
	dht11.GPIO_Mode=GPIO_Mode_OUT;//输出模式
	dht11.GPIO_Pin=GPIO_Pin_3;
	dht11.GPIO_OType=GPIO_OType_OD;
	dht11.GPIO_PuPd=GPIO_PuPd_NOPULL;
	dht11.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&dht11);	
	GPIO_SetBits(GPIOA,GPIO_Pin_3);
}


//主机发送一个起始信号
int Start_ask(void){
	//printf("ceshi\n");
	DHT11_outMode();
	GPIO_SetBits(GPIOA,GPIO_Pin_3);
	//拉低 20ms
	GPIO_ResetBits(GPIOA,GPIO_Pin_3);
	delay_ms(20);
	//拉高
	GPIO_SetBits(GPIOA,GPIO_Pin_3);
	delay_us(30);//延时30us
	
	//响应
	DHT11_InMode();//切换为输入模式
	//等待变低  等待的时间不能太长 建议是80us  
	int t1=1;	
	while((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3))&&t1<100){
		delay_us(1);
		t1++;
	}
	//printf("t1:%d\r\n",t1);
	
	int t=0;	
	while(!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)&&t<80){//如果一直维持低电平 就循环 ---》等待变高点平	
		t++;
		delay_us(1);		
	}	//printf("t=%d\r\n",t);
	if(t>=80) return 0;

	//等待 变低
	t1=0;
	while((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3))&&t1<80){
		delay_us(1);
		t1++;
	}
	//printf("t1=%d\r\n",t1);
	if(t1>=80) return 0;
		
	return 1;
}


u8 Read_Bit(void){
	DHT11_InMode();//输入模式
	//等待点平变低
	while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)==SET);
	//等待电平 变高  
	while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)==RESET);
	delay_us(40);//40us以后如果读到仍旧是高电平 意味着  这一位数据为1
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)==SET){
		return 1;
	}
	return 0;
}

u8 Read_Byte(void){
	u8 dht11_data=0;
	for(int i=7;i>=0;i--){//MSB  高位在前
		dht11_data|=Read_Bit()<<i;
	}
	return dht11_data;
}


u8 buf[5]={0};
int DHT11_WORK(u8 *humi,u8 *temp){
	u8 data;
	
	if(Start_ask())//响应
	{
	//	printf("响应\r\n");
		for(int i=0;i<5;i++){
			buf[i]=Read_Byte();
		}
		if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4]){
			*humi=buf[0];		
			*temp=buf[2];
			return 1;
		}
		DHT11_outMode();
		
	}
	return 0;
}







