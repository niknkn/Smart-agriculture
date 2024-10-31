#ifndef _DHT11_H_
#define _DHT11_H_
	void det11_init(void);
	void DHT11_InMode(void);
	void DHT11_outMode(void);
	int DHT11_WORK(u8 *humi,u8 *temp);
	u8 Read_Byte(void);
	u8 Read_Bit(void);
	int Start_ask(void);
	
#endif
