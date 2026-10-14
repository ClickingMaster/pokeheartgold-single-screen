#include "global.h"

#include "application/vs_recorder/vs_recorder_internal.h"

#include "bg_window.h"
#include "font_sprite.h"
#include "obj_char_transfer.h"
#include "sprite_system.h"

// VS Recorder menu rows: the text label of every row sprite (built without -ipa file, see the Makefile).

typedef struct VsRecorderIntPair {
    int v[2];
} VsRecorderIntPair;

typedef struct VsRecorderVramPair {
    NNS_G2D_VRAM_TYPE v[2];
} VsRecorderVramPair;

typedef struct VsRecorderS8x6 {
    s8 v[6];
} VsRecorderS8x6;

#pragma force_active on

extern const u8 ov40_02244DC0[2][5];

// x offsets of the decor sprites (vs_recorder_ui.c)
const VsRecorderS8x6 gVsRecorderDecorSpriteXOffsets = {
    { -1, -3, 2, -1, -3, 2 }
};

static const VsRecorderVramPair sVrams = {
    { NNS_G2D_VRAM_TYPE_2DMAIN, NNS_G2D_VRAM_TYPE_2DSUB }
};
static const VsRecorderIntPair sPltts = {
    { 9999, 10000 }
};

void VsRecorder_CreateMenuRowText(VsRecorderWork *work) {
    TextOBJTemplate template;
    Window window;
    VsRecorderMenuRow *rows[2];
    VsRecorderVramPair vrams;
    VsRecorderIntPair pltts;
    s16 x;
    s16 y;
    int i;
    int j;
    int size;

    vrams = sVrams;
    pltts = sPltts;
    for (i = 0; i < 5; i++) {
        rows[0] = &work->choiceRows[i];
        rows[1] = &work->pathRows[i];
        for (j = 0; j < 2; j++) {
            InitWindow(&window);
            AddTextWindowTopLeftCorner(work->bgConfig, &window, 20, 2, 0, 0);
            rows[j]->textLayout = TextOBJLayout_New(&window, HEAP_ID_109);
            size = FontOAM_GetCharSize(&window, vrams.v[j], HEAP_ID_109);
            ObjCharTransfer_ReserveCharSpace(size, TRUE, vrams.v[j], &rows[j]->charAlloc);
            ManagedSprite_GetPositionXY(rows[j]->sprite, &x, &y);
            template.fontSystem = work->fontSystem;
            template.window = &window;
            template.spriteList = SpriteManager_GetSpriteList(work->spriteManager);
            template.plttResourceProxy = SpriteManager_FindPlttResourceProxy(work->spriteManager, pltts.v[j]);
            template.sprite = rows[j]->sprite->sprite;
            template.offset = rows[j]->charAlloc.offset;
            template.x = x + 36;
            template.y = y - 8;
            template.unk_20 = 3;
            template.unk_24 = ov40_02244DC0[j][i] - 1;
            template.vram = vrams.v[j];
            template.heapID = HEAP_ID_109;
            rows[j]->text = TextOBJ_Create(&template, rows[j]->textLayout);
            TextOBJ_SetPalOffsetRespectVramOffset(rows[j]->text, 1);
            RemoveWindow(&window);
            TextOBJ_SetSpritesDrawFlag(rows[j]->text, FALSE);
        }
    }
}
