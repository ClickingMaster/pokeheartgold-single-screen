#include "global.h"
#include "sprite_system.h"
#include "pokepic.h"
#include "obj_pltt_transfer.h"
#include "vram_transfer_manager.h"
#include "sys_task.h"
#include "sys_task_api.h"
#pragma force_active on
typedef struct {void *pixels;u32 narc,member;} Asset;
typedef struct {u8 prefix[0x14];u16 bank;u8 gap[0x96];SpriteSystem *spriteSystem;Asset *assets[4];} Battle;
typedef struct {u32 heap;u8 prefix[0x14];u32 *cursor;u8 gap[0xa4];Battle *battle;void *paddingC4;PaletteData *palette;u8 gapCC[0x6c];SpriteManager *spriteManager;ManagedSprite *sprites[5];u32 active[5];void *follow[5];} Work;
typedef struct {ManagedSprite*sprite;u8 a,b,pad[2];Pokepic*picture;SysTask*task;} Follow;
static inline u32 Read(Work*d){u32*p=d->cursor;u32 value=*p;d->cursor=p+1;return value;}
int ov07_0221D3CC(void*,u32);Pokepic*ov07_0221FA48(void*,u32);void ov07_0221D4B0(SysTask*,void*);void ov07_0221D874(Work*);
void ov07_0221D874(Work*d){void*pixels;u32 battler,option,slot,id,index;u32 ids[6];ManagedSpriteTemplate t;Pokepic*picture;ManagedSprite*sprite;Asset*assets;u32 narc,member;s16 x,y;int i;Follow*follow;
 d->cursor=(u32*)d->cursor+1;battler=Read(d);option=Read(d);slot=Read(d);id=Read(d);ids[0]=id+20001+5000*d->battle->bank;ids[1]=id+20001+5000*d->battle->bank;ids[2]=id+20001+5000*d->battle->bank;ids[3]=id+20001+5000*d->battle->bank;ids[4]=0;ids[5]=0;index=ov07_0221D3CC(d,battler);assets=d->battle->assets[index];narc=assets->narc;member=assets->member;pixels=assets->pixels;picture=ov07_0221FA48(d,index);if(picture){x=Pokepic_GetAttr(picture,0);y=Pokepic_GetAttr(picture,1);y-=Pokepic_GetAttr(picture,41);}t.x=x;t.y=y;t.z=0;t.animation=0;t.drawPriority=100;t.pal=0;t.vram=NNS_G2D_VRAM_TYPE_2DMAIN;t.bgPriority=1;t.vramTransfer=0;for(i=0;i<6;i++)t.resIdList[i]=ids[i];sprite=SpriteSystem_NewSprite(d->battle->spriteSystem,d->spriteManager,&t);if(!picture)ManagedSprite_SetDrawFlag(sprite,0);else if(Pokepic_GetAttr(picture,6)==1)ManagedSprite_SetDrawFlag(sprite,0);
 if(ov07_0221FA48(d,index)){NNSG2dImageProxy*proxy=Sprite_GetImageProxy(sprite->sprite);GF_CreateNewVramTransferTask((NNS_GFD_DST_TYPE)19,proxy->vramLocation.baseAddrOfVram[1],pixels,3200);}if(ov07_0221FA48(d,index)){u32 offset=ObjPlttTransfer_GetPaletteVramOffset(Sprite_GetPaletteProxy(sprite->sprite),NNS_G2D_VRAM_TYPE_2DMAIN);PaletteData_LoadNarc(d->palette,(NarcId)narc,member,(enum HeapID)d->heap,(PaletteBufferId)2,32,(u16)(offset*16));}if(d->sprites[slot])GF_AssertFail();d->sprites[slot]=sprite;d->active[slot]=1;if(option==1&&ov07_0221FA48(d,index)){d->follow[slot]=Heap_Alloc((enum HeapID)d->heap,16);follow=d->follow[slot];follow->sprite=sprite;((Follow*)d->follow[slot])->picture=ov07_0221FA48(d,index);follow=d->follow[slot];follow->a=0;follow=d->follow[slot];follow->b=0;follow=d->follow[slot];{SysTask *task=SysTask_CreateOnMainQueue(ov07_0221D4B0,follow,0x1001);follow=d->follow[slot];follow->task=task;}}}
