// This is where the "user" interrupts handlers should go
// The *must* be declared in "user_interrupts.h"

#include "maindefs.h"
#ifndef __XC8
#include <timers.h>
#else
#include <plib/timers.h>
#endif
#include "user_interrupts.h"
#include "messages.h"
#include "my_leds.h"
#include "my_adc.h"
#include "my_i2c.h"
#include "my_uart.h"
#include <delays.h>

#define TEST_MASTER_I2C 0
#define TEST_MASTER_UART 0

// A function called by the interrupt handler
// This one does the action I wanted for this program on a timer0 interrupt

void timer0_int_handler() {
    unsigned int val;
    int length, msgtype;

    // toggle an LED
#ifdef __USE18F2680
    //LATBbits.LATB0 = !LATBbits.LATB0;
    //DBG2_TOGGLE();
#endif
    // reset the timer
    WriteTimer0(0);
    // try to receive a message and, if we get one, echo it back
    length = FromMainHigh_recvmsg(sizeof(val), (unsigned char *)&msgtype, (void *) &val);
    if (length == sizeof (val)) {
        ToMainHigh_sendmsg(sizeof (val), MSGT_TIMER0, (void *) &val);
    }
}

// A function called by the interrupt handler
// This one does the action I wanted for this program on a timer1 interrupt

void timer1_int_handler() {
    // read the timer and then send an empty message to main()
#ifdef __USE18F2680
    //LATBbits.LATB1 = !LATBbits.LATB1;
#endif

    {
#if (TEST_MASTER_UART || TEST_MASTER_I2C) && I2C_MASTER
        unsigned char dataA1[1] = {
            0xA1
        };
        unsigned char dataA2[3] = {
            0xA2, 0xCD, 0xEF
        };
        unsigned char dataB1[1] = {
            0xB1
        };
        unsigned char uartA1[2] = {
            0x1E, 0xA1
        };
        unsigned char uartA2[4] = {
            0x1E, 0xA1, 0xDE, 0x54
        };
        unsigned char uartB1[2] = {
            0x4E, 0xB1
        };
#endif
        
#if TEST_MASTER_UART && I2C_MASTER
        DBG0_TOGGLE();
        uart_send(4, uartA2);
        Delay10KTCYx(255);
#endif

#if TEST_MASTER_I2C && I2C_MASTER
        DBG1_TOGGLE();

        i2c_master_set_target(0x1E);
        while (i2c_master_send(1, dataA1) != 0);
        while (i2c_master_recv(5) != 0);
        //Delay10KTCYx(255);

        while (i2c_master_send(3, dataA2) != 0);
        while (i2c_master_recv(1) != 0);
        //Delay10KTCYx(255);

        i2c_master_set_target(0x4E);
        while (i2c_master_send(1, dataB1) != 0);
        while (i2c_master_recv(5) != 0);
        //Delay10KTCYx(255);
#endif
    }

    ToMainLow_sendmsg(0, MSGT_TIMER1, (void *) 0);

    // reset the timer
    WriteTimer1(0);
}

void adc_int_handler() {
    ToMainHigh_sendmsg(0, MSGT_ADC_RECV_COMPLETE, (void *) 0);
}