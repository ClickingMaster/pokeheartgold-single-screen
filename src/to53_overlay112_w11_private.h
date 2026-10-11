#ifndef TO53_R3_W11_PRIVATE_H
#define TO53_R3_W11_PRIVATE_H

#include "global.h"

void WaitingIcon_Delete(void *);
void ManagedSprite_SetDrawFlag(void *, int);
void ManagedSprite_SetAnim(void *, int);
int ManagedSprite_GetActiveAnim(void *);
void ManagedSprite_SetDrawPriority(void *, int);
void ManagedSprite_SetAnimateFlag(void *, int);
void ManagedSprite_SetPositionXYWithSubscreenOffset(void *, int, int, int);
void ManagedSprite_SetPositionXY(void *, int, int);
int ov112_021E7668(void *);
int IsPaletteFadeFinished(void);
void Sound_SetSceneAndPlayBGM(int, int, int);
void PlayBGM(int);
int TextPrinterCheckActive(u8);
void PokewalkerApp_StartMessage(void *, int, int, int);
void GF_AssertFail(void);
void *WaitingIcon_New(void *, int);
void ManagedSprite_SetPaletteOverride(void *, int);
void ManagedSprite_SetPositionXYWithSubscreenOffset(void *, int, int, int);
extern u8 ov112_021FECE4[];
extern u8 ov112_021FECE5[];
extern u8 ov112_021FECE6[];
extern u8 ov112_021FECE7[];
void PokewalkerApp_CreateWaitingIcon(void *);
void PokewalkerApp_DeleteWaitingIcon(void *);
void PokewalkerApp_ResetCursor(void *);
void PokewalkerApp_SetCursorPosition(void *, int);
void PokewalkerApp_HideCursor(void *);
void PokewalkerApp_PressCursor(void *);
void ov112_021EA5A4(void *, int);
void ov112_021EA60C(void *);
void ov112_021EA64C(void *);
void PokewalkerApp_ShowSprite(void *, int);
void PokewalkerApp_HideSprite(void *, int);
void PokewalkerApp_SetSpriteAnim(void *, int, int);
void PokewalkerApp_SetSpritePosition(void *, int, int, int);
void PokewalkerApp_ClearParticles(void *);
int PokewalkerApp_FindFreeParticle(void *);
void ov112_021EA9BC(void *, const s16 *);
void ov112_021EAA10(void *);
void ov112_021EAA34(void *, int, int);
int PokewalkerApp_MainMenu_Enter(void);
int PokewalkerApp_MainMenu_Exit(void);
int ov112_021EAF48(void);
int PokewalkerApp_MainMenu_PrintWelcome(void *);
int PokewalkerApp_MainMenu_WaitWelcome(void *);
int PokewalkerApp_MainMenu_PrintReceive(void *);
int PokewalkerApp_MainMenu_WaitReceive(void *);
int PokewalkerApp_MainMenu_PrintStrollReady2(void *);
int PokewalkerApp_MainMenu_WaitStrollReady2(void *);

#endif
