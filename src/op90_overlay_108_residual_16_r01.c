#pragma force_active on
#include "global.h"
#include "heap.h"
#include "sprite.h"
#include "unk_02009D48.h"
typedef struct {
    Sprite *sprite;
    SpriteResourcesHeader *header;
    SpriteResourceHeaderList *list;
    int unk0C;
} Managed;
typedef struct {
    enum HeapID heapId;
    u8 pad[12];
    SpriteList *main;
    SpriteList *sub;
    u8 rest[0x144 - 24];
    GF_2DGfxResMan *man[4];
} Work;
void ov108_021E84F8(Work *, u8, u8);
Managed *ov108_021E8540(Work *w, u16 x, u16 y, u8 transfer, u8 priority, u8 id, u8 anim, int vram);
// @fn ov108_021E8540
Managed *ov108_021E8540(Work *w, u16 x, u16 y, u8 transfer, u8 priority, u8 id, u8 anim, int vram) {
    Managed *m;
    SpriteTemplate t;
    m = Heap_Alloc(w->heapId, sizeof(Managed));
    MI_CpuFill8(m, 0, sizeof(Managed));
    m->list = Heap_Alloc(w->heapId, sizeof(SpriteResourceHeaderList));
    m->list->headers = Heap_Alloc(w->heapId, sizeof(SpriteResourcesHeader));
    m->header = m->list->headers;
    CreateSpriteResourcesHeader(m->header, 0xe000 + id, 0xe000, 0xe000, 0xe000, -1, -1, 0, transfer, w->man[0], w->man[1], w->man[2], w->man[3], NULL, NULL);
    if (w->sub != NULL) {
        t.spriteList = w->sub;
    } else {
        t.spriteList = w->main;
    }
    t.header = m->header;
    t.position.x = FX32_CONST(x);
    t.position.y = FX32_CONST(y);
    t.position.z = 0;
    if (vram == 2) {
        t.position.y += FX32_CONST(192);
    }
    t.scale.x = FX32_ONE;
    t.scale.y = FX32_ONE;
    t.scale.z = FX32_ONE;
    t.rotation = 0;
    t.drawPriority = priority;
    t.whichScreen = (NNS_G2D_VRAM_TYPE)vram;
    t.heapID = w->heapId;
    ov108_021E84F8(w, id, anim);
    m->sprite = Sprite_CreateAffine(&t);
    GF_ASSERT(m->sprite != NULL);
    return m;
}
