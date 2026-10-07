#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {void *battle;u8 pad[8];int heap;} Parent;
typedef struct {Parent *parent;u32 pad;void *palette;u8 pad2[0x300];void *manager;} Work;
extern void *NARC_New(int,int),*BattleSystem_GetSpriteSystem(void *);extern int GetItemIndexMapping(int,int),GetItemIconCell(void),GetItemIconAnim(void);
extern void SpriteSystem_LoadCharResObjFromOpenNarc(void *,void *,void *,int,int,int,int),SpriteSystem_LoadPaletteBufferFromOpenNarc(void *,int,void *,void *,void *,int,int,int,int,int),SpriteSystem_LoadCellResObjFromOpenNarc(void *,void *,void *,int,int,int),SpriteSystem_LoadAnimResObjFromOpenNarc(void *,void *,void *,int,int,int),NARC_Delete(void *);
void ov08_02223D80(Work *);
void ov08_02223D80(Work *w){
void *narc=NARC_New(18,w->parent->heap),*system=BattleSystem_GetSpriteSystem(w->parent->battle);u32 i;int id;
for(i=0;i<6;i++){id=0xb4b7+i;SpriteSystem_LoadCharResObjFromOpenNarc(system,w->manager,narc,GetItemIndexMapping(1,1),0,2,id);SpriteSystem_LoadPaletteBufferFromOpenNarc(w->palette,3,system,w->manager,narc,GetItemIndexMapping(1,2),0,1,2,id);}
SpriteSystem_LoadCellResObjFromOpenNarc(system,w->manager,narc,GetItemIconCell(),0,0xb4b7);SpriteSystem_LoadAnimResObjFromOpenNarc(system,w->manager,narc,GetItemIconAnim(),0,0xb4b7);NARC_Delete(narc);
}
