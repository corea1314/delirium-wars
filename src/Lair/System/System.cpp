
#include "System.h"

#include <windows.h>

unsigned long SysMan::GetTime()
{
	return timeGetTime();
}