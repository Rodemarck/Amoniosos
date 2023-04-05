#include <msp430.h>
#include <string.h>
#include "mapeamento.h"
#include "funcoes.h"
#include "meuio.h"

void Init_GPIO();
void init_timers();
long long i;


int main(void){
    WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer
    Posicao comandos = inicia_lista_comandos();


    adiciona_comando(comandos, "led", &led);
    adiciona_comando(comandos, "eco", &eco);
    //TODO registrar novos comandos

    // Configure GPIO
    Init_GPIO();
    init_UART();
    init_timers();

    while (1){
        print("\n\rHello Word 1\n\r", 1);
        __no_operation();                         // For debugger
        do {
            __no_operation();                         // For debugger
            myGetLine (linha, 1);
            procura_comando(comandos, linha);
        } while(strcmp(linha,"sair"));
        print("\n\rBye.....\n\r", 1);
    }
}


void Init_GPIO(){
    P1DIR = 0xFF;
    P2DIR = 0xFF;
    P1REN = 0xFF;
    P2REN = 0xFF;
    P1OUT = 0x00;
    P2OUT = 0x00;
    P1DIR |= BIT0;
    P6DIR |= BIT6;
    P2DIR &= ~BIT3;
    P2REN |= BIT3;
    P2OUT |= BIT3;
    P4DIR &= ~BIT1;
    P4REN |= BIT1;
    P4OUT |= BIT1;


    P1OUT &= ~BIT0;
    P6OUT &= ~BIT6;
}

void init_timers(){
    //TODO implementar as configurações de timer
}

