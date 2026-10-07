#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
typedef struct {
    u8 pad0[0x1d48];
    u16 sendBlock;
    u16 reqBlock;
    u16 totalBlock;
    u8 pad1[4];
    u8 active;
} MbChildSlot;
extern u8 *mbc;
extern u32 _02247480;
void MBi_calc_sendblock(int child);
#define SLOT(i) ((MbChildSlot *)(mbc + (i) * 0x5d4))
// @fn MBi_calc_sendblock
void MBi_calc_sendblock(int child) {
    if (!(_02247480 & (1 << child))) return;
    if (SLOT(child)->active == 0 || SLOT(child)->totalBlock == 0) return;
    if (SLOT(child)->reqBlock <= SLOT(child)->sendBlock && SLOT(child)->sendBlock <= SLOT(child)->reqBlock + 2) {
        SLOT(child)->sendBlock++;
    } else {
        SLOT(child)->sendBlock = SLOT(child)->reqBlock;
    }
}
