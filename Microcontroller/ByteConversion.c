//Converts Bytes to Integers and Integers to Bytes

//name : intToBytes
//description: converts an integer to two bytes
//params: int temp - integer to split into bytes
//returns: char * - pointer to the bytes of data
char *intToBytes(int temp)
{
	char byte[2];
	temp &= 0xFF00;
	temp >> 8;
	byte[0] = temp;
	temp &= 0x00FF;
	byte[1] = temp;
	return byte;
}
//name: bytesToInt
//description: converts two bytes to an integer
//params: char b1, b2 - bytes to combine to an integer
//returns: int - integer of the combined bytes
int bytesToInt(char b1, char b2)
{
	int temp;
	temp = b1;
	temp = temp << 8;
	temp |= b2;
	return temp;
}	