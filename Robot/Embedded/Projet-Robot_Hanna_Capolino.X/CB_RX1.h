/* 
 * File:   CB_RX1.h
 * Author: TP-EO-1
 *
 * Created on 9 décembre 2022, 16:28
 */

#ifndef CB_RX1_H
#define	CB_RX1_H

void CB_RX1_Add(unsigned char value);
unsigned char CB_RX1_Get(void);
int CB_RX1_GetRemainingSize(void);
int CB_RX1_GetDataSize(void);
#endif	/* CB_RX1_H */

