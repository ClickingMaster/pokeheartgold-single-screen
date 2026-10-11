#ifndef TO50_OV99_R9_R5_PRIVATE_H
#define TO50_OV99_R9_R5_PRIVATE_H
#include "global.h"
typedef struct Ov99EventMainWork {
    u8 pad00[0xc];
    u32 heapId;
    u8 pad10[4];
    void *spriteContext;
    u8 pad18[0x98];
    u32 screen;
    u32 toggleRequested;
    u32 refreshRequested;
} Ov99EventMainWork;
void *OverlayManager_GetData(void *manager);
void BeginNormalPaletteFade(int,int,int,int,int,int,int);
BOOL IsPaletteFadeFinished(void);
void ov99_021E8828(Ov99EventMainWork *work);
void ov99_021E8788(Ov99EventMainWork *work);
void ov99_021E8C50(Ov99EventMainWork *work);
BOOL PokeathlonUtil_CheckExitInput(void);
BOOL ov99_021E90F4(Ov99EventMainWork *work);
void PlaySE(u32 seq);
void GF_AssertFail(void);
void PokeathlonUtil_DrawSprites(void *ctx);
extern u8 _021EA5E4[];
BOOL PokeathlonEventRecord_Main(void *manager, int *state);
#endif
