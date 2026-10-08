#pragma force_active on
#include "global.h"
#include "touch_hitbox_controller.h"
#include "touchscreen.h"

typedef struct Ov40Entry {
    TouchscreenHitbox *hitbox;
    u8 unk4[0x18];
} Ov40Entry;

typedef struct Ov40Unk {
    u8 pad0[0x10];
    Ov40Entry entries[12];
    u8 pad160[0xD4];
    TouchHitboxController *controller;
    TouchscreenHitbox hitboxes[24];
} Ov40Unk;

typedef struct Ov40HitboxDef {
    s16 x;
    s16 y;
    s16 halfWidth;
    s16 halfHeight;
} Ov40HitboxDef;

typedef struct Ov40HitboxDefs {
    Ov40HitboxDef defs[12];
} Ov40HitboxDefs;

extern const Ov40HitboxDefs ov40_02245C40;

void ov40_022437C0(u32 index, u32 event, void *arg);
void ov40_022436D4(Ov40Unk *data);

// @fn ov40_022436D4
void ov40_022436D4(Ov40Unk *data) {
    int i;
    Ov40HitboxDefs defs;

    for (i = 0; i < 12; i++) {
        data->entries[i].hitbox = &data->hitboxes[i];
    }
    defs = ov40_02245C40;
    for (; i < 24; i++) {
        data->hitboxes[i].rect.top = defs.defs[i - 12].y - defs.defs[i - 12].halfHeight;
        data->hitboxes[i].rect.left = defs.defs[i - 12].x - defs.defs[i - 12].halfWidth;
        data->hitboxes[i].rect.bottom = defs.defs[i - 12].y + defs.defs[i - 12].halfHeight;
        data->hitboxes[i].rect.right = defs.defs[i - 12].x + defs.defs[i - 12].halfWidth;
    }
    data->hitboxes[22].rect.top = 152;
    data->hitboxes[22].rect.left = 32;
    data->hitboxes[22].rect.bottom = 184;
    data->hitboxes[22].rect.right = 128;
    data->hitboxes[23].rect.top = 152;
    data->hitboxes[23].rect.left = 128;
    data->hitboxes[23].rect.bottom = 184;
    data->hitboxes[23].rect.right = 224;
    data->controller = TouchHitboxController_Create(data->hitboxes, 24, ov40_022437C0, data, HEAP_ID_109);
}
