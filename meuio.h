/*
 * meuio.h
 *
 *  Created on: 11 de mar de 2023
 *      Author: rodem
 *
 *  PORRA NEM PERDE TEMPO LENDO ISSO, A NÃO SER O
 */

#ifndef MEUIO_H_
#define MEUIO_H_

#include "tipo.h"
#include "funcoes.h"
#define COMMBUFFERLENGH 64
#define MAXSTRINGLENGTH 256


#define MCLK_FREQ_MHZ 8                     // MCLK = 8MHz


volatile int    TXBufferEmpty0, TXBufferEmpty1, ptrWrCH0, ptrWrCH1, 
                ptrRdCH0, ptrRdCH1, RdFinishCH0, RdFinishCH1, kk  ;
int iRdCH0, iRdCH1, iWrCH0, iWrCH1;
char bufferRxCH0[COMMBUFFERLENGH+1], bufferRxCH1[COMMBUFFERLENGH+1],
    strRdCH0[MAXSTRINGLENGTH],strRdCH1[MAXSTRINGLENGTH],
    strWrCH0[MAXSTRINGLENGTH],strWrCH1[MAXSTRINGLENGTH];


void print(char * linha, int canal){
    if(canal == 0){
        int posix = iWrCH0;
        while(strWrCH0[posix] != '\0') 
            ++posix;
        
        while (*linha){
            strWrCH0[posix++] = *linha++;
            posix &= COMMBUFFERLENGH-1;
        }
    }else{
        int posix = iWrCH1;
        while(strWrCH1[posix] != '\0') 
            ++posix;
        while (*linha){
            strWrCH1[posix++] = *linha++;
            posix &= COMMBUFFERLENGH-1;
        }
    }
}

void _ler_canal_0(){
    if (RdFinishCH0 || ptrRdCH0 == ptrWrCH0) return;
    strRdCH0[iRdCH0] = bufferRxCH0[ptrRdCH0++];
    if(strRdCH0[iRdCH0++] == '\r'){
        strRdCH0[iRdCH0-1] = '\0';
        RdFinishCH0 = TRUE;
    }
    ptrRdCH0 &=COMMBUFFERLENGH-1;
    iRdCH0 &= MAXSTRINGLENGTH-1;
}

void _escreve_canal_0(){
    if(TXBufferEmpty0 && strWrCH0[iWrCH0]){
        TXBufferEmpty0 = FALSE;
        UCA0TXBUF = strWrCH0[iWrCH0];
        if(strWrCH0[iWrCH0] != '\0')
            ++iWrCH0;
        iWrCH0 &= MAXSTRINGLENGTH-1;
    }
}

void _interpreta_canal_0(){
    if(RdFinishCH0){
        procura_comando(strRdCH0);
        //strRdCH0[iRdCH0] = 1;
        iRdCH0 = 0;
        RdFinishCH0 = FALSE;
    }
}

void _ler_canal_1(){
    __no_operation();
    if (RdFinishCH1 || iRdCH1 == ptrWrCH1) return;
    strRdCH1[iRdCH1] = bufferRxCH1[ptrRdCH1++];
    if(strRdCH1[iRdCH1++] == '\r'){
        strRdCH1[iRdCH1-1] = '\0';
        iRdCH1 = 0;
        RdFinishCH1 = TRUE;
    }
    ptrRdCH1 &=COMMBUFFERLENGH-1;
    iRdCH1 &= MAXSTRINGLENGTH-1;
}

void _escreve_canal_1(){
    if(TXBufferEmpty1 && strWrCH1[iWrCH1]){
        TXBufferEmpty1 = FALSE;
        UCA1TXBUF = strWrCH1[iWrCH1];
        if(strWrCH1[iWrCH1++] == '\0')
            --iWrCH1;
        iWrCH1 &= MAXSTRINGLENGTH-1;
    }
}


void _interpreta_canal_1(){
    if(RdFinishCH1){
        procura_comando(strRdCH1);
        strRdCH1[iRdCH1] = 1;
        iRdCH1 = 0;
        RdFinishCH1 = FALSE;
    }
}




//interrupção de UART A0
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


//interrupção de UART A1
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
      __no_operation();
      break;
    case USCI_UART_UCTXIFG:
        __no_operation();
        TXBufferEmpty1 = TRUE;
        break;
    case USCI_UART_UCSTTIFG: break;
    case USCI_UART_UCTXCPTIFG: break;
    default: break;
  }
}



































void Software_Trim()
{
    unsigned int oldDcoTap = 0xffff;
    unsigned int newDcoTap = 0xffff;
    unsigned int newDcoDelta = 0xffff;
    unsigned int bestDcoDelta = 0xffff;
    unsigned int csCtl0Copy = 0;
    unsigned int csCtl1Copy = 0;
    unsigned int csCtl0Read = 0;
    unsigned int csCtl1Read = 0;
    unsigned int dcoFreqTrim = 3;
    unsigned char endLoop = 0;

    do
    {
        CSCTL0 = 0x100;                         // DCO Tap = 256
        do
        {
            CSCTL7 &= ~DCOFFG;                  // Clear DCO fault flag
        }while (CSCTL7 & DCOFFG);               // Test DCO fault flag

        __delay_cycles((unsigned int)3000 * MCLK_FREQ_MHZ);// Wait FLL lock status (FLLUNLOCK) to be stable
                                                           // Suggest to wait 24 cycles of divided FLL reference clock
        while((CSCTL7 & (FLLUNLOCK0 | FLLUNLOCK1)) && ((CSCTL7 & DCOFFG) == 0));

        csCtl0Read = CSCTL0;                   // Read CSCTL0
        csCtl1Read = CSCTL1;                   // Read CSCTL1

        oldDcoTap = newDcoTap;                 // Record DCOTAP value of last time
        newDcoTap = csCtl0Read & 0x01ff;       // Get DCOTAP value of this time
        dcoFreqTrim = (csCtl1Read & 0x0070)>>4;// Get DCOFTRIM value

        if(newDcoTap < 256)                    // DCOTAP < 256
        {
            newDcoDelta = 256 - newDcoTap;     // Delta value between DCPTAP and 256
            if((oldDcoTap != 0xffff) && (oldDcoTap >= 256)) // DCOTAP cross 256
                endLoop = 1;                   // Stop while loop
            else
            {
                dcoFreqTrim--;
                CSCTL1 = (csCtl1Read & (~DCOFTRIM)) | (dcoFreqTrim<<4);
            }
        }
        else                                   // DCOTAP >= 256
        {
            newDcoDelta = newDcoTap - 256;     // Delta value between DCPTAP and 256
            if(oldDcoTap < 256)                // DCOTAP cross 256
                endLoop = 1;                   // Stop while loop
            else
            {
                dcoFreqTrim++;
                CSCTL1 = (csCtl1Read & (~DCOFTRIM)) | (dcoFreqTrim<<4);
            }
        }

        if(newDcoDelta < bestDcoDelta)         // Record DCOTAP closest to 256
        {
            csCtl0Copy = csCtl0Read;
            csCtl1Copy = csCtl1Read;
            bestDcoDelta = newDcoDelta;
        }

    }while(endLoop == 0);                      // Poll until endLoop == 1

    CSCTL0 = csCtl0Copy;                       // Reload locked DCOTAP
    CSCTL1 = csCtl1Copy;                       // Reload locked DCOFTRIM
    while(CSCTL7 & (FLLUNLOCK0 | FLLUNLOCK1)); // Poll until FLL is locked
}





void Init_UART(){
    PM5CTL0 &= ~LOCKLPM5;                    // Disable the GPIO power-on default high-impedance mode
                                                   // to activate 1previously configured port settings

    __bis_SR_register(SCG0);                 // disable FLL
    CSCTL3 |= SELREF__REFOCLK;               // Set REFO as FLL reference source
    CSCTL1 = DCOFTRIMEN_1 | DCOFTRIM0 | DCOFTRIM1 | DCORSEL_3;// DCOFTRIM=3, DCO Range = 8MHz
    CSCTL2 = FLLD_0 + 243;                  // DCODIV = 8MHz
    __delay_cycles(3);
    __bic_SR_register(SCG0);                // enable FLL
    Software_Trim();                        // Software Trim to get the best DCOFTRIM value
    CSCTL4 = SELMS__DCOCLKDIV | SELA__REFOCLK; // set default REFO(~32768Hz) as ACLK source, ACLK = 32768Hz
                                                 // default DCODIV as MCLK and SMCLK source

    // Configure UART pins
    P1SEL0 |= BIT6 | BIT7;                    // set 2-UART pin as second function
    P4SEL0 |= BIT2 | BIT3;                    // set 2-UART pin as second function
    // Configure UART
    UCA0CTLW0 |= UCSWRST;
    UCA1CTLW0 |= UCSWRST;
    UCA0CTLW0 |= UCSSEL__SMCLK;
    UCA1CTLW0 |= UCSSEL__SMCLK;
    // Baud Rate calculation
    // 8.000.000/(16*9600) = 52.083
    // Fractional portion = 0.083
    // User's Guide Table 17-4: UCBRSx = 0x49
    // UCBRFx = int ( (52.083-52)*16) = 1
    UCA0BR0 = 52;                             // 8000000/16/9600
    UCA1BR0 = 52;                             // 8000000/16/9600
    UCA0BR1 = 0x00;
    UCA1BR1 = 0x00;
    UCA0MCTLW = 0x4900 | UCOS16 | UCBRF_1;
    UCA1MCTLW = 0x4900 | UCOS16 | UCBRF_1;
    UCA0CTLW0 &= ~UCSWRST;                    // Initialize eUSCI
    UCA1CTLW0 &= ~UCSWRST;                    // Initialize eUSCI
    UCA0IE |= UCRXIE | UCTXIE;                         // Enable USCI_A0 RX interrupt
    UCA1IE |= UCRXIE | UCTXIE;                         // Enable USCI_A0 RX interrupt
    TXBufferEmpty0 = TRUE;
    TXBufferEmpty1 = TRUE;
    RdFinishCH0 = FALSE;
    RdFinishCH1 = FALSE;
    int x;
    for( x=0; x<MAXSTRINGLENGTH; x++){
        strRdCH0[x] = 1;
        strRdCH1[x] = 1;
    }
    strWrCH0[0] = '\0';
    strWrCH1[0] = '\0';

    __bis_SR_register(GIE);         // Enter LPM3, interrupts enabled //LPM3_bits|
}
#endif /* MEUIO_H_ */
