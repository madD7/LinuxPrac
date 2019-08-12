#include "appInclude.h"
#include "debug.h"


static int verbosity=0;


void setVerbosity(int value)
{
	verbosity = value;
}


void debugPrint(int verb, const char * msg, ...)
{
	if(verb <= verbosity)
	{
		va_list argptr;
		
		va_start(argptr, msg);
		vfprint(stdout, msg, argptr);
		va_end(argptr);
	}
}


