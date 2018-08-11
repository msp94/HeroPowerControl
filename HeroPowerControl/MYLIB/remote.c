#include "remote.h"
#include "control.h"
#include "judge.h"
#include "control.h"
#include "monitor.h"
#include "led.h"

RC_Ctl_t RC_Ctl;
unsigned char rc_buffer[25];
RemoteParam_Struct remoteparam;
/**
  * @brief  遥控器处理函数
  * @param  无
  * @retval 无
 **/
void RemoteController(void)
{
	move_xy();
}
/**
  * @brief  速度缓冲函数
  * @param  1、最小缓冲值2、目标速度3、当下速度4、缓冲增量5、缓冲模式
  * @retval 无
 **/
int16_t Ramp_function(int16_t delta_Min,int16_t target,int16_t now, int16_t ramp_Coeff, u8 mode)
{
	int16_t Lenght=target-now;	
	if(abs(Lenght)<delta_Min)
		now=target;
	else
	{
		if(mode == 0)
		{
			if(target < 0)
			{
				if(abs(Lenght) > HIGH_RmeoteSpeed)
				{
					return target / 3;
				}
			}
		}
		if(Lenght>0)
			now+=ramp_Coeff;
		else if(Lenght<0)
			now-=ramp_Coeff;
	}
	return now;
}
/**
  * @brief  将遥控器控制参数转化为底盘目标速度
  * @param  无
  * @retval 无
 **/
void move_xy(void)//底盘控制
{
	int x = 0;
	static int x2 = 0;
	int y = 0;
	static int y2 = 0;
	double sin = 0.0;
	double cos = 0.0;
	int pwm1 = 0;
	int pwm2 = 0;
	int pwm3 = 0;
	int pwm4 = 0;
	int pwm0 = 0;
	
	if(KEY_E & RC_Ctl.key.v)
	{
		x = -FlagCheck(KEY_A & RC_Ctl.key.v)*NORMAL_RemoteSpeed + FlagCheck(KEY_D & RC_Ctl.key.v)*NORMAL_RemoteSpeed;
		y = FlagCheck(KEY_W & RC_Ctl.key.v)*NORMAL_RemoteSpeed - FlagCheck(KEY_S & RC_Ctl.key.v)*NORMAL_RemoteSpeed;
	}
	else 
	{
		x = RC_Ctl.rc.ch0-1024
		- FlagCheck(KEY_A & RC_Ctl.key.v)*HIGH_RmeoteSpeed + FlagCheck(KEY_D & RC_Ctl.key.v)*HIGH_RmeoteSpeed;
		y = RC_Ctl.rc.ch1-1024
		+ FlagCheck(KEY_W & RC_Ctl.key.v)*HIGH_RmeoteSpeed - FlagCheck(KEY_S & RC_Ctl.key.v)*HIGH_RmeoteSpeed;
	}
	x2 = Ramp_function(66,x,x2,6,0);
	y2 = Ramp_function(66,y,y2,6,1);
	/****************************************/
	sin=y2/sqrt(x2*x2+y2*y2);
	cos=x2/sqrt(x2*x2+y2*y2);
	if(ABS(y2)>ABS(x2))
	{
		pwm0=ABS(y2);
	}
	else
	{
		pwm0=ABS(x2);
	}
	pwm1=pwm0*(sin+cos);
	pwm2=pwm0*(sin-cos);
	pwm3=pwm0*(sin-cos);
	pwm4=pwm0*(sin+cos);
	
	remoteparam.RemoteLefUpSpeed =pwm1*MaxGivenSpeed/660;
	remoteparam.RemoteRigUpSpeed =pwm2*MaxGivenSpeed/660;
	remoteparam.RemoteLefDoSpeed =pwm3*MaxGivenSpeed/660;
	remoteparam.RemoteRigDoSpeed =pwm4*MaxGivenSpeed/660;
}
/**
  * @brief  遥控器数据包处理
  * @param  缓存区地址
  * @retval 无
 **/
void RemoteDataPrcess(uint8_t *sbus_rx_buffer)
{
	RC_Ctl.rc.ch0 = (sbus_rx_buffer[0]| (sbus_rx_buffer[1] << 8)) & 0x07ff;             //!< Channel 0
	RC_Ctl.rc.ch1 = ((sbus_rx_buffer[1] >> 3) | (sbus_rx_buffer[2] << 5)) & 0x07ff;     //!< Channel 1
	RC_Ctl.rc.ch2 = ((sbus_rx_buffer[2] >> 6) | (sbus_rx_buffer[3] << 2) |              //!< Channel 2
	(sbus_rx_buffer[4] << 10)) & 0x07ff;
	RC_Ctl.rc.ch3 = ((sbus_rx_buffer[4] >> 1) | (sbus_rx_buffer[5] << 7)) & 0x07ff;     //!< Channel 3			
	RC_Ctl.rc.s1 = ((sbus_rx_buffer[5] >> 4)& 0x000C) >> 2;                             //!< Switch left
	RC_Ctl.rc.s2 = ((sbus_rx_buffer[5] >> 4)& 0x0003);                                  //!< Switch right
	RC_Ctl.mouse.x = sbus_rx_buffer[6] | (sbus_rx_buffer[7] << 8);                      //!< Mouse X axis
	RC_Ctl.mouse.y = sbus_rx_buffer[8] | (sbus_rx_buffer[9] << 8);                      //!< Mouse Y axis
	RC_Ctl.mouse.z = sbus_rx_buffer[10] | (sbus_rx_buffer[11] << 8);                    //!< Mouse Z axis
	RC_Ctl.mouse.press_l = sbus_rx_buffer[12];                                          //!< Mouse Left Is Press ?
	RC_Ctl.mouse.press_r = sbus_rx_buffer[13];                                          //!< Mouse Right Is Press ?
	RC_Ctl.key.v = sbus_rx_buffer[14] | (sbus_rx_buffer[15] << 8);
	framerate.DBUS++;
}
/**
  * @brief  遥控器数据初始化
  * @param  无
  * @retval 无
 **/
void RemoteDataInit(void)
{
	RC_Ctl.rc.ch0 = 1024;
	RC_Ctl.rc.ch1 = 1024;
	RC_Ctl.rc.ch2 = 1024;
	RC_Ctl.rc.ch3 = 1024;
	RC_Ctl.rc.s1 = 3;
	RC_Ctl.rc.s2 = 3;
	RC_Ctl.mouse.x = 0;
	RC_Ctl.mouse.y = 0;
	RC_Ctl.mouse.z = 0;
	RC_Ctl.mouse.press_l = 0;
	RC_Ctl.mouse.press_r = 0;
	RC_Ctl.key.v = 0;
}
