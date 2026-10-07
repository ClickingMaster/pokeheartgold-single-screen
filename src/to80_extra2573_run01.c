#pragma force_active on
#include "global.h"
#include "ov115_transition_helpers.h"
#include "heap.h"
#include "bg_window.h"
#include "text.h"
#include "sprite.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "pm_string.h"
#include "field_system.h"
#include "unk_0200FA24.h"
#include "unk_02026E30.h"
#include <string.h>
#pragma force_active on

typedef struct Triple {s32 x,y,z;} Triple;
typedef struct QuadraticInterp {s32 value,start,linear,quadratic,frame,duration;} QuadraticInterp;
typedef struct Interp {s32 value,start,delta,frame,duration,screen;} Interp;
typedef union {Triple native;VecFx32 vector;} Position;
typedef struct {void *list;u8 renderer[0x128];void *resourceManagers[4];} Manager;
typedef struct {void *objects[4];u32 header[9];} Resource;
typedef union {
    Interp fade;
    struct {s32 value,start,delta,frame,duration;Manager manager;} live;
} FadeManager;
typedef struct {u8 raw[20];} TickAnim;
typedef struct {s16 timer,count;void *sprites[4];TickAnim anims[4];} TickWork;
typedef union {Ov115Work creation;TickWork tick;} Effect;
typedef struct {
    QuadraticInterp x,y,slide;
    FadeManager fadeManager;
    Resource resources[4];Sprite *sprites[4];
    VecFx32 first,second;
    Effect effect;Window window;
    int mode,id;u8 unused[8];
} Data;
typedef struct {s32 state,wait,timer;Data *data;FieldSystem *field;int *finished;u8 unused[8];NARC *narc;} State;
typedef struct {u16 species;u8 sprite,frames;u16 unused,message;} Params;
typedef char DataSize[(sizeof(Data)==0x314)?1:-1];
typedef char DataOffsets[(offsetof(Data,resources)==0x198 && offsetof(Data,sprites)==0x268 && offsetof(Data,effect)==0x290 && offsetof(Data,window)==0x2f4)?1:-1];
typedef char ManagerOffset[(offsetof(Data,fadeManager.live.manager)==0x5c)?1:-1];
void ov01_021F05C4(void *,void *,int);
void ov01_021F0614(void *,void *,void *,int,int,int,int,int,int);
void ov01_021F074C(Triple *,s32,s32,s32);
void ov01_021EFEC8(QuadraticInterp *,s32,s32,s32,s32);
BOOL ov01_021EFF28(QuadraticInterp *);
void *ov01_021F0718(void *,void *,void *,void *,void *,void *);
void ov01_021EFE34(Interp *,s32,s32,s32);
BOOL ov01_021EFE44(Interp *);
void ov01_021F0DDC(void *);
void ov01_021EFCF8(void *,void *,void *,void *,void *);
void ov01_021F0E74(void);
u8 ov01_021F0E90(void);
void ov01_021F0F08(void *,int);
void ov01_021F0FB8(u32);
void ov01_021F1060(void);
struct SysTask *ov01_021F0DC8(void *);
BOOL ov01_021F1044(void);
BOOL ov01_021F1008(void);
void ov01_021F0EAC(void);
void ov01_021F0EC0(void);
void ov01_021F06EC(void *,void *);
void ov01_021F05F4(void *);
void ov115_02260254(void *,int,void *,u8,u16);
void ov115_0225F09C(void *);
int ov115_0225F0B4(TickWork *);
int ov115_0225F978(void *,int,const void *);
static inline void CreateSprites(Manager*manager,Resource resources[4],Sprite*sprites[4]){
    Resource*cursor=resources;int i;
    for(i=0;i<4;i++,cursor++){
        Resource*selected;int animation;
        if(i<3){selected=cursor;animation=0;}else{selected=&resources[i-1];animation=1;}
        sprites[i]=ov01_021F0718(manager,selected,0,0,0,0);
        Sprite_SetDrawFlag(sprites[i],0);
        Sprite_SetAnimCtrlSeq(sprites[i],animation);
        Sprite_SetPriority(sprites[i],1);
    }
}
int ov115_0225F978(void*state,int heap,const void*record){State*s=state;const Params*params=record;Position pos;Position initial;Data*d=s->data;Resource*p;void*manager;int i,ready,a;String*str;
switch(s->state){case 0:s->data=Heap_Alloc((enum HeapID)heap,0x314);memset(s->data,0,0x314);d=s->data;ov01_021F05C4(&d->fadeManager.live.manager,(void*)10,4);if(!ov115_0225F968(s->field)){ov01_021F0614(s->narc,&d->fadeManager.live.manager,&d->resources[0],207,1,208,209,210,600000);d->mode=0;d->id=207;}else{ov01_021F0614(s->narc,&d->fadeManager.live.manager,&d->resources[0],211,1,212,213,214,600000);d->mode=1;d->id=211;}ov01_021F0614(s->narc,&d->fadeManager.live.manager,&d->resources[1],params->species,1,params->species+1,params->species+2,params->species+3,600001);ov01_021F0614(s->narc,&d->fadeManager.live.manager,&d->resources[2],params->sprite,12,48,49,50,600002);ov01_021F0614(s->narc,&d->fadeManager.live.manager,&d->resources[3],59,1,60,61,62,600003);s->state++;break;
case 1:ov01_021F074C(&initial.native,8192,8192,0);CreateSprites(&d->fadeManager.live.manager,d->resources,d->sprites);ov115_02260254(d->sprites[0],heap,(void*)d->id,14,0);ov115_02260254(d->sprites[1],heap,(void*)(u32)params->species,14,0);GfGfx_EngineATogglePlanes(16,1);ov115_0225F020(&d->effect.creation,&d->fadeManager.live.manager,&d->resources[3],0x80000,0x60000,heap);ov01_021F0DDC(s->field);s->state++;break;
case 2:ov01_021EFCF8((void*)1,(void*)16,(void*)16,&s->wait,(void*)1);s->timer=0;s->state++;break;
case 3:if(++s->timer==8)ov01_021F0E74();if(s->wait)s->state++;break;
case 4:if(ov01_021F0E90()==1){ov01_021F0F08(s->narc,151);G2x_SetBlendAlpha_(0x4000050,1,30,0,8);s->state++;}break;
case 5:ov01_021EFEC8(&d->x,-0x80000,0x38000,0x50000,6);ov01_021F074C(&pos.native,d->x.value,0x5c000,0);Sprite_SetMatrix(d->sprites[0],&pos.vector);pos.vector.y+=0x4000;pos.vector.x+=0x10000;Sprite_SetMatrix(d->sprites[2],&pos.vector);Sprite_SetDrawFlag(d->sprites[0],1);Sprite_SetDrawFlag(d->sprites[2],1);ov01_021EFEC8(&d->slide,0x180000,0xc8000,-0x50000,6);ov01_021F074C(&pos.native,d->slide.value,0x5c000,0);Sprite_SetMatrix(d->sprites[1],&pos.vector);pos.vector.y+=0x4000;pos.vector.x-=0x10000;Sprite_SetMatrix(d->sprites[3],&pos.vector);Sprite_SetDrawFlag(d->sprites[1],1);Sprite_SetDrawFlag(d->sprites[3],1);GfGfxLoader_GXLoadPalFromOpenNarc(s->narc,16,GF_PAL_LOCATION_MAIN_BG,GF_PAL_SLOT_2_OFFSET,32,(enum HeapID)heap);GfGfx_EngineATogglePlanes(4,0);AddWindowParameterized(s->field->bgConfig,&d->window,2,21,13,11,2,2,1);FillWindowPixelBuffer(&d->window,0);str=ov115_0225F158(params->message,heap);AddTextPrinterParameterizedWithColor(&d->window,(FontID)0,str,0,0,0,0x10200,0);String_Delete(str);s->wait=3;s->state++;break;
case 6:if(s->wait>0){if(--s->wait==0){ov01_021F0FB8(3);GfGfx_EngineATogglePlanes(1,1);GfGfx_EngineATogglePlanes(4,1);}}else ov115_0225F0B4(&d->effect.tick);ov01_021EFF28(&d->x);ov01_021F074C(&pos.native,d->x.value,0x5c000,0);d->first=pos.vector;Sprite_SetMatrix(d->sprites[0],&pos.vector);pos.vector.y+=0x4000;pos.vector.x+=0x10000;Sprite_SetMatrix(d->sprites[2],&pos.vector);ready=ov01_021EFF28(&d->slide);ov01_021F074C(&pos.native,d->slide.value,0x5c000,0);d->second=pos.vector;Sprite_SetMatrix(d->sprites[1],&pos.vector);pos.vector.y+=0x4000;pos.vector.x-=0x10000;Sprite_SetMatrix(d->sprites[3],&pos.vector);if(ready==1)s->state++;break;
case 7:ready=ov115_0225F0B4(&d->effect.tick);a=ov01_021F1044();if(ready&&a){ov01_021EFE34(&d->fadeManager.fade,0,16,3);ov01_021F1060();s->state++;}break;
case 8:ready=ov01_021EFE44(&d->fadeManager.fade);ov01_021F0DC8(&d->fadeManager.fade);if(ready==1){ov115_02260254(d->sprites[0],heap,(void*)d->id,0,0);ov115_02260254(d->sprites[1],heap,(void*)(u32)params->species,0,0);Sprite_SetAnimActiveFlag(d->sprites[2],1);Sprite_SetAnimSpeed(d->sprites[2],8192);Sprite_SetAnimActiveFlag(d->sprites[3],1);Sprite_SetAnimSpeed(d->sprites[3],8192);ov01_021F0F08(s->narc,152);s->state++;}break;
case 9:ov01_021EFE34(&d->fadeManager.fade,16,0,6);ov01_021F0FB8(4);SetBgPriority(0,1);s->state++;break;
case 10:ready=ov01_021EFE44(&d->fadeManager.fade);ov01_021F0DC8(&d->fadeManager.fade);if(ready==1){s->state++;s->wait=8;}break;
case 11:if(s->wait>0)s->wait--;else{ov01_021EFEC8(&d->x,0,-8192,0,params->frames);ov01_021EFEC8(&d->y,0,-8192,0,params->frames);s->wait=0;s->state++;}break;
case 12:s->wait++;ready=ov01_021EFF28(&d->x);ov01_021EFF28(&d->y);if((s->wait/2)%2==0)ov01_021F074C(&pos.native,d->first.x+d->x.value,d->first.y+d->y.value,0);else ov01_021F074C(&pos.native,d->first.x-d->x.value,d->first.y-d->y.value,0);if(ready)d->first=pos.vector;Sprite_SetMatrix(d->sprites[0],&pos.vector);pos.vector.y+=0x4000;pos.vector.x+=0x10000;Sprite_SetMatrix(d->sprites[2],&pos.vector);if((s->wait/2)%2==0)ov01_021F074C(&pos.native,d->second.x-d->x.value,d->second.y-d->y.value,0);else ov01_021F074C(&pos.native,d->second.x+d->x.value,d->second.y+d->y.value,0);if(ready)d->second=pos.vector;Sprite_SetMatrix(d->sprites[1],&pos.vector);pos.vector.y+=0x4000;pos.vector.x-=0x10000;Sprite_SetMatrix(d->sprites[3],&pos.vector);if(ready){s->state++;GfGfx_EngineATogglePlanes(4,0);ov01_021EFEC8(&d->x,0,0xc0000,0x18000,16);ov01_021EFEC8(&d->y,0,0xc0000,0x18000,16);BeginNormalPaletteFade(3,0,0,32767,8,1,HEAP_ID_FIELD1);}break;
case 13:ov01_021EFF28(&d->x);ov01_021EFF28(&d->y);ov01_021F074C(&pos.native,d->first.x-d->x.value,d->first.y-d->y.value,0);Sprite_SetMatrix(d->sprites[0],&pos.vector);pos.vector.y+=0x4000;pos.vector.x+=0x10000;Sprite_SetMatrix(d->sprites[2],&pos.vector);ov01_021F074C(&pos.native,d->second.x+d->x.value,d->second.y+d->y.value,0);Sprite_SetMatrix(d->sprites[1],&pos.vector);pos.vector.y+=0x4000;pos.vector.x-=0x10000;Sprite_SetMatrix(d->sprites[3],&pos.vector);if(IsPaletteFadeFinished())s->state++;break;
case 14:sub_0200FBF4(1,32767);if(s->finished)*s->finished=1;ov115_0225F09C(&d->effect.creation);RemoveWindow(&d->window);for(i=0;i<4;i++)Sprite_Delete(d->sprites[i]);for(i=0,p=d->resources,manager=&d->fadeManager.live.manager;i<4;i++,p++)ov01_021F06EC(manager,p);ov01_021F05F4(&d->fadeManager.live.manager);ov01_021F1060();ov01_021F0EC0();*(volatile u16*)0x4000050=0;return 1;}
if(s->state!=14){SpriteList_RenderAndAnimateSprites(d->fadeManager.live.manager.list);if(s->state>4){Thunk_G3X_Reset();ov01_021F1008();ov01_021F0EAC();RequestSwap3DBuffers(GX_SORTMODE_AUTO,GX_BUFFERMODE_Z);}}return 0;}
