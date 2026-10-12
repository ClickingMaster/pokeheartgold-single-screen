#include "battle/battle_dpad_cursor.h"

#include "global.h"

#include "constants/sndseq.h"

#include "battle/battle_cursor.h"

#include "heap.h"
#include "sound_playback.h"
#include "system.h"

#pragma force_active on

BattleDpadCursor *BattleDpadCursor_New(enum HeapID heapID) {
    BattleDpadCursor *cursor = Heap_Alloc(heapID, sizeof(BattleDpadCursor));
    u8 *byte = (u8 *)cursor;
    u32 size = sizeof(BattleDpadCursor);
    do {
        *byte++ = 0;
    } while (--size != 0);
    return cursor;
}

void BattleDpadCursor_Free(BattleDpadCursor *cursor) {
    Heap_Free(cursor);
}

BattleCursor *BattleDpadCursor_GetBattleCursor(const BattleDpadCursor *cursor) {
    return cursor->battleCursor;
}

u8 BattleDpadCursor_GetPosition(const BattleDpadCursor *cursor) {
    return cursor->position;
}

u8 BattleDpadCursor_IsActive(const BattleDpadCursor *cursor) {
    return cursor->active;
}

void BattleDpadCursor_SetActive(BattleDpadCursor *cursor, u8 active) {
    cursor->active = active;
}

void BattleDpadCursor_SetBattleCursor(BattleDpadCursor *cursor, BattleCursor *battleCursor) {
    cursor->battleCursor = battleCursor;
}

void BattleDpadCursor_SetPosition(BattleDpadCursor *cursor, u8 position) {
    cursor->position = position;
    if (cursor->active == TRUE) {
        BattleCursor_SetRect(cursor->battleCursor,
            cursor->boxes[cursor->position].left,
            cursor->boxes[cursor->position].width,
            cursor->boxes[cursor->position].top,
            cursor->boxes[cursor->position].height);
    }
}

void BattleDpadCursor_ResetPosition(BattleDpadCursor *cursor) {
    cursor->position = 0;
    cursor->prevPosition = 0xFF;
}

void BattleDpadCursor_SetBoxes(BattleDpadCursor *cursor, const DpadMenuBox *boxes) {
    BattleDpadCursor_ResetPosition(cursor);
    cursor->boxes = boxes;
    cursor->enabledMask = 0xFFFFFFFF;
    if (cursor->active == TRUE) {
        boxes = cursor->boxes;
        BattleCursor_SetRect(cursor->battleCursor, boxes->left, boxes->width, boxes->top, boxes->height);
    }
}

void BattleDpadCursor_SetEnabledMask(BattleDpadCursor *cursor, u32 enabledMask) {
    cursor->enabledMask = enabledMask;
}

#pragma force_active reset

BOOL BattleDpadCursor_TryActivate(BattleDpadCursor *cursor) {
    if (cursor->active == TRUE) {
        return TRUE;
    }
    if (gSystem.newKeys & (PAD_BUTTON_A | PAD_BUTTON_B | PAD_KEY_RIGHT | PAD_KEY_LEFT | PAD_KEY_UP | PAD_KEY_DOWN)) {
        cursor->active = TRUE;
        BattleCursor_SetRect(cursor->battleCursor,
            cursor->boxes[cursor->position].left,
            cursor->boxes[cursor->position].width,
            cursor->boxes[cursor->position].top,
            cursor->boxes[cursor->position].height);
        PlaySE(SEQ_SE_DP_SELECT);
    }
    return FALSE;
}

// TRUE when the box's neighbour in the direction opposite to the move is flagged (bit 7) as a link back to the box
// the cursor came from; directions 0-3 are up, down, left, right.
BOOL BattleDpadCursor_HasReturnLink(const DpadMenuBox *box, u32 direction) {
    switch (direction) {
    case 0:
        if (box->buttonDown & 0x80) {
            return TRUE;
        }
        break;
    case 1:
        if (box->buttonUp & 0x80) {
            return TRUE;
        }
        break;
    case 2:
        if (box->buttonRight & 0x80) {
            return TRUE;
        }
        break;
    case 3:
        if (box->buttonLeft & 0x80) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}
