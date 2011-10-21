#include "Buffer.h"
/* Initialize buffer */
void BUF_INIT(struct buffer *buf) {
	buf->head = buf->tail = 0;
}

/* Gets the top element of the buffer */
char BUF_GET(struct buffer *buf) {
	char byte = buf->data[buf->head];
	buf->head=((++buf->head)%e_BUF_SIZE);
	return byte;
}

int BUF_LENGTH(struct buffer *buf) {
	return buf->tail;
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
	if ((((*buf).tail + 1) % e_BUF_SIZE) == (*buf).head)
		return 1;
	else
		return 0;
}

void BUF_ADD(struct buffer *buf, char byte) {
		buf->data[(*buf).tail] = byte;
		buf->tail=(++(*buf).tail) % e_BUF_SIZE;
}