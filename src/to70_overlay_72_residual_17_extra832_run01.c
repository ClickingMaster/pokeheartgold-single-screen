#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 p[0x1c];int state;u8 p2[0x74];int a;u8 p3[4];int b;u8 p4[0xb30];void *fmt,*msg;} Work;
extern int ov72_0223A588(Work *);extern void BufferIntegerAsString(void *,u32,s32,u32,u32,u32);extern void ov72_0223A280(Work *,void *,int,int,int);extern void ov72_02238680(Work *,int,int);
int ov72_02239CB4(Work *),ov72_02239CD8(Work *);
int ov72_02239CB4(Work *w){int v=ov72_0223A588(w);if(v!=0){if(v>0){w->b=v;w->state=35;}else w->state=29;}return 3;}
int ov72_02239CD8(Work *w){BufferIntegerAsString(w->fmt,0,w->b,3,2,1);BufferIntegerAsString(w->fmt,1,w->a,2,2,1);ov72_0223A280(w,w->msg,1,1,0xf0f);ov72_02238680(w,47,36);return 3;}
