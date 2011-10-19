#include <p18cxxx.h>
#include "Globals.h"
#include "Buffer.h"
#include "Serial.h"


/* Timer0 interval in microseconds */
#define TIMER0                  500000


/* Initialize Timer0 */
void timer0_init(void) {
    /* Clock periods needed to get the time interval */
    unsigned long i     = (float)FOSC * TIMER0 / 1000000;

    /* TMR0ON T08BIT T0CS T0SE PSA T0PS2 T0PS1 T0PS0 */
    T0CON               = 0b10000000;

    /* Set prescaler */
    if (i >> 16) {
        for (i >>= 1; i >> 16; i >>= 1, T0CON++);
    } else {
        T0CON |= 8;
    }

    /* Set TMR0 = 65536 - i, so after i periods, an interrupt will
       fire up */
    i                   = (0xFFFF ^ i) + 1;
    TMR0H               =  i >> 8;
    TMR0L               = i & 0xFF;

    /* Clear TMR0IF flag */
    INTCONbits.TMR0IF   = 0;
    /* Enable Timer0 interrupt */
    INTCONbits.TMR0IE   = 1;
}

/* Initialize ventricle sense */
void sense_init(void) {
    /* Set ventricle sense (pin33) to be input */
    TRISBbits.TRISB0    = 1;
}

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


/* Timer0 event handler */
void on_timer0(void) {
    char                buf[6];
    unsigned int        d;
    float               v;

    /* Reset Timer0 */
    timer0_init();
    
    /* Start conversation with A/D converter */
    adc_start();


    /* Get ventricle voltage */
    v   = 5.0 * adc_get() / 65535;
   
   

    /* Get atrial voltage */
    v   = 5.0 * adc_get() / 65535;
   
    /* Stop conversation with A/D converter */
    adc_stop();

  
}
