#include "to52_r7_overlay14_r52_internal.h"

typedef struct Ov14SpriteScale {
    void *sprite;
} Ov14SpriteScale;

typedef struct Ov14BoxWork {
    u8 padding[0x34];
    u8 *boxData;
} Ov14BoxWork;

void ov14_021F33FC(Ov14SpriteScale *work);
void ov14_021F3420(Ov14BoxWork *work, u32 enabled, u32 first, u32 end, u32 check);
void ov14_021F3488(Ov14BoxWork *work, u32 flags, u32 enabled);

void ov14_021F33FC(Ov14SpriteScale *work) {
    ManagedSprite_SetAffineTranslation(work->sprite, 0, 0);
    ManagedSprite_SetAffineScale(work->sprite, 1.0f, 1.0f);
    ManagedSprite_SetAffineOverwriteMode(work->sprite, 0);
}

void ov14_021F3420(Ov14BoxWork *work, u32 enabled, u32 first, u32 end, u32 check) {
    if (check == 1) {
        for (; first < end; first++) {
            if (ov14_021E6070(work, first, 6, 0) == 0) {
                ov14_021F2A9C(work->boxData, work->boxData[0x4094 + first], 1);
            } else {
                ov14_021F2A9C(work->boxData, work->boxData[0x4094 + first], 0);
            }
        }
    } else {
        for (; first < end; first++) {
            ov14_021F2A9C(work->boxData, work->boxData[0x4094 + first], enabled);
        }
    }
}

void ov14_021F3488(Ov14BoxWork *work, u32 flags, u32 enabled) {
    u32 check = (flags & 0x80) != 0;

    if (flags & 1) {
        ov14_021F3420(work, enabled, 0, 0x1e, check);
    }
    if (flags & 2) {
        ov14_021F3420(work, enabled, 0x1e, 0x24, check);
    }
}
