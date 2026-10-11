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


int ov112_021EC3B0(Ov112State *p) { W(p, 0x1e524) = PokewalkerApp_StartMessageBox(p, 2, 13); return 30; }

int ov112_021EC3C8(Ov112State *p) {
    if (!TextPrinterCheckActive((u8)W(p, 0x1e524))) { PokewalkerApp_CreateWaitingIcon(p); return 31; }
    return 30;
}

int ov112_021EC3F0(Ov112State *p) { SaveGameNormal((void *)W(p, 0x20)); Save_ClearStatusFlags((void *)W(p, 0x20)); return 32; }

int ov112_021EC404(Ov112State *p) { PokewalkerApp_DeleteWaitingIcon(p); W(p, 4) = 0; return 34; }

int ov112_021EC414(Ov112State *p) {
    if (!ManagedSprite_IsAnimated((void *)W(p, 0x1e530))) { W(p, 8) = W(p, 12); return W(p, 8); }
    return 33;
}

int ov112_021EC434(Ov112State *p) { PokewalkerApp_HideCursor(p); return 35; }

void ov112_021EC440(Ov112State *p, int a, int b, int c) { W(p, 0x1ec50) = a; PokewalkerApp_SetCursorPosition(p, b); PokewalkerApp_PressCursor(p); W(p, 12) = c; }

void ov112_021EC460(Ov112State *p, int a, int b, int c) { W(p, 0x1e524) = PokewalkerApp_StartMessageBox(p, a, b); W(p, 12) = c; }
