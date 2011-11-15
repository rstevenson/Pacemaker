/* Turn off watchdog timer */
#pragma config WDT=OFF

/* Set oscillator mode to be HS (high speed) */
#pragma config OSC=HS


#include <p18cxxx.h>
#include "Globals.h"
#include "TxBuffer.h"
#include "RcBuffer.h"
#include "Serial.h"
#include "Packet.h"
#include "Adc.h"
#include "Timer.h"
#include "PaceSense.h"

/* Value of the SPBRG registor for the given baud rate */

struct packet i_CommIn; // structure of package based on requirements given

struct params Parameters;

unsigned long int Tnow = 0;

unsigned int Tm_sVRP=0;

unsigned int Tm_pVRP=0;

unsigned int Tm_vPace=0;

unsigned int len = 0;

unsigned short condS[2] = {0,0}; //sense condition (current and previous)

unsigned short condP[2] = {0,0}; //pace condition (current and previous)

unsigned int opState; //Operation state of the FSM

unsigned int p = 0;

unsigned short t = 0;

enum y_magnet m_magnet;

Bool Timer1 = false;
Bool Timer3 = false;
Bool Send = false;
Bool Receive = false;

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
    if (PIR1bits.RCIF == 1) {
		if (RCSTAbits.OERR){  //checks for error and reinitializes rcbuffer
            RCSTAbits.CREN = 0;
            RCSTAbits.CREN = 1;
            RcBUF_ADD(RCREG);
            RcBUF_INIT();
		}else{
            RcBUF_ADD(RCREG); 	// Add the byte into receiving buffer
            len = RcBUF_LENGTH();
            if ((opState==k_idle)&&(len == 16))
                opState = k_commState;
		}
    }
   // Checks to see if the timer1 interrupt has been fired.
   // if(PIR1bits.TMR1IF == 1)
   // {
//		Timer1 = true; //sets a global boolean
//		PIE1bits.TMR1IE   = 0;
  //  }
	// Checks to see if the timer3 interrupt has been fired.
    if (PIR2bits.TMR3IF==1)
    {
		Timer3 = true; //sets a global boolean
		PIE2bits.TMR3IE   = 0;
	}
	
    /* If the microcontroller sent a byte */
    if (PIR1bits.TXIF && PIE1bits.TXIE) {
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
    Tnow = 0;
    initComm();
    sense_init();
    adc_init();
    RcBUF_INIT();
    TxBUF_INIT();
    opState = k_idle;

    while (1) {
	//Timer 3 = Pacing
	if (Timer3)
	{
	Tnow++;
        condS[1] = condS[0]; //current -> previous
        condP[1] = condP[0];
	if (SenseVRP()&&(condS[1]==0)){ //in sVRP condition ture, set the time
            condS[0] = 1;
            Tm_sVRP = Tnow;
        }else
            condS[0] = 0;
        if (PaceVRP(Parameters.p_vPaceAmp) && condP[1] == 0){ //inpvrp condition true, set time
            condP[0] = 1;
            Tm_pVRP = Tnow;
        }else
            condP[0] = 0;
        if (vPace(Parameters.p_vPaceAmp)){ //start of pace, set time
            Tm_vPace = Tnow;
            p++;
        }
       	pace(Parameters.p_vPaceAmp,Parameters.p_VRP,Tnow);
        Update(Tnow,Tm_sVRP,Tm_pVRP,Tm_vPace,Parameters.p_VRP,Parameters.p_10vPaceWidth);
	setWaitInt(Parameters); 
        t++;
        if (t == 2){
            if (opState == k_stream){
                on_timer1();
                sendStream(egramToStream(get_VVoltage(),get_fmarker()));  // sends a egram package with 4 bytes containing m_vraw and f_marker.
            }
        }else if (t > 2)
            t = 0;
        timer3_init();
	Timer3 = false;

    }
	if (opState == k_commState){
		len = RcBUF_LENGTH();
		if (len==16)//checks to see if the recieving buffer is "full"
		{
			i_CommIn = receivePacket(); // if so it recieves the data from the buffer and puts into a package structure
			if (!i_CommIn.SYNC == 0x00){
				if (i_CommIn.FnCode == k_pparams){
					Parameters = packetToParams(i_CommIn);
                                        setWaitInt(Parameters);
                                        opState = k_idle;
				}
				else if(i_CommIn.FnCode == k_echo){
					sendPacket(paramsToPacket(Parameters));
					opState = k_idle;
				}
				else if(i_CommIn.FnCode == k_egram)
				{
					sendPacket(egramToPacket(k_egram,get_VVoltage(),get_fmarker()));
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
					sendPacket(egramToPacket(k_estop,get_VVoltage(),get_fmarker()));
					opState = k_idle;
					//PIE1bits.TMR1IE = 0;
					//PIR1bits.TMR1IF = 0;
					//T1CONbits.TMR1ON = 0;
					//Timer1 = false;
				}
			}	
		}	
	}
     OSCCONbits.IDLEN = 1;
     Sleep(); //makes the microcontroller sleep after data is processed, waits for more data
    }
}
