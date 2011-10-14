//Converts Bytes to Integers and Integers to Bytes

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

int bytesToInt(char b1, char b2)
{
	int temp;
	temp = b1;
	temp = temp << 8;
	temp |= b2;
	return temp;
}	