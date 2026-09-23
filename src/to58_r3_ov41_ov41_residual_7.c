#include "global.h"

void ov41_02246304(void *work, void *narc, int fileId, int compressed, int id,
                   int vram, int plttNum);

void *AddPlttResObjFromOpenNarc(void *, void *, int, int, int, int, int, int);
void SpriteTransfer_CreatePlttTransferTask(void *);

void ov41_02246304(void *work, void *narc, int fileId, int compressed, int id,
                   int vram, int plttNum)
{
    void *resource = AddPlttResObjFromOpenNarc(*(void **)((u8 *)work + 0x4c),
                                                narc, fileId, compressed,
                                                plttNum, id, vram, 0xe);
    SpriteTransfer_CreatePlttTransferTask(resource);
}
