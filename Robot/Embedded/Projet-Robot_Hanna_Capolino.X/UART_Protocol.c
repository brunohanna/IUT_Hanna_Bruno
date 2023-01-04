#include <xc.h>
#include "UART_Protocol.h"
#include "CB_TX1.h"
#include "IO.h"

unsigned char UartCalculateChecksum(int msgFunction,
        int msgPayloadLength, unsigned char* msgPayload) {
    //Fonction prenant entrée la trame et sa longueur pour calculer le checksum
    unsigned char checksum = 0;
    checksum ^= 0xFE;
    checksum ^= (unsigned char) (msgFunction >> 8);
    checksum ^= (unsigned char) (msgFunction >> 0);
    checksum ^= (unsigned char) (msgPayloadLength >> 8);
    checksum ^= (unsigned char) (msgPayloadLength >> 0);

    for (int i = 0; i < msgPayloadLength; i++) {
        checksum ^= msgPayload[i];
    }
    return checksum;
}

void UartEncodeAndSendMessage(int msgFunction,
        int msgPayloadLength, unsigned char* msgPayload) {
    unsigned char message[msgPayloadLength + 6];
    int pos = 0;
    message[pos++] = 0xFE;
    message[pos++] = (unsigned char) (msgFunction >> 8);
    message[pos++] = (unsigned char) (msgFunction >> 0);
    message[pos++] = (unsigned char) (msgPayloadLength >> 8);
    message[pos++] = (unsigned char) (msgPayloadLength >> 0);
    for (int i = 0; i < msgPayloadLength; i++) {
        message[pos++] = msgPayload[i];
    }
    message[pos++] = UartCalculateChecksum(msgFunction, msgPayloadLength, msgPayload);
    SendMessage(message, pos);
    //Fonction d?encodage et d?envoi d?un message
}

int msgDecodedFunction = 0;
int msgDecodedPayloadLength = 0;
unsigned char msgDecodedPayload[128];
int msgDecodedPayloadIndex = 0;

void UartDecodeMessage(unsigned char c) {
    int rcvState = STATE_WAITING;
    switch (rcvState) {
        case STATE_WAITING:
            if (c == 0xFE) rcvState = STATE_FUNCTION_MSB;
            break;
        case STATE_FUNCTION_MSB:
            msgDecodedFunction = (int) (c << 8);
            rcvState = STATE_FUNCTION_LSB;
            break;
        case STATE_FUNCTION_LSB:
            msgDecodedFunction += (int) (c);
            rcvState = STATE_PAYLOAD_LENGTH_MSB;
            break;
        case STATE_PAYLOAD_LENGTH_MSB:
            msgDecodedPayloadLength = (int) (c << 8);
            rcvState = STATE_PAYLOAD_LENGTH_LSB;
            break;
        case STATE_PAYLOAD_LENGTH_LSB:
            msgDecodedPayloadLength += (int) (c);
            if (msgDecodedPayloadLength == 0)
                rcvState = STATE_CHECKSUM;
            else if (msgDecodedPayloadLength >= 128)
                rcvState = STATE_WAITING;
            else {
                msgDecodedPayloadIndex = 0;
                rcvState = STATE_PAYLOAD;
            }
            break;
        case STATE_PAYLOAD:
            msgDecodedPayload[msgDecodedPayloadIndex] = c;
            msgDecodedPayloadIndex++;
            if (msgDecodedPayloadIndex >= msgDecodedPayloadLength)
                rcvState = STATE_CHECKSUM;
            break;
        case STATE_CHECKSUM:
        {
            unsigned char calculatedChecksum = UartCalculateChecksum(msgDecodedFunction, msgDecodedPayloadLength, msgDecodedPayload);
            if (calculatedChecksum == c) {
                //Success, on a un message valide
                UartProcessDecodedMessage(msgDecodedFunction, msgDecodedPayloadLength, msgDecodedPayload);
            }
            rcvState = STATE_WAITING;
        }
            break;
        default:
            rcvState = STATE_WAITING;
            break;
            //Fonction prenant en entrée un octet et servant à reconstituer les trames
    }
}

void UartProcessDecodedMessage(unsigned char function,
        unsigned char payloadLength, unsigned char* payload) {
    switch (function) {
        case FUNCTION_TEXT:
            //UartEncodeAndSendMessage(msgFunction, msgPayloadLength, msgPayload);
            break;
        case FUNCTION_LED:
            if (payloadLength == 2) {
                switch (payload[0]) {
                    case 0:
                        LED_BLANCHE = (payload[1] == 1);
                        break;
                    case 1:
                        LED_ORANGE = (payload[1] == 1);
                        break;
                    case 2:
                        LED_BLEUE = (payload[1] == 1);
                        break;
                }
            }
            break;
        case FUNCTION_IR:
            if (payloadLength <= 3) {

            }
            break;
        case FUNCTION_SPEED:
            if (payloadLength <= 2) {

            }
            break;
        default:
            break;
    }
    //Fonction appelée après le décodage pour exécuter l?action
    //correspondant au message reçu
}


//*************************************************************************/
//Fonctions correspondant aux messages
//*************************************************************************/