

//name: calcCheckSum
//description: calculates a checksum
//params: char data[13] - array of data
//return: char
char calcCheckSum(char data[13]);

//name: paramsToPacket
//description: takes programmable params and puts it into a packet
//params: struct params par - params to put in a packet
//return: struct packet
struct packet paramsToPacket(struct params par);

struct packet egramToPacket(char FnCode, int m_vrawValue, int f_markerValue);

struct stream egramToStream(int m_vrawValue, int f_markerValue);
//name: buffToPacket
//description: takes data from a buffer and puts it into a packet
//params: struct buffer *buf - buffer to take data from
//return: struct packet

struct packet buffToPacket(struct buffer *rbuf);

//name: packetToParams
//description: takes a packet and outputs the parameters
//params: struct packet commIn - packet to get parameters from
//return: struct params

struct params packetToParams(struct packet commIn);

