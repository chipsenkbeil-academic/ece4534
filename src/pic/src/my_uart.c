#include <p18cxxx.h>
#include "maindefs.h"
#ifndef __XC8
#include <usart.h>
#else
#include <plib/usart.h>
#endif
#include "my_uart.h"
#include "my_leds.h"
#include "my_i2c.h"
#include "checksum.h"

static uart_comm *uc_ptr;

static uart_comm *uc_tx_ptr;

void enableUart(void) {
    RCSTAbits.SPEN = 1;
    TRISCbits.TRISC7 = 1;
    TRISCbits.TRISC6 = 0;
    TXSTAbits.SYNC = 0;

}

void uart_send_int_handler() {
#ifndef __USE18F26J50
    if(uc_tx_ptr->bufptr == uc_tx_ptr->buflen)
    {
        //done transmitting current message, check for new
        unsigned char msgtype;

        unsigned char len  = FromMainLow_recvmsg(MSGLEN, &msgtype, uc_tx_ptr->buffer);

        uc_tx_ptr->bufptr = 0;
        if(len == MSGQUEUE_EMPTY || msgtype != MSGT_UART_TRANSMIT)
        {
            //if no data, disable interrupt
            uc_tx_ptr->buflen = 0;
            PIE1bits.TXIE = 0;
            return;
        }
        else
            uc_tx_ptr->buflen = len;

    }
    if(!BusyUSART())
    {
        WriteUSART(uc_tx_ptr->buffer[uc_tx_ptr->bufptr]);
        uc_tx_ptr->bufptr++;
    }
    
#endif
}

void uart_send(unsigned char length, unsigned char *msg){
    //send message to queue for uart tx handler
    FromMainLow_sendmsg(length, MSGT_UART_TRANSMIT, (void*) msg);
    
    //enable and trigger transmit interrupt
    PIE1bits.TXIE = 1;
}

/**
 * Checks for a header and resets the system if it exists.
 * @param byte The byte to compare to the known header value
 * @return The header value
 */
static unsigned char uart_check_for_header(unsigned char byte) {
    if (byte == HEADER_DATA_BYTE) {
        uc_ptr->buffer[0] = byte;

        uc_ptr->buflen = 1;
        uc_ptr->state = UART_READ_ADDRESS_BYTE;

        return 1;
    } else {
        return 0;
    }
}

static void uart_state_machine(unsigned char newByte) {
    unsigned char header = 0x00, addr = 0x00, msgType = 0x00, state = 0x00;
    
    if(newByte == HEADER_DATA_BYTE)
    {
        uc_ptr->state = UART_READ_HEADER_BYTE;
        uc_ptr->buflen = 0;
    }
    
    uc_ptr->buffer[uc_ptr->buflen] = newByte;

    state = uc_ptr->state;

    header = uc_ptr->buffer[0];
    addr = uc_ptr->buffer[1];
    msgType = uc_ptr->buffer[2];


    switch (uc_ptr->state) {
        case UART_READ_HEADER_BYTE: // Read bytes until header matches
        {
            if (header == HEADER_DATA_BYTE) {
                uc_ptr->buflen++; // Can proceed to reading bytes
                uc_ptr->state = UART_READ_ADDRESS_BYTE;
            }
            break;
        }

        case UART_READ_ADDRESS_BYTE:
        {
            switch (addr) {
                case SENSOR_PIC_READ_ADDRESS: // Case of a sensor read
                case MOTOR_PIC_READ_ADDRESS:  // Case of a motor read
                    // Send the slave address
                    ToMainLow_sendmsg(1, MSGT_UART_DATA,
                            (void *) (uc_ptr->buffer + 1));
                    uc_ptr->buflen = 0;
                    uc_ptr->bufmaxlen = -1;

                    uc_ptr->state = UART_READ_HEADER_BYTE;
                    break;

                default: // All other cases should have additional bytes
                    uc_ptr->buflen++;
                    uc_ptr->state = UART_READ_TYPE_BYTE;
                    break;
            }
            break;
        }

        // Case of getting the message type, which will let us know
        // how long the message should be
        //
        // Message: Header Byte + (Message Type/Data) + Checksum Byte
        case UART_READ_TYPE_BYTE:
        {

            switch (msgType) {
                //
                // Message: HEADER + ADDRESS + TYPE + CHECKSUM
                //
                case MSGT_GET_SENSOR_DATA:
                    uc_ptr->bufmaxlen = GET_SENSOR_DATA_LENGTH;
                    uc_ptr->buflen++;
                    uc_ptr->state = UART_VALIDATE_CHECKSUM_BYTE;
                    break;

                //
                // Message: HEADER + ADDRESS + TYPE + CHECKSUM
                //
                case MSGT_GET_ENCODER_DATA:
                    uc_ptr->bufmaxlen = GET_ENCODER_DATA_LENGTH;
                    uc_ptr->buflen++;
                    uc_ptr->state = UART_VALIDATE_CHECKSUM_BYTE;
                    break;

                //
                // Message: HEADER + ADDRESS + TYPE + DATA1 + DATA2 + CHECKSUM
                //
                case MSGT_SET_MOTOR_DATA:
                    uc_ptr->bufmaxlen = SET_MOTOR_DATA_LENGTH;
                    uc_ptr->buflen++;
                    uc_ptr->state = UART_READ_DATA_BYTES;
                    break;

                default: // Unknown message type!
                    // Reset buffer
                    uc_ptr->buflen = 0;
                    uc_ptr->bufmaxlen = -1;
                    uc_ptr->state = UART_READ_HEADER_BYTE;
                    break;
                }
            break;
        }

        case UART_READ_DATA_BYTES:
        {
            if (uc_ptr->buflen++ >= uc_ptr->bufmaxlen) {
                uc_ptr->state = UART_VALIDATE_CHECKSUM_BYTE;
            }
            break;
        }

        case UART_VALIDATE_CHECKSUM_BYTE:
        {
            //
            // TAKE A CLOSER LOOK AT THIS! THOUGHT I SAW THE CHECKSUM (0X43)
            // BEING PASSED AS THE LENGTH OF THE BYTE ARRAY, WHICH WOULD CAUSE
            // A STACK OVERFLOW!
            //
            unsigned char valid =
                isValidChecksum(uc_ptr->buffer, uc_ptr->buflen, newByte);

            // If valid, send message
            if (valid == 1)
            {
                // Send message without header and checksum bytes
                ToMainLow_sendmsg(
                        uc_ptr->buflen - 1,
                        MSGT_UART_DATA,
                        (void *) (uc_ptr->buffer) + 1);
                //uart_lthread((void *) 0, MSGT_UART_DATA, uc_ptr->buflen, (void *) (uc_ptr->buffer));

                DBG1_TOGGLE();

                if (uc_ptr->buffer[2] == MSGT_SET_MOTOR_DATA) {
                    //DBG1_TOGGLE();
                } else if (uc_ptr->buffer[2] == MSGT_GET_SENSOR_DATA) {
                    //DBG1_TOGGLE();
                }
            } else if (valid == 0) {
                //DBG2_TOGGLE();
                //DBG1_TOGGLE();
            } else {
                // ERROR!
                //DBG2_TOGGLE();
            }

            // Reset buffer
            uc_ptr->buflen = 0;
            uc_ptr->bufmaxlen = -1;

            uc_ptr->state = UART_READ_HEADER_BYTE;
            break;
        }
    }
}

void uart_recv_int_handler() {
#ifdef __USE18F26J50
    if (DataRdy1USART()) {
        unsigned char c = RCREG, addr = 0, msgType = 0, length = 0;
        uc_ptr->buffer[uc_ptr->buflen] = Read1USART();
#else
    if (DataRdyUSART()) {
        unsigned char c = RCREG;
        int i;
        unsigned char temp[MAXUARTBUF];

#endif
        for (i = 0; i < MAXUARTBUF; ++i) {
            temp[i] = uc_ptr->buffer[i];
        }

        uart_state_machine(c);
    }
        
#ifdef __USE18F26J50
    if (USART1_Status.OVERRUN_ERROR == 1) {
#else
    if (USART_Status.OVERRUN_ERROR == 1) {
#endif
        // we've overrun the USART and must reset
        // send an error message for this
        RCSTAbits.CREN = 0;
        RCSTAbits.CREN = 1;
        ToMainLow_sendmsg(0, MSGT_OVERRUN, (void *) 0);
    }
}

void init_uart(uart_comm *uc, uart_comm *uc_tx) {
    uc_ptr = uc;
    uc_tx_ptr = uc_tx;
    
    uc_ptr->buflen = 0;
    uc_ptr->bufmaxlen = -1;
    uc_ptr->state = UART_READ_HEADER_BYTE;

    uc_tx->buflen = 0;
    uc_tx->bufptr = 0;
    uc_tx->bufmaxlen - 1;
    uc_tx->state = UART_READ_HEADER_BYTE;
}