#include "overlay_2/fashion_case_gifts.h"

#include "fashion_case.h"
#include "math_util.h"
#include "field/field_motion_blur.h"
#include "field/map_object_render.h"
#include "field/field_follow_mon.h"
#include "screen_fade.h"
#include "script_pokemon_util.h"
#include "map_object_movement_commands.h"
#include "field_effects.h"

static const u8 sGiftAccessories[] = { 47, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 95, 96 };

BOOL FashionCase_GiftAccessoriesFull(SaveData *saveData) {
    u8 i;
    FashionCase *fashionCase = Save_FashionData_GetFashionCase(Save_FashionData_Get(saveData));

    for (i = 0; i < 39; i++) {
        if (FashionCase_CanGiveAccessory(fashionCase, sGiftAccessories[i], 1) == TRUE) {
            return FALSE;
        }
    }

    return TRUE;
}

u8 FashionCase_GetRandomGiftAccessory(void) {
    u8 rand = LCRandom() % 39;
    return sGiftAccessories[rand];
}

BOOL FashionCase_HasAllBackdrops(SaveData *saveData) {
    FashionCase *fashionCase = Save_FashionData_GetFashionCase(Save_FashionData_Get(saveData));

    for (u8 i = 0; i < 14; i++) {
        if (FashionCase_HasWallpaper(fashionCase, i) == FALSE) {
            return FALSE;
        }
    }

    return TRUE;
}

u8 FashionCase_GetRandomMissingBackdrop(SaveData *saveData) {
    u8 cnt = 0;
    u8 data[14];

    for (u8 i = 0; i < 14; i++) {
        data[i] = 0;
    }

    FashionCase *fashionCase = Save_FashionData_GetFashionCase(Save_FashionData_Get(saveData));

    for (u8 i = 0; i < 14; i++) {
        if (FashionCase_HasWallpaper(fashionCase, i) == 0) {
            data[cnt] = i;
            cnt++;
        }
    }

    if (cnt == 0) {
        GF_ASSERT(FALSE);
        return 0;
    }

    u8 rand = LCRandom() % cnt;
    return data[rand];
}
