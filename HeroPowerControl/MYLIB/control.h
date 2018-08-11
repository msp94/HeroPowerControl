#ifndef __CONTROL_H__
#define __CONTROL_H__

/*---------------------遥控参数---------------------*/
#define HIGH_RmeoteSpeed		660
#define NORMAL_RemoteSpeed		200
/*---------------------云台参数---------------------*/
/*---------------------底盘参数---------------------*/
#define IS_CHASSIS_PID_DEBUG	0

#define PowerLimtParam			6.0f
#define MaxGivenSpeed			7500

#define ChassisP				6.0f   //3.0  //底盘pid
#define ChassisI				0.1f   //0.08
#define ChassisD				0.0f
#define ChassisILim				4000
#define MaxOutValue				12000  //最大输出量
/*---------------------摩擦轮参数---------------------*/
/*---------------------转盘参数---------------------*/
/*---------------------取球参数---------------------*/
/*---------------------电机控制---------------------*/
#define IS_CTR_CAN2_MOTOR_1		1
#define IS_CTR_CAN2_MOTOR_2		1
#define IS_CTR_CAN2_MOTOR_3		1
#define IS_CTR_CAN2_MOTOR_4		1

/*---------------------任务开启---------------------*/
#define IS_OPEN_PRINT_TASK		1
#define IS_OPEN_CAN1_TASK		1
#define IS_OPEN_CAN2_TASK		1
#define IS_OPEN_USART2_TASK		1
#define IS_OPEN_USART6_TASK		1
#define IS_OPEN_LED_TASK		1
#define IS_OPEN_CONTROL_TASK	1
#define IS_OPEN_MONITOR_TASK	1

#endif
