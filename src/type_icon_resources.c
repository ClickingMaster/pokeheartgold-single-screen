#include "type_icon_resources.h"

#include "global.h"

static int TypeIcon_GetCellFile(void);
static int TypeIcon_GetAnimFile(void);

static const u8 sMoveCategoryIconPalettes[3] = { 0, 1, 0 };

static const int sMoveCategoryIconCharFiles[3] = { 244, 246, 245 };

static const int sTypeIconCharFiles[23] = {
    234, 225, 227, 235, 229, 237, 231, 228, 238, 236, 226, 241, 233, 222, 223, 230, 221, 224, 240, 219, 220, 232, 239
};

static const u8 sTypeIconPalettes[23] = { 0, 0, 1, 1, 0, 0, 2, 1, 0, 2, 0, 1, 2, 0, 1, 1, 2, 0, 0, 1, 1, 2, 0 };

int TypeIcon_GetCharFile(int type) {
    GF_ASSERT((u32)type < NELEMS(sTypeIconCharFiles));
    return sTypeIconCharFiles[type];
}

int TypeIcon_GetPlttFile(void) {
    return 74;
}

static int TypeIcon_GetCellFile(void) {
    return 242;
}

static int TypeIcon_GetAnimFile(void) {
    return 243;
}

u32 TypeIcon_GetPalette(u32 type) {
    GF_ASSERT(type < NELEMS(sTypeIconPalettes));
    return sTypeIconPalettes[type];
}

NarcId TypeIcon_GetNarc(void) {
    return NARC_a_0_0_8;
}

void TypeIcon_LoadChar(SpriteSystem *system, SpriteManager *manager, NNS_G2D_VRAM_TYPE vram, int type, int resource) {
    NarcId narc = TypeIcon_GetNarc();
    int file = TypeIcon_GetCharFile(type);
    SpriteSystem_LoadCharResObj(system, manager, narc, file, TRUE, vram, resource);
}

void TypeIcon_LoadPltt(SpriteSystem *system, SpriteManager *manager, int vram, int resource) {
    NarcId narc = TypeIcon_GetNarc();
    int file = TypeIcon_GetPlttFile();
    SpriteSystem_LoadPlttResObj(system, manager, narc, file, FALSE, 3, vram, resource);
}

void TypeIcon_LoadPaletteBuffer(PaletteData *palette, int buffer, SpriteSystem *system, SpriteManager *manager, int vram, int resource) {
    NarcId narc = TypeIcon_GetNarc();
    int file = TypeIcon_GetPlttFile();
    SpriteSystem_LoadPaletteBuffer(palette, (PaletteBufferId)buffer, system, manager, narc, file, FALSE, 3, vram, resource);
}

void TypeIcon_LoadCellAnim(SpriteSystem *system, SpriteManager *manager, int cell, int animation) {
    NarcId narc = TypeIcon_GetNarc();
    int file = TypeIcon_GetCellFile();
    SpriteSystem_LoadCellResObj(system, manager, narc, file, TRUE, cell);
    narc = TypeIcon_GetNarc();
    file = TypeIcon_GetAnimFile();
    SpriteSystem_LoadAnimResObj(system, manager, narc, file, TRUE, animation);
}

void TypeIcon_UnloadChar(SpriteManager *manager, int resource) {
    SpriteManager_UnloadCharObjById(manager, resource);
}

void TypeIcon_UnloadPltt(SpriteManager *manager, int resource) {
    SpriteManager_UnloadPlttObjById(manager, resource);
}

void TypeIcon_UnloadCellAnim(SpriteManager *manager, int cell, int animation) {
    SpriteManager_UnloadCellObjById(manager, cell);
    SpriteManager_UnloadAnimObjById(manager, animation);
}

ManagedSprite *TypeIcon_NewSprite(SpriteSystem *system, SpriteManager *manager, int type, ManagedSpriteTemplate *template) {
    ManagedSpriteTemplate copy = *template;
    copy.pal = TypeIcon_GetPalette(type);
    return SpriteSystem_NewSprite(system, manager, &copy);
}

void thunk_ManagedSprite_DeleteAndFreeResources(ManagedSprite *sprite) {
    Sprite_DeleteAndFreeResources(sprite);
}

int MoveCategoryIcon_GetCharFile(u32 category) {
    GF_ASSERT(category < NELEMS(sMoveCategoryIconCharFiles));
    return sMoveCategoryIconCharFiles[category];
}

u32 MoveCategoryIcon_GetPalette(u32 category) {
    GF_ASSERT(category < NELEMS(sMoveCategoryIconPalettes));
    return sMoveCategoryIconPalettes[category];
}

NarcId MoveCategoryIcon_GetNarc(void) {
    return NARC_a_0_0_8;
}

void MoveCategoryIcon_LoadChar(SpriteSystem *system, SpriteManager *manager, int vram, int category, int resource) {
    NarcId narc = MoveCategoryIcon_GetNarc();
    int file = MoveCategoryIcon_GetCharFile(category);
    SpriteSystem_LoadCharResObj(system, manager, narc, file, TRUE, vram, resource);
}

void MoveCategoryIcon_UnloadChar(SpriteManager *manager, int resource) {
    SpriteManager_UnloadCharObjById(manager, resource);
}

void MoveCategoryIcon_DeleteSprite(ManagedSprite *sprite) {
    Sprite_DeleteAndFreeResources(sprite);
}
