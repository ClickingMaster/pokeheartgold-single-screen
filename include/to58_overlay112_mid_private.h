#include "global.h"
#pragma require_prototypes off

typedef struct Ov112State {
    u8 pad[8];
    int state;
    u8 rest[0x1e524 - 12];
    u32 textPrinter;
} Ov112State;

extern int (*sMainMenuStates[])(Ov112State *);
extern int TextPrinterCheckActive(u8);











#include "system.h"
#define W(p,o) (*(u32 *)((u8 *)(p)+(o)))
extern void PokewalkerApp_RemoveWindowBank();
extern void PokewalkerApp_AddWindowBank(void *, int);
extern void PokewalkerApp_HideCursor(Ov112State *);
extern int ov112_021E9338(Ov112State *);
extern int PokewalkerApp_HandleTwoChoiceMenu(Ov112State *);







extern void FillBgTilemapRect(void *, int, int, int, int, int, int, int);
extern void BgCommitTilemapBufferToVram(void *, int);
extern void PokewalkerApp_StartMessage(Ov112State *, int, int, int);
extern void PokewalkerApp_HideSprite(Ov112State *, int);
extern int PokewalkerApp_StartMessageBox(Ov112State *, int, int);
extern void BgClearTilemapBufferAndCommit(void *, int);
extern void PokewalkerApp_LoadTilemap(Ov112State *, int, int);
extern void PokewalkerApp_PrintExit(Ov112State *, int);
extern void PokewalkerApp_StartSubBgScroll(Ov112State *);






extern void ov112_021EA64C(Ov112State *);
extern void ov112_021EA5A4(Ov112State *, int);
#define H(p,o) (*(u16 *)((u8 *)(p)+(o)))






extern void BufferBoxMonNickname(void *, int, void *);
extern void PokewalkerApp_LoadMonPicture(void *, void *, void *, int);
extern void PokewalkerApp_ShowSprite(Ov112State *, int);
extern u32 MTRandom(void);
extern void PokewalkerApp_SpawnFallingParticle(Ov112State *);
extern void sub_02032674(void *, void *, void *);
extern void sub_02032650(void *, u32, u16);






extern u8 ov112_021FECE4[], ov112_021FECE5[], ov112_021FECE6[], ov112_021FECE7[];
extern void ManagedSprite_SetPositionXYWithSubscreenOffset(void *, int, int, int);
extern void ManagedSprite_SetAnim(void *, int);
extern void ManagedSprite_SetPaletteOverride(void *, int);
extern void ManagedSprite_SetDrawFlag(void *, int);
