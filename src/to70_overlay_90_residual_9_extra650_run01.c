#pragma force_active on
#include "global.h"
extern s32 FX_Div(s32,s32);
typedef struct {s32 current,start,speed,accel,duration;} Motion;
void ov90_02258CF0(Motion *,s32,s32,s32,s32);
int ov90_02258D4C(Motion *,s32);
void ov90_02258CF0(Motion *m,s32 start,s32 end,s32 speed,s32 duration){s32 d=end-start-(s32)(((s64)speed*(duration<<12)+2048)>>12);s32 a=FX_Div((s32)(((s64)d*8192+2048)>>12),(duration*duration)<<12);m->current=start;m->start=start;m->speed=speed;m->accel=a;m->duration=duration;}
int ov90_02258D4C(Motion *m,s32 time){int done;s32 linear,quad;if(time>=m->duration){time=m->duration;done=1;}else done=0;linear=(s32)(((s64)m->speed*(time<<12)+2048)>>12);quad=(s32)(((s64)m->accel*((time*time)<<12)+2048)>>12);quad=FX_Div(quad,8192);m->current=m->start+(linear+quad);return done;}
