#include "to52_r7_overlay14_r52_internal.h"

typedef struct Ov14SpriteScale {
    void *sprite;
} Ov14SpriteScale;

typedef struct Ov14BoxWork {
    u8 padding[0x34];
    u8 *boxData;
} Ov14BoxWork;

void PCBox_ResetSpriteAffine(Ov14SpriteScale *work);
void PCBox_SetIconRangeTranslucent(Ov14BoxWork *work, u32 enabled, u32 first, u32 end, u32 check);
void PCBox_SetIconsTranslucent(Ov14BoxWork *work, u32 flags, u32 enabled);

void PCBox_ResetSpriteAffine(Ov14SpriteScale *work) {
    ManagedSprite_SetAffineTranslation(work->sprite, 0, 0);
    ManagedSprite_SetAffineScale(work->sprite, 1.0f, 1.0f);
    ManagedSprite_SetAffineOverwriteMode(work->sprite, 0);
}

void PCBox_SetIconRangeTranslucent(Ov14BoxWork *work, u32 enabled, u32 first, u32 end, u32 check) {
    if (check == 1) {
        for (; first < end; first++) {
            if (PCBox_GetSlotMonData(work, first, 6, 0) == 0) {
                PCBox_SetSpriteTranslucent(work->boxData, work->boxData[0x4094 + first], 1);
            } else {
                PCBox_SetSpriteTranslucent(work->boxData, work->boxData[0x4094 + first], 0);
            }
        }
    } else {
        for (; first < end; first++) {
            PCBox_SetSpriteTranslucent(work->boxData, work->boxData[0x4094 + first], enabled);
        }
    }
}

void PCBox_SetIconsTranslucent(Ov14BoxWork *work, u32 flags, u32 enabled) {
    u32 check = (flags & 0x80) != 0;

    if (flags & 1) {
        PCBox_SetIconRangeTranslucent(work, enabled, 0, 0x1e, check);
    }
    if (flags & 2) {
        PCBox_SetIconRangeTranslucent(work, enabled, 0x1e, 0x24, check);
    }
}
