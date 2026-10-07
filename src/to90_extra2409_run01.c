#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct FlagView {u8 pad[0x64];u8 enabled[26];} FlagView;
typedef struct Work {void*owner,*windows,*p8,*sprite,*p10,*p14,*p18,*bg,*p20,*msg;u8 pad28[0x14];s16 selection,other;u32 w40,w44;int cursor,state;u8 pad50[0x14];u8 enabled[26];} Work;
extern void sub_02019688(void*,int,int,int,int),sub_02019B08(void*,int),ov70_0224190C(Work*,int),ov70_02242FC4(void*,void*,void*,int,u32),String_Delete(void*),ov70_02243EB8(void*,void*,void*,int),sub_020196E8(void*,int,int,int),sub_020197F4(void*,int),Sprite_SetAnimCtrlSeq(void*,int),ov70_02238F9C(void*,int,int),Sprite_SetDrawFlag(void*,int);extern void*NewString_ReadMsgData(void*,int);extern int ov70_02243F7C(Work*,int);extern u8 ov70_02245E26[],ov70_02245E27[];
int ov70_0224316C(Work*);
#pragma opt_strength_reduction off
int ov70_0224316C(Work*w){sub_02019688(w->bg,0,100,34,1);sub_02019B08(w->bg,0);ov70_0224190C(w,4);MI_CpuFill8(w->enabled,1,26);int i=0;int offset=i;for(;i<9;i++,offset+=16){void*s=NewString_ReadMsgData(w->msg,i+110);u32 color;int enabled;u8*dest;if(ov70_02243F7C(w,i)==1){color=0xf0e02;dest=((FlagView*)((u8*)w+i))->enabled;enabled=1;}else{color=0x80902;dest=((FlagView*)((u8*)w+i))->enabled;enabled=0;}*dest=enabled;ov70_02242FC4(w->bg,(u8*)w->windows+offset,s,2,color);String_Delete(s);}ov70_02243EB8(w->bg,w->msg,(u8*)w->windows+0xe0,68);sub_020196E8(w->bg,0,16,0);sub_020197F4(w->bg,0);Sprite_SetAnimCtrlSeq(w->sprite,61);ov70_02238F9C(w->sprite,(ov70_02245E26[w->selection*2]+16)*8,ov70_02245E27[w->selection*2]*8);w->cursor=w->selection;Sprite_SetDrawFlag(w->sprite,1);w->state=5;return -1;}
