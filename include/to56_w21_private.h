#ifndef TO56_W21_PRIVATE_H
#define TO56_W21_PRIVATE_H

#define FillBgTilemapRect Tracked_FillBgTilemapRect
#include "global.h"

#include "filesystem.h"
#include "unk_0208805C.h"
#undef FillBgTilemapRect

typedef struct Ov08Entry {
    u8 pad00[0x16];
    u8 percent : 7;
    u8 percentHigh : 1;
    u8 pad17[4];
    u8 padFlags : 7;
    u8 disabled : 1;
    u32 hp;
    u32 baseHp;
    u32 maxHp;
    u8 pad28[0x28];
} Ov08Entry;

extern BOOL ov08_0221DAE4(void *work);
extern BOOL ov08_0221DB04(void *work);
extern void ov08_02220C5C(void *work, int state);
extern void ov08_0221DD28(void *work);
extern void ov08_0221DC3C(void *work, int state);
extern void ov08_0221DD70(void *work, int state);
extern void ov08_0222171C(void *work, int state);
extern void ov08_022221CC(void *work, int state);
extern void ov08_02222524(void *work, int state);
extern BOOL NNS_G2dGetUnpackedScreenData(void *file, NNSG2dScreenData **screenData);
extern void FillBgTilemapRect(BgConfig *, int, int, int, int, int, int, int);
extern const u8 _02224E2C[6];
extern const u32 ov08_02224F80[];

int ov08_0221D5DC(u8 *work, int index);
u8 ov08_0221D614(u8 *work, int index, int delta);
void ov08_0221D6CC(u8 *work, int state);
void ov08_0221D74C(u8 *work, int fill, u16 x, int y);
void ov08_0221D77C(u8 *work, int fill, u8 row);
void ov08_0221D81C(u8 *work);
void ov08_0221D840(u8 *work, int state);
void ov08_0221D8B0(u8 *work, int state);
u8 ov08_0221DAC4(u8 *work);

#endif
