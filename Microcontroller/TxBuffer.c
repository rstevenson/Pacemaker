#include "Buffer.h"
#include "Globals.h"


struct buffer txbuf;
/* Initialize buffer */

void TxBUF_INIT(void) {
    txbuf.head = txbuf.tail = 0;
}

/* Gets the top element of the buffer */
char TxBUF_GET(void) {
    char byte = txbuf.data[txbuf.head];
    txbuf.head=((++txbuf.head) & e_Mask);
    return byte;
}

int TxBUF_LENGTH(void) {
int length;
    if (txbuf.head <= txbuf.tail) {
        length = (txbuf.tail - txbuf.head);
    }else if (txbuf.tail < txbuf.head) {
        length = e_BUF_SIZE - (txbuf.head - txbuf.tail);
    }
    return length;
}

Bool TxBUF_EMPTY(void) {
    if((txbuf.head == txbuf.tail))
        return true;
    else
        return false;
}

Bool TxBUF_FULL(void) {
    if (((txbuf.tail + 1) & e_Mask) == txbuf.head)
        return true;
    else
        return false;
}

void TxBUF_ADD(char byte) {
        txbuf.data[txbuf.tail] = byte;
        txbuf.tail=((++txbuf.tail) & e_Mask);
}