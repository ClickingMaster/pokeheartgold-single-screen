#pragma force_active on
#include "global.h"
#include "sprite_system.h"
typedef struct {u8 pad[0x2f4];SpriteSystem *sys;SpriteManager *mgr;ManagedSprite *targets[61];ManagedSprite *pieces[8];u8 rest[0x84b8];u16 active;u8 gap[8];u16 flag;} Inner;
extern const ManagedSpriteTemplate ov14_021F83E4;
extern const s8 sPCBoxOutlineOffsetsX[],sPCBoxOutlineOffsetsY[];
extern void PCBox_SetSpriteDrawPriority(Inner *,u32,u32);
void PCBox_CreateHeldItemIconOutline(Inner *p);
#pragma force_active on
void PCBox_CreateHeldItemIconOutline(Inner *p){
 ManagedSpriteTemplate t;s16 x,y;u32 i,priority,draw;
 if(p->active==0)return;
 ManagedSprite_GetPositionXY(p->targets[11],&x,&y);
 if(p->pieces[0]==NULL){t=ov14_021F83E4;t.drawPriority=ManagedSprite_GetDrawPriority(p->targets[11])+1;t.bgPriority=ManagedSprite_GetPriority(p->targets[11]);t.resIdList[0]=0xc11f;p->flag=1;
 for(i=0;i<8;i++){t.x=x+sPCBoxOutlineOffsetsX[i];t.y=y+sPCBoxOutlineOffsetsY[i];p->pieces[i]=SpriteSystem_NewSprite(p->sys,p->mgr,&t);ManagedSprite_SetPaletteOverride(p->pieces[i],8);ManagedSprite_SetDrawFlag(p->pieces[i],0);}p->flag=0;
 }else{draw=ManagedSprite_GetDrawPriority(p->targets[11]);priority=ManagedSprite_GetPriority(p->targets[11]);for(i=0;i<8;i++){ManagedSprite_SetPositionXY(p->pieces[i],x+sPCBoxOutlineOffsetsX[i],y+sPCBoxOutlineOffsetsY[i]);PCBox_SetSpriteDrawPriority(p,i+61,draw+1);ManagedSprite_SetPriority(p->pieces[i],priority);ManagedSprite_SetDrawFlag(p->pieces[i],0);}}
}
