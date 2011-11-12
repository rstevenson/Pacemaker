//Converts Bytes to Integers and Integers to Bytes

//name : intToBytes
//description: converts an integer to two bytes
//params: int temp - integer to split into bytes
//returns: char * - pointer to the bytes of data
unsigned char *intToBytes(int tint)
{

	unsigned char byte[2];
	int temp;
	temp = tint & 0xFF00;
	temp = temp >> 8;
	byte[0] = temp;
	temp = tint & 0x00FF;
	byte[1] = temp;
	return byte;
}
//name: bytesToInt
//description: converts two bytes to an integer
//params: char b1, b2 - bytes to combine to an integer
//returns: int - integer of the combined bytes
int bytesToInt(unsigned char b1, unsigned char b2)
{
	unsigned int temp;
	temp = b1;
	temp = temp << 8;
	temp |= b2;
	return temp;
}	