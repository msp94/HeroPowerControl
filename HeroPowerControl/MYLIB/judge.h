#ifndef __JUDGE_H__
#define __JUDGE_H__

#include "sys.h"
#include "includes.h"

typedef __packed struct//0x0004实时功率热量数据
{
	float chassisVolt;
	float chassisCurrent;
	float chassisPower;
	float chassisPowerBuffer;
	uint16_t shooterHeat0;
	uint16_t shooterHeat1;
}extPowerHeatData_t;
typedef union
{
	u8 c[4];
	float f;
}wl4data;

extern int32_t PowerOldTime;
extern extPowerHeatData_t		Judge_extPowerHeatData;

void Receive104(u8 *candata);
#endif
