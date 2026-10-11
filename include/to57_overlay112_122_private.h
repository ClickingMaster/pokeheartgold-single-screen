#ifndef TO57_NEW39_PRIVATE_H
#define TO57_NEW39_PRIVATE_H
#include "global.h"
extern u8 gSystem[];
typedef struct Ov112State {
    u8 pad0[0x18];
    void *bg;
    u8 pad1[0x1E440 - 0x1C];
    void *walker;
    u8 pad2[4];
    void *msg;
    u8 pad3[0x1E468 - 0x1E44C];
    void *field468;
    u8 pad4[0x1EA98 - 0x1E46C];
    u8 win1[0x10];
    u8 win2[0x1A8];
    u32 selection;
    u8 pad5[0x28];
    u32 field1EC7C;
    u8 pad6[0x1F2D0 - 0x1EC80];
    s32 counter;
    u8 pad7[0x14];
    u8 box[0x88];
} Ov112State;
void PokewalkerApp_LoadTilemap(void *, int, int);
void PokewalkerApp_PrintString(void *, int, void *, int, int);
void PokewalkerApp_PrintExit(void *, int);
void PokewalkerApp_SetCursorPosition(void *, int);
int PokewalkerApp_GetTouchedButton(int);
void PokewalkerApp_SelectMenuItem(void *, int, int, int);
void PlaySE(int);
void PokewalkerApp_HideCursor(void *);
void FillBgTilemapRect(void *, int, int, int, int, int, int, int);
void ClearWindowTilemapAndCopyToVram(void *);
void BgCommitTilemapBufferToVram(void *, int);
void *Pokewalker_TryGetBoxMon(void *, void *);
void ov112_021EA5A4(void *, int);
void BufferBoxMonNickname(void *, int, void *);
void PokewalkerApp_LoadMonPicture(void *, void *, void *, int);
void PokewalkerApp_StartMessage(void *, int, int, int);
void PokewalkerApp_HideSprite(void *, int);
u32 MTRandom(void);
void PokewalkerApp_SpawnFallingParticle(void *);
void PokewalkerApp_PlayTransferSE(int);
int ov112_021EE2D4(void *);
int ov112_021EE31C(void *);
int ov112_021EE3E0(void *);
int ov112_021EE464(void *);
int ov112_021EE4B8(void *);
int ov112_021EE4D0(void *);
#endif
