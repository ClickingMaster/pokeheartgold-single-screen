#pragma force_active on
#include "global.h"
#pragma force_active on
int sub_02023F30(void*);void MapObject_SetFacingVector(void*,VecFx32*);void ov01_021F8708(void*,void*,u32);
typedef struct {u8 facing,pad,mode,pad2;void *object;} State;
extern State *sub_0205F40C(void *);extern int ov01_021FA2D4(void *),MapObject_GetFacingDirection(void *),sub_0205F330(void *);extern void GF_AssertFail(void),ov01_021F8874(void *,void *,int),ov01_021FA3E8(void *,void *),ov01_021F8C88(void *,void *);
extern void (*ov01_022089CC[])(void *,void *,State *,int);
void ov01_021F878C(void *);
void ov01_021F8708(void*a,void*b,u32 dir){VecFx32 v={0,0,0};int n=sub_02023F30(b)/4096;switch(dir){case 0:break;case 1:if(n>=24)v.z=6*4096;break;case 2:if(n<40)v.x=4096;else v.x=-5*4096;break;case 3:if(n<56)v.x=-4096;else v.x=5*4096;break;}v.z+=2048;MapObject_SetFacingVector(a,&v);}
