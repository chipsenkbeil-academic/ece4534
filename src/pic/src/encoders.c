#include "encoders.h"
#include <adc.h>
int leftDelta, rightDelta, temp;

void initEncoders()
{
    leftDelta = 0;
    rightDelta = 0;

    // make inputs digital
    ADCON1 = 0xf;

    //interrupt on rising edge
    INTCON2bits.INTEDG0 = 1;
    INTCON2bits.INTEDG1 = 1;

    //set high priority
    INTCON3bits.INT1P = 1;

    //enable interrupts
    INTCONbits.INT0E = 1;
    INTCON3bits.INT1E = 1;


}

void serviceRightEncoderInterrupt()
{
    if(!PORTAbits.RA1)
        rightDelta--;
    else
        rightDelta++;
}
void serviceLeftEncoderInterrupt()
{
    if(!PORTAbits.RA0)
        leftDelta++;
    else
        leftDelta--;
}

int getRightChange()
{
    temp = rightDelta;
    if(temp>16384)
    {
        temp = 16384;
        rightDelta -=16384;
    }
    else if(temp<-16384)
    {
        temp = -16384;
        rightDelta+= 16384;
    }
    else
    {
        rightDelta = 0;
    }
    temp += 16384;

    return temp;
}


int getLeftChange()
{
    temp = leftDelta;
    if(temp>16384)
    {
        temp = 16384;
        leftDelta -=16384;
    }
    else if(temp<-16384)
    {
        temp = -16384;
        leftDelta+= 16384;
    }
    else
    {
        leftDelta = 0;
    }
    temp += 16384;
    
    return temp;
}


