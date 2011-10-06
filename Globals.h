
/* Types */
enum y_pacingState {PERMANENT, TEMPORARY, PACE_NOW, MAGNET,
                    POWER_ON_RESET} ;
                    
enum y_pacingMode  {Off,AAT,VVT,AOO,AAI,VOO,VVI,VDD,DOO,DDI,
                    DDD,AOOR,AAIR,VOOR,VVIR,DOOR,DDIR,DDDR} ;

enum y_magnet  {INPLACE,NOT_INPLACE};    

typedef enum
{
    FALSE=0,
    TRUE
} Bool; 

struct packet
{
	char SYNC;
	char FnCode;
	char Data[13];
	char ChkSum;
};

/* Programmable Parameters */


/* Baud rate for serial communication */
#define BAUD_RATE 57600

/* Frequency of the oscillator */
#define FOSC 4000000

#define k_egram 0x47
#define k_echo  0x49
#define k_estop 0x62
#define k_pparams 0x55
#define k_soh    0x01
#define k_sync   0x16

