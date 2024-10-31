#include "stm32f4xx.h"
#include "dht11.h"
#include "delay.h"
#include "stdio.h"
void det11_init(){
	//1������ʱ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
    //2.��������
    GPIO_InitTypeDef DHT11;
    //��д����
    DHT11.GPIO_Pin = GPIO_Pin_3;
    //��д�ٶ�
    DHT11.GPIO_Speed = GPIO_Speed_50MHz;
    DHT11.GPIO_PuPd = GPIO_PuPd_NOPULL ;
    //��д����
    DHT11.GPIO_Mode = GPIO_Mode_OUT;
    //����������ߵ�ƽҲ��������͵�ƽ
    DHT11.GPIO_OType = GPIO_OType_OD;
    GPIO_Init(GPIOA,&DHT11);
    //���������ߡ�
    GPIO_SetBits(GPIOA,GPIO_Pin_3);  

}


void DHT11_InMode(){//����ģʽ����
	GPIO_InitTypeDef dht11;
	dht11.GPIO_Mode=GPIO_Mode_IN;//����ģʽ
	dht11.GPIO_Pin=GPIO_Pin_3;
	dht11.GPIO_PuPd=GPIO_PuPd_NOPULL;
	dht11.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&dht11);	
}


void DHT11_outMode(){//���ģʽ����
	GPIO_InitTypeDef dht11;
	dht11.GPIO_Mode=GPIO_Mode_OUT;//���ģʽ
	dht11.GPIO_Pin=GPIO_Pin_3;
	dht11.GPIO_OType=GPIO_OType_OD;
	dht11.GPIO_PuPd=GPIO_PuPd_NOPULL;
	dht11.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&dht11);	
	GPIO_SetBits(GPIOA,GPIO_Pin_3);
}


//��������һ����ʼ�ź�
int Start_ask(void){
	//printf("ceshi\n");
	DHT11_outMode();
	GPIO_SetBits(GPIOA,GPIO_Pin_3);
	//���� 20ms
	GPIO_ResetBits(GPIOA,GPIO_Pin_3);
	delay_ms(20);
	//����
	GPIO_SetBits(GPIOA,GPIO_Pin_3);
	delay_us(30);//��ʱ30us
	
	//��Ӧ
	DHT11_InMode();//�л�Ϊ����ģʽ
	//�ȴ����  �ȴ���ʱ�䲻��̫�� ������80us  
	int t1=1;	
	while((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3))&&t1<100){
		delay_us(1);
		t1++;
	}
	//printf("t1:%d\r\n",t1);
	
	int t=0;	
	while(!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)&&t<80){//���һֱά�ֵ͵�ƽ ��ѭ�� ---���ȴ���ߵ�ƽ	
		t++;
		delay_us(1);		
	}	//printf("t=%d\r\n",t);
	if(t>=80) return 0;

	//�ȴ� ���
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
	DHT11_InMode();//����ģʽ
	//�ȴ���ƽ���
	while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)==SET);
	//�ȴ���ƽ ���  
	while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)==RESET);
	delay_us(40);//40us�Ժ���������Ծ��Ǹߵ�ƽ ��ζ��  ��һλ����Ϊ1
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)==SET){
		return 1;
	}
	return 0;
}

u8 Read_Byte(void){
	u8 dht11_data=0;
	for(int i=7;i>=0;i--){//MSB  ��λ��ǰ
		dht11_data|=Read_Bit()<<i;
	}
	return dht11_data;
}


u8 buf[5]={0};
int DHT11_WORK(u8 *humi,u8 *temp){
	u8 data;
	
	if(Start_ask())//��Ӧ
	{
	//	printf("��Ӧ\r\n");
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







