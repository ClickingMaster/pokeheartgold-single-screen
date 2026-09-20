#ifndef TO56_R3_W19_PRIVATE_H
#define TO56_R3_W19_PRIVATE_H
#include "global.h"
typedef struct R3Work {
    u32 heapId;
    u32 flags;
    u32 charVram;
    void *charMan;
    void *plttMan;
    void *cellMan;
    void *animMan;
    void *chars[12];
    void *cells[12];
    void *anims[12];
    void *palette;
    void *palette2;
} R3Work;
void *AddCharResObjFromNarc(void *, u32, u32, u32, u32, u32, u32);
void *AddCellOrAnimResObjFromNarc(void *, u32, u32, u32, u32, u32, u32);
void *AddPlttResObjFromNarc(void *, u32, u32, u32, u32, u32, u32, u32);
void SpriteTransfer_CreateCharTransferTask_AllocAtEnd(void *);
void SpriteTransfer_CreateExtPlttTransferTask(void *);
void ov96_021E9D10(R3Work *);
#endif
