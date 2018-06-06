/* Includes */
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

int j = 0;

void InitializeTimer(int period = 55999){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	TIM_TimeBaseInitTypeDef timerInitStructure;
	timerInitStructure.TIM_Prescaler = 29;
	timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	timerInitStructure.TIM_Period = period;
	timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	timerInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM4, &timerInitStructure);

	TIM_Cmd(TIM4, ENABLE);

}

void PWM(int duty = 500){

	TIM_OCInitTypeDef outputChannelInit = {0,};
	outputChannelInit.TIM_OCMode = TIM_OCMode_PWM1;
	outputChannelInit.TIM_Pulse = duty;//(57143+1)*duty/100000 - 1;
	outputChannelInit.TIM_OutputState = TIM_OutputState_Enable;
	outputChannelInit.TIM_OCPolarity = TIM_OCPolarity_High;

	TIM_OC3Init(TIM4, &outputChannelInit);
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);

}

void InitializeLEDs(){
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	GPIO_InitTypeDef gpioStructure;
	gpioStructure.GPIO_Pin = GPIO_Pin_14;
	gpioStructure.GPIO_Mode = GPIO_Mode_AF;
	gpioStructure.GPIO_Speed = GPIO_Speed_100MHz;
	gpioStructure.GPIO_OType = GPIO_OType_PP;
	gpioStructure.GPIO_PuPd = GPIO_PuPd_UP;

	//economia de energia
	GPIO_InitTypeDef gpioStructure2;
	gpioStructure2.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_13 | GPIO_Pin_15
							| GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4
							| GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
	gpioStructure2.GPIO_Mode = GPIO_Mode_AN;
	gpioStructure2.GPIO_Speed = GPIO_Speed_100MHz;
	gpioStructure2.GPIO_OType = GPIO_OType_PP;
	gpioStructure2.GPIO_PuPd = GPIO_PuPd_UP;

	GPIO_Init(GPIOD, &gpioStructure);

	GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_TIM4);
}

void ConfigInt(){
	/* Set variables used */
		GPIO_InitTypeDef GPIO_InitStruct;
	    EXTI_InitTypeDef EXTI_InitStruct;
	    NVIC_InitTypeDef NVIC_InitStruct;

	    /* Enable clock for GPIOA */
	    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	    /* Enable clock for SYSCFG */
	    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	    /* Set pin as input */
	    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	    GPIO_Init(GPIOA, &GPIO_InitStruct);

	    /* Tell system that you will use PA0 for EXTI_Line0 */
	    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);

	    /* PA0 is connected to EXTI_Line0 */
	    EXTI_InitStruct.EXTI_Line = EXTI_Line0;
	    /* Enable interrupt */
	    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	    /* Interrupt mode */
	    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	    /* Triggers on rising and falling edge */
	    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	    /* Add to EXTI */
	    EXTI_Init(&EXTI_InitStruct);

	    /* Add IRQ vector to NVIC */
	    /* PA0 is connected to EXTI_Line0, which has EXTI0_IRQn vector */
	    NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;
	    /* Set priority */
	    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
	    /* Set sub priority */
	    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
	    /* Enable interrupt */
	    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	    /* Add to NVIC */
	    NVIC_Init(&NVIC_InitStruct);
}

/* Set interrupt handlers */
/* Handle PA0 interrupt */
extern "C" void EXTI0_IRQHandler(void) {
	//interrupcao
     //Make sure that interrupt flag is set
    if (EXTI_GetITStatus(EXTI_Line0) != RESET) {
        // Do your stuff when PA0 is changed */
        	GPIO_ToggleBits(GPIOD,GPIO_Pin_12);

        	TIM4->CCR3 = 6000 - 100*j;
        	j++;

        	if (j == 30 ){
        		TIM4->CCR3 = 6000;
        	}

        // Clear interrupt flag
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}

int main(void)
{
	int i=0;

	InitializeLEDs();
	InitializeTimer();
	PWM(0);

	/* TODO - Add your application code here */
	SystemInit();
	//ConfigInt();

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_EXTI);

	STM_EVAL_LEDInit(LED4);

	/* Infinite loop */

	while (1){
		i++;
		//PWM(4200);
		//TIM4->CCR3 = 4200;
	}
}


/*
 * Callback used by stm32f4_discovery_audio_codec.c.
 * Refer to stm32f4_discovery_audio_codec.h for more info.
 */
extern "C" void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size){
  /* TODO, implement your code here */
  return;
}

/*
 * Callback used by stm324xg_eval_audio_codec.c.
 * Refer to stm324xg_eval_audio_codec.h for more info.
 */
extern "C" uint16_t EVAL_AUDIO_GetSampleCallBack(void){
  /* TODO, implement your code here */
  return -1;
}
