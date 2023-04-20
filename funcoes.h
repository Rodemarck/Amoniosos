/*
 * funcoes.h
 *
 *  Created on: 11 de mar de 2023
 *      Author: rodem
 *
 *  Está muito acoplado eu sei e um dia melhora.
 *  Este arquivo está responsável pelo controle do projeto.
 *  Ou seja aqui está o tratamento de comandos e tudo mais.
 *
 *  O contador para a frequencia de 1KHz foi achada no chute
 *  e regra de 3 depois que achamos algo útil.
 *
 *  O fator de tensão deveria encontrado uma regra de 3 que faz
 *  sim sentido. Como o nosso SACDAC possui precisãod e 12 bits
 *  então pode representar valores de 0 a 4095 e sendo a
 *  voltagem maxima 3.3v, ou seja 3330mV
 *
 *  4095 - 3300
 *   x   - 1
 *
 *   3300x = 4095
 *   x = 4095/3300
 *   x = 1,24
 *
 *   Eu tentei fazer um sistema de funções com hash e tudo mais
 *   mas era inútil, melhor encadear a porcaria de uma estrutura
 *   de if-else utilizando strcmp.
 *
 */

#ifndef FUNCOES_H_
#define FUNCOES_H_

#include <msp430.h>
#include <stdio.h>
#include <stdlib.h>
#include "meuio.h"
#include "meutimer.h"

//frequencia base 1KHz (encontrada por multiplas tentativas e erros
//além de uma regra de 3 torta)
#define FREQ_BASE 4004

//frequencia base para as funções, deveria ser 20, mas devido a problemas
//de atenção cheguei a /10 que bateu
#define FREQ_FUNC FREQ_BASE/10

//Fator de multiplicação de tensão
#define FATOR_TENSAO 1.24f

//tamanho máximo dos comandos, pq sim!
#define TAMANHO_COMANDO 16

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
//Totalmente inútil, mas exemplo é exemplo.
void eco(char* t){
    print("I.A. responde : \"", 1);
    print(t+4, 1);
    print("\"\n\r", 1);
}

//Transforma a fequencia desejada em número para ser
//contado pelo contador do timer de maneira a chegar
//nesa frequencia
int map_freq(int freq){
    return (int)((float)FREQ_FUNC / freq);
}

//Mesma coisa do map_freq, só que para a tensão
int map_amp(int amp){
    return (int) (amp * FATOR_TENSAO);
}

//Função que muda a função gerada para onda quadrada
void quad(char *t){
    int freq, amp;
    sscanf(t,"quad%d%d", &freq, &amp);
    set_tipo(TIPO_QUAD, map_freq(freq), map_amp(amp));
}

//Mesma coisa só que para onda senoidal
void sen(char *t){
    int freq, amp;
    sscanf(t,"sen%d%d", &freq, &amp);
    set_tipo(TIPO_SENO, map_freq(freq), map_amp(amp));
}

//Idem, para triangular
void tri(char *t){
    int freq, amp;
    sscanf(t,"tri%d%d", &freq, &amp);
    set_tipo(TIPO_TRIA, map_freq(freq), map_amp(amp));
}

//procura o comando na lista de comandos, considerando a primeira palavra
void procura_comando(char *texto){
    char comando[TAMANHO_COMANDO];

//    scanf de string, pegar a primeira palavra do texto e salvar na string
//    comando.
    sscanf(texto,"%s",comando);


    if(0 == strcmp(comando,"sen"))
        sen(texto);
    else if(0 == strcmp(comando,"quad"))
        quad(texto);
    else if(0 == strcmp(comando,"tri"))
        tri(texto);
    else if(0 == strcmp(comando,"led"))
        led(texto);
    else if(0 == strcmp(comando,"eco"))
        eco(texto);
    else{//Server para debug
        print("Comando não encontrado [", 1);
        print(comando, 1);
        print("]\n\r", 1);
    }
}



#endif /* FUNCOES_H_ */
