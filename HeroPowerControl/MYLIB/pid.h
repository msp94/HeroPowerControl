#ifndef __PID_H__
#define __PID_H__

#include "sys.h"
#include "can.h"

/********绝对式********/
typedef struct
{
	float kp;
	float ki;
	float kd;	
	float errILim;
	
	float errNow; 
	float ctrOut;
	
	float errOld;
	float errP;
	float errI;
	float errD;

	float MaxOut;
}PID_AbsoluteType;
typedef struct
{
	PID_AbsoluteType	LefUpPid;   //左上pid
	PID_AbsoluteType	LefDoPid;	//左下pid
	PID_AbsoluteType	RigUpPid;   //右上pid
	PID_AbsoluteType	RigDoPid;   //右下pid

	int16_t LimtOutValue;//限制功率后的所有电机输出量的总和
	float 	MergeFinalOutValue;//所有电机输出量的总和
	int16_t LefUpFinalOutValue;		//左上最后输出值
	int16_t LefDoFinalOutValue;		//左下最后输出值
	int16_t RigUpFinalOutValue;		//右上最后输出值
	int16_t RigDoFinalOutValue;		//右下最后输出值
	
	int16_t LefUpBackSpeed; //can总线反馈速度
	int16_t LefDoBackSpeed;
	int16_t RigUpBackSpeed;
	int16_t	RigDoBackSpeed;
}ChassisParam_Struct;

extern ChassisParam_Struct chassisparam;;

void PID_AbsoluteMode(PID_AbsoluteType *);
void ChassisDataInit(void);
void ChassisController(void);                       //底盘控制
void ChassisPwm(int16_t,int16_t,int16_t,int16_t);   //发送函数
void ChassisPidCtr(int16_t,int16_t,int16_t,int16_t);//底盘pid控制

#endif
