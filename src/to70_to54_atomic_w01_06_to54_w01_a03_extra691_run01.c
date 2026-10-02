#pragma force_active on
#include "global.h"
typedef struct {u8 p[0xc];u32 flags;u8 q[4];u16 a,b;} Info;
typedef struct {u8 p[0x18];u32 *pc;u8 q[0xa4];Info *info;} W;
extern int ov07_0223192C(W*,int);typedef struct {u32 a[4];} Mask;extern Mask ov07_02234B98;
void ov07_0221D174(W*w);
void ov07_0221D1B8(W*w);
void ov07_0221D1B8(W*w) {Mask mask;u32 f,i;mask=ov07_02234B98;w->pc++;f=w->info->flags;if(f!=0){w->pc++;for(i=0;i<4;i++){if(mask.a[i]&f)break;w->pc++;}}w->pc+=*w->pc;}
