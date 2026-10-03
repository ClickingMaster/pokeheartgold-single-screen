#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct {u32 word;u16 attr;} Sprite;
typedef struct {Sprite *sprite;void *handle;u8 type;u8 pad[3];} Work;
extern Work *ov13_0224F4B8;extern u8 ov13_02244F44[][4];
extern void *ov13_0224128C(int,int),*ov13_02242528(int,void(*)(void),int,int);
extern Sprite *ov13_02240184(int,int);extern void ov13_02227FAC(void),ov13_022425F0(int,void*),ov13_02241D98(Sprite*),ov13_022412B0(Work**);
void ov13_02227EB0(int),ov13_02227F68(void);
void ov13_02227EB0(int type){int option;if(ov13_0224F4B8)return;{Work *w=ov13_0224128C(12,4);option=*(volatile u8*)&ov13_02244F44[type][0];ov13_0224F4B8=w;w->type=type;}ov13_0224F4B8->sprite=ov13_02240184(0,option);ov13_0224F4B8->sprite->word=(ov13_0224F4B8->sprite->word&0xfe00ff00)|0x26|0xe50000;ov13_0224F4B8->sprite->attr=(ov13_0224F4B8->sprite->attr&~0xc00)|0x800;ov13_0224F4B8->handle=ov13_02242528(0,ov13_02227FAC,0,120);}
