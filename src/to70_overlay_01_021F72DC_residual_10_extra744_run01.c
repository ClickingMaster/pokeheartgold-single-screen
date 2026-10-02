#pragma force_active on
#include "global.h"
typedef struct{u8 a,b,c,d;void*actor;}Data;
extern Data*sub_0205F40C(void*);
extern int ov01_021FA2D4(void*),MapObject_GetFacingDirection(void*),sub_0205F330(void*);
extern int*MapObject_GetFacingVector(void*);
extern void ov01_021FA3E8(void*,void*),ov01_021F8C88(void*,void*),ov01_021F8708(void*,void*,int);
extern void(*ov01_022089E8[])(void*,void*,Data*,int);
void ov01_021F84F4(void*);
void ov01_021F84F4(void*w){struct{volatile int value;}dir;Data*d=sub_0205F40C(w);void*actor=d->actor;int index;if(actor==0||ov01_021FA2D4(w)==1)return;dir.value=MapObject_GetFacingDirection(w);index=sub_0205F330(w);if(index>=4)GF_AssertFail();ov01_022089E8[index](w,actor,d,dir.value);ov01_021F8708(w,actor,dir.value);d->a=dir.value;d->c=sub_0205F330(w);ov01_021FA3E8(w,actor);ov01_021F8C88(w,actor);}
