#ifndef _MAIN_H_
#define _MAIN_H_

#include "stm32f4xx.h"
#include "led.h"
#include "key.h"
#include "beep.h"
#include "delay.h"
#include "nvic.h"
#include "systick.h"
#include "timx.h"
#include "pwm.h"
#include "usart.h"
#include "stdio.h"
#include "dht11.h"
#include "stdio.h"
#include "iic.h"
#include "spi.h"
#include "adc.h"
#include "wifi.h"
#include "oled.h"

void main_init();

u8 buff_recvwifi;
u8 data=0;
u8 humi,temp;
u16 light;
int wifi_send_flag=0;

int kaiji_flag=0;
#endif
