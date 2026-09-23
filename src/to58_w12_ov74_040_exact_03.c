#include "global.h"

extern u8 _0223B73C[];
extern u8 ov74_0223D454[];
extern u8 ov74_0223E2D4[];
extern void GF_RunVramTransferTasks(void);
extern void OamManager_ApplyAndResetBuffers(void);
extern void DoScheduledBgGpuUpdates(void *);
extern void GfGfxLoader_GXLoadPal(int, int, int, int, int, void *);
extern void ov74_02235728(int, int, int, int, int, int);
extern void ov74_0223589C(int, int);
extern void *ov74_02235930(int, void *, int, int, int);
extern void *Heap_Alloc(int, int);
extern void Heap_Free(void *);

extern u16 MATH_CalcCRC16(void *, void *, int);
extern void OS_GetMacAddress(u8 *);
extern void *sub_02035754(int);
extern void CRYPTO_RC4Init(void *, void *, int);
extern void CRYPTO_RC4Encrypt(void *, void *, int, void *);

extern u32 GetItemIndexMapping(u16, int);
extern u32 GetItemIconCell(u32);
extern u32 GetItemIconAnim(u32);
void ov74_02235CE4(u8 *p, int choice, u8 *q);
void ov74_02235CE4(u8 *p, int choice, u8 *q) {
    u32 item;
    u32 charId;
    u32 plttId;
    u32 cell;
    u32 anim;
    switch (choice) {
    case 3: item = *(u32 *)(q + 4); break;
    case 8: item = 0x1c6; break;
    case 9: item = 0x1c4; break;
    case 10: item = 0x1c7; break;
    case 12: item = 0x1d3; break;
    case 14: item = 0x1ba; break;
    case 15: item = 0x1f5; break;
    }
    charId = GetItemIndexMapping(item, 1);
    plttId = GetItemIndexMapping(item, 2);
    cell = GetItemIconCell(plttId);
    anim = GetItemIconAnim(cell);
    ov74_02235728(0x12, charId, plttId, cell, anim, 1);
    ov74_0223589C(0, 0x100000);
    *(void **)(p + 0x208) = ov74_02235930(1, *(void **)(p + 0x208), 0x80, 0, 0);
}

void ov74_02235DA4(u8 *p);
void ov74_02235DA4(u8 *p) {
    GfGfxLoader_GXLoadPal(0x71, 0x21, 4, 0x100, 0xc0, *(void **)(p + 8));
}
