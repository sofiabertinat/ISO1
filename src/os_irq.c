/*
 * os_irq.c
 *
 *  Created on: Aug 13, 2022
 *      Author: Sofia Bertinat
 */


#include "../inc/os_irq.h"
#include "../inc/os_core.h"
#include "../inc/os_hook.h"

#define V_IRQ_SIZE 50

void* v_isr[V_IRQ_SIZE];
uint8_t param_isr[V_IRQ_SIZE];


/* */
void os_isr_init(void)
{
	int i;
	for(i=0;i<V_IRQ_SIZE;i++)
	{
		v_isr[i] = NULL;
		param_isr[i] = 0;
	}
}

/**/
void os_my_IRQ_Init (void)
{
		/* Se inicializan las interrupciones (LPCopen) */
		Chip_PININT_Init(LPC_GPIO_PIN_INT);

		/* Inicializacion de cada evento de interrupcion (LPCopen) */

		/* TEC1 FALL */
		Chip_SCU_GPIOIntPinSel(0, 0, 4); 							/*(Canal 0 a 7, Puerto GPIO, Pin GPIO) */
		Chip_PININT_ClearIntStatus( LPC_GPIO_PIN_INT, PININTCH0);
		Chip_PININT_SetPinModeEdge(LPC_GPIO_PIN_INT, PININTCH0);	/* Se configura el canal para que se active por flanco */
		Chip_PININT_EnableIntLow(LPC_GPIO_PIN_INT, PININTCH0);		/* Se configura para que el flanco sea el de bajada */

		/* TEC1 RISE */
		Chip_SCU_GPIOIntPinSel(1, 0, 4);							/* (Canal 0 a 7, Puerto GPIO, Pin GPIO) */
		Chip_PININT_ClearIntStatus( LPC_GPIO_PIN_INT, PININTCH1);
		Chip_PININT_SetPinModeEdge(LPC_GPIO_PIN_INT, PININTCH1);	/* Se configura el canal para que se active por flanco */
		Chip_PININT_EnableIntHigh(LPC_GPIO_PIN_INT, PININTCH1);		/* En este caso el flanco es de subida */

		/* TEC2 FALL */
		Chip_SCU_GPIOIntPinSel(2, 0, 8);
		Chip_PININT_ClearIntStatus( LPC_GPIO_PIN_INT, PININTCH2);
		Chip_PININT_SetPinModeEdge(LPC_GPIO_PIN_INT, PININTCH2);
		Chip_PININT_EnableIntLow(LPC_GPIO_PIN_INT, PININTCH2);

		/* TEC2 RISE */
		Chip_SCU_GPIOIntPinSel(3, 0, 8);
		Chip_PININT_ClearIntStatus( LPC_GPIO_PIN_INT, PININTCH3);
		Chip_PININT_SetPinModeEdge(LPC_GPIO_PIN_INT, PININTCH3);
		Chip_PININT_EnableIntHigh(LPC_GPIO_PIN_INT, PININTCH3);

		/* TEC3 FALL */
		Chip_SCU_GPIOIntPinSel(4, 0, 9);
		Chip_PININT_ClearIntStatus( LPC_GPIO_PIN_INT, PININTCH4);
		Chip_PININT_SetPinModeEdge(LPC_GPIO_PIN_INT, PININTCH4);
		Chip_PININT_EnableIntLow(LPC_GPIO_PIN_INT, PININTCH4);

		/* TEC3 RISE */
		Chip_SCU_GPIOIntPinSel(5, 0, 9);
		Chip_PININT_ClearIntStatus( LPC_GPIO_PIN_INT, PININTCH5);
		Chip_PININT_SetPinModeEdge(LPC_GPIO_PIN_INT, PININTCH5);
		Chip_PININT_EnableIntHigh(LPC_GPIO_PIN_INT, PININTCH5);

		/* TEC4 FALL */
		Chip_SCU_GPIOIntPinSel(6, 1, 9);
		Chip_PININT_ClearIntStatus( LPC_GPIO_PIN_INT, PININTCH6);
		Chip_PININT_SetPinModeEdge(LPC_GPIO_PIN_INT, PININTCH6);
		Chip_PININT_EnableIntLow(LPC_GPIO_PIN_INT, PININTCH6);

		/* TEC4 RISE */
		Chip_SCU_GPIOIntPinSel(7, 1, 9);
		Chip_PININT_ClearIntStatus( LPC_GPIO_PIN_INT, PININTCH7);
		Chip_PININT_SetPinModeEdge(LPC_GPIO_PIN_INT, PININTCH7);
		Chip_PININT_EnableIntHigh(LPC_GPIO_PIN_INT, PININTCH7);

		os_isr_init();
}

/* */
void os_isr_register(LPC43XX_IRQn_Type irq, void* pIsr, uint8_t param)
{
	v_isr[irq] = pIsr;
	param_isr[irq] = param;
	NVIC_ClearPendingIRQ(irq);
	NVIC_EnableIRQ(irq);
}

/* */
void os_isr_unregister( LPC43XX_IRQn_Type irq)
{
	v_isr[irq] = NULL;
	param_isr[irq] = 0;
	NVIC_ClearPendingIRQ(irq);
	NVIC_DisableIRQ(irq);
}

/* */
void os_IRQHandler(LPC43XX_IRQn_Type irq)
{
	bool (*fun_isr)(void *param);
	static bool rescheduler = FALSE;

	rescheduler = FALSE;

	if(v_isr[irq] != NULL)
	{
		os_set_state(OS_IRQ);

		fun_isr = v_isr[irq];

		rescheduler = fun_isr(&param_isr[irq]);

		os_set_previous_state();

		NVIC_ClearPendingIRQ(irq);


		if (rescheduler)
		{
			os_cpu_yield();
		}
	}
	else
	{
		NVIC_ClearPendingIRQ(irq);
		errorHook(NULL, ISR_ERROR);
	}
}

/*==================[interrupt service routines]=============================*/

void DAC_IRQHandler(void)
{
	os_IRQHandler(DAC_IRQn);
}

void M0APP_IRQHandler(void)
{
	os_IRQHandler(M0APP_IRQn);
}

void DMA_IRQHandler(void)
{
	os_IRQHandler( DMA_IRQn);
}

void FLASH_EEPROM_IRQHandler(void)
{
	os_IRQHandler(RESERVED1_IRQn);
}

void ETH_IRQHandler(void)
{
	os_IRQHandler(ETHERNET_IRQn);
}

void SDIO_IRQHandler(void)
{
	os_IRQHandler(SDIO_IRQn);
}

void LCD_IRQHandler(void)
{
	os_IRQHandler(LCD_IRQn);
}

void USB0_IRQHandler(void)
{
	os_IRQHandler(USB0_IRQn);
}

void USB1_IRQHandler(void)
{
	os_IRQHandler(USB1_IRQn);
}

void SCT_IRQHandler(void)
{
	os_IRQHandler(SCT_IRQn);
}

void RIT_IRQHandler(void)
{
	os_IRQHandler(RITIMER_IRQn);
}

void TIMER0_IRQHandler(void)
{
	os_IRQHandler(TIMER0_IRQn);
}

void TIMER1_IRQHandler(void)
{
	os_IRQHandler(TIMER1_IRQn);
}

void TIMER2_IRQHandler(void)
{
	os_IRQHandler(TIMER2_IRQn);
}

void TIMER3_IRQHandler(void)
{
	os_IRQHandler(TIMER3_IRQn);
}

void MCPWM_IRQHandler(void)
{
	os_IRQHandler(MCPWM_IRQn);
}

void ADC0_IRQHandler(void)
{
	os_IRQHandler(ADC0_IRQn);
}

void I2C0_IRQHandler(void)
{
	os_IRQHandler(I2C0_IRQn);
}

void SPI_IRQHandler(void)
{
	os_IRQHandler(I2C1_IRQn);
}

void I2C1_IRQHandler(void)
{
	os_IRQHandler(SPI_INT_IRQn);
}

void ADC1_IRQHandler(void)
{
	os_IRQHandler(ADC1_IRQn);
}

void SSP0_IRQHandler(void)
{
	os_IRQHandler(SSP0_IRQn);
}

void SSP1_IRQHandler(void)
{
	os_IRQHandler(SSP1_IRQn);
}

void UART0_IRQHandler(void)
{
	os_IRQHandler(USART0_IRQn);
}

void UART1_IRQHandler(void)
{
	os_IRQHandler(UART1_IRQn);
}

void UART2_IRQHandler(void)
{
	os_IRQHandler(USART2_IRQn);
}

void UART3_IRQHandler(void)
{
	os_IRQHandler(USART3_IRQn);
}

void I2S0_IRQHandler(void)
{
	os_IRQHandler(I2S0_IRQn);
}

void I2S1_IRQHandler(void)
{
	os_IRQHandler(I2S1_IRQn);
}

void SPIFI_IRQHandler(void)
{
	os_IRQHandler(RESERVED4_IRQn);
}

void SGPIO_IRQHandler(void)
{
	os_IRQHandler(SGPIO_INT_IRQn);
}

void GPIO0_IRQHandler(void)
{
	os_IRQHandler(PIN_INT0_IRQn);
}

void GPIO1_IRQHandler(void)
{
	os_IRQHandler(PIN_INT1_IRQn);
}

void GPIO2_IRQHandler(void)
{
	os_IRQHandler(PIN_INT2_IRQn);
}

void GPIO3_IRQHandler(void)
{
	os_IRQHandler(PIN_INT3_IRQn);
}

void GPIO4_IRQHandler(void)
{
	os_IRQHandler(PIN_INT4_IRQn);
}

void GPIO5_IRQHandler(void)
{
	os_IRQHandler(PIN_INT5_IRQn);
}

void GPIO6_IRQHandler(void)
{
	os_IRQHandler(PIN_INT6_IRQn);
}

void GPIO7_IRQHandler(void)
{
	os_IRQHandler(PIN_INT7_IRQn);
}

void GINT0_IRQHandler(void)
{
	os_IRQHandler(GINT0_IRQn);
}

void GINT1_IRQHandler(void)
{
	os_IRQHandler(GINT1_IRQn);
}

void EVRT_IRQHandler(void)
{
	os_IRQHandler(EVENTROUTER_IRQn);
}

void CAN1_IRQHandler(void)
{
	os_IRQHandler(C_CAN1_IRQn);
}

void ADCHS_IRQHandler(void)
{
	os_IRQHandler(ADCHS_IRQn);
}

void ATIMER_IRQHandler(void)
{
	os_IRQHandler(ATIMER_IRQn);
}

void RTC_IRQHandler(void)
{
	os_IRQHandler(RTC_IRQn);
}

void WDT_IRQHandler(void)
{
	os_IRQHandler(WWDT_IRQn);
}

void M0SUB_IRQHandler(void)
{
	os_IRQHandler(M0SUB_IRQn);
}

void CAN0_IRQHandler(void)
{
	os_IRQHandler(C_CAN0_IRQn);
}

void QEI_IRQHandler(void)
{
	os_IRQHandler(QEI_IRQn);
}
