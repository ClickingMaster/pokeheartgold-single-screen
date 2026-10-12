#include "global.h"

#include "pokeathlon/pokeathlon_course_internal.h"

#include "filesystem.h"
#include "follow_mon.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "oam_manager.h"
#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "sprite_transfer.h"
#include "sys_task_api.h"
#include "char_tiles.h"

void PokeathlonCourseMonGfx_InitManager(PokeathlonCourseMonGfxManager *manager, u32 count, SpriteList *spriteList) {
    int i;
    if (count > 16) {
        GF_AssertFail();
    }
    manager->count = count;
    manager->loadedCount = 0;
    manager->loaded = FALSE;
    manager->shiftLargeUp = FALSE;
    manager->mons = Heap_Alloc(manager->heapId, count * sizeof(PokeathlonCourseMonGfx));
    if (spriteList == NULL) {
        manager->ownSpriteList = G2dRenderer_Init(manager->count, &manager->renderer, manager->heapId);
        manager->spriteList = manager->ownSpriteList;
        manager->ownsSpriteList = TRUE;
    } else {
        manager->spriteList = spriteList;
        manager->ownsSpriteList = FALSE;
    }
    for (i = 0; i < 4; i++) {
        manager->resMans[i] = Create2DGfxResObjMan(manager->count, (GfGfxResType)i, manager->heapId);
    }
}

void PokeathlonCourseMonGfx_FreeManager(PokeathlonCourseMonGfxManager *manager) {
    u8 i;
    for (i = 0; i < manager->count; i++) {
        SpriteTransfer_DeleteCharTransferTask(manager->mons[i].charRes);
        SpriteTransfer_DeletePlttTransferTask(manager->mons[i].plttRes);
    }
    for (i = 0; i < 4; i++) {
        Destroy2DGfxResObjMan(manager->resMans[i]);
    }
    Heap_Free(manager->mons);
    if (manager->ownsSpriteList != FALSE) {
        SpriteList_Delete(manager->ownSpriteList);
    }
    Heap_Free(manager);
}

// Starts a task that loads count Pokemon sprites (infos[baseIndex..]) into the manager, one per frame.
void PokeathlonCourseMonGfx_StartLoad(PokeathlonCourseMonGfxManager *manager, s32 count, PokeathlonCourseMonInfo *infos, PokeathlonCourseMonGfxTemplate *template, u32 priority, int baseIndex) {
    int i;
    PokeathlonCourseMonGfxLoad *load = Heap_AllocAtEnd(manager->heapId, sizeof(PokeathlonCourseMonGfxLoad));
    load->count = count;
    load->manager = manager;
    load->template = *template;
    load->state = 0;
    load->baseIndex = baseIndex;
    load->current = 0;
    manager->loaded = FALSE;
    for (i = 0; i < count; i++) {
        load->infos[i] = infos[i];
    }
    SysTask_CreateOnMainQueue(PokeathlonCourseMonGfx_LoadTask, load, priority);
}

BOOL PokeathlonCourseMonGfx_IsLoaded(PokeathlonCourseMonGfxManager *manager) {
    return manager->loaded;
}

#pragma force_active on

PokeathlonCourseMonGfx *PokeathlonCourseMonGfx_Get(PokeathlonCourseMonGfxManager *manager, int index) {
    if (index >= manager->count) {
        GF_AssertFail();
    }
    return &manager->mons[index];
}

#pragma force_active reset

Sprite *PokeathlonCourseMonGfx_GetSprite(PokeathlonCourseMonGfx *mon) {
    return mon->sprite;
}

PokeathlonCourseMonInfo *PokeathlonCourseMonGfx_GetInfo(PokeathlonCourseMonGfx *mon) {
    return &mon->info;
}

// Copies the Pokemon's (shiny) palette out of its overworld model.
void PokeathlonCourseMonGfx_LoadPalette(PokeathlonCourseMonInfo *info, enum HeapID heapId, void *dest) {
    void *model;
    u8 *palette;

    model = AllocAndReadWholeNarcMemberByIdPair(NARC_data_mmodel_mmodel, PokeathlonCourse_GetMonModelFileId(info->species, info->form, info->gender), heapId);
    palette = (u8 *)NNS_G3dGetTex(model);
    palette += ((NNSG3dResTex *)palette)->plttInfo.ofsPlttData;
    if (info->isShiny != 0) {
        palette += 0x20;
    }
    MI_CpuCopy8(palette, dest, 0x20);
    Heap_Free(model);
}

void PokeathlonCourseMonGfx_SetPalette(PokeathlonCourseMonGfxManager *manager, int index, int vram, void *palette) {
    BOOL loadMain = FALSE;
    BOOL loadSub = FALSE;
    NNSG2dImagePaletteProxy *paletteProxy;

    if (vram == 3) {
        loadMain = TRUE;
        loadSub = TRUE;
    } else if (vram == 1) {
        loadMain = TRUE;
    } else {
        loadSub = TRUE;
    }
    paletteProxy = SpriteTransfer_GetPaletteProxy(manager->mons[index].plttRes, SpriteTransfer_GetCharProxy(manager->mons[index].charRes));
    DC_FlushRange(palette, 0x20);
    if (loadMain) {
        GX_LoadOBJPltt(palette, NNS_G2dGetImagePaletteLocation(paletteProxy, NNS_G2D_VRAM_TYPE_2DMAIN), 0x20);
    }
    if (loadSub) {
        GXS_LoadOBJPltt(palette, NNS_G2dGetImagePaletteLocation(paletteProxy, NNS_G2D_VRAM_TYPE_2DSUB), 0x20);
    }
}

#pragma force_active on

// Adds the sprite resources of one Pokemon (cell / animation files of NARC a/1/4/9 by size and transfer mode)
// and converts the 8 frames of its overworld model texture into sprite characters.
void PokeathlonCourseMonGfx_Load(PokeathlonCourseMonGfxManager *manager, PokeathlonCourseMonGfx *mon, PokeathlonCourseMonInfo *info, int vram, u8 id, BOOL vramTransfer) {
    int cellFile;
    int animFile;
    int dim;
    int i;
    void *model;
    u8 *texData;
    NNSG3dResTex *tex;
    NNSG2dImagePaletteProxy *plttProxy;
    NNSG2dImageProxy *charProxy;
    NNSG2dCharacterData *charData;
    BOOL loadMain;
    BOOL loadSub;
    u32 size;
    int charFile;
    u8 *pltt;
    int j;
    u32 offset;
    if (vramTransfer) {
        if (info->isLarge) {
            cellFile = 10;
            charFile = 11;
            animFile = 9;
        } else {
            cellFile = 7;
            charFile = 8;
            animFile = 6;
        }
    } else {
        if (info->isLarge) {
            cellFile = 4;
            charFile = 5;
            animFile = 3;
        } else {
            cellFile = 1;
            charFile = 2;
            animFile = 0;
        }
    }
    mon->charRes = AddCharResObjFromNarc(manager->resMans[GF_GFX_RES_TYPE_CHAR], NARC_a_1_4_9, charFile, FALSE, id, vram, manager->heapId);
    mon->plttRes = AddPlttResObjFromNarc(manager->resMans[GF_GFX_RES_TYPE_PLTT], NARC_a_0_4_9, 30, FALSE, id, vram, 1, manager->heapId);
    mon->cellRes = AddCellOrAnimResObjFromNarc(manager->resMans[GF_GFX_RES_TYPE_CELL], NARC_a_1_4_9, cellFile, FALSE, id, GF_GFX_RES_TYPE_CELL, manager->heapId);
    mon->animRes = AddCellOrAnimResObjFromNarc(manager->resMans[GF_GFX_RES_TYPE_ANIM], NARC_a_1_4_9, animFile, FALSE, id, GF_GFX_RES_TYPE_ANIM, manager->heapId);
    SpriteTransfer_CreateCharTransferTask_AllocAtEnd(mon->charRes);
    SpriteTransfer_CreateExtPlttTransferTask(mon->plttRes);
    loadMain = FALSE;
    loadSub = FALSE;
    charProxy = SpriteTransfer_GetCharProxy(mon->charRes);
    plttProxy = SpriteTransfer_GetPaletteProxy(mon->plttRes, charProxy);
    charData = GF2DGfxResObj_GetCharDataPtr(mon->charRes);
    if (vram == 3) {
        loadMain = TRUE;
        loadSub = TRUE;
    } else if (vram == 1) {
        loadMain = TRUE;
    } else {
        loadSub = TRUE;
    }
    model = AllocAndReadWholeNarcMemberByIdPair(NARC_data_mmodel_mmodel, PokeathlonCourse_GetMonModelFileId(info->species, info->form, info->gender), manager->heapId);
    if (info->isLarge) {
        size = 0x800;
        dim = 8;
    } else {
        size = 0x200;
        dim = 4;
    }
    tex = NNS_G3dGetTex(model);
    texData = (u8 *)tex + tex->texInfo.ofsTex;
    for (i = 0, offset = 0; i < 8; i++) {
        sub_020145B4(texData + offset, dim, 0, 0, dim, dim, manager->buffer);
        if (!vramTransfer) {
            if (manager->shiftLargeUp && info->isLarge) {
                for (j = 0; j < 0x800; j++) {
                    if (j < 0x800 - dim * 32) {
                        manager->buffer[j] = manager->buffer[j + dim * 32];
                    } else {
                        manager->buffer[j] = 0;
                    }
                }
            }
            DC_FlushRange(manager->buffer, size);
            if (loadMain) {
                u32 location = NNS_G2dGetImageLocation(charProxy, NNS_G2D_VRAM_TYPE_2DMAIN);
                GX_LoadOBJ(manager->buffer, location + offset, size);
            }
            if (loadSub) {
                u32 location = NNS_G2dGetImageLocation(charProxy, NNS_G2D_VRAM_TYPE_2DSUB);
                GXS_LoadOBJ(manager->buffer, location + offset, size);
            }
        }
        MIi_CpuCopyFast((u32 *)manager->buffer, (u32 *)((u8 *)charData->pRawData + offset), size);
        offset += size;
    }
    pltt = (u8 *)tex + tex->plttInfo.ofsPlttData;
    if (info->isShiny) {
        pltt += 0x20;
    }
    DC_FlushRange(pltt, 0x20);
    if (loadMain) {
        GX_LoadOBJPltt(pltt, NNS_G2dGetImagePaletteLocation(plttProxy, NNS_G2D_VRAM_TYPE_2DMAIN), 0x20);
    }
    if (loadSub) {
        GXS_LoadOBJPltt(pltt, NNS_G2dGetImagePaletteLocation(plttProxy, NNS_G2D_VRAM_TYPE_2DSUB), 0x20);
    }
    Heap_Free(model);
}

#pragma force_active reset

void PokeathlonCourseMonGfx_CreateSprite(PokeathlonCourseMonGfxManager *manager, int priority, int vramTransfer, NNS_G2D_VRAM_TYPE whichScreen, u8 resourceId, u8 index) {
    SpriteResourcesHeader header;
    SpriteTemplate template;
    Sprite *sprite;
    int offset;

    CreateSpriteResourcesHeader(&header, resourceId, resourceId, resourceId, resourceId, -1, -1, vramTransfer, priority, manager->resMans[GF_GFX_RES_TYPE_CHAR], manager->resMans[GF_GFX_RES_TYPE_PLTT], manager->resMans[GF_GFX_RES_TYPE_CELL], manager->resMans[GF_GFX_RES_TYPE_ANIM], NULL, NULL);
    if (whichScreen == NNS_G2D_VRAM_TYPE_2DBOTH) {
        whichScreen = NNS_G2D_VRAM_TYPE_2DMAIN;
    }
    template.spriteList = manager->spriteList;
    template.header = &header;
    template.heapID = manager->heapId;
    template.position.x = 0;
    template.position.y = 0;
    template.position.z = 0;
    template.scale.x = FX32_ONE;
    template.scale.y = FX32_ONE;
    template.scale.z = FX32_ONE;
    template.rotation = 0;
    template.whichScreen = whichScreen;
    template.drawPriority = 0;
    // (byte offset of the Pokemon, computed before the sprite is created: needed to match)
    offset = index * sizeof(PokeathlonCourseMonGfx);
    sprite = Sprite_CreateAffine(&template);
    ((PokeathlonCourseMonGfx *)((u8 *)manager->mons + offset))->sprite = sprite;
    if (((PokeathlonCourseMonGfx *)((u8 *)manager->mons + offset))->sprite == NULL) {
        GF_AssertFail();
    }
    Sprite_SetAnimActiveFlag(((PokeathlonCourseMonGfx *)((u8 *)manager->mons + offset))->sprite, TRUE);
    Sprite_SetAnimCtrlSeq(((PokeathlonCourseMonGfx *)((u8 *)manager->mons + offset))->sprite, 0);
    Sprite_SetDrawFlag(((PokeathlonCourseMonGfx *)((u8 *)manager->mons + offset))->sprite, FALSE);
}

void PokeathlonCourseMonGfx_LoadTask(SysTask *task, void *data) {
    PokeathlonCourseMonGfxLoad *load = data;
    do {
        if (PokeathlonCourseMonGfx_LoadStep(load)) {
            load->manager->loaded = TRUE;
            Heap_Free(load);
            SysTask_Destroy(task);
            return;
        }
    } while (load->template.loadAtOnce);
}

#pragma force_active on

BOOL PokeathlonCourseMonGfx_LoadStep(PokeathlonCourseMonGfxLoad *load) {
    u8 index;
    PokeathlonCourseMonGfx *mon;
    switch (load->state) {
    case 0:
        index = (u8)(load->baseIndex + load->current);
        PokeathlonCourseMonGfx_Load(load->manager, &load->manager->mons[index], (PokeathlonCourseMonInfo *)((u32)load->infos + (u32)index * sizeof(PokeathlonCourseMonInfo)), load->template.vram, index, load->template.vramTransfer);
        load->manager->mons[index].info = load->infos[index];
        load->manager->mons[index].topRow = PokeathlonCourseMonGfx_FindTopRow(&load->manager->mons[index]);
        load->manager->mons[index].shiftedUp = load->manager->shiftLargeUp != FALSE && load->infos[index].isLarge != 0;
        if (load->manager->mons[index].shiftedUp) {
            load->manager->mons[index].topRow += 8;
        }
        if (load->template.vramTransfer == FALSE) {
            mon = &load->manager->mons[index];
            GF2DGfxResObj_FreeData(mon->charRes);
            GF2DGfxResObj_FreeData(mon->plttRes);
        }
        load->current++;
        if (load->current >= load->count) {
            load->current = 0;
            load->state = 1;
        }
        break;
    case 1:
        index = (u8)(load->baseIndex + load->current);
        PokeathlonCourseMonGfx_CreateSprite(load->manager, load->template.priority, load->template.vramTransfer, load->template.whichScreen, index, index);
        load->current++;
        load->manager->loadedCount++;
        if (load->current >= load->count) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

#pragma force_active reset

u8 PokeathlonCourseMonGfx_GetTopRow(PokeathlonCourseMonGfx *mon) {
    return mon->topRow;
}

#pragma force_active on

u8 PokeathlonCourseMonGfx_FindTopRow(PokeathlonCourseMonGfx *mon) {
    NNSG2dCharacterData *charData = GF2DGfxResObj_GetCharDataPtr(mon->charRes);
    int size = mon->info.isLarge ? 8 : 4;
    u8 result = 255;
    int y;
    int base;
    void *pixels = charData->pRawData;
    y = 0;
    if (size > 0) {
        base = 0;
        do {
            u8 min = 255;
            int x = 0;
            for (; x < size; x++) {
                u8 row = PokeathlonCourseMonGfx_FindTopRowInTile(pixels, (x + base) * 32);
                if (row != 255 && min > row) {
                    min = row;
                }
            }
            if (min != 255) {
                result = (u8)(min + y * 8);
                break;
            }
            base += size;
            y++;
        } while (y < size);
    }
    GF_ASSERT(result != 255);
    return result;
}

#pragma force_active reset

// Returns the first pixel row of the 8x8 tile at data + offset that is not transparent, 0xFF if none.
u8 PokeathlonCourseMonGfx_FindTopRowInTile(const u8 *data, u32 offset) {
    u8 row = 0;
    u8 result = 0xFF;

    data += offset;
    do {
        u8 column = 0;
        do {
            if (*data != 0) {
                result = row;
                break;
            }
            column++;
            data++;
        } while (column < 4);
        if (column != 4) {
            break;
        }
        row++;
    } while (row < 8);
    return result;
}

// File of the Pokemon's overworld model in NARC data/mmodel/mmodel.
int PokeathlonCourse_GetMonModelFileId(int species, int form, int gender) {
    int model;

    if (species <= 0 || species > NATIONAL_DEX_COUNT) {
        model = 1;
    } else {
        model = SpeciesToOverworldModelIndexOffset(species) + 297;
        if (OverworldModelLookupHasFemaleForm(species)) {
            if (gender == MON_FEMALE) {
                model++;
            }
        } else {
            if (form > OverworldModelLookupFormCount(species)) {
                form = 0;
            }
            model += form;
        }
    }
    return model;
}

void PokeathlonCourseMonGfx_SetScale(PokeathlonCourseMonGfx *mon, f32 x, f32 y) {
    Sprite *sprite;
    VecFx32 scale = { 0, 0, 0 };

    sprite = PokeathlonCourseMonGfx_GetSprite(mon);
    scale.x = (fx32)(x > 0.0f ? x * 4096.0f + 0.5f : x * 4096.0f - 0.5f);
    scale.y = (fx32)(y > 0.0f ? y * 4096.0f + 0.5f : y * 4096.0f - 0.5f);
    Sprite_SetAffineScale(sprite, &scale);
}

// Large Pokemon of the following loads are drawn one tile row higher; only before loading.
void PokeathlonCourseMonGfx_ShiftLargeUp(PokeathlonCourseMonGfxManager *manager) {
    if (manager == NULL) {
        GF_AssertFail();
    }
    if (manager->loaded != FALSE) {
        GF_AssertFail();
    }
    manager->shiftLargeUp = TRUE;
}

BOOL PokeathlonCourseMonGfx_IsShiftedUp(PokeathlonCourseMonGfx *mon) {
    return mon->shiftedUp;
}

#pragma force_active on

void PokeathlonCourse_InitObjTransfer(ObjCharTransferTemplate *charTemplate, int plttCount, enum HeapID heapId, GXOBJVRamModeChar modeMain, GXOBJVRamModeChar modeSub) {
    ObjCharTransfer_InitEx(charTemplate, modeMain, modeSub);
    ObjPlttTransfer_Init(plttCount, heapId);
    ObjCharTransfer_ClearBuffers();
    ObjPlttTransfer_Reset();
}

#pragma force_active reset

void PokeathlonCourse_FreeObjTransfer(void) {
    OamManager_Free();
    ObjCharTransfer_Destroy();
    ObjPlttTransfer_Destroy();
}
