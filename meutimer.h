/*
 * timer.h
 *
 *  Created on: 4 de abr de 2023
 *      Author: Rodemarck
 *
 *  Está muito acoplado eu sei e um dia melhora.
 *  Este arquivo está responsável pela configuração
 *  dos timers e utilizando suas interupções para
 *  fazer o gerador de função.
 *
 *  Notem que Existe um jeito melhor para fazer tudo isso
 *  que é por tudo como função de timer, o led piscando como
 *  PWM, e utilziar combo de maneira decente (dá para colocar
 *  interrupção do DAC como uma função do timer) mas primeiro
 *  se faz o caminho feliz.
 *
 *  As configurações do DAC estão no arquivo meuio, na função
 *  Init_GPIO.
 *
 *  A função Init_timer configura os timers que estamos utilizando
 *  e como não estamos utilizando funções de timer, apenas suas
 *  interrupções da maneira mais porca, porém fácil, possivel, só
 *  precisamos "definir o tempo a ser temporizado".
 *
 *  O led piscando a 1 hz está atrelada ao timer b0 e eu defini
 *  o led 1 para o gerador e o led 2 para o buffer. Como a freq
 *  e tudo mais é o mesmo então  vamos emular uma maquina de
 *  estados na interupção do timer com um switch. Ou seja, se
 *  estiver no estado X vai fazer x, se eu mudar o estado para
 *  Y vai fazer y. Vide a interupção B1 do timer B0 (penultimo trecho
 *  de código do arquivo e eu sei que o nome é uma bosta, mas fazer o q?).
 *
 *  Deve haver 3 tipos de funções e elas tem que operar no mesmo
 *  pino, e para isso tbm fiz uma emualação cagada de uma maquina
 *  de estados que está na interrupção B1 do timer B1. Eu decidi
 *  que cada função vai ter 20 valores(fonte: vozes da minha cabeça)
 *  e ai eu só preciso pegar a freq e multiplicar por 20(já que são
 *  20 passos por função), ou seja dimunir o registrador de controle
 *  do contador do timer B1,(Deveria ser o TB1CL0, mas TB1CCR0 é um
 *  registrador que transfere o valor para TB1CL0 com algum tipo de
 *  tratamento baseado no clock escolhido e suas divisões, segundo o
 *  datasheet, não vejo a necessidade mas se existe tem motivo)
 *
 *
 */

#ifndef MEUTIMER_H_
#define MEUTIMER_H_
#include "time.h"

#define MODO_GERA 0
#define MODO_BUFF 1
#define TIPO_QUAD 0
#define TIPO_TRIA 1
#define TIPO_SENO 2

int lixo;
int tipo = TIPO_QUAD;
int modo = MODO_GERA;
int val;

char timer_func_tick = 0;
char timer_led_tick = 0;
char timer_buff_tick = 0;
int index = 0;

//valores de senos com 20 passos (a cada 18°)
//e serão multiplicados a tensão maxima para
//fazer a tensão pico a pico variar
const long sen_ref[] = {
                       204800,
                       268000,
                       325100,
                       370400,
                       399500,
                       409500,
                       399500,
                       370400,
                       325100,
                       268000,
                       204800,
                       141500,
                       84400,
                       39100,
                       10000,
                       0,
                       10000,
                       39100,
                       84400,
                       141500,
                       204800
};



//valores inteiros de referencia serem colocados
int sen_value[20] = {0};
int tri_value[20] = {0};
int qua_value[20] = {0};


void set_tipo(int t,int count, int divisor_tensao){
    TB1CCR0 = count;
    tipo = t;
    unsigned int x;
    switch(t){
    case TIPO_SENO:
        for(x=0; x < 20 ; x++){
            sen_value[x] = (int) (sen_ref[x] / divisor_tensao);
        }
        break;
    case TIPO_QUAD:
        //TODO
        break;
    case TIPO_TRIA:
        //TODO
        break;
    }

    __no_operation();

}

void _desenha(){
    if(timer_func_tick){
        timer_func_tick = 0;
        ++index;
        if(index>=20)
            index = 0;

        switch(tipo){
        case TIPO_QUAD:
            SAC0DAT = qua_value[index];
            break;
        case TIPO_TRIA:
            SAC0DAT = tri_value[index];
            break;
        case TIPO_SENO:
            SAC0DAT = sen_value[index];
            break;
        }
    }
}



void configura_timer_gerador(int freq){
    TB1CCR0 = freq;
}


void Init_timer(){
    /*
         *    bits configuration for TB0CTL register
         *
                TBCLGRP_0 //Each TBxCLn latch loads independently
                TBCLGRP_1
                TBCLGRP_2
                TBCLGRP_3
                | // Counter length
                CNTL_0 // 16-bit, TBxR(max) = 0FFFFh
                CNTL_1 //12-bit, TBxR(max) = 0FFFh
                CNTL_2 //10-bit, TBxR(max) = 03FFh
                CNTL_3 // 8-bit, TBxR(max) = 0FFh
                | // Timer_B clock source select
                TBSSEL_0 //TBxCLK
                TBSSEL_1 //ACLK
                TBSSEL_2 //SMCLK
                TBSSEL_3 //INCLK
                | // input divider. These bits, along with the TBIDEX bits, select the divider for the input clock
                ID_0 // Div 1
                ID_1 // Div 2
                ID_2 // Div 4
                ID_3 // Div 8
                | // Mode control. Set MC = 00h when Timer_B is not in use to conserve power.
                MC_0 // Stop mode: Timer is halted
                MC_1 // Up mode: Timer counts up to TBxCL0
                MC_2 // Continuous mode: Timer counts up to the value set by CNTL
                MC_3 // Up/down mode: Timer counts up to TBxCL0 and down to 0000h
                | TBCLR // Timer_B clear. Setting this bit clears TBxR, the clock divider logic (the divider setting remains unchanged), and the count direction
                | // Timer_B interrupt enable.
                TBIE_0 // interrupt disable.
                TBIE_1 // interrupt enable.
        */

    //timer 0 config
    TB0CTL |= TBCLGRP_0 | CNTL_0 | TBSSEL_1 | ID_3 | MC_1 | TBIE_1;
    //timer 1 config
    TB1CTL |= TBCLGRP_0 | CNTL_0 | TBSSEL_2 | ID_0 | MC_2 | TBIE_1;

    //CONTADOR PRO LED 1
    TB0CCR0 = 65535;


    set_tipo(TIPO_SENO,600,100);
}













// interrupção do timer 0
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER0_B1_VECTOR
__interrupt void interrupcao_B1_timer_b0 (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_B1_VECTOR))) Timer_B1 (void)
#else
#error Compiler not supported!
#endif
{
    lixo = TB0IV;
    timer_led_tick = 1;
}


// interrupção do timer 1
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER1_B1_VECTOR
__interrupt void interrupcao_B1_timer_b1 (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER1_B1_VECTOR))) Timer1_B1 (void)
#else
#error Compiler not supported!
#endif
{
    lixo = TB1IV;
    timer_func_tick = 1;
    /*switch(__even_in_range(TB1IV,TB1IV_TBIFG)){
    case TB1IV_NONE:
        break;
    case TB1IV_TB1CCR1:
        __no_operation();
        timer_func_tick = 1;
        break;
    case TB1IV_TB1CCR2:
        __no_operation();
        timer_func_tick = 1;
        break;
    case TB1IV_TBIFG:
        __no_operation();
        timer_func_tick = 1;
        break;
    }*/
}


#endif /* MEUTIMER_H_ */
