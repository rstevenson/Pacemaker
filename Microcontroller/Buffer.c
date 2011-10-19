

/* Initialize buffer size */
#define e_BUF_SIZE 17

/* Define the buffer structure */
struct buffer {
	int head;
	int tail;
	char data[e_BUF_SIZE];
};

/* Initialize buffer */
void BUF_INIT(struct buffer *buf) {
	buf->head = buf->tail = 0;
}

/* Gets the top element of the buffer */
char BUF_GET(struct buffer *buf) {
	char byte = buff.data[buff.head];
	buff.head++;
	return byte;
}

int BUF_LENGTH(struct buffer *buf) {
	return *buf.tail;
}

int BUF_SIZE(struct buffer *buf) {
	return e_BUF_SIZE;
}

enum Bool BUF_EMPTY(struct buffer *buf) {
	if((buf.head == buf.tail) && (buf.data == 0)) {
		return 1;
	}
	else {
		return 0;
	}
}

enum Bool BUF_FULL(struct buffer *buf) {
	if(buf.data == e_BUF_SIZE) {
		return 1;
	}
	else {
		return 0;
	}
}

void BUF_ADD(struct buffer *buf, char byte) {
	if (!(buf.size == e_BUF_SIZE)) {
		buf.tail++;
		buf.data[buf.tail] = byte;
	}
}