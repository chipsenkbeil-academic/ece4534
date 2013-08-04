#ifndef __maindefs
#define __maindefs

#ifdef __XC8
#include <xc.h>
#ifdef _18F45J10
#define __USE18F45J10 1
#else
#ifdef _18F2680
#define __USE18F2680 1
#else
#ifdef _18F26J50
#define __USE18F26J50 1
#endif
#endif
#endif
#else
#ifdef __18F45J10
#define __USE18F45J10 1
#else
#ifdef __18F2680
#define __USE18F2680 1
#else
#ifdef __18F26J50
#define __USE18F26J50 1
#endif
#endif
#endif
#include <p18cxxx.h>
#endif

// Message type definitions
#define MSGT_TIMER0 10
#define MSGT_TIMER1 11
#define MSGT_MAIN1 20
#define	MSGT_OVERRUN 30
#define MSGT_UART_DATA 31
#define MSGT_UART_TRANSMIT 29
#define MSGT_I2C_DBG 41
#define	MSGT_I2C_DATA 40
#define MSGT_I2C_RQST 42
#define MSGT_I2C_MASTER_SEND_COMPLETE 43
#define MSGT_I2C_MASTER_SEND_FAILED 44
#define MSGT_I2C_MASTER_RECV_COMPLETE 45
#define MSGT_I2C_MASTER_RECV_FAILED 46

// Indicates that the A/D has received some data
// that is available to be sent over I2C
#define MSGT_ADC_RECV_COMPLETE 50
#define MSGT_ADC_SEND 51
#define MSGT_UART_TO_I2C 60
#define MSGT_I2C_TO_UART 70

#define MSGT_GET_SENSOR_DATA 0xB1
#define MSGT_GET_ENCODER_DATA 0xA1
#define MSGT_SET_MOTOR_DATA 0xA2

#define MSGT_RETURN_ENCODER_DATA 0xC1
#define MSGT_RETURN_SENSOR_DATA 0xD1
#define MSGT_RETURN_MOTOR_DATA 0xC2

#define HEADER_DATA_BYTE 0xFF

#define SENSOR_PIC_WRITE_ADDRESS 0x4E
#define SENSOR_PIC_READ_ADDRESS 0x4F
#define MOTOR_PIC_WRITE_ADDRESS 0x1E
#define MOTOR_PIC_READ_ADDRESS 0x1F

#define GET_SENSOR_DATA_LENGTH 2        // Addr, MsgType
#define SET_MOTOR_DATA_LENGTH 4         // Addr, MsgType, Left, Right
#define GET_ENCODER_DATA_LENGTH 2       // Addr, MsgType
#define RETURN_ENCODER_DATA_LENGTH 5    // MsgType, FL, BL, FR, BR
#define RETURN_SENSOR_DATA_LENGTH 5     // MsgType, FP, LP, RP, FLine

#define MASTER_MAX_WAIT 4 // The maximum number of timer1 triggers to wait
                          // before assuming a connection has been lost

// PIC TYPES
#define I2C_MASTER 0 // Says that this is the master i2c pic
#define SENSOR_PIC 0 // Says that this is the sensor pic
#define MOTOR_PIC 1  // Says that this is the motor pic

#endif

