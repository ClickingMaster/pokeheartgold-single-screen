#include <nitro/types.h>

extern "C" {
#include <nitro/os/mutex.h>
#include <nitro/os/thread.h>
#include <nitro/rtc.h>

#include "dwc/net_common_internal.h"
#include "dwc/stats_upload_internal.h"
}

#include "ppw_lobby_state.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "lobby_timer.h"
#include "ppw_lobby_work.h"

#include "../../lib/NitroDWC/src/tree-iterator4.h"

#include <string.h>
using std::memcmp;

// A system profile value with the flag that it must be uploaded.
struct ProfileValue {
    u32 channel;
    PPWLobbyTime time;
    u32 unk_0C;

    ProfileValue(u32 c, const PPWLobbyTime &t, u32 e)
        : channel(c) {
        PPWLobbyTime *d = &time;
        *d = t;
        unk_0C = e;
    }

    ProfileValue(const ProfileValue &other)
        : channel(other.channel) {
        PPWLobbyTime *d = &time;
        *d = other.time;
        unk_0C = other.unk_0C;
    }
};

struct TrackedProfile {
    ProfileValue value;
    BOOL changed;

    TrackedProfile(u32 c, const PPWLobbyTime &t, u32 e, BOOL ch)
        : value(ProfileValue(c, t, e))
        , changed(ch) {
    }
};

static inline bool IsSameProfile(const PPWLobbySystemProfile *profile, const ProfileValue *value) {
    return memcmp(profile, value, sizeof(ProfileValue)) == 0;
}

// Stores the profile as the player's system profile and marks it for upload, unless it is the same.
static inline void SetSystemProfile(PPWLobbyPlayer *player, const TrackedProfile &profile) {
    const ProfileValue *value = &profile.value;
    if (!IsSameProfile(&player->systemProfile, value)) {
        player->systemProfile.channel = value->channel;
        player->systemProfile.time = value->time;
        player->systemProfile.unk_0C = value->unk_0C;
        player->systemProfileChanged = TRUE;
    }
}

static inline int GetState(const PPWLobbyWork &work) {
    return work.state;
}

typedef PPWLobbyWebRequest Request;

extern "C" Request *gPPWLobbyCurrentRequest;
extern "C" Request *gPPWLobbyFinishedRequest;
extern "C" u8 gPPWLobbyPlayersVtable[];
extern "C" char sStr_Star[];

// nitro/mi/memory.h does not compile as C++.
extern "C" void MI_CpuCopy8(const void *src, void *dst, u32 size);

extern "C" {
void *LobbyByteVector_Data(LobbyCharVector *);
int LobbyChat_Process(void);
int PPW_LobbyShutdownAsync(void);
LobbyEventDeque *LobbyEventDeque_Destroy(LobbyEventDeque *);
void *PPWLobbyPlayerSet_Destroy(u32 *);
int LobbyChat_GetLastError(void);
void LobbyTreeIterator_Advance(Iterator *);
void LobbyTimerMap_Erase(Iterator *, LobbyTimerMap *, Iterator);
void LobbyChat_EnumChannelsAsync(const char *, void (*)(int, int, char **, int, int *), int);
}

extern "C" int PPW_LobbyProcess(void);

static inline char *StrCStr(Str *s) {
    return !s->l.isLong ? s->s.buf : s->l.ptr;
}

static inline void Request_Finish(Request *r, int ok, void *buf, u32 len) {
    void *param = r->param;
    Request *tmp;
    StatsUpload_Stop();
    tmp = gPPWLobbyFinishedRequest;
    gPPWLobbyFinishedRequest = gPPWLobbyCurrentRequest;
    gPPWLobbyCurrentRequest = tmp;
    r->callback(ok, buf, len, param);
}

static inline void Request_Succeed(Request *r, void *buf, const u32 &size) {
    void *param = r->param;
    u32 len = size;
    Request *tmp;
    StatsUpload_Stop();
    tmp = gPPWLobbyFinishedRequest;
    gPPWLobbyFinishedRequest = gPPWLobbyCurrentRequest;
    gPPWLobbyCurrentRequest = tmp;
    r->callback(1, buf, len, param);
}

static inline int Request_Send(Request *r) {
    int ret;
    StatsUpload_Stop();
    StatsUpload_Start(r->server, StrCStr(&r->key));
    if (r->useLastHash) {
        u32 n = r->body.size;
        ret = StatsUpload_UploadWithLastHash(StrCStr(&r->page), r->pid, LobbyByteVector_Data(&r->body), n, 0);
    } else {
        u32 n = r->body.size;
        ret = StatsUpload_Upload(StrCStr(&r->page), r->pid, LobbyByteVector_Data(&r->body), n, 0);
    }
    if (ret == 0) {
        return 1;
    }
    StatsUpload_Stop();
    return 0;
}

static inline int Request_Poll(Request *r) {
    int len = 0;
    switch (StatsUpload_Process()) {
    case STATS_UPLOAD_DONE: {
        const char *data = StatsUpload_GetResponseBody(&len);
        void *buf = DWC_Alloc(0, len);
        if (buf != NULL) {
            MI_CpuCopy8(data, buf, len);
        }
        if (buf != NULL) {
            Request_Succeed(r, buf, len);
        } else {
            Request_Finish(r, 0, NULL, 0);
        }
        if (buf != NULL) {
            DWC_Free(0, buf, 0);
        }
        return 1;
    }
    case STATS_UPLOAD_FAILED:
        DWC_ClearError();
        if (--r->retries != 0) {
            if (Request_Send(r)) {
                return 0;
            }
        }
        Request_Finish(r, 0, NULL, 0);
        return 1;
    case STATS_UPLOAD_NONE:
        DWC_ClearError();
        Request_Finish(r, 0, NULL, 0);
        return 1;
    }
    return 0;
}

static inline void Request_Delete(Request *request) {
    if (request != NULL) {
        request->vtable->destroy(request);
    }
}

static inline int Request_Update(void) {
    Request *r = gPPWLobbyCurrentRequest;
    if (r == NULL) {
        return 0;
    }
    if (Request_Poll(r) && gPPWLobbyFinishedRequest != NULL) {
        Request_Delete(gPPWLobbyFinishedRequest);
        gPPWLobbyFinishedRequest = NULL;
    }
    return 1;
}

static inline void Lobby_Destroy(PPWLobbyWork *w) {
    LobbyVipVector_Destroy(&w->schedule.vips);
    LobbyEventDeque_Destroy(&w->schedule.pendingEvents);
    LobbyEventVector_Destroy(&w->schedule.events);
    LobbyTimerMap_Clear(&w->timers);
    LobbyTimerMap_Destroy(&w->timers);
    w->players.vtable = gPPWLobbyPlayersVtable;
    PPWLobbyPlayers_Clear(&w->players);
    LobbyCharVector_Destroy(&w->players.self.userProfile);
    PPWLobbyPlayerSet_Destroy(w->players.playerSet);
    LobbyString_Destroy(&w->subChannel.name);
    LobbyString_Destroy(&w->mainChannel.name);
    LobbyString_Destroy(&w->channelBaseName);
}

static inline void Lobby_Delete(PPWLobbyWork *w) {
    if (w != NULL) {
        Lobby_Destroy(w);
        if (w != NULL) {
            DWC_Free(0, w, 0);
        }
    }
}

static inline void Lobby_SetError(int code) {
    PPWLobbyWork *w = gPPWLobbyWork;
    if (code != 0) {
        w->error = code;
        w->state = PPW_LOBBY_STATE_ERROR;
    }
}

static inline bool Lobby_IsError(void) {
    return gPPWLobbyWork != NULL && GetState(*gPPWLobbyWork) == PPW_LOBBY_STATE_ERROR;
}

static inline int Timer_Update(LobbyTimerMapNode *t, s64 now) {
    if (now - t->timer.lastTime > t->timer.interval) {
        t->timer.lastTime = now;
        if (!t->timer.callback(t->timer.param)) {
            return 0;
        }
    }
    return 1;
}

static inline Iterator Iterator_Make(void *p) {
    Iterator t;
    t.current = p;
    return t;
}

static inline void Lobby_UpdateTimers(void) {
    PPWLobbyWork *w = gPPWLobbyWork;
    RTCTime time;
    RTCDate date;
    s64 now;
    void *end;
    Iterator it;
    DWC_GetDateTime(&date, &time);
    now = RTC_ConvertDateTimeToSecond(&date, &time);
    it.current = w->timers.leftmost;
    end = &w->timers.root;
    while (it.current != end) {
        if (!Timer_Update((LobbyTimerMapNode *)it.current, now)) {
            void *saved = it.current;
            Iterator res;
            LobbyTreeIterator_Advance(&it);
            LobbyTimerMap_Erase(&res, &w->timers, Iterator_Make(saved));
        } else {
            LobbyTreeIterator_Advance(&it);
        }
    }
    PPWLobbyPlayers_UpdateStates(&w->players);
    PPWLobbySchedule_Process(&w->schedule);
}

static inline int Lobby_IsReady(PPWLobbyWork *w) {
    if (w->schedule.state >= 2) {
        return 1;
    }
    return 0;
}

static inline void Lobby_EnumChannels(void) {
    Str name;
    Str query;
    LobbyString_InitFromCStr(&name, sStr_Star);
    PPWLobby_MakeChannelName(&query, 0, &name);
    LobbyChat_EnumChannelsAsync(StrCStr(&query), PPWLobby_OnEnumChannels, 0);
    LobbyString_Destroy(&query);
    LobbyString_Destroy(&name);
}

static inline void Lobby_SetEnterChannelError(int r) {
    PPWLobbyWork *w = gPPWLobbyWork;
    int code;
    switch (r) {
    case 1:
        code = 2;
        break;
    case 2:
        code = 3;
        break;
    default:
        code = 0;
        break;
    }
    if (code != 0) {
        w->error = code;
        w->state = PPW_LOBBY_STATE_ERROR;
    }
}

static inline int Lobby_Process(void) {
    switch (LobbyChat_Process()) {
    case 0:
        PPW_LobbyShutdownAsync();
        if (gPPWLobbyWork != NULL) {
            Lobby_Delete(gPPWLobbyWork);
            gPPWLobbyWork = NULL;
        }
        return PPW_LOBBY_STATE_CLOSED;
    case 3:
        if (gPPWLobbyWork->state == PPW_LOBBY_STATE_CONNECTING) {
            gPPWLobbyWork->state = PPW_LOBBY_STATE_CONNECTED;
        }
        break;
    case 5:
        Lobby_SetError(LobbyChat_GetLastError());
        break;
    case 1:
    case 2:
    case 4:
        break;
    }
    Lobby_UpdateTimers();
    switch (gPPWLobbyWork->state) {
    case PPW_LOBBY_STATE_NOT_INITIALIZED:
        return PPW_LOBBY_STATE_CLOSED;
    case PPW_LOBBY_STATE_CONNECTED:
        gPPWLobbyWork->state = PPW_LOBBY_STATE_WAIT_SCHEDULE;
        break;
    case PPW_LOBBY_STATE_WAIT_SCHEDULE:
        if (Lobby_IsReady(gPPWLobbyWork)) {
            gPPWLobbyWork->state = PPW_LOBBY_STATE_ENUM_CHANNELS;
        }
        break;
    case PPW_LOBBY_STATE_ENUM_CHANNELS: {
        Lobby_EnumChannels();
        gPPWLobbyWork->state = PPW_LOBBY_STATE_SEARCHING_CHANNEL;
        break;
    }
    case PPW_LOBBY_STATE_ENTER_MAIN_CHANNEL: {
        int r = PPWLobby_EnterMainChannel();
        if (r != 0) {
            Lobby_SetEnterChannelError(r);
        } else {
            gPPWLobbyWork->state = PPW_LOBBY_STATE_WAIT_MAIN_CHANNEL;
        }
        break;
    }
    case PPW_LOBBY_STATE_WAIT_MAIN_CHANNEL:
        if (PPWLobby_IsMainChannelReady()) {
            PPWLobbyPlayer *self = &gPPWLobbyWork->players.self;
            self->systemProfileChanged = 0;
            TrackedProfile profile(self->systemProfile.channel, self->systemProfile.time, 1, TRUE);
            SetSystemProfile(self, profile);
            self->systemProfileReceived = TRUE;
            gPPWLobbyWork->state = PPW_LOBBY_STATE_READY;
            gPPWLobbyWork->callbacks.onEnterChannel(1, 0);
        }
        break;
    case PPW_LOBBY_STATE_CONNECTING:
    case PPW_LOBBY_STATE_SEARCHING_CHANNEL:
    case PPW_LOBBY_STATE_READY:
    case PPW_LOBBY_STATE_CLOSED:
    case PPW_LOBBY_STATE_ERROR:
        break;
    }
    return gPPWLobbyWork->state;
}

int PPW_LobbyProcess(void) {
    int polled = Request_Update();
    if (polled) {
        if (gPPWLobbyWork == NULL) {
            return PPW_LOBBY_STATE_CLOSED;
        }
    } else if (gPPWLobbyWork == NULL) {
        return PPW_LOBBY_STATE_NOT_INITIALIZED;
    }
    if (Lobby_IsError()) {
        return PPW_LOBBY_STATE_ERROR;
    }
    return Lobby_Process();
}
