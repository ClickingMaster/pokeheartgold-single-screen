#ifndef TO52_R6_CARD19_PRIVATE_H
#define TO52_R6_CARD19_PRIVATE_H
#include "global.h"
typedef struct Ov88Screen {
    u16 width;
    u16 height;
    u8 pad[8];
    u8 data[];
} Ov88Screen;
typedef struct Ov88ScreenOwner {
    u32 unk0;
    Ov88Screen *screen;
} Ov88ScreenOwner;
void CopyToBgTilemapRect(void *, u8, u8, u8, u8, u8, const void *, u8, u8, u8, u8);
void BgTilemapRectChangePalette(void *, u8, u8, u8, u8, u8, u8);
void ScheduleBgTilemapBufferTransfer(void *, u8);
u32 ov45_0222AD4C(void *);
void ov88_02258EFC(void *, void *, void *);
void ov88_02258F60(void *, void *, void *);
void ov88_02258D90(Ov88ScreenOwner *, void **, u32, u32, u8);
void ov88_02258DE8(Ov88ScreenOwner *, void **, u32, u32, u32, u32, u32, u32, u32);
void ov88_02258EA8(u8 *, void *, void *, void *, void *, void *);
#endif
