/*
 * timer.h
 *
 *  Created on: 4 de abr de 2023
 *      Author: Rodemarck
 *
 *  Est� muito acoplado eu sei e um dia melhora.
 *  Este arquivo est� respons�vel pela configura��o
 *  dos timers e utilizando suas interup��es para
 *  fazer o gerador de fun��o.
 *
 *  Notem que Existe um jeito melhor para fazer tudo isso
 *  que � por tudo como fun��o de timer, o led piscando como
 *  PWM, e utilziar combo de maneira decente (d� para colocar
 *  interrup��o do DAC como uma fun��o do timer) mas primeiro
 *  se faz o caminho feliz.
 *
 *  As configura��es do DAC est�o no arquivo meuio, na fun��o
 *  Init_GPIO.
 *
 *  A fun��o Init_timer configura os timers que estamos utilizando
 *  e como n�o estamos utilizando fun��es de timer, apenas suas
 *  interrup��es da maneira mais porca, por�m f�cil, possivel, s�
 *  precisamos "definir o tempo a ser temporizado".
 *
 *  O led piscando a 1 hz est� atrelada ao timer b0 e eu defini
 *  o led 1 para o gerador e o led 2 para o buffer. Como a freq
 *  e tudo mais � o mesmo ent�o  vamos emular uma maquina de
 *  estados na interup��o do timer com um switch. Ou seja, se
 *  estiver no estado X vai fazer x, se eu mudar o estado para
 *  Y vai fazer y. Vide a interup��o B1 do timer B0 (penultimo trecho
 *  de c�digo do arquivo e eu sei que o nome � uma bosta, mas fazer o q?).
 *
 *  Deve haver 3 tipos de fun��es e elas tem que operar no mesmo
 *  pino, e para isso tbm fiz uma emuala��o cagada de uma maquina
 *  de estados que est� na interrup��o B1 do timer B1. Eu decidi
 *  que cada fun��o vai ter 20 valores(fonte: vozes da minha cabe�a)
 *  e ai eu s� preciso pegar a freq e multiplicar por 20(j� que s�o
 *  20 passos por fun��o), ou seja dimunir o registrador de controle
 *  do contador do timer B1,(Deveria ser o TB1CL0, mas TB1CCR0 � um
 *  registrador que transfere o valor para TB1CL0 com algum tipo de
 *  tratamento baseado no clock escolhido e suas divis�es, segundo o
 *  datasheet, n�o vejo a necessidade mas se existe tem motivo)
 *
 *
 */

#ifndef MEUTIMER_H_
#define MEUTIMER_H_
#include "time.h"

#include "funcoes.h"
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

//valores de senos com 20 passos (a cada 18�)
//e ser�o multiplicados a tens�o maxima para
//fazer a tens�o pico a pico variar
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

const long qua_ref[] = {
    0, 409500, 0, 409500, 0, 409500, 0, 409500, 0, 409500, 0, 409500, 0, 409500, 0, 409500, 0, 409500, 0, 409500, 0
};

const long tri_ref[] = {
    0,
    41000,
    81900,
    122900,
    163800,
    204800,
    245700,
    286700,
    327600,
    368600,
    409500,
    368600,
    327600,
    286700,
    245700,
    204800,
    163800,
    122900,
    81900,
    41000,
    0
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
        for(x = 0 ; x < 20; x++) {
            qua_value[x] = (int) (qua_ref[x] / divisor_tensao);
        }
        break;
    case TIPO_TRIA:
        for(x = 0 ; x < 20; x++) {
            tri_value[x] = (int) (tri_ref[x] / divisor_tensao);
        }
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
    TB1CTL |= TBCLGRP_0 | CNTL_0 | TBSSEL_2 | ID_0 | MC_1 | TBIE_1;

    //CONTADOR PRO LED 1
    TB0CCR0 = 1500;

    TB1CCR0 = 1500;
    set_tipo(TIPO_SENO,map_freq(1),100);
}













// interrup��o do timer 0
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


// interrup��o do timer 1
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
