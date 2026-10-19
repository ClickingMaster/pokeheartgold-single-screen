#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <stdarg.h>

#include "ppw_lobby_work.h"

void LobbyCharVector_Grow(LobbyCharVector *vector, u32 size);
void *LobbyByteVector_Data(LobbyCharVector *vector);
void LobbyStringVectorBuffer_EraseAtEnd(LobbyStringVectorBuffer *buffer, u32 count, LobbyTag tag);

void LobbyStringVectorBuffer_EraseAtEnd(LobbyStringVectorBuffer *buffer, u32 count, LobbyTag tag) {
    Str *end = buffer->data + buffer->size;
    buffer->size -= count;
    if (!count) {
        return;
    }
    do {
        end--;
        LobbyString_Destroy(end);
    } while (--count);
}

PPWLobbySchedule *PPWLobbySchedule_Init(PPWLobbySchedule *schedule) {
    schedule->state = 0;
    schedule->hasTime = 0;
    schedule->time.seconds = 0;
    schedule->startTime = 0;
    schedule->startRequested = FALSE;
    schedule->lockTime = 0;
    schedule->unk_20 = 0;
    schedule->roomFlag = 0;
    schedule->roomType = 0;
    schedule->season = 0;
    schedule->events.data = NULL;
    schedule->events.size = 0;
    schedule->events.capacity = 0;
    schedule->pendingEvents.start = 0;
    schedule->pendingEvents.size = 0;
    schedule->pendingEvents.map.blocks = NULL;
    schedule->pendingEvents.map.start = 0;
    schedule->pendingEvents.map.size = 0;
    schedule->pendingEvents.map.capacity = 0;
    schedule->vips.data = NULL;
    schedule->vips.size = 0;
    schedule->vips.capacity = 0;
    return schedule;
}

BOOL PPWLobbySchedule_Copy(PPWLobbySchedule *schedule, PPWLobbyScheduleRecord *out, u32 *size) {
    u32 needed = (schedule->events.size - 1) * sizeof(LobbyEvent) + sizeof(PPWLobbyScheduleRecord);
    u32 capacity = *size;
    *size = needed;
    if (capacity < needed || !out) {
        return FALSE;
    }
    out->lockTime = schedule->lockTime;
    out->unk_04 = schedule->unk_20;
    out->roomFlag = schedule->roomFlag;
    out->roomType = schedule->roomType;
    out->season = schedule->season;
    out->eventCount = schedule->events.size;
    if (out->eventCount) {
        MI_CpuCopy8(LobbyEventVector_Data(&schedule->events), out->events, schedule->events.size * sizeof(LobbyEvent));
    }
    return TRUE;
}

LobbyEvent *LobbyEventVector_Data(LobbyEventVector *vector) {
    if (vector->size == 0) {
        return NULL;
    }
    return vector->data;
}

void PPWLobbySchedule_CopyToVector(LobbyCharVector *out, PPWLobbySchedule *schedule) {
    u32 size;
    size = 0;
    PPWLobbySchedule_Copy(schedule, NULL, &size);
    {
        u32 n = size;
        u32 zero = 0;
        out->data = NULL;
        out->size = zero;
        out->capacity = zero;
        LobbyCharVector_Grow(out, n);
    }
    PPWLobbySchedule_Copy(schedule, LobbyByteVector_Data(out), &size);
}
