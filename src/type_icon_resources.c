#include "type_icon_resources.h"

#include "global.h"
extern const int gTypeIconCharFiles[23];
extern const u8 gTypeIconPalettes[23];
extern const int gMoveCategoryIconCharFiles[3];
extern const u8 gMoveCategoryIconPalettes[3];
int TypeIcon_GetCharFile(int type);
int TypeIcon_GetPlttFile(void);
int TypeIcon_GetCellFile(void);
int TypeIcon_GetAnimFile(void);
u32 TypeIcon_GetPalette(u32 type);
NarcId TypeIcon_GetNarc(void);
void TypeIcon_LoadChar(SpriteSystem *system, SpriteManager *manager, NNS_G2D_VRAM_TYPE vram, int type, int resource);
void TypeIcon_LoadPltt(SpriteSystem *system, SpriteManager *manager, int vram, int resource);
void TypeIcon_LoadPaletteBuffer(PaletteData *palette, int buffer, SpriteSystem *system, SpriteManager *manager, int vram, int resource);
void TypeIcon_LoadCellAnim(SpriteSystem *system, SpriteManager *manager, int cell, int animation);
void TypeIcon_UnloadChar(SpriteManager *manager, int resource);
void TypeIcon_UnloadPltt(SpriteManager *manager, int resource);
void TypeIcon_UnloadCellAnim(SpriteManager *manager, int cell, int animation);
ManagedSprite *TypeIcon_NewSprite(SpriteSystem *system, SpriteManager *manager, int type, ManagedSpriteTemplate *template);
void thunk_ManagedSprite_DeleteAndFreeResources(ManagedSprite *sprite);
int MoveCategoryIcon_GetCharFile(u32 category);
u32 MoveCategoryIcon_GetPalette(u32 category);
NarcId MoveCategoryIcon_GetNarc(void);
void MoveCategoryIcon_LoadChar(SpriteSystem *system, SpriteManager *manager, int vram, int category, int resource);
void MoveCategoryIcon_UnloadChar(SpriteManager *manager, int resource);
void MoveCategoryIcon_DeleteSprite(ManagedSprite *sprite);

int TypeIcon_GetCharFile(int type) {
    GF_ASSERT((u32)type < 23);
    return gTypeIconCharFiles[type];
}

int TypeIcon_GetPlttFile(void) {
    return 74;
}

int TypeIcon_GetCellFile(void) {
    return 242;
}

int TypeIcon_GetAnimFile(void) {
    return 243;
}

u32 TypeIcon_GetPalette(u32 type) {
    GF_ASSERT(type < 23);
    return gTypeIconPalettes[type];
}

NarcId TypeIcon_GetNarc(void) {
    return (NarcId)8;
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
    GF_ASSERT(category < 3);
    return gMoveCategoryIconCharFiles[category];
}

u32 MoveCategoryIcon_GetPalette(u32 category) {
    GF_ASSERT(category < 3);
    return gMoveCategoryIconPalettes[category];
}

NarcId MoveCategoryIcon_GetNarc(void) {
    return (NarcId)8;
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
