#pragma force_active on
#include "global.h"
#include <nitro/os/cache.h>
#include "filesystem.h"
#include "follow_mon.h"
#include "heap.h"
#include "pokemon.h"
#include "sprite_system.h"
#include "unk_02013FDC.h"

typedef struct Ov112WalkerMonSprite {
    int large;
    int species;
    ManagedSprite *small;
    ManagedSprite *big;
    ManagedSprite *shadow;
} Ov112WalkerMonSprite;

typedef struct Ov112ModelInfo {
    u8 unk0;
    u8 isLarge;
    u8 unk2;
    u8 unk3;
} Ov112ModelInfo;

extern int ov112_021F0E14(int species, int form, int gender);
extern void ov112_021F1D70(Sprite *sprite, void *buf, u32 size, u32 offset);
void ov112_021F2098(Ov112WalkerMonSprite *work, u16 species, u8 form, int gender, u8 shiny, int heapId);

// @fn ov112_021F2098
void ov112_021F2098(Ov112WalkerMonSprite *work, u16 species, u8 form, int gender, u8 shiny, int heapId) {
    NARC *narc;
    void *file;
    NNSG3dResTex *tex;
    Sprite *sprite;
    u8 *pixels;
    int dim;
    s16 x;
    s16 y;
    Ov112ModelInfo info;
    int size;
    void *buf;
    int i;
    u32 offset;
    u8 *pltt;
    BOOL draw;

    ReadWholeNarcMemberByIdPair(&info, NARC_fielddata_tsurepoke_tp_param, SpeciesToOverworldModelIndexOffset(species));
    if (info.isLarge) {
        size = 0x800;
        sprite = work->big->sprite;
        dim = 8;
        work->large = TRUE;
        ManagedSprite_GetPositionXY(work->big, &x, &y);
        ManagedSprite_SetPositionXY(work->shadow, x, y);
    } else {
        size = 0x200;
        sprite = work->small->sprite;
        dim = 4;
        work->large = FALSE;
        ManagedSprite_GetPositionXY(work->small, &x, &y);
        ManagedSprite_SetPositionXY(work->shadow, x, y);
    }
    work->species = species;
    narc = NARC_New(NARC_data_mmodel_mmodel, (enum HeapID)heapId);
    if (!sub_02070438(species, form)) {
        form = 0;
    }
    file = NARC_AllocAndReadWholeMember(narc, ov112_021F0E14(species, form, gender), (enum HeapID)heapId);
    tex = NNS_G3dGetTex(file);
    pixels = (u8 *)tex + tex->texInfo.ofsTex;
    buf = Heap_AllocAtEnd((enum HeapID)heapId, size);
    for (i = 0, offset = 0; i < 8; i++) {
        sub_020145B4(pixels + offset, dim, 0, 0, dim, dim, buf);
        ov112_021F1D70(sprite, buf, size, offset);
        offset += size;
    }
    Heap_Free(buf);
    Sprite_GetVramType(sprite);
    pltt = (u8 *)tex + tex->plttInfo.ofsPlttData;
    if (shiny) {
        pltt += 0x20;
    }
    DC_FlushRange(pltt, 0x20);
    GX_LoadOBJPltt(pltt, NNS_G2dGetImagePaletteLocation(Sprite_GetPaletteProxy(sprite), NNS_G2D_VRAM_TYPE_2DMAIN), 0x20);
    Heap_Free(file);
    NARC_Delete(narc);
    thunk_Sprite_SetDrawFlag(sprite, TRUE);
    draw = TRUE;
    if (work->species == 50 || work->species == 51) {
        draw = FALSE;
    }
    ManagedSprite_SetDrawFlag(work->shadow, draw);
}
