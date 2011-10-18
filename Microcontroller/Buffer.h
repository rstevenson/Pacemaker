/* A simple cyclic buffer implementation */
/* The size of such buffer is 2^5 = 32 */

#define BUF_BITS 5
#define BUF_SIZE (1 << BUF_BITS)
#define BUF_MASK (BUF_SIZE - 1)

/* Initialize buffer */
#define BUF_INIT(buf) do {			\
	(buf).head = (buf).tail = 0;		\
    } while (0)

/* Pop an element from the head of the FIFO buffer */
#define BUF_GET(buf) ((buf).data[(buf).head++ & BUF_MASK])

/* Append an element into the end of the FIFO buffer */
#define BUF_ADD(buf, c) do {				\
	(buf).data[(buf).tail++ & BUF_MASK] = c;	\
    } while (0)

/* Append an array of elements into the buffer */
#define BUF_ADDSTR(buf, str) do {		\
	char __i;				\
	for(__i = 0; (str)[__i]; __i++) {	\
	    BUF_ADD(buf, (str)[__i]);		\
	}					\
    } while(0)

/* Test if the buffer is empty */
#define BUF_EMPTY(buf) (!((((buf).tail-1) ^ (buf).head) & BUF_MASK))

/* Test if the buffer is full */
#define BUF_FULL(buf) (!(((buf).tail ^ (buf).head) & BUF_MASK))

/* Return the number of elements in the buffer */
#define BUF_LEN(buf) (((buf).tail - (buf).head) & BUF_MASK)


/* A struct for the cyclic buffer */
struct buffer {
    char data[BUF_SIZE];
    char head;
    char tail;
};
