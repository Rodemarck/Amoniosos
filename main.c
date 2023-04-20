#include <msp430.h>
#include <string.h>
#include "funcoes.h"
#include "meuio.h"
#include "meutimer.h"

long long i;

void Init_GPIO();


//Main
int main(void){
    WDTCTL = WDTPW | WDTHOLD;

    // Configure GPIO
    Init_GPIO();
    Init_UART();
    Init_timer();
    //print("\n\rOlá caro usuario", 1);
    while (1){
        __no_operation();                         // For debugger
        //print("\n\rSó que não...", 1);
        do {

            __no_operation();                         // For debugger
            myGetLine (linha, 1);

            procura_comando(linha);
        } while(strcmp(linha,"sair"));
        //print("\n\rVou deixar você em paz...\n\r", 1);
    }
}



void Init_GPIO(){


    //DIREÇÃO DOS PINOS
    P1DIR       |= BIT0 | BIT1 | BIT2 | BIT3 |BIT5;
    P2DIR       &= ~BIT3;
    P4DIR       &= ~BIT1;
    P6DIR       |= BIT6;

    //REGISTOR PULL UP/DOWN
    P2REN       |= BIT3;
    P4REN       |= BIT1;

    //FUNÇÕES ESPECIAIS DOS PINOS
    /** P1.1 como saída do amplificador operacional 0*/
    P1SEL0      |= BIT1;
    P1SEL1      |= BIT1;


    //Setar saida dos pinos
    P1OUT       |= BIT5;
    P2OUT       |= BIT3;
    P4OUT       |= BIT1;



    //CONFIGURAÇÃO DA PORRA DO SAC DAC
    /**
     * Configurando o amplificador operacional, operational amplifier (OA)...
     * Usem a imagem que mandei no discord [https://cdn.discordapp.com/attachments/1072655359199682594/1098480422859583588/image.png]

       Registrador SACxOA (Smart Analog Combo x Operational Amplifier)

       Seleção da entrada positiva do OA (Positive SELect)
           PSEL_0 -> Entrada externa positiva (pino com a função OAx+)
           PSEL_1 -> DAC (Conversor de digital para analogico)
           PSEL_2 -> Entrada emparelhada (Verificar datasheet)
           PSEL_3 -> Reservado (não faaz nada)

       Ativação da entrada positiva do OA (Positive MUX ENable)
           PMUXEN_0 -> desativar entrada positiva
           PMUXEN_1 -> ativar entrada positiva

       Seleção da entrada negativa do OA (Negative SELect)
           NSEL_0 -> Entrada externa negativa (pino com a função OAx-)
           NSEL_1 -> Fonte do amplificador Operacional
           NSEL_2 -> Entrada emparelhada (Verificar datasheet)
           NSEL_3 -> Reservado (não faaz nada)

       Ativação da entrada negativa do OA (Negative MUX ENable)
           NMUXEN_0 -> Desativar entrada negativa
           NMUXEN_1 -> Ativar entrada negativa

       Ativação do OA
           OAEN_0   -> Desativar OA
           OAEN_1   -> Ativar OA

       Modo de energia
           OAPM_0   -> Alta velocidade, com alto consumo
           OAPM_1   -> Baixa velocidade, com baixo consumo

       Ativação do SAC(SAC Enable)
           SACEN_0  -> Ativar Sac
           SACEN_1  -> Desativar Sac
     */
    SAC0OA = PSEL_1 | PMUXEN_1 | NSEL_1 | NMUXEN_1 | OAEN_1 | OAPM_0 | SACEN_1;


    SAC0PGA |= MSEL1;
    SAC0DAC |= DACEN_1 | DACIE_0| DACDMAE_0 | DACLSEL_0| DACSREF_0;
}
