#ifndef CLIENTBACK_H
#define CLIENTBACK_H
void writeInFifo(void);
static void removeFifo(void);
void fatal(char *s);
#endif
