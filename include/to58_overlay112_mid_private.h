#include "global.h"
#pragma require_prototypes off

typedef struct Ov112State {
    u8 pad[8];
    int state;
    u8 rest[0x1e524 - 12];
    u32 textPrinter;
} Ov112State;

extern int (*ov112_021FF6C4[])(Ov112State *);
extern int TextPrinterCheckActive(u8);











#include "system.h"
#define W(p,o) (*(u32 *)((u8 *)(p)+(o)))
extern void ov112_021E9C10();
extern void ov112_021E9A78(Ov112State *, int);
extern void ov112_021EA570(Ov112State *);
extern int ov112_021E9338(Ov112State *);
extern int ov112_021E9750(Ov112State *);







extern void FillBgTilemapRect(void *, int, int, int, int, int, int, int);
extern void BgCommitTilemapBufferToVram(void *, int);
extern void ov112_021EC460(Ov112State *, int, int, int);
extern void ov112_021EA688(Ov112State *, int);
extern int ov112_021EA08C(Ov112State *, int, int);
extern void BgClearTilemapBufferAndCommit(void *, int);
extern void ov112_021E7CA4(Ov112State *, int, int);
extern void ov112_021E9F40(Ov112State *, int);
extern void ov112_021ED2F4(Ov112State *);






extern void ov112_021EA64C(Ov112State *);
extern void ov112_021EA5A4(Ov112State *, int);
#define H(p,o) (*(u16 *)((u8 *)(p)+(o)))






extern void BufferBoxMonNickname(void *, int, void *);
extern void ov112_021E9290(void *, void *, void *, int);
extern void ov112_021EA670(Ov112State *, int);
extern u32 MTRandom(void);
extern void ov112_021EA7D0(Ov112State *);
extern void sub_02032674(void *, void *, void *);
extern void sub_02032650(void *, u32, u16);






extern u8 ov112_021FECE4[], ov112_021FECE5[], ov112_021FECE6[], ov112_021FECE7[];
extern void ManagedSprite_SetPositionXYWithSubscreenOffset(void *, int, int, int);
extern void ManagedSprite_SetAnim(void *, int);
extern void ManagedSprite_SetPaletteOverride(void *, int);
extern void ManagedSprite_SetDrawFlag(void *, int);
