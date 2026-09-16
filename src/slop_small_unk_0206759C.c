#include "global.h"

#include "player_data.h"
#include "save_trainer_card.h"

void sub_0206759C(SaveData *saveData, int a1);

void sub_0206759C(SaveData *saveData, int a1) {
    int *arr;
    PlayerProfile *profile;
    u8 i;
    if (a1 <= 0) {
        return;
    }
    arr = TrainerCard_GetBadgeShininessArr(Save_TrainerCard_Get(saveData));
    profile = Save_PlayerData_GetProfile(saveData);
    for (i = 0; i < 8; i++) {
        int shininess;
        if (!PlayerProfile_TestBadgeFlag(profile, i)) {
            continue;
        }
        shininess = GetShininessOfBadgeI(i, arr);
        if (shininess > 0 && shininess < 0xc8) {
            shininess = shininess - a1 * 10;
        } else {
            GF_ASSERT(shininess < 0xc8);
            shininess = 0;
        }
        if (shininess < 0) {
            shininess = 0;
        }
        SetShininessOfBadgeI(i, shininess, arr);
    }
}
