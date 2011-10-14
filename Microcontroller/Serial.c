//Serial Communications 
//hides the serial communications
//Calculates a checksum based on the data given

#include "Buffer.h"
#include "Globals.h"
#include <p18cxxx.h>
#include "ByteConversion.h"

void initializeCom()
{
    SPBRG = (FOSC/(BAUD_RATE*4))-1; //calculates the SPGRG value for 16 bit BRG
    //SPBRG = (FOSC/(BAUD_RATE*16))-1; //8 bit BRG
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
	BAUDCONbits.BRG16 = 1; //0 if 8 BRG
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

void setParams(struct params *p, struct packet pack)
{
	int temp;
	p->p_pacingState = pack.Data[0];
	p->p_pacingMode = pack.Data[1];
	p->p_hysteresis = pack.Data[2];
	p->p_hysteresisInterval=bytesToInt(pack.Data[3],pack.Data[4]);
	p->p_lowrateInterval=bytesToInt(pack.Data[5],pack.Data[6]);
	p->p_vPaceAmp=bytesToInt(pack.Data[7],pack.Data[8]);
	p->p_10vPaceWidth=bytesToInt(pack.Data[9],pack.Data[10]);
	p->p_VRP=bytesToInt(pack.Data[11],pack.Data[12]);		
}

char calcCheckSum(char data[13])// Calculates the checksum by XOR'ng all the data
{
	char _i;
	char temp;
	temp = data[0];
	for (_i=1;_i<13;_i++)
		temp ^= data[_i];
	return temp;	
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

void paramsToPacket(struct params par, struct packet pack)
{
	int temp;
	char *tByte;
	pack.SYNC = k_sync;
	pack.FnCode = k_pparams;
	pack.Data[0] = par.p_pacingState;
	pack.Data[1] = par.p_pacingMode;
	pack.Data[2] = par.p_hysteresis;
	tByte=intToBytes(par.p_hysteresisInterval);
	pack.Data[3] = tByte[0];
	pack.Data[4] = tByte[1];
	tByte=intToBytes(par.p_hysteresisInterval);
	pack.Data[5] = tByte[0];
	pack.Data[6] = tByte[1];
	tByte=intToBytes(par.p_lowrateInterval);
	pack.Data[7] = tByte[0];
	pack.Data[8] = tByte[1];
	tByte=intToBytes(par.p_vPaceAmp);
	pack.Data[9] = tByte[0];
	pack.Data[10] = tByte[1];
	tByte=intToBytes(par.p_10vPaceWidth);
	pack.Data[11] = tByte[0];
	pack.Data[12] = tByte[1];
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
		if ((validHeader(temp.FnCode)) && 
			(temp.SYNC == k_sync) &&
			(temp.ChkSum == chk))
		{
			*commIn=temp; //checks if there is a valid header and sets the buffer equal to temp
			return 1;
		}else
			return 0;
} 

short sendPacket(struct packet commOut, struct buffer *tbuf)// puts the send packet together in a buffer
{
	char _i;
	BUF_ADD(*tbuf, commOut.SYNC);// inserts SYNC variable in first
	BUF_ADD(*tbuf, commOut.FnCode);// inserts K_PPARAMS
	for (_i = 0; _i<13; _i++)// Inserts data 
		BUF_ADD(*tbuf, commOut.Data[_i]);
	BUF_ADD(*tbuf,calcCheckSum(commOut.Data));// inserts checksum
	PIE1bits.TXIE = 1;	//enables sending interrupt	
}

short sendChar(char c, struct buffer *tbuf)
{
	BUF_ADD(*tbuf,c); //adds the char to the buffer
	PIE1bits.TXIE = 1;	//enables sending interrupt	
}

void processFncode(struct packet commIn, struct params *p, struct buffer *tbuf)
{
	struct packet commOut;
	if (commIn.FnCode == k_pparams)
		setParams(&p,commIn); //suspicious pointer conversion
	else if (commIn.FnCode == k_echo){
		paramsToPacket(*p,commOut);
		sendPacket(commOut,&tbuf);
	}
		
}
