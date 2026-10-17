#include "global.h"

void MoveRelearner_DimSubScreen(int on);

// Darkens the bottom screen behind the move relearner's yes/no prompt (on: 1) or restores it (on: 0).

void MoveRelearner_DimSubScreen(int on) {
    if (on == 1) {
        G2S_SetBlendBrightness(GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ, -8);
    } else {
        G2S_BlendNone();
    }
}
