#pragma force_active on
#include "global.h"
#include "gf_gfx_loader.h"
#include "nnsys/g3d/binres/res_struct_accessor.h"
#include "nnsys/g3d/binres/res_struct_accessor_inline.h"
typedef struct Model49 {
    void *res;
    NNSG3dResMdlSet *mdlSet;
    NNSG3dResMdl *mdl;
    NNSG3dResTex *tex;
} Model49;
typedef struct Anim49 {
    u8 unk00[8];
    void *anmObj;
    u8 unk0C[8];
} Anim49;
typedef struct Obj49 {
    Model49 models[2];
    void *texRes[2][3];
    Anim49 anims[2][4];
} Obj49;
typedef struct Data49 {
    u8 unk000[0x120];
    int mdlIds[2];
    int texIds[2][3];
    int animIds[2][4];
    u32 animCounts[2];
    u32 skip[2][3];
} Data49;
void ov49_02258830(void **dest, NARC *narc, int fileId, enum HeapID heapId);
void sub_020180BC(Anim49 *anim, Model49 *model, NARC *narc, int fileId, enum HeapID heapId, enum HeapID heapId2);
void NNS_G3dAnmObjDisableID(void *anmObj, int id);
void ov49_0225D854(Obj49 *obj, NARC *narc, enum HeapID heapId2, Data49 *data, enum HeapID heapId);
// @fn ov49_0225D854
void ov49_0225D854(Obj49 *obj, NARC *narc, enum HeapID heapId2, Data49 *data, enum HeapID heapId) {
    int i;
    int j;
    u32 k;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 3; j++) {
            ov49_02258830(&obj->texRes[i][j], narc, data->texIds[i][j], heapId);
        }
    }
    for (i = 0; i < 2; i++) {
        obj->models[i].res = GfGfxLoader_LoadFromOpenNarc(narc, data->mdlIds[i], FALSE, heapId, FALSE);
        obj->models[i].mdlSet = NNS_G3dGetMdlSet(obj->models[i].res);
        obj->models[i].mdl = NNS_G3dGetMdlByIdx(obj->models[i].mdlSet, 0);
        obj->models[i].tex = NNS_G3dGetTex(obj->texRes[i][0]);
    }
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 4; j++) {
            if (data->animIds[i][j] != data->mdlIds[0]) {
                sub_020180BC(&obj->anims[i][j], &obj->models[i], narc, data->animIds[i][j], heapId, heapId2);
                if (j >= 1) {
                    for (k = 0; k < data->animCounts[i]; k++) {
                        if (k != data->skip[i][j - 1]) {
                            NNS_G3dAnmObjDisableID(obj->anims[i][j].anmObj, k);
                        }
                    }
                }
            }
        }
    }
}
