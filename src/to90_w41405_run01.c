#pragma force_active on
#include "global.h"
#include "unk_02009D48.h"
#pragma force_active on
#pragma opt_optimizecontrolflow off
typedef struct { s16 kind; u8 unused[6]; } Record;
typedef struct { u8 charCount, paletteCount, cellCount, animationCount, charKind, paletteKind, cellKind, animationKind; SpriteList *sprites; G2dRenderer renderer; GF_2DGfxResMan *managers[4]; Record *charRecords, *paletteRecords, *cellRecords, *animationRecords; } Work;
typedef char RendererSizeCheck[sizeof(G2dRenderer)==0x128?1:-1];
void *ov02_0224B690(int, int);

void ov02_02248728(Work *, int, int, int, int, int, int, int, int, int);
void ov02_02248728(Work *work, int sprites, int chars, int palettes, int cells, int animations, int charKind, int paletteKind, int cellKind, int animationKind)
{
 int i;
 work->charCount = chars; work->paletteCount = palettes; work->cellCount = cells; work->animationCount = animations;
 work->charKind = charKind; work->paletteKind = paletteKind; work->cellKind = cellKind; work->animationKind = animationKind;
 work->sprites = G2dRenderer_Init(sprites, &work->renderer, HEAP_ID_FIELD1);
 G2dRenderer_SetSubSurfaceCoords(&work->renderer, 0, 0x200000);
 work->managers[0] = Create2DGfxResObjMan(chars, GF_GFX_RES_TYPE_CHAR, HEAP_ID_FIELD1);
 work->managers[1] = Create2DGfxResObjMan(palettes, GF_GFX_RES_TYPE_PLTT, HEAP_ID_FIELD1);
 work->managers[2] = Create2DGfxResObjMan(cells, GF_GFX_RES_TYPE_CELL, HEAP_ID_FIELD1);
 work->managers[3] = Create2DGfxResObjMan(animations, GF_GFX_RES_TYPE_ANIM, HEAP_ID_FIELD1);
 work->charRecords = ov02_0224B690(4, chars * 8);
 work->paletteRecords = ov02_0224B690(4, palettes * 8);
 work->cellRecords = ov02_0224B690(4, cells * 8);
 work->animationRecords = ov02_0224B690(4, animations * 8);
 i = 0; if (chars > 0) for (; i < chars; i++) work->charRecords[i].kind = charKind;
 i = 0; if (palettes > 0) for (; i < palettes; i++) work->paletteRecords[i].kind = paletteKind;
 i = 0; if (cells > 0) for (; i < cells; i++) work->cellRecords[i].kind = cellKind;
 i = 0; if (animations > 0) for (; i < animations; i++) work->animationRecords[i].kind = animationKind;
}
