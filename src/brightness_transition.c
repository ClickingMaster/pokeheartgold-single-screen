#include "brightness_transition_internal.h"

// Master brightness transitions (fade the whole screen to white or black over a number of frames) and the per-scanline
// window bounds of the window fade effects (ARM9 static 0x02010AB0-0x02010EC8).

void sub_02010AB0(s32 *starts, s32 *ends, s32 *deltas, const u8 *startBytes, const u8 *endBytes, s32 steps) {
    starts[0] = startBytes[0] << 7;
    starts[1] = startBytes[1] << 7;
    starts[2] = startBytes[2] << 7;
    starts[3] = startBytes[3] << 7;
    ends[0] = endBytes[0];
    ends[1] = endBytes[1];
    ends[2] = endBytes[2];
    ends[3] = endBytes[3];
    deltas[0] = sub_02010A6C(startBytes[0], endBytes[0], steps);
    deltas[1] = sub_02010A6C(startBytes[1], endBytes[1], steps);
    deltas[2] = sub_02010A6C(startBytes[2], endBytes[2], steps);
    deltas[3] = sub_02010A6C(startBytes[3], endBytes[3], steps);
}

void BrightnessTransition_Start(BrightnessController *ctl, BOOL direction) {
    s32 start;
    s32 end;
    BrightnessTransition *transition = Heap_Alloc(ctl->heapId, sizeof(BrightnessTransition));
    ctl->transition = transition;
    memset(transition, 0, sizeof(BrightnessTransition));
    transition = ctl->transition;
    if (!direction) {
        if (ctl->setting == 0x7fff) {
            start = 0x10;
            end = 0;
        } else if (ctl->setting == 0) {
            start = -0x10;
            end = 0;
        } else {
            start = -0x10;
            end = 0;
            GF_AssertFail();
        }
    } else {
        if (ctl->setting == 0x7fff) {
            start = 0;
            end = 0x10;
        } else if (ctl->setting == 0) {
            start = 0;
            end = -0x10;
        } else {
            start = 0;
            end = -0x10;
            GF_AssertFail();
        }
    }
    SetMasterBrightness(ctl->screen, start);
    transition->steps = ctl->duration;
    transition->delay = ctl->target;
    transition->counter = 0;
    transition->current = start << 7;
    transition->target = end << 7;
    transition->delta = sub_02010A6C(start, end, ctl->duration);
    transition->screen = ctl->screen;
    ctl->state++;
}

BOOL BrightnessTransition_Run(BrightnessController *ctl) {
    BOOL done = FALSE;
    BrightnessTransition *transition = ctl->transition;
    switch (ctl->state) {
    case 1:
        if (BrightnessTransition_Step(transition) == TRUE) {
            ctl->state++;
        }
        break;
    case 2:
        Heap_Free(transition);
        ctl->transition = NULL;
        done = TRUE;
        ctl->state++;
        break;
    case 3:
        done = TRUE;
        break;
    }
    return done;
}

#pragma force_active on

BOOL BrightnessTransition_Step(BrightnessTransition *transition) {
    BOOL done = FALSE;
    if (++transition->counter >= transition->delay) {
        transition->counter = 0;
        if (transition->steps - 1 > 0) {
            transition->steps--;
            transition->current += transition->delta;
        } else {
            transition->current = transition->target;
            done = TRUE;
        }
        SetMasterBrightness(transition->screen, transition->current / 128);
    }
    return done;
}

// Writes one scanline's horizontal window bounds (window 0 or 1 of the buffer, main or sub screen) during HBlank.
static inline void ApplyWindowScanline(ScanlineWindowBuffer *buffer, int line, ScanlineBufferController *ctl) {
    u32 screen = ctl->screen;
    s16 right = buffer->right[line];
    s16 left = buffer->left[line];
    if (!buffer->window) {
        if (!screen) {
            if (reg_GX_DISPSTAT & REG_GX_DISPSTAT_HBLK_MASK) {
                reg_G2_WIN0H = ((left << 8) & 0xFF00) | (u8)right;
                reg_G2_WIN0V = 192;
            }
        } else {
            if (reg_GX_DISPSTAT & REG_GX_DISPSTAT_HBLK_MASK) {
                reg_G2S_DB_WIN0H = ((left << 8) & 0xFF00) | (u8)right;
                reg_G2S_DB_WIN0V = 192;
            }
        }
    } else {
        if (!screen) {
            if (reg_GX_DISPSTAT & REG_GX_DISPSTAT_HBLK_MASK) {
                reg_G2_WIN1H = ((left << 8) & 0xFF00) | (u8)right;
                reg_G2_WIN1V = 192;
            }
        } else {
            if (reg_GX_DISPSTAT & REG_GX_DISPSTAT_HBLK_MASK) {
                reg_G2S_DB_WIN1H = ((left << 8) & 0xFF00) | (u8)right;
                reg_G2S_DB_WIN1V = 192;
            }
        }
    }
}

// HBlank callback: applies the next scanline's window bounds from one or both buffers.
void ScanlineWindow_HBlankCallback(ScanlineBufferController *ctl) {
    int line;
    if (ctl == NULL) {
        GF_AssertFail();
    }
    line = reg_GX_VCOUNT;
    if (line < 192) {
        line++;
        if (line > 191) {
            line -= 192;
        }
        if (ctl->count == 1) {
            ApplyWindowScanline(sub_02010EE0(ctl, 0), line, ctl);
        } else {
            ApplyWindowScanline(sub_02010EE0(ctl, 0), line, ctl);
            ApplyWindowScanline(sub_02010EE0(ctl, 1), line, ctl);
        }
    }
}

#pragma force_active off

void ScanlineBufferController_Init(ScanlineBufferController *ctl, int mode, int screen, u32 heapId) {
    int i;
    switch (mode) {
    case 0:
    case 1:
        ctl->buffer = Heap_Alloc(heapId, 0x604);
        ctl->count = 1;
        ctl->screen = screen;
        *(s32 *)((u8 *)ctl->buffer + 0x600) = mode;
        break;
    case 2:
        ctl->buffer = Heap_Alloc(heapId, 0xc08);
        ctl->count = 2;
        ctl->screen = screen;
        for (i = 0; i < 2; i++) {
            *(s32 *)((u8 *)ctl->buffer + i * 0x604 + 0x600) = i;
        }
        break;
    }
}
