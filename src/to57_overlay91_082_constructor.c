#include "to57_overlay91_082_private.h"

typedef struct Ov91Mgr82 {
    void *bgConfig;
    u8 pad04[0x18];
    void *spriteList;
    u8 pad20[0x128];
    void *resMan[4];
    u8 pad158[0xf4];
    NARC *narc;
} Ov91Mgr82;

typedef struct Ov91Interp82 {
    u8 raw[0x18];
} Ov91Interp82;
typedef struct Ov91Work82 {
    SpriteResource *res[4];
    Header82 header;
    Sprite *sprite;
    Window82 window;
    TextOBJ *textObj;
    Layout *fontLayout;
    Transfer82 charTransfer;
    void *string;
    SpriteResource *textPltt;
    Ov91Interp82 interp1;
    Ov91Interp82 interp2;
    s16 counter;
    s16 counter2;
    u16 state;
    s16 delay;
    int heapId;
} Ov91Work82;

void ov91_02261580(Ov91Work82 *, Ov91Mgr82 *, NARC *, int);

void ov91_02261580(Ov91Work82 *work, Ov91Mgr82 *mgr, NARC *narc, int heapId) {
    Simple82 spriteTemplate;
    TextTemplate82 textTemplate;
    work->heapId = heapId;
    work->res[0] = AddCharResObjFromOpenNarc(mgr->resMan[0], narc, 10, FALSE, 0x8c, 1, heapId);
    work->res[1] = AddPlttResObjFromOpenNarc(mgr->resMan[1], narc, 11, FALSE, 0x8c, 1, 1, heapId);
    work->res[2] = AddCellOrAnimResObjFromOpenNarc(mgr->resMan[2], narc, 9, FALSE, 0x8c, 2, heapId);
    work->res[3] = AddCellOrAnimResObjFromOpenNarc(mgr->resMan[3], narc, 8, FALSE, 0x8c, 3, heapId);
    GF_ASSERT(SpriteTransfer_CreateCharTransferTask_UpdateMappingTypeFromHW_AllocAtEnd(work->res[0]));
    GF_ASSERT(SpriteTransfer_CreatePlttTransferTask(work->res[1]));
    sub_0200A740(work->res[0]);
    sub_0200A740(work->res[1]);
    CreateSpriteResourcesHeader(&work->header, 0x8c, 0x8c, 0x8c, 0x8c, -1, -1, 0, 0, mgr->resMan[0], mgr->resMan[1], mgr->resMan[2], mgr->resMan[3], NULL, NULL);
    spriteTemplate.spriteList = mgr->spriteList;
    spriteTemplate.header = &work->header;
    spriteTemplate.position.x = -0x40000;
    spriteTemplate.position.y = 0x30000;
    spriteTemplate.priority = 1;
    spriteTemplate.whichScreen = 1;
    spriteTemplate.heapID = heapId;
    work->sprite = Sprite_Create(&spriteTemplate);
    Sprite_SetDrawFlag(work->sprite, FALSE);
    work->string = String_New(0x10, heapId);
    AddTextWindowTopLeftCorner(mgr->bgConfig, &work->window, 3, 2, 0, 0);
    work->fontLayout = TextOBJLayout_New(&work->window, heapId);
    GF_ASSERT(sub_02021AC8(TextOBJLayout_GetCharSize(work->fontLayout, 1), TRUE, 1, &work->charTransfer) == TRUE);
    work->textPltt = AddPlttResObjFromNarc(mgr->resMan[1], 0x10, 7, FALSE, 0x96, 1, 1, heapId);
    GF_ASSERT(SpriteTransfer_CreatePlttTransferTask(work->textPltt));
    sub_0200A740(work->textPltt);
    textTemplate.font = *(void **)((u8 *)mgr + 0x18c);
    textTemplate.window = &work->window;
    textTemplate.spriteList = mgr->spriteList;
    textTemplate.plttResourceProxy = SpriteTransfer_GetPaletteProxy(work->textPltt, NULL);
    textTemplate.sprite = work->sprite;
    textTemplate.offset = *(u32 *)((u8 *)work + 0x54);
    textTemplate.x = -7;
    textTemplate.y = -5;
    textTemplate.unk_20 = 0;
    textTemplate.unk_24 = 0;
    textTemplate.vram = 1;
    textTemplate.heapID = heapId;
    work->textObj = TextOBJ_Create(&textTemplate, work->fontLayout);
    TextOBJ_SetSpritesDrawFlag(work->textObj, FALSE);
    RemoveWindow(&work->window);
    ov91_0225D40C(&work->interp1, -0x40000, 0x20000, 0x24000, 8);
    ov91_0225D40C(&work->interp2, 0x30000, 0x30000, 0xc000, 4);
    work->counter = 0;
    work->counter2 = 4;
}
