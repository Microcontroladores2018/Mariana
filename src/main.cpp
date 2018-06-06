/* Includes */
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"


//int duty=0;

//void PWM(int duty);
/* Funo de interrupo do Timer4, ou seja, roda de 20ms em 20ms */
//extern "C" void TIM4_IRQHandler()
//{
	//TIM4->SR = 0; /* Zera a contagem quando esta chega a 57143 - 20ms */
	//PWM(duty); /* Inicia o PWM com o duty cicle definido pelo valor da varivel duty */
//}


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

	   //NVIC_InitTypeDef nvicStructure; /* Estrutura para a configurao da estrutura de interrupo */
	    //nvicStructure.NVIC_IRQChannel = TIM4_IRQn; /* Interrupo global do Timer 4 */
	    //nvicStructure.NVIC_IRQChannelPreemptionPriority = 0; /* Prioridade 0 */
	    //nvicStructure.NVIC_IRQChannelSubPriority = 1; /* Subprioridade 1 */
	    //nvicStructure.NVIC_IRQChannelCmd = ENABLE; /* Habilita a interrupo */
	    //NVIC_Init(&nvicStructure); /* Inicia a estrutura de interrupo */

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

        // Clear interrupt flag
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}



/*//le valor da porta GPIOA3
void ReadAnalog(){
	//le valor analogico
	//calcula valor por conversão
	//manda valor p SetPWM()
}
*/

// Inicia a conversao do ADC
int ADC_Update()
{
	 ADC_SoftwareStartConv(ADC1);

	 while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)){
		 __WFI();
	 }
	 return ADC_GetConversionValue(ADC1);

}

// Configura ADC
void ADC_Init()
{
	 ADC_InitTypeDef ADC_init_structure;
	 GPIO_InitTypeDef GPIO_initStructre;
	 // Configura Clock
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	 RCC_APB2PeriphClockLPModeCmd(RCC_APB2Periph_ADC1,DISABLE);
	 RCC_AHB1PeriphClockCmd(RCC_AHB1ENR_GPIOCEN,ENABLE);
	 // Configuracao do pino para analogico
	 GPIO_initStructre.GPIO_Pin = GPIO_Pin_0;/* O canal 10 - pino PC0 */
	 GPIO_initStructre.GPIO_Mode = GPIO_Mode_AN; /* Configurado como analgico */
	 GPIO_initStructre.GPIO_PuPd = GPIO_PuPd_NOPULL;
	 GPIO_Init(GPIOC,&GPIO_initStructre);

	 ADC_DeInit();
	 ADC_init_structure.ADC_DataAlign = ADC_DataAlign_Right;
	 ADC_init_structure.ADC_Resolution = ADC_Resolution_12b;
	 ADC_init_structure.ADC_ContinuousConvMode = ENABLE;
	 ADC_init_structure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	 ADC_init_structure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	 ADC_init_structure.ADC_NbrOfConversion = 1;
	 ADC_init_structure.ADC_ScanConvMode = DISABLE;
	 ADC_Init(ADC1,&ADC_init_structure);


	 // Escolhe o canal e o sampling time
	 ADC_RegularChannelConfig(ADC1,ADC_Channel_10,1,ADC_SampleTime_144Cycles);
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

	/*
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_DOWN;
	GPIO_Init(GPIOB, &GPIO_InitStructure);*/

	/* Infinite loop */

	while (1){
		i++;
		//PWM(4200);
		TIM4->CCR3 = 4200;
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
