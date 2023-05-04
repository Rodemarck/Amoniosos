/*
 * data_logger.h
 *
 *  Created on: Apr 30, 2023
 *      Author: david
 */

#ifndef DATA_LOGGER_H_
#define DATA_LOGGER_H_

#include <msp430.h>
#include <stdio.h>
#include <stdlib.h>
#include "meuio.h"
#include "meutimer.h"

#define BUFFER_LENGTH 64

uint16_t buffer_0[BUFFER_LENGTH];
uint16_t buffer_1[BUFFER_LENGTH];

volatile int buffer_0_index, buffer_1_index;

void Init_ADC(){
    ADCENC = 0
    ADCCTL0 |= ADCSHT_2;            // 16 ADCCLK cycles
    ADCCTL0 |= ADCON;               // ADC ON
    ADCCTL1 |= ADCSHP;              // ample-and-hold pulse-mode select  ==>> ADCCLK = MODOSC; sampling timer
    ADCCTL2 &= ~ADCRES;             // clear ADCRES in ADCCTL
    ADCCTL2 |= ADCRES_2;            // 12-bit conversion results
    // avaliar portas
    ADCMCTL0 |= ADCINCH_3;          // ADC input Channel  Port 1.3
    //  ADCMCTL0 |= ADCINCH_6;          // ADC input Channel  Port 1.6
    
    ADCIE |= ADCIE0;                // Enable the Interrupt request for a completed ADC_B conversion
    ADCCTL0 |= ADCENC;              // Enable conversions
    ADCCTL0 |= ADCSC;               // Start conversion
    ADCCTL0 |= ADCMSC;

    __bis_SR_register(LPM0_bits | GIE);                
    __no_operation();

    PMMCTL0_H = PMMPW_H;                                        
    PMMCTL2 |= INTREFEN;  
    ADCENC = 1
}


void ADC_ler_canal_0(){

}

void ADC_escreve_canal_0(){

}

void ADC_interpreta_canal_0(){

}

void ADC_ler_canal_1(){

}

void ADC_escreve_canal_1(){

}

void ADC_interpreta_canal_1(){

}


#endif /* DATA_LOGGER_H_ */
