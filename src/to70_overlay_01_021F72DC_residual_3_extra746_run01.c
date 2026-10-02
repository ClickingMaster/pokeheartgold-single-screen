#pragma force_active on
#include "global.h"
typedef struct{u8 a,b,c,d;void*actor;}Data;
extern Data*sub_0205F40C(void*);
extern int ov01_021FA2D4(void*),MapObject_GetFacingDirection(void*),sub_0205F330(void*);
extern int*MapObject_GetFacingVector(void*);
extern void ov01_021FA3E8(void*,void*),ov01_021F8C88(void*,void*),ov01_021F8708(void*,void*,int);
extern void(*ov01_02208AC0[])(void*,void*,Data*,int);
void ov01_021F772C(void*);
void ov01_021F772C(void*w){struct{volatile int value;}dir;Data*d=sub_0205F40C(w);void*actor=d->actor;int index;if(ov01_021FA2D4(w)==1||actor==0)return;dir.value=MapObject_GetFacingDirection(w);index=sub_0205F330(w);if(index>=9)GF_AssertFail();ov01_02208AC0[index](w,actor,d,dir.value);MapObject_GetFacingVector(w)[2]=2048;d->c=dir.value;d->d=sub_0205F330(w);ov01_021FA3E8(w,actor);ov01_021F8C88(w,actor);}
