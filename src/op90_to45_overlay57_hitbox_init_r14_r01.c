#pragma force_active on
#include "global.h"
#include "touchscreen.h"
typedef struct HitboxSlot {
    TouchscreenHitbox *hitbox;
    u8 unk04[0xC];
} HitboxSlot;
typedef struct Ov57HitboxWork {
    u8 unk000[0x1EC];
    void *controller;
    TouchscreenHitbox hitboxes[21];
    u8 unk244[0x44];
    HitboxSlot slots[21];
} Ov57HitboxWork;
extern TouchscreenHitbox ov57_0223BEF4[];
extern u8 ov57_0223AEA8[];
void *TouchHitboxController_Create(const TouchscreenHitbox *hitboxes, int count, const void *callbacks, void *arg, u32 heapId);
void ov57_0223BB84(Ov57HitboxWork *work);

// @fn ov57_0223BB84
void ov57_0223BB84(Ov57HitboxWork *work) {
    const TouchscreenHitbox empty = { 0 };
    int i;
    for (i = 0; i < 13; i++) {
        work->hitboxes[i] = ov57_0223BEF4[i];
    }
    for (; i < 21; i++) {
        work->hitboxes[i] = empty;
        work->slots[i].hitbox = &work->hitboxes[i];
    }
    work->controller = TouchHitboxController_Create(work->hitboxes, 21, ov57_0223AEA8, work, 52);
}
