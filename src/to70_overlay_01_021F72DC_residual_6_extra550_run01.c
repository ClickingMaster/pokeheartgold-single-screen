#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{s8 a,b,dir;volatile s8 motion;void *sprite;}State;
extern State *sub_0205F40C(void*);
extern int ov01_021FA2D4(void*),MapObject_GetFacingDirection(void*),sub_0205F330(void*),sub_02023EF4(void*);
extern void sub_02023EE0(void*,int),sub_02023F40(void*,int),sub_02023F04(void*,int),ov01_021FA3E8(void*,void*),ov01_021F8C88(void*,void*);

void ov01_021F7CE4(void *obj);

void ov01_021F7CE4(void *obj){int dir;State*s=sub_0205F40C(obj);void *sprite=s->sprite;int anim;if(ov01_021FA2D4(obj)!=1&&sprite!=0){dir=MapObject_GetFacingDirection(obj);if(sub_0205F330(obj)==0){anim=(u32)dir>1?1:0;if(anim!=sub_02023EF4(sprite)||s->motion==0){sub_02023EE0(sprite,anim);sub_02023F40(sprite,0);sub_02023F04(sprite,0);}sub_02023F04(sprite,0);}else{int other=(u32)dir>1?1:0;if(other!=sub_02023EF4(sprite)){sub_02023EE0(sprite,other);sub_02023F40(sprite,0);}sub_02023F04(sprite,1<<12);}s->dir=dir;s->motion=sub_0205F330(obj);ov01_021FA3E8(obj,sprite);ov01_021F8C88(obj,sprite);}}
