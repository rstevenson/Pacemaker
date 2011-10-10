/* Turn off watchdog timer */
#pragma config WDT=OFF

/* Set oscillator mode to be HS (high speed) */
#pragma config OSC=HS


#include <p18cxxx.h>
#include "Globals.h"
#include "Buffer.h"
#include "Serial.h"

/* Value of the SPBRG registor for the given baud rate */

/* Sending buffer */
struct buffer txbuf;
/* Receiving buffer */
struct buffer rcbuf;

struct packet i_CommIn; // structure of package based on requirements given

//y_magnet m_magnet;
//y_pacingState p_pacingState;
//y_pacingMode p_pacingMode;
//Bool p_hysteresis;
//unsigned int p_hysteresisInterval;
//unsigned int p_lowrateInterval;
//unsigned int p_vPaceAmp;
//unsigned float p_vPaceWidth;
//unsigned int p_VRP;
//unsigned int FNCODE;

/* Interrupt handler function */
void intr_handler(void);

/* Setting interrupt entrance on PIC18 */
#pragma code intr_section = 0x8
void intr_entry(void) {
    _asm goto intr_handler _endasm
}
#pragma code



/* Interrupt handler function */
#pragma interrupt intr_handler
void intr_handler(void) {
    /* If the microcontroller received a byte */ 
    if (PIR1bits.RCIF) {
	/* Add the byte into receiving buffer */
		BUF_ADD(rcbuf, RCREG);
    }
    /* If the microcontroller sent a byte */
    if (PIR1bits.TXIF) {
	/* If there is nothing to send (the sending buffer is empty) */
	if (BUF_EMPTY(txbuf)) {
	    /* Turn off sending interrupt */
	    PIE1bits.TXIE = 0;
	} else {
	    /* Send the first byte in the sending buffer */
	    TXREG = BUF_GET(txbuf);
	}
    }
}

/* Main entrance */
void main(void) {
    initializeCom();
    while (1) {
		if (BUF_FULL(rcbuf))//checks to see if the recieving buffer is full
	    {
		   if (buffToPacket (&i_CommIn,&rcbuf))	// if so it recieves the data from the buffer and puts into a package structure
		   		sendPacket(i_CommIn.Data, &txbuf);// sends the package it recieved back
		   else
		   		sendChar(0x00,&txbuf);// if the buffer is not full, it sends back 0 to the DCM
		}
		OSCCONbits.IDLEN = 1;
    	Sleep(); //makes the microcontroller sleep
    }
}
