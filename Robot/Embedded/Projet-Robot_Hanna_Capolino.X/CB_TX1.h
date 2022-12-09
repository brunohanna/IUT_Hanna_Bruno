/* 
 * File:   CB_TX1.h
 * Author: TP-EO-1
 *
 * Created on 7 décembre 2022, 10:52
 */

#ifndef CB_TX1_H
#define	CB_TX1_H

void SendMessage(unsigned char* message, int length);
void CB_TX1_Add(unsigned char value);
unsigned char CB_TX1_Get(void);
unsigned char CB_TX1_IsTranmitting(void);
int CB_TX1_RemainingSize(void);
void SendOne();
int CB_TX1_GetDataSize(void);
#endif	/* CB_TX1_H */

