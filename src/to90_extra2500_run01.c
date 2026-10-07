#pragma force_active on
#include "global.h"
#include "sprite_system.h"
typedef struct{u8 prefix[0x50];SpriteManager*manager;SpriteSystem*system;u8 gap[0x1c];ManagedSprite*sprites[3];}Work;void ov95_021E6000(Work*);
#pragma force_active on
void ov95_021E6000(Work*w){ManagedSpriteTemplate t;u32 zero=0;int bg=2;t.x=128;t.y=120;t.z=zero;t.animation=zero;t.drawPriority=zero;t.pal=zero;t.vramTransfer=zero;t.vram=NNS_G2D_VRAM_TYPE_2DMAIN;t.bgPriority=bg;t.resIdList[0]=20000;t.resIdList[1]=20001;t.resIdList[2]=20002;t.resIdList[3]=20003;t.resIdList[4]=bg-3;t.resIdList[5]=bg-3;w->sprites[0]=SpriteSystem_NewSprite(w->system,w->manager,&t);ManagedSprite_TickFrame(w->sprites[0]);ManagedSprite_SetAffineOverwriteMode(w->sprites[0],2);t.x=0;t.y=-16;t.bgPriority=0;t.resIdList[0]=25000;t.resIdList[1]=25001;t.resIdList[2]=25002;t.resIdList[3]=25003;w->sprites[1]=SpriteSystem_NewSprite(w->system,w->manager,&t);ManagedSprite_TickFrame(w->sprites[1]);t.y=144;w->sprites[2]=SpriteSystem_NewSprite(w->system,w->manager,&t);ManagedSprite_TickFrame(w->sprites[2]);}
