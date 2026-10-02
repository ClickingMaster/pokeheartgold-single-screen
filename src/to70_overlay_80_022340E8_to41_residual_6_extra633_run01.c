#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u8 p[0x10];u8 mode;u8 p2[0x50-0x11];void *sprites[4];void *other[4];void *party,*otherparty;}Work;
typedef struct{s16 x,y;}XY;
extern XY ov80_0223BEB8[],ov80_0223BEC8[];
extern int BattleArcade_GetMonCount(int,int),BattleArcade_GetOpponentMonCount(int,int);
extern void *ov80_0222F3FC(void*,int,int),*Party_GetMonByIndex(void*,int);
extern u32 GetMonData(void*,int,void*);
extern void Sprite_SetDrawFlag(void*,int);

void ov80_02234BEC(Work*w,void *sys,int other);

void ov80_02234BEC(Work*w,void *sys,int other){int count=BattleArcade_GetMonCount(w->mode,1);int n=BattleArcade_GetOpponentMonCount(w->mode,1);int i;if(other==0){for(i=0;i<count;i++){w->sprites[i]=ov80_0222F3FC(sys,ov80_0223BEB8[i].x+8,ov80_0223BEB8[i].y+4);if(GetMonData(Party_GetMonByIndex(w->party,i),6,0)==0)Sprite_SetDrawFlag(*(void**)w->sprites[i],0);}}else{for(i=0;i<n;i++){w->other[i]=ov80_0222F3FC(sys,ov80_0223BEC8[i].x+8,ov80_0223BEC8[i].y+4);if(GetMonData(Party_GetMonByIndex(w->otherparty,i),6,0)==0)Sprite_SetDrawFlag(*(void**)w->other[i],0);}}}
