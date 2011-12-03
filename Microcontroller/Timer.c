#include <p18cxxx.h>
#include "Adc.h"

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

