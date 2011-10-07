//Serial Communications 
//hides the serial communications
//Calculates a checksum based on the data given

#include "Buffer.h"
#include "Globals.h"
#include <p18cxxx.h>

void initializeCom()
{
    SPBRG = 12;
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
char calcCheckSum(char data[13])
{
	char _i;
	char temp;
	for (_i=0;_i<13;_i++)
		temp ^= data[_i];
	return temp;	
}

short validHeader(char fncode)
{
	if ((fncode == k_egram)|
		(fncode == k_echo)|
		(fncode == k_estop)|
		(fncode == k_pparams))
		return 1;
	else
		return 0;
}

short buffToPack(struct packet *commIn, struct buffer *buf)

// Recieves data from buffer and put it in the package structure 

{
		struct packet temp;
		char i;
		temp.SYNC = BUF_GET(*buf); // inserts the first byte into the sync variable from the program
		temp.FnCode = BUF_GET(*buf);// inserts the second byte in the function code variable
		for (i=0; i<13; i++) // inserts the next 12 bytes into the data character array
		     {
		       temp.Data[i]=BUF_GET(*buf);
		     } 
	
	    temp.ChkSum=BUF_GET(*buf); // inserts the last byte into the checksum variable
		if ((validHeader(temp.FnCode)) && (temp.SYNC == k_sync))
		{
			*commIn=temp;
			return 1;
		}else
			return 0;
} 

short sendData(char data[13], struct buffer *tbuf)
{
	char _i;
	BUF_ADD(*tbuf, k_sync);
	BUF_ADD(*tbuf, k_pparams);
	for (_i = 0; _i<13; _i++)
		BUF_ADD(*tbuf, data[_i]);
	BUF_ADD(*tbuf,calcCheckSum(data));
	PIE1bits.TXIE = 1;		
}


