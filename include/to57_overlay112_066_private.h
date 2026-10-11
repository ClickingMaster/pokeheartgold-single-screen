#include "global.h"

#include "system.h"

typedef struct Ov112State {
    u8 raw[0x1f300];
} Ov112State;

#define W(p, o) (*(int *)((p)->raw + (o)))
#define P(p, o) (*(void **)((p)->raw + (o)))

extern const int ov112_021FF09C[];

void PokewalkerApp_LoadTilemap(Ov112State *, int, int);
void PokewalkerApp_PrintString(Ov112State *, int, void *, int, int);
void PokewalkerApp_PrintExit(Ov112State *, int);
void PokewalkerApp_SetCursorPosition(Ov112State *, int);
int PokewalkerApp_GetTouchedButton(int);
void ov112_021EC440(Ov112State *, int, int, int);
void PlaySE(int);
int ov112_021EB9C0(void);
int ov112_021EB9E4(void);
void ov112_021EB90C(Ov112State *, int);
int ov112_021EB950(Ov112State *, int);
void PokewalkerApp_HideCursor(Ov112State *);
void GF_AssertFail(void);
void FillBgTilemapRect(void *, int, int, int, int, int, int, int);
void BgCommitTilemapBufferToVram(void *, int);
void ov112_021EC460(Ov112State *, int, int, int);
int ov112_021EB5DC(Ov112State *);
int ov112_021EB664(Ov112State *);
int ov112_021EB780(Ov112State *);
int ov112_021EB7D8(Ov112State *);
