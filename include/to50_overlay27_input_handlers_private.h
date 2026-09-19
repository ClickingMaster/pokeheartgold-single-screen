#ifndef TO50_OVERLAY27_RESIDUAL23_PRIVATE_H
#define TO50_OVERLAY27_RESIDUAL23_PRIVATE_H
#define ov27_0225CD94 ov27_0225CD94_old_decl
#include "overlay_27_helpers_internal.h"
#undef ov27_0225CD94
extern const u8 ov27_0225D120[];
extern u8 gSystem[];
int TouchscreenHitbox_FindRectAtTouchNew(const void *hitboxes);
u32 ov27_0225CD94(Ov27Work *work);
void ov27_0225CEAC(Ov27Work *work);
void PlaySE(u16 seq);
#define NEW_KEYS (*(u32 *)(gSystem + 0x48))
#endif
