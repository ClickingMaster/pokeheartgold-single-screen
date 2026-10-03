#pragma force_active on
#include "global.h"
typedef struct {u8 p[0x4c];void *bg;u8 q[0x38c];void *msg;u8 r[0x94];void *obj;} W;
extern void *sub_0201956C(void*,int,int,int);extern void sub_020195F4(void*,int,int,int,int);extern void sub_020196B8(void*,int,void*,int,int);
void ov81_0224271C(W*);
void ov81_0224271C(W*w) {w->obj=sub_0201956C(w->bg,2,1,100);sub_020195F4(w->obj,0,4,18,4);sub_020196B8(w->obj,0,w->msg,144,1);}
