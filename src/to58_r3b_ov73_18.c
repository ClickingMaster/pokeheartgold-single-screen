#include "global.h"

typedef struct {
    void *bgConfig;
    int bgId;
    int tileStart;
    u32 plttSlot;
    u8 x;
    u8 y;
    u8 ignoreTouchFlag : 4;
    u8 initialCursorPos : 4;
    u8 shapeParam;
} Ov73YesNoTemplate;

extern void *YesNoPrompt_Create(u32);
extern void YesNoPrompt_InitFromTemplate(void *, const Ov73YesNoTemplate *);
void *ov73_021E8390(void *, u8, int);

void *ov73_021E8390(void *bgConfig, u8 y, int tileStart) {
    Ov73YesNoTemplate template;
    void *prompt = YesNoPrompt_Create(150);
    template.bgConfig = bgConfig;
    template.bgId = 0;
    template.tileStart = tileStart;
    template.plttSlot = 8;
    template.x = 23;
    template.y = y;
    template.ignoreTouchFlag = 0;
    template.initialCursorPos = 0;
    template.shapeParam = 0;
    YesNoPrompt_InitFromTemplate(prompt, &template);
    return prompt;
}
