#include "global.h"
#include "system.h"
#pragma require_prototypes off

typedef struct Ov112State {
    u8 raw[0x1f300];
} Ov112State;

extern int TextPrinterCheckActive(u8);
extern int ManagedSprite_IsAnimated(void *);
extern void BeginNormalPaletteFade(int, int, int, int, int, int, int);
extern int IsPaletteFadeFinished(void);
extern int SaveGameNormal(void *);
extern void Save_ClearStatusFlags(void *);
extern int _s32_div_f(int, int);
extern int PokewalkerApp_StartMessageBox(Ov112State *, int, int);
extern void PokewalkerApp_CreateWaitingIcon(Ov112State *);
extern void PokewalkerApp_DeleteWaitingIcon(Ov112State *);
extern void PokewalkerApp_SetCursorPosition(Ov112State *, int);
extern void PokewalkerApp_HideCursor(Ov112State *);
extern void PokewalkerApp_PressCursor(Ov112State *);
extern void ov112_021EA64C(Ov112State *);
extern void PokewalkerApp_HideSprite(Ov112State *, int);
extern void PokewalkerApp_UpdateParticles(Ov112State *);
extern void PokewalkerApp_StartLink(Ov112State *);
extern void ov112_021E7484(int);
extern void PokewalkerApp_LoadTilemap(Ov112State *, int, int);
extern void PokewalkerApp_ShowYesNoMenu(Ov112State *, int);
extern void PokewalkerApp_ShowSaveEraseStepsMenu(Ov112State *, int);
extern void PokewalkerApp_AddWindowBank(void *, int);
extern void PokewalkerApp_RemoveWindowBank(Ov112State *, int);
extern void PokewalkerApp_PrintExit(Ov112State *, int);
extern void ov112_021ED2F4(Ov112State *);
extern void BgClearTilemapBufferAndCommit(void *, int);
void ov112_021EC460(Ov112State *, int, int, int);
extern int (*ov112_021FF730[])(Ov112State *);
extern int (*ov112_021FF740[])(Ov112State *);
extern int (*ov112_021FF7CC[])(Ov112State *);

#define W(p, o) (*(int *)((p)->raw + (o)))
#define H(p, o) (*(u16 *)((p)->raw + (o)))


int ov112_021EBAB0(void) { return 4; }
