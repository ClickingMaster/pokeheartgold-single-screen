#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern "C" {
#include <nitro/rtc.h>
}

struct TrueType {};

struct FalseType {};

template <class T, class U>
inline void Assign(T &d, const U &s, TrueType, FalseType) {
    d = s;
}

struct String {
    union {
        struct {
            u32 flag : 1;
            u32 capacity : 31;
            u32 size;
            char *data;
        } l;

        struct {
            u8 flag : 1;
            u8 size : 7;
            char data[11];
        } s;
    } raw;

    const char *c_str() const {
        return !raw.l.flag ? raw.s.data : raw.l.data;
    }

    void destroy();
};

extern "C" void LobbyString_Destroy(String *s);

inline void String::destroy() {
    LobbyString_Destroy(this);
}

extern "C" void LobbyString_InitFromCStr(String *s, const char *p);
extern "C" void LobbyChat_FormatNick(String *s, void *user);
extern "C" void LobbyString_ConcatCStr(String *out, const String *a, const char *b);
struct Root;
extern "C" void LobbyChatManager_GetGameName(String *, Root *);
extern "C" void LobbyChatManager_GetNick(String *, Root *);
extern "C" void LobbyChatManager_GetRealName(String *, Root *);
extern "C" void LobbyChatManager_GetGameKey(String *, Root *);

struct Task {
    s64 time;
    s64 period;
    Root *root;
    int (*func)(void);

    Task() {
    }

    Task(s64 t, Root *r, int (*f)(void)) {
        time = t;
        period = 30;
        root = r;
        func = f;
    }
};

template <class A, class B>
struct Pair {
    A first;
    B second;

    template <class C, class D>
    Pair(const Pair<C, D> &p) {
        const D *src = &p.second;
        Assign(first, p.first, TrueType(), FalseType());
        B *dst = &second;
        *dst = *src;
    }
};

struct InsertResult {
    void *node;
    bool inserted;
};
struct Map;
extern "C" void LobbyTimerMap_Insert(InsertResult *out, Map *m, const Pair<u32, Task> *v);

struct Map {
    u32 words[4];

    InsertResult insert(const Pair<u32, Task> &v) {
        InsertResult r;
        LobbyTimerMap_Insert(&r, this, &v);
        return r;
    }
};

struct Root {
    u32 connection, state;
    u8 pad[0x94];
    Map queue;
};

extern "C" Pair<int, Task> LobbyTimerMap_MakeEntry(int k, const Task &t);

struct Callbacks {
    void (*f[4])(void);
    void *data;
};

extern Root *gLobbyChatManager;
extern "C" const u8 _version_NINTENDO_DWC_LOBBY[];
extern "C" void OSi_ReferSymbol(const void *);
extern "C" const char sStr_DWCUser[], sStr_DWCName[], sStr_Dot[], sStr_PeerchatGsNintendowifiNet[];
extern "C" void *LobbyMem_Alloc(u32);
extern "C" Root *LobbyChatManager_Init(Root *, String *, String *, int, void *, String *, String *, String *, void *);
extern "C" int LobbyMem_InstallOutOfMemoryHandler(void);
extern "C" void LobbyChat_OnRaw(void), LobbyChat_OnDisconnected(void), LobbyChat_OnInvited(void), LobbyChat_OnPrivateMessage(void), LobbyChat_OnNickError(void), LobbyChat_OnFillInUser(void), LobbyChat_OnConnected(void);
extern "C" int LobbyChat_TimerConnectTimeout(void);
extern "C" u32 chatConnectSpecialA(const char *, int, const char *, const char *, const char *, const char *, Callbacks *, void (*)(void), void (*)(void), void (*)(void), void *, int);
extern "C" void DWC_GetDateTime(RTCDate *, RTCTime *);
extern "C" void LobbyMem_ReleaseReserve(void), LobbyChatManager_Destroy(Root *), DWC_Free(int, void *, int);

inline void Free(void *p) {
    if (p) {
        DWC_Free(0, p, 0);
    }
}

inline void DeleteRoot(Root *r) {
    if (r) {
        LobbyChatManager_Destroy(r);
        Free(r);
    }
}

extern "C" void MI_CpuFill8(void *, u8, u32);
extern "C" int LobbyChat_Initialize(const char *a, const char *b, int n, u8 *config, void *user);
#pragma force_active on

extern "C" int LobbyChat_Initialize(const char *a, const char *b, int n, u8 *config, void *user) {
    String outer1, outer2, outer3;
    Callbacks callbacks;
    String name2, name1;
    String s7, s6, s5, s4, s3, s2, s1;
    RTCDate date;
    RTCTime time;
    Root *root;
    int error;
    OSi_ReferSymbol(_version_NINTENDO_DWC_LOBBY);
    if (gLobbyChatManager) {
        return 4;
    }
    LobbyChat_FormatNick(&outer1, user);
    LobbyString_InitFromCStr(&outer2, sStr_DWCUser);
    LobbyString_InitFromCStr(&outer3, sStr_DWCName);
    {
        int firstReady = 0, secondReady = firstReady;
        Root *created = (Root *)LobbyMem_Alloc(0xac);
        if (created) {
            LobbyString_InitFromCStr(&name2, b);
            firstReady = 1;
            LobbyString_InitFromCStr(&name1, a);
            secondReady = firstReady;
            created = LobbyChatManager_Init(created, &name1, &name2, n, user, &outer1, &outer2, &outer3, config);
        }
        gLobbyChatManager = created;
        if (secondReady) {
            name1.destroy();
        }
        if (firstReady) {
            name2.destroy();
        }
    }
    if (!gLobbyChatManager) {
        outer3.destroy();
        outer2.destroy();
        outer1.destroy();
        return 1;
    }
    if (!LobbyMem_InstallOutOfMemoryHandler()) {
        error = 1;
        goto failure;
    }
    {
        MI_CpuFill8(&callbacks, 0, 20);
        callbacks.f[0] = LobbyChat_OnRaw;
        callbacks.f[1] = LobbyChat_OnDisconnected;
        callbacks.f[2] = LobbyChat_OnPrivateMessage;
        callbacks.f[3] = LobbyChat_OnInvited;
        callbacks.data = *(void **)(config + 8);
        LobbyChatManager_GetGameName(&s7, gLobbyChatManager);
        LobbyString_ConcatCStr(&s6, &s7, sStr_Dot);
        LobbyString_ConcatCStr(&s5, &s6, sStr_PeerchatGsNintendowifiNet);
        LobbyChatManager_GetNick(&s4, gLobbyChatManager);
        LobbyChatManager_GetRealName(&s3, gLobbyChatManager);
        LobbyChatManager_GetGameName(&s2, gLobbyChatManager);
        LobbyChatManager_GetGameKey(&s1, gLobbyChatManager);
        u32 connection = chatConnectSpecialA(s5.c_str(), 0x1a0b, s4.c_str(), s3.c_str(), s2.c_str(), s1.c_str(), &callbacks, LobbyChat_OnNickError, LobbyChat_OnFillInUser, LobbyChat_OnConnected, *(void **)(config + 8), 0);
        s1.destroy();
        s2.destroy();
        s3.destroy();
        s4.destroy();
        s5.destroy();
        s6.destroy();
        s7.destroy();
        if (!connection) {
            error = 2;
            goto failure;
        }
        gLobbyChatManager->connection = connection;
        if (gLobbyChatManager->state != 5) {
            gLobbyChatManager->state = 1;
        }
    }
    {
        root = gLobbyChatManager;
        DWC_GetDateTime(&date, &time);
        root->queue.insert(LobbyTimerMap_MakeEntry(0, Task(RTC_ConvertDateTimeToSecond(&date, &time), root, LobbyChat_TimerConnectTimeout)));
    }
    outer3.destroy();
    outer2.destroy();
    outer1.destroy();
    return 0;
failure:
    LobbyMem_ReleaseReserve();
    root = gLobbyChatManager;
    if (root) {
        DeleteRoot(root);
        gLobbyChatManager = 0;
    }
    outer3.destroy();
    outer2.destroy();
    outer1.destroy();
    return error;
}
