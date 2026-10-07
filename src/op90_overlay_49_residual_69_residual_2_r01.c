#pragma force_active on
#include "global.h"
#include <nnsys/g3d/binres/res_struct_accessor.h>
#include <nnsys/fnd/allocator.h>
#include "heap.h"

typedef struct Ov49Tex {
    u8 unk0[0xC];
    NNSG3dResTex *tex;
} Ov49Tex;

typedef struct Ov49Anim {
    void *anim;
    u8 unk4[0x10];
} Ov49Anim;

typedef struct Ov49Unk {
    u8 pad0[0x10550];
    Ov49Tex texs[15];
    NNSG3dResFileHeader *files[39];
    Ov49Anim anims[16];
    NNSFndAllocator allocator;
} Ov49Unk;

typedef struct Ov49AnimDef {
    u8 texIndex;
    u8 fileIndex;
    u8 animIds[2];
} Ov49AnimDef;

extern const Ov49AnimDef ov49_0226A70C[];

void sub_020180BC(Ov49Anim *animation, Ov49Tex *model, void *narc, int fileId, enum HeapID heapID, NNSFndAllocator *allocator);
void ov49_022657B4(Ov49Unk *data, void *narc, enum HeapID heapID);

// @fn ov49_022657B4
void ov49_022657B4(Ov49Unk *data, void *narc, enum HeapID heapID) {
    int i, j;
    const Ov49AnimDef *def = ov49_0226A70C;
    for (i = 0; i < 39; i++) {
        for (j = 0; j < 2; j++) {
            if (def->animIds[j] == 17) {
                continue;
            }
            if (data->anims[def->animIds[j]].anim != NULL) {
                continue;
            }
            data->texs[def->texIndex].tex = NNS_G3dGetTex(data->files[def->fileIndex]);
            sub_020180BC(&data->anims[def->animIds[j]], &data->texs[def->texIndex], narc, def->animIds[j] + 183, heapID, &data->allocator);
        }
        def++;
    }
}
