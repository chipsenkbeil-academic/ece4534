#include "maindefs.h"
#include <stdio.h>
#include "messages.h"
#include "timer1_thread.h"
#include "my_i2c.h"
#include "my_adc.h"
#include "my_leds.h"

unsigned char testmsg[MSGLEN];

void init_timer1_lthread(timer1_thread_struct *tptr) {
    tptr->msgcount = 0;
}

// This is a "logical" thread that processes messages from TIMER1
// It is not a "real" thread because there is only the single main thread
// of execution on the PIC because we are not using an RTOS.

int timer1_lthread(timer1_thread_struct *tptr, int msgtype, int length, unsigned char *msgbuffer) {
    ////////////////////////////////////////////////////////////////////////////
    // SENSOR AND MOTOR PICS SHOULD ATTEMPT TO RESTORE THEIR STATES IF WRONG
    ////////////////////////////////////////////////////////////////////////////
#if !I2C_MASTER
    tptr->msgcount = 0;
    switch (msgbuffer[0]) {
#if SENSOR_PIC
        case MSGT_GET_SENSOR_DATA: A0_ON(); break;

        default: // Restore state
            msgbuffer[0] = MSGT_GET_SENSOR_DATA;
            A0_OFF();
            break;
#elif MOTOR_PIC
        case MSGT_GET_ENCODER_DATA: break;
        case MSGT_SET_MOTOR_DATA: break;

        default: // Restore state
            msgbuffer[0] = MSGT_GET_ENCODER_DATA;
            break;
#endif
    }
#endif

    ////////////////////////////////////////////////////////////////////////////
    // SENSOR PIC SHOULD ATTEMPT TO READ AN ADC PIN
    ////////////////////////////////////////////////////////////////////////////
#if SENSOR_PIC
    DBG1_TOGGLE();
    read_ADC();
#endif

#if MOTOR_PIC
    DBG1_TOGGLE();
#endif


#if I2C_MASTER
    if (++tptr->msgcount >= MASTER_MAX_WAIT) {
        unsigned char dataFullStop[3] = {
            0xA2, 0x00, 0x00
        };
        
        DBG2_TOGGLE();

        i2c_master_set_target(0x1E);
        while (i2c_master_send(3, dataFullStop) != 0);

        tptr->msgcount = 0;
    }
#endif
    return 0;
}