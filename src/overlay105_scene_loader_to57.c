#include "overlay105_scene_lifecycle_r46_private.h"

extern const u32 _021E5DC4[];
extern const u32 ov105_021E5DC8[];
extern Ov105NarcFiles _021E5E80;
extern Ov105NarcFiles ov105_021E5E8C;
extern Ov105NarcFiles ov105_021E5E98;
extern Ov105NarcFiles ov105_021E5EA4;

void ov105_021E5CA4(Ov105App *app) {
    u8 i;
    NNSG3dResTex *tex;
    NARC *narc;
    Ov105ModelSet *ms;
    Ov105NarcFiles *files;
    void *anm;

    narc = NARC_New((NarcId)0xF0, (enum HeapID)0x97);
    HeapExp_FndInitAllocator(&app->allocator, (enum HeapID)0x97, 4);
    if (app->unk9C == 0) {
        if (app->unk9D == 0) {
            files = &ov105_021E5E8C;
        } else {
            files = &_021E5E80;
        }
        app->seTable = ov105_021E5DC8;
    } else {
        if (app->unk9D == 0) {
            files = &ov105_021E5EA4;
        } else {
            files = &ov105_021E5E98;
        }
        app->seTable = _021E5DC4;
    }
    ms = &app->models[0];
    ms->fileHeader = NARC_AllocAndReadWholeMember(narc, files->modelFile, (enum HeapID)0x97);
    GF3dRender_InitObjFromHeader(&ms->renderObj, &ms->resMdl, &ms->fileHeader);
    tex = NNS_G3dGetTex(app->models[0].fileHeader);
    NNSi_G3dModifyMatFlag(ms->resMdl, FALSE, (NNSG3dMatFlag)0x40);
    NNSi_G3dModifyMatFlag(ms->resMdl, FALSE, (NNSG3dMatFlag)0x80);
    NNSi_G3dModifyMatFlag(ms->resMdl, FALSE, (NNSG3dMatFlag)0x200);
    NNSi_G3dModifyMatFlag(ms->resMdl, FALSE, (NNSG3dMatFlag)0x400);
    for (i = 0; i < 4; i++) {
        ms->anmData[i] = NARC_AllocAndReadWholeMember(narc, files->animFiles[i], (enum HeapID)0x97);
        anm = NNS_G3dGetAnmByIdx(ms->anmData[i], 0);
        ms->anmObj[i] = NNS_G3dAllocAnmObj(&app->allocator, anm, ms->resMdl);
        NNS_G3dAnmObjInit(ms->anmObj[i], anm, ms->resMdl, tex);
        NNS_G3dRenderObjAddAnmObj(&ms->renderObj, ms->anmObj[i]);
    }
    NARC_Delete(narc);
}
