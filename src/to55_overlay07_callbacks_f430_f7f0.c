#include "global.h"

#define PTR_AT(p, o)    (*(void **)((u8 *)(p) + (o)))
#define U32_AT(p, o)    (*(u32 *)((u8 *)(p) + (o)))
#define READ_WORD(work) ((work)->script++, *(work)->script)
#define RENDERER(work)  PTR_AT((work)->inner, 0xAC)

typedef struct Ov07ScriptWork {
    u8 pad00[0x18];
    u32 *script;
    u8 pad1C[0xA4];
    void *inner;
    u8 padC4[8];
    void *managers[4];
    void *sprites[16];
    u8 pad11C[0x20];
    void *drawSprites[16];
    u8 pad17C[0x3C];
    void *narcChar;
    void *narcPalette;
} Ov07ScriptWork;

void GF_AssertFail(void);
void *SpriteManager_New(void *renderer);
void SpriteSystem_InitSprites(void *, void *, u32);
void *SpriteSystem_GetRenderer(void *);
void G2dRenderer_SetSubSurfaceCoords(void *, int, int);
void SpriteSystem_InitManagerWithCapacities(void *, void *, u32 *);
void SpriteSystem_LoadCharResObjFromOpenNarc(void *, void *, void *, int, int, int, int);
void SpriteSystem_LoadPaletteBufferFromOpenNarc(void *, int, void *, void *, void *, int, int, int, int, int);
void SpriteSystem_LoadCellResObjFromOpenNarc(void *, void *, void *, int, int, int);
void SpriteSystem_LoadAnimResObjFromOpenNarc(void *, void *, void *, int, int, int);
void SpriteSystem_FreeResourcesAndManager(void *, void *);
void ManagedSprite_SetDrawFlag(void *, int);

void ov07_0221F430(Ov07ScriptWork *work);
void ov07_0221F4CC(Ov07ScriptWork *work);
void ov07_0221F514(Ov07ScriptWork *work);
void ov07_0221F574(Ov07ScriptWork *work);
void ov07_0221F5BC(Ov07ScriptWork *work);
void ov07_0221F7C4(Ov07ScriptWork *work);
void ov07_0221F7F0(Ov07ScriptWork *work);

typedef struct H04Template {
    s16 x, y, z;
    u16 animation;
    int drawPriority;
    int pal;
    int vram;
    int resIdList[6];
    int bgPriority;
    int vramTransfer;
} H04Template;
typedef struct H04Work {
    u8 pad00[0x18];
    u32 *script;
    u8 pad1c[0x94 - 0x1c];
    u32 vars[10];
    u8 padbc[4];
    void *inner;
    void *unkc4;
    void *unkc8;
    void *managers[4];
    void *sprites[10];
    H04Template template;
} H04Work;
void GF_AssertFail(void);
void *ov07_0221C470(H04Work *);
s16 ov07_02221F80(H04Work *, void *, int);
void *SpriteSystem_NewSprite(void *, void *, const H04Template *);
void *ov07_0222304C(u32);
void ov07_0221F604(H04Work *);
void ov07_0221F710(H04Work *);
#define READ(w) ((w)->script++, *(w)->script)
void ov07_0221F430(Ov07ScriptWork *work) {
    u32 index = READ_WORD(work);
    u32 count = READ_WORD(work);
    u32 capacities[6];
    int i;

    work->script++;
    if (work->managers[index] != NULL) {
        GF_AssertFail();
    }
    work->managers[index] = SpriteManager_New(RENDERER(work));
    if (work->managers[index] == NULL) {
        GF_AssertFail();
    }
    SpriteSystem_InitSprites(RENDERER(work), work->managers[index], count);
    G2dRenderer_SetSubSurfaceCoords(SpriteSystem_GetRenderer(RENDERER(work)), 0, 0x110000);
    for (i = 0; i < 6; i++) {
        capacities[i] = *work->script;
        work->script++;
    }
    SpriteSystem_InitManagerWithCapacities(RENDERER(work), work->managers[index], capacities);
}

void ov07_0221F4CC(Ov07ScriptWork *work) {
    u32 index = READ_WORD(work);
    u32 file = READ_WORD(work);
    work->script++;
    SpriteSystem_LoadCharResObjFromOpenNarc(RENDERER(work), work->managers[index], work->narcChar, file, 1, 1, file + 5000);
}

void ov07_0221F514(Ov07ScriptWork *work) {
    u32 index = READ_WORD(work);
    u32 file = READ_WORD(work);
    u32 slot = READ_WORD(work);
    work->script++;
    SpriteSystem_LoadPaletteBufferFromOpenNarc(PTR_AT(work, 0xC8), 2, RENDERER(work), work->managers[index], work->narcPalette, file, 0, 1, slot, file + 5000);
}

void ov07_0221F574(Ov07ScriptWork *work) {
    u32 index = READ_WORD(work);
    u32 file = READ_WORD(work);
    work->script++;
    SpriteSystem_LoadCellResObjFromOpenNarc(RENDERER(work), work->managers[index], PTR_AT(work, 0x1C0), file, 1, file + 5000);
}

void ov07_0221F5BC(Ov07ScriptWork *work) {
    u32 index = READ_WORD(work);
    u32 file = READ_WORD(work);
    work->script++;
    SpriteSystem_LoadAnimResObjFromOpenNarc(RENDERER(work), work->managers[index], PTR_AT(work, 0x1C4), file, 1, file + 5000);
}

void ov07_0221F604(H04Work *work) {
    H04Template template;
    u32 managerIndex = READ(work);
    u32 callbackId = READ(work);
    void *entity;
    void *sprite;
    int count;
    int i;
    void (*callback)(H04Work *, void *, void *, void *);

    work->script++;
    entity = ov07_0221C470(work);
    template.x = ov07_02221F80(work, entity, 0);
    template.y = ov07_02221F80(work, entity, 1);
    template.z = 0;
    template.animation = 0;
    template.drawPriority = 100;
    template.pal = 0;
    template.vram = 1;
    template.bgPriority = 1;
    template.vramTransfer = 0;
    for (i = 0; i < 6; i++) {
        template.resIdList[i] = *work->script + 5000;
        work->script++;
    }
    work->template = template;
    sprite = SpriteSystem_NewSprite(RENDERER(work), work->managers[managerIndex], &template);
    count = *work->script;
    work->script++;
    for (i = 0; i < count; i++) {
        work->vars[i] = *work->script;
        work->script++;
    }
    for (; i < 10; i++) {
        work->vars[i] = 0;
    }
    callback = ov07_0222304C(callbackId);
    callback(work, RENDERER(work), work->managers[managerIndex], sprite);
}

void ov07_0221F710(H04Work *work) {
    H04Template template;
    u32 managerIndex = READ(work);
    u32 spriteIndex = READ(work);
    void *entity;
    void *sprite;
    int i;

    work->script++;
    entity = ov07_0221C470(work);
    template.x = ov07_02221F80(work, entity, 0);
    template.y = ov07_02221F80(work, entity, 1);
    template.z = 0;
    template.animation = 0;
    template.drawPriority = 100;
    template.pal = 0;
    template.vram = 1;
    template.bgPriority = 1;
    template.vramTransfer = 0;
    for (i = 0; i < 6; i++) {
        template.resIdList[i] = *work->script + 5000;
        work->script++;
    }
    work->template = template;
    sprite = SpriteSystem_NewSprite(RENDERER(work), work->managers[managerIndex], &template);
    if (work->sprites[spriteIndex] != NULL) {
        GF_AssertFail();
    }
    work->sprites[spriteIndex] = sprite;
}
void ov07_0221F7C4(Ov07ScriptWork *work) {
    u32 index = READ_WORD(work);
    work->script++;
    if (work->managers[index] != NULL) {
        SpriteSystem_FreeResourcesAndManager(RENDERER(work), work->managers[index]);
    }
    work->managers[index] = NULL;
}

void ov07_0221F7F0(Ov07ScriptWork *work) {
    u32 index = READ_WORD(work);
    u32 draw = READ_WORD(work);
    work->script++;
    ManagedSprite_SetDrawFlag(work->drawSprites[index], draw);
}
