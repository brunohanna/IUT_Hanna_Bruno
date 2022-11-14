/* 
 * File:   timer.h
 * Author: TP-EO-1
 *
 * Created on 2 septembre 2022, 16:09
 */

#ifndef TIMER_H
#define TIMER_H
void InitTimer23(void);
void InitTimer1(void);
void InitTimer4(void);
void SetFreqTimer1(float freq);
void SetFreqTimer4(float freq);
void ResetTimestamp();

extern unsigned long timestamp;

#endif

