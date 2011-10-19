
//Function Prototypes for serial.c defined functions

//name: initComm
//description: initializes the uart on the microcontroller
void initComm(void);

//name: setParams
//description: sets the parameters in p
//params: struct params *p - parameters to set
//params: struct packet commIn - packet to get parameters from
//void setParams(struct params *p, struct packet pack);

//name: processFncode
//description: sends different data depending on the fncode/changes state
//params: struct params *p - params to send to other functions
//params: struct packet commIn - packet to get data from
//params: struct buffer *tbuf - buffer to get data from
//void processFncode(struct packet commIn, struct params *p, struct buffer *tbuf);

//name: paramsToPacket
//description: takes programmable params and puts it into a packet
//params: struct params par - params to put in a packet
//params: struct packet commOut - packet to put parameters into
//void paramsToPacket(struct params par, struct packet commOut);

//name: buffToPacket
//description: takes data from a buffer and puts it into a packet
//params: struct packet *commIn - packet to put data in
//params: struct buffer *buf - buffer to take data from
//returns: 1 if proper data received, 0 if not
//short buffToPacket(struct packet *commIn, struct buffer *buf);

//name: sendPacket
//description: sends a packet
//params: struct packet commOut - packet to send
//params: struct buffer *tbuf - sending buffer
void sendPacket(struct packet commOut, struct buffer *tbuf);

//name: receivePacket
//description: receives a packet
//params: struct buffer rbuf - receiving buffer
//return: struct packet
struct packet receivePacket(struct buffer rbuf);

//name: calcCheckSum
//description: calculates the checksum
//params: char data[13] - data bytes to calculate from
//returns: char - calculated checksum byte
char calcCheckSum(char data[13]);
