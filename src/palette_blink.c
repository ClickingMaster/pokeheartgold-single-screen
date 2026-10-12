#include "palette_blink.h"

#include "global.h"

#include "bg_window.h"
#include "palette.h"
#include "sys_task.h"
#include "systask_environment.h"
#include "palette_vram_addr.h"

typedef void (*PaletteBlinkApplyFunc)(PaletteBlink *blink, const u16 *colors);

enum PaletteBlinkState {
    PALETTE_BLINK_RESTART,
    PALETTE_BLINK_RUN,
    PALETTE_BLINK_PAUSE,
    PALETTE_BLINK_END,
};

struct PaletteBlink {
    SysTask *task;
    PaletteBlinkApplyFunc apply;
    PaletteData *plttData; // NULL: BG palette RAM
    u16 source[16];
    u16 current[16];
    u8 screen;
    u8 palette;
    u8 state;
    u8 timer; // 0-31
};

void PaletteBlink_Task(SysTask *task, void *data);
BOOL PaletteBlink_UpdateColors(PaletteBlink *blink);
void PaletteBlink_LoadMainBgPltt(PaletteBlink *blink, const u16 *colors);
void PaletteBlink_LoadSubBgPltt(PaletteBlink *blink, const u16 *colors);
void PaletteBlink_LoadMainBgBuffer(PaletteBlink *blink, const u16 *colors);
void PaletteBlink_LoadSubBgBuffer(PaletteBlink *blink, const u16 *colors);

PaletteBlink *PaletteBlink_New(PaletteData *plttData, PMLCDTarget screen, u32 palette, enum HeapID heapID) {
    SysTask *task = CreateSysTaskAndEnvironment(PaletteBlink_Task, sizeof(PaletteBlink), 0, heapID);
    PaletteBlink *blink = SysTask_GetData(task);
    u16 *source;

    if (plttData != NULL) {
        if (screen == PM_LCD_TOP) {
            source = PaletteData_GetUnfadedBuf(plttData, PLTTBUF_MAIN_BG);
            blink->apply = PaletteBlink_LoadMainBgBuffer;
        } else {
            source = PaletteData_GetUnfadedBuf(plttData, PLTTBUF_SUB_BG);
            blink->apply = PaletteBlink_LoadSubBgBuffer;
        }
    } else {
        if (screen == PM_LCD_TOP) {
            source = GetMainBgPlttAddr();
            blink->apply = PaletteBlink_LoadMainBgPltt;
        } else {
            source = GetSubBgPlttAddr();
            blink->apply = PaletteBlink_LoadSubBgPltt;
        }
    }
    MIi_CpuCopy16(source + palette * 16, blink->source, sizeof(blink->source));
    MIi_CpuCopy16(source + palette * 16, blink->current, sizeof(blink->current));
    blink->task = task;
    blink->plttData = plttData;
    blink->screen = screen;
    blink->palette = palette;
    blink->state = PALETTE_BLINK_RUN;
    blink->timer = 0;
    return blink;
}

void PaletteBlink_SetState(void *blink_, s32 command) {
    PaletteBlink *blink = blink_;

    switch (command) {
    case 0:
        blink->state = PALETTE_BLINK_RESTART;
        break;
    case 1:
        blink->state = PALETTE_BLINK_PAUSE;
        break;
    case 2:
        blink->state = PALETTE_BLINK_END;
        break;
    }
}

void PaletteBlink_Delete(PaletteBlink *blink) {
    DestroySysTaskAndEnvironment(blink->task);
}

void PaletteBlink_Task(SysTask *task, void *data) {
    PaletteBlink *blink = data;

    switch (blink->state) {
    case PALETTE_BLINK_RESTART:
        blink->timer = 0;
        blink->state = PALETTE_BLINK_RUN;
        break;
    case PALETTE_BLINK_RUN:
        if (PaletteBlink_UpdateColors(blink) == TRUE) {
            blink->apply(blink, blink->current);
        }
        blink->timer++;
        if (blink->timer == 32) {
            blink->timer = 0;
        }
        break;
    case PALETTE_BLINK_PAUSE:
        break;
    case PALETTE_BLINK_END:
        blink->apply(blink, blink->source);
        DestroySysTaskAndEnvironment(task);
        break;
    }
}

// colors 11-14 show their own color for 24 frames, then color 15 for 8
BOOL PaletteBlink_UpdateColors(PaletteBlink *blink) {
    u32 i;

    if (blink->timer == 0) {
        for (i = 0; i < 16; i++) {
            if ((1 << i) & 0x7800) {
                blink->current[i] = blink->source[i];
            }
        }
        return TRUE;
    }
    if (blink->timer == 24) {
        for (i = 0; i < 16; i++) {
            if ((1 << i) & 0x7800) {
                blink->current[i] = blink->source[15];
            }
        }
        return TRUE;
    }
    return FALSE;
}

void PaletteBlink_LoadMainBgPltt(PaletteBlink *blink, const u16 *colors) {
    BG_LoadPlttData(GF_PAL_LOCATION_MAIN_BG, colors, 32, blink->palette * 32);
}

void PaletteBlink_LoadSubBgPltt(PaletteBlink *blink, const u16 *colors) {
    BG_LoadPlttData(GF_PAL_LOCATION_SUB_BG, colors, 32, blink->palette * 32);
}

void PaletteBlink_LoadMainBgBuffer(PaletteBlink *blink, const u16 *colors) {
    PaletteData_LoadPalette(blink->plttData, colors, PLTTBUF_MAIN_BG, blink->palette * 16, 32);
}

void PaletteBlink_LoadSubBgBuffer(PaletteBlink *blink, const u16 *colors) {
    PaletteData_LoadPalette(blink->plttData, colors, PLTTBUF_SUB_BG, blink->palette * 16, 32);
}
