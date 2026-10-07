#pragma force_active on
#include "global.h"
#include "bg_window.h"
#include "gf_gfx_loader.h"
#include "render_window.h"
#include "text.h"

typedef struct Ov48Template {
    u8 unk0[0xC];
} Ov48Template;

typedef struct Ov48Entry {
    u8 unk0[0x28];
} Ov48Entry;

typedef struct Ov48Unk {
    int state;
    u8 unk4[0x90];
    void *scrnData;
    NNSG2dScreenData *screen;
    u8 unk9C[4];
    Ov48Entry entries[5];
    u8 unk168[0x40];
    Window window;
    u8 unk1B8[0x12C];
    u8 unk2E4[4];
} Ov48Unk;

typedef struct Ov48Ctx {
    BgConfig *bgConfig;
    u8 unk4[0x140];
    NARC *narc;
} Ov48Ctx;

extern const u8 _0225B164[];
extern const Ov48Template ov48_0225B268[];

void ov48_0225A288(Ov48Unk *data);
void ov48_0225A894(Ov48Entry *entry, Ov48Ctx *ctx, const Ov48Template *tmpl, u8 kind, enum HeapID heapID);
String *ov48_0225B0C4(void *msg, int id);
void ov48_0225A6DC(Ov48Unk *data, Ov48Ctx *ctx, enum HeapID heapID);
void ov48_0225AAAC(void *a0, Ov48Ctx *ctx, void *a2, enum HeapID heapID);
void ov48_0225A5C4(void *a0);
void ov48_0225A00C(Ov48Unk *data, Ov48Ctx *ctx, void *msg, enum HeapID heapID);

// @fn ov48_0225A00C
void ov48_0225A00C(Ov48Unk *data, Ov48Ctx *ctx, void *msg, enum HeapID heapID) {
    int i;

    data->state = 0;
    ov48_0225A288(data);
    for (i = 0; i < 5; i++) {
        ov48_0225A894(&data->entries[i], ctx, &ov48_0225B268[i], _0225B164[i], heapID);
    }
    AddWindowParameterized(ctx->bgConfig, &data->window, GF_BG_LYR_SUB_0, 2, 19, 27, 4, 11, 40);
    FillWindowPixelBuffer(&data->window, 15);
    AddTextPrinterParameterized(&data->window, 1, ov48_0225B0C4(msg, 0), 0, 0, 0xFF, NULL);
    DrawFrameAndWindow2(&data->window, FALSE, 10, 10);
    ov48_0225A6DC(data, ctx, heapID);
    data->scrnData = GfGfxLoader_GetScrnDataFromOpenNarc(ctx->narc, 19, FALSE, &data->screen, heapID);
    ov48_0225AAAC(data->unk1B8, ctx, data->unk2E4, heapID);
    ov48_0225A5C4(data->unk2E4);
}
