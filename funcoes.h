/*
 * funcoes.h
 *
 *  Created on: 11 de mar de 2023
 *      Author: rodem
 *
 *  Est� muito acoplado eu sei e um dia melhora.
 *  Este arquivo est� respons�vel pelo controle do projeto.
 *  Ou seja aqui est� o tratamento de comandos e tudo mais.
 *
 *  O contador para a frequencia de 1KHz foi achada no chute
 *  e regra de 3 depois que achamos algo �til.
 *
 *  O fator de tens�o deveria encontrado uma regra de 3 que faz
 *  sim sentido. Como o nosso SACDAC possui precis�od e 12 bits
 *  ent�o pode representar valores de 0 a 4095 e sendo a
 *  voltagem maxima 3.3v, ou seja 3330mV
 *
 *  4095 - 3300
 *   x   - 1
 *
 *   3300x = 4095
 *   x = 4095/3300
 *   x = 1,24
 *
 *   Eu tentei fazer um sistema de fun��es com hash e tudo mais
 *   mas era in�til, melhor encadear a porcaria de uma estrutura
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
//al�m de uma regra de 3 torta)
#define FREQ_BASE 4004

//frequencia base para as fun��es, deveria ser 20, mas devido a problemas
//de aten��o cheguei a /10 que bateu
#define FREQ_FUNC FREQ_BASE/10

//Fator de multiplica��o de tens�o
#define FATOR_TENSAO 330000

//tamanho m�ximo dos comandos, pq sim!
#define TAMANHO_COMANDO 16

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
//Totalmente in�til, mas exemplo � exemplo.
void eco(char* t){
    print("I.A. responde : \"", 1);
    print(t+4, 1);
    print("\"\n\r", 1);
}

//Transforma a fequencia desejada em n�mero para ser
//contado pelo contador do timer de maneira a chegar
//nesa frequencia
int map_freq(int freq){
    return (int)((float)FREQ_FUNC / freq);
}

//Mesma coisa do map_freq, s� que para a tens�o
int map_amp(int amp){
    return (int) (FATOR_TENSAO/amp);
}

//Fun��o que muda a fun��o gerada para onda quadrada
void quad(char *t){
    int freq, amp;
    sscanf(t,"quad%d%d", &freq, &amp);
    freq = map_freq(freq);
    amp = map_amp(amp);
    set_tipo(TIPO_QUAD, map_freq(freq), map_amp(amp));
}

//Mesma coisa s� que para onda senoidal
void sen(char *t){
    int freq, amp;
    sscanf(t,"sin%d%d", &freq, &amp);
    freq = map_freq(freq);
    amp = map_amp(amp);
    set_tipo(TIPO_SENO, freq, amp);
}

//Idem, para triangular
void tri(char *t){
    int freq, amp;
    sscanf(t,"tri%d%d", &freq, &amp);
    freq = map_freq(freq);
    amp = map_amp(amp);
    set_tipo(TIPO_TRIA, map_freq(freq), map_amp(amp));
}

//procura o comando na lista de comandos, considerando a primeira palavra
void procura_comando(char *texto){
    char comando[TAMANHO_COMANDO];

//    scanf de string, pegar a primeira palavra do texto e salvar na string
//    comando.
    sscanf(texto,"%s",comando);


    if(0 == strcmp(comando,"sin"))
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
        print("Comando n�o encontrado [", 1);
        print(comando, 1);
        print("]\n\r", 1);
    }
}



#endif /* FUNCOES_H_ */
