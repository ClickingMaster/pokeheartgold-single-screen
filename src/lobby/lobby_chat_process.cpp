#include <nitro/types.h>

extern "C" {
#include <nitro/rtc.h>
}

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "lobby_chat_manager.h"

extern "C" void DWC_GetDateTime(RTCDate *date, RTCTime *time);
extern "C" void chatThink(void *chat);

struct It;
extern "C" void ov45_02236E58(It *it);

struct It {
    LobbyTimerMapNode *n;

    It() {
    }

    It(LobbyTimerMapNode *p)
        : n(p) {
    }

    It &operator++() {
        ov45_02236E58(this);
        return *this;
    }

    It operator++(int) {
        LobbyTimerMapNode *old = n;
        ov45_02236E58(this);
        return It(old);
    }

    bool operator!=(const It &o) const {
        return n != o.n;
    }

    LobbyTimerMapNode *operator->() const {
        return n;
    }
};

extern "C" It LobbyTimerMap_Erase(LobbyTimerMap *t, It pos);

static inline bool IsFatal() {
    return gLobbyChatManager != NULL && gLobbyChatManager->state == 5;
}

static inline int GetState() {
    return gLobbyChatManager->state;
}

static inline bool Tick(LobbyTimerMapNode *n, const s64 &now) {
    if (now - n->timer.lastTime > n->timer.interval) {
        n->timer.lastTime = now;
        if (!n->timer.callback(n->timer.param)) {
            return false;
        }
    }
    return true;
}

static inline void SetError(LobbyChatManager *m, u32 code) {
    m->lastError = code;
    m->state = 5;
}

static inline void UpdateState(LobbyChatManager *m) {
    switch (m->state) {
    case 0:
    case 1:
    case 3:
    case 4:
        break;
    case 2:
        if (m->connectedFrames++ == 60) {
            m->connectedFrames = 0;
            if (m->state != 5) {
                m->state = 3;
            }
        }
        break;
    }
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

static inline It Iterator_Make(LobbyTimerMapNode *p) {
    It t;
    t.n = p;
    return t;
}

extern "C" int LobbyChat_Process(void);
#pragma push
#pragma opt_common_subs off

extern "C" int LobbyChat_Process(void) {
    LobbyChatManager *g = gLobbyChatManager;
    if (g == NULL) {
        return 0;
    }
    if (g != NULL && g->state == 5) {
        return 5;
    }
    void *p = g->connection;
    if (p != NULL) {
        chatThink(p);
    }
    LobbyChatManager *w = gLobbyChatManager;
    RTCTime time;
    RTCDate date;
    s64 now;
    LobbyTimerMapNode *end;
    It it;
    DWC_GetDateTime(&date, &time);
    now = RTC_ConvertDateTimeToSecond(&date, &time);
    it.n = w->timerMap.leftmost;
    end = (LobbyTimerMapNode *)&w->timerMap.root;
    for (LobbyTimerMapNode *n = it.n; n != end; n = it.n) {
        if (!Timer_Update(n, now)) {
            LobbyTimerMapNode *saved = it.n;
            ov45_02236E58(&it);
            LobbyTimerMap_Erase(&w->timerMap, Iterator_Make(saved));
        } else {
            ov45_02236E58(&it);
        }
    }
    LobbyChatManager *g2 = gLobbyChatManager;
    if (g2 != NULL && g2->state == 5) {
        return 5;
    }
    LobbyChatManager *m2 = gLobbyChatManager;
    if (g2->unk54 >= 3) {
        g2->unk54 = 0;
        SetError(gLobbyChatManager, 3);
        return GetState();
    }
    UpdateState(m2);
    return GetState();
}

#pragma pop
