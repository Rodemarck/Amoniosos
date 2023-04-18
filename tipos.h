/*
 * tipos.h
 *
 *  Created on: 2 de abr de 2023
 *      Author: Rodemarck
 */

#ifndef TIPOS_H_
#define TIPOS_H_

typedef void (*Funcao)(char*);
typedef void (*FuncaoV)(void);

typedef struct {
    int pid;
    long estado;
    FuncaoV func;
}Processo;



#endif /* TIPOS_H_ */
