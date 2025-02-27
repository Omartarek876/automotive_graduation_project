/*
 * R307.h
 *
 *  Created on: Feb 16, 2025
 *      Author: Omar Tarek
 */

#ifndef R307_FINGER_PRINT_DRIVER_R307_H_
#define R307_FINGER_PRINT_DRIVER_R307_H_

#include "../Uart_Driver/uart.h"  // UART driver for Tiva C
#include "../Critical_Files/Platform_Types.h"

// R307 Constants
#define R307_START_CODE_HIGH   0xEF
#define R307_START_CODE_LOW    0x01
#define R307_DEFAULT_ADDRESS   0xFFFFFFFF

// Packet Identifiers
#define R307_COMMAND_PACKET    0x01
#define R307_ACK_PACKET        0x07

// Command Codes
#define R307_CMD_GEN_IMAGE     0x01
#define R307_CMD_IMAGE2TZ      0x02
#define R307_CMD_MATCH         0x03
#define R307_CMD_SEARCH        0x04
#define R307_CMD_REG_MODEL     0x05
#define R307_CMD_STORE         0x06
#define R307_CMD_TEMPLATE_NUM  0x1D

// Response Codes
#define R307_ACK_OK            0x00

#define R307_MAX_DATA_LEN      64

typedef struct {
    uint8 start_code[2];
    uint32 address;
    uint8 packet_id;
    uint16 length;
    uint8 data[R307_MAX_DATA_LEN];
    uint16 checksum;
} R307_Packet;

// Function Prototypes
void R307_init(void);
boolean R307_captureFinger(void);
boolean R307_image2Tz(uint8 buffer_id);
boolean R307_createModel(void);
boolean R307_storeModel(uint16 page_id, uint8 buffer_id);
boolean R307_searchFingerprint(uint16 start_page, uint16 page_num, uint16 *match_page_id, uint16 *match_score);

#endif /* R307_FINGER_PRINT_DRIVER_R307_H_ */
