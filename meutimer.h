/*
 * timer.h
 *
 *  Created on: 4 de abr de 2023
 *      Author: Rodemarck
 */

#ifndef MEUTIMER_H_
#define MEUTIMER_H_

int lixo;
int tipo;

void Init_timer(){
    /*
       *   bits configuration for TB0CCTLn register
       *
                CM_0 // No capture
                CM_1 // Capture on rising edge
                CM_2 // Capture on falling edge
                CM_3 // Capture on both rising and falling edges
                 |  // Capture/compare input select.
                CCIS_0 // CCIxA
                CCIS_1 // CCIxB
                CCIS_2 // GND
                CCIS_3 // VCC
                 | // Synchronize capture source
                 SCS_0 //  Asynchronous capture
                 SCS_1 //  Synchronous capture
                 | //Compare latch load.
                 CLLD_0 // TBxCLn loads on write to TBxCCRn
                 CLLD_1 // TBxCLn loads when TBxR counts to 0
                 CLLD_2 // TBxCLn loads when TBxR counts to 0 (up or continuous mode). TBxCLn loads when TBxR counts to TBxCL0 or to 0 (up/down mode).
                 CLLD_3 // TBxCLn loads when TBxR counts to TBxCL
                 | // Capture mode
                 CAP_0 // Compare mode
                 CAP_1 // Capture mode
                 | // Output mode
                 OUTMOD_0 //  OUT bit value
                 OUTMOD_1 //  Set
                 OUTMOD_2 //  Toggle/reset
                 OUTMOD_3 //  Set/reset
                 OUTMOD_4 //  Toggle
                 OUTMOD_5 //  Reset
                 OUTMOD_6 //  Toggle/set
                 OUTMOD_7 //  Reset/set
                 | // Capture/compare interrupt enable.
                 CCIE_0 // Interrupt disabled
                 CCIE_1 //  Interrupt enabled
                 | // Output. For output mode 0, this bit directly controls the state of the outpu
                 OUT_0 // Output low
                 OUT_1 // Output high
                 | // Capture overflow. This bit indicates a capture overflow occurred. COV must be reset with software
                 COV_0 // No capture overflow occurred
                 COV_1 // Capture overflow occurred
      */

    //timer 0 config
    TB0CTL |= TBCLGRP_0 | CNTL_0 | TBSSEL_2 | ID_3 | MC_1 | TBIE_1;
    //timer 1 config
    TB1CTL |= TBCLGRP_0 | CNTL_0 | TBSSEL_2 | ID_0 | MC_1 | TBIE_1;

    /*
       *   bits configuration for TB0CCTLn register
       *
                CM_0 // No capture
                CM_1 // Capture on rising edge
                CM_2 // Capture on falling edge
                CM_3 // Capture on both rising and falling edges
                 |  // Capture/compare input select.
                CCIS_0 // CCIxA
                CCIS_1 // CCIxB
                CCIS_2 // GND
                CCIS_3 // VCC
                 | // Synchronize capture source
                 SCS_0 //  Asynchronous capture
                 SCS_1 //  Synchronous capture
                 | //Compare latch load.
                 CLLD_0 // TBxCLn loads on write to TBxCCRn
                 CLLD_1 // TBxCLn loads when TBxR counts to 0
                 CLLD_2 // TBxCLn loads when TBxR counts to 0 (up or continuous mode). TBxCLn loads when TBxR counts to TBxCL0 or to 0 (up/down mode).
                 CLLD_3 // TBxCLn loads when TBxR counts to TBxCL
                 | // Capture mode
                 CAP_0 // Compare mode
                 CAP_1 // Capture mode
                 | // Output mode
                 OUTMOD_0 //  OUT bit value
                 OUTMOD_1 //  Set
                 OUTMOD_2 //  Toggle/reset
                 OUTMOD_3 //  Set/reset
                 OUTMOD_4 //  Toggle
                 OUTMOD_5 //  Reset
                 OUTMOD_6 //  Toggle/set
                 OUTMOD_7 //  Reset/set
                 | // Capture/compare interrupt enable.
                 CCIE_0 // Interrupt disabled
                 CCIE_1 //  Interrupt enabled
                 | // Output. For output mode 0, this bit directly controls the state of the outpu
                 OUT_0 // Output low
                 OUT_1 // Output high
                 | // Capture overflow. This bit indicates a capture overflow occurred. COV must be reset with software
                 COV_0 // No capture overflow occurred
                 COV_1 // Capture overflow occurred
      */

    //não estamos utilizando essas funções do timer
    //timer 0 function
    // TB0CCTL0 |= CM_0 | CCIE_1;
    // TB0CCTL1 |= CM_0 | CAP_0 ;

    //timer 1 function
    // TB1CCTL0 |= CM_0 | CCIE_1;
    // TB1CCTL1 |= CM_0 | CAP_0 ;



    //registrador de limite, o timer vai contar até o registrador 0
    //e ai vai chamar a interrupção e voltar a contar do 0

    //registrador 0 do timer b0
    TB0CCR0 = 65000;
    //registrador 0 do timer b1
    TB1CCR0 = 520;

        //52  = 10k
        //520 = 1k

}

void configura_timer_gerador(int freq){
    TB1CCR0 = freq;
}



// interrupção do timer 0
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER0_B1_VECTOR
__interrupt void Timer_B1 (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_B1_VECTOR))) Timer_B1 (void)
#else
#error Compiler not supported!
#endif
{
    lixo = TB0IV;//limpa interrupção do timer 0
    P1OUT ^= BIT3;//alternar o pino P1.3 ()
    P1OUT ^=BIT0;//alterna o pino P1.0 (led)

}


// interrupção do timer 1
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER1_B1_VECTOR
__interrupt void Timer1_B1 (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER1_B1_VECTOR))) Timer1_B1 (void)
#else
#error Compiler not supported!
#endif
{
    lixo = TB1IV; //limpa interrupção do timer 1
    P1OUT ^= BIT5;//alterna o pino P1.5 (função quadrada)

}


#endif /* MEUTIMER_H_ */
