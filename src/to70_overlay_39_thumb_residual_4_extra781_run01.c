#pragma force_active on
#include "global.h"
typedef struct {void *context;u8 p[0x140];void *arg;u8 q[0x48];u8 a[128];u8 c[412];u8 first,second;u8 r[0x3a];int value;u8 t[0x25];u8 timer;} W;
extern int ov39_02227DB8(W*);extern void ov39_022271A4(void*,void*,void*);extern void ov39_02227088(void*,void*,int,void*,void*);extern void ov39_02227188(void*,void*,int);
int ov39_02227420(W*,int,void*,void*,int);
int ov39_02227420(W*w,int mode,void*a,void*b,int flag) {if(ov39_02227DB8(w)==0)return 0;w->first=mode;w->second=flag;ov39_022271A4(w->context,a,w->a);ov39_02227088(w->context,b,flag,w->c,w->arg);ov39_02227188(w->context,w->c,mode);w->timer=60;w->value=0x5208;return 1;}
