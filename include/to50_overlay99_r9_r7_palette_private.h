#ifndef TO50_OV99_R9_R7_PRIVATE_H
#define TO50_OV99_R9_R7_PRIVATE_H
#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "gf_gfx_loader.h"
typedef struct Ov99R9R7Work {
    u32 unk0;
    BgConfig *bgConfig;
    NARC *narc;
    enum HeapID heapId;
    u8 pad10[0xa0];
    void *unkB0;
} Ov99R9R7Work;
void ov99_021E87D0(Ov99R9R7Work *work);
#endif
