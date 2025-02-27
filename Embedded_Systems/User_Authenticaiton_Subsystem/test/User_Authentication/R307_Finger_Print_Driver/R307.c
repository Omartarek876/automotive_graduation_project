/*
 * R307.c
 *
 *  Created on: Feb 16, 2025
 *      Author: Omar Tarek
 */

#include "R307.h"


// R307 Constants
#define R307_START_CODE_HIGH   0xEF
#define R307_START_CODE_LOW    0x01
#define R307_DEFAULT_ADDRESS   0xFFFFFFFF

#define R307_COMMAND_PACKET    0x01
#define R307_ACK_PACKET        0x07


static void R307_UART_SendByte(uint8 byte) {
    UART_sendByte(byte , 1);
}

static uint8 R307_UART_ReceiveByte(void) {
    return uart_RecieveByte(1);
}

static void R307_sendPacket(uint8 packet_id, uint8 *data, uint16 data_len) {
    uint16 packet_len = data_len + 2;
    uint16 checksum = packet_id;
    uint16 i = 0;
    checksum += ((packet_len >> 8) & 0xFF) + (packet_len & 0xFF);
    for (i = 0; i < data_len; i++) {
        checksum += data[i];
    }

    R307_UART_SendByte(R307_START_CODE_HIGH);
    R307_UART_SendByte(R307_START_CODE_LOW);

    R307_UART_SendByte((R307_DEFAULT_ADDRESS >> 24) & 0xFF);
    R307_UART_SendByte((R307_DEFAULT_ADDRESS >> 16) & 0xFF);
    R307_UART_SendByte((R307_DEFAULT_ADDRESS >> 8) & 0xFF);
    R307_UART_SendByte(R307_DEFAULT_ADDRESS & 0xFF);

    R307_UART_SendByte(packet_id);

    R307_UART_SendByte((packet_len >> 8) & 0xFF);
    R307_UART_SendByte(packet_len & 0xFF);

    for (i = 0; i < data_len; i++) {
        R307_UART_SendByte(data[i]);
    }

    R307_UART_SendByte((checksum >> 8) & 0xFF);
    R307_UART_SendByte(checksum & 0xFF);
}

static boolean R307_receivePacket(R307_Packet *packet) {
    uint16 i = 0;

    packet->start_code[0] = R307_UART_ReceiveByte();
    packet->start_code[1] = R307_UART_ReceiveByte();

    if (packet->start_code[0] != R307_START_CODE_HIGH || packet->start_code[1] != R307_START_CODE_LOW) {
        return FALSE;
    }

    packet->address = 0;
    packet->address |= ((uint32)R307_UART_ReceiveByte()) << 24;
    packet->address |= ((uint32)R307_UART_ReceiveByte()) << 16;
    packet->address |= ((uint32)R307_UART_ReceiveByte()) << 8;
    packet->address |= ((uint32)R307_UART_ReceiveByte());

    packet->packet_id = R307_UART_ReceiveByte();

    packet->length = ((uint16)R307_UART_ReceiveByte()) << 8;
    packet->length |= R307_UART_ReceiveByte();

    uint16 data_len = packet->length - 2;

    for (i = 0; i < data_len; i++) {
        packet->data[i] = R307_UART_ReceiveByte();
    }

    packet->checksum = ((uint16)R307_UART_ReceiveByte()) << 8;
    packet->checksum |= R307_UART_ReceiveByte();

    uint16 calc_checksum = packet->packet_id;
    calc_checksum += ((packet->length >> 8) & 0xFF) + (packet->length & 0xFF);
    for (i = 0; i < data_len; i++) {
        calc_checksum += packet->data[i];
    }

    return calc_checksum == packet->checksum;
}

static boolean R307_sendCommand(uint8 command, uint8 *params, uint16 params_len, R307_Packet *response) {
    uint8 packet_data[64];
    packet_data[0] = command;
    uint16 i = 0;
    for (i = 0; i < params_len; i++) {
        packet_data[1 + i] = params[i];
    }

    R307_sendPacket(R307_COMMAND_PACKET, packet_data, params_len + 1);

    return R307_receivePacket(response) && response->packet_id == R307_ACK_PACKET && response->data[0] == R307_ACK_OK;
}

void R307_init(void) {
    uartInit(&uart0_cfg);
    uartInit(&uart2_cfg);
}

boolean R307_captureFinger(void) {
    R307_Packet response;
    return R307_sendCommand(R307_CMD_GEN_IMAGE, NULL_PTR, 0, &response);
}

boolean R307_image2Tz(uint8 buffer_id) {
    R307_Packet response;
    return R307_sendCommand(R307_CMD_IMAGE2TZ, &buffer_id, 1, &response);
}

boolean R307_createModel(void) {
    R307_Packet response;
    return R307_sendCommand(R307_CMD_REG_MODEL, NULL_PTR, 0, &response);
}

boolean R307_storeModel(uint16 page_id, uint8 buffer_id) {
    R307_Packet response;
    uint8 params[3];
    params[0] = buffer_id;
    params[1] = (page_id >> 8) & 0xFF;
    params[2] = page_id & 0xFF;
    return R307_sendCommand(R307_CMD_STORE, params, 3, &response);
}

boolean R307_searchFingerprint(uint16 start_page, uint16 page_num, uint16 *match_page_id, uint16 *match_score) {
    R307_Packet response;
    uint8 params[5];

    params[0] = 0x01;
    params[1] = (start_page >> 8) & 0xFF;
    params[2] = start_page & 0xFF;
    params[3] = (page_num >> 8) & 0xFF;
    params[4] = page_num & 0xFF;

    if (!R307_sendCommand(R307_CMD_SEARCH, params, 5, &response)) {
        return FALSE;
    }

    *match_page_id = ((uint16)response.data[1] << 8) | response.data[2];
    *match_score = ((uint16)response.data[3] << 8) | response.data[4];
    return TRUE;
}



