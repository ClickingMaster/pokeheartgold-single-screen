#pragma force_active on
#include "global.h"
extern void*sub_0205F40C(void*);extern int ov01_021FA2D4(void*),MapObject_GetFacingDirection(void*),sub_0205F330(void*);extern void ov01_021F8874(void*,void*,int),ov01_021FA3E8(void*,void*),ov01_021F8C88(void*,void*);typedef void(*Callback)(void*,void*,void*,int);extern struct{Callback unused;Callback funcs[2];}ov01_022089CC;
void ov01_021F878C(void*w);
void ov01_021F878C(void*w){int dir;u8*p=sub_0205F40C(w);void*s=*(void**)(p+4);int i;Callback f;if(s&&ov01_021FA2D4(w)!=1){dir=MapObject_GetFacingDirection(w);i=sub_0205F330(w);if(i>=2)GF_AssertFail();f=ov01_022089CC.funcs[i];f(w,s,p,dir);ov01_021F8874(w,s,dir);p[0]=dir;p[2]=sub_0205F330(w);ov01_021FA3E8(w,s);ov01_021F8C88(w,s);}}
