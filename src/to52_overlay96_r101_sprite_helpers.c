#include "to52_overlay96_r101_sprite_helpers_private.h"

ManagedSprite *ov96_0220ECA4(SpriteSystem *system, SpriteManager *manager, u16 x, u16 y, u16 animation, u16 priority, BOOL second) {
    ManagedSpriteTemplate template0 = ov96_0221CFEC;
    ManagedSpriteTemplate template1 = ov96_0221D020;
    ManagedSpriteTemplate *template = second ? &template1 : &template0;
    ManagedSprite *sprite;
    if (system == NULL) {
        GF_AssertFail();
    }
    if (manager == NULL) {
        GF_AssertFail();
    }
    template->x = x;
    template->y = y;
    template->animation = animation;
    template->drawPriority = priority;
    sprite = SpriteSystem_NewSpriteWithYOffset(system, manager, template, 0x1E0000);
    ManagedSprite_SetAffineOverwriteMode(sprite, 2);
    ManagedSprite_SetAnimateFlag(sprite, TRUE);
    ManagedSprite_SetDrawFlag(sprite, FALSE);
    return sprite;
}

ManagedSprite *ov96_0220ED34(SpriteSystem *system, SpriteManager *manager, u16 x, u16 y, u16 animation, u16 priority) {
    ManagedSpriteTemplate template = ov96_0221D054;
    ManagedSprite *sprite;
    if (system == NULL) {
        GF_AssertFail();
    }
    if (manager == NULL) {
        GF_AssertFail();
    }
    template.x = x;
    template.y = y;
    template.animation = animation;
    template.drawPriority = priority;
    sprite = SpriteSystem_NewSpriteWithYOffset(system, manager, &template, 0x1E0000);
    ManagedSprite_SetAnimateFlag(sprite, TRUE);
    ManagedSprite_SetDrawFlag(sprite, FALSE);
    return sprite;
}

void ov96_0220ED9C(Ov96R101SetupWork *work) {
    BgConfig *bgConfig = work->bgConfig;
    enum HeapID heapId = (enum HeapID)work->heapId;
    GraphicsModes modes = ov96_0221CF0C;
    int i;

    SetBothScreensModesAndDisable(&modes);

    Ov96BgTemplates7 templates = ov96_0221D088;
    for (i = 0; i < 7; i++) {
        InitBgFromTemplate(bgConfig, (u8)ov96_0221CF58[i], &templates.items[i], 0);
        BgClearTilemapBufferAndCommit(bgConfig, (u8)ov96_0221CF58[i]);
        BG_ClearCharDataRange((u8)ov96_0221CF58[i], 0x20, 0, heapId);
    }
    work->msgData = NewMsgDataFromNarc((MsgDataLoadType)1, (NarcId)0x1B, 0x135, (enum HeapID)work->heapId);
    work->messageFormat = MessageFormat_New((enum HeapID)work->heapId);
    FontID_Alloc(4, (enum HeapID)work->heapId);
    GfGfx_EngineATogglePlanes(4, 0);
}
