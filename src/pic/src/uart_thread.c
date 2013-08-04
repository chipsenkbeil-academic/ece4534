#include "maindefs.h"
#include <stdio.h>
#include "uart_thread.h"
#include "my_uart.h"
#include "my_leds.h"
#include "my_i2c.h"

// This is a "logical" thread that processes messages from the UART
// It is not a "real" thread because there is only the single main thread
// of execution on the PIC because we are not using an RTOS.

int uart_lthread(uart_thread_struct *uptr, int msgtype, int length, unsigned char *msgbuffer) {
#ifndef __USE18F26J50
    unsigned char address;
    if (msgtype == MSGT_OVERRUN) {
    } else if (msgtype == MSGT_UART_DATA) {
        // print the message (this assumes that the message
        // 		was a printable string)
        msgbuffer[length] = '\0'; // null-terminate the array as a string=

#if I2C_MASTER
        address = msgbuffer[0];
        i2c_master_set_target(address & 0xFE);
        if (address & 0x1) {
            //while(i2c_master_send(0, (void *) 0));
            //while(i2c_master_recv(5) != 0);
            if (i2c_master_recv(5) != 0);
                //DBG1_TOGGLE();

            //uart_send(i2c_communication()->buflen-1, i2c_communication()->buffer+1);
        } else {
            //while(i2c_master_send(length-1, msgbuffer+1) != 0);
            if (i2c_master_send(length-1, msgbuffer+1) != 0);
                //DBG2_TOGGLE();
        }
#else
        start_i2c_slave_reply(length, msgbuffer);
#endif
        
        //DBG0_TOGGLE();
    }
#endif
}