#ifndef TO57_R3_09_PRIVATE_H
#define TO57_R3_09_PRIVATE_H

#include "global.h"

typedef struct Ov07Slot {
    u8 pad00[0x18];
    int active;
    int kind;
} Ov07Slot;

typedef struct Ov07Context {
    void *narc;
    u8 pad04[0xC0 - 4];
    void *config;
    void *bgConfig;
    void *palette;
} Ov07Context;

void GF_AssertFail(void);
int ov07_0223192C(void *, u16);
int ov07_0221FB7C(void *, int);
int ov07_0221BFC0(void *);
void GfGfxLoader_LoadCharData(int, int, void *, int, int, int, int, int);
void GfGfxLoader_LoadScrnData(int, int, void *, int, int, int, int, int);
void PaletteData_LoadNarc(void *, int, int, int, int, int, int);
void BgClearTilemapBufferAndCommit(void *, int);
void ov07_0221DD14(Ov07Slot **, int);
void ov07_0221DD38(Ov07Slot **, Ov07Slot *, int);
Ov07Slot *ov07_0221DD8C(Ov07Slot **, int);
BOOL ov07_0221DDB0(Ov07Slot **, Ov07Context *, int);
void ov07_0221DE04(Ov07Slot **, Ov07Context *, int, void *);

#endif
