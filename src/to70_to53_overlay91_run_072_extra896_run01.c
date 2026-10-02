#pragma force_active on
#include "global.h"
#include "sprite.h"
typedef struct{u8 pad[0x1c];SpriteList*list;u8 pad2[0x74c];SpriteResourcesHeader resources;}Work;
typedef struct{int active;Sprite*s;u8 pad[20];}Entry;
void ov91_02260D14(Work*,Entry*,int,enum HeapID);
void ov91_02260D14(Work*w,Entry*entry,int palette,enum HeapID heap){SpriteTemplate t={0};int i;t.spriteList=w->list;t.header=&w->resources;t.scale.x=9;t.scale.y=9;t.scale.z=9;t.drawPriority=32;t.whichScreen=NNS_G2D_VRAM_TYPE_2DSUB;t.heapID=heap;for(i=0;i<8;i++){entry->s=Sprite_CreateAffine(&t);Sprite_SetDrawFlag(entry->s,0);Sprite_SetAffineOverwriteMode(entry->s,1);Sprite_SetPaletteOverride(entry->s,palette);entry->active=0;entry++;}}
