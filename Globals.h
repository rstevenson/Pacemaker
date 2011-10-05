#ifndef _GLOBALS_H
#define _GLOBALS_H

/* Types */
enum y_pacingState {PERMANENT, TEMPORARY, PACE_NOW, MAGNET,
                    POWER_ON_RESET} p_pacingState;
                    
enum y_pacingMode  {Off,AAT,VVT,AOO,AAI,VOO,VVI,VDD,DOO,DDI,
                    DDD,AOOR,AAIR,VOOR,VVIR,DOOR,DDIR,DDDR} p_pacingMode;

enum y_pacingState  {PERMANENT, TEMPORARY, PACE_NOW, MAGNET, 
POWER_ON_RESET} p_pacingState;

enum y_magnet  {INPLACE,NOT_INPLACE} m_magnet;    

typedef enum
{
    FALSE=0,
    TRUE
} Bool; 

/* Programmable Parameters */
Bool p_hysteresis;
unsigned int p_hysteresisInterval;
unsigned int p_lowrateInterval;
unsigned int p_vPaceAmp;
unsigned float p_vPaceWidth;
unsigned int p_VRP;
unsigned int FNCODE;


/* Baud rate for serial communication */
#define BAUD_RATE 19200

/* Frequency of the oscillator */
#define FOSC 4000000

#define k_egram 0x47
#define k_echo  0x49
#define k_estop 0x62
#define k_pparams 0x55
#define k_soh    0x01
#define k_sync   0x16

struct i_CommIn 
{
char SYNC;
char FnCode;
p_pacingMode;
p_pacingState;
p_hysteresis;
int p_hysteresisInterval;
int p_lowrateInterval;
int p_vPaceAmp;
int p_vPaceWidth;
int p_VRP;  
char Data ChkSum;
};


#endif
