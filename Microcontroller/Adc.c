#include <p18cxxx.h>
#include "Globals.h"
#include "Buffer.h"
#include "Serial.h"
#include "Timer.h"

unsigned int AVoltage;
unsigned int VVoltage;

/* Initialize A/D converter */
void adc_init(void) {
    /* Enable ventricle sensing (MICRO_VENT_SENSE_BLANK_CNTL) */  
    TRISDbits.TRISD4    = 0;
    PORTDbits.RD4       = 1;

    /* Enable atrial sensing (MICRO_ATRIAL_SENSE_BLANK_CNTL) */  
    TRISDbits.TRISD5    = 0;
    PORTDbits.RD5       = 1;

    /* Set SCK to be output pin */
    TRISCbits.TRISC3    = 0;
    /* Set SDI to be input pin */
    TRISCbits.TRISC4    = 1;
    /* Set SDO to be output pin */
    TRISCbits.TRISC5    = 0;
    /* Set SS to be input pin */
    TRISAbits.TRISA5    = 1;
}

/* Read a number from A/D converter */
unsigned int adc_get(void) {
    unsigned int c= 0;
    char i;
    for (i = 0; i < 16; i++) {
        /* Flip clock line (SCK) */
        PORTCbits.SCK   = 0;
        PORTCbits.SCK   = 1;
        /* Shift buffer */
        c<<= 1;
        /* Read a bit and place it at the end of the buffer */
        c|= PORTCbits.SDI;
    }
    return c;
}

/* Wait for A/D converter to be ready for reading */
void adc_start(void) {
    /* Ask if A/D converter is ready */
    PORTCbits.SDO       = 0;
    PORTCbits.SCK       = 0;
    PORTCbits.SCK       = 1;
    PORTCbits.SDO       = 1;
                        
    /* Wait for reply */
    while (!PORTCbits.SDI);
}

/* Turn off the conversation with A/D converter */
void adc_stop(void) {
    PORTCbits.SCK       = 0;
    PORTCbits.SDO       = 0;
}

unsigned int get_VVoltage(void){
	return VVoltage;
}		

/* Timer1 event handler */
void on_timer1(void) {
	unsigned long temp;
    /* Reset Timer1 */
    //timer1_init();
    /* Start conversation with A/D converter */
    T3CONbits.TMR3ON = 0;
    adc_start();
    temp = adc_get();
    VVoltage   = (int)(5000 * temp / 65535);
    temp = adc_get();
    AVoltage   = (int)(5000 * temp / 65535); 
    /* Stop conversation with A/D converter */
    adc_stop();
    T3CONbits.TMR3ON = 1;
}

unsigned int get_AVoltage(void){
	return AVoltage;
}



