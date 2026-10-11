#pragma force_active on
#include "global.h"
typedef struct {u8 pad[0x34];void *state;} Work;
extern void PCBox_ClearMonInfoAndSelection(Work *),ov14_021F0C0C(Work *),PCBox_SlideOutActionMenu(void *),ov14_021E8328(void *),PCBox_HideSelectedIconOutline(Work *),ov14_021E88F8(void *),PCBox_WaitForUpdateFunc(Work *,void *,int),PCBox_SetSpriteVisible(void *,int,int),PCBox_InitSpriteSystem(void *),PCBox_LoadSpriteResources(void *),PCBox_CreateSprites(Work *),PCBox_CreateTrayTextObjs(void *);
extern u8 ov14_021EA674[];
extern void GfGfx_EngineATogglePlanes(int,int),GfGfx_EngineBTogglePlanes(int,int);

void ov14_021F2914(Work *w);
void ov14_021F2930(Work *w);
void ov14_021F297C(Work *w);

void ov14_021F2914(Work *w){PCBox_ClearMonInfoAndSelection(w);PCBox_SetSpriteVisible(w->state,11,0);ov14_021F0C0C(w);}
void ov14_021F2930(Work *w){PCBox_SlideOutActionMenu(*(void **)((u8*)w->state+0x2f0));ov14_021E8328(*(void **)((u8*)w->state+0x2f0));PCBox_HideSelectedIconOutline(w);if(*(u16*)((u8*)w->state+0x88c8))ov14_021E88F8(*(void **)((u8*)w->state+0x2f0));PCBox_WaitForUpdateFunc(w,ov14_021EA674,140);}
void ov14_021F297C(Work *w){GfGfx_EngineATogglePlanes(16,1);GfGfx_EngineBTogglePlanes(16,1);PCBox_InitSpriteSystem(w->state);PCBox_LoadSpriteResources(w->state);PCBox_CreateSprites(w);PCBox_CreateTrayTextObjs(w->state);}
