#ifndef TO57_R2_08_PRIVATE_H
#define TO57_R2_08_PRIVATE_H
#include "global.h"

#include "overlay_99.h"

typedef struct CourseRecordWork {
    void *alloc;
    void *bgConfig;
    u32 unk08;
    u32 heapId;
    void *textContext;
    void *spriteContext;
    void *sprites[26];
    u32 unk80;
    u32 unk84;
    u32 requestedPage;
    u32 updatePending;
    u32 applyPending;
} CourseRecordWork;

typedef struct SpriteCounts6 {
    u32 v[6];
} SpriteCounts6;

extern u8 SDK_OVERLAY_OVY_98_ID;
extern char _021EA5C0[];
extern const u8 ov99_021E96D8[];
extern const SpriteCounts6 ov99_021E95A4;
void HandleLoadOverlay(u32 id, int mode);
void UnloadOverlayByID(u32 id);
void GF_AssertFail(void);
void ov99_021E5B54(CourseRecordWork *, const void *);
void ov99_021E5B74(void *);
void PokeathlonUtil_ResetDisplay(void);
void ov99_021E5C88(CourseRecordWork *);
void ov99_021E5D58(CourseRecordWork *);
void *PokeathlonUtil_CreateTextContext(u32, void *, u32, const void *, u32);
void *PokeathlonUtil_CreateSpriteContext(u32, const void *, u32);
void ov99_021E6274(CourseRecordWork *);
void ov99_021E6438(CourseRecordWork *);
void ov99_021E64E0(CourseRecordWork *, u32);
void ov99_021E5FE8(CourseRecordWork *, u32);
void ov99_021E6050(CourseRecordWork *, u32);
void ov99_021E6530(CourseRecordWork *, u32);
void ov99_021E6018(CourseRecordWork *);
void ov99_021E6250(CourseRecordWork *);
void ov99_021E5F74(CourseRecordWork *);
BOOL ov99_021E6638(CourseRecordWork *);
void ov99_021E5D2C(CourseRecordWork *);
void PokeathlonUtil_FreeTextContext(void *, u32);
void PokeathlonUtil_FreeSpriteContext(void *, void *, u32);
void PokeathlonUtil_DrawSprites(void *);
void PokeathlonUtil_DisableDisplay(void);
void PlaySE(u32);
void ResetVisibleHardwareWindows(int);
void Main_SetVBlankIntrCB(void (*)(void *), void *);
void SetKeyRepeatTimers(int, int);
void BeginNormalPaletteFade(u32, u32, u32, u32, u32, u32, u32);
BOOL IsPaletteFadeFinished(void);
#endif
