#pragma force_active on
#include "global.h"
#include "heap.h"
#include "sprite_system.h"
#include "pokepic.h"
#include "palette.h"
#include "obj_pltt_transfer.h"
typedef struct PicInfo07 {
    void *image;
    NarcId narcId;
    int fileId;
} PicInfo07;
typedef struct Priorities07 {
    u32 v[6];
} Priorities07;
typedef struct MonSprites07 {
    enum HeapID heapId;
    int battler;
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    PaletteData *paletteData;
    int resIds[4];
    ManagedSprite *sprites[4];
    PicInfo07 *infos[4];
    u8 flags[4];
    Pokepic *pokepics[4];
} MonSprites07;
extern const Priorities07 ov07_02234BD8;
SpriteSystem *BattleSystem_GetSpriteSystem(void *bsys);
SpriteManager *BattleSystem_GetSpriteManager(void *bsys);
PaletteData *BattleSystem_GetPaletteData(void *bsys);
PicInfo07 *ov12_0223BB88(void *bsys, int battler);
void ov12_0223C1C4(void *bsys, u8 *flags);
void ov12_0223C1F4(void *bsys, Pokepic **pokepics);
void GF_CreateNewVramTransferTask(u32 type, u32 dest, void *src, u32 size);
MonSprites07 *ov07_0221FB90(void *bsys, enum HeapID heapId, int battler);
// @fn ov07_0221FB90
MonSprites07 *ov07_0221FB90(void *bsys, enum HeapID heapId, int battler)
{
    Priorities07 priorities;
    int resIds [ 6 ];
    ManagedSpriteTemplate template;
    MonSprites07 * work;
    int i;
    NARC * narc;
    int j;
    int slot;
    int resId;
    Pokepic * pokepic;
    PicInfo07 * info;
    ManagedSprite * sprite;
    NarcId narcId;
    int fileId;
    s16 y;
    int y2;
    priorities = ov07_02234BD8;
    work = Heap_Alloc(heapId, sizeof( MonSprites07 ));
    work->heapId = heapId;
    work->battler = battler;
    work->spriteSystem = BattleSystem_GetSpriteSystem(bsys);
    work->spriteManager = BattleSystem_GetSpriteManager(bsys);
    work->paletteData = BattleSystem_GetPaletteData(bsys);
    for (i = 0; i < 4; i++)
    {
        work->sprites[i] = NULL;
        work->infos[i] = ov12_0223BB88(bsys, i);
    }
    ov12_0223C1C4(bsys, work->flags);
    ov12_0223C1F4(bsys, work->pokepics);
    narc = NARC_New(NARC_a_0_0_8, heapId);
    for (i = 0; i < 4; ++i)
    {
        if ((i == work->battler || work->battler == 0xFF) && work->pokepics[i] != NULL)
        {
            resId = i + 0xD903 + 5000 * work->battler;
            SpriteSystem_LoadCharResObjFromOpenNarc(work->spriteSystem, work->spriteManager, narc, 76, FALSE, 1, resId);
            SpriteSystem_LoadPaletteBufferFromOpenNarc(work->paletteData, PLTTBUF_MAIN_OBJ, work->spriteSystem, work->spriteManager, narc, 75, FALSE, 1, 1, resId);
            SpriteSystem_LoadCellResObjFromOpenNarc(work->spriteSystem, work->spriteManager, narc, 77, FALSE, resId);
            SpriteSystem_LoadAnimResObjFromOpenNarc(work->spriteSystem, work->spriteManager, narc, 78, FALSE, resId);
        }
    }
    NARC_Delete(narc);
    for (slot = 0; slot < 4; slot++)
    {
        void *image;
        int x;
        if (slot != work->battler && work->battler != 0xFF)
        {
            continue;
        }
        work->resIds[slot] = (slot + 0xD903 + 5000 * work->battler);
        resIds[0] = slot + 0xD903 + 5000 * work->battler;
        resIds[1] = slot + 0xD903 + 5000 * work->battler;
        resIds[2] = 5000 * work->battler + (slot + 0xD903);
        resIds[3] = slot + 0xD903 + 5000 * work->battler;
        resIds[4] = 0;
        resIds[5] = 0;
        info = work->infos[slot];
        narcId = info->narcId;
        fileId = info->fileId;
        image = info->image;
        pokepic = work->pokepics[slot];
        if (pokepic == NULL)
        {
            continue;
        }
        x = Pokepic_GetAttr(pokepic, 0);
        y = Pokepic_GetAttr(pokepic, 1);
        y2 = y - Pokepic_GetAttr(pokepic, 41);
        template.x = x;
        template.y = y2;
        template.z = 0;
        template.animation = 0;
        template.drawPriority = priorities.v[work->flags[slot]];
        template.pal = 0;
        template.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
        template.bgPriority = 1;
        template.vramTransfer = 0;
        for (j = 0; j < 6; j++)
        {
            template.resIdList[j] = resIds[j];
        }
        sprite = SpriteSystem_NewSprite(work->spriteSystem, work->spriteManager, &template);
        ManagedSprite_TickFrame(sprite);
        work->sprites[slot] = sprite;
        if (pokepic == NULL)
        {
            ManagedSprite_SetDrawFlag(sprite, FALSE);
        }
        else
        {
            if (Pokepic_IsActive(pokepic) == FALSE)
            {
                ManagedSprite_SetDrawFlag(sprite, FALSE);
            }
            else
            {
                if (Pokepic_GetAttr(pokepic, 6) == 1)
                {
                    ManagedSprite_SetDrawFlag(sprite, FALSE);
                }
            }
        }
        if (pokepic != NULL)
        {
            GF_CreateNewVramTransferTask(19, Sprite_GetImageProxy(sprite->sprite)->vramLocation.baseAddrOfVram[NNS_G2D_VRAM_TYPE_2DMAIN], image, 3200);
        }
        if (pokepic != NULL)
        {
            PaletteData_LoadNarc(work->paletteData, narcId, fileId, work->heapId, PLTTBUF_MAIN_OBJ, 32, ObjPlttTransfer_GetPaletteVramOffset(Sprite_GetPaletteProxy(sprite->sprite), NNS_G2D_VRAM_TYPE_2DMAIN) * 16);
        }
    }
    return work;
}
