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
#include "robot.h"
#include "main.h"
#include "os.h"

int main(void) {
    /***************************************************************************************************/
    //Initialisation de l?oscillateur
    /****************************************************************************************************/
    InitOscillator();

    /****************************************************************************************************/
    // Configuration des entrées sorties
    /****************************************************************************************************/
    InitIO();

    //    LED_BLANCHE = 1;
    //    LED_BLEUE = 1;
    //    LED_ORANGE = 1;

    /****************************************************************************************************/
    // Boucle Principale
    /****************************************************************************************************/
    InitPWM();
    InitADC1();
    InitTimer23();
    InitTimer1();

    while (1) {
        SetNextRobotStateInAutomaticMode();
        OperatingSystemLoop();
        if (ADCIsConversionFinished()) {
            ADCClearConversionFinishedFlag();
            unsigned int * result = ADCGetResult();
            float volts [5];
            int i;
            for (i = 0; i < 5; i++) {
                volts[i] = ((float) result [i])* 3.3 / 4096 * 3.2;
            }
            robotState.distanceTelemetreExGauche = 34 / volts[0] - 5;
            robotState.distanceTelemetreGauche = 34 / volts[1] - 5;
            robotState.distanceTelemetreCentre = 34 / volts[2] - 5;
            robotState.distanceTelemetreDroit = 34 / volts[3] - 5;
            robotState.distanceTelemetreExDroit = 34 / volts[4] - 5;
            if (robotState.distanceTelemetreDroit < 30) {
                LED_ORANGE = 1;
            } else {
                LED_ORANGE = 0;
            }
            if (robotState.distanceTelemetreCentre < 30) {
                LED_BLEUE = 1;
            } else {
                LED_BLEUE = 0;
            }
            if (robotState.distanceTelemetreGauche < 30) {
                LED_BLANCHE = 1;
            } else {
                LED_BLANCHE = 0;
            }
        }
    }


} // fin main
