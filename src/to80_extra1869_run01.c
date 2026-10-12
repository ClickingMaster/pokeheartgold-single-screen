#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 p[0xc4];u32 width,height,pitch;} Work;
extern int CommState_SetError(int);extern void GF_AssertFail(void);extern int ov01_021F6328(int,int);extern int ov01_021F635C(u32,int,Work *);
int ov01_021F654C(Work *,int,int,u8 *);
#pragma opt_lifetimes off
int ov01_021F654C(Work *w,int x,int y,u8 *out){u32 index=(y/32)*w->width+x/32;u32 v;int pitch,pixel;if(index>=w->width*w->height&&CommState_SetError(1)!=0)return 0;if(index>=w->width*w->height)GF_AssertFail();pitch=w->pitch;pixel=y;pixel=pixel*pitch;v=ov01_021F635C(index,ov01_021F6328(x+pixel,pitch),w);if(out!=0)*out=v;if(v>3)return 0;return 1;}
