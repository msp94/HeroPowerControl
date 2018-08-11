#ifndef __MONITOR_H__
#define __MONITOR_H__

#include "sys.h"

typedef struct
{
	u16 leftup;
	u16 rightup;
	u16 leftdown;
	u16 rightdown;
	u16 DBUS;
}FrameRate_Struct;

extern FrameRate_Struct framerate;

#endif
