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
void ov112_021E9FA4(void *, int, void *, int, int);
void ov112_021E9F40(void *, int);
void ov112_021EA51C(void *, int);
int ov112_021E9888(int);
void ov112_021EC440(void *, int, int, int);
void PlaySE(int);
void ov112_021EA570(void *);
void FillBgTilemapRect(void *, int, int, int, int, int, int, int);
void ClearWindowTilemapAndCopyToVram(void *);
void BgCommitTilemapBufferToVram(void *, int);
void *Pokewalker_TryGetBoxMon(void *, void *);
void ov112_021EA5A4(void *, int);
void BufferBoxMonNickname(void *, int, void *);
void ov112_021E9290(void *, void *, void *, int);
void ov112_021EC460(void *, int, int, int);
void ov112_021EA688(void *, int);
u32 MTRandom(void);
void ov112_021EA7D0(void *);
void ov112_021ED314(int);
int ov112_021EE2D4(void *);
int ov112_021EE31C(void *);
int ov112_021EE3E0(void *);
int ov112_021EE464(void *);
int ov112_021EE4B8(void *);
int ov112_021EE4D0(void *);
#endif
