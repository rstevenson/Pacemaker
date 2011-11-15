#include "Globals.h"
#include <p18cxxx.h>
#include "ByteConversion.h"
#include "Timer.h"

Bool m_vs[2] = {false,false}; //sense
Bool sVRP[2] = {false,false}; //in sense VRP
Bool pVRP[2] = {false,false}; //in pace VRP
Bool Pace[2] = {false,false}; //in a pace
unsigned int c_vp[3] = {0,0,0};
unsigned int f_waitInt; //wait interval
unsigned long int senseT = 0; //held for sense time
unsigned long int paceT = 0; //held for pace time
Bool senseCond[2] = {false,false}; //held for sense condition
Bool paceCond[2] = {false,false}; //held for pace condition
Bool pHeld = false; //pace is held
Bool sHeld = false; //sense is held

void setSenseTime(unsigned long T);
void setPaceTime(unsigned long T);
void updateCond(void);
Bool In_vPace(unsigned long tn, unsigned long tm, unsigned int p_PW);
Bool paceHeldFor(unsigned long Tn, unsigned int duration);
Bool senseHeldFor(unsigned long Tn, unsigned int duration);

/* Initialize ventricle sense */
void sense_init(void) {
    /* Set ventricle sense (pin33) to be input */
    TRISBbits.TRISB0 = 1;
	timer3_init();
}

//updates the m_vs, sVRP, pVRP, and Pace variables
void Update(unsigned long Tn, unsigned long Tms, unsigned long Tmp, unsigned long Tmpace,
        unsigned int VRP, unsigned int p_PW)
{
    /* sVRP updates*/
    sVRP[1] = sVRP[0];
    m_vs[1] = m_vs[0];
    m_vs[0] = PORTBbits.RB0;
    if (Tms > 0)
    	sVRP[0] = ((Tn - Tms) <= VRP);
    else
    	sVRP[0] = 0;
    /* pVRP updates*/
    pVRP[1] = pVRP[0];
    if (Tmp > 0)
    	pVRP[0] = ((Tn - Tmp) <= VRP);
    else
    	pVRP[0] = 0;
    /* In Pace update*/
    Pace[1] = Pace[0];
    if (Tmpace > 0)
    	Pace[0] = In_vPace(Tn,Tmpace,p_PW);
    else
    	Pace[0] = 0;
}

//checks if its in sense VRP
Bool SenseVRP (void) {
    if ((!sVRP[0])&&(m_vs[1])&&(m_vs[0]))
		return true;
    return false;
}

//checks if its in pace VRP
Bool PaceVRP(unsigned int vPA)
{
    if ((!pVRP[0])&&(c_vp[1]==vPA)&&(!c_vp[2]))
		return true;
    return false;
}		

//checks if its in VRP
Bool In_VRP(void)
{
    if ((sVRP[0])||(pVRP[0]))
		return true;
    return false;
}

//outputs f_marker
int get_fmarker(void){
    if (sVRP[0]){  //In_sVRP
	if (!(sVRP[1])){ //Not prior In_sVRP
            return bytesToInt('V','S');
	}else
            return bytesToInt('(',')');  //prior In_sVRP
    }else if (pVRP[0]){ //In_pVRP
        if (!(pVRP[1])){ //not prior In_pVRP
            return bytesToInt('V','P');
        }else
            return bytesToInt('(',')'); //prior In_pVRP
    }else
	return bytesToInt('-','-'); //Not In_sVRP & not In_pVRP
}	

//if the in pace condition is met
Bool vPace(unsigned int vPA) //will set tm_vPace
{
    if ((c_vp[1] == vPA)&&(!c_vp[2]))
		return true;
    return false;
}	

//will check if its in a pace
Bool In_vPace(unsigned long tn, unsigned long tm, unsigned int p_PW)
{
    if (tm > 0)
    if ((tn - tm) <= p_PW)
		return true;
    return false;
}

//sets f_waitInt depending on programmable Parameters
//and current and past VRP values
void setWaitInt(struct params par)
{
    if ((!sVRP[1])&&(sVRP[0])){
        if (par.p_hysteresis)
            f_waitInt =  (par.p_lowrateInterval+par.p_hysteresisInterval);
        else
            f_waitInt =  par.p_lowrateInterval;
    }else if((!pVRP[1])&&(pVRP[0]))
        f_waitInt = par.p_lowrateInterval;
}

//outputs c_vp
//checks at the end of f_wait-VRP if the "held for" conditions were held
//sets p/sHeld if the conditions go false
void pace(unsigned int vPA, unsigned int VRP, unsigned long Tn)
{
    updateCond();
    setSenseTime(Tn);
    setPaceTime(Tn);
    c_vp[2] = c_vp[1];
    c_vp[1] = c_vp[0];
    if ((Tn-paceT) == (f_waitInt-VRP))
	{
    	if ((!pHeld) && (!sHeld))
        	c_vp[0] = vPA;
        pHeld = sHeld = true;
    }else if((Tn-paceT) != (f_waitInt-VRP)){
    	if (paceCond[0] == 0)
    		pHeld = false;
    	if (senseCond[0] == 0)
    		sHeld = false;
	}        
	if ((Pace[1]) && (!Pace[0]))
        c_vp[0] = 0;	 
}

//sets start time for "held for"
void setSenseTime(unsigned long T)
{
    if ((senseCond[0] == true) && (senseCond[1]==false))
        senseT = T;
}

//sets start time for "held for"
void setPaceTime(unsigned long T)
{
    if ((paceCond[0] == true) && (paceCond[1]==false))
        paceT = T;
}


// updates the "held for" conditions
void updateCond(void)
{
    paceCond[1] = paceCond[0];
    paceCond[0] = ((!In_VRP())&&(c_vp[1] == 0));
    senseCond[1] = senseCond[0];
    senseCond[0] = ((!In_VRP()) && (m_vs[0]==false));
}