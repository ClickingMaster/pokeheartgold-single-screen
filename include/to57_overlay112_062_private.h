#ifndef TO57_R3_12B_PRIVATE_H
#define TO57_R3_12B_PRIVATE_H
#include "global.h"

#include "system.h"
typedef struct Ov112State {
    u8 raw[0x1f300];
} Ov112State;
#define W(p, o) (*(int *)((p)->raw + (o)))
#define P(p, o) (*(void **)((p)->raw + (o)))
void FillBgTilemapRect(void *, int, int, int, int, int, int, int);
void BgCommitTilemapBufferToVram(void *, int);
void ov112_021EC460(Ov112State *, int, int, int);
void PokewalkerApp_LoadTilemap(Ov112State *, int, int);
void ov112_021E9FA4(Ov112State *, int, void *, int, int);
void ov112_021E9F40(Ov112State *, int);
void ov112_021EA51C(Ov112State *, int);
int ov112_021E9888(int);
void ov112_021EC440(Ov112State *, int, int, int);
void PlaySE(int);
int ov112_021EB9C0(void);
int ov112_021EB9E4(void);
void ov112_021EB90C(Ov112State *, int);
int ov112_021EB950(Ov112State *, int);
void ov112_021EA570(Ov112State *);
int ov112_021EB208(Ov112State *);
int ov112_021EB244(Ov112State *);
int ov112_021EB28C(Ov112State *);
int ov112_021EB388(Ov112State *);
#endif
