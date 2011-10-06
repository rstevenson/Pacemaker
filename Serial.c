//Serial Communications 
//hides the serial communications
//Calculates a checksum based on the data given

#include "Buffer.h"
#include "Globals.h"

char calcCheckSum(char data[13])
{
	char _i;
	char temp;
	for (_i=0;_i<13;_i++)
		temp ^= data[_i];
	return temp;	
}	

Bool validHeader(struct packet commIn)
{
	if ((commIn.FnCode == k_egram)|
		(commIn.FnCode == k_echo)|
		(commIn.FnCode == k_estop)|
		(commIn.FnCode == k_pparams))
		return TRUE;
	else
		return FALSE;
}