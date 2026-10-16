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
extern "C" void LobbyChat_FormatNick(String *s, void *userId);
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
extern "C" int LobbyChat_Initialize(const char *gameName, const char *gameKey, int prefix, u8 *config, void *userId);
#pragma force_active on

extern "C" int LobbyChat_Initialize(const char *gameName, const char *gameKey, int prefix, u8 *config, void *userId) {
    String nick, userName, realName;
    Callbacks callbacks;
    String gameKeyStr, gameNameStr;
    String hostGameName, hostPrefix, host, nickArg, realNameArg, gameNameArg, gameKeyArg;
    RTCDate date;
    RTCTime time;
    Root *root;
    int error;
    OSi_ReferSymbol(_version_NINTENDO_DWC_LOBBY);
    if (gLobbyChatManager) {
        return 4;
    }
    LobbyChat_FormatNick(&nick, userId);
    LobbyString_InitFromCStr(&userName, sStr_DWCUser);
    LobbyString_InitFromCStr(&realName, sStr_DWCName);
    {
        int firstReady = 0, secondReady = firstReady;
        Root *created = (Root *)LobbyMem_Alloc(0xac);
        if (created) {
            LobbyString_InitFromCStr(&gameKeyStr, gameKey);
            firstReady = 1;
            LobbyString_InitFromCStr(&gameNameStr, gameName);
            secondReady = firstReady;
            created = LobbyChatManager_Init(created, &gameNameStr, &gameKeyStr, prefix, userId, &nick, &userName, &realName, config);
        }
        gLobbyChatManager = created;
        if (secondReady) {
            gameNameStr.destroy();
        }
        if (firstReady) {
            gameKeyStr.destroy();
        }
    }
    if (!gLobbyChatManager) {
        realName.destroy();
        userName.destroy();
        nick.destroy();
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
        LobbyChatManager_GetGameName(&hostGameName, gLobbyChatManager);
        LobbyString_ConcatCStr(&hostPrefix, &hostGameName, sStr_Dot);
        LobbyString_ConcatCStr(&host, &hostPrefix, sStr_PeerchatGsNintendowifiNet);
        LobbyChatManager_GetNick(&nickArg, gLobbyChatManager);
        LobbyChatManager_GetRealName(&realNameArg, gLobbyChatManager);
        LobbyChatManager_GetGameName(&gameNameArg, gLobbyChatManager);
        LobbyChatManager_GetGameKey(&gameKeyArg, gLobbyChatManager);
        u32 connection = chatConnectSpecialA(host.c_str(), 0x1a0b, nickArg.c_str(), realNameArg.c_str(), gameNameArg.c_str(), gameKeyArg.c_str(), &callbacks, LobbyChat_OnNickError, LobbyChat_OnFillInUser, LobbyChat_OnConnected, *(void **)(config + 8), 0);
        gameKeyArg.destroy();
        gameNameArg.destroy();
        realNameArg.destroy();
        nickArg.destroy();
        host.destroy();
        hostPrefix.destroy();
        hostGameName.destroy();
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
    realName.destroy();
    userName.destroy();
    nick.destroy();
    return 0;
failure:
    LobbyMem_ReleaseReserve();
    root = gLobbyChatManager;
    if (root) {
        DeleteRoot(root);
        gLobbyChatManager = 0;
    }
    realName.destroy();
    userName.destroy();
    nick.destroy();
    return error;
}
