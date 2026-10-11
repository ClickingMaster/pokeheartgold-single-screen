#include "overlay14_sprite_manager_private.h"

void PCBox_SetSpriteDrawPriority(void *p, u32 slot, u32 priority) {
    U16_AT(p, 0x88d2) = 1;
    ManagedSprite_SetDrawPriority(PTR_AT(p, 0x2fc + slot * 4), priority);
    U16_AT(p, 0x88d2) = 0;
}

void PCBox_SetSpriteTranslucent(void *p, u32 slot, u32 enabled) {
    if (enabled == 1) {
        ManagedSprite_SetOamMode(PTR_AT(p, 0x2fc + slot * 4), 1);
    } else {
        ManagedSprite_SetOamMode(PTR_AT(p, 0x2fc + slot * 4), 0);
    }
}

void PCBox_InitSpriteSystem(void *p) {
    R50Counts counts;
    R50Transfer transfer;
    R50Oam oam;

    counts = ov14_021F80D4;
    PTR_AT(p, 0x2f4) = SpriteSystem_Alloc(0xa);
    PTR_AT(p, 0x2f8) = SpriteManager_New(PTR_AT(p, 0x2f4));
    transfer = ov14_021F80EC;
    oam = ov14_021F80A8;
    SpriteSystem_Init(PTR_AT(p, 0x2f4), &transfer, &oam, 0x20);
    SpriteSystem_InitSprites(PTR_AT(p, 0x2f4), PTR_AT(p, 0x2f8), 0x67);
    SpriteSystem_InitManagerWithCapacities(PTR_AT(p, 0x2f4), PTR_AT(p, 0x2f8), &counts);
    G2dRenderer_SetSubSurfaceCoords(SpriteSystem_GetRenderer(PTR_AT(p, 0x2f4)), 0, 2 << 20);
}

void PCBox_FreeSpriteSystem(void *p) {
    SpriteSystem_FreeResourcesAndManager(PTR_AT(p, 0x2f4), PTR_AT(p, 0x2f8));
    SpriteSystem_Free(PTR_AT(p, 0x2f4));
}

void PCBox_LoadSpriteResources(void *p) {
    PCBox_LoadTrayBoxResources(p);
    PCBox_LoadWallpaperListResources(p);
    PCBox_LoadMonIconResources(p);
    PCBox_LoadUiSpriteResources(p);
    PCBox_LoadItemIconResources(p);
    PCBox_LoadMonPicResources(p);
    PCBox_LoadTypeIconResources(p);
}

void PCBox_CreateSprites(void *p) {
    PCBox_CreateTrayBoxSprites(PTR_AT(p, 0x34));
    PCBox_CreateWallpaperListSprites(PTR_AT(p, 0x34));
    PCBox_CreateMonIconSprites(PTR_AT(p, 0x34));
    PCBox_CreateUiSprites(PTR_AT(p, 0x34));
    PCBox_CreateItemIconSprites(PTR_AT(p, 0x34));
    PCBox_CreateMonPicSprites(p);
    PCBox_CreateTypeIconSprites(PTR_AT(p, 0x34));
}

void PCBox_DeleteSprite(void *p, u32 slot) {
    u32 offset = slot * 4;
    void **sprites = (void **)((u8 *)p + 0x2fc);

    if (*(void **)((u8 *)sprites + offset) != NULL) {
        Sprite_DeleteAndFreeResources(*(void **)((u8 *)sprites + offset));
        *(void **)((u8 *)sprites + offset) = NULL;
    }
}
