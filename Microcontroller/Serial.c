//Serial Communications 
//hides the serial communications
//Calculates a checksum based on the data given

#include "TxBuffer.h"
#include "ByteConversion.h"
#include "Globals.h"
#include "Packet.h"
#include <p18cxxx.h>

void initComm()
{
    SPBRG = 16; //calculates the SPGRG value for 16 bit BRG
    /* Configure the pins for UART */
    TRISCbits.TRISC6 = 1;
    TRISCbits.TRISC7 = 1;
    /* Enable asynchronous mode */
    TXSTAbits.SYNC = 0;
    /* Enable serial port */
    RCSTAbits.SPEN = 1;
    /* Enable transmission (sending) */
    TXSTAbits.TXEN = 1;
	TXSTAbits.BRGH = 1;
	BAUDCONbits.BRG16 = 1; //1 if 16 BRG
    /* Enable receiving */
    RCSTAbits.CREN = 1;

    /* Enable interrupt priority */
    RCONbits.IPEN = 1;
    /* Set high priority for sending and receiving interrupt */
    IPR1bits.RCIP = 1;
    IPR1bits.TXIP = 1;
    /* Enable high priority interrupt */
    INTCONbits.GIEH = 1;
    /* Enable receiving interrupt */
    PIE1bits.RCIE = 1;
}

short validHeader(char fncode)// Checks to see if the header is valid as per requirements
{
	if ((fncode == k_egram)||
		(fncode == k_echo)||
		(fncode == k_estop)||
		(fncode == k_pparams))
		return 1;
	else
		return 0;
}

void sendPacket(struct packet commOut)// puts the send packet together in a buffer
{
	char _i;
	TxBUF_ADD(commOut.SYNC);// inserts SYNC variable in first
	TxBUF_ADD(commOut.FnCode);// inserts the FnCode
	for (_i = 0; _i<13; _i++)// Inserts data 
		TxBUF_ADD(commOut.Data[_i]);
	TxBUF_ADD(commOut.ChkSum);// inserts checksum
	PIE1bits.TXIE = 1;	//enables sending interrupt	
}

struct packet receivePacket(void)// takes receiving buffer and puts it in a packet
{
	struct packet temp;
	char chk;
	temp = buffToPacket();
	chk = calcCheckSum(temp.Data);
	if ((validHeader(temp.FnCode)) && 
		(temp.SYNC == k_sync) && 
		(temp.ChkSum == chk))
	{
		return temp;
	}else{
		temp.SYNC = 0x00;
		return temp;
	}	
}

short sendChar(char c)
{
	TxBUF_ADD(c); //adds the char to the buffer
	PIE1bits.TXIE = 1;	//enables sending interrupt	
}

struct stream egramToStream (int m_vrawValue, int f_markerValue)

{
 	char *temp;
   	struct stream streamPackage;
    temp = intToBytes(m_vrawValue);// converts m_vraw into 2 bytes
    streamPackage.streamArray[0]= temp[0];// puts the first byte of m_vraw into an array
    streamPackage.streamArray[1]= temp[1];// puts the second byte of m_vraw into an array
    temp= intToBytes(f_markerValue);// converts f_marker into 2 bytes
 	streamPackage.streamArray[2]= temp[0];// puts the first byte of f_marker into an array
    streamPackage.streamArray[3]= temp[1];// puts the second byte of f_marker into an array
   	return  streamPackage;// returns the structure
}


void sendStream(struct stream streamPackage)// puts the stream packet together in a buffer
{
	char _i;
    for (_i = 0; _i<4; _i++)// Inserts data 
		{
			TxBUF_ADD(streamPackage.streamArray[_i]);// inserts the bytes contained the streamPackage in the buffer
		}
	PIE1bits.TXIE = 1;	//enables sending interrupt	
}