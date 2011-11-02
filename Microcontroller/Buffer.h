#include "TxBuffer.h"
#include "RcBuffer.h"

/* Initialize buffer size */
#define e_BUF_SIZE 32
#define e_Mask e_BUF_SIZE - 1

/* Define the buffer structure */
struct buffer 
{
	char head;
	char tail;
	char data[e_BUF_SIZE];
};
