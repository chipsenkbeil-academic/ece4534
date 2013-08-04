#ifndef __my_i2c_h
#define __my_i2c_h

#include "messages.h"

#define MAXI2CBUF MSGLEN
typedef struct __i2c_comm {
	unsigned char buffer[MAXI2CBUF];
        unsigned char bufind;
	unsigned char	buflen;
	unsigned char	event_count;
	unsigned char	status;
	unsigned char	error_code;
	unsigned char	error_count;
	unsigned char outbuffer[MAXI2CBUF];
	unsigned char outbuflen;
	unsigned char outbufind;
	unsigned char slave_addr;
} i2c_comm;

#define I2C_IDLE 0x5
#define I2C_STARTED 0x6
#define	I2C_RCV_DATA 0x7
#define I2C_SLAVE_SEND 0x8

#define I2C_MASTER_START_SEND 0x9
#define I2C_MASTER_START_RECEIVE 0xA
#define I2C_MASTER_RESTART 0xB
#define I2C_MASTER_STOP 0xC
#define I2C_MASTER_SEND_ACK 0xD
#define I2C_MASTER_RECEIVE_ACK 0xE
#define I2C_MASTER_NACK 0xF
#define I2C_MASTER_SEND 0x10
#define I2C_MASTER_RECEIVE 0x11
#define I2C_MASTER_IDLE 0x12
#define I2C_MASTER_RECEIVE_SEND_ADDRESS 0x13
#define I2C_MASTER_RECEIVE_BEGIN_READ 0x14

#define I2C_ERR_THRESHOLD 1
#define I2C_ERR_OVERRUN 0x4
#define I2C_ERR_NOADDR 0x5
#define I2C_ERR_NODATA 0x6
#define I2C_ERR_MSGTOOLONG 0x7
#define I2C_ERR_MSG_TRUNC 0x8

void init_i2c(i2c_comm *);
void i2c_int_handler(void);
void start_i2c_slave_reply(unsigned char,unsigned char *);
void i2c_configure_slave(unsigned char);
void i2c_configure_master(unsigned char);
unsigned char i2c_master_send(unsigned char,unsigned char *);
unsigned char i2c_master_recv(unsigned char);
void i2c_master_set_target(unsigned char);
i2c_comm* i2c_communication(void);

#endif