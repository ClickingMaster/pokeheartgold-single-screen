#ifndef TO50_OV99_R9_R4_PRIVATE_H
#define TO50_OV99_R9_R4_PRIVATE_H
#include "global.h"
typedef u8 Ov99EventWork;
extern u8 SDK_OVERLAY_OVY_98_ID;
void HandleLoadOverlay(u32 id, s32 mode);
void *OverlayManager_GetArgs(void *manager);
void GF_AssertFail(void);
BOOL Heap_Create(u32 parent, u32 heapId, u32 size);
void *OverlayManager_CreateAndGetData(void *manager, u32 size, u32 heapId);
void MI_CpuFill8(void *dest, u8 value, u32 size);
void ov99_021E93DC(void *work, void *args);
void ov98_0221F090(void);
void ov99_021E8590(void *work);
void ov99_021E860C(void *work);
void *ov98_0221EABC(u32 heapId, void *bgConfig, u32 count, const void *config, u32 value);
void ov99_021E88D4(void *work);
void ov99_021E8788(void *work);
void ov99_021E8C04(void *work);
void ov99_021E8818(void *work);
void ResetVisibleHardwareWindows(int screen);
void Main_SetVBlankIntrCB(void (*callback)(void *), void *arg);
void ov99_021E856C(void *work);
extern const u32 ov99_021EA45C[];
BOOL PokeathlonEventRecord_Init(void *manager, int *state);
#endif
