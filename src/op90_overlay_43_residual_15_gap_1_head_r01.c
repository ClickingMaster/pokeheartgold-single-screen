#pragma force_active on
#include "global.h"
#include "list_menu_items.h"
#include "text.h"
#include "msgdata.h"
#include "sprite.h"

typedef struct {
    u8 data[6];
} Ov43Layout;

typedef struct {
    int msgId;
    int value;
} Ov43ListEntry;

typedef struct {
    MsgData *msgData;
    int unused;
} Ov43Unused;

typedef struct {
    void *bgConfig;
    SpriteList *spriteList;
    u8 unk8[0x54 - 0x8];
    MsgData *msgData;
    u8 unk58[0x88 - 0x58];
    SpriteResourcesHeader header;
} Ov43Ctx;

typedef struct {
    u8 unk0[0x38];
} Ov43Item;

typedef struct {
    u8 unk0[8];
    Ov43Item items[3];
    u8 unkB0[4];
    void *unkB4;
    Window window;
    Window window2;
    String *string;
    u8 unkDC[8];
    ListMenuItem *lists[2];
    u8 unkEC[0xF8 - 0xEC];
    Sprite *sprite;
} Ov43Work;

extern const u16 ov43_0222ED74[];
extern const Ov43Layout ov43_0222EE08[];
extern const Ov43ListEntry ov43_0222EEE0[2][4];
extern const SpriteTemplate ov43_0222EEB0;
void ov43_0222C788(Ov43Item *item, Ov43Ctx *ctx, const Ov43Layout *layout, u16 id, enum HeapID heapID);
void *ov42_02227060(SpriteList *spriteList, int a1, int a2, enum HeapID heapID);
void ov42_02227228(void *a0, int a1, int a2, enum HeapID heapID);
void ov43_0222A24C(void *bgConfig, Window *window, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9);
void ov43_0222B944(Ov43Work *work, int unused, Ov43Ctx *ctx, enum HeapID heapID);

// @fn ov43_0222B944
void ov43_0222B944(Ov43Work *work, int unused, Ov43Ctx *ctx, enum HeapID heapID) {
    SpriteTemplate template;
    String *string;
    int i;
    int j;

    for (i = 0; i < 3; i++) {
        ov43_0222C788(&work->items[i], ctx, &ov43_0222EE08[i], ov43_0222ED74[i], heapID);
    }
    work->unkB4 = ov42_02227060(ctx->spriteList, 0, 16, heapID);
    ov42_02227228(work->unkB4, 1, 2, heapID);
    ov43_0222A24C(ctx->bgConfig, &work->window, 1, 21, 21, 9, 2, 11, 0x14F, 0);
    string = NewString_ReadMsgData(ctx->msgData, 7);
    AddTextPrinterParameterizedWithColor(&work->window, 4, string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 15, 0), NULL);
    String_Delete(string);
    for (i = 0; i < 2; i++) {
        work->lists[i] = ListMenuItems_New(4, heapID);
        for (j = 0; j < 4; j++) {
            ListMenuItems_AppendFromMsgData(work->lists[i], ctx->msgData, ov43_0222EEE0[i][j].msgId, ov43_0222EEE0[i][j].value);
        }
    }
    ov43_0222A24C(ctx->bgConfig, &work->window2, 1, 2, 19, 27, 4, 11, 0x161, 15);
    work->string = String_New(128, heapID);
    template = ov43_0222EEB0;
    template.spriteList = ctx->spriteList;
    template.header = &ctx->header;
    template.heapID = heapID;
    work->sprite = Sprite_CreateAffine(&template);
    Sprite_SetDrawFlag(work->sprite, FALSE);
}
