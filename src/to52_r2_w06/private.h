#ifndef TO45_SOL_R3_OV40_PC_MODE_PREFIX_H
#define TO45_SOL_R3_OV40_PC_MODE_PREFIX_H

#include "global.h"
#include <nitro/fs/overlay.h>

#define PTR(p, off)  (*(void **)((u8 *)(p) + (off)))
#define U32(p, off)  (*(u32 *)((u8 *)(p) + (off)))
#define S32(p, off)  (*(s32 *)((u8 *)(p) + (off)))
#define U16(p, off)  (*(u16 *)((u8 *)(p) + (off)))
#define S16(p, off)  (*(s16 *)((u8 *)(p) + (off)))
#define U8(p, off)   (*(u8 *)((u8 *)(p) + (off)))

typedef struct Block44 { u32 v[11]; } Block44;
typedef void (*HitboxCallback)(u32, int, void *);

void ov40_022307DC(void *, int, int);
void GfGfx_EngineATogglePlanes(int, int);
void GfGfx_EngineBTogglePlanes(int, int);
void *TouchHitboxController_Create(const void *, int, HitboxCallback, void *, int);
int TouchHitboxController_IsTriggered(void *);
void TouchHitboxController_Destroy(void *);
int ov40_0222DA84(void *, int);
int ov40_0222DA00(void *, void *, int, int);
void ov40_0222DED0(void *, int);
void ov40_02235FFC(void *);
void PaletteData_BlendPalettes(void *, int, int, int, int);
BOOL TouchscreenHitbox_TouchNewIsIn(const void *);
void ov40_02230944(void *);
void ov40_0223610C(void);
void ov40_0222DFB0(void *);
void ov40_022359B4(void *, int);
void ov40_02230964(void *, int);
void ov40_0222F9D4(void *, void *);
void ov40_0222F734(void *);
void ov40_0222E9B8(void *, void *, void *, void *);
void ov40_0222FA5C(void *, void *);
void ov40_0222F740(void *, void *, int);
void ov40_0222FA88(void *);
void ov40_0222F6D0(void *, s16);
int ov40_0222F38C(void *, void *);
void ov40_0222FA24(void *);
void ov40_0222F720(void *);
void ov40_0222F920(void *, void *);
void ov40_02235FA0(void *);
void SetBgPriority(int, int);
void Heap_Free(void *);
void sub_020314BC(void *);
void BgClearTilemapBufferAndCommit(void *, int);
void ov40_02235B10(void *);
void ov40_0222DAA8(void *);
void ov40_0222D88C(void *);
void ov40_0222FB90(void *, int);
int ov40_0222FBB4(void *);
void ov41_0224B57C(void);
void ov40_0222BC54(void *);
void ov40_02235994(void);
void ov40_0222DD08(void *);
void ov40_0222BF64(void *, int, int, void *);
void ov40_0222BF80(void *, int);
void UnloadOverlayByID(int);
void Main_SetVBlankIntrCB(void (*)(void *), void *);
void ov40_0222BD04(void *);
int ov40_0223D5CC(void *);
void ov40_0223077C(void *, void *, int, int);
void sub_020879E0(void *, int);
void sub_02087A08(void *, int, int);
void PlaySE(int);
void *ov40_0223D540(void *);
int GlobalTerminalNet_RequestDressUpDownload(void *, void *);
int GlobalTerminalNet_GetError(void *, void **);
void StopSE(int, int);
void ov40_02230CDC(void *, int, void *, void *);
BOOL System_GetTouchNew(void);

extern const u8 ov40_02245708[];
extern const u8 ov40_022451C4[];
extern const Block44 ov40_022451F4;
extern u8 SDK_OVERLAY_OVY_41_ID[];
void ov40_02235FD0(u32, int, void *);

int ov40_02234A38(void *);
int ov40_02234BBC(void *);
int ov40_02234D94(void *);
int ov40_02234F98(void *);

#endif
