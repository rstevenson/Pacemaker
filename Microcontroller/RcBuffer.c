#include "Buffer.h"

struct buffer rcbuf;

/* Initialize buffer */
void RcBUF_INIT(void) {
    rcbuf.head = rcbuf.tail = 0;
}

/* Gets the top element of the buffer */
char RcBUF_GET(void) {
    char byte = rcbuf.data[rcbuf.head];
    rcbuf.head=((++rcbuf.head) & e_Mask);
    return byte;
}

int RcBUF_LENGTH(void) {

int length;
    if (rcbuf.head <= rcbuf.tail) {
        length = (rcbuf.tail - rcbuf.head);
    }else if (rcbuf.tail < rcbuf.head) {
        length = e_BUF_SIZE - (rcbuf.head - rcbuf.tail);
    }
    return length;
}

int RcBUF_SIZE(void) {
    return e_BUF_SIZE;
}

short RcBUF_EMPTY(void) {
    if((rcbuf.head == rcbuf.tail))
        return 1;
    else
        return 0;
}

short RcBUF_FULL(void) {
    if (((rcbuf.tail + 1) & e_Mask) == rcbuf.head)
        return 1;
    else
        return 0;
}

void RcBUF_ADD(char byte) {
        rcbuf.data[rcbuf.tail] = byte;
        rcbuf.tail=((++rcbuf.tail) & e_Mask);
}