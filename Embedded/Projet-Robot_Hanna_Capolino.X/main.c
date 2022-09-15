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
        } // fin main
    }
