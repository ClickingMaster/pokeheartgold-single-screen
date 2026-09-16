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

void ov12_02261E40(SysTask *task, s32 *arg1) {
    s32 temp_r0;

    temp_r0 = *arg1;
    switch (temp_r0) {
    case 0:
        if (IsBrightnessTransitionActive(1) == 0) {
            *arg1 = 2;
            return;
        }
        StartBrightnessTransition(4, 0x10, 0, (GXBlendPlaneMask)0x3D, 1);
        *arg1 += 1;
        return;
    case 1:
        if (IsBrightnessTransitionActive(1) == 1) {
            StartBrightnessTransition(4, 0, 0x10, (GXBlendPlaneMask)0x3D, 1);
            *arg1 += 1;
            return;
        }
        return;
    case 2:
        if (IsBrightnessTransitionActive(1) == 1) {
            Heap_Free(arg1);
            SysTask_Destroy(task);
        }
        break;
    }
}

void ov12_02261EB8(BattleSystem *bsys) {
    ov12_0223BFFC(bsys, 1);
    BattleController_TryEmitExitRecording(bsys, BattleSystem_GetBattleContext(bsys));
}

void ov12_02261ED4(BattleSystem *bsys) {
    ov12_0223BFFC(bsys, 2);
    BattleController_TryEmitExitRecording(bsys, BattleSystem_GetBattleContext(bsys));
}

u8 ov12_02261EF0(BattleSystem *bsys, s32 battlerId, u8 trainerClass) {
    s32 profileVersion;
    u8 newTrainerClass;

    newTrainerClass = trainerClass;
    if (!(BattleSystem_GetBattleType(bsys) & 4) || ((newTrainerClass != 0) && (newTrainerClass != 1))) {
        return newTrainerClass;
    }

    profileVersion = PlayerProfile_GetVersion(BattleSystem_GetPlayerProfile(bsys, battlerId));
    switch (profileVersion) {
    case 0:
        newTrainerClass += 0x7D;
        break;
    case 12:
        newTrainerClass += 0x7F;
        break;
    }
    return newTrainerClass;
}
