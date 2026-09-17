#ifndef TO50_OV65_R14_R4_PRIVATE_H
#define TO50_OV65_R14_R4_PRIVATE_H
#include <nitro/gx/gx_load.h>
#include <nitro/os/interrupt.h>

#include "global.h"

#include "bg_window.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_manager.h"
#include "string.h"
#include "system.h"
#include "unk_0200B150.h"
#include "vram_transfer_manager.h"
#define U16(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define U32(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))
typedef struct Ov65StateR14R4 {
    u8 pad0[0x180];
    void *bgConfig;
    u8 pad184[0x678];
    u8 objData[0x1900];
    u16 palPairs[16];
    u8 pad211C[0x20];
    u32 updateFlag;
} Ov65StateR14R4;
extern void ov65_0221CD0C(void *);
extern void ov65_0221CCB0(void *);
extern void ov65_0221FAE0(void *);
extern void ov65_0221D1C8(void *);
extern void FontID_Release(u32);
BOOL WirelessTradeSelectMon_Exit(OverlayManager *, int *);
void ov65_0221CE1C(void *);
#endif
