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

//Exemplo de função
void led(char* t){
//    estração das variaveis
    int led, ligado;

//    O sscanf é praticamente como um printf invertido, ao inves de
//    passar os valores depois da string, passamos os endereços onde
//    serão salvos. e como ele só vai passar os valores de %d, podemos
//    escrever o nome da função e ele vai ignorar
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


//Função simples de eco, repete o texto que foi mandado
void eco(char* t){
    print("I.A. responde : \"", 1);
    print(t+4, 1);
    print("\"\n\r", 1);
}


//TODO fazer comando de desenhar função quadrada


//TODO fazer comando de desenhar função triangular


//TODO fazer comando de desenhar função senoide

//TODO fazer comando de leitura dos pinos




#endif /* FUNCOES_H_ */
