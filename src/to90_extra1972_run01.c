#pragma force_active on
#include "global.h"
#pragma force_active on
#define B(p,o) (*((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define SH(p,o) (*(s16*)((u8*)(p)+(o)))
#define U(p,o) (*(u32*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
extern const u8 ov07_022366EB[],ov07_022366EC[],ov07_022366ED[],ov07_022366EE[],ov07_022366EF[];
void ov07_02222590(void*,u32,u32,u32,u32,u32,u32);int ov07_0222260C(void*);void ov07_02222268(void*,s16,s16,s16,s16,u16);int ov07_022222B4(void*);void Pokepic_SetAttr(void*,int,int);int Pokepic_GetAttr(void*,int);void ov07_022226C4(void*,s16,s16,u32,int);void BattleAnimSystem_DestroyTask(void*,void*);void Heap_Free(void*);
void ov07_022281C0(void*task,u8*w);
void ov07_022281C0(void*task,u8*w){
 switch(B(w,0)){
 case 0:{int n=B(w,1)*5;ov07_02222590(w+0x10,ov07_022366EB[n],ov07_022366EC[n],ov07_022366ED[n],ov07_022366EE[n],100,ov07_022366EF[n]);B(w,1)++;B(w,0)++;break;}
 case 1:if(!ov07_0222260C(w+0x10))B(w,0)++;Pokepic_SetAttr(P(w,0xc),12,SH(w,0x10));Pokepic_SetAttr(P(w,0xc),13,SH(w,0x12));ov07_022226C4(P(w,0xc),SH(w,2),SH(w,6),U(w,0x24),0);break;
 case 2:ov07_02222268(w+0x10,0,0,(s16)Pokepic_GetAttr(P(w,0xc),1),0,5);B(w,0)++;break;
 case 3:if(!ov07_022222B4(w+0x10))B(w,0)++;Pokepic_SetAttr(P(w,0xc),1,SH(w,0x12));break;
 case 4:{int n=B(w,1)*5;ov07_02222590(w+0x10,ov07_022366EB[n],ov07_022366EC[n],ov07_022366ED[n],ov07_022366EE[n],100,ov07_022366EF[n]);SH(w,2)=Pokepic_GetAttr(P(w,0xc),1);B(w,0)++;break;}
 case 5:if(!ov07_0222260C(w+0x10))B(w,0)++;Pokepic_SetAttr(P(w,0xc),12,SH(w,0x10));Pokepic_SetAttr(P(w,0xc),13,SH(w,0x12));ov07_022226C4(P(w,0xc),SH(w,2),SH(w,6),U(w,0x24),1);break;
 default:Pokepic_SetAttr(P(w,0xc),1,SH(w,4));Pokepic_SetAttr(P(w,0xc),6,1);Pokepic_SetAttr(P(w,0xc),12,256);Pokepic_SetAttr(P(w,0xc),13,256);BattleAnimSystem_DestroyTask(P(w,8),task);Heap_Free(w);break;
 }
}
