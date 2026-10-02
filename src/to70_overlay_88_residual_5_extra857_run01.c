#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u32 data;u8 pending,changed;u8 pad[2];u32 count;} State;
extern int ov45_0222DCF4(void *,u32 *);extern void ov45_0222D594(void *,void *,void *);
int ov88_02258EFC(State *,void *,void *);
int ov88_02258EFC(State *s,void *arg,void *context){u32 first,second;struct {u8 a,b,c,d;} value;int changed=0;if(ov45_0222DCF4(arg,&first)==0)return changed;if(s->pending==1){s->pending=0;changed=1;s->changed=changed;}ov45_0222D594(context,&first,&second);ov45_0222D594(&second,s,&value);if(value.c>=s->count||value.b!=0||value.a!=0){changed=1;s->changed=changed;s->data=second;}return changed;}
