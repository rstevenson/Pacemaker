#include "Globals.h"

void TxBUF_INIT(void);

int TxBUF_LENGTH(void);

Bool TxBUF_EMPTY(void);

Bool TxBUF_FULL(void);

void TxBUF_ADD(char byte);

char TxBUF_GET(void);
