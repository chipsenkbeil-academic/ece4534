

#ifndef ENCODERS_H
#define	ENCODERS_H


void initEncoders();

void serviceRightEncoderInterrupt();

void serviceLeftEncoderInterrupt();

int getRightChange();

int getLeftChange();



#endif	/* ENCODERS_H */

