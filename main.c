#include <msp430.h>
#include <string.h>
#include "mapeamento.h"
#include "funcoes.h"
#include "meuio.h"

void Init_GPIO();
long long i;


int main(void){
    WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer
    Posicao comandos = inicia_lista_comandos();
    adiciona_comando(comandos, "led", &led);
    adiciona_comando(comandos, "eco", &eco);


    // Configure GPIO
    Init_GPIO();
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




