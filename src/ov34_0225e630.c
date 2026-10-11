#include "global.h"

#include "sprite.h"
#include "union_chat_log.h"

// The last function of the Union Room chat log (union_chat_log.c); its object is linked after the
// overlay's data (asm/union_chat_log_data.s).

// force_active only sets a section flag here; kept so the object stays identical.
#pragma force_active on

// An arrow pressed this frame stops animating; once released it restarts from frame 1.
void UnionChatLog_UpdateArrows(UnionChatLogData *data) {
    int i;
    for (i = 0; i < UNION_CHAT_LOG_NUM_ARROWS; i++) {
        Sprite_GetAnimationFrame(data->sprites[i]);
        if (data->arrowPressed[i] == TRUE) {
            Sprite_SetAnimActiveFlag(data->sprites[i], FALSE);
            data->arrowPressed[i] = FALSE;
        } else if (!Sprite_GetAnimActiveFlag(data->sprites[i])) {
            Sprite_SetAnimActiveFlag(data->sprites[i], TRUE);
            Sprite_SetAnimationFrame(data->sprites[i], 1);
        }
    }
}
