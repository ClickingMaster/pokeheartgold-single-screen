#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "lobby_timer.h"

void LobbyTimerMap_MakeEntry(LobbyTimerMapEntry *entry, int id, LobbyTimer *timer) {
    LobbyTimer *dst = &entry->timer;
    entry->id = id;
    dst->lastTime = timer->lastTime;
    dst->interval = timer->interval;
    dst->param = timer->param;
    dst->callback = timer->callback;
}
