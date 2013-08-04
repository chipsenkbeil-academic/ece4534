#ifndef __my_uart_h
#define __my_uart_h

#include "messages.h"

// States for UART handling
#define UART_READ_HEADER_BYTE       0x01
#define UART_READ_ADDRESS_BYTE      0x02
#define UART_READ_TYPE_BYTE         0x03
#define UART_READ_DATA_BYTES        0x04
#define UART_VALIDATE_CHECKSUM_BYTE 0x05

#define MAXUARTBUF 8
#if (MAXUARTBUF > MSGLEN)
#define MAXUARTBUF MSGLEN
#endif
typedef struct __uart_comm {
	unsigned char buffer[MAXUARTBUF];
	unsigned char	buflen;
        unsigned char   bufptr;
        unsigned char   bufmaxlen; // Set based on message type, maximum length
        unsigned char   state;
} uart_comm;

void enableUart(void);
void init_uart(uart_comm *, uart_comm *);
void uart_send_int_handler(void);
void uart_recv_int_handler(void);
void uart_send(unsigned char, unsigned char*);
#endif
