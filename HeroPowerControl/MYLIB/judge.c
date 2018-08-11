#include "judge.h"

wl4data w4d;//用于数据转化，将4字节转换成float型
int32_t PowerOldTime = 0;//每次数据接收的系统时间

extPowerHeatData_t	Judge_extPowerHeatData = {0,0,0,60,0,0};

//接收裁判系统功率信息
void Receive104(u8 *candata)
{
	OS_ERR err;
	w4d.c[0] = candata[0];
	w4d.c[1] = candata[1];
	w4d.c[2] = candata[2];
	w4d.c[3] = candata[3];
	Judge_extPowerHeatData.chassisPowerBuffer = w4d.f;
	PowerOldTime = OSTimeGet(&err);
}
