#ifndef TO54_W19_PRIVATE_H
#define TO54_W19_PRIVATE_H
#include "global.h"

#include "system.h"
#pragma require_prototypes off

typedef struct Ov112State {
    u8 raw[0x1f300];
} Ov112State;
typedef struct Ov112ListLayout {
    u8 pad[0x1ec58];
    s8 list[6][5];
} Ov112ListLayout;
#define W(p, o)  (*(s32 *)((p)->raw + (o)))
#define H(p, o)  (*(u16 *)((p)->raw + (o)))
#define S8(p, o) (*(s8 *)((p)->raw + (o)))
#define P(p, o)  (*(void **)((p)->raw + (o)))

extern const s32 ov112_021FF0C4[];
extern int ov112_021EC7B8(Ov112State *);
extern void PokewalkerApp_PrintString(Ov112State *, int, void *, int, u32);
extern u32 PokewalkerApp_GetTouchedButton(int);
extern void PokewalkerApp_PrintSubString(Ov112State *, int, void *, int, u32);
extern void PokewalkerApp_SetCursorPosition(Ov112State *, int);
extern void PokewalkerApp_HideCursor(Ov112State *);
extern void PokewalkerApp_ShowSprite(Ov112State *, int);
extern void PokewalkerApp_HideSprite(Ov112State *, int);
extern void PokewalkerApp_SetSpriteAnim(Ov112State *, int, int);
extern void *PokewalkerApp_StartMessageBox(Ov112State *, int, int);
extern void ov112_021EC440(Ov112State *, int, int, int);
extern void PokewalkerApp_LoadTilemap(Ov112State *, int, int);
extern void PokewalkerApp_PrintExit(Ov112State *, int);
extern void PokewalkerApp_RemoveWindowBank(Ov112State *, int);
extern void PokewalkerApp_AddWindowBank(void *, int);
extern void *NewString_ReadMsgData(void *, int);
extern void String_Delete(void *);
extern void BufferIntegerAsString(void *, int, int, int, int, int);
extern void *ReadMsgData_ExpandPlaceholders(void *, void *, int, int);
extern int Pokewalker_CourseIsUnlocked(void *, int);
extern void BufferPokewalkerCourseName(void *, int, u8);

int ov112_021ECA88(Ov112State *p);
void ov112_021EC8A4(Ov112State *p, int row, int count);
void ov112_021ECA18(Ov112State *p, int pos);
void ov112_021ECDA0(Ov112State *p, int dir);

void ov112_021EC7D0(Ov112State *p);
int ov112_021EC950(Ov112State *p);

#endif
