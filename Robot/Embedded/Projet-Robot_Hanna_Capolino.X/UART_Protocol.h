/* 
 * File:   UART_Protocol.h
 * Author: TP-EO-1
 *
 * Created on 4 janvier 2023, 13:10
 */

#ifndef UART_PROTOCOL_H
#define	UART_PROTOCOL_H

#define STATE_WAITING 0
#define STATE_FUNCTION_MSB 1
#define STATE_FUNCTION_LSB 2
#define STATE_PAYLOAD_LENGTH_MSB 3
#define STATE_PAYLOAD_LENGTH_LSB 4
#define STATE_PAYLOAD 5
#define STATE_CHECKSUM 6

#define FUNCTION_TEXT 0x0080
#define FUNCTION_LED 0x0020
#define FUNCTION_IR 0x0030
#define FUNCTION_SPEED 0x0040

unsigned char UartCalculateChecksum(int msgFunction,int msgPayloadLength, unsigned char* msgPayload);
void UartEncodeAndSendMessage(int msgFunction,int msgPayloadLength, unsigned char* msgPayload);
void UartDecodeMessage(unsigned char c);
void UartProcessDecodedMessage(unsigned char function,unsigned char payloadLength, unsigned char* payload);

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* UART_PROTOCOL_H */

