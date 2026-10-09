#pragma force_active on
#include "global.h"
#include "sprite_system.h"
#include "bg_window.h"
#include "unk_02013534.h"
#include "obj_char_transfer.h"

typedef struct {
    ManagedSprite *sprite;
    u8 unk4[0x10];
    TextOBJ *textObj;
    UnkStruct_02013910 *textData;
    UnkStruct_02021AC8 charAlloc;
} Ov40Choice;

typedef struct {
    u8 unk0[0x1C];
    SpriteManager *spriteMan;
    u8 unk20[4];
    BgConfig *bgConfig;
    u8 unk28[0x50 - 0x28];
    UnkStruct_02013534 *fontSystem;
    u8 unk54[0x534 - 0x54];
    Ov40Choice choices[5];
    Ov40Choice subChoices[5];
} Ov40Work;

typedef struct {
    int v[2];
} Ov40IntPair;

typedef struct {
    NNS_G2D_VRAM_TYPE v[2];
} Ov40VramPair;

extern const u8 ov40_02244DC0[2][5];
typedef struct { s8 v[6]; } Ov40S8x6;
const Ov40S8x6 ov40_02244DA8 = {{-1, -3, 2, -1, -3, 2}};
static const Ov40VramPair sVrams = {{NNS_G2D_VRAM_TYPE_2DMAIN, NNS_G2D_VRAM_TYPE_2DSUB}};
static const Ov40IntPair sPltts = {{9999, 10000}};
void ov40_0222D2A0(Ov40Work *work);

// @fn ov40_0222D2A0
void ov40_0222D2A0(Ov40Work *work) {
    TextOBJTemplate template;
    Window window;
    Ov40Choice *choices[2];
    Ov40VramPair vrams;
    Ov40IntPair pltts;
    s16 x;
    s16 y;
    int i;
    int j;
    int size;

    vrams = sVrams;
    pltts = sPltts;
    for (i = 0; i < 5; i++) {
        choices[0] = &work->subChoices[i];
        choices[1] = &work->choices[i];
        for (j = 0; j < 2; j++) {
            InitWindow(&window);
            AddTextWindowTopLeftCorner(work->bgConfig, &window, 20, 2, 0, 0);
            choices[j]->textData = sub_02013910(&window, HEAP_ID_109);
            size = sub_02013688(&window, vrams.v[j], HEAP_ID_109);
            sub_02021AC8(size, TRUE, vrams.v[j], &choices[j]->charAlloc);
            ManagedSprite_GetPositionXY(choices[j]->sprite, &x, &y);
            template.fontSystem = work->fontSystem;
            template.window = &window;
            template.spriteList = SpriteManager_GetSpriteList(work->spriteMan);
            template.plttResourceProxy = SpriteManager_FindPlttResourceProxy(work->spriteMan, pltts.v[j]);
            template.sprite = choices[j]->sprite->sprite;
            template.offset = choices[j]->charAlloc.offset;
            template.x = x + 36;
            template.y = y - 8;
            template.unk_20 = 3;
            template.unk_24 = ov40_02244DC0[j][i] - 1;
            template.vram = vrams.v[j];
            template.heapID = HEAP_ID_109;
            choices[j]->textObj = TextOBJ_Create(&template, choices[j]->textData);
            sub_020138E0(choices[j]->textObj, 1);
            RemoveWindow(&window);
            TextOBJ_SetSpritesDrawFlag(choices[j]->textObj, FALSE);
        }
    }
}
