#pragma force_active on
#include "global.h"
typedef struct{void *args;u8 p[0x1c-4];int state;u8 q[0xbdc-0x20];void *message;u8 r[0x136c-0xbe0];u8 menu[4];}Work;
extern int ov72_0223AF48(void*);extern void ov72_0223A350(Work*,void*,int,int,int),ov72_02238680(Work*,int,int);int ov72_02239F60(Work*);int ov72_02239F8C(Work*);
int ov72_02239F60(Work*w){int result=ov72_0223AF48(w->menu);if(result){if(result==2)w->state=29;else{w->state=41;*(int*)((u8*)w->args+32)=1;}}return 3;}
int ov72_02239F8C(Work*w){ov72_0223A350(w,w->message,26,1,0xf0f);ov72_02238680(w,47,42);return 3;}
