#include "global.h"
#include "sprite.h"
#include "system.h"
#include <string.h>

typedef struct Ov90WindowBits {
    u8 value : 5;
    u8 enable : 1;
    u8 unused : 2;
} Ov90WindowBits;

typedef struct Ov90Slide {
    Sprite *sprites[2];
    s16 step;
    s16 displacement;
    u8 front[0xC0];
    u8 back[0xC0];
    SysTask *task;
    Ov90WindowBits savedControl;
    Ov90WindowBits savedBlend;
    u8 pad192[2];
    u32 savedMain;
} Ov90Slide;

extern const s32 ov90_0225C2B4[][3];
extern Sprite *ov90_02258EB4(void *,SpriteList *,int,int,u16,enum HeapID);
extern void ov90_0225BE08(void *),ov90_0225BD84(Ov90Slide *);
void ov90_0225BAD0(Ov90Slide *p,void *system,void *header,void *heapWord);
static inline void set_mask(u32 planes,BOOL effect){u32 value=(*(volatile u16*)0x4000048&~63u)|planes;if(effect)value|=0x20;*(volatile u16*)0x4000048=(u16)value;}
#pragma force_active on
#pragma opt_arithtransformation off
void ov90_0225BAD0(Ov90Slide *p,void *system,void *header,void *heapWord){s32 i;u32 bits;memset(p->front,0,192);memset(p->back,0,192);p->step=0;p->displacement=0;p->task=NULL;for(i=0;i<2;i++){p->sprites[i]=ov90_02258EB4(system,header,(s16)(ov90_0225C2B4[i][0]>>12),(s16)(ov90_0225C2B4[i][1]>>12),0,(enum HeapID)(u32)heapWord);Sprite_SetAnimCtrlSeq(p->sprites[i],5);Sprite_SetPriority(p->sprites[i],0);}bits=(*(volatile u32*)0x4000000&0xe000)>>13;*(volatile u32*)0x4000000=(*(volatile u32*)0x4000000&0xffff1fff)|((bits&~1)<<13);set_mask(31,TRUE);*(volatile u16*)0x4000040=0;*(volatile u16*)0x4000044=0;*(u8*)&p->savedControl=*(volatile u8*)0x400004a;p->savedMain=((*(volatile u32*)0x4000000&0xe000)>>13)&2;p->savedBlend.value=15;p->savedBlend.enable=1;Main_SetHBlankIntrCB(ov90_0225BE08,p);Sprite_SetFlipMode(p->sprites[1],2);ov90_0225BD84(p);}
