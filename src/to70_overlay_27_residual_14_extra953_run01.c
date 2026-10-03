#pragma force_active on
#include "global.h"
typedef struct{u8 p[0x17];u8 active:1;u8 rest:7;} Contest;
typedef struct{u8 p[0x10];void *field;u8 p2[0x394];void *sprite;u8 p3[0xf4];u8 flag;u8 p4[0x79];u8 flag2;} Work;
extern Contest *FieldSystem_BugContest_Get(void *);
extern u16 *BugContest_GetSportBallsAddr(Contest *);
extern void ov27_0225BF84(Work *,int,int,int),ov27_0225BFCC(Work *,Contest *),Sprite_SetDrawFlag(void *,int);
void ov27_0225C088(Work *);
void ov27_0225C088(Work *w){Contest *c=FieldSystem_BugContest_Get(w->field);if(!c)GF_AssertFail();ov27_0225BF84(w,14,14,*BugContest_GetSportBallsAddr(c));if(c->active)ov27_0225BFCC(w,c);Sprite_SetDrawFlag(w->sprite,c->active);w->flag2=0;w->flag=0;}
