#pragma force_active on
#include "global.h"
typedef struct{u8 p[0x388];void*sprite;}Work;
typedef struct{u8 p[24];}Row;extern Row ov27_0225D3C4[],ov27_0225D3C5[],ov27_0225D3C6[];extern void Sprite_SetAnimCtrlSeq(void*,u32),Sprite_SetMatrix(void*,VecFx32*);void ov27_0225CD18(Work*,int,int);
void ov27_0225CD18(Work*w,int row,int col){VecFx32 pos;row-=2;col*=3;pos.x=ov27_0225D3C4[row].p[col]<<12;pos.y=(ov27_0225D3C5[row].p[col]+256)<<12;pos.z=0;Sprite_SetAnimCtrlSeq(w->sprite,ov27_0225D3C6[row].p[col]);Sprite_SetMatrix(w->sprite,&pos);}
