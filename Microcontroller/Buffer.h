/* Initialize buffer size */
#define e_BUF_SIZE 17

/* Define the buffer structure */
struct buffer 
{
	char head;
	char tail;
	char data[e_BUF_SIZE];
};

void BUF_INIT(struct buffer *buf);

int BUF_LENGTH(struct buffer *buf);

short BUF_EMPTY(struct buffer *buf);

short BUF_FULL(struct buffer *buf);

void BUF_ADD(struct buffer *buf, char byte);

char BUF_GET(struct buffer *buf);
