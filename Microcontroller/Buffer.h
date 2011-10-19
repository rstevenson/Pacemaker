void BUF_INIT(struct buffer *buf);

int BUF_LENGTH(struct buffer *buf);

enum Bool BUF_EMPTY(struct buffer *buf);

enum Bool BUF_FULL(struct buffer *buf);

char BUF_GET(struct buffer *buf);

void BUF_ADD(struct buffer *buf, char byte);