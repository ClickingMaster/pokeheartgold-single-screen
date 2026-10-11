#ifndef TO46_OVERLAY40_MODE_TRANSITION_TAIL_PRIVATE_H
#define TO46_OVERLAY40_MODE_TRANSITION_TAIL_PRIVATE_H

#include "global.h"

#define PTR(p, off) (*(void **)((u8 *)(p) + (off)))
#define S32(p, off) (*(s32 *)((u8 *)(p) + (off)))

void BgClearTilemapBufferAndCommit(void *, int);
void GfGfx_EngineATogglePlanes(int, int);
void GfGfx_EngineBTogglePlanes(int, int);
void PaletteData_LoadPaletteSlotFromHardware(void *, int, int, int);
void SetBgPriority(int, int);
void GlobalTerminalNet_UnpackDressUpPhoto(void *, void *);
void GlobalTerminalNet_CopyRequestHeader(void *, void *);
void VsRecorder_ResetBg2(void *);
void VsRecorder_SetSubState(void *, int);
int ov40_0222DA00(void *, void *, int, int);
void ov40_0222E79C(void *, void *);
void ov40_0222E7B8(void *, void *);
void ov40_0222E7DC(void *, int);
void ov40_022307DC(void *, int, int);
void ov40_02230964(void *, int);
void ov40_02235868(void *, u32);
void ov40_02235940(void);
void ov40_02235994(void);
void ov40_02235C7C(void *, int);
void ov40_02236130(void *);
void *AccessoryPortrait_Create(void *, void *);
void AccessoryPortrait_Destroy(void);
void AccessoryPortrait_SetMonVisible(void *, int);

int ov40_02235434(void *);
int ov40_02235644(void *);

#endif
