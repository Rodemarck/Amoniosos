/*
 * mapeamento.h
 *
 *  Created on: 9 de mar de 2023
 *      Author: rodem
 */

#ifndef MAPEAMENTO_H_
#define MAPEAMENTO_H_

#include <stdio.h>
#include <stdlib.h>
#include "meuio.h"
#include "tipos.h"

#define TAMANHO_COMANDO 16
#define SEED    0x12345678
#define SEED_OP 0x5bd1e995

// algotimo MurmurOAAT -> https://stackoverflow.com/questions/7666509/hash-function-for-string
unsigned long hash_string(char *ptr){
    unsigned long hash = SEED;
    while(*ptr){
        hash ^= *ptr++;
        hash *= SEED_OP;
        hash ^= hash >> 15;
    }
    return hash;

}


struct pos_comandos{
    struct pos_comandos * prox;
    char *comando;
    unsigned long hash;
    Funcao f;
};

typedef struct pos_comandos* Posicao;

void procura_comando(Posicao cabeca, char *texto){
    char comando[TAMANHO_COMANDO];
    sscanf(texto,"%s",comando);
    unsigned long hash = hash_string(comando);
    Posicao p;
    for(p = cabeca; p->prox != NULL; p = p->prox){
        if(p->prox->hash == hash){
            if(0 == strcmp(comando,p->prox->comando)){
                p->prox->f(texto);
                return;
            }
        }
    }
    print("Comando não encontrado [", 1);
    print(comando, 1);
    print("]\n\r", 1);
}

Posicao inicia_lista_comandos(){
    Posicao p = (Posicao) malloc(sizeof(struct pos_comandos));
    p->prox = NULL;
    return p;
}
void adiciona_comando(Posicao cabeca, char * comando, Funcao f){
    int t = strlen(comando);
    t = t < TAMANHO_COMANDO ? t : TAMANHO_COMANDO;

    Posicao p = (Posicao) malloc(sizeof(struct pos_comandos));
    p->comando = (char*) malloc(t * (sizeof(char)) );
    strcpy(p->comando,comando);
    p->hash = hash_string(comando);
    p->f = f;
    p->prox = NULL;
    Posicao aux = cabeca;
    while(aux->prox != NULL)
        aux = aux->prox;
    aux->prox = p;
}

void int_to_str(int n, char* ptr){
    int aux,i = 0,valor = n >0 ? n: (-1 * n);
    char temp;
    *ptr++ = 0;
    do{
        ++i;
        aux = valor%10;
        valor/=10;
        *ptr++ = '0' + aux;
    }while(valor);
    if(n<0){
        *ptr= '-';
        ++i;
    }else
        --ptr;
    aux = i/2;
    for(valor=aux; valor >= 0; valor--){
        temp = *(ptr - (valor));
        *(ptr - (valor)) = *(ptr - (i - valor));
        *(ptr - (i - valor)) = temp;
    }
}





#endif /* MAPEAMENTO_H_ */
