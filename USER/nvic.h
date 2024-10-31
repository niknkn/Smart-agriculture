#ifndef _NVIC_H_
#define _NVIC_H_

	
	
	extern int kaiji_flag;
	void EXTI4_IRQHandler(void);
	void EXTI9_5_IRQHandler(void);
	void EXTI15_10_IRQHandler(void);
	
	void exti_1234key_init(void);

	void NVIC_init(void);
	void NVIC_tim6_init(void);
	void NVIC_tim4_init(void);
	void NVIC_usart1_init(void);
	void NVIC_usart2_init();
	
#endif
