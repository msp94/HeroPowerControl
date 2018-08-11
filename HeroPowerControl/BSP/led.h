#ifndef __LED_H__
#define __LED_H__

#include "sys.h"

#define  LED0 PBout(9)
#define  LASER PEout(11)

void Led_Configuration(void);

#endif
