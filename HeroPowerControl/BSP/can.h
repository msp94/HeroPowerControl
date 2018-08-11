#ifndef __CAN1_H__
#define __CAN1_H__

#include "sys.h"
#include "includes.h"


extern OS_SEM	CAN2_SEM;

extern CanRxMsg rx_message2;

void CAN2_Configuration(void);

#endif 
