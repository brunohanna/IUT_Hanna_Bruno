#include <xc.h>
#include "timer.h"
#include "IO.h"
#include "PWM.h"
#include "ADC.h"
#include "robot.h"
#include "main.h"
#include "os5C.h"

unsigned char stateRobot;
unsigned int lastTimestamp = 0;
float vitesse = 1;

void OperatingSystemLoop5c(void) {
    switch (stateRobot) {
        case STATE_ATTENTE:
            timestamp = 0;
            PWMSetSpeedConsigne(0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
            stateRobot = STATE_ATTENTE_EN_COURS;

        case STATE_ATTENTE_EN_COURS:
            stateRobot = STATE_AVANCE;
            break;

        case STATE_AVANCE:
            PWMSetSpeedConsigne(-20.0*vitesse, MOTEUR_DROIT);
            PWMSetSpeedConsigne(20.0*vitesse, MOTEUR_GAUCHE);
            stateRobot = STATE_AVANCE_EN_COURS;
            break;
        case STATE_AVANCE_EN_COURS:
            SetNextRobotStateInAutomaticMode5c();
            break;

        case STATE_TOURNE_GAUCHE:
            PWMSetSpeedConsigne(-20.0*vitesse, MOTEUR_DROIT);
            PWMSetSpeedConsigne(-8.0*vitesse, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_GAUCHE_EN_COURS;
            break;
        case STATE_TOURNE_GAUCHE_EN_COURS:
            SetNextRobotStateInAutomaticMode5c();
            break;

        case STATE_TOURNE_DROITE:
            PWMSetSpeedConsigne(8.0*vitesse, MOTEUR_DROIT);
            PWMSetSpeedConsigne(20.0*vitesse, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_DROITE_EN_COURS;
            break;
        case STATE_TOURNE_DROITE_EN_COURS:
            SetNextRobotStateInAutomaticMode5c();
            break;

        case STATE_TOURNE_SUR_PLACE_GAUCHE:
            PWMSetSpeedConsigne(-15.0*vitesse, MOTEUR_DROIT);
            PWMSetSpeedConsigne(-15.0*vitesse, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS;
            break;
        case STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS:
            SetNextRobotStateInAutomaticMode5c();
            break;

        case STATE_TOURNE_SUR_PLACE_DROITE:
            PWMSetSpeedConsigne(15.0*vitesse, MOTEUR_DROIT);
            PWMSetSpeedConsigne(15.0*vitesse, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS;
            break;
        case STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS:
            SetNextRobotStateInAutomaticMode5c();
            break;

        case STATE_TOURNE_SUR_PLACE:
            PWMSetSpeedConsigne(15.0*vitesse, MOTEUR_DROIT);
            PWMSetSpeedConsigne(15.0*vitesse, MOTEUR_GAUCHE);
            lastTimestamp = timestamp;
            stateRobot = STATE_TOURNE_SUR_PLACE_EN_COURS;
            break;
        case STATE_TOURNE_SUR_PLACE_EN_COURS:
            if ((timestamp - lastTimestamp) > 300)
                stateRobot = STATE_AVANCE;
            break;
            
        case STATE_LEGET_TOURNE_DROITE:
            PWMSetSpeedConsigne(-10.0*vitesse, MOTEUR_DROIT);
            PWMSetSpeedConsigne(25.0*vitesse, MOTEUR_GAUCHE);
            stateRobot = STATE_LEGET_TOURNE_DROITE_EN_COURS;
            break;
        case STATE_LEGET_TOURNE_DROITE_EN_COURS:
            SetNextRobotStateInAutomaticMode5c();
            break;
            
        case STATE_LEGET_TOURNE_GAUCHE:
            PWMSetSpeedConsigne(25.0*vitesse, MOTEUR_DROIT);
            PWMSetSpeedConsigne(-10.0*vitesse, MOTEUR_GAUCHE);
            stateRobot = STATE_LEGET_TOURNE_DROITE_EN_COURS;
            break;
        case STATE_LEGET_TOURNE_GAUCHE_EN_COURS:
            SetNextRobotStateInAutomaticMode5c();
            break;

        default:
            stateRobot = STATE_ATTENTE;
            break;
    }
}

unsigned char nextStateRobot = 0;
unsigned char positionObstacle = 0;

void SetNextRobotStateInAutomaticMode5c() {
    
    //Détermination de la position des obstacles en fonction des télémètres
    if (robotState.distanceTelemetreExGauche < 15) //Obstacle à l'extreme droite
        positionObstacle = positionObstacle + 16;
    if (robotState.distanceTelemetreGauche < 18) //Obstacle à droite
        positionObstacle = positionObstacle + 8;
    if (robotState.distanceTelemetreCentre < 20) //Obstacle au centre
        positionObstacle = positionObstacle + 4;
    if (robotState.distanceTelemetreDroit < 18) //Obstacle à gauche
        positionObstacle = positionObstacle + 2;
    if (robotState.distanceTelemetreExDroit < 15) //Obstacle à l'extreme gauche
        positionObstacle = positionObstacle + 1;
    
    if ((robotState.distanceTelemetreGauche >= 30) 
            && (robotState.distanceTelemetreCentre >= 30) 
            && (robotState.distanceTelemetreDroit >= 30) 
            && (robotState.distanceTelemetreExGauche >= 15) 
            && (robotState.distanceTelemetreExDroit >= 15))
        vitesse = 1.4;
    else {vitesse = 1.0;}
    
    if ((robotState.distanceTelemetreGauche <= 8) 
            || (robotState.distanceTelemetreCentre <= 8) 
            || (robotState.distanceTelemetreDroit <= 8)
            || (robotState.distanceTelemetreExGauche <= 5)
            || (robotState.distanceTelemetreExDroit <= 5)){
        nextStateRobot = STATE_TOURNE_SUR_PLACE;
        return;
    }
    //Détermination de l?état à venir du robot
    switch (positionObstacle) {
        case 0b00000:   
            nextStateRobot = STATE_AVANCE;
            break;
        case 0b00001:
            nextStateRobot = STATE_LEGET_TOURNE_GAUCHE;
            break;
        case 0b00010:
            nextStateRobot = STATE_TOURNE_GAUCHE;
            break;
        case 0b00011:
            nextStateRobot = STATE_TOURNE_GAUCHE;
            break;
        case 0b00100:
            nextStateRobot = STATE_TOURNE_SUR_PLACE;
            break;
        case 0b00101:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
            break;
        case 0b00110:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
            break;
        case 0b00111:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
            break;
        case 0b01000:
            nextStateRobot = STATE_TOURNE_DROITE;
            break;
        case 0b01001:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_DROITE;
            break;
        case 0b01010:
            nextStateRobot = STATE_TOURNE_SUR_PLACE;
            break;
        case 0b01011:
            nextStateRobot = STATE_TOURNE_SUR_PLACE;
            break;
        case 0b01100:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_DROITE;
            break;
        case 0b01101:
            nextStateRobot = STATE_TOURNE_SUR_PLACE;
            break;
        case 0b01110:
            nextStateRobot = STATE_TOURNE_SUR_PLACE;
            break;
        case 0b01111:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
            break;
        case 0b10000:
            nextStateRobot = STATE_LEGET_TOURNE_DROITE;
            break;
        case 0b10001:
            nextStateRobot = STATE_TOURNE_SUR_PLACE;
            break;
        case 0b10010:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
            break;
        case 0b10011:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
            break;
        case 0b10100:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_DROITE;
            break;
        case 0b10101:
            nextStateRobot = STATE_TOURNE_SUR_PLACE;
            break;
        case 0b10110:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
            break;
        case 0b10111:
            nextStateRobot = STATE_TOURNE_SUR_PLACE;
            break;
        case 0b11000:
            nextStateRobot = STATE_TOURNE_DROITE;
            break;
        case 0b11001:
            nextStateRobot = STATE_TOURNE_DROITE;
            break;
        case 0b11010:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_DROITE;
            break;
        case 0b11011:
            nextStateRobot = STATE_TOURNE_SUR_PLACE;
            break;
        case 0b11100:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_DROITE;
            break;
        case 0b11101:
            nextStateRobot = STATE_TOURNE_SUR_PLACE;
            break;
        case 0b11110:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_DROITE;
            break;
        case 0b11111:
            nextStateRobot = STATE_TOURNE_SUR_PLACE;
            break;
            
        default:
            stateRobot = STATE_ATTENTE;
            break;
    }
    //Si l?on n?est pas dans la transition de l?étape en cours
    if (nextStateRobot != stateRobot - 1)
        stateRobot = nextStateRobot;
}

