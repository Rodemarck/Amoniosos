/*
 * so.h
 *
 *  Created on: 21 de abr de 2023
 *      Author: rodem
 */

#ifndef SO_H_
#define SO_H_

#include "tipo.h"
#include "funcoes.h"
#include "meutimer.h"
/**
 * Simulando maquina de estados com uma variavel long long, com 8 bytes (over kill)
 *
 *   |
     |
     |
     |  0  |

 */

//#define


#define ESTADO_UART 0
#define ESTADO_GERA 1
#define ESTADO_BUFF 2
#define ESTADO_LED1 3

#define UART_WR_CH0 0
#define UART_RD_CH0 1
#define UART_EX_CH0 2
#define UART_WR_CH1 3
#define UART_RD_CH1 4
#define UART_EX_CH1 5

short gerador = 0;
short maquina_estado_L1 = -1;
char maquina_estado_L2 = ESTADO_UART;

#endif /* SO_H_ */
