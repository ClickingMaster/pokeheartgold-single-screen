#include "to46_overlay15_gap11_private.h"

void BagApp_ShowTMHMTypeIcons(BagWorkRaw *work, u16 item, BOOL draw) {
    u16 move;
    u16 type;
    u16 category;
    NarcId narc;

    ManagedSprite_SetDrawFlag(*(ManagedSprite **)&work->raw[0x26C], draw);
    ManagedSprite_SetDrawFlag(*(ManagedSprite **)&work->raw[0x270], draw);
    if (draw != FALSE) {
        move = TMHMGetMove(item);
        type = GetMoveAttr(move, MOVEATTR_TYPE);
        category = GetMoveAttr(move, MOVEATTR_CLASS);
        narc = TypeIcon_GetNarc();
        SpriteSystem_ReplaceCharResObj(*(SpriteSystem **)&work->raw[0x248], *(SpriteManager **)&work->raw[0x24C], narc, TypeIcon_GetCharFile(type), TRUE, 0xC103);
        ManagedSprite_SetPaletteOverride(*(ManagedSprite **)&work->raw[0x26C], TypeIcon_GetPalette(type) + 4);
        narc = MoveCategoryIcon_GetNarc();
        SpriteSystem_ReplaceCharResObj(*(SpriteSystem **)&work->raw[0x248], *(SpriteManager **)&work->raw[0x24C], narc, MoveCategoryIcon_GetCharFile(category), TRUE, 0xC104);
        ManagedSprite_SetPaletteOverride(*(ManagedSprite **)&work->raw[0x270], MoveCategoryIcon_GetPalette(category) + 4);
    }
}

void BagApp_CreateSpriteSystem(BagWorkRaw *work) {
    SpriteResourceCountsListUnion counts = sBagSpriteResourceCounts;
    *(SpriteSystem **)&work->raw[0x248] = SpriteSystem_Alloc(HEAP_ID_6);
    *(SpriteManager **)&work->raw[0x24C] = SpriteManager_New(*(SpriteSystem **)&work->raw[0x248]);
    OamManagerParam oam = sBagOamManagerParam;
    OamCharTransferParam transfer = sBagOamCharTransferParam;
    SpriteSystem_Init(*(SpriteSystem **)&work->raw[0x248], &oam, &transfer, 0x20);
    SpriteSystem_InitSprites(*(SpriteSystem **)&work->raw[0x248], *(SpriteManager **)&work->raw[0x24C], 0x27);
    SpriteSystem_InitManagerWithCapacities(*(SpriteSystem **)&work->raw[0x248], *(SpriteManager **)&work->raw[0x24C], &counts);
}
