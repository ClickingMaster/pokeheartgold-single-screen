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

Pokepic *ov12_022612A4(BattleSystem *bsys, PokepicManager *pokepicManager, PokepicTemplate *template, s32 xPos, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9, s32 index, PokepicAnimScript *animScript, PokepicCallback callback) {
    u8 *sp20;
    s32 sp1C;
    s32 x;
    PokepicManager *ppm;
    s32 temp_r0;
    Pokepic *pokepic;
    s32 whichFacing;
    s32 opponentFlags;

    ppm = pokepicManager;
    x = xPos;
    sp20 = ov12_0223BB94(ov12_0223A99C(bsys), index);
    opponentFlags = ov12_0223AB0C(bsys, index);
    temp_r0 = 1 & opponentFlags;
    sp1C = temp_r0;
    if (temp_r0 != 0) {
        whichFacing = 2;
    } else {
        whichFacing = 0;
    }
    sub_02014540((NarcId)template->narcID, template->charDataID, HEAP_ID_BATTLE, sp20, template->personality, 0, whichFacing, (s32)template->species);
    ov12_0223BBA8(ov12_0223A99C(bsys), index, template->narcID);
    ov12_0223BBC0(ov12_0223A99C(bsys), index, template->palDataID);
    ov12_0223BBD8(ov12_0223A99C(bsys), index, arg6);
    pokepic = PokepicManager_CreatePokepicAt(ppm, template, x, arg4 + arg6, arg5, index, index, animScript, callback);
    if (sp1C != 0) {
        if (opponentFlags > 1) {
            opponentFlags = opponentFlags >> 1;
        }
        Pokepic_SetAttr(pokepic, 0x2A, opponentFlags);
        Pokepic_SetAttr(pokepic, 0x2E, arg9);
        Pokepic_SetAttr(pokepic, 0x14, arg4 + 0x24);
        Pokepic_SetAttr(pokepic, 0x15, arg8);
        Pokepic_SetAttr(pokepic, 0x16, 0x24 - arg6);
        Pokepic_SetAttr(pokepic, 0x29, arg7);
    }
    return pokepic;
}
