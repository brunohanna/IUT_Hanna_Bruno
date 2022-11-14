#ifndef PWM_H
#define	PWM_H
#define MOTEUR_DROIT 0
#define MOTEUR_GAUCHE 1

void InitPWM(void);
//void PWMSetSpeed(float vitesseEnPourcents, int DIRECTION);
void PWMUpdateSpeed();
void PWMSetSpeedConsigne(float vitesseEnPourcents, int DIRECTION);

#endif	/* XC_HEADER_TEMPLATE_H */
