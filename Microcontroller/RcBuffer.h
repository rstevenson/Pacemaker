#include "Buffer.h"

extern struct buffer rcbuf;

#define RcBUF_INIT() (rcbuf.head = rcbuf.tail = 0) 

#define RcBUF_LENGTH() ((rcbuf.head <= rcbuf.tail) ? (rcbuf.tail - rcbuf.head) : (e_BUF_SIZE - (rcbuf.head - rcbuf.tail)))

#define RcBUF_ADD(c) do {				\
	rcbuf.data[(rcbuf.tail = rcbuf.tail+1 & e_Mask)] = c;	\
    } while (0)

#define RcBUF_GET() rcbuf.data[(rcbuf.head = rcbuf.head + 1 & e_Mask)]
