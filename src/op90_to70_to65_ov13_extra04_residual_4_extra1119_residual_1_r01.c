#pragma force_active on
#include "global.h"
#include <nitro/code32.h>

typedef struct { char s[23]; } Name23;
typedef struct { char s[21]; } Name21;
typedef struct { char s[22]; } Name22;
typedef struct {
    u8 pad0[8];
    void *res8;
    void *resC;
    void *res10;
} ResWork;
extern struct { u32 pad[2]; ResWork *work; } ov13_0224F54C;
extern const Name23 ov13_02245573;
extern const Name21 ov13_02245548;
extern const Name22 ov13_0224555D;
extern u8 ov13_02246474[], ov13_02246488[], ov13_022464A0[], ov13_022464B8[];
void WifiSetup_LoadGraphics(const void *name, void (*load)(const void *, u32, u32));
void *WifiSetup_LocalizePath(const void *name);
void *WifiSetupFile_Load(void *data, int a, int b);
void ov13_02239228(void *res);
void ov13_022392B4(void);
void ov13_02233000(void);

#define REG16(a) (*(vu16 *)(a))
#define SET_PRI3(a) REG16(a) = (u16)((REG16(a) & ~3) | 3)
// @fn ov13_02233000
void ov13_02233000(void) {
    Name23 scrName;
    Name21 plttName;
    Name22 charName;
    scrName = ov13_02245573;
    plttName = ov13_02245548;
    charName = ov13_0224555D;
    REG16(0x0400000C) = (REG16(0x0400000C) & 0x43) | 0xe18;
    WifiSetup_LoadGraphics(ov13_02246474, GX_LoadOBJPltt);
    WifiSetup_LoadGraphics(ov13_02246488, GX_LoadBG3Char);
    WifiSetup_LoadGraphics(ov13_022464A0, GX_LoadBG2Char);
    WifiSetup_LoadGraphics(&plttName, GX_LoadBGPltt);
    WifiSetup_LoadGraphics(ov13_022464B8, GX_LoadBG3Scr);
    {
    void *f; void *r;
    f = WifiSetup_LocalizePath(&scrName); r = WifiSetupFile_Load(f, 0, 4); ov13_0224F54C.work->res8 = r;
    ov13_02239228(ov13_0224F54C.work->res8);
    ov13_022392B4();
    f = WifiSetup_LocalizePath(&plttName); r = WifiSetupFile_Load(f, 0, 4); ov13_0224F54C.work->resC = r;
    f = WifiSetup_LocalizePath(&charName); r = WifiSetupFile_Load(f, 0, 4); ov13_0224F54C.work->res10 = r;
    }
    SET_PRI3(0x04001008);
    SET_PRI3(0x0400100A);
    SET_PRI3(0x04000008);
    REG16(0x0400000A) = (u16)((REG16(0x0400000A) & ~3) | 2);
    SET_PRI3(0x0400000C);
    REG16(0x0400000E) = (u16)((REG16(0x0400000E) & ~3) | 2);

}
