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

//Exemplo de fun��o
void led(char* t){
//    estra��o das variaveis
    int led, ligado;

//    O sscanf � praticamente como um printf invertido, ao inves de
//    passar os valores depois da string, passamos os endere�os onde
//    ser�o salvos. e como ele s� vai passar os valores de %d, podemos
//    escrever o nome da fun��o e ele vai ignorar
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


//Fun��o simples de eco, repete o texto que foi mandado
void eco(char* t){
    print("I.A. responde : \"", 1);
    print(t+4, 1);
    print("\"\n\r", 1);
}


//TODO fazer comando de desenhar fun��o quadrada


//TODO fazer comando de desenhar fun��o triangular


//TODO fazer comando de desenhar fun��o senoide

//TODO fazer comando de leitura dos pinos




#endif /* FUNCOES_H_ */
