#ifndef		__DEBUG_H_
#define		__DEBUG_H_


#include <stdarg.h>


#define		DBG_PRINTALL	2
#define		DBG_PRINTERR	1


void setVerbosity(int level);
void debugPrint(int verb, const char* msg, ...);



#endif


