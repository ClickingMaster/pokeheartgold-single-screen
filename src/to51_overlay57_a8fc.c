#include "to51_overlay57_a8fc_private.h"

int ov57_0223A8FC(void *work) {
    Ov57A8FCWork *w = work;
    int result;
    YesNoPromptTemplate template;

    switch (w->state) {
    case 0:
        if (!ov57_022384C0(work)) {
            return 11;
        }
        PaletteData_BeginPaletteFade(w->paletteData, 2, 0x80B, 0, 0, 10, 0);
        PaletteData_BeginPaletteFade(w->paletteData, 8, 0xFFFF, 0, 0, 10, 0);
        ov57_0223B948(work, FALSE);
        GfGfx_EngineBTogglePlanes(1, FALSE);
        w->state++;
        break;
    case 1:
        if (PaletteData_GetSelectedBuffersBitmask(w->paletteData) != 0) {
            break;
        }
        PaletteData_SetAutoTransparent(w->paletteData, FALSE);
        ov57_022397B0(w->bgConfig, &w->window, 4, 2, 1, 27, 4, 58);
        MI_CpuFill8(&template, 0, sizeof(template));
        template.bgConfig = w->bgConfig;
        template.bgId = 4;
        template.tileStart = 230;
        template.plttSlot = 5;
        template.x = 25;
        template.y = 6;
        template.ignoreTouchFlag = w->touchMode;
        w->prompt = YesNoPrompt_Create(HEAP_ID_52);
        YesNoPrompt_InitFromTemplate(w->prompt, &template);
        ov57_02239728(&w->window, 4, 14, 0);
        w->state++;
        break;
    case 2:
        GfGfx_EngineBTogglePlanes(1, TRUE);
        w->state++;
        // fallthrough
    case 3:
        result = YesNoPrompt_HandleInput(w->prompt);
        if (result == 0) {
            break;
        }
        if (result != 1 && result != 2) {
            break;
        }
        w->touchMode = YesNoPrompt_IsInTouchMode(w->prompt);
        w->result = result;
        PaletteData_SetAutoTransparent(w->paletteData, TRUE);
        YesNoPrompt_Reset(w->prompt);
        YesNoPrompt_Destroy(w->prompt);
        ClearFrameAndWindow2(&w->window, FALSE);
        RemoveWindow(&w->window);
        w->state++;
        break;
    case 4:
        PaletteData_BeginPaletteFade(w->paletteData, 2, 0x80B, 0, 10, 0, 0);
        PaletteData_BeginPaletteFade(w->paletteData, 8, 0xFFFF, 0, 10, 0, 0);
        w->state++;
        break;
    case 5:
        if (PaletteData_GetSelectedBuffersBitmask(w->paletteData) != 0) {
            break;
        }
        switch (w->result) {
        case 0:
            goto result_default;
        case 1:
            ov57_0223B774(work);
            ov57_0223B7A8(work);
            ov57_022399F8(work);
            ov57_022383AC(work);
            ov57_02237F3C(work);
            ov57_022383D0(work, TRUE);
            result = 6;
            w->delay = 20;
            break;
        case 2:
            result = 4;
            break;
        default:
result_default:
            result = 7;
            break;
        }
        ov57_0223B948(work, TRUE);
        w->state = 0;
        return result;
    }
    return 7;
}
