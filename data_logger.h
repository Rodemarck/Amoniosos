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
    // DEFINIR ADC (OLHAR DOCUMENTACAO)

    //

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
