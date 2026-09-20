#include "to56_overlay96_r20_part1_private.h"
#pragma force_active on

#define LOAD_GROUP(slot, bit, file, id, charVram)                                                                    \
    do {                                                                                                             \
        if (work->flags & (bit)) {                                                                                   \
            work->chars[slot] = AddCharResObjFromNarc(work->charMan, 0x99, file + 2, 0, id, charVram, work->heapId); \
            work->cells[slot] = AddCellOrAnimResObjFromNarc(work->cellMan, 0x99, file + 1, 0, id, 2, work->heapId);  \
            work->anims[slot] = AddCellOrAnimResObjFromNarc(work->animMan, 0x99, file, 0, id, 3, work->heapId);      \
            SpriteTransfer_CreateCharTransferTask_AllocAtEnd(work->chars[slot]);                                     \
        }                                                                                                            \
    } while (0)

void ov96_021E9D10(R3Work *work) {
    LOAD_GROUP(0, 0x001, 1, 0x14, 1);
    LOAD_GROUP(1, 0x004, 4, 0x15, work->charVram);
    LOAD_GROUP(2, 0x002, 7, 0x16, work->charVram);
    LOAD_GROUP(4, 0x010, 14, 0x1A, 1);
    LOAD_GROUP(5, 0x020, 17, 0x1B, 1);
    LOAD_GROUP(3, 0x008, 11, 0x19, 2);
    LOAD_GROUP(6, 0x040, 20, 0x1C, 1);
    LOAD_GROUP(7, 0x080, 23, 0x1D, 1);
    LOAD_GROUP(8, 0x100, 26, 0x1E, 2);
    LOAD_GROUP(9, 0x200, 29, 0x1F, 1);
    LOAD_GROUP(10, 0x400, 32, 0x1F, 2);
    LOAD_GROUP(11, 0x800, 35, 0x20, 1);
    u32 vram = work->charVram;
    if (vram == 2) {
        vram = 3;
    }
    work->palette = AddPlttResObjFromNarc(work->plttMan, 0x99, 0, 0, 0x17, vram, 1, work->heapId);
    SpriteTransfer_CreateExtPlttTransferTask(work->palette);
    work->palette2 = NULL;
    if ((work->flags & 0x008) || (work->flags & 0x100) || (work->flags & 0x400)) {
        work->palette2 = AddPlttResObjFromNarc(work->plttMan, 0x99, 0xA, 0, 0x18, 2, 1, work->heapId);
        SpriteTransfer_CreateExtPlttTransferTask(work->palette2);
    }
}
