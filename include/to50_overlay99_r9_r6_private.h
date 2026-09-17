#ifndef TO50_OV99_R9_R6_PRIVATE_H
#define TO50_OV99_R9_R6_PRIVATE_H
#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "gf_gfx_loader.h"

typedef struct Ov99R9R6Work {
    u32 unk0;
    BgConfig *bgConfig;
    NARC *narc;
    enum HeapID heapId;
} Ov99R9R6Work;

extern const GraphicsModes ov99_021EA374;
typedef struct Ov99BgTemplates {
    BgTemplate templates[6];
} Ov99BgTemplates;
extern const Ov99BgTemplates ov99_021EA4F4;
extern const u8 ov99_021EA59C[6];
void SpriteSystem_TransferOam(void *work);
void DoScheduledBgGpuUpdates(BgConfig *bgConfig);
void ov99_021E856C(Ov99R9R6Work *work);
void ov99_021E8590(Ov99R9R6Work *work);
void ov99_021E860C(Ov99R9R6Work *work);
#endif
