/* 
 * File:   my_adc.h
 * Author: Robert C. Senkbeil
 *
 * Created on February 1, 2013, 11:42 PM
 */

#ifndef __my_adc
#define	__my_adc

// Represents total number of readings
// that can be stored in a structure
#define MAX_ADC_READINGS_STORED 10

// Represents total channels supported
#define MAX_CHANNELS 4

typedef struct __adc_thread_struct {
        unsigned int value[MAX_CHANNELS];
} adc_thread_struct;

/*
 * Enables the A/D converter.
 */
void init_ADC(adc_thread_struct* adcPtr);

/*
 * Reads a value from the A/D converter
 * ands sends the data to main().
 */
void read_ADC(void);

/*
 * Disables the A/D converter.
 */
void stop_ADC(void);

int adc_lthread(adc_thread_struct* adcPtr, int msgtype,
                int length, unsigned char *msgbuffer);

#endif

