/*
 * Created: 4/14/2024 10:21:27 PM
 * Author: Jake
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "helper_funcs.h"

extern void clock_init(void);
volatile uint8_t usart_flag = 0;
char in_data;

uint16_t tableChoice = lookupTable;

// DMA initialization for transferring waveform data to the DAC
void dma_init(void){
	// DMA reset
	DMA.CTRL |= DMA_RESET_bm;
	
	// One block = entire array, need 2 bytes per burst
	DMA.CH0.CTRLA = DMA_CH_SINGLE_bm | DMA_CH_BURSTLEN_2BYTE_gc | DMA_CH_REPEAT_bm;
	
	// Source address is beginning of array, increment on each trigger, repeat it back to first address after block finish
	DMA.CH0.ADDRCTRL = DMA_CH_SRCRELOAD_BLOCK_gc | DMA_CH_SRCDIR_INC_gc | DMA_CH_DESTDIR_INC_gc | DMA_CH_DESTRELOAD_BURST_gc;
	
	// Set trigger source to event channel 0
	DMA.CH0.TRIGSRC = DMA_CH_TRIGSRC_EVSYS_CH0_gc;
	
	// Set size of block
	DMA.CH0.TRFCNT = (uint16_t)(512);
	
	// Set source and destination addresses
	DMA.CH0.SRCADDR0 = (uint8_t)((uintptr_t)tableChoice);
	DMA.CH0.SRCADDR1 = (uint8_t)(((uintptr_t)tableChoice) >> 8);
	DMA.CH0.SRCADDR2 = (uint8_t)((uint32_t)((uintptr_t)tableChoice) >> 16);
	
	DMA.CH0.DESTADDR0 = (uint8_t)((uintptr_t)&DACA.CH1DATA);
	DMA.CH0.DESTADDR1 = (uint8_t)(((uintptr_t)&DACA.CH1DATA) >> 8);
	DMA.CH0.DESTADDR2 = (uint8_t)((uint32_t)((uintptr_t)&DACA.CH1DATA) >> 16);

	// Enable DMA
	DMA.CH0.CTRLA |= DMA_CH_ENABLE_bm;
	DMA.CTRL |= DMA_ENABLE_bm; // |= is important to preserve other bits in register
}

// USART receive interrupt service routine
ISR(USARTD0_RXC_vect){
	USARTD0.CTRLA = USART_RXCINTLVL_OFF_gc;
	usart_flag = 1;
	in_data = USARTD0.DATA;
	if (usart_flag){
		usart_flag = 0;
		
		// Change frequency based on received character
		if (in_data == 'e'){
			TCC1.CNT = 0;
			TCC0.PER = 121;
			TCC0.CTRLA = TC_CLKSEL_DIV1_gc;
		}
		else if (in_data == '4'){
			TCC1.CNT = 0;
			TCC0.PER = 114;
			TCC0.CTRLA = TC_CLKSEL_DIV1_gc;
		}
		else if (in_data == 'r'){
			TCC1.CNT = 0;
			TCC0.PER = 108;
			TCC0.CTRLA = TC_CLKSEL_DIV1_gc;
		}
		else if (in_data == '5'){
			TCC1.CNT = 0;
			TCC0.PER = 102;
			TCC0.CTRLA = TC_CLKSEL_DIV1_gc;
		}
		else if (in_data == 't'){
			TCC1.CNT = 0;
			TCC0.PER = 96;
			TCC0.CTRLA = TC_CLKSEL_DIV1_gc;
		}
		else if (in_data == 'y'){
			TCC1.CNT = 0;
			TCC0.PER = 91;
			TCC0.CTRLA = TC_CLKSEL_DIV1_gc;
		}
		else if (in_data == '7'){
			TCC1.CNT = 0;
			TCC0.PER = 86;
			TCC0.CTRLA = TC_CLKSEL_DIV1_gc;
		}
		else if (in_data == 'u'){
			TCC1.CNT = 0;
			TCC0.PER = 81;
			TCC0.CTRLA = TC_CLKSEL_DIV1_gc;
		}
		else if (in_data == '8'){
			TCC1.CNT = 0;
			TCC0.PER = 76;
			TCC0.CTRLA = TC_CLKSEL_DIV1_gc;
		}
		else if (in_data == 'i'){
			TCC1.CNT = 0;
			TCC0.PER = 72;
			TCC0.CTRLA = TC_CLKSEL_DIV1_gc;
		}
		else if (in_data == '9'){
			TCC1.CNT = 0;
			TCC0.PER = 68;
			TCC0.CTRLA = TC_CLKSEL_DIV1_gc;
		}
		else if (in_data == 'o'){
			TCC1.CNT = 0;
			TCC0.PER = 64;
			TCC0.CTRLA = TC_CLKSEL_DIV1_gc;
		}
		else if (in_data == 'p'){
			TCC1.CNT = 0;
			TCC0.PER = 60;
			TCC0.CTRLA = TC_CLKSEL_DIV1_gc;
		}
		else if (in_data == 's'){
			// Switch waveform table
			if (tableChoice == lookupTable){
				tableChoice = triangleTable;
				dma_init();
			}
			else{
				tableChoice = lookupTable;
				dma_init();
			}
		}
	}
	TCC0.CNT = 0;
	USARTD0.CTRLA = USART_RXCINTLVL_MED_gc;
}

// Timer overflow interrupt service routine
ISR(TCC1_OVF_vect){
	TCC0.CTRLA = TC_CLKSEL_OFF_gc;
}

int main(void)
{
	// Initialize peripherals
	clock_init();
	usart_init();
	dac_init_ch1();
	tcc0_init();
	tcc1_init();
	
	DMA.CH0.CTRLA |= DMA_CH_TRFREQ_bm;
	
	PORTC.DIRSET = 0b10000000; // Pin 7 is shutdown for speaker
	PORTC.OUTSET = 0b10000000;
	
	while (1)
	{
		// Main loop does nothing, waiting for interrupts
	}
}
