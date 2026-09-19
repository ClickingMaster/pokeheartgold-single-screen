#ifndef W20_R6_PRIVATE_H
#define W20_R6_PRIVATE_H
#include "global.h"

#include "bg_window.h"
#include "overlay_manager.h"
#include "palette.h"
#include "screen_fade.h"
#include "sound_02004A44.h"
#include "sprite_system.h"
#include "unk_02019BA4.h"
typedef struct R8WorkW20 {
    BgConfig *bg;
    u32 unk4;
    NARC *narc;
    enum HeapID heap;
    u8 pad010[0x3E0];
    void *alloc3F0;
    u32 flags;
    u32 unk3F8;
    GridInputHandler *grid;
    void *text;
    void *unk404;
} R8WorkW20;
extern const DpadMenuBox ov99_021EA250[];
extern const GridCallbacks ov99_021E9FA0;
extern const TouchscreenHitbox ov99_021EA03C[];
extern const char _021EA5D8;
void GF_AssertFail(void);
void PlaySE(u16);
void ov99_021E7794(R8WorkW20 *);
BOOL ov99_021E7598(R8WorkW20 *);
BOOL ov99_021E7BC8(R8WorkW20 *);
BOOL ov98_0221EF24(void);
void ov98_0221E6CC(void *);
BOOL PokeathlonMedals_Main(OverlayManager *, int *);
void ov99_021E7A54(R8WorkW20 *);
void ov99_021E7A78(R8WorkW20 *);
#endif
