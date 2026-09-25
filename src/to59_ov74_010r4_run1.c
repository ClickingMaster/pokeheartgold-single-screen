#include "global.h"
#include "main.h"
#include "title_screen.h"
#include "pm_string.h"
#include "text.h"
#include "render_text.h"
#include "system.h"
#include "constants/sndseq.h"
#include "to58_overlay74_010_private.h"
#include "yes_no_prompt.h"
#include "sprite.h"
#include "gf_gfx_planes.h"
extern void ov74_02236034(int);
extern void GF_RunVramTransferTasks(void);
extern void OamManager_ApplyAndResetBuffers(void);
extern void ov74_02232B18(u8 *);
extern void ov74_02231A1C(u8 *, u8 *, int);
extern u32 ov74_0223C9B8[];
extern void ov74_02232F5C(u8 *);
extern void PlaySE(u32);

void ov74_02232F5C(u8 *work);

void ov74_02232F5C(u8 *work) {
    YesNoPromptTemplate template;
    template.bgConfig = *(BgConfig **)(work + 0x20);
    template.bgId = 0;
    template.tileStart = 0x200;
    template.plttSlot = 8;
    template.x = 25;
    template.y = 7;
    template.shapeParam = 0;
    template.initialCursorPos = 1;
    YesNoPrompt_InitFromTemplate(*(YesNoPrompt **)(work + 0xE88C), &template);
}
