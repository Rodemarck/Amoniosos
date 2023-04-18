#include <msp430.h>
#include <string.h>
#include "mapeamento.h"
#include "funcoes.h"
#include "meuio.h"
#include "meutimer.h"

void Init_GPIO();
void init_timers();
long long i;


int main(void){
    WDTCTL = WDTPW | WDTHOLD; //led 14 20

    Posicao comandos = inicia_lista_comandos();
    //adiciona_comando(comandos, "led", &led);
    adiciona_comando(comandos, "eco", &eco);
    adiciona_comando(comandos, "quad", &quad);



    // Configure GPIO
    Init_GPIO();
    Init_timer();
    init_UART();
    Init_timer();

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
