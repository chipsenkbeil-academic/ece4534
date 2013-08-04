#include <adc.h>
#include <delays.h>

#include "my_leds.h"
#include "messages.h"
#include "maindefs.h"
#include "my_adc.h"

//
// CH0 = TOGGLE A1 if less than 150 mV (Ultrasonic) 0x08 shifted over two
// CH1 = TOGGLE A0 if over 2V (0x65 shifted over two)
// CH10 = TOGGLE DBG0 if over 2V (0x65 shifted over two)
// CH8 = TOGGLE DBG2 if over 300mV (0x10 shifted over two)

/*
 * Channels supported for ADC (in order of reading.
 */
static char channels[MAX_CHANNELS] = {
    ADC_CH0, ADC_CH1, ADC_CH8, ADC_CH10
};

/*
 * The index in the channel array of the current channel.
 */
static char currentChannel;

#define GET_CHANNEL(index) \
channels[index]

#define GET_CURRENT_CHANNEL() \
channels[currentChannel]

#define SET_CURRENT_CHANNEL(index) \
currentChannel = index;

/*
 * Move to next channel (or wrap around to first channel).
 * Sets the channel for ADC, which needs to chargeup delay.
 */
#define NEXT_CHANNEL() \
do { \
    if (++currentChannel >= MAX_CHANNELS) \
        currentChannel = 0; \
    SetChanADC(GET_CURRENT_CHANNEL()); \
} while(0);

/*
 * Handles analog to digital conversions.
 *
 * Modified by Robert C. Senkbeil
 */

void init_ADC(adc_thread_struct* adcPtr) {
    int i;
    
    // Initialize to nothing
    for (i = 0; i < MAX_CHANNELS; ++i)
        adcPtr->value[i] = 0;

    OpenADC(
        ADC_FOSC_32 &       // Accuracy, how much time the conversion takes (higher is
                           // more accurate, but consumes more processing power,
                           // making the PIC run more slowly)

        ADC_RIGHT_JUST &   // Right-justify bits

        ADC_0_TAD,         // After switching channels, capacitor needs to discharge,
                           // this indicates that not waiting at all (would be a
                           // problem if immediately tried to use new channel)

        ADC_CH0 &          // Enable RA0R (can be used for A/D)

        ADC_CH1 &          // Enable RA1R (can be used for A/D)

        ADC_INT_ON &       // Enable interrupts from ADC

        ADC_VREFPLUS_EXT & // Enable RA2R (used for input 5+ V)

        ADC_VREFMINUS_EXT, // Enable RA3R (used for ground)

        //0b1111); // All digital
        0b0100); // All analog
        //0b1011);           // Page 250 on datasheet, sets A0-A3 as analog input

    ADC_INT_ENABLE();

    // Use SetChanADC(ADC_CH1) to look at sensor channel
    SET_CURRENT_CHANNEL(0);
    SetChanADC(GET_CURRENT_CHANNEL()); // Use RA0R to get input
    Delay10TCYx(50);
}

void read_ADC(void) {
    ConvertADC(); // Start conversion
    /*while(BusyADC()); // Wait for ADC conversion
    {
        // Senkbeil: In future, want to add a tag to the data being transferred
        //           to indicate that this is an A/D value
        int adcValue = ReadADC(); // Use ADC/1023*(5V<- or closer to 5.07V)
                                  // to calculate the voltage (1023 is max from ten bits)

        ToMainLow_sendmsg(
                MSGLEN,
                MSGT_ADC_RECV_COMPLETE,
                (void*) adcValue);
    }*/
    Delay1KTCYx(1);
}


void stop_ADC(void) {
    CloseADC();
}

// This is a "logical" thread that processes messages from ADC
// It is not a "real" thread because there is only the single main thread
// of execution on the PIC because we are not using an RTOS.

/*
 * Should be receiving a 10-bit value in the form of a two-byte character array.
 *
 * Will store value into A/D persistent data (overwrite oldest value if need to).
 */
int adc_lthread(adc_thread_struct* adcPtr, int msgtype,
                int length, unsigned char* msgbuffer) {
    int adcValue = ReadADC(); // Use ADC/1023*(5V<- or closer to 5.07V)
                              // to calculate the voltage (1023 is max from ten bits)

    // msgbuffer[0] = Lowest two bits are highest bits of 10-bit value
    // msgbuffer[1] = Represents lowest eight bits of the 10-bit value
    //adcPtr->value = (((int) msgbuffer[0] & 0x03) << 8) | ((int) msgbuffer[1]);
    //adcPtr->value = adcPtr->value;
    adcPtr->value[currentChannel] = adcValue;

    // Oepn up next channel to read
    NEXT_CHANNEL();

    Delay1KTCYx(1);
}