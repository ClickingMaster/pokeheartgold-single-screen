#ifndef TO50_OVERLAY93_PALETTE_TILEMAP_R7_PRIVATE_H
#define TO50_OVERLAY93_PALETTE_TILEMAP_R7_PRIVATE_H

#include "global.h"

#include "bg_window.h"

#define PTR_AT(p, off) (*(void **)((u8 *)(p) + (off)))
#define U8_AT(p, off)  (*(u8 *)((u8 *)(p) + (off)))

void ov93_02260BB0(void *sys);

#endif
