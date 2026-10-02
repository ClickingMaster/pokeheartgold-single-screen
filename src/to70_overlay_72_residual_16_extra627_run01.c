#pragma force_active on
#include "global.h"
typedef struct {u8 p[0x1c];int state;u8 q[0x74];int result,number;u8 r[0xb34];void *fmt,*msg;} W;
extern int ov72_0223A588(W *);extern void ov72_02238680(W *,int,int);
extern void BufferIntegerAsString(void*,int,int,int,int,int);
extern void ov72_0223A280(W*,void*,int,int,int);
int ov72_02239BEC(W *w);
int ov72_02239C14(W *w);
int ov72_02239BEC(W *w) {int v=ov72_0223A588(w);if(v!=0){if(v>0){w->result=v;ov72_02238680(w,11,32);}else w->state=23;}return 3;}
int ov72_02239C14(W *w) {if(w->number>=100){BufferIntegerAsString(w->fmt,0,w->number,3,2,1);ov72_0223A280(w,w->msg,10,1,0xf0f);}else {BufferIntegerAsString(w->fmt,0,w->number,2,2,1);ov72_0223A280(w,w->msg,6,1,0xf0f);}ov72_02238680(w,47,33);return 3;}
