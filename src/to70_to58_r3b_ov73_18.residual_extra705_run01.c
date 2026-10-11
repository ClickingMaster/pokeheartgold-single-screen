#pragma force_active on
#include "global.h"
typedef struct {u32 data[9];} Resources;
typedef struct {void *list;Resources *header;s32 pos[3],scale[3];u16 rotation;u16 pad;u32 priority;int screen,heap;} Template;
typedef struct {u8 p[0xd24];void *resources[4];u8 p34[0x30];Resources header;u8 p88[0x48];void *sprites[2];} Work;
typedef struct{s16 x,y;} XY;
extern XY ov73_021EA684[];extern void CreateSpriteResourcesHeader(Resources *,int,int,int,int,int,int,int,int,void *,void *,void *,void *,int,int);extern void ov73_021E82A8(Template *,Work *,Resources *,int);extern void *Sprite_CreateAffine(Template *);extern void Sprite_SetAnimActiveFlag(void *,int);extern void Sprite_SetAnimCtrlSeq(void *,int);extern void Sprite_SetDrawFlag(void *,int);extern void GfGfx_EngineATogglePlanes(int,int);extern void GfGfx_EngineBTogglePlanes(int,int);extern void WirelessStatusIcon_Create(void);
void ov73_021E82D4(Work *);
void ov73_021E82D4(Work *w){Template t;int i;CreateSpriteResourcesHeader(&w->header,0,0,0,0,-1,-1,0,0,w->resources[0],w->resources[1],w->resources[2],w->resources[3],0,0);ov73_021E82A8(&t,w,&w->header,1);for(i=0;i<2;i++){t.pos[0]=(u16)ov73_021EA684[i].x<<12;t.pos[1]=(u16)ov73_021EA684[i].y<<12;w->sprites[i]=Sprite_CreateAffine(&t);Sprite_SetAnimActiveFlag(w->sprites[i],1);Sprite_SetAnimCtrlSeq(w->sprites[i],i);Sprite_SetDrawFlag(w->sprites[i],0);}GfGfx_EngineATogglePlanes(16,1);GfGfx_EngineBTogglePlanes(16,1);WirelessStatusIcon_Create();}
