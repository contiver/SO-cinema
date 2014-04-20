#ifndef INITIALIZER_H
#define INITIALIZER_H 

#include "dbAccess.h"

/* To be changed by every different implementation. Used to initialize
 * each particular requirements (e.g. FIFOs).
 */
void initializeClient(void);
void terminateClient(void);

#endif
