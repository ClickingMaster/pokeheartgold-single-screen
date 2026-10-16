#include "global.h"

#include "sprite.h"
#include "union_chat_log.h"

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
