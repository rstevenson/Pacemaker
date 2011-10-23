/* Turn off watchdog timer */
#pragma config WDT=OFF

/* Set oscillator mode to be HS (high speed) */
#pragma config OSC=HS


#include <p18cxxx.h>
#include "Globals.h"
#include "Buffer.h"
#include "Serial.h"
#include "Packet.h"
#include "Adc.h"

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
			on_timer1();
			sendStream(egramToStream(get_VVoltage(),'--'),&txbuf);  // sends a egram package with 4 bytes containing random values of m_vraw and f_marker for testing purposes.
			OSCCONbits.IDLEN = 1;
     		Sleep(); //makes the microcontroller sleep
		}

    /* If the microcontroller received a byte */

    if (PIR1bits.RCIF) {
	/* Add the byte into receiving buffer */
		BUF_ADD(&rcbuf, RCREG);
		if ((BUF_LENGTH(&rcbuf) ==16)&&(opState==k_idle))
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
    sense_init();
    adc_init();
    BUF_INIT(&rcbuf);
    BUF_INIT(&txbuf);
	opState = k_idle;
    while (1) {
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
						sendPacket(egramToPacket(k_egram,get_VVoltage(),'--'),&txbuf);
						timer1_init();
						opState = k_stream;
					}
				}	
			}
		}else if(opState == k_stream){
			if (BUF_LENGTH(&rcbuf)==16)
			{
				i_CommIn = receivePacket(&rcbuf);
				if (!i_CommIn.SYNC == 0x00){
					if(i_CommIn.FnCode == k_estop)
					{
						sendPacket(egramToPacket(k_estop,get_VVoltage(),'--'),&txbuf);
						opState = k_commState;
						PIE1bits.TMR1IE = 0;
						PIR1bits.TMR1IF = 0;
						T1CONbits.TMR1ON = 0;
					}
				}	
			}	
		}
	 //sendPacket(egramToPacket(k_egram,0x0000,'--'),&txbuf);			
	 //OSCCONbits.IDLEN = 1;
	 //BAUDCONbits.WUE = 1;
     //Sleep(); //makes the microcontroller sleep
    }
}
