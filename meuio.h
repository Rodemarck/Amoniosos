/*
 * meuio.h
 *
 *  Created on: 11 de mar de 2023
 *      Author: rodem
 */

#ifndef MEUIO_H_
#define MEUIO_H_

#define MCLK_FREQ_MHZ 8                     // MCLK = 8MHz
#define TRUE 1
#define FALSE 0
#define COMMBUFFERLENGH 128
volatile int  TXBufferEmpty0, TXBufferEmpty1, ptrWrCH0, ptrWrCH1, ptrRdCH0, ptrRdCH1 ;
char bufferRxCH0[COMMBUFFERLENGH], bufferRxCH1[COMMBUFFERLENGH];
char linha[COMMBUFFERLENGH];

// Software Trim to get the best DCOFTRIM value
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
char myGetChar (int canal){
    char dado;
    if(canal==0){
        if (ptrRdCH0 == ptrWrCH0)
            return (0);
        dado = bufferRxCH0[ptrRdCH0];
        ptrRdCH0++;
        ptrRdCH0 &=COMMBUFFERLENGH;
        return(dado);
    }else {
        if (ptrRdCH1 == ptrWrCH1)

            return (0);
        dado = bufferRxCH1[ptrRdCH1];
        ptrRdCH1++;
        ptrRdCH1 &=COMMBUFFERLENGH-1;
        return(dado);
    }
}



void  myGetLine (char * buffer, int canal){
    unsigned char maxBuffer = COMMBUFFERLENGH;
    char dado;
    do {
        dado = myGetChar(canal);
        if (dado){
          *buffer++ = dado;
           maxBuffer--;
           if (dado=='\r'){
               *--buffer=0;
               return;
           }
        }


    }
    while (maxBuffer);
    *buffer=0;
}


void myPutch(char dado, int canal){
    if (canal==0){
    while (!TXBufferEmpty0);
    TXBufferEmpty0 = FALSE;
    UCA0TXBUF = dado;
    } else {
        while (!TXBufferEmpty1);
           TXBufferEmpty1 = FALSE;
           UCA1TXBUF = dado;

    }
}

void print(char * linha, int canal){
    while (*linha)
        myPutch(*linha++, canal);
}


#endif /* MEUIO_H_ */
