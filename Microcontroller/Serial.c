//Serial Communications 
//hides the serial communications
//Calculates a checksum based on the data given

#include "Buffer.h"
#include "Globals.h"
#include <p18cxxx.h>

void initializeCom()
{
    SPBRG = (FOSC/(BAUD_RATE*16))-1; //calculates the SPGRG value
    /* Configure the pins for UART */
    TRISCbits.TRISC6 = 1;
    TRISCbits.TRISC7 = 1;

    /* Enable serial port */
    RCSTAbits.SPEN = 1;
    /* Enable asynchronous mode */
    TXSTAbits.SYNC = 0;
    /* Enable transmission (sending) */
    TXSTAbits.TXEN = 1;
	TXSTAbits.BRGH = 1;
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

    /* Initialize receiving buffer */

    /* Enable sending interrupt */
}

void processPack(struct packet p)
{
	
}
char calcCheckSum(char data[13])// Calculates the checksum by XOR'ng all the data
{
	char _i;
	char temp;
	for (_i=0;_i<13;_i++)
		temp ^= data[_i];
	return temp;	
}

short validHeader(char fncode)// Checks to see if the header is valid as per requirements
{
	if ((fncode == k_egram)|
		(fncode == k_echo)|
		(fncode == k_estop)|
		(fncode == k_pparams))
		return 1;
	else
		return 0;
}

short buffToPacket(struct packet *commIn, struct buffer *buf)

// Recieves data from buffer and put it in the package structure 

{
		struct packet temp;
		char i,chk;
		temp.SYNC = BUF_GET(*buf); // inserts the first byte into the sync variable from the program
		temp.FnCode = BUF_GET(*buf);// inserts the second byte in the function code variable
		for (i=0; i<13; i++) // inserts the next 12 bytes into the data character array
		     {
		       temp.Data[i]=BUF_GET(*buf);
		     } 
	
	    temp.ChkSum=BUF_GET(*buf); // inserts the last byte into the checksum variable
		chk = calcCheckSum(temp.Data);
		if ((validHeader(temp.FnCode)) && (temp.SYNC == k_sync) && (temp.ChkSum == chk))
		{
			*commIn=temp; //checks if there is a valid header and sets the buffer equal to temp
			return 1;
		}else
			return 0;
} 

short sendPacket(char data[13], struct buffer *tbuf)// puts the send packet together in a buffer
{
	char _i;
	BUF_ADD(*tbuf, k_sync);// inserts SYNC variable in first
	BUF_ADD(*tbuf, k_pparams);// inserts K_PPARAMS
	for (_i = 0; _i<13; _i++)// Inserts data 
		BUF_ADD(*tbuf, data[_i]);
	BUF_ADD(*tbuf,calcCheckSum(data));// inserts checksum
	PIE1bits.TXIE = 1;	//enables sending interrupt	
}

short sendChar(char c, struct buffer *tbuf)
{
	BUF_ADD(*tbuf,c); //adds the char to the buffer
	PIE1bits.TXIE = 1;	//enables sending interrupt	
}
