/* Turn off watchdog timer */
#pragma config WDT=OFF

/* Set oscillator mode to be HS (high speed) */
#pragma config OSC=HS


#include <p18cxxx.h>
#include "Globals.h"
#include "Buffer.h"
#include "Serial.h"

/* Value of the baud rate as per the data sheet */
#define BAUD_RATE = 57.6;

/* Value of the SPBRG registor for the given baud rate */
/* Sending buffer */
struct buffer txbuf;
/* Receiving buffer */
struct buffer rcbuf;

struct packet i_CommIn;

//y_magnet m_magnet;
//y_pacingState p_pacingState;
//y_pacingMode p_pacingMode;
Bool p_hysteresis;
unsigned int p_hysteresisInterval;
unsigned int p_lowrateInterval;
unsigned int p_vPaceAmp;
unsigned float p_vPaceWidth;
unsigned int p_VRP;
unsigned int FNCODE;

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
    char c;
    char d;
    char i;

    /* Set baud rate */
    SPBRG = 42;
    /* Configure the pins for UART */
    TRISCbits.TRISC6 = 1;
    TRISCbits.TRISC7 = 1;

    /* Enable serial port */
    RCSTAbits.SPEN = 1;
    /* Enable asynchronous mode */
    TXSTAbits.SYNC = 0;
    /* Enable transmission (sending) */
    TXSTAbits.TXEN = 1;
    /* Enable receiving */
    RCSTAbits.CREN = 1;

    /* Enable interrupt priority */
    RCONbits.IPEN = 1;
    /* Set high priority for sending and receiving interrupt */
    IPR1bits.RCIP = 1;
    IPR1bits.TXIP = 1;
    /* Enable high priority interrupt */
    INTCONbits.GIEH = 1;
    /* Enable receiving interrupt */
    PIE1bits.RCIE = 1;

    /* Initialize receiving buffer */
    BUF_INIT(rcbuf);
    /* Initialize sending buffer */
    BUF_INIT(txbuf);
    /* Enable sending interrupt */
    PIE1bits.TXIE = 1;

    /* Set counter */
    i = 0;
    while (1) {
	    if BUF_FULL(rcbuf)
	    {
		   //disable receiving interrupts
		   PIE1bits.RCIE = 0;
		   //calcCheckSum(i_CommIn.Data);
		   //enable receiving interrupts
		   PIE1bits.RCIE = 1;
		}
	/* If the receiving buffer is not empty and there is enough
	   space in the sending buffer */
	/* Put the microcontroller in idle mode */
	/* WARNING: If you want to debug the code with MPLAB Sim, you
	   need to remove the following two lines, since MPLAB Sim
	   cannot be waked up in idle mode by UART interrupt */
	OSCCONbits.IDLEN = 1;
        Sleep();
    }
}
