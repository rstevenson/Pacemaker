#include "Buffer.h"
#include "ByteConversion.h"
#include "Globals.h"

char calcCheckSum(char data[13])// Calculates the checksum by XOR'ng all the data
{
	char _i;
	char temp;
	temp = data[0];
	for (_i=1;_i<13;_i++)
		temp ^= data[_i];
	return temp;	
}

struct packet paramsToPacket(struct params par)
{
	int temp;
	struct packet commOut;
	char *tByte;
	commOut.SYNC = k_sync;
	commOut.FnCode = k_pparams;
	commOut.Data[0] = par.p_pacingState;
	commOut.Data[1] = par.p_pacingMode;
	commOut.Data[2] = par.p_hysteresis;
	tByte=intToBytes(par.p_hysteresisInterval);
	commOut.Data[3] = tByte[0];
	commOut.Data[4] = tByte[1];
	tByte=intToBytes(par.p_hysteresisInterval);
	commOut.Data[5] = tByte[0];
	commOut.Data[6] = tByte[1];
	tByte=intToBytes(par.p_lowrateInterval);
	commOut.Data[7] = tByte[0];
	commOut.Data[8] = tByte[1];
	tByte=intToBytes(par.p_vPaceAmp);
	commOut.Data[9] = tByte[0];
	commOut.Data[10] = tByte[1];
	tByte=intToBytes(par.p_10vPaceWidth);
	commOut.Data[11] = tByte[0];
	commOut.Data[12] = tByte[1];
	commOut.ChkSum = calcCheckSum(commOut.Data);
	return commOut;
}

struct params packetToParams(struct packet commIn)
{	
	struct params temp;
	temp.p_pacingState = commIn.Data[0];
	temp.p_pacingMode = commIn.Data[1];
	temp.p_hysteresis = commIn.Data[2];
	temp.p_hysteresisInterval=bytesToInt(commIn.Data[3],commIn.Data[4]);
	temp.p_lowrateInterval=bytesToInt(commIn.Data[5],commIn.Data[6]);
	temp.p_vPaceAmp=bytesToInt(commIn.Data[7],commIn.Data[8]);
	temp.p_10vPaceWidth=bytesToInt(commIn.Data[9],commIn.Data[10]);
	temp.p_VRP=bytesToInt(commIn.Data[11],commIn.Data[12]);	
	return temp;	
}

struct packet buffToPacket(struct buffer *rbuf)
// Recieves data from buffer and put it in the package structure 

{
		struct packet temp;
		char i,chk;
		temp.SYNC = BUF_GET(rbuf); // inserts the first byte into the sync variable from the program
		temp.FnCode = BUF_GET(rbuf);// inserts the second byte in the function code variable
		for (i=0; i<13; i++) // inserts the next 12 bytes into the data character array
		     {
		       temp.Data[i]=BUF_GET(rbuf);
		     } 
	    temp.ChkSum=BUF_GET(rbuf); // inserts the last byte into the checksum variable
		return temp; //returns temp
} 

//takes the inputted values of m_vraw and f_marker and puts them into and returns the values in a structure of type stream corrosponding to the referances in the requirements (5.2.4)

struct stream egramToStream (int m_vrawValue, int f_markerValue)

{
	char *temp;
   struct stream streamPackage;
    temp = intToBytes(m_vrawValue);// converts m_vraw into 2 bytes
    streamPackage.streamArray [0]= temp[0];// puts the first byte of m_vraw into an array
    streamPackage.streamArray [1]= temp[1];// puts the second byte of m_vraw into an array
    temp= intToBytes(f_markerValue);// converts f_marker into 2 bytes
	streamPackage.streamArray [2]= temp[0];// puts the first byte of f_marker into an array
    streamPackage.streamArray [3]= temp[1];// puts the second byte of f_marker into an array
   return  streamPackage;// returns the structure
    	
  
} 