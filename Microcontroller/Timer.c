#include <p18cxxx.h>
#include "Adc.h"

/* Initialize Timer0 */
void timer1_init(void) {
 
   	T1CON               = 0b11110001;

    /* Set TMR1 to fire an interrupt up after 750 ticks (3ms) */ 
    TMR1H               = 0xFB;
    TMR1L               = 0x11;

    /* Clear TMR0IF flag */
    PIR1bits.TMR1IF   = 0;
    /* Enable Timer1 interrupt */
    PIE1bits.TMR1IE   = 1;
}

void timer3_init(void) {
 
   T3CON               = 0b10000001;

    /* Set TMR3 to fire an interrupt up after 250 ticks (1ms) */ 
    TMR3H               = 0xFF;
    TMR3L               = 0x05;

    /* Clear TMR3IF flag */
     PIR2bits.TMR3IF   = 0;
    /* Enable Timer3 interrupt */
    PIE2bits.TMR3IE   = 1;
}

