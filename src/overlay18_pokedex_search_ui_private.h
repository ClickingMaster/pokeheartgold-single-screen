#ifndef POKEHEARTGOLD_OVERLAY18_RESIDUAL_25_H
#define POKEHEARTGOLD_OVERLAY18_RESIDUAL_25_H

#include "global.h"

#include "sprite_system.h"
#include "system.h"

typedef struct Ov18Bits18C7 {
    u8 unk_0 : 5;
    u8 unk_5 : 1;
    u8 unk_6 : 1;
    u8 unk_7 : 1;
} Ov18Bits18C7;

void PlaySE(u16 seqNo);
int PlayerProfile_GetTrainerGender(void *profile);
u8 GetMonPicHeightBySpeciesGenderForm(u16 species, u8 gender, u8 whichFacing, u8 form, u32 pid);
void *GridInputHandler_Create(const void *hitBoxes, const void *dpadBoxes, const void *callbacks, void *callbackParam, BOOL inputState, u8 initialPos, u32 heapId);
const void *GridInputHandler_GetDpadBox(void *handler, int target);
void DpadMenuBox_GetPosition(const void *dpadBox, u8 *px, u8 *py);
int GridInputHandler_HandleInput_AllowHold(void *handler);
void MenuInputStateMgr_SetState(void *mgr, int state);
void Bg_SetTextDimAndAffineParams(void *bgConfig, u8 bgId, int op, int value, void *mtx, int centerX, int centerY);

extern const u8 ov18_021FA304[];
extern const u8 ov18_021FA310[];
extern const u8 sSearchMenuCursorAnims[];
extern const u8 sSearchMenuGridCallbacks[];
extern const u8 sSearchMenuHitboxes[];
extern const u8 sSearchMenuDpadBoxes[];

u32 PokedexApp_GetScrollbarLength(void *p, int idx, const u8 *table);
int PokedexApp_GetScrollbarMinY(void *p, int idx, const u8 *table, int x, u32 width);
int PokedexApp_GetScrollbarMaxY(void *p, int idx, const u8 *table, int x, u32 width);
void PokedexApp_SetSpriteAnim(void *p, int idx, int anim);
void PokedexApp_SetSpriteVisible(void *p, int idx, int draw);
void PokedexApp_SetSpritePos(void *p, int idx, int x, int y, int a4);
void PokedexApp_LoadMonPicToSprite(void *p, u16 species, u8 form, u8 gender, int facing, int a5, int a6);
void PokedexApp_GetSeenFormAndGender(void *p, u32 index, u8 *form, u8 *gender);
void PokedexApp_ShowFormMonPics(void *p, s8 a1, int a2);
void PokedexApp_SetFormListIcon(void *p, int idx, int value);
void PokedexApp_DrawFormListIcons(void *p);
void PokedexApp_UpdateScrollArrows(void *p, s8 a1, s8 a2, int a3);

u32 PokedexApp_GetFormScrollbarY(void *p, int idx);
u32 PokedexApp_GetAreaScrollbarY(void *p, int idx);
void PokedexApp_SyncFormScrollbar(void *p);
void PokedexApp_SyncAreaScrollbar(void *p);
void ov18_021F6844(void *p, int idx, int value);
void PokedexApp_ShowFormCompareMonPic(void *p, int a1, int a2, int a3);
void PokedexApp_UpdateFormCompareSelector(void *p);
void PokedexApp_UpdateSearchMenuCursor(void *p, int a1);

#endif
