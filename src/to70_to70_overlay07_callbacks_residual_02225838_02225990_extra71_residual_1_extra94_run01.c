#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct Flow {u8 state,pad1[3];void *owner;u8 pad8[0x1c];void *pic;u8 pad28[0x1a];s16 delta;int interval,direction,count,y,amount;} Flow;
extern void Pokepic_SetAttr(void*,int,int),ov07_02223224(void*),ov07_0221C448(void*,void*),Heap_Free(void*);
void ov07_02225904(void*,Flow*);
typedef struct Init {u32 state;void *owner;u8 pad8[0x10];void *data;u8 objectData[0xc];void *managed;u32 other;void *anim;} Init;
extern Init *ov07_022324D8(void*,int);
extern void ov07_02231FE4(void*,void*);
extern int ov07_0221BFD0(void*);
extern u32 ov07_0221C4A8(void *p, int i);
extern void ov07_022323FC(void*,int,void*,u32*);
extern void *Sprite_GetPaletteProxy(void*);
extern u32 ObjPlttTransfer_GetPaletteVramOffset(void*,int);
extern void *ov07_02222F10(void*,int,int,u16,int,s8,s8,u8,u8,u16,int);
extern void ov07_02225810(void*,void*),ov07_0221C410(void*,void(*)(void*,void*),void*);
void ov07_02225838(void*);
void ov07_02225838(void*owner){Init*w=ov07_022324D8(owner,52);ov07_02231FE4(owner,w);int a5,a4,a3,a2;int a0=((s32)ov07_0221C4A8(owner, 0));int a1=((s32)ov07_0221C4A8(owner, 1));a2=((s32)ov07_0221C4A8(owner, 2));a3=((s32)ov07_0221C4A8(owner, 3));a4=((s32)ov07_0221C4A8(owner, 4));a5=((s32)ov07_0221C4A8(owner, 5));u32 other;ov07_022323FC(owner,a0,w->objectData,&other);u32 pal=ObjPlttTransfer_GetPaletteVramOffset(Sprite_GetPaletteProxy(*(void**)w->managed),1);int heap=ov07_0221BFD0(owner);w->anim=ov07_02222F10(w->data,heap,2,(u16)(pal<<4),16,(s8)a1,(s8)a2,(u8)a3,(u8)a4,(u16)a5,1100);ov07_0221C410(w->owner,ov07_02225810,w);}
