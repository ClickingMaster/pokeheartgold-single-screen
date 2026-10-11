#include "global.h"

#include "wifi_plaza_internal.h"

// Wi-Fi Plaza player objects: one billboard sprite (field character model) plus a model under it for every player
// on the plaza map (overlay 49 drives them). The first functions are the last lobby client helpers.

void ov45_02230394(int result, void *arg) {
    WiFiPlazaLobbyClient *client = arg;
    if (client->unk_69e == 1) {
        ov45_0222F154();
    } else if (result == 0) {
        _022577C0->unk_69f = 1;
    }
}

u32 ov45_022303BC(WiFiPlazaLobbyClientUnk1B4 *unk, int key) {
    u32 result = 0;
    int i;
    for (i = 0; i < unk->count; i++) {
        if (key == unk->values[i].key) {
            result = unk->values[i].value;
        }
    }
    return result;
}

// The list header overlays the first 8 bytes of WiFiPlazaLobbyClient.unk_6a0.
void ov45_022303E4(WiFiPlazaLobbyClientUnk6A0 *unk, int count, enum HeapID heapID) {
    WiFiPlazaLobbyIdValueList *list = (WiFiPlazaLobbyIdValueList *)unk;
    list->items = Heap_Alloc(heapID, count * sizeof(WiFiPlazaLobbyIdValue));
    list->capacity = count;
    list->count = 0;
}

void ov45_022303FC(WiFiPlazaLobbyClientUnk6A0 *unk) {
    WiFiPlazaLobbyIdValueList *list = (WiFiPlazaLobbyIdValueList *)unk;
    Heap_Free(list->items);
    list->items = NULL;
}

void ov45_0223040C(WiFiPlazaLobbyClientUnk6A0 *unk, const void *data, u32 count) {
    WiFiPlazaLobbyIdValueList *list = (WiFiPlazaLobbyIdValueList *)unk;
    GF_ASSERT(count < list->capacity);
    if (count >= list->capacity) {
        count = list->capacity;
    }
    MIi_CpuCopy32(data, (u32 *)list->items, count * sizeof(WiFiPlazaLobbyIdValue));
    list->count = count;
}

BOOL ov45_02230434(WiFiPlazaLobbyClientUnk6A0 *unk, int id) {
    WiFiPlazaLobbyIdValueList *list = (WiFiPlazaLobbyIdValueList *)unk;
    int i;
    for (i = 0; i < list->count; i++) {
        if (id == list->items[i].id) {
            return TRUE;
        }
    }
    return FALSE;
}

u32 ov45_0223045C(WiFiPlazaLobbyClientUnk6A0 *unk, int id) {
    WiFiPlazaLobbyIdValueList *list = (WiFiPlazaLobbyIdValueList *)unk;
    int i;
    for (i = 0; i < list->count; i++) {
        if (id == list->items[i].id) {
            return list->items[i].value;
        }
    }
    return 0;
}

void ov45_0223048C(u32 *dest, const void *data) {
    memcpy(dest, data, 0x2D8);
}

WiFiPlazaPlayerObjects *ov45_02230498(u32 count, BOOL mode, enum HeapID heapID, enum HeapID loadHeapID) {
    WiFiPlazaPlayerObjects *objects;
    int i;
    NARC *modelNarc;
    NARC *narc;
    void *file;
    GF_3DGfxRawResObj *texture;
    WiFiPlazaBillboardParams params;
    int skipped;

    objects = Heap_Alloc(heapID, sizeof(WiFiPlazaPlayerObjects));
    memset(objects, 0, sizeof(WiFiPlazaPlayerObjects));
    objects->objects = Heap_Alloc(heapID, count * sizeof(WiFiPlazaPlayerObject));
    objects->count = count;
    for (i = 0; i < objects->count; i++) {
        ov45_02230AB4(&objects->objects[i]);
    }

    objects->models = GF2dGfxRawResMan_Create(1, heapID);
    objects->sequences = GF2dGfxRawResMan_Create(2, heapID);
    objects->textures = GF3dGfxRawResMan_Create(20, heapID);
    BillboardLists_Create(1, heapID);
    params.count = count;
    params.heapID = heapID;
    objects->billboards = Billboard_Create(&params);

    modelNarc = NARC_New(NARC_data_mmodel_mmodel, heapID);
    narc = NARC_New(NARC_a_2_0_7, heapID);
    file = GfGfxLoader_LoadFromOpenNarc(narc, 0x7F, FALSE, loadHeapID, FALSE);
    GF2dGfxRawResMan_AllocObj(objects->models, file, 0x7F);
    ov45_0222D740(file);
    objects->lightFlags = NNS_G3dMdlGetMdlLightEnableFlag(NNS_G3dGetMdlByIdx(NNS_G3dGetMdlSet(file), 0), 0);

    for (i = 0; i < 2; i++) {
        file = GfGfxLoader_LoadFromOpenNarc(modelNarc, ov45_02254C38[i], FALSE, loadHeapID, FALSE);
        GF2dGfxRawResMan_AllocObj(objects->sequences, file, ov45_02254C38[i]);
    }

    if (mode == FALSE) {
        skipped = 1;
    } else {
        skipped = 0;
    }
    {
        const WiFiPlazaSpriteConfig *config = ov45_02254C48;
        int index = 0;
        do {
            if (skipped != index) {
                BOOL loadTexture = config->ownTexture == 0;
                file = GfGfxLoader_LoadFromOpenNarc(modelNarc, config->fileId, FALSE, loadHeapID, FALSE);
                texture = GF3dGfxRawResMan_AllocObj(objects->textures, file, config->fileId, loadTexture, loadHeapID);
                if (loadTexture == TRUE) {
                    GF3dGfxRawResObj_AllocVramAndGetKeys(texture);
                    GF3dGfxRawResObj_LoadTex(texture);
                    GF3dGfxRawResObj_FreeVramAndSecondaryHeader(texture);
                }
            }
            config++;
            index++;
        } while (index < 20);
    }

    ov45_022309E8(&objects->model, narc, 0x80, loadHeapID);
    NARC_Delete(modelNarc);
    NARC_Delete(narc);
    return objects;
}

void ov45_02230638(WiFiPlazaPlayerObjects *objects) {
    ov45_02230A44(&objects->model);
    GF3dGfxRawResMan_FreeAllObjs(objects->textures);
    GF2dGfxRawResMan_FreeAllObjs(objects->models);
    GF2dGfxRawResMan_FreeAllObjs(objects->sequences);
    Billboard_Delete(objects->billboards);
    BillboardLists_Delete();
    GF3dGfxRawResMan_Destroy(objects->textures);
    GF2dGfxRawResObj_Destroy(objects->models);
    GF2dGfxRawResObj_Destroy(objects->sequences);
    Heap_Free(objects->objects);
    Heap_Free(objects);
}

void ov45_02230680(WiFiPlazaPlayerObjects *objects) {
    u32 i;
    for (i = 0; i < objects->count; i++) {
        ov45_02230ACC(&objects->objects[i]);
        ov45_02230CB0(&objects->objects[i]);
        ov45_02230DF4(&objects->objects[i]);
    }
}

void ov45_022306B4(WiFiPlazaPlayerObjects *objects) {
    u32 i;
    BillboardLists_Draw();
    if (ov45_02230A58(&objects->model)) {
        for (i = 0; i < objects->count; i++) {
            if (ov45_02230AA4(&objects->objects[i]) == TRUE) {
                ov45_02230AC0(&objects->objects[i]);
            }
        }
    }
}

void ov45_022306F4(WiFiPlazaPlayerObjects *objects) {
    sub_02023910(objects->billboards);
}

void ov45_02230700(WiFiPlazaPlayerObjects *objects, int alpha) {
    ov45_02230A4C(&objects->model, alpha);
}

WiFiPlazaPlayerObject *ov45_0223070C(WiFiPlazaPlayerObjects *objects, UnkStruct_ov42_02228110 *player) {
    WiFiPlazaBillboardResource resource;
    WiFiPlazaBillboardCreateParams create;
    WiFiPlazaBillboardSequence sequence;
    UnkStruct_ov44_02232914 gridPos;
    const WiFiPlazaSpriteConfig *config;
    GF_3DGfxRawResObj *texture;
    void *modelFile;
    NNSG3dResTex *tex;
    NNSG3dTexKey tex4x4Key;
    NNSG3dTexKey texKey;

    WiFiPlazaPlayerObject *object = ov45_02230A6C(objects);
    object->player = player;
    VecFx32 position = { 0, 0, 0 };
    VecFx32 scale = ov45_02254C3C;
    config = ov45_0223099C(ov42_02228188(player, 7));
    modelFile = GF2dGfxRawResObj_GetData(GF2dGfxRawResMan_GetObjById(objects->models, 0x7F));
    texture = GF3dGfxRawResMan_GetObjById(objects->textures, config->fileId);
    tex = GF3dGfxRawResObj_GetTex(texture);
    if (config->ownTexture == 1) {
        texKey = 0x119;
    } else {
        texKey = 0x118;
    }
    BillboardGfxSequence_Init(GF2dGfxRawResObj_GetData(GF2dGfxRawResMan_GetObjById(objects->sequences, texKey)), &sequence);
    if (config->ownTexture == 1) {
        BillboardResource_Init(&resource, modelFile, tex, &ov45_02254C98, &sequence);
    } else {
        texKey = GF3dGfxRawResObj_GetTexKey(texture);
        tex4x4Key = GF3dGfxRawResObj_GetTex4x4Key(texture);
        BillboardResource_InitWithKeys(&resource, modelFile, tex, &ov45_02254C98, &sequence, texKey, tex4x4Key, GF3dGfxRawResObj_GetPlttKey(texture));
    }
    gridPos = ov42_022282DC(player);
    WiFiPlazaMap_PosToVecFx32(&gridPos.unk0, &position.x);
    position.z += FX32_ONE * 16;
    create.billboards = objects->billboards;
    create.resource = &resource;
    create.position = position;
    create.scale = scale;
    object->sprite = BillboardObject_Create(&create);
    BillboardObject_SetCallback(object->sprite, ov45_02230E64, object);
    ov45_02230A5C(&objects->model.resource, &object->model);
    ModelInstance_SetPosition(&object->model, position.x, FX32_ONE * 2, position.z - FX32_ONE * 8);
    object->mode = 1;
    object->visible = 1;
    object->culled = 0;
    object->lightFlags = objects->lightFlags;
    return object;
}

void ov45_02230884(WiFiPlazaPlayerObject *object) {
    BillboardObject_Delete(object->sprite);
    memset(object, 0, sizeof(WiFiPlazaPlayerObject));
}

void ov45_0223089C(WiFiPlazaPlayerObject *object, u32 mode) {
    object->mode = mode;
}

void ov45_022308B0(WiFiPlazaPlayerObject *object, u32 lightFlags) {
    object->lightFlags = lightFlags;
}

u32 ov45_022308B8(WiFiPlazaPlayerObject *object) {
    return object->culled;
}

void ov45_022308C0(WiFiPlazaPlayerObject *object, const UnkStruct_ov44_02232914 *gridPos) {
    VecFx32 position;
    WiFiPlazaMap_PosToVecFx32(&gridPos->unk0, &position.x);
    position.z += FX32_ONE * 16;
    ov45_022308E4(object, &position);
}

void ov45_022308E4(WiFiPlazaPlayerObject *object, const VecFx32 *position) {
    BillboardObject_SetPosition(object->sprite, position);
    ModelInstance_SetPosition(&object->model, position->x, FX32_ONE * 2, position->z - FX32_ONE * 8);
}

void ov45_02230908(WiFiPlazaPlayerObject *object, VecFx32 *position) {
    *position = *BillboardObject_GetPosition(object->sprite);
}

void ov45_02230920(WiFiPlazaPlayerObject *object, u32 sequence) {
    BillboardObject_SetSequence(object->sprite, ov45_022309C4(1, sequence));
    BillboardObject_SetSequenceFrame(object->sprite, 0);
}

void ov45_0223093C(WiFiPlazaPlayerObject *object, u32 effect) {
    object->effectActive = 1;
    object->effect = effect;
    object->timer = 0;
    object->step = 1;
}

void ov45_02230968(WiFiPlazaPlayerObject *object) {
    object->effectActive = 0;
}

void ov45_02230974(WiFiPlazaPlayerObject *object, u8 step) {
    object->step = step;
}

void ov45_02230978(WiFiPlazaPlayerObject *object, int visible) {
    object->visible = visible;
    ov45_02230E28(object);
}

u32 ov45_02230994(WiFiPlazaPlayerObject *object) {
    return object->visible;
}

const WiFiPlazaSpriteConfig *ov45_0223099C(int spriteId) {
    int i;
    for (i = 0; i < 20; i++) {
        if (spriteId == ov45_02254C48[i].spriteId) {
            return &ov45_02254C48[i];
        }
    }
    GF_ASSERT(FALSE);
    return NULL;
}

// Sequence of a sprite: the walking one (+4) or the standing one.
u32 ov45_022309C4(int standing, u32 sequence) {
    return standing == 0 ? sequence + 4 : sequence;
}

u32 ov45_022309D0(u32 divisor, u16 value, u32 factor) {
    return ((u16)(value + 1) * factor / divisor) << FX32_SHIFT;
}

void ov45_022309E8(WiFiPlazaObjectModel *model, NARC *narc, int fileId, enum HeapID heapID) {
    NNSG3dResFileHeader *file;

    WiFiPlazaMap_LoadModelFile(&file, narc, fileId, heapID);
    model->resource.file = file;
    model->resource.set = NNS_G3dGetMdlSet(model->resource.file);
    model->resource.model = NNS_G3dGetMdlByIdx(model->resource.set, 0);
    model->resource.texture = NNS_G3dGetTex(model->resource.file);
    GF3dRender_BindModelSet(model->resource.file, model->resource.texture);
    NNS_G3dMdlSetMdlPolygonIDAll(model->resource.model, 20);
}

void ov45_02230A44(WiFiPlazaObjectModel *model) {
    ModelResource_Free(&model->resource);
}

void ov45_02230A4C(WiFiPlazaObjectModel *model, int alpha) {
    model->alpha = alpha;
    NNS_G3dMdlSetMdlAlphaAll(model->resource.model, alpha);
}

int ov45_02230A58(WiFiPlazaObjectModel *model) {
    return model->alpha;
}

void ov45_02230A5C(ModelResource *resource, ModelInstance *instance) {
    ModelInstance_Init(instance, resource);
}

WiFiPlazaPlayerObject *ov45_02230A6C(WiFiPlazaPlayerObjects *objects) {
    u32 i;

    for (i = 0; i < objects->count; i++) {
        if (!ov45_02230AA4(&objects->objects[i])) {
            return &objects->objects[i];
        }
    }
    GF_ASSERT(FALSE);
    return NULL;
}

BOOL ov45_02230AA4(WiFiPlazaPlayerObject *object) {
    if (object->player != NULL) {
        return TRUE;
    }
    return FALSE;
}

void ov45_02230AB4(WiFiPlazaPlayerObject *object) {
    memset(object, 0, sizeof(*object));
}

void ov45_02230AC0(WiFiPlazaPlayerObject *object) {
    ModelInstance_Draw(&object->model);
}

#pragma force_active on

// Follows the player's action (ov42 field 5) and its frame (field 8) and moves the sprite to the player's position.
void ov45_02230ACC(WiFiPlazaPlayerObject *object) {
    int action;
    u32 actionFrame;
    UnkStruct_ov44_02232914 gridPos;
    if (object->mode) {
        action = ov42_02228188(object->player, 5);
        actionFrame = ov42_02228188(object->player, 8);
        if (object->actionFrame > actionFrame || object->action != action) {
            if (ov45_02230DC4(object->action) == TRUE) {
                object->savedSequence = BillboardObject_GetSequence(object->sprite);
                object->savedFrame = BillboardObject_GetSequenceFrame(object->sprite);
            }
            object->action = action;
        }
        {
            void (*func)(WiFiPlazaPlayerObject *) = ov45_02254F28[action];
            object->actionFrame = actionFrame;
            func(object);
        }
        gridPos = ov42_022282F4(object->player);
        ov45_022308C0(object, &gridPos);
    }
}

#pragma force_active reset

void ov45_02230B64(WiFiPlazaPlayerObject *object) {
    u32 index = ov42_02228188(object->player, 6);
    u32 sequence = ov45_022309C4(1, index);
    BillboardObject_SetSequence(object->sprite, sequence);
    BillboardObject_SetSequenceFrame(object->sprite, 0);
}

void ov45_02230B8C(WiFiPlazaPlayerObject *object) {
    u32 index = ov42_02228188(object->player, 6);
    u32 sequence = ov45_022309C4(1, index);
    u16 duration = ov42_02228188(object->player, 9);
    u16 frame = ov42_02228188(object->player, 8);
    s32 offset = ov45_022309D0(duration, frame, 8);
    BillboardObject_SetSequence(object->sprite, sequence);
    if (object->savedSequence == sequence) {
        BillboardObject_SetSequenceFrame(object->sprite, 0);
        BillboardObject_AdvanceFrame(object->sprite, offset + object->savedFrame);
    } else {
        BillboardObject_SetSequenceFrame(object->sprite, 0);
        BillboardObject_AdvanceFrame(object->sprite, offset);
    }
}

void ov45_02230BFC(WiFiPlazaPlayerObject *object) {
    u16 frame = ov42_02228188(object->player, 8);
    if (frame < 4) {
        BillboardObject_SetSequenceFrame(object->sprite, FX32_ONE * 4);
        return;
    }
    BillboardObject_SetSequence(object->sprite, ov45_022309C4(1, ov42_02228188(object->player, 6)));
    BillboardObject_SetSequenceFrame(object->sprite, 0);
}

void ov45_02230C40(WiFiPlazaPlayerObject *object) {
    u32 index = ov42_02228188(object->player, 6);
    u32 sequence = ov45_022309C4(0, index);
    u16 duration = ov42_02228188(object->player, 9);
    u16 frame = ov42_02228188(object->player, 8);
    s32 offset = ov45_022309D0(duration, frame, 4);
    BillboardObject_SetSequence(object->sprite, sequence);
    if (object->savedSequence == sequence) {
        BillboardObject_SetSequenceFrame(object->sprite, 0);
        BillboardObject_AdvanceFrame(object->sprite, offset + object->savedFrame);
    } else {
        BillboardObject_SetSequenceFrame(object->sprite, 0);
        BillboardObject_AdvanceFrame(object->sprite, offset);
    }
}

void ov45_02230CB0(WiFiPlazaPlayerObject *object) {
    if (object->mode == 0 && object->effectActive == 1) {
        ov45_02254F1C[object->effect](object);
    }
}

#pragma force_active on

// Effect 1 (ov45_02254F1C): turns around, one direction every 4 ticks.
void ov45_02230CD8(WiFiPlazaPlayerObject *object) {
    if (((int)object->timer) % 4 == 0) {
        BillboardObject_SetSequence(object->sprite, ov45_022309C4(1, ov45_02254C34[(u32)object->timer >> 2]));
        BillboardObject_SetSequenceFrame(object->sprite, 0);
    }
    if (object->timer + object->step < 16) {
        object->timer += object->step;
    } else {
        object->timer = 0;
    }
}

#pragma force_active reset

void ov45_02230D20(WiFiPlazaPlayerObject *object) {
    if (object->timer == 0) {
        ov42_02228188(object->player, 6);
        BillboardObject_SetSequence(object->sprite, ov45_022309C4(1, ov42_02228188(object->player, 6)));
        BillboardObject_SetSequenceFrame(object->sprite, FX32_ONE * 4);
        object->timer++;
    }
}

void ov45_02230D5C(WiFiPlazaPlayerObject *object) {
    if (object->timer == 0) {
        BillboardObject_SetSequence(object->sprite, ov45_022309C4(1, ov42_02228188(object->player, 6)));
        BillboardObject_SetSequenceFrame(object->sprite, FX32_ONE * 4);
    } else if (object->timer == 4) {
        BillboardObject_SetSequence(object->sprite, ov45_022309C4(1, ov42_02228188(object->player, 6)));
        BillboardObject_SetSequenceFrame(object->sprite, FX32_ONE * 12);
    }
    object->timer = (object->timer + 1) % 8;
}

BOOL ov45_02230DC4(u32 action) {
    switch (action) {
    case 2:
    case 3:
    case 5:
    case 6:
    case 10:
    case 11:
        return TRUE;
    default:
        return FALSE;
    }
}

void ov45_02230DF4(WiFiPlazaPlayerObject *object) {
    if (ov45_02230AA4(object)) {
        if (ov45_02230E78(object->sprite) == FALSE) {
            object->culled = 1;
        } else {
            object->culled = 0;
        }
        ov45_02230E28(object);
    }
}

void ov45_02230E28(WiFiPlazaPlayerObject *object) {
    if (object->culled == 0 && object->visible == 1) {
        BillboardObject_SetVisible(object->sprite, TRUE);
        ModelInstance_SetVisible(&object->model, TRUE);
    } else {
        BillboardObject_SetVisible(object->sprite, FALSE);
        ModelInstance_SetVisible(&object->model, FALSE);
    }
}

void ov45_02230E64(void *sprite, WiFiPlazaPlayerObject *object) {
    void *model = BillboardObject_GetModelNoCast(sprite);
    NNS_G3dMdlSetMdlLightEnableFlagAll(model, object->lightFlags);
}
