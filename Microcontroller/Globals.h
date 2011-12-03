
/* Types */
enum y_pacingState {PERMANENT} ;
                    
enum y_pacingMode  {VVI} ;

enum y_magnet  {INPLACE,NOT_INPLACE};    

typedef enum
{
    false=0,
    true
} Bool; 

struct packet
{
	char SYNC;
	char FnCode;
	char Data[13];
	char ChkSum;
};

struct params
{
	enum y_pacingState p_pacingState;
	enum y_pacingMode p_pacingMode;
	Bool p_hysteresis;
	unsigned int p_hysteresisInterval;
	unsigned int p_lowrateInterval;
	unsigned int p_vPaceAmp;
	unsigned int p_10vPaceWidth;
	unsigned int p_VRP;
};

struct stream
{
	char streamArray[4];
};

/* Baud rate for serial communication */
#define BAUD_RATE 57600

/* Frequency of the oscillator */
#define FOSC 4000000

/*Constants based on requirements*/
#define k_egram 0x47
#define k_echo  0x49
#define k_estop 0x62
#define k_pparams 0x55
#define k_sync   0x16
#define k_streamPeriod 4 //ms
#define k_idle 0x00 //idle state
#define k_commState 0x01 //communication state
#define k_stream 0x02
