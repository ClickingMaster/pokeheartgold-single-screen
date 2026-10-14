#include "global.h"

#include "frontier/frontier_script_ui.h"

#include "sys_task_api.h"

// Tasks of the frontier map's shake (FrtCmd_ShakeMap) and window (FrtCmd_SetWindowDelayed) commands, and the menu alignment flags
// (FrtCmd_MenuSetAlignRight/MenuSetAlignBottom).

#pragma force_active on
void FrontierMap_ShakeTask(SysTask *task, FrontierMapShake *shake) {
    shake->frame++;
    if (shake->frame > shake->interval) {
        shake->frame = 0;
        shake->count--;
        if (shake->count < 0) {
            SysTask_Destroy(task);
            MI_CpuFill8(shake, 0, sizeof(FrontierMapShake));
            return;
        }
        shake->x = -shake->x;
        shake->y = -shake->y;
    }
}

static inline void SetWindow0Rect(int left, int top, int right, int bottom) {
    reg_G2_WIN0H = ((left << 8) & 0xFF00) | right;
    reg_G2_WIN0V = ((top << 8) & 0xFF00) | bottom;
}

static inline void SetWindow0Inside(int planes, int effect) {
    volatile u16 *reg = &reg_G2_WININ;
    int value = (reg[0] & ~0x3F) | planes;
    if (effect) {
        value |= 0x20;
    }
    reg[0] = value;
}

static inline void SetWindowOutside(int planes, int effect) {
    volatile u16 *reg = &reg_G2_WINOUT;
    int value = (reg[0] & ~0x3F) | planes;
    if (effect) {
        value |= 0x20;
    }
    reg[0] = value;
}

static inline void SetVisibleWindows(u32 windows) {
    reg_GX_DISPCNT = (reg_GX_DISPCNT & 0xFFFF1FFF) | windows;
}

void FrontierMap_WindowTask(SysTask *task, FrontierMapWindowTask *data) {
    if (data->delay <= 0) {
        if (data->enable == TRUE) {
            SetWindow0Inside(0xF, TRUE);
            SetWindowOutside(0x1F, TRUE);
            SetWindow0Rect(data->left, data->top, data->right, data->bottom);
            SetVisibleWindows(REG_GX_DISPCNT_W0_MASK);
        } else {
            SetVisibleWindows(0);
        }
        SysTask_Destroy(task);
        MI_CpuFill8(data, 0, sizeof(FrontierMapWindowTask));
    } else {
        data->delay--;
    }
}

void FrontierMenu_SetAlignRight(FrontierMenu *menu, int value) {
    menu->alignRight = (u8)value;
}

void FrontierMenu_SetAlignBottom(FrontierMenu *menu, int value) {
    menu->alignBottom = (u8)value;
}
#pragma force_active reset
