#include <msp430.h>
#include <string.h>
#include "mapeamento.h"
#include "funcoes.h"
#include "meuio.h"

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




#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCI_A0_VECTOR))) USCI_A0_ISR (void)
#else
#error Compiler not supported!
#endif
{
  switch(__even_in_range(UCA0IV,USCI_UART_UCTXCPTIFG))
  {
    case USCI_NONE: break;
    case USCI_UART_UCRXIFG:
        bufferRxCH0[ptrWrCH0] = UCA0RXBUF;
        ptrWrCH0++;
        ptrWrCH0 &=COMMBUFFERLENGH-1;
        if (ptrWrCH0==ptrRdCH0){
            ptrRdCH0++;
            ptrRdCH0 &=COMMBUFFERLENGH-1;
        }
//      while(!(UCA0IFG&UCTXIFG));
//      UCA0TXBUF = UCA0RXBUF;
      __no_operation();
      break;
    case USCI_UART_UCTXIFG:
        TXBufferEmpty0 = TRUE;
        break;
    case USCI_UART_UCSTTIFG: break;
    case USCI_UART_UCTXCPTIFG: break;
    default: break;
  }
}

