#include "Globals.h"
#include <p18cxxx.h>
#include "ByteConversion.h"

Bool m_vs[2] = {false,false};
Bool sVRP[2] = {false,false};
Bool pVRP[2] = {false,false};
Bool Pace[2] = {false,false};
unsigned int c_vp[3] = {0,0,0};
unsigned int f_waitInt;
unsigned int senseT = 0;
unsigned int paceT = 0;
Bool senseCond[2] = {false,false};
Bool paceCond[2] = {false,false};

void setSenseTime(unsigned int T);
void setPaceTime(unsigned int T);
void updateCond(void);
Bool In_vPace(unsigned int tn, unsigned int tm, unsigned int p_PW);
Bool paceHeldFor(unsigned int Tn, unsigned int duration);
Bool senseHeldFor(unsigned int Tn, unsigned int duration);

/* Initialize ventricle sense */
void sense_init(void) {
    /* Set ventricle sense (pin33) to be input */
    TRISBbits.TRISB0 = 1;
}

void Update(unsigned int Tn, unsigned int Tms, unsigned int Tmp, unsigned int Tmpace,
        unsigned int VRP, unsigned int p_PW)
{
    /* sVRP updates*/
    sVRP[1] = sVRP[0];
    m_vs[1] = m_vs[0];
    m_vs[0] = PORTBbits.RB0;
    sVRP[0] = ((Tn - Tms) <= VRP);
    /* pVRP updates*/
    pVRP[1] = pVRP[0];
    pVRP[0] = ((Tn - Tmp) <= VRP);
    /* In Pace update*/
    Pace[1] = Pace[0];
    Pace[0] = In_vPace(Tn,Tmpace,p_PW);
}

Bool SenseVRP (void) {
    if ((!sVRP[0])&&(m_vs[1])&&(m_vs[0]))
	return true;
    return false;
}

Bool PaceVRP(unsigned int vPA)
{
    if ((!pVRP[0])&&(c_vp[1]==vPA)&&(!c_vp[2]))
	return true;
    return false;
}		

Bool In_VRP(void)
{
    if ((sVRP[0])||(pVRP[0]))
	return true;
    return false;
}

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

Bool vPace(unsigned int vPA) //will set tm_vPace
{
    if ((c_vp[1] == vPA)&&(!c_vp[2]))
	return true;
    return false;
}	

//will check if its in a pace
Bool In_vPace(unsigned int tn, unsigned int tm, unsigned int p_PW)
{
    if ((tn - tm) <= p_PW)
	return true;
    return false;
}

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

unsigned int getWaitInt(void)
{
    return f_waitInt;
}

void pace(unsigned int vPA, unsigned int pVRP, unsigned int Tn)
{
    updateCond();
    setSenseTime(Tn);
    setPaceTime(Tn);
    c_vp[2] = c_vp[1];
    c_vp[1] = c_vp[0];
    if ((!senseHeldFor(Tn,f_waitInt-pVRP)) && (!paceHeldFor(Tn,f_waitInt-pVRP)))
        c_vp[0] = vPA;
    else if ((Pace[1]) && (!Pace[0]))
        c_vp[0] = 0;
}

void setSenseTime(unsigned int T)
{
    if ((senseCond[0] == true) && (senseCond[1]==false))
        senseT = T;
}

void setPaceTime(unsigned int T)
{
    if ((paceCond[0] == true) && (paceCond[1]==false))
        paceT = T;
}

void updateCond(void)
{
    paceCond[1] = paceCond[0];
    paceCond[0] = ((!In_VRP())&&(c_vp[1] == 0));
    senseCond[1] = senseCond[0];
    senseCond[0] = ((!In_VRP()) && (m_vs[0]==false));
}

Bool paceHeldFor(unsigned int Tn, unsigned int duration)
{
    if (paceCond[0] == true)
        if ((Tn - paceT) >= duration)
            return true;
    return false;
}

Bool senseHeldFor(unsigned int Tn, unsigned int duration)
{
    if (senseCond[0] == true)
        if ((Tn - senseT) >= duration)
            return true;
    return false;
}