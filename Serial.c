//Serial Communications 
//hides the serial communications
//Calculates a checksum based on the data given
char calcCheckSum(char data[13])
{
	char _i;
	char temp;
	for (_i=0;_i<13;_i++)
		temp ^= data[_i];
	return temp;	
}	

/*char validHeader(void)
{
	if ((i_CommIn.FnCode == k_egram)|
		(i_CommIn.FnCode == k_echo)|
		(i_CommIn.FnCode == k_estop)|
		(i_CommIn.FnCode == k_pparams))
		return 0;
	else
		return 1;
}
*/
		