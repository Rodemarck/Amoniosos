#include <msp430.h>
#include <string.h>
#include "funcoes.h"
#include "meuio.h"
#include "meutimer.h"
#include "so.h"


void Init_GPIO();



//Main
int main(void){
    WDTCTL = WDTPW | WDTHOLD;
    // Configure GPIO
    Init_GPIO();
    Init_UART();
    Init_timer();
    print("olá parceiro\n\r",1);
    while (1){
        switch(++maquina_estado_L1){
        case ESTADO_UART:
            switch(++maquina_estado_L2){
            case UART_WR_CH0:
                _escreve_canal_0();
                break;
            case UART_RD_CH0:
                _ler_canal_0();
                break;
            case UART_EX_CH0:
                _interpreta_canal_0();
                break;
            case UART_WR_CH1:
                _escreve_canal_1();
                break;
            case UART_RD_CH1:
                _ler_canal_1();
                break;
            case UART_EX_CH1:
                __no_operation();
                _interpreta_canal_1();
                maquina_estado_L2 = 0;
                break;
            }
            break;
        case ESTADO_GERA:
            __no_operation();
            _desenha();
            break;
        case ESTADO_BUFF:
            switch(maquina_estado_L2){

            }
            break;
        case ESTADO_LED1:
            maquina_estado_L1 = -1;
            if(timer_led_tick){
                P1OUT ^= BIT0;
                timer_led_tick = 0;
            }
            break;
        }
    }
}



void Init_GPIO(){


    //DIRE��O DOS PINOS
    P1DIR       |= BIT0 | BIT1 | BIT2 | BIT3 |BIT5;
    P2DIR       &= ~BIT3;
    P4DIR       &= ~BIT1;
    P6DIR       |= BIT6;

    //REGISTOR PULL UP/DOWN
    P2REN       |= BIT3;
    P4REN       |= BIT1;

    //FUN��ES ESPECIAIS DOS PINOS
    /** P1.1 como sa�da do amplificador operacional 0*/
    P1SEL0      |= BIT1;
    P1SEL1      |= BIT1;


    //Setar saida dos pinos
    P1OUT       |= BIT5;
    P2OUT       |= BIT3;
    P4OUT       |= BIT1;



    //CONFIGURA��O DA PORRA DO SAC DAC
    /**
     * Configurando o amplificador operacional, operational amplifier (OA)...
     * Usem a imagem que mandei no discord [https://cdn.discordapp.com/attachments/1072655359199682594/1098480422859583588/image.png]

       Registrador SACxOA (Smart Analog Combo x Operational Amplifier)

       Sele��o da entrada positiva do OA (Positive SELect)
           PSEL_0 -> Entrada externa positiva (pino com a fun��o OAx+)
           PSEL_1 -> DAC (Conversor de digital para analogico)
           PSEL_2 -> Entrada emparelhada (Verificar datasheet)
           PSEL_3 -> Reservado (n�o faaz nada)

       Ativa��o da entrada positiva do OA (Positive MUX ENable)
           PMUXEN_0 -> desativar entrada positiva
           PMUXEN_1 -> ativar entrada positiva

       Sele��o da entrada negativa do OA (Negative SELect)
           NSEL_0 -> Entrada externa negativa (pino com a fun��o OAx-)
           NSEL_1 -> Fonte do amplificador Operacional
           NSEL_2 -> Entrada emparelhada (Verificar datasheet)
           NSEL_3 -> Reservado (n�o faaz nada)

       Ativa��o da entrada negativa do OA (Negative MUX ENable)
           NMUXEN_0 -> Desativar entrada negativa
           NMUXEN_1 -> Ativar entrada negativa

       Ativa��o do OA
           OAEN_0   -> Desativar OA
           OAEN_1   -> Ativar OA

       Modo de energia
           OAPM_0   -> Alta velocidade, com alto consumo
           OAPM_1   -> Baixa velocidade, com baixo consumo

       Ativa��o do SAC(SAC Enable)
           SACEN_0  -> Ativar Sac
           SACEN_1  -> Desativar Sac
     */
    SAC0OA = PSEL_1 | PMUXEN_1 | NSEL_1 | NMUXEN_1 | OAEN_1 | OAPM_0 | SACEN_1;


    SAC0PGA |= MSEL1;
    SAC0DAC |= DACEN_1 | DACIE_0| DACDMAE_0 | DACLSEL_0| DACSREF_0;
}
