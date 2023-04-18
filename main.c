#include <msp430.h>
#include <string.h>
#include "mapeamento.h"
#include "funcoes.h"
#include "meuio.h"

void Init_GPIO();
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


    while (1){
        print("\n\rHello Word 1\n\r", 1);
        __no_operation();                         // For debugger
        do {
            __no_operation();                         // For debugger
            myGetLine (linha, 1);
            procura_comando(comandos, linha);
        } while(strcmp(linha,"sair"));
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

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCI_A1_VECTOR))) USCI_A_ISR (void)
#else
#error Compiler not supported!
#endif
{
  switch(__even_in_range(UCA1IV,USCI_UART_UCTXCPTIFG))
  {
    case USCI_NONE: break;
    case USCI_UART_UCRXIFG:
                __no_operation();
                bufferRxCH1[ptrWrCH1] = UCA1RXBUF;
                __no_operation();
                ptrWrCH1++;
                ptrWrCH1 &=COMMBUFFERLENGH-1;
                if (ptrWrCH1==ptrRdCH1){
                    ptrRdCH1++;
                    ptrRdCH1 &=COMMBUFFERLENGH-1;
                }
        //      while(!(UCA0IFG&UCTXIFG));
        //      UCA0TXBUF = UCA0RXBUF;
      __no_operation();
      break;
    case USCI_UART_UCTXIFG:
        TXBufferEmpty1 = TRUE;
        break;
    case USCI_UART_UCSTTIFG: break;
    case USCI_UART_UCTXCPTIFG: break;
    default: break;
  }
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=PORT2_VECTOR
__interrupt void PORT_2(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(PORT2_VECTOR))) PORT_2 (void)
#else
#error Compiler not supported!
#endif
{
  switch(__even_in_range(P2IV,P2IV_16)) {
    case P2IV_0 :
        _no_operation();
        break;
    case P2IV_2 :
        _no_operation();
        break;
    case P2IV_4 :
        _no_operation();
        break;
    case P2IV_6 :
        _no_operation();
        break;
    case P2IV_8 :
        _no_operation();
        print("Teste\n\r", 1);
        break;
    case P2IV_10 :
        _no_operation();
        break;
    case P2IV_12 :
        _no_operation();
        break;
    case P2IV_14 :
        _no_operation();
        break;
    case P2IV_16 :
        _no_operation();
        break;

    default: break;
  }
}




