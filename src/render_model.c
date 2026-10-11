#include "render_model.h"

#include "global.h"

#include "gf_3d_render.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "sys_task_api.h"

typedef struct RenderModel {
    u32 loaded;
    NNSG3dResFileHeader *file;
    NNSG3dResMdlSet *set;
    NNSG3dResMdl *model;
    NNSG3dResTex *texture;
} RenderModel;

void RenderModel_Clear(RenderModel *model);
void RenderModel_InitFromFile(RenderModel *model, NNSG3dResFileHeader *file, u32 index);
void RenderModel_LoadFromNarc(RenderModel *model, int index, NARC *narc, int fileId, enum HeapID heapID, BOOL atEnd);
void RenderModel_BindTextures(RenderModel *model);
void RenderModel_BindTexturesTask(SysTask *task, void *data);
void RenderModel_BindTexturesOnVBlank(RenderModel *model);
void RenderModel_FreeFile(RenderModel *model);
void RenderModel_ReleaseTextures(RenderModel *model);
void RenderModel_Free(UnkOv01_021FFECC_sub *data);

void RenderModel_Clear(RenderModel *model) {
    u32 count = sizeof(*model);
    u8 *bytes = (u8 *)model;
    do {
        *bytes++ = 0;
    } while (--count);
}

void RenderModel_InitFromFile(RenderModel *model, NNSG3dResFileHeader *file, u32 index) {
    RenderModel_Clear(model);
    model->file = file;
    model->loaded = FALSE;
    model->set = NNS_G3dGetMdlSet(file);
    model->model = NNS_G3dGetMdlByIdx(model->set, index);
    model->texture = NNS_G3dGetTex(file);
}

void RenderModel_LoadFromNarc(RenderModel *model, int index, NARC *narc, int fileId, enum HeapID heapID, BOOL atEnd) {
    void *file = GfGfxLoader_LoadFromOpenNarc(narc, fileId, FALSE, heapID, atEnd);
    RenderModel_InitFromFile(model, file, index);
}

void RenderModel_BindTextures(RenderModel *model) {
    GF3dRender_AllocAndLoadTexResources(model->texture);
    NNS_G3dBindMdlSet(model->set, model->texture);
    model->loaded = TRUE;
}

void RenderModel_BindTexturesTask(SysTask *task, void *data) {
    RenderModel_BindTextures(data);
    SysTask_Destroy(task);
}

void RenderModel_BindTexturesOnVBlank(RenderModel *model) {
    SysTask *task = SysTask_CreateOnVBlankQueue(RenderModel_BindTexturesTask, model, 0xFFFF);
    GF_ASSERT(task != NULL);
}

void RenderModel_FreeFile(RenderModel *model) {
    if (model->file) {
        Heap_Free(model->file);
    }
}

void RenderModel_ReleaseTextures(RenderModel *model) {
    if (model->texture) {
        NNSG3dTexKey key, key4x4;
        NNS_G3dTexReleaseTexKey(model->texture, &key, &key4x4);
        NNS_GfdFreeTexVram(key);
        NNS_GfdFreeTexVram(key4x4);
        NNS_GfdFreePlttVram(NNS_G3dPlttReleasePlttKey(model->texture));
        model->texture = NULL;
    }
}

void RenderModel_Free(UnkOv01_021FFECC_sub *data) {
    RenderModel *model = (RenderModel *)data;
    RenderModel_ReleaseTextures(model);
    RenderModel_FreeFile(model);
    RenderModel_Clear(model);
}
