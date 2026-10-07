#pragma force_active on
#include "global.h"
#include "ov115_transition_helpers.h"
#include "heap.h"
#pragma force_active on
typedef struct{int x,start,linear,quadratic,frame,duration;}Motion;
typedef struct{int value,start,delta,frame,duration,screen;u8 reserved[16];}Brightness;
typedef struct{u8 bytes[0x34];}Resource;
typedef struct{void *list;u8 remainder4[0x13c-4];Resource first;}SpriteSystem;
typedef struct{Motion motion;Brightness brightness;void *effect;SpriteSystem system;Resource second;void *sprite;u8 animation[0x90];u8 window[16];int scrolling,scroll,timer;}Data;
typedef char DataSize[(sizeof(Data)==0x298)?1:-1];
typedef char DataLayout[(offsetof(Data,system)==0x44 && offsetof(Data,system.first)==0x180 && offsetof(Data,second)==0x1b4 && offsetof(Data,window)==0x27c && offsetof(Data,timer)==0x294)?1:-1];
typedef struct {void *pad[2],*bg,*save;} Parent;
typedef struct {u32 state;int flag;u32 pad;Data *data;Parent *parent;int *done;u32 pad2[2];void *narc;} Context;
typedef struct {int x;u32 msg;u16 type;u8 pad[2];u8 res[4],bg[3];} Args;
extern void GfGfx_EngineATogglePlanes(int,int);
extern void GfGfxLoader_GXLoadPalFromOpenNarc(void*,int,int,int,int,int),AddWindowParameterized(void*,void*,int,int,int,int,int,int,int),FillWindowPixelBuffer(void*,int),AddTextPrinterParameterizedWithColor(void*,int,void*,int,int,int,u32,void*),String_Delete(void*),ov01_021F05C4(void*,int,int),ov01_021F0614(void*,void*,void*,int,int,int,int,int,int),Sprite_SetDrawFlag(void*,int),Sprite_SetPriority(void*,int),Sprite_SetMatrix(void*,void*),ov115_02260254(void*,int,int,int,int),ov01_021EFCF8(int,int,int,void*,int),ov01_021F0454(void*,int,int,int,int,int,void*,int),ov01_021F0B78(void*,void*,int,int,int,int,int),SetBgPriority(int,int),ov01_021F0B5C(void*),ov01_021EFEC8(void*,int,int,int,int),ov01_021F074C(void*,int,int,int),ov01_021EFE34(void*,int,int,int),ov01_021F0DC8(void*),SetBlendBrightness(int,int,int),ScheduleSetBgPosText(void*,int,int,int),BeginNormalPaletteFade(int,int,int,int,int,int,int),sub_0200FBF4(int,int),Sprite_Delete(void*),ov115_0225F09C(void*),ov01_021F06EC(void*,void*),ov01_021F05F4(void*),RemoveWindow(void*),BgSetPosTextAndCommit(void*,int,int,int),SpriteList_RenderAndAnimateSprites(void*);
extern void *ov115_0225F1BC(void*,int),*ov01_021F0718(void*,void*,int,int,int,int),*ov01_021F0B44(void);
extern int ov01_021EFE30(void*),ov115_0225F0B4(void*),ov01_021EFF28(void*),ov01_021EFE44(void*),IsPaletteFadeFinished(void);
int ov115_0225F220(Context*,int,const Args*);
int ov115_0225F220(Context *c,int heap,const Args *args){u32 state=c->state;Data *d=c->data;void *str;int result;VecFx32 position;switch(state){case 0:c->data=Heap_Alloc((enum HeapID)heap,sizeof(Data));memset(c->data,0,sizeof(Data));d=c->data;GfGfxLoader_GXLoadPalFromOpenNarc(c->narc,16,0,64,32,heap);GfGfx_EngineATogglePlanes(4,0);AddWindowParameterized(c->parent->bg,d->window,2,0,10,16,2,2,1);FillWindowPixelBuffer(d->window,0);if(args->type==23)str=ov115_0225F1BC(c->parent->save,heap);else str=ov115_0225F158(args->msg,heap);AddTextPrinterParameterizedWithColor(d->window,0,str,0,0,0,0x10200,0);String_Delete(str);ov01_021F05C4(&d->system,8,3);ov01_021F0614(c->narc,&d->system,&d->system.first,args->res[0],1,args->res[1],args->res[2],args->res[3],600000);ov01_021F0614(c->narc,&d->system,&d->second,59,1,60,61,62,600001);d->sprite=ov01_021F0718(&d->system,&d->system.first,0x110000,0x42000,0,0);Sprite_SetDrawFlag(d->sprite,0);ov115_0225F020((struct Ov115Work*)d->animation,&d->system,&d->second,0x48000,0x4a000,heap);ov115_02260254(d->sprite,heap,args->res[0],14,0);d->effect=ov01_021F0B44();c->state++;break;
case 1:ov01_021EFCF8(1,16,16,&c->flag,1);c->state++;break;
case 2:if(c->flag)c->state=state+1;break;
case 3:ov01_021F0454(c->narc,args->bg[2],args->bg[1],args->bg[0],0,1,c->parent->bg,3);d->scrolling=1;ov01_021F0B78(c,d->effect,6,8,16,31,23);SetBgPriority(2,0);SetBgPriority(3,1);SetBgPriority(0,2);GfGfx_EngineATogglePlanes(8,1);GfGfx_EngineATogglePlanes(2,0);c->state++;break;
case 4:if(ov01_021EFE30(c)){c->state++;ov01_021F0B5C(d->effect);d->timer=10;}break;
case 5:if(--d->timer<0){GfGfx_EngineATogglePlanes(16,1);if(ov115_0225F0B4(d->animation)==1)c->state++;}break;
case 6:ov01_021EFEC8(&d->motion,0x110000,args->x,-0x40000,4);Sprite_SetDrawFlag(d->sprite,1);Sprite_SetPriority(d->sprite,1);ov01_021F074C(&position,d->motion.x,0x42000,0);Sprite_SetMatrix(d->sprite,&position);c->state++;break;
case 7:result=ov01_021EFF28(&d->motion);ov01_021F074C(&position,d->motion.x,0x42000,0);Sprite_SetMatrix(d->sprite,&position);if(result==1)c->state++;break;
case 8:ov01_021EFE34(&d->brightness,0,16,3);d->timer=10;c->state++;break;
case 9:if(--d->timer<0){result=ov01_021EFE44(&d->brightness);ov01_021F0DC8(&d->brightness);if(result==1){ov115_02260254(d->sprite,heap,args->res[0],0,0);SetBlendBrightness(-14,33,1);ScheduleSetBgPosText(c->parent->bg,2,0,-((d->motion.x>>12)-92));GfGfx_EngineATogglePlanes(4,1);c->state++;}}break;
case 10:ov01_021EFE34(&d->brightness,16,0,3);c->state++;break;
case 11:result=ov01_021EFE44(&d->brightness);ov01_021F0DC8(&d->brightness);if(result==1){c->state++;d->timer=26;}break;
case 12:if(--d->timer<0)c->state++;break;
case 13:BeginNormalPaletteFade(3,0,0,32767,15,1,4);c->state++;break;
case 14:if(IsPaletteFadeFinished())c->state++;break;
case 15:sub_0200FBF4(1,32767);if(c->done)*c->done=1;Sprite_Delete(d->sprite);ov115_0225F09C(d->animation);ov01_021F06EC(&d->system,&d->system.first);ov01_021F06EC(&d->system,&d->second);ov01_021F05F4(&d->system);RemoveWindow(d->window);*(volatile u32*)0x4000000 &=0xffff1fff;SetBlendBrightness(0,0,1);BgSetPosTextAndCommit(c->parent->bg,2,0,0);return 1;
}if(d->scrolling==1){ScheduleSetBgPosText(c->parent->bg,3,0,d->scroll);d->scroll=(d->scroll+30)%512;}if(c->state!=15)SpriteList_RenderAndAnimateSprites(d->system.list);return 0;}
