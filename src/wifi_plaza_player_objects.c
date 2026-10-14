#include "global.h"

#include "wifi_plaza_internal.h"

// Wi-Fi Plaza player objects: one billboard sprite (field character model) plus a model under it for every player
// on the plaza map (overlay 49 drives them). The first functions are the last lobby client helpers.

void WiFiPlazaLobbyClient_OnMatchDisconnect(int result, void *arg) {
    WiFiPlazaLobbyClient *client = arg;
    if (client->unk_69e == 1) {
        WiFiPlazaLobbyClient_StopMatch();
    } else if (result == 0) {
        gWiFiPlazaLobbyClient->unk_69f = 1;
    }
}

u32 WiFiPlazaLobbySchedule_FindValue(WiFiPlazaLobbySchedule *unk, int key) {
    u32 result = 0;
    int i;
    for (i = 0; i < unk->count; i++) {
        if (key == unk->values[i].key) {
            result = unk->values[i].value;
        }
    }
    return result;
}

// The list header overlays the first 8 bytes of WiFiPlazaLobbyClient.surveyData.
void WiFiPlazaLobbyIdValueList_Init(WiFiPlazaLobbySurveyData *unk, int count, enum HeapID heapID) {
    WiFiPlazaLobbyIdValueList *list = (WiFiPlazaLobbyIdValueList *)unk;
    list->items = Heap_Alloc(heapID, count * sizeof(WiFiPlazaLobbyIdValue));
    list->capacity = count;
    list->count = 0;
}

void WiFiPlazaLobbyIdValueList_Free(WiFiPlazaLobbySurveyData *unk) {
    WiFiPlazaLobbyIdValueList *list = (WiFiPlazaLobbyIdValueList *)unk;
    Heap_Free(list->items);
    list->items = NULL;
}

void WiFiPlazaLobbyIdValueList_Set(WiFiPlazaLobbySurveyData *unk, const void *data, u32 count) {
    WiFiPlazaLobbyIdValueList *list = (WiFiPlazaLobbyIdValueList *)unk;
    GF_ASSERT(count < list->capacity);
    if (count >= list->capacity) {
        count = list->capacity;
    }
    MIi_CpuCopy32(data, (u32 *)list->items, count * sizeof(WiFiPlazaLobbyIdValue));
    list->count = count;
}

BOOL WiFiPlazaLobbyIdValueList_Contains(WiFiPlazaLobbySurveyData *unk, int id) {
    WiFiPlazaLobbyIdValueList *list = (WiFiPlazaLobbyIdValueList *)unk;
    int i;
    for (i = 0; i < list->count; i++) {
        if (id == list->items[i].id) {
            return TRUE;
        }
    }
    return FALSE;
}

u32 WiFiPlazaLobbyIdValueList_GetValue(WiFiPlazaLobbySurveyData *unk, int id) {
    WiFiPlazaLobbyIdValueList *list = (WiFiPlazaLobbyIdValueList *)unk;
    int i;
    for (i = 0; i < list->count; i++) {
        if (id == list->items[i].id) {
            return list->items[i].value;
        }
    }
    return 0;
}

void WiFiPlazaLobbyClient_CopySurveyData(u32 *dest, const void *data) {
    memcpy(dest, data, 0x2D8);
}

WiFiPlazaPlayerObjects *WiFiPlazaPlayerObjects_New(u32 count, BOOL mode, enum HeapID heapID, enum HeapID loadHeapID) {
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
        WiFiPlazaPlayerObject_Clear(&objects->objects[i]);
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
    WiFiPlaza_DisableModelLighting(file);
    objects->lightFlags = NNS_G3dMdlGetMdlLightEnableFlag(NNS_G3dGetMdlByIdx(NNS_G3dGetMdlSet(file), 0), 0);

    for (i = 0; i < 2; i++) {
        file = GfGfxLoader_LoadFromOpenNarc(modelNarc, sWiFiPlazaSpriteSequenceFiles[i], FALSE, loadHeapID, FALSE);
        GF2dGfxRawResMan_AllocObj(objects->sequences, file, sWiFiPlazaSpriteSequenceFiles[i]);
    }

    if (mode == FALSE) {
        skipped = 1;
    } else {
        skipped = 0;
    }
    {
        const WiFiPlazaSpriteConfig *config = sWiFiPlazaSpriteConfigs;
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

    WiFiPlazaObjectModel_Load(&objects->model, narc, 0x80, loadHeapID);
    NARC_Delete(modelNarc);
    NARC_Delete(narc);
    return objects;
}

void WiFiPlazaPlayerObjects_Delete(WiFiPlazaPlayerObjects *objects) {
    WiFiPlazaObjectModel_Free(&objects->model);
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

void WiFiPlazaPlayerObjects_Update(WiFiPlazaPlayerObjects *objects) {
    u32 i;
    for (i = 0; i < objects->count; i++) {
        WiFiPlazaPlayerObject_FollowPlayer(&objects->objects[i]);
        WiFiPlazaPlayerObject_UpdateEffect(&objects->objects[i]);
        WiFiPlazaPlayerObject_UpdateCulling(&objects->objects[i]);
    }
}

void WiFiPlazaPlayerObjects_Draw(WiFiPlazaPlayerObjects *objects) {
    u32 i;
    BillboardLists_Draw();
    if (WiFiPlazaObjectModel_GetAlpha(&objects->model)) {
        for (i = 0; i < objects->count; i++) {
            if (WiFiPlazaPlayerObject_IsUsed(&objects->objects[i]) == TRUE) {
                WiFiPlazaPlayerObject_DrawModel(&objects->objects[i]);
            }
        }
    }
}

void WiFiPlazaPlayerObjects_ClearPendingFree(WiFiPlazaPlayerObjects *objects) {
    Billboard_ClearPendingFree(objects->billboards);
}

void WiFiPlazaPlayerObjects_SetModelAlpha(WiFiPlazaPlayerObjects *objects, int alpha) {
    WiFiPlazaObjectModel_SetAlpha(&objects->model, alpha);
}

WiFiPlazaPlayerObject *WiFiPlazaPlayerObjects_Add(WiFiPlazaPlayerObjects *objects, Field2DObject *player) {
    WiFiPlazaBillboardResource resource;
    WiFiPlazaBillboardCreateParams create;
    WiFiPlazaBillboardSequence sequence;
    Field2DPoint gridPos;
    const WiFiPlazaSpriteConfig *config;
    GF_3DGfxRawResObj *texture;
    void *modelFile;
    NNSG3dResTex *tex;
    NNSG3dTexKey tex4x4Key;
    NNSG3dTexKey texKey;

    WiFiPlazaPlayerObject *object = WiFiPlazaPlayerObjects_GetFreeObject(objects);
    object->player = player;
    VecFx32 position = { 0, 0, 0 };
    VecFx32 scale = sWiFiPlazaPlayerSpriteScale;
    config = WiFiPlaza_GetSpriteConfig(Field2DObject_GetParam(player, 7));
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
        BillboardResource_Init(&resource, modelFile, tex, &sWiFiPlazaBillboardRanges, &sequence);
    } else {
        texKey = GF3dGfxRawResObj_GetTexKey(texture);
        tex4x4Key = GF3dGfxRawResObj_GetTex4x4Key(texture);
        BillboardResource_InitWithKeys(&resource, modelFile, tex, &sWiFiPlazaBillboardRanges, &sequence, texKey, tex4x4Key, GF3dGfxRawResObj_GetPlttKey(texture));
    }
    gridPos = Field2DObject_GetPos(player);
    WiFiPlazaMap_PosToVecFx32(&gridPos.unk0, &position.x);
    position.z += FX32_ONE * 16;
    create.billboards = objects->billboards;
    create.resource = &resource;
    create.position = position;
    create.scale = scale;
    object->sprite = BillboardObject_Create(&create);
    BillboardObject_SetCallback(object->sprite, WiFiPlazaPlayerObject_ApplyLightFlags, object);
    WiFiPlaza_InitModelInstance(&objects->model.resource, &object->model);
    ModelInstance_SetPosition(&object->model, position.x, FX32_ONE * 2, position.z - FX32_ONE * 8);
    object->mode = 1;
    object->visible = 1;
    object->culled = 0;
    object->lightFlags = objects->lightFlags;
    return object;
}

void WiFiPlazaPlayerObject_Delete(WiFiPlazaPlayerObject *object) {
    BillboardObject_Delete(object->sprite);
    memset(object, 0, sizeof(WiFiPlazaPlayerObject));
}

void WiFiPlazaPlayerObject_SetMode(WiFiPlazaPlayerObject *object, u32 mode) {
    object->mode = mode;
}

void WiFiPlazaPlayerObject_SetLightFlags(WiFiPlazaPlayerObject *object, u32 lightFlags) {
    object->lightFlags = lightFlags;
}

u32 WiFiPlazaPlayerObject_IsCulled(WiFiPlazaPlayerObject *object) {
    return object->culled;
}

void WiFiPlazaPlayerObject_SetGridPosition(WiFiPlazaPlayerObject *object, const Field2DPoint *gridPos) {
    VecFx32 position;
    WiFiPlazaMap_PosToVecFx32(&gridPos->unk0, &position.x);
    position.z += FX32_ONE * 16;
    WiFiPlazaPlayerObject_SetPosition(object, &position);
}

void WiFiPlazaPlayerObject_SetPosition(WiFiPlazaPlayerObject *object, const VecFx32 *position) {
    BillboardObject_SetPosition(object->sprite, position);
    ModelInstance_SetPosition(&object->model, position->x, FX32_ONE * 2, position->z - FX32_ONE * 8);
}

void WiFiPlazaPlayerObject_GetPosition(WiFiPlazaPlayerObject *object, VecFx32 *position) {
    *position = *BillboardObject_GetPosition(object->sprite);
}

void WiFiPlazaPlayerObject_SetStandingSequence(WiFiPlazaPlayerObject *object, u32 sequence) {
    BillboardObject_SetSequence(object->sprite, WiFiPlaza_GetSpriteSequence(1, sequence));
    BillboardObject_SetSequenceFrame(object->sprite, 0);
}

void WiFiPlazaPlayerObject_StartEffect(WiFiPlazaPlayerObject *object, u32 effect) {
    object->effectActive = 1;
    object->effect = effect;
    object->timer = 0;
    object->step = 1;
}

void WiFiPlazaPlayerObject_StopEffect(WiFiPlazaPlayerObject *object) {
    object->effectActive = 0;
}

void WiFiPlazaPlayerObject_SetEffectStep(WiFiPlazaPlayerObject *object, u8 step) {
    object->step = step;
}

void WiFiPlazaPlayerObject_SetVisible(WiFiPlazaPlayerObject *object, int visible) {
    object->visible = visible;
    WiFiPlazaPlayerObject_ApplyVisibility(object);
}

u32 WiFiPlazaPlayerObject_IsVisible(WiFiPlazaPlayerObject *object) {
    return object->visible;
}

const WiFiPlazaSpriteConfig *WiFiPlaza_GetSpriteConfig(int spriteId) {
    int i;
    for (i = 0; i < 20; i++) {
        if (spriteId == sWiFiPlazaSpriteConfigs[i].spriteId) {
            return &sWiFiPlazaSpriteConfigs[i];
        }
    }
    GF_ASSERT(FALSE);
    return NULL;
}

// Sequence of a sprite: the walking one (+4) or the standing one.
u32 WiFiPlaza_GetSpriteSequence(int standing, u32 sequence) {
    return standing == 0 ? sequence + 4 : sequence;
}

u32 WiFiPlaza_CalcSequenceFrame(u32 divisor, u16 value, u32 factor) {
    return ((u16)(value + 1) * factor / divisor) << FX32_SHIFT;
}

void WiFiPlazaObjectModel_Load(WiFiPlazaObjectModel *model, NARC *narc, int fileId, enum HeapID heapID) {
    NNSG3dResFileHeader *file;

    WiFiPlazaMap_LoadModelFile(&file, narc, fileId, heapID);
    model->resource.file = file;
    model->resource.set = NNS_G3dGetMdlSet(model->resource.file);
    model->resource.model = NNS_G3dGetMdlByIdx(model->resource.set, 0);
    model->resource.texture = NNS_G3dGetTex(model->resource.file);
    GF3dRender_BindModelSet(model->resource.file, model->resource.texture);
    NNS_G3dMdlSetMdlPolygonIDAll(model->resource.model, 20);
}

void WiFiPlazaObjectModel_Free(WiFiPlazaObjectModel *model) {
    ModelResource_Free(&model->resource);
}

void WiFiPlazaObjectModel_SetAlpha(WiFiPlazaObjectModel *model, int alpha) {
    model->alpha = alpha;
    NNS_G3dMdlSetMdlAlphaAll(model->resource.model, alpha);
}

int WiFiPlazaObjectModel_GetAlpha(WiFiPlazaObjectModel *model) {
    return model->alpha;
}

void WiFiPlaza_InitModelInstance(ModelResource *resource, ModelInstance *instance) {
    ModelInstance_Init(instance, resource);
}

WiFiPlazaPlayerObject *WiFiPlazaPlayerObjects_GetFreeObject(WiFiPlazaPlayerObjects *objects) {
    u32 i;

    for (i = 0; i < objects->count; i++) {
        if (!WiFiPlazaPlayerObject_IsUsed(&objects->objects[i])) {
            return &objects->objects[i];
        }
    }
    GF_ASSERT(FALSE);
    return NULL;
}

BOOL WiFiPlazaPlayerObject_IsUsed(WiFiPlazaPlayerObject *object) {
    if (object->player != NULL) {
        return TRUE;
    }
    return FALSE;
}

void WiFiPlazaPlayerObject_Clear(WiFiPlazaPlayerObject *object) {
    memset(object, 0, sizeof(*object));
}

void WiFiPlazaPlayerObject_DrawModel(WiFiPlazaPlayerObject *object) {
    ModelInstance_Draw(&object->model);
}

#pragma force_active on

// Follows the player's action (ov42 field 5) and its frame (field 8) and moves the sprite to the player's position.
void WiFiPlazaPlayerObject_FollowPlayer(WiFiPlazaPlayerObject *object) {
    int action;
    u32 actionFrame;
    Field2DPoint gridPos;
    if (object->mode) {
        action = Field2DObject_GetParam(object->player, 5);
        actionFrame = Field2DObject_GetParam(object->player, 8);
        if (object->actionFrame > actionFrame || object->action != action) {
            if (WiFiPlazaPlayerObject_ActionKeepsSequence(object->action) == TRUE) {
                object->savedSequence = BillboardObject_GetSequence(object->sprite);
                object->savedFrame = BillboardObject_GetSequenceFrame(object->sprite);
            }
            object->action = action;
        }
        {
            void (*func)(WiFiPlazaPlayerObject *) = sWiFiPlazaPlayerObjectActions[action];
            object->actionFrame = actionFrame;
            func(object);
        }
        gridPos = Field2DObject_GetDrawPos(object->player);
        WiFiPlazaPlayerObject_SetGridPosition(object, &gridPos);
    }
}

#pragma force_active reset

void WiFiPlazaPlayerObject_ShowStandingSprite(WiFiPlazaPlayerObject *object) {
    u32 index = Field2DObject_GetParam(object->player, 6);
    u32 sequence = WiFiPlaza_GetSpriteSequence(1, index);
    BillboardObject_SetSequence(object->sprite, sequence);
    BillboardObject_SetSequenceFrame(object->sprite, 0);
}

void WiFiPlazaPlayerObject_PlayStandingSequence(WiFiPlazaPlayerObject *object) {
    u32 index = Field2DObject_GetParam(object->player, 6);
    u32 sequence = WiFiPlaza_GetSpriteSequence(1, index);
    u16 duration = Field2DObject_GetParam(object->player, 9);
    u16 frame = Field2DObject_GetParam(object->player, 8);
    s32 offset = WiFiPlaza_CalcSequenceFrame(duration, frame, 8);
    BillboardObject_SetSequence(object->sprite, sequence);
    if (object->savedSequence == sequence) {
        BillboardObject_SetSequenceFrame(object->sprite, 0);
        BillboardObject_AdvanceFrame(object->sprite, offset + object->savedFrame);
    } else {
        BillboardObject_SetSequenceFrame(object->sprite, 0);
        BillboardObject_AdvanceFrame(object->sprite, offset);
    }
}

void WiFiPlazaPlayerObject_HoldWalkFrameThenRestart(WiFiPlazaPlayerObject *object) {
    u16 frame = Field2DObject_GetParam(object->player, 8);
    if (frame < 4) {
        BillboardObject_SetSequenceFrame(object->sprite, FX32_ONE * 4);
        return;
    }
    BillboardObject_SetSequence(object->sprite, WiFiPlaza_GetSpriteSequence(1, Field2DObject_GetParam(object->player, 6)));
    BillboardObject_SetSequenceFrame(object->sprite, 0);
}

void WiFiPlazaPlayerObject_PlayWalkingSequence(WiFiPlazaPlayerObject *object) {
    u32 index = Field2DObject_GetParam(object->player, 6);
    u32 sequence = WiFiPlaza_GetSpriteSequence(0, index);
    u16 duration = Field2DObject_GetParam(object->player, 9);
    u16 frame = Field2DObject_GetParam(object->player, 8);
    s32 offset = WiFiPlaza_CalcSequenceFrame(duration, frame, 4);
    BillboardObject_SetSequence(object->sprite, sequence);
    if (object->savedSequence == sequence) {
        BillboardObject_SetSequenceFrame(object->sprite, 0);
        BillboardObject_AdvanceFrame(object->sprite, offset + object->savedFrame);
    } else {
        BillboardObject_SetSequenceFrame(object->sprite, 0);
        BillboardObject_AdvanceFrame(object->sprite, offset);
    }
}

void WiFiPlazaPlayerObject_UpdateEffect(WiFiPlazaPlayerObject *object) {
    if (object->mode == 0 && object->effectActive == 1) {
        sWiFiPlazaPlayerObjectEffects[object->effect](object);
    }
}

#pragma force_active on

// Effect 1 (sWiFiPlazaPlayerObjectEffects): turns around, one direction every 4 ticks.
void WiFiPlazaPlayerObject_EffectTurnAround(WiFiPlazaPlayerObject *object) {
    if (((int)object->timer) % 4 == 0) {
        BillboardObject_SetSequence(object->sprite, WiFiPlaza_GetSpriteSequence(1, sWiFiPlazaTurnAroundDirections[(u32)object->timer >> 2]));
        BillboardObject_SetSequenceFrame(object->sprite, 0);
    }
    if (object->timer + object->step < 16) {
        object->timer += object->step;
    } else {
        object->timer = 0;
    }
}

#pragma force_active reset

void WiFiPlazaPlayerObject_EffectSetWalkFrameOnce(WiFiPlazaPlayerObject *object) {
    if (object->timer == 0) {
        Field2DObject_GetParam(object->player, 6);
        BillboardObject_SetSequence(object->sprite, WiFiPlaza_GetSpriteSequence(1, Field2DObject_GetParam(object->player, 6)));
        BillboardObject_SetSequenceFrame(object->sprite, FX32_ONE * 4);
        object->timer++;
    }
}

void WiFiPlazaPlayerObject_EffectWalkInPlace(WiFiPlazaPlayerObject *object) {
    if (object->timer == 0) {
        BillboardObject_SetSequence(object->sprite, WiFiPlaza_GetSpriteSequence(1, Field2DObject_GetParam(object->player, 6)));
        BillboardObject_SetSequenceFrame(object->sprite, FX32_ONE * 4);
    } else if (object->timer == 4) {
        BillboardObject_SetSequence(object->sprite, WiFiPlaza_GetSpriteSequence(1, Field2DObject_GetParam(object->player, 6)));
        BillboardObject_SetSequenceFrame(object->sprite, FX32_ONE * 12);
    }
    object->timer = (object->timer + 1) % 8;
}

BOOL WiFiPlazaPlayerObject_ActionKeepsSequence(u32 action) {
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

void WiFiPlazaPlayerObject_UpdateCulling(WiFiPlazaPlayerObject *object) {
    if (WiFiPlazaPlayerObject_IsUsed(object)) {
        if (WiFiPlaza_IsSpriteInView(object->sprite) == FALSE) {
            object->culled = 1;
        } else {
            object->culled = 0;
        }
        WiFiPlazaPlayerObject_ApplyVisibility(object);
    }
}

void WiFiPlazaPlayerObject_ApplyVisibility(WiFiPlazaPlayerObject *object) {
    if (object->culled == 0 && object->visible == 1) {
        BillboardObject_SetVisible(object->sprite, TRUE);
        ModelInstance_SetVisible(&object->model, TRUE);
    } else {
        BillboardObject_SetVisible(object->sprite, FALSE);
        ModelInstance_SetVisible(&object->model, FALSE);
    }
}

void WiFiPlazaPlayerObject_ApplyLightFlags(void *sprite, WiFiPlazaPlayerObject *object) {
    void *model = BillboardObject_GetModelNoCast(sprite);
    NNS_G3dMdlSetMdlLightEnableFlagAll(model, object->lightFlags);
}
