/* Turn off watchdog timer */
#pragma config WDT=OFF

/* Set oscillator mode to be HS (high speed) */
#pragma config OSC=HS


#include <p18cxxx.h>
#include "Globals.h"
#include "Buffer.h"
#include "Serial.h"
#include "Packet.h"

/* Value of the SPBRG registor for the given baud rate */

/* Sending buffer */
struct buffer txbuf;
/* Receiving buffer */
struct buffer rcbuf;

struct packet i_CommIn; // structure of package based on requirements given

struct params Parameters;

struct stream  o_CommOut;

unsigned int opState; //Operation state of the FSM

enum y_magnet m_magnet;

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

   // Checks to see if the timer interrupt has been fired.
     if(PIR1bits.TMR1IF == 1)
		{
			sendSream(egramToPacket (1, 2, 5),&txbuf);  // sends a egram package with 4 bytes containing random values of m_vraw and f_marker for testing purposes.
		}
			
			
    /* If the microcontroller received a byte */ 

    /* If the microcontroller received a byte */

    if (PIR1bits.RCIF) {
	/* Add the byte into receiving buffer */
		BUF_ADD(&rcbuf, RCREG);
		opState = k_commState;
    }
    /* If the microcontroller sent a byte */
    if (PIR1bits.TXIF) {
/* If there is nothing to send (the sending buffer is empty) */
		if (BUF_EMPTY(&txbuf)) {
/* Turn off sending interrupt */
			PIE1bits.TXIE = 0;
		} else {
/* Send the first byte in the sending buffer */
			TXREG = BUF_GET(&txbuf);
		}
    }

 			
}

/* Main entrance */
void main(void) {
    initComm();
    BUF_INIT(&rcbuf);
    BUF_INIT(&txbuf);
	opState = k_idle;
    while (1) {
	    while (opState == k_idle);
		if (opState == k_commState){
			if (BUF_LENGTH(&rcbuf)==16)//checks to see if the recieving buffer is full
			{
				i_CommIn = receivePacket(&rcbuf); // if so it recieves the data from the buffer and puts into a package structure
				if (!i_CommIn.SYNC == 0x00){
					if (i_CommIn.FnCode == k_pparams){
						Parameters = packetToParams(i_CommIn);
					}else if(i_CommIn.FnCode == k_echo){
						sendPacket(paramsToPacket(Parameters),&txbuf);
					}else if(i_CommIn.FnCode == k_egram)
					{
						sendPacket(egramToPacket(k_egram,0xffff,'--'),&txbuf);
						opState = k_stream;
					}
				}	
			}
		}else if(opState == k_stream){
			sendPacket(egramToPacket(k_egram,0xff2d,'--'),&txbuf); //just a egram to send (wont be here in actual code)	
			if (BUF_LENGTH(&rcbuf)==16)
			{
				i_CommIn = receivePacket(&rcbuf);
				if (!i_CommIn.SYNC == 0x00){
					if(i_CommIn.FnCode == k_estop)
					{
						sendPacket(egramToPacket(k_estop,0x2343,'--'),&txbuf);
						opState = k_commState;
					}
				}	
			}	
		}		
//
// OSCCONbits.IDLEN = 1;
     // Sleep(); //makes the microcontroller sleep
    }
}
