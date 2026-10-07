#pragma force_active on
#include "global.h"
#pragma force_active on
#define B(w,n) (*((u8*)(w)+(n)))
#define H(w,n) (*(u16*)((u8*)(w)+(n)))
#define P(w,n) (*(void**)((u8*)(w)+(n)))
extern void Sprite_SetDrawFlag(void*,int),Sprite_SetMatrix(void*,const VecFx32*),Sprite_SetAnimCtrlSeq(void*,int),PlaySE(u16),GfGfx_EngineATogglePlanes(u8,u8);extern int ov96_021E839C(void*);
int ov96_021E667C(void*);
int ov96_021E667C(void*w){switch(B(w,0x70d)){case 0:{VecFx32 pos;Sprite_SetDrawFlag(P(w,0x714),0);PlaySE(2201);pos.z=0;pos.x=524288;pos.y=(H(w,0x70e)+72)<<12;Sprite_SetMatrix(P(w,0x708),&pos);Sprite_SetAnimCtrlSeq(P(w,0x708),3);Sprite_SetDrawFlag(P(w,0x708),1);B(w,0x70d)=1;break;}case 1:B(w,0x70c)++;if(B(w,0x70c)>60){B(w,0x70c)=0;B(w,0x70d)=2;}break;case 2:if(P(w,0xd6c))((void(*)(void*))P(w,0xd6c))(w);if(ov96_021E839C(w)){PlaySE(2200);Sprite_SetAnimCtrlSeq(P(w,0x710),0);Sprite_SetDrawFlag(P(w,0x710),1);}GfGfx_EngineATogglePlanes((u8)(*(u32*)((u8*)w+0x718)),1);P(w,0xd6c)=0;B(w,0x70d)=3;break;case 3:B(w,0x70c)++;if(B(w,0x70c)>120){B(w,0x70c)=0;B(w,0x70d)=4;return 1;}break;case 4:return 1;}return 0;}
