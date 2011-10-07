//Getting out of scope problems here
//no errors yet. ??
void initializeCom();
void processPack(struct packet p);
short buffToPack(struct packet *commIn, struct buffer *buf);
short sendData(char data[13], struct buffer *tbuf);
char calcCheckSum(char data[13]);
short validHeader(char fncode);
