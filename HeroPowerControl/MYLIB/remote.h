#ifndef __REMOTE_H__
#define __REMOTE_H__

#include "sys.h"
#include "includes.h"

/* ----------------------- RC Channel Definition---------------------------- */
#define RC_CH_VALUE_MIN    ((uint16_t)364 )
#define RC_CH_VALUE_OFFSET ((uint16_t)1024)
#define RC_CH_VALUE_MAX    ((uint16_t)1684)
/* ----------------------- RC Switch Definition----------------------------- */
#define UP   ((uint16_t)1)
#define MID  ((uint16_t)3)
#define DOWN ((uint16_t)2)
/* ----------------------- PC Key Definition-------------------------------- */
#define KEY_B         0x8000
#define KEY_V         0x4000
#define KEY_C         0x2000
#define KEY_X         0x1000
#define KEY_Z         0x0800
#define KEY_G         0x0400
#define KEY_F         0x0200
#define KEY_R         0x0100
#define KEY_E         0x0080
#define KEY_Q         0x0040
#define KEY_CTRL      0x0020
#define KEY_SHIFT     0x0010
#define KEY_D         0x0008
#define KEY_A         0x0004
#define KEY_S         0x0002
#define KEY_W         0x0001
/* ----------------------- Data Struct ------------------------------------- */
typedef struct
{
	int16_t RemoteLefUpSpeed;    //左上给定速度
	int16_t RemoteLefDoSpeed;    //左下给定速度
	int16_t RemoteRigUpSpeed;    //右上给定速度
	int16_t RemoteRigDoSpeed;    //右下给定速度
}RemoteParam_Struct;

typedef struct
{
	struct
	{
		uint16_t ch0;
		uint16_t ch1;
		uint16_t ch2;
		uint16_t ch3;
		uint8_t s1;
		uint8_t s2;
	}rc;
	struct
	{
		int16_t x;
		int16_t y;
		int16_t z;
		uint8_t press_l;
		uint8_t press_r;
	}mouse;
	struct
	{
		uint16_t v;
	}key;
}RC_Ctl_t;
/* ----------------------- Internal Data ----------------------------------- */

extern unsigned char rc_buffer[25];
extern RC_Ctl_t RC_Ctl;
extern RemoteParam_Struct remoteparam;

#define ABS(x) ((x)>0? (x):(-(x)))
#define FlagCheck(z) ((z)>0 ? 1 : 0)


void RemoteController(void);
void move_xy(void);
void RemoteDataInit(void);
void RemoteDataPrcess(uint8_t *);
int16_t Ramp_function(int16_t delta_Min,int16_t target,int16_t now, int16_t ramp_Coeff, u8 mode);

#endif
