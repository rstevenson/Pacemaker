#include "Globals.h"
#include <p18cxxx.h>

Bool m_vs[2] = {false,false};
Bool sVRP[2] = {false,false};
Bool pVRP[2] = {false,false};
int c_vp[3] = {false,false,false};
int f_waitInt;

/* Initialize ventricle sense */
void sense_init(void) {
    /* Set ventricle sense (pin33) to be input */
    TRISBbits.TRISB0 = 1;
}
	
void Update_sVRP(unsigned int Tn, unsigned int Tm, unsigned int VRP){
//1 is previous state of the variable and 0 is current state of the variable
    sVRP[0] = sVRP[1];
    m_vs[0] = m_vs[1];
    m_vs[1] = PORTBbits.RB0;
    sVRP[1] = ((Tn - Tm) <= VRP);
}

void Update_pVRP(unsigned int Tn, unsigned int Tm, unsigned int VRP){
//1 is previous state of the variable and 0 is current state of the variable
    pVRP[0] = pVRP[1];
    pVRP[1] = ((Tn - Tm) <= VRP);
}

short SenseVRP (void) {
    if ((!sVRP[0])&&(m_vs[1])&&(m_vs[0])){
	return 1;
    }
    return 0;
}

short PaceVRP(unsigned int vPA)
{
    if ((!pVRP[0])&&(c_vp[1]==vPA)&&(!c_vp[2]))
	return 1;
    return 0;
}		

short In_VRP(void)
{
    if ((sVRP[0])||(pVRP[0]))
	return 1;
    return 0;
}

int get_fmarker(void){
    if (sVRP[0]){  //In_sVRP
	if (!(sVRP[1])){ //Not prior In_sVRP
            return 'VS';
	}else
            return '()';  //prior In_sVRP
    }else if (pVRP[0]){ //In_pVRP
        if (!(pVRP[1])){ //not prior In_pVRP
            return 'VP';
        }else
            return '()'; //prior In_pVRP
    }else
	return '--'; //Not In_sVRP & not In_pVRP						
}	

short vPace(unsigned int vPA) //will set tm_vPace
{
    if ((c_vp[1] == vPA)&&(!c_vp[2]))
	return 1;
    return 0;
}	

//will check if its in a pace
short In_vPace(unsigned int tn, unsigned int tm, unsigned int p_PW)
{
    if ((tn - tm) <= p_PW)
	return 1;
    return 0;
}

void waitInt(struct params par)
{
    if ((!sVRP[1])&&(sVRP[0])){
        if (par.p_hysteresis)
            f_waitInt =  (par.p_lowrateInterval+par.p_hysteresisInterval);
        else
            f_waitInt =  par.p_lowrateInterval;
    }else if((!pVRP[1])&&(pVRP[0]))
        f_waitInt = par.p_lowrateInterval;
}

int getWaitInt(void)
{
    return f_waitInt;
}
