//name: bytesToInt
//description: converts two bytes to an integer
//params: char b1, b2 - bytes to combine to an integer
//returns: int - integer of the combined bytes
int bytesToInt(unsigned char b1, unsigned char b2);

//name : intToBytes
//description: converts an integer to two bytes
//params: int tint - integer to split into bytes
//returns: char * - pointer to the bytes of data
unsigned char *intToBytes(int tint);
