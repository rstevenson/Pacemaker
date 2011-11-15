#include "Buffer.h"

extern struct buffer txbuf;

#define TxBUF_INIT() (txbuf.head = txbuf.tail = 0) 

#define TxBUF_LENGTH() ((txbuf.head <= txbuf.tail) ? (txbuf.tail - txbuf.head) : (e_BUF_SIZE - (txbuf.head - txbuf.tail)))

#define TxBUF_ADD(c) (txbuf.data[(txbuf.tail = txbuf.tail+1 & e_Mask)] = c)

#define TxBUF_GET() (txbuf.data[(txbuf.head = txbuf.head+1 & e_Mask)])

#define TxBUF_EMPTY() (txbuf.head == txbuf.tail)