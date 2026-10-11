#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{s8 a,b,dir;volatile s8 motion;void *sprite;}State;
extern State *sub_0205F40C(void*);
extern int ov01_021FA2D4(void*),MapObject_GetFacingDirection(void*),sub_0205F330(void*),BillboardObject_GetSequence(void*);
extern void BillboardObject_SetSequence(void*,int),BillboardObject_SetSequenceFrame(void*,int),BillboardObject_AdvanceFrame(void*,int),ov01_021FA3E8(void*,void*),ov01_021F8C88(void*,void*);

void ov01_021F7CE4(void *obj);

void ov01_021F7CE4(void *obj){int dir;State*s=sub_0205F40C(obj);void *sprite=s->sprite;int anim;if(ov01_021FA2D4(obj)!=1&&sprite!=0){dir=MapObject_GetFacingDirection(obj);if(sub_0205F330(obj)==0){anim=(u32)dir>1?1:0;if(anim!=BillboardObject_GetSequence(sprite)||s->motion==0){BillboardObject_SetSequence(sprite,anim);BillboardObject_SetSequenceFrame(sprite,0);BillboardObject_AdvanceFrame(sprite,0);}BillboardObject_AdvanceFrame(sprite,0);}else{int other=(u32)dir>1?1:0;if(other!=BillboardObject_GetSequence(sprite)){BillboardObject_SetSequence(sprite,other);BillboardObject_SetSequenceFrame(sprite,0);}BillboardObject_AdvanceFrame(sprite,1<<12);}s->dir=dir;s->motion=sub_0205F330(obj);ov01_021FA3E8(obj,sprite);ov01_021F8C88(obj,sprite);}}
