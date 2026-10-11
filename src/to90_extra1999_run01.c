#pragma force_active on
#include "global.h"
#include "sprite_system.h"
typedef struct {u8 pad[0x2f4];SpriteSystem *sys;SpriteManager *mgr;ManagedSprite *targets[61];ManagedSprite *pieces[8];u8 rest[0x84b8];u16 active;u8 gap[8];u16 flag;} Inner;
extern const ManagedSpriteTemplate ov14_021F810C;
extern const s8 sPCBoxOutlineOffsetsX[],sPCBoxOutlineOffsetsY[];
extern void PCBox_SetSpriteDrawPriority(Inner *,u32,u32);

typedef struct {u8 pad[0x21];u8 selection;u8 rest[0x12];Inner *inner;} Context;
void PCBox_CreateSelectedIconOutline(Context *p);
#pragma force_active on
void PCBox_CreateSelectedIconOutline(Context *p){
 ManagedSpriteTemplate t;s16 x,y;Inner *inner;u32 selected;u16 i;u32 draw,priority;NNSG2dImageProxy *proxy;
 if(p->selection==255)return;
 inner=p->inner;selected=*((u8*)inner+0x4094+p->selection);ManagedSprite_GetPositionXY(inner->targets[selected],&x,&y);
 if(inner->pieces[0]==NULL){t=ov14_021F810C;t.drawPriority=ManagedSprite_GetDrawPriority(inner->targets[selected])+1;t.bgPriority=ManagedSprite_GetPriority(inner->targets[selected]);t.resIdList[0]=0xc0e0+selected;inner->flag=1;
 for(i=0;i<8;i++){t.x=x+sPCBoxOutlineOffsetsX[i];t.y=y+sPCBoxOutlineOffsetsY[i];inner->pieces[i]=SpriteSystem_NewSprite(inner->sys,inner->mgr,&t);ManagedSprite_SetPaletteOverride(inner->pieces[i],8);}inner->flag=0;
 }else{proxy=Sprite_GetImageProxy(inner->targets[selected]->sprite);draw=ManagedSprite_GetDrawPriority(inner->targets[selected]);priority=ManagedSprite_GetPriority(inner->targets[selected]);for(i=0;i<8;i++){Sprite_SetImageProxy(inner->pieces[i]->sprite,proxy);ManagedSprite_SetPositionXY(inner->pieces[i],x+sPCBoxOutlineOffsetsX[i],y+sPCBoxOutlineOffsetsY[i]);PCBox_SetSpriteDrawPriority(p->inner,i+61,draw+1);ManagedSprite_SetPriority(p->inner->pieces[i],priority);ManagedSprite_SetDrawFlag(p->inner->pieces[i],1);}}
}
