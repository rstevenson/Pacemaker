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
#include "Timer.h"

/* Value of the SPBRG registor for the given baud rate */

struct packet i_CommIn; // structure of package based on requirements given

struct params Parameters;

unsigned int Tnow;

unsigned int Tm_sVRP;

unsigned int Tm_pVRP;

unsigned int Tm_vPace;

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
    /* If the microcontroller received a byte */
       if (PIR1bits.RCIF) {

		if (RCSTAbits.OERR){  //checks for error and reinitializes rcbuffer
			RCSTAbits.CREN = 0;
			RCSTAbits.CREN = 1;
			RcBUF_ADD(RCREG);
			RcBUF_INIT();
		}else{
			RcBUF_ADD(RCREG); 	// Add the byte into receiving buffer
			if ((RcBUF_LENGTH() ==16))
				opState = k_commState;
		}		
    }
   // Checks to see if the timer interrupt has been fired.
     if(PIR1bits.TMR1IF == 1)
		{
			on_timer1();
			sendStream(egramToStream(get_VVoltage(),'--'));  // sends a egram package with 4 bytes containing m_vraw and f_marker.
			OSCCONbits.IDLEN = 1;
     		Sleep(); //makes the microcontroller sleep
		}

		if (PIR2bits.TMR3IF==1)
		{
			Tnow++;
			if (SenseVRP())
				Tm_sVRP = Tnow;
			UpdateVRP(Tnow,Tm_sVRP,Parameters.p_VRP);
			adc_start();
				
		}
    /* If the microcontroller sent a byte */
    if (PIR1bits.TXIF) {
/* If there is nothing to send (the sending buffer is empty) */
		if (TxBUF_EMPTY()) {
/* Turn off sending interrupt */
			PIE1bits.TXIE = 0;
		} else {
/* Send the first byte in the sending buffer */
			TXREG = TxBUF_GET();
		}
    }

 			
}

/* Main entrance */
void main(void) {
    initComm();
    sense_init();
    adc_init();
    RcBUF_INIT();
    TxBUF_INIT();
	opState = k_idle;
    while (1) {
		if (opState == k_commState){
			if (RcBUF_LENGTH()==16)//checks to see if the recieving buffer is "full"
			{
				i_CommIn = receivePacket(); // if so it recieves the data from the buffer and puts into a package structure
				if (!i_CommIn.SYNC == 0x00){
					if (i_CommIn.FnCode == k_pparams){
						Parameters = packetToParams(i_CommIn);
						opState = k_idle;
					}
					else if(i_CommIn.FnCode == k_echo){
						sendPacket(paramsToPacket(Parameters));
						opState = k_idle;
					}
					else if(i_CommIn.FnCode == k_egram)
					{
						sendPacket(egramToPacket(k_egram,get_VVoltage(),'--'));
						timer1_init();
						opState = k_stream;
					}
				}else
					opState = k_idle;	
			}
		}else if(opState == k_stream){
			if (RcBUF_LENGTH()==16)
			{
				i_CommIn = receivePacket();
				if (!i_CommIn.SYNC == 0x00){
					if(i_CommIn.FnCode == k_estop)
					{
						sendPacket(egramToPacket(k_estop,get_VVoltage(),'--'));
						opState = k_idle;
						PIE1bits.TMR1IE = 0;
						PIR1bits.TMR1IF = 0;
						T1CONbits.TMR1ON = 0;
					}
				}
				opState = k_idle;	
			}	
		}			
	 OSCCONbits.IDLEN = 1;
     Sleep(); //makes the microcontroller sleep after data is processed, waits for more data
    }
}
