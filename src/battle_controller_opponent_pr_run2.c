#include "global.h"

#include "battle/battle_controller_opponent.h"
#include "battle/battle_input.h"
#include "battle/battle_system.h"
#include "battle/overlay_12_0224E4FC.h"

#include "brightness.h"
#include "pokemon.h"
#include "pokepic.h"
#include "sound.h"
#include "unk_02013FDC.h"

Pokepic *ov12_022612A4(BattleSystem *bsys, PokepicManager *pokepicManager, PokepicTemplate *template, s32 xPos, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9, s32 index, PokepicAnimScript *animScript, PokepicCallback callback);
u8 ov12_02261EF0(BattleSystem *bsys, s32 battlerId, u8 trainerClass);
void ov12_02261B2C(BattleSystem *bsys, s32 arg1, int trainerClass, int resTag, BOOL isLink, s16 x, s16 y);
void ov12_02261E40(SysTask *task, s32 *arg1);
void ov12_02261EB8(BattleSystem *bsys);
void ov12_02261ED4(BattleSystem *bsys);

void ov12_02261B2C(BattleSystem *bsys, s32 arg1, int trainerClass, int resTag, BOOL isLink, s16 x, s16 y) {
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    PaletteData *paletteData;
    int battlerPosition;

    spriteSystem = BattleSystem_GetSpriteSystem(bsys);
    spriteManager = BattleSystem_GetSpriteManager(bsys);
    paletteData = BattleSystem_GetPaletteData(bsys);
    if (1 & arg1) {
        battlerPosition = 2;
    } else {
        battlerPosition = 0;
    }
    sub_02070C24(spriteSystem, spriteManager, paletteData, x, y, trainerClass, battlerPosition, isLink, resTag, HEAP_ID_BATTLE);
}
