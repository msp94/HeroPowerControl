#include "pid.h"
#include "control.h"
#include "remote.h"
#include "judge.h"
#include "STMGood.h"

ChassisParam_Struct chassisparam = {{0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f},
{0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f},
{0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f},
{0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f},
0,0.0f,0,0,0,0,0,0,0,0};
/**
  * @brief  底盘控制
  * @param  无
  * @retval 无
 **/
void ChassisController(void)
{
	if(remoteparam.RemoteLefUpSpeed>MaxGivenSpeed)  remoteparam.RemoteLefUpSpeed =MaxGivenSpeed;
	if(remoteparam.RemoteLefDoSpeed>MaxGivenSpeed)  remoteparam.RemoteLefDoSpeed =MaxGivenSpeed;
	if(remoteparam.RemoteRigUpSpeed>MaxGivenSpeed)  remoteparam.RemoteRigUpSpeed =MaxGivenSpeed;
	if(remoteparam.RemoteRigDoSpeed>MaxGivenSpeed)  remoteparam.RemoteRigDoSpeed =MaxGivenSpeed;
	
	if(remoteparam.RemoteLefUpSpeed<-MaxGivenSpeed) remoteparam.RemoteLefUpSpeed =-MaxGivenSpeed;
	if(remoteparam.RemoteLefDoSpeed<-MaxGivenSpeed) remoteparam.RemoteLefDoSpeed =-MaxGivenSpeed;
	if(remoteparam.RemoteRigUpSpeed<-MaxGivenSpeed) remoteparam.RemoteRigUpSpeed =-MaxGivenSpeed;
	if(remoteparam.RemoteRigDoSpeed<-MaxGivenSpeed) remoteparam.RemoteRigDoSpeed =-MaxGivenSpeed;
		
	ChassisPidCtr(remoteparam.RemoteLefUpSpeed,remoteparam.RemoteLefDoSpeed,
	-remoteparam.RemoteRigUpSpeed,-remoteparam.RemoteRigDoSpeed);//需具体调试方向
}
/**
  * @brief  底盘pid控制
  * @param  1、左上目标速度 2、右上 3、左下 4、右下
  * @retval 无
 **/
void ChassisPidCtr(int16_t LefUpTarspd,int16_t LefDoTarspd,int16_t RigUpTarspd,int16_t RigDoTarspd)
{
	OS_ERR err;
	#if IS_CHASSIS_PID_DEBUG
	chassisparam.LefUpPid.kp        =P;
	chassisparam.LefUpPid.ki        =I;
	chassisparam.LefUpPid.kd        =D;
	chassisparam.LefDoPid.kp        =P;
	chassisparam.LefDoPid.ki        =I;
	chassisparam.LefDoPid.kd        =D;
	chassisparam.RigUpPid.kp        =P;
	chassisparam.RigUpPid.ki        =I;
	chassisparam.RigUpPid.kd        =D;
	chassisparam.RigDoPid.kp        =P;
	chassisparam.RigDoPid.ki        =I;
	chassisparam.RigDoPid.kd        =D;
	#endif
	
	#if IS_CTR_CAN2_MOTOR_1
	chassisparam.LefUpPid.errNow    =LefUpTarspd-chassisparam.LefUpBackSpeed;
	PID_AbsoluteMode(&chassisparam.LefUpPid);
	chassisparam.LefUpFinalOutValue =chassisparam.LefUpPid.ctrOut;
	#endif
	#if IS_CTR_CAN2_MOTOR_3
	chassisparam.LefDoPid.errNow    =LefDoTarspd-chassisparam.LefDoBackSpeed;
	PID_AbsoluteMode(&chassisparam.LefDoPid);
	chassisparam.LefDoFinalOutValue =chassisparam.LefDoPid.ctrOut;
	#endif
	#if IS_CTR_CAN2_MOTOR_2
	chassisparam.RigUpPid.errNow    =RigUpTarspd-chassisparam.RigUpBackSpeed;
	PID_AbsoluteMode(&chassisparam.RigUpPid);
	chassisparam.RigUpFinalOutValue =chassisparam.RigUpPid.ctrOut;
	#endif
	#if IS_CTR_CAN2_MOTOR_4
	chassisparam.RigDoPid.errNow    =RigDoTarspd-chassisparam.RigDoBackSpeed;
	PID_AbsoluteMode(&chassisparam.RigDoPid);
	chassisparam.RigDoFinalOutValue =chassisparam.RigDoPid.ctrOut;
	#endif
	
	if(OSTimeGet(&err) - PowerOldTime > 6)
	{
		Judge_extPowerHeatData.chassisPowerBuffer = 5.0f;
	}
	if(Judge_extPowerHeatData.chassisPowerBuffer < 50.0f)
	{
		chassisparam.MergeFinalOutValue = 
		abs(chassisparam.LefUpFinalOutValue)+abs(chassisparam.RigUpFinalOutValue)
		+abs(chassisparam.LefDoFinalOutValue)+abs(chassisparam.RigDoFinalOutValue);
		
		chassisparam.LimtOutValue = 
		Judge_extPowerHeatData.chassisPowerBuffer 
		* Judge_extPowerHeatData.chassisPowerBuffer 
		* PowerLimtParam;
		chassisparam.LefUpFinalOutValue = chassisparam.LimtOutValue*chassisparam.LefUpFinalOutValue/chassisparam.MergeFinalOutValue;
		chassisparam.RigUpFinalOutValue = chassisparam.LimtOutValue*chassisparam.RigUpFinalOutValue/chassisparam.MergeFinalOutValue;
		chassisparam.LefDoFinalOutValue = chassisparam.LimtOutValue*chassisparam.LefDoFinalOutValue/chassisparam.MergeFinalOutValue;
		chassisparam.RigDoFinalOutValue = chassisparam.LimtOutValue*chassisparam.RigDoFinalOutValue/chassisparam.MergeFinalOutValue;
	}
	ChassisPwm(chassisparam.LefUpFinalOutValue,chassisparam.RigUpFinalOutValue,
	   chassisparam.LefDoFinalOutValue,chassisparam.RigDoFinalOutValue);
}
/**
  * @brief  PID参数初始化
  * @param  无
  * @retval 无
 **/
void ChassisDataInit(void)
{
	chassisparam.LefUpPid.kp        	=ChassisP;
	chassisparam.LefUpPid.ki        	=ChassisI;
	chassisparam.LefUpPid.kd        	=ChassisD;
	chassisparam.LefUpPid.errILim   	=ChassisILim;
	chassisparam.LefUpPid.MaxOut		=MaxOutValue;
	
	chassisparam.LefDoPid.kp        	=ChassisP;
	chassisparam.LefDoPid.ki        	=ChassisI;
	chassisparam.LefDoPid.kd        	=ChassisD;
	chassisparam.LefDoPid.errILim   	=ChassisILim;
	chassisparam.LefDoPid.MaxOut		=MaxOutValue;

	chassisparam.RigUpPid.kp        	=ChassisP;
	chassisparam.RigUpPid.ki        	=ChassisI;
	chassisparam.RigUpPid.kd        	=ChassisD;
	chassisparam.RigUpPid.errILim   	=ChassisILim;
	chassisparam.RigUpPid.MaxOut		=MaxOutValue;	

	chassisparam.RigDoPid.kp        	=ChassisP;
	chassisparam.RigDoPid.ki			=ChassisI;
	chassisparam.RigDoPid.kd			=ChassisD;
	chassisparam.RigDoPid.errILim   	=ChassisILim;
	chassisparam.RigDoPid.MaxOut		=MaxOutValue;
}
/**
  * @brief  底盘四电机输出量发送
  * @param  0x201、0x202、0x203、0x204
  * @retval 无
 **/
void ChassisPwm(int16_t pwm1,int16_t pwm2,int16_t pwm3,int16_t pwm4)
{
	CanTxMsg tx_message;
	tx_message.StdId=0x200;
	tx_message.IDE=CAN_Id_Standard;
	tx_message.RTR=CAN_RTR_Data;
	tx_message.DLC=0x08;
	tx_message.Data[0]=(unsigned char)(pwm1>>8);
	tx_message.Data[1]=(unsigned char)pwm1;
	tx_message.Data[2]=(unsigned char)(pwm2>>8);
	tx_message.Data[3]=(unsigned char)pwm2;
	tx_message.Data[4]=(unsigned char)(pwm3>>8);
	tx_message.Data[5]=(unsigned char)pwm3;
	tx_message.Data[6]=(unsigned char)(pwm4>>8);
	tx_message.Data[7]=(unsigned char)pwm4;
	CAN_Transmit(CAN2,&tx_message);
}
/**
  * @brief  绝对式PID控制
  * @param  pid参数
  * @retval 无
 **/
void PID_AbsoluteMode(PID_AbsoluteType *pid)
{
	if(pid->kp<0) 		 pid->kp			=-pid->kp;
	if(pid->ki<0) 		 pid->ki			=-pid->ki;
	if(pid->kd<0)			 pid->kd      =-pid->kd;
	if(pid->errILim<0) pid->errILim =-pid->errILim;
	pid->errP=pid->errNow;
	pid->errI+=pid->errNow;
	if(pid->errILim!=0)
	{
		if(pid->errI>pid->errILim)
			pid->errI=pid->errILim;
		else if(pid->errI<-pid->errILim)
			pid->errI=-pid->errILim;
  }
	pid->errD   =pid->errNow-pid->errOld;
	pid->errOld =pid->errNow;
	pid->ctrOut =pid->kp*pid->errP+pid->ki*pid->errI+pid->kd*pid->errD;
	
	if(pid->ctrOut > pid->MaxOut)  pid->ctrOut = pid->MaxOut;
	if(pid->ctrOut < -pid->MaxOut) pid->ctrOut = -pid->MaxOut;
}
