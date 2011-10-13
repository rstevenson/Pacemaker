
//Function Prototypes for serial.c defined functions
void initializeCom(void);
void setParams(struct params *p, struct packet pack);
short sendPacket(struct packet commOut, struct buffer *tbuf);
void processFncode(struct packet commIn, struct params *p, struct buffer *tbuf);
void paramsToPacket(struct params par, struct packet pack);
short buffToPacket(struct packet *commIn, struct buffer *buf);
short sendPacket(struct packet commOut, struct buffer *tbuf);
short sendChar(char c, struct buffer *tbuf);
char calcCheckSum(char data[13]);
short validHeader(char fncode);
