#ifndef __SERIAL_H__
#define __SERIAL_H__

#include <fxlib.h>
#include <stdio.h>
#include <stdlib.h>

#include "syscalls.h"

#define READ_TIMEOUT 1000

typedef struct packet_s {
    void* data;
    unsigned short psize;
    unsigned char pid;
} packet_t;

typedef struct con_packet_s {
    unsigned char val;
} con_packet_t;

unsigned char serialConf[] = {
    0,
    9, // 115200 baud
    0, // no parity
    0, // 8 bits
    0, // 1 stop bit
    0
};

/**
 * Initialises the serial port
 * 
 * @returns [
 *      0 -> success
 *      1 -> serial close failure
 *      2 -> interface is already open
 *      3 -> malformed serialConf
 *      4 -> unknown error 
 * ]
 */
int serialInit() {
    int result;

    // close the serial port first, in case it is already open
    if (Serial_Close(1))
        return 1;

    switch (Serial_Open(serialConf)) {
        case 0:
            Serial_ClearRxBuffer();
            Serial_ClearTxBuffer();
            return 0;
        case 3:
            return 2;
        case 4:
            return 3;
        default:
            return 4;
    }
}

/**
 * Attempts to get a packet
 * 
 * @return [
 *      0 -> success
 *      1 -> no data received
 *      2 -> not enough data received
 *      3 -> serial is not open
 * ]
 */
int serialGetPacket(packet_t* packet) {
    unsigned char* data;
    unsigned short timeout;
    short readBytes = 0;
    unsigned char temp;

    if (Serial_IsOpen() == 3)
        return 3;

    for (timeout = 0; timeout < READ_TIMEOUT && !Serial_GetRxBytesAvailable(); timeout++)
        Sleep(1);

    if (Serial_GetRxBytesAvailable() < 3)
        return 1;

    Serial_ReadByte(&packet->pid);
    Serial_ReadByte(&temp);
    packet->psize = temp << 8;
    Serial_ReadByte(&temp);
    packet->psize += temp;

    if (!packet->psize)
        return 0;

    data = (unsigned char*) calloc(packet->psize, sizeof(unsigned char));

    for (timeout = 0; timeout < READ_TIMEOUT && Serial_GetRxBytesAvailable() < packet->psize; timeout++)
        Sleep(1);

    Serial_ReadBytes(data, packet->psize, &readBytes);

    if (readBytes < packet->psize) {
        free(data);
        return 2;
    }

    packet->data = data;

    return 0;
}

/**
 * SER_CONNECT
 * Attempts to make a connection over serial
 * 
 * @returns [
 *      0 -> success
 *      1 -> data failure
 *      2 -> incorrect response
 *      3 -> serial is not open
 * ]
 */
int serialConnect() {
    unsigned char challenge = 0x93; // hardcoded for now
    unsigned char spBuf[16] = { 0 };
    int result = 0;
    packet_t packet;
    con_packet_t* con_packet;

    if (Serial_IsOpen() == 3)
        return 3;

    Serial_WriteByte(0x01);
    Serial_WriteByte(0x00);
    Serial_WriteByte(0x01);
    Serial_WriteByte(challenge);

    result = serialGetPacket(&packet);

    if (result && packet.pid != 0x81) {
        if (packet.psize)
            free(packet.data);

        return result;
    }

    con_packet = (con_packet_t*) packet.data;
    if (con_packet->val == (unsigned char) ~challenge)
        result = 0; // success
    else
        result = 2; // failure

    free(packet.data);

    return result;
}

#endif //__SERIAL_H__