#include "Globals.h"

void RcBUF_INIT(void);

int RcBUF_LENGTH(void);

Bool RcBUF_EMPTY(void);

Bool RcBUF_FULL(void);

void RcBUF_ADD(char byte);

char RcBUF_GET(void);
