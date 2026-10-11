#pragma force_active on
#include "global.h"
#pragma force_active on
#define W(p,o) (*(u32*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*((u8*)(p)+(o)))

typedef struct W40144Icon{u8 model,unused,kind;}W40144Icon;
typedef struct W40144Work{s16 unused,count;u32 prefix[2];u8 windows[4][120];u8 gap[0x87c-0x1ec];W40144Icon*icons[4];u8 gap2[0x954-0x88c];s32 delay;volatile u32 fade[3];}W40144Work;
typedef struct W40144Model{void*model;u32 pad[3];}W40144Model;
typedef struct W40144FadeView{u8 pad[0x958];volatile u32 value;}W40144FadeView;
typedef struct W40144IconView{u8 pad[0x87c];W40144Icon*icon;}W40144IconView;
typedef struct W40144Context{u8 gap[0x10558];W40144Model models[256];}W40144Context;
void ModelInstance_SetVisible(void*,int),NNS_G3dMdlSetMdlAlphaAll(void*,int);int ov49_02265968(void*,int),ov49_02265B28(void*,void*,int,int),ov49_02265BE8(void*,void*,int,int,int),ov49_02265B94(void*,void*,int,int,int);int ov49_02266820(void*,W40144Work*);
int ov49_02266820(void*ctx,W40144Work*s){int done;if(s->icons[0]->kind==17)GF_AssertFail();if(s->delay>0){s->delay--;ModelInstance_SetVisible(s->windows[0],0);}else if(s->count<3&&ov49_02265968(s,s->count+1)==1){ModelInstance_SetVisible(s->windows[0],1);if(ov49_02265B28(ctx,s,0,0)==1){(s->count)++;s->delay=3;if(s->count<3)ov49_02265BE8(ctx,s,0,0,0);else ModelInstance_SetVisible(s->windows[0],0);}}int i=1;done=1;u8*window=s->windows[0];window+=120;W40144IconView *view=(void*)((u8*)s+4);for(;i<=3;i++,window+=120,view=(void*)((u8*)view+4)){if(ov49_02265968(s,i)==1){if(s->count>i-1){ModelInstance_SetVisible(window,1);if(ov49_02265B94(ctx,s,i,0,10240)==0)done=0;else{W40144FadeView*entry=(void*)((u8*)s+(i-1)*4);if(entry->value==0){ModelInstance_SetVisible(window,0);NNS_G3dMdlSetMdlAlphaAll(((W40144Context*)ctx)->models[view->icon->model].model,31);}else{(entry->value)--;NNS_G3dMdlSetMdlAlphaAll(((W40144Context*)ctx)->models[view->icon->model].model,(s32)(entry->value*20)/8);done=0;}}}else done=0;}}return done;}
