//Getting out of scope problems here
//no errors yet. ??
void initializeCom(void);
void processPack(struct packet p);
short buffToPacket(struct packet *commIn, struct buffer *buf);
short sendPacket(char data[13], struct buffer *tbuf);
short sendChar(char c, struct buffer *tbuf);
char calcCheckSum(char data[13]);
short validHeader(char fncode);
