#include "global.h"

void ov41_0224AFF8(void **out, void **managers, int id, void *narc,
                   int charFile, int plttFile, int cellFile, int animFile,
                   int plttVram, int base);
void *AddCharResObjFromOpenNarc(void *, void *, int, int, int, int, int);
void *AddPlttResObjFromOpenNarc(void *, void *, int, int, int, int, int, int);
void *AddCellOrAnimResObjFromOpenNarc(void *, void *, int, int, int, int, int);
void SpriteTransfer_CreateCharTransferTask_AllocAtEnd(void *);
void SpriteTransfer_CreatePlttTransferTask(void *);
void sub_0200A740(void *);

void ov41_0224AFF8(void **out, void **managers, int id, void *narc,
                   int charFile, int plttFile, int cellFile, int animFile,
                   int plttVram, int base)
{
    out[0] = AddCharResObjFromOpenNarc(managers[0], narc, charFile, FALSE,
                                        base + charFile, 2, id);
    SpriteTransfer_CreateCharTransferTask_AllocAtEnd(out[0]);
    sub_0200A740(out[0]);

    out[1] = AddPlttResObjFromOpenNarc(managers[1], narc, plttFile, FALSE,
                                       base + plttFile, 2, plttVram, id);
    SpriteTransfer_CreatePlttTransferTask(out[1]);
    sub_0200A740(out[1]);

    out[2] = AddCellOrAnimResObjFromOpenNarc(managers[2], narc, cellFile, FALSE,
                                              base + cellFile, 2, id);
    out[3] = AddCellOrAnimResObjFromOpenNarc(managers[3], narc, animFile, FALSE,
                                              base + animFile, 3, id);
}
