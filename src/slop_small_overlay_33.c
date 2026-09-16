#include "global.h"

#include "bg_window.h"
#include "sys_task.h"

typedef struct Ov33Config {
    const WindowTemplate *windowTemplates;
    const void *hitboxes;
    u8 unk8;
    u8 unk9;
    u16 unkA;
} Ov33Config;

typedef struct Ov33Work {
    BgConfig *bgConfig;
    u8 pad04[0x1b8 - 4];
    u16 state;
    u16 count;
    u32 touchedHitbox;
} Ov33Work;

extern const Ov33Config ov33_0225DA50[];
void ov33_0225D9D4(Ov33Work *work, u8 palette);

void ov33_0225D9D4(Ov33Work *work, u8 palette) {
    const Ov33Config *config = &ov33_0225DA50[work->count - 1];
    BgTilemapRectChangePalette(work->bgConfig, 5, config->unk8, (u8)(config->unk9 + (config->unkA + 6) * work->touchedHitbox), 0x20, 6, palette);
    ScheduleBgTilemapBufferTransfer(work->bgConfig, 5);
}
