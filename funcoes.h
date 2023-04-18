/*
 * funcoes.h
 *
 *  Created on: 11 de mar de 2023
 *      Author: rodem
 */

#ifndef FUNCOES_H_
#define FUNCOES_H_

#include <msp430.h>
#include <stdio.h>
#include <stdlib.h>
#include "meuio.h"
#include "meutimer.h"

#define FREQ_MIN 520
#define FREQ_MAX 52

void led(char* t){
    int led, ligado;
    sscanf(t,"led%d%d",&led, &ligado);
    switch(led){
        case 1:
            if(ligado ==1)
                P1OUT |= BIT0;
            else if(ligado == 0)
                P1OUT &= ~BIT0;
            else
                P1OUT ^= BIT0;
            break;
        case 2:
                if(ligado ==1)
                    P6OUT |= BIT6;
                else if(ligado == 0)
                    P6OUT &= ~BIT6;
                else
                    P6OUT ^= BIT6;
                break;
    }
}
void eco(char* t){
    char arg[64];
    strcpy(arg,t+4);
    print("I.A. responde : \"", 1);
    print(arg, 1);
    print("\"\n\r", 1);
}

int map_freq(int freq){
    if(freq <1)
        return FREQ_MIN;
    if(freq > 10)
        return FREQ_MAX;
    return (int)((float)FREQ_MIN / freq);
}

void quad(char *t){
    int freq, amp;
    sscanf(t,"quad%d%d", &freq, &amp);
    configura_timer_gerador(map_freq(freq));
}



#endif /* FUNCOES_H_ */
