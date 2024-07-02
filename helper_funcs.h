/*
 * helper_funcs.h
 *
 * Created: 4/14/2024 12:12:15 AM
 *  Author: Jake
 */ 
#ifndef HELPER_FUNCS_H_
#define HELPER_FUNCS_H_

#include <avr/io.h>

// Sine wave lookup table for DAC waveform generation
extern uint16_t lookupTable[256];

// Triangle wave lookup table for DAC waveform generation
extern uint16_t triangleTable[256];

// Initialize DAC for single channel operation on channel 0
void dac_init(void);

// Initialize DAC for single channel operation on channel 1
void dac_init_ch1(void);

// Initialize Timer/Counter 0 for waveform generation
void tcc0_init(void);

// Initialize Timer/Counter 1 for overflow interrupt
void tcc1_init(void);

// Initialize USART for serial communication
void usart_init(void);

#endif
