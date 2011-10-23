#include "Buffer.h"

#define e_Mask e_BUF_SIZE - 1
/* Initialize buffer */
void BUF_INIT(struct buffer *buf) {
	buf->head = buf->tail = 0;
}

/* Gets the top element of the buffer */
char BUF_GET(struct buffer *buf) {
	char byte = buf->data[buf->head];
	buf->head=((++buf->head) & e_Mask);
	return byte;
}

int BUF_LENGTH(struct buffer *buf) {

int length;
	if (buf->head <= buf->tail) {
		length = (buf->tail) - (buf->head);
	}else if (buf->tail < buf->head) {
		length = e_BUF_SIZE - ((buf->head) % (buf->tail));
	}
	return length;
}

int BUF_SIZE(struct buffer *buf) {
	return e_BUF_SIZE;
}

short BUF_EMPTY(struct buffer *buf) {
	if((buf->head == buf->tail))
		return 1;
	else
		return 0;
}

short BUF_FULL(struct buffer *buf) {
	if ((((*buf).tail + 1) & e_Mask) == (*buf).head)
		return 1;
	else
		return 0;
}

void BUF_ADD(struct buffer *buf, char byte) {
		buf->data[(*buf).tail] = byte;
		buf->tail=((++(*buf).tail) & e_Mask);
}