#pragma force_active on
#include "global.h"
#include "sprite_system.h"
#include "battle/battle_system.h"

typedef struct W13Work {
    BattleSystem **root;
    u8 pad[0x308];
    SpriteManager *manager;
} W13Work;

typedef struct Entry {
    int value;
    int unused[4];
} Entry;
extern const Entry ov08_02225DEC[], ov08_02225DDC[], ov08_02225DE0[], ov08_02225DE4[], ov08_02225DE8[];
ManagedSprite *ov08_02223EA4(W13Work *work, int index);

// @fn ov08_02223EA4
ManagedSprite *ov08_02223EA4(W13Work *work, int index) {
    ManagedSpriteTemplate tmpl;
    SpriteSystem *system = BattleSystem_GetSpriteSystem(*work->root);
    tmpl.x = 0;
    tmpl.y = 0;
    tmpl.z = 0;
    tmpl.animation = 0;
    tmpl.drawPriority = ov08_02225DEC[index].value;
    tmpl.pal = 0;
    tmpl.vram = NNS_G2D_VRAM_TYPE_2DSUB;
    tmpl.resIdList[0] = ov08_02225DDC[index].value;
    tmpl.resIdList[1] = ov08_02225DE0[index].value;
    tmpl.resIdList[2] = ov08_02225DE4[index].value;
    tmpl.resIdList[3] = ov08_02225DE8[index].value;
    tmpl.bgPriority = 1;
    tmpl.vramTransfer = 0;
    return SpriteSystem_NewSprite(system, work->manager, &tmpl);
}
