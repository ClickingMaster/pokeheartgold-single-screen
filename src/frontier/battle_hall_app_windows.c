#include "global.h"

#include "frontier/battle_hall_app.h"

#include "render_window.h"

static const WindowTemplate sBattleHallAppWindowTemplates[] = {
    { GF_BG_LYR_MAIN_0, 2,  19, 27, 4,  12, 1    },
    { GF_BG_LYR_MAIN_0, 10, 20, 9,  2,  13, 0x89 },
    { GF_BG_LYR_MAIN_1, 1,  0,  31, 22, 13, 1    },
    { GF_BG_LYR_SUB_0,  2,  19, 27, 4,  2,  0xA  },
};

void BattleHallApp_AddWindows(BgConfig *bgConfig, Window *windows) {
    u8 i;

    for (i = 0; i < 4; i++) {
        AddWindow(bgConfig, &windows[i], &sBattleHallAppWindowTemplates[i]);
        FillWindowPixelBuffer(&windows[i], 0);
    }
}

void BattleHallApp_RemoveWindows(Window *windows) {
    u16 i;

    for (i = 0; i < 4; i++) {
        RemoveWindow(&windows[i]);
    }
}

void BattleHallApp_DrawMessageFrame(Window *window, u32 frame) {
    LoadUserFrameGfx2(window->bgConfig, (GFBgLayer)GetWindowBgId(window), 0x3D9, 10, (u8)frame, HEAP_ID_105);
    FillWindowPixelBuffer(window, 15);
    DrawFrameAndWindow2(window, FALSE, 0x3D9, 10);
}

YesNoPrompt *BattleHallApp_CreateYesNo(enum HeapID heapId) {
    return YesNoPrompt_Create(heapId);
}

void BattleHallApp_DestroyYesNo(YesNoPrompt *prompt) {
    YesNoPrompt_Destroy(prompt);
}

#pragma push
#pragma opt_propagation off
void BattleHallApp_InitYesNo(YesNoPrompt *prompt, BgConfig *bgConfig, u8 ignoreTouch) {
    YesNoPromptTemplate template;

    MI_CpuFill8(&template, 0, sizeof(template));
    {
        int zero = 0;
        template.tileStart = 109;
        template.plttSlot = 14;
        template.bgConfig = bgConfig;
        template.bgId = zero;
        template.x = 24;
        template.y = 10;
        template.ignoreTouchFlag = ignoreTouch;
        template.initialCursorPos = 0;
        template.shapeParam = zero;
    }
    YesNoPrompt_InitFromTemplate(prompt, &template);
}
#pragma pop

u32 BattleHallApp_HandleYesNo(YesNoPrompt *prompt) {
    return YesNoPrompt_HandleInput(prompt);
}
