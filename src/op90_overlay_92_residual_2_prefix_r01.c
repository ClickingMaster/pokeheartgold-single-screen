#pragma force_active on
#include "global.h"
typedef struct {
    u8 data[0x78];
} Inst;
typedef struct {
    u8 data[0x10];
} Res;
typedef struct {
    u8 data[0x14];
} Anim;
typedef struct {
    Inst inst;
    Res res;
    Anim anims[3];
    u8 gap[0x1E0 - 0xC4];
    VecFx32 pos;
    u8 tail[0x20C - 0x1EC];
} Obj;
typedef struct {
    u8 pad0[0x114];
    Obj fixed[5];
    Obj actors[8];
    u8 pad2[0x2BB0 - 0x1BB0];
    u32 seed;
    u32 colour;
} World;
typedef struct {
    u8 pad0[0x3C];
    u32 enabled;
} Config;
typedef struct {
    void *owner;
    World *world;
    u8 pad8[0xC];
    u8 colour;
    u8 pad15[0x44 - 0x15];
    int netId;
    void *res;
    u8 pad4C[0x54 - 0x4C];
    void *spriteMan;
    u8 pad58[0x5C - 0x58];
    void *palette;
    u8 pad60[0x6C - 0x60];
    u8 alloc[0x80 - 0x6C];
    u32 seed;
    void *sprite;
    Config *config;
} Ctx;
typedef struct {
    int pair[16][2];
} Table;
typedef struct {
    int item[4];
} Small;
extern const Table ov92_02263A14;
extern const Small ov92_022638A0;
u32 GetLCRNGSeed(void);
void ov92_0225D3CC(Ctx *);
void ov92_0225D894(void *);
void Main_SetVBlankIntrCB(void (*)(void *), void *);
void HBlankInterruptDisable(void);
void GF_CreateVramTransferManager(int, int);
void G2dRenderer_SetObjCharTransferReservedRegion(int, int);
void G2dRenderer_SetPlttTransferReservedRegion(int);
void sub_0203A880(void);
void *sub_0203A4AC(int);
void PaletteData_LoadPalette(void *, void *, int, int, int);
void Heap_Free(void *);
int sub_020347A0(void);
int sub_0203769C(void);
void ov92_0225C5A8(Ctx *);
int ov92_0225D8C4(Ctx *);
void *ov92_0225EB40(int, void **);
World *ov92_0225EB88(int, int, void **);
void sub_020210BC(void);
void sub_02021148(int);
void ov92_0225E9B4(Ctx *);
void ov92_0225E3C4(Ctx *);
void ov92_0225E6A4(Ctx *, World *);
void ov92_0225DE70(World *);
void ov92_0225DE08(World *);
void ov92_0225E820(World *);
void ov92_0225E93C(World *);
int ov92_0226077C(Ctx *, int);
void ov92_0225E130(Ctx *, int);
void ov92_0225D9A8(Obj *, void *, int);
void ov92_0225D9B4(Obj *, int);
void sub_020180BC(Anim *, Res *, void *, int, int, void *);
void sub_020181D4(Inst *, Anim *);
void sub_02018198(Anim *, int);
void sub_020182A0(Obj *, int);
void *SpriteManager_GetSpriteList(void *);
void *ov90_02258BD4(void *, int);
void PaletteData_LoadPaletteSlotFromHardware(void *, int, int, int);
void ov00_021E69A8(int);
void ov92_0225C5C4(Ctx *ctx);
#define ACTOR(c, o) ((Obj *)((u8 *)(c)->world->actors + (o)))
#define PW(c, o) ((World *)((u8 *)(c)->world + (o)))
#define AF(c, o, f) (*(__typeof__((c)->world->actors[0].f) *)((u8 *)&(c)->world->actors[0].f + (o)))
#define OBJ(w, o) ((Obj *)((u8 *)(w) + (o)))
#define OANIM(w, o, n) (&OBJ(w, o)->anims[n])
#define ORES(w, o) (&OBJ(w, o)->res)
#define OINST(w, o) (&OBJ(w, o)->inst)

// @fn ov92_0225C5C4
#pragma opt_propagation off
#pragma opt_strength_reduction off
void ov92_0225C5C4(Ctx *ctx) {
    NNSG2dPaletteData *pal;
    Small small;
    Table table;
    int i;
    void *raw;
    int n;
    World *world;
    int offset;
    int shift;
    int fixed;
    int step;
    int (*pair)[2];

    ctx->seed = GetLCRNGSeed();
    ov92_0225D3CC(ctx);
    Main_SetVBlankIntrCB(ov92_0225D894, ctx);
    HBlankInterruptDisable();
    GF_CreateVramTransferManager(32, 0x71);
    G2dRenderer_SetObjCharTransferReservedRegion(1, 0x200010);
    G2dRenderer_SetPlttTransferReservedRegion(1);
    sub_0203A880();
    raw = sub_0203A4AC(0x71);
    NNS_G2dGetUnpackedPaletteData(raw, &pal);
    PaletteData_LoadPalette(ctx->palette, pal->pRawData, 2, 0xE0, 32);
    Heap_Free(raw);
    n = sub_020347A0();
    ctx->netId = sub_0203769C();
    ov92_0225C5A8(ctx);
    if (ov92_0225D8C4(ctx) == 1) {
        ctx->owner = ov92_0225EB40(n, &ctx->res);
    }
    ctx->world = ov92_0225EB88(n, ctx->netId, &ctx->res);
    ctx->world->colour = ctx->colour;
    ctx->world->seed = ctx->seed;
    sub_020210BC();
    sub_02021148(4);
    ov92_0225E9B4(ctx);
    ov92_0225E3C4(ctx);
    ov92_0225E6A4(ctx, ctx->world);
    ov92_0225DE70(ctx->world);
    ov92_0225DE08(ctx->world);
    ov92_0225E820(ctx->world);
    ov92_0225E93C(ctx->world);
    ov92_0225E130(ctx, ov92_0226077C(ctx, ctx->netId));
    table = ov92_02263A14;
    step = 5;
    i = 0;
    pair = table.pair;
    offset = i;
    shift = offset;
    fixed = offset;
    do {
        ov92_0225D9A8(ACTOR(ctx, offset), ctx->res, (*pair)[0]);
        ov92_0225D9B4(ACTOR(ctx, offset), ctx->colour);
        sub_020180BC(&AF(ctx, offset, anims[0]), &AF(ctx, offset, res), ctx->res, (*pair)[1], 0x71, ctx->alloc);
        sub_020181D4(&AF(ctx, offset, inst), &AF(ctx, offset, anims[0]));
        sub_02018198(&AF(ctx, offset, anims[0]), 0);
        sub_020182A0(ACTOR(ctx, offset), 0);
        pair = ctx->colour ? (int (*)[2])pair : pair;
        if (i % 2) {
            PW(ctx, offset)->actors[0].pos.x += (fx32)(shift > 0 ? fixed + 0.5f : fixed - 0.5f);
        } else {
            PW(ctx, offset)->actors[0].pos.x -= (fx32)(shift > 0 ? fixed + 0.5f : fixed - 0.5f);
        }
        if (i < 8) {
            PW(ctx, offset)->actors[0].pos.z += FX32_CONST((i + 1) * step);
        } else {
            PW(ctx, offset)->actors[0].pos.z -= (fx32)(shift > 0 ? fixed + 0.5f : fixed - 0.5f);
        }
        pair++;
        shift += 5;
        offset += sizeof(Obj);
        fixed += 5 * FX32_ONE;
        i++;
    } while (i < 8);
    ov92_0225D9A8(&ctx->world->fixed[4], ctx->res, 0x47);
    ov92_0225D9B4(&ctx->world->fixed[4], ctx->colour);
    sub_020182A0(&ctx->world->fixed[4], 0);
    small = ov92_022638A0;
    n = small.item[ov92_0226077C(ctx, ctx->netId)];
    ov92_0225D9A8(&ctx->world->fixed[0], ctx->res, n);
    ov92_0225D9B4(&ctx->world->fixed[0], ctx->colour);
    ov92_0225D9A8(&ctx->world->fixed[1], ctx->res, 0x33);
    ov92_0225D9A8(&ctx->world->fixed[2], ctx->res, 0x39);
    ov92_0225D9A8(&ctx->world->fixed[3], ctx->res, 0x37);
    ov92_0225D9B4(&ctx->world->fixed[1], ctx->colour);
    ov92_0225D9B4(&ctx->world->fixed[2], ctx->colour);
    ov92_0225D9B4(&ctx->world->fixed[3], ctx->colour);
    world = ctx->world;
    offset = 0x320;
    sub_020180BC(OANIM(world, offset, 0), ORES(world, offset), ctx->res, 0x35, 0x71, ctx->alloc);
    sub_020181D4(OINST(world, offset), OANIM(world, offset, 0));
    sub_02018198(OANIM(world, offset, 0), 0);
    sub_020180BC(OANIM(world, offset, 1), ORES(world, offset), ctx->res, 0x34, 0x71, ctx->alloc);
    sub_02018198(OANIM(world, offset, 1), 0);
    sub_020180BC(OANIM(world, offset, 2), ORES(world, offset), ctx->res, 0x3A, 0x71, ctx->alloc);
    sub_02018198(OANIM(world, offset, 2), 0);
    world = ctx->world;
    offset = 0x52C;
    sub_020180BC(OANIM(world, offset, 0), ORES(world, offset), ctx->res, 0x38, 0x71, ctx->alloc);
    sub_020181D4(OINST(world, offset), OANIM(world, offset, 0));
    sub_02018198(OANIM(world, offset, 0), 0);
    world = ctx->world;
    offset = 0x738;
    sub_020180BC(OANIM(world, offset, 0), ORES(world, offset), ctx->res, 0x36, 0x71, ctx->alloc);
    sub_020181D4(OINST(world, offset), OANIM(world, offset, 0));
    sub_02018198(OANIM(world, offset, 0), 0);
    if (ctx->colour) {
        ctx->world->fixed[0].pos.y = -0x8000;
        ctx->world->fixed[1].pos.y = -0x8000;
        ctx->world->fixed[2].pos.y = -0x8000;
        ctx->world->fixed[3].pos.y = -0x8000;
    }
    ctx->sprite = ov90_02258BD4(SpriteManager_GetSpriteList(ctx->spriteMan), 0x71);
    PaletteData_LoadPaletteSlotFromHardware(ctx->palette, 2, 0, 0x200);
    if (ctx->config->enabled) {
        ov00_021E69A8(0x71);
    }
}
#pragma opt_propagation reset
#pragma opt_strength_reduction reset
