#include "global.h"

#include "dwc/net_common_internal.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <stdarg.h>

#include "ppw_lobby_internal.h"
#include "ppw_lobby_state.h"

// What PPW_LobbyGetTimeInfo reports: the current time, the channel time and the schedule's start time, in seconds.
typedef struct PPWLobbyTimeInfo {
    s64 currentTime;
    PPWLobbyTime channelTime;
    s64 startTime;
} PPWLobbyTimeInfo;

int PPW_LobbyGetTimeInfo(PPWLobbyTimeInfo *out);
int PPW_LobbyGetSchedule(void *p, u32 *size);

void PPWLobbyChannel_GetName(Str *out, PPWLobbyChannel *channel) {
    LobbyString_InitCopy(out, &channel->name);
}

int PPW_LobbyGetTimeInfo(PPWLobbyTimeInfo *out) {
    PPWLobbyWork *work = gPPWLobbyWork;
    if (!work) {
        return 4;
    }
    if (work) {
        if (work->state == PPW_LOBBY_STATE_ERROR) {
            return 7;
        }
    }
    if (work) {
        if (PPWLobby_GetState() != PPW_LOBBY_STATE_READY) {
            return 4;
        }
    }
    RTCDate date;
    RTCTime time;
    DWC_GetDateTime(&date, &time);
    out->currentTime = RTC_ConvertDateTimeToSecond(&date, &time);
    volatile PPWLobbySchedule *schedule = &work->schedule;
    out->channelTime.seconds = schedule->time.seconds;
    out->startTime = schedule->startTime;
    return 0;
}

int PPW_LobbyGetSchedule(void *p, u32 *size) {
    PPWLobbyWork *work = gPPWLobbyWork;
    if (!work) {
        return 4;
    }
    if (work) {
        if (work->state == PPW_LOBBY_STATE_ERROR) {
            return 7;
        }
    }
    if (work) {
        if (PPWLobby_GetState() != PPW_LOBBY_STATE_READY) {
            return 4;
        }
    }
    return PPWLobbySchedule_Copy(&work->schedule, p, size) ? 0 : 3;
}
