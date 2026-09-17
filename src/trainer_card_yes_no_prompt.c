#include "trainer_card_yes_no_prompt_private.h"
#include "yes_no_prompt.h"

void ov51_021E7804(u8 *state, int unused) {
    YesNoPromptTemplate template;
    MI_CpuFill8(&template, 0, sizeof(template));
    template.bgConfig = *(BgConfig **)state;
    template.bgId = 4;
    template.tileStart = 0x3A5;
    template.plttSlot = 0xB;
    template.x = 0x1A;
    template.y = 6;
    template.ignoreTouchFlag = *(u32 *)(state + 0x310C);
    template.initialCursorPos = 0;
    YesNoPrompt_InitFromTemplate(*(YesNoPrompt **)(state + 0x33C0), &template);
    ScheduleBgTilemapBufferTransfer(*(BgConfig **)state, 4);
}
