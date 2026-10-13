#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

struct RTCDate {
    u32 year, month, day, week;
};

struct RTCTime {
    u32 hour, minute, second;
};

extern "C" s64 RTC_ConvertDateTimeToSecond(const RTCDate *date, const RTCTime *time);
extern "C" void DWC_GetDateTime(RTCDate *date, RTCTime *time);
extern "C" void chatThink(void *p);

struct Node {
    Node *a, *b, *c;
    u32 key;
    s64 last;
    s64 interval;
    void *arg;
    int (*cb)(void *arg);
};
struct It;
extern "C" void ov45_02236E58(It *it);

struct It {
    Node *n;

    It() {
    }

    It(Node *p)
        : n(p) {
    }

    It &operator++() {
        ov45_02236E58(this);
        return *this;
    }

    It operator++(int) {
        Node *old = n;
        ov45_02236E58(this);
        return It(old);
    }

    bool operator!=(const It &o) const {
        return n != o.n;
    }

    Node *operator->() const {
        return n;
    }
};

struct Tree {
    u32 size;
    Node header;
};

extern "C" It ov45_02236C40(Tree *t, It pos);

struct Mgr {
    void *unk0;
    int state;
    u8 pad8[0x54 - 0x8];
    u32 unk54;
    u32 unk58;
    int unk5C;
    u8 pad60[0x9C - 0x60];
    Tree tree;
};

extern "C" Mgr *ov45_022577DC;

static inline bool IsFatal() {
    return ov45_022577DC != NULL && ov45_022577DC->state == 5;
}

static inline int GetState() {
    return ov45_022577DC->state;
}

static inline bool Tick(Node *n, const s64 &now) {
    if (now - n->last > n->interval) {
        n->last = now;
        if (!n->cb(n->arg)) {
            return false;
        }
    }
    return true;
}

static inline void SetError(Mgr *m, u32 code) {
    m->unk58 = code;
    m->state = 5;
}

static inline void UpdateState(Mgr *m) {
    switch (m->state) {
    case 0:
    case 1:
    case 3:
    case 4:
        break;
    case 2:
        if (m->unk5C++ == 60) {
            m->unk5C = 0;
            if (m->state != 5) {
                m->state = 3;
            }
        }
        break;
    }
}

static inline int Timer_Update(Node *t, s64 now) {
    if (now - t->last > t->interval) {
        t->last = now;
        if (!t->cb(t->arg)) {
            return 0;
        }
    }
    return 1;
}

static inline It Iterator_Make(Node *p) {
    It t;
    t.n = p;
    return t;
}

extern "C" int ov45_02240874(void);
#pragma push
#pragma opt_common_subs off

extern "C" int ov45_02240874(void) {
    Mgr *g = ov45_022577DC;
    if (g == NULL) {
        return 0;
    }
    if (g != NULL && g->state == 5) {
        return 5;
    }
    void *p = g->unk0;
    if (p != NULL) {
        chatThink(p);
    }
    Mgr *w = ov45_022577DC;
    RTCTime time;
    RTCDate date;
    s64 now;
    Node *end;
    It it;
    DWC_GetDateTime(&date, &time);
    now = RTC_ConvertDateTimeToSecond(&date, &time);
    it.n = w->tree.header.c;
    end = &w->tree.header;
    for (Node *n = it.n; n != end; n = it.n) {
        if (!Timer_Update(n, now)) {
            Node *saved = it.n;
            ov45_02236E58(&it);
            ov45_02236C40(&w->tree, Iterator_Make(saved));
        } else {
            ov45_02236E58(&it);
        }
    }
    Mgr *g2 = ov45_022577DC;
    if (g2 != NULL && g2->state == 5) {
        return 5;
    }
    Mgr *m2 = ov45_022577DC;
    if (g2->unk54 >= 3) {
        g2->unk54 = 0;
        SetError(ov45_022577DC, 3);
        return GetState();
    }
    UpdateState(m2);
    return GetState();
}

#pragma pop
