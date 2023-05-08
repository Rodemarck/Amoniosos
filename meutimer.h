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
#define STEP_SIZE 64
unsigned long current_time = 0;

int lixo;
int tipo = TIPO_QUAD;
int modo = MODO_GERA;
int val;

unsigned long timer_step = 10;
unsigned long timer_func = 0;

char timer_buff_tick = 0;
int index = 0;


//valores de senos com 64 passos (a cada 18�)
//e ser�o multiplicados a tens�o maxima para
//fazer a tens�o pico a pico variar
const long sen_ref [] = {204750,224819,244694,264185,283104,301268,318503,334642,349530,363023,374993,385323,393914,400683,405565,408514,409500,408514,405565,400683,393914,385323,374993,363023,349530,334642,318503,301268,283104,264185,244694,224819,204750,184680,164805,145314,126395,108231,90996,74857,59969,46476,34506,24176,15585,8816,3934,985,0,985,3934,8816,15585,24176,34506,46476,59969,74857,90996,108231,126395,145314,164805,184680};
const long qua_ref [] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,409500,409500,409500,409500,409500,409500,409500,409500,409500,409500,409500,409500,409500,409500,409500,409500,409500,409500,409500,409500,409500,409500,409500,409500,409500,409500,409500,409500,409500,409500,409500,409500};
const long tri_ref [] = {0,12800,25600,38400,51200,64000,76800,89600,102400,115200,128000,140800,153600,166400,179200,192000,204800,217600,230400,243200,256000,268800,281600,294400,307200,320000,332800,345600,358400,371200,384000,396800,409600,396800,384000,371200,358400,345600,332800,320000,307200,294400,281600,268800,256000,243200,230400,217600,204800,192000,179200,166400,153600,140800,128000,115200,102400,89600,76800,64000,51200,38400,25600,12800,0};

//valores inteiros de referencia serem colocados
int values[STEP_SIZE] = {0};


void set_tipo(int t,int count, int divisor_tensao){

    timer_step = count;
    timer_func = current_time + timer_step;
    unsigned int x;
    switch(t){
    case TIPO_SENO:
        for(x=0; x < STEP_SIZE ; x++){
            values[x] = (int) (sen_ref[x] / divisor_tensao);
        }
        break;
    case TIPO_QUAD:
        for(x = 0 ; x < STEP_SIZE; x++) {
            values[x] = (int) (qua_ref[x] / divisor_tensao);
        }
        break;
    case TIPO_TRIA:
        for(x = 0 ; x < STEP_SIZE; x++) {
            values[x] = (int) (tri_ref[x] / divisor_tensao);
        }
        break;
    }

    __no_operation();

}

void _desenha(){

    if(current_time > timer_func ){
        timer_func+= timer_step;
        ++index;
        index &= STEP_SIZE-1;
        SAC0DAT = values[index];
    }
}



void configura_timer_gerador(int freq){
    TB1CCR0 = freq;
}


void Init_timer(){

    //TB1CCTL1 = CCIFG_0 | COV_1 | CCIE_1 | CAP_0 | SCS_1 | CCIS_3 | CM_3;
    //TB1CCTL2 = CCIFG_0 | COV_1 | CCIE_1 | CAP_0 | SCS_1 | CCIS_3 | CM_3;
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

    //desabilita
    TB0CTL &= ~ (BITE | BITF);


    TB0CTL |= TBCLGRP_0 | CNTL_0 | TBSSEL_2 | ID_0 | MC_1 | TBIE_1;

    //CONTADOR PRO LED 1
    //4050
    TB0CCR0 = 40;


    //TB1CCR0 = 1500;
    set_tipo(TIPO_SENO,map_freq(500),200);
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
    ++current_time;
}

/*
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
    //timer_func_tick = 1;
    timer_func_tick = 0;
    ++index;
    index &= STEP_SIZE-1;
    SAC0DAT = values[index];
}

*/
#endif /* MEUTIMER_H_ */
