#ifndef _IIC_H_
#define _IIC_H_
	
	#define IIC_PORT GPIOB
	
	#define IIC_SDA_PIN GPIO_Pin_7
	#define IIC_SCL_PIN GPIO_Pin_6
	
	//SCL时钟写操作
	#define SCL_Write(N) (N)?GPIO_SetBits(IIC_PORT,IIC_SCL_PIN):GPIO_ResetBits(IIC_PORT,IIC_SCL_PIN)
	//SDA数据写操作
	#define SDA_Write(N) (N)?GPIO_SetBits(IIC_PORT,IIC_SDA_PIN):GPIO_ResetBits(IIC_PORT,IIC_SDA_PIN)
	
	u8 AT24C02_read_data(u8 addr);
	void IIC_AT24C02_write_data(u8 addr,u8 data);
	u8 IIC_read_Byte(void);
	void IIC_send_Byte(u8 data);
	void IIC_send_Ask(u8 ask);
	u8 IIC_get_Ask(void);
	void stop_signal();
	void start_signal();
	void SDA_in();
	void SDA_out();
	void iic_init();
	
#endif