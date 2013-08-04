/* 
 * File:   my_leds.h
 * Author: Robert C. Senkbeil
 *
 * Created on February 3, 2013, 1:55 PM
 */

#ifndef __MY_LEDS
#define	__MY_LEDS

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

/******************************************************************************/
/* DBG (red) LED functions.                                                   */
/******************************************************************************/

/*
 * Turns on the LED from DBG0.
 */
#define DBG0_ON() \
LATBbits.LATB3 = 1

/*
 * Turns on the LED from DBG1.
 */
#define DBG1_ON() \
LATBbits.LATB4 = 1

/*
 * Turns on the LED from DBG2.
 */
#define DBG2_ON() \
LATBbits.LATB5 = 1

/*
 * Turns off the LED from DBG0.
 */
#define DBG0_OFF() \
LATBbits.LATB3 = 0

/*
 * Turns off the LED from DBG1.
 */
#define DBG1_OFF() \
LATBbits.LATB4 = 0

/*
 * Turns off the LED from DBG2.
 */
#define DBG2_OFF() \
LATBbits.LATB5 = 0

/*
 * Toggles the LED from DBG0 into the opposite state.
 */
#define DBG0_TOGGLE() \
LATBbits.LATB3 = (~(LATBbits.LATB3 & 0x1))

/*
 * Toggles the LED from DBG1 into the opposite state.
 */
#define DBG1_TOGGLE() \
LATBbits.LATB4 = (~(LATBbits.LATB4 & 0x1))

/*
 * Toggles the LED from DBG2 into the opposite state.
 */
#define DBG2_TOGGLE() \
LATBbits.LATB5 = (~(LATBbits.LATB5 & 0x1))

/*
 * Sets LED from DBG0 to on if value is greater
 * than zero.
 *
 * Note: Must be an valid value for comparison.
 */
#define DBG0_SET(value) \
LATBbits.LATB3 = (((value) > 0) & 0x1)

/*
 * Sets LED from DBG1 to on if value is greater
 * than zero.
 *
 * Note: Must be an valid value for comparison.
 */
#define DBG1_SET(value) \
LATBbits.LATB4 = (((value) > 0) & 0x1)

/*
 * Sets LED from DBG2 to on if value is greater
 * than zero.
 *
 * Note: Must be an valid value for comparison.
 */
#define DBG2_SET(value) \
LATBbits.LATB5 = (((value) > 0) & 0x1)

/*
 * Sets DBG LEDS using individual set logic.
 * v1: DBG0
 * v2: DBG1
 * v3: DBG2
 *
 * Note: Must be an valid value for comparison.
 */
#define DBG_ALL_SET(v1, v2, v3) \
do { \
    DBG0_SET(v1); \
    DBG1_SET(v2); \
    DBG2_SET(v3); \
} while(0)

/******************************************************************************/
/* A (yellow) LED functions.                                                  */
/******************************************************************************/

/*
 * Turns on the LED from A0.
 */
#define A0_ON() \
LATCbits.LATC0 = 1

/*
 * Turns on the LED from A1.
 */
#define A1_ON() \
LATCbits.LATC1 = 1

/*
 * Turns on the LED from A2.
 */
#define A2_ON() \
LATCbits.LATC2 = 1

/*
 * Turns off the LED from A0.
 */
#define A0_OFF() \
LATCbits.LATC0 = 0

/*
 * Turns off the LED from A1.
 */
#define A1_OFF() \
LATCbits.LATC1 = 0

/*
 * Turns off the LED from A2.
 */
#define A2_OFF() \
LATCbits.LATC2 = 0

/*
 * Toggles the LED from A0 into the opposite state.
 */
#define A0_TOGGLE() \
LATCbits.LATC0 = (~(LATCbits.LATC0 & 0x1))

/*
 * Toggles the LED from A1 into the opposite state.
 */
#define A1_TOGGLE() \
LATCbits.LATC1 = (~(LATCbits.LATC1 & 0x1))

/*
 * Toggles the LED from A2 into the opposite state.
 */
#define A2_TOGGLE() \
LATCbits.LATC2 = (~(LATCbits.LATC2 & 0x1))

/*
 * Sets LED from A0 to on if value is greater
 * than zero.
 *
 * Note: Must be an valid value for comparison.
 */
#define A0_SET(value) \
LATCbits.LATC0 = (((value) > 0) & 0x1)

/*
 * Sets LED from A1 to on if value is greater
 * than zero.
 *
 * Note: Must be an valid value for comparison.
 */
#define A1_SET(value) \
LATCbits.LATC1 = (((value) > 0) & 0x1)

/*
 * Sets LED from A2 to on if value is greater
 * than zero.
 *
 * Note: Must be an valid value for comparison.
 */
#define A2_SET(value) \
LATCbits.LATC2 = (((value) > 0) & 0x1)

/*
 * Sets A LEDS using individual set logic.
 * v1: A0
 * v2: A1
 * v3: A2
 *
 * Note: Must be an valid value for comparison.
 */
#define A_ALL_SET(v1, v2, v3) \
do { \
    A0_SET(v1); \
    A1_SET(v2); \
    A2_SET(v3); \
} while(0)

/******************************************************************************/
/* Initialization functions for LEDs.                                         */
/******************************************************************************/

/*
 * Flashes the red LEDs.
 */
#define INIT_RED_LEDS() \
do { \
    LATB = 0x3F; \
    DBG0_OFF(); \
    DBG1_OFF(); \
    DBG2_OFF(); \
} while(0)

/*
 * Flashes the yellow LEDs.
 */
#define INIT_YELLOW_LEDS() \
do { \
    LATC = 0x3F; \
    A0_OFF(); \
    A1_OFF(); \
    A2_OFF(); \
} while(0)

#endif
