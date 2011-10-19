char calcCheckSum(char data[13]);
//name: paramsToPacket
//description: takes programmable params and puts it into a packet
//params: struct params par - params to put in a packet
//return: struct packet
struct packet paramsToPacket(struct params par);

struct params packetToParams(struct packet commIn);
//name: buffToPacket
//description: takes data from a buffer and puts it into a packet
//params: struct buffer *buf - buffer to take data from
//return: struct packet
struct packet buffToPacket(struct buffer rbuf);

