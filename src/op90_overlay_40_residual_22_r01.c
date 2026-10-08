#pragma force_active on
#include "global.h"
#include "sprite.h"
#include "palette.h"
#include "pokemon_icon_idx.h"
#include "filesystem_files_def.h"

typedef struct SpriteSystem SpriteSystem;
typedef struct SpriteManager SpriteManager;

typedef struct {
    s16 x;
    s16 y;
    s16 z;
    u16 animation;
    int drawPriority;
    int pal;
    NNS_G2D_VRAM_TYPE vram;
    int resIdList[6];
    int bgPriority;
    int vramTransfer;
} Ov40SpriteTemplate;

u8 SpriteSystem_LoadPaletteBuffer(PaletteData *plttData, PaletteBufferId bufferId, SpriteSystem *spriteSystem, SpriteManager *spriteManager, NarcId narcId, int fileId, BOOL compressed, int pltt_num, int vram, int resId);
BOOL SpriteSystem_LoadCellResObj(SpriteSystem *spriteSystem, SpriteManager *spriteManager, NarcId narcId, int fileId, BOOL compressed, int resId);
BOOL SpriteSystem_LoadAnimResObj(SpriteSystem *spriteSystem, SpriteManager *spriteManager, NarcId narcId, int fileId, BOOL compressed, int resId);
BOOL SpriteSystem_LoadCharResObjAtEndWithHardwareMappingType(SpriteSystem *spriteSystem, SpriteManager *spriteManager, NarcId narcId, int fileId, BOOL compressed, int vram, int resId);
ManagedSprite *SpriteSystem_NewSprite(SpriteSystem *spriteSystem, SpriteManager *spriteManager, const Ov40SpriteTemplate *template);
void ManagedSprite_SetPaletteOverrideOffset(ManagedSprite *sprite, int offset);
void ManagedSprite_SetAnim(ManagedSprite *sprite, int anim);
void ManagedSprite_TickFrame(ManagedSprite *sprite);
void ManagedSprite_SetDrawPriority(ManagedSprite *sprite, int priority);

typedef struct {
    s16 x;
    s16 y;
} Ov40IconPos;

typedef struct {
    Ov40IconPos pos[2][12];
} Ov40IconLayout;

typedef struct {
    u8 unk0[4];
    void *summary;
    u8 unk8[4];
    ManagedSprite *sprites[12];
} Ov40Work;

typedef struct {
    u8 unk0[0x18];
    SpriteSystem *spriteSys;
    SpriteManager *spriteMan;
    u8 unk20[8];
    PaletteData *plttData;
} Ov40Ctx;

extern const Ov40IconLayout ov40_02244F90;
u64 sub_0203088C(void *summary, int field, int index);
BOOL ov40_02230404(Ov40Work *work);
void ov40_02230424(Ov40Work *work, Ov40Ctx *ctx);

// @fn ov40_02230424
void ov40_02230424(Ov40Work *work, Ov40Ctx *ctx) {
    int mon;
    int count = 0;
    Ov40IconLayout layout = ov40_02244F90;
    Ov40SpriteTemplate template;
    int species[12];
    int forms[12];
    int sel;
    SpriteSystem *spriteSys = ctx->spriteSys;
    SpriteManager *spriteMan = ctx->spriteMan;
    PaletteData *plttData = ctx->plttData;
    int step;
    int start;
    int end;
    int fm;
    int sp;
    int n;
    int i;
    int form;

    SpriteSystem_LoadPaletteBuffer(plttData, PLTTBUF_MAIN_OBJ, spriteSys, spriteMan, NARC_poketool_icongra_poke_icon, sub_02074490(), FALSE, 3, NNS_G2D_VRAM_TYPE_2DMAIN, 100000);
    SpriteSystem_LoadCellResObj(spriteSys, spriteMan, NARC_poketool_icongra_poke_icon, sub_0207449C(), FALSE, 100000);
    SpriteSystem_LoadAnimResObj(spriteSys, spriteMan, NARC_poketool_icongra_poke_icon, sub_020744A8(), FALSE, 100000);
    step = 6;
    end = 6;
    n = 0;
    start = 0;
    sel = 0;
    if (ov40_02230404(work) == TRUE) {
        step = 3;
        end = 3;
        sel = 1;
    }
    do {
        for (i = start; i < end; i++) {
            species[n] = 0xFF;
            mon = sub_0203088C(work->summary, 0, i);
            form = sub_0203088C(work->summary, 1, i);
            if (mon != 0) {
                species[n] = mon;
                forms[n] = form;
                n++;
            }
        }
        for (i = n; i < end; i++) {
            species[n] = 0;
            forms[n] = 0;
            n++;
        }
        start += step;
        end += step;
    } while (end <= 12);
    for (i = 0; i < 12; i++) {
        work->sprites[i] = NULL;
        if (species[i] != 0) {
            sp = species[i];
            fm = forms[i];
            SpriteSystem_LoadCharResObjAtEndWithHardwareMappingType(spriteSys, spriteMan, NARC_poketool_icongra_poke_icon, GetMonIconNaixEx(sp, FALSE, fm), FALSE, NNS_G2D_VRAM_TYPE_2DMAIN, 100000 + count);
            template.x = layout.pos[sel][i].x + 8;
            template.y = layout.pos[sel][i].y - 12;
            template.z = 0;
            template.animation = 0;
            template.drawPriority = 0;
            template.pal = 0;
            template.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
            template.bgPriority = 0;
            template.vramTransfer = 0;
            template.resIdList[0] = 100000 + count;
            template.resIdList[1] = 100000;
            template.resIdList[2] = 100000;
            template.resIdList[3] = 100000;
            template.resIdList[4] = -1;
            template.resIdList[5] = -1;
            work->sprites[count] = SpriteSystem_NewSprite(ctx->spriteSys, ctx->spriteMan, &template);
            ManagedSprite_SetPaletteOverrideOffset(work->sprites[count], GetMonIconPaletteEx(sp, fm, 0) + 4);
            ManagedSprite_SetAnim(work->sprites[count], 1);
            ManagedSprite_TickFrame(work->sprites[count]);
            ManagedSprite_SetDrawPriority(work->sprites[count], 12 - i);
            count++;
        }
    }
}
