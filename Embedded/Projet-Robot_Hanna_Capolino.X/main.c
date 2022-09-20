/* 
 * File:   main.c
 * Author: TP-EO-1 // CAPOLINO HANNA ESE B
 *
 * Created on 2 septembre 2022, 15:15
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "ChipConfig.h"
#include "IO.h"
#include "timer.h"
#include "PWM.h"
#include "ADC.h"


unsigned int ADCValue0;
unsigned int ADCValue1;
unsigned int ADCValue2;

int main(void) {
    /***************************************************************************************************/
    //Initialisation de l?oscillateur
    /****************************************************************************************************/
    InitOscillator();

    /****************************************************************************************************/
    // Configuration des entrées sorties
    /****************************************************************************************************/
    InitIO();

    LED_BLANCHE = 1;
    LED_BLEUE = 1;
    LED_ORANGE = 1;

    /****************************************************************************************************/
    // Boucle Principale
    /****************************************************************************************************/
    InitPWM();
    InitADC1();
    InitTimer23();
    InitTimer1();
    
        while (1) {
            if(ADCIsConversionFinished())
            {
                unsigned int * result = ADCGetResult();
                ADCValue0 = result[0];
                ADCValue1 = result[1];
                ADCValue2 = result[2];
                ADCClearConversionFinishedFlag();
            }
        } // fin main
    }
