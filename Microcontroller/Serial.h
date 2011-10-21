
//Function Prototypes for serial.c defined functions

//name: initComm
//description: initializes the uart on the microcontroller
void initComm(void);

//name: sendPacket
//description: sends a packet
//params: struct packet commOut - packet to send
//params: struct buffer *tbuf - sending buffer
void sendPacket(struct packet commOut, struct buffer *tbuf);

//name: receivePacket
//description: receives a packet
//params: struct buffer rbuf - receiving buffer
//return: struct packet
//return: if invalid data, sets the SYNC byte to 0x00
struct packet receivePacket(struct buffer *rbuf);

//name: calcCheckSum
//description: calculates the checksum
//params: char data[13] - data bytes to calculate from
//returns: char - calculated checksum byte
char calcCheckSum(char data[13]);
