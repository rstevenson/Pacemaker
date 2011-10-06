/* Turn off watchdog timer */
#pragma config WDT=OFF

/* Set oscillator mode to be HS (high speed) */
#pragma config OSC=HS


#include <p18cxxx.h>
#include "Globals.h"
#include "Serial.h"

/*Value of the baud rate as per the data sheet */
#define BAUD_RATE = 57.6;

/* Value of the SPBRG registor for the given baud rate */
#define BRG_VALUE ((int)(((FOSC) / (BAUD_RATE) / 64) - 1))





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
    SPBRG = BRG_VALUE;

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
    /* Append a string to sending buffer */
    BUF_ADDSTR(txbuf, "Hello!\n\r");
    /* Enable sending interrupt */
    PIE1bits.TXIE = 1;

    /* Set counter */
    i = 0;
    while (1) {
	/* If the receiving buffer is not empty and there is enough
	   space in the sending buffer */
	if (!BUF_EMPTY(rcbuf) && BUF_LEN(txbuf) + 4 < BUF_SIZE) {
	    /* Read one byte from the receiving buffer */
            c = BUF_GET(rcbuf);
	    /* Get the first hex code of the byte */ 
	    d = c >> 4;
	    d += (d < 10) ? '0' : ('a' - 10);
	    /* Put the code in sending buffer */
	    BUF_ADD(txbuf, d);
	    /* Get the second hex code of the byte */ 
	    d = c & 0xF;
	    d += (d < 10) ? '0' : ('a' - 10);
	    /* Put the code in sending buffer */
	    BUF_ADD(txbuf, d);
	    
            i += 1;
	    /* If counter == 10, start a new line and clear counter,
	       otherwise, output a white space */
            if (i == 10) {
		BUF_ADD(txbuf, '\n');
		BUF_ADD(txbuf, '\r');
                i = 0;
            } else {
                BUF_ADD(txbuf, ' ');
            }
	    /* Now the sending buffer is non-empty, we enable
	       transmission interrupt to send the contents in sending
	       buffer */
	    PIE1bits.TXIE = 1;
	}
	/* Put the microcontroller in idle mode */
	/* WARNING: If you want to debug the code with MPLAB Sim, you
	   need to remove the following two lines, since MPLAB Sim
	   cannot be waked up in idle mode by UART interrupt */
	OSCCONbits.IDLEN = 1;
        Sleep();
    }
}
