#include "global.h"

#include "dwc/net_common_internal.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "ppw_lobby_state.h"
#include "ppw_lobby_work.h"

extern u8 gPPWLobbyMainChannelSecretMode[];

void PPWLobbyChannel_GetName(Str *out, PPWLobbyChannel *channel);
void LobbyChat_SetChannelMode(const char *channel, const u8 *mode);
void PPWLobby_SendMessage(int id, int channelKind, int type, int unused, int mode, const void *data, u32 size);
void LobbyEventDeque_EraseAtBegin(LobbyEventDeque *deque, u32 count, LobbyTag tag);
BOOL PPWLobbyPlayers_IsFull(PPWLobbyPlayers *players);

BOOL PPWLobbySchedule_GetQuestionnaire(PPWLobbySchedule *schedule, void *out) {
    if (!schedule->hasQuestionnaire) {
        return FALSE;
    }
    MI_CpuCopy8(schedule->questionnaire, out, sizeof(schedule->questionnaire));
    return TRUE;
}

BOOL PPWLobbySchedule_SetQuestionnaire(PPWLobbySchedule *schedule, const void *in) {
    MI_CpuCopy8(in, schedule->questionnaire, sizeof(schedule->questionnaire));
    schedule->hasQuestionnaire = TRUE;
    return TRUE;
}

void PPWLobbySchedule_Process(PPWLobbySchedule *schedule) {
    Str name;
    RTCTime time;
    RTCDate date;
    u32 zero;
    LobbyEvent *event;
    s64 now;
    switch (schedule->state) {
    case 0:
    case 1:
    case 2:
    case 3:
        break;
    case 4:
        if (!PPWLobbySchedule_CanStart(schedule)) {
            return;
        }
        PPWLobbyChannel_GetName(&name, &PPWLobby_GetWork()->mainChannel);
        LobbyChat_SetChannelMode(name.l.isLong == 0 ? name.s.buf : name.l.ptr, gPPWLobbyMainChannelSecretMode);
        LobbyString_Destroy(&name);
        zero = 0;
        PPWLobby_SendMessage(-1, 0, 1, 0, 0, &zero, 4);
        PPWLobbySchedule_Start(schedule);
        return;
    case 5:
        if (!schedule->pendingEvents.size) {
            return;
        }
        event = schedule->pendingEvents.map.blocks[(schedule->pendingEvents.map.start + (schedule->pendingEvents.start >> 7)) % schedule->pendingEvents.map.capacity] + (schedule->pendingEvents.start & 127);
        DWC_GetDateTime(&date, &time);
        now = RTC_ConvertDateTimeToSecond(&date, &time);
        if (event->key > now - schedule->startTime) {
            return;
        }
        do {
            LobbyTag tag;
            PPWLobby_GetWork()->callbacks.onLobbyEvent(event->payload);
            memset(&tag, 0, sizeof tag);
            LobbyEventDeque_EraseAtBegin(&schedule->pendingEvents, 1, tag);
            if (!schedule->pendingEvents.size) {
                return;
            }
            {
                LobbyEvent *next = schedule->pendingEvents.map.blocks[(schedule->pendingEvents.map.start + (schedule->pendingEvents.start >> 7)) % schedule->pendingEvents.map.capacity] + (schedule->pendingEvents.start & 127);
                event->key = next->key;
                event->payload = next->payload;
            }
        } while (event->key <= now - schedule->startTime);
        break;
    }
}

BOOL PPWLobbySchedule_CanStart(PPWLobbySchedule *schedule) {
    RTCTime time;
    RTCDate date;
    s64 now;
    if (schedule->state == 5 || PPWLobby_GetWork()->state != PPW_LOBBY_STATE_READY) {
        return FALSE;
    }
    if (schedule->startTime != 0) {
        return FALSE;
    }
    if (schedule->startRequested) {
        return TRUE;
    }
    if (PPWLobbyPlayers_IsFull(&PPWLobby_GetWork()->players)) {
        return TRUE;
    }
    DWC_GetDateTime(&date, &time);
    now = RTC_ConvertDateTimeToSecond(&date, &time);
    return schedule->time.seconds + (s64)(u64)schedule->lockTime < now;
}

BOOL PPWLobbySchedule_Start(PPWLobbySchedule *schedule) {
    RTCTime time;
    RTCDate date;
    if (schedule->state != 5 && PPWLobby_GetWork()->state == PPW_LOBBY_STATE_READY) {
        DWC_GetDateTime(&date, &time);
        schedule->startTime = RTC_ConvertDateTimeToSecond(&date, &time);
        schedule->state = 5;
        schedule->startRequested = FALSE;
        PPWLobby_GetWork()->callbacks.onLobbyEvent(0);
        return TRUE;
    }
    schedule->startRequested = TRUE;
    return FALSE;
}
