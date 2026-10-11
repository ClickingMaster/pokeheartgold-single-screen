// BattleDpadCursor_HandleInput, the last function of battle_dpad_cursor: a separate object because overlay 8's
// rodata object is linked between it and the rest of the module.
#include "global.h"

#include "constants/sndseq.h"

#include "battle/battle_cursor.h"
#include "battle/battle_dpad_cursor.h"

#include "sound_playback.h"
#include "system.h"

#pragma force_active on

// Returns the selected box on A, -2 on B and -1 otherwise. The d-pad moves to the neighbour in that direction,
// skipping boxes that are not enabled; a neighbour flagged as a return link (bit 7) leads back to the previous box.
int BattleDpadCursor_HandleInput(BattleDpadCursor *cursor) {
    u8 input;
    u8 direction;
    u8 neighborIsEnabled;
    u8 next;
    u8 x, y, w, h;

    if (BattleDpadCursor_TryActivate(cursor) == FALSE) {
        return -1;
    }
    if (gSystem.newKeys & PAD_KEY_UP) {
        input = DpadMenuBox_GetNeighborInDirection(cursor->boxes, NULL, NULL, NULL, NULL, cursor->position, 0);
        direction = 0;
    } else if (gSystem.newKeys & PAD_KEY_DOWN) {
        input = DpadMenuBox_GetNeighborInDirection(cursor->boxes, NULL, NULL, NULL, NULL, cursor->position, 1);
        direction = 1;
    } else if (gSystem.newKeys & PAD_KEY_LEFT) {
        input = DpadMenuBox_GetNeighborInDirection(cursor->boxes, NULL, NULL, NULL, NULL, cursor->position, 2);
        direction = 2;
    } else if (gSystem.newKeys & PAD_KEY_RIGHT) {
        input = DpadMenuBox_GetNeighborInDirection(cursor->boxes, NULL, NULL, NULL, NULL, cursor->position, 3);
        direction = 3;
    } else {
        input = 0xFF;
    }
    if (input != 0xFF) {
        neighborIsEnabled = TRUE;
        if (input & 0x80) {
            if (cursor->prevPosition != 0xFF) {
                input = cursor->prevPosition;
            } else {
                input ^= 0x80;
            }
        }
        while (TRUE) {
            if (cursor->enabledMask & (1 << input)) {
                break;
            }
            neighborIsEnabled = FALSE;
            next = DpadMenuBox_GetNeighborInDirection(cursor->boxes, NULL, NULL, NULL, NULL, input, direction) & 0x7F;
            if (next == input || next == cursor->position) {
                input = cursor->position;
                break;
            }
            input = next;
        }
        if (cursor->position != input) {
            DpadMenuBox_GetPosition(&cursor->boxes[input], &x, &y);
            DpadMenuBox_GetDimensions(&cursor->boxes[input], &w, &h);
            if (BattleDpadCursor_HasReturnLink(&cursor->boxes[input], direction) == TRUE && neighborIsEnabled) {
                cursor->prevPosition = cursor->position;
            } else {
                cursor->prevPosition = 0xFF;
            }
            cursor->position = input;
            ov12_0226BAFC(cursor->battleCursor, x, w, y, h);
            PlaySE(SEQ_SE_DP_SELECT);
        }
        return -1;
    }
    if (gSystem.newKeys & PAD_BUTTON_A) {
        return cursor->position;
    } else if (gSystem.newKeys & PAD_BUTTON_B) {
        PlaySE(SEQ_SE_DP_DECIDE);
        return -2;
    }
    return -1;
}
