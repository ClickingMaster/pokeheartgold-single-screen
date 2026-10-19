#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
struct String;
extern "C" void LobbyString_InitCopy(String *dst, const String *src);
extern "C" void LobbyString_InitFromCStr(String *dst, const char *src);
extern "C" void LobbyString_Assign(String *dst, const String *src);
extern "C" void LobbyString_Destroy(String *s);

struct String {
    u32 w[3];

    String() {
        for (u32 i = 0; i < 3; i++) {
            w[i] = 0;
        }
    }

    String(const char *s) {
        LobbyString_InitFromCStr(this, s);
    }

    String(const String &s) {
        LobbyString_InitCopy(this, &s);
    }

    void destroy() {
        LobbyString_Destroy(this);
    }

    String &operator=(const String &s) {
        LobbyString_Assign(this, &s);
        return *this;
    }
};

extern "C" String LobbyString_ConcatCStr(const String &a, const char *b);
extern "C" String LobbyString_Concat(const String &a, const String &b);

inline String operator+(const String &a, const char *b) {
    return LobbyString_ConcatCStr(a, b);
}

inline String operator+(const String &a, const String &b) {
    return LobbyString_Concat(a, b);
}

struct Triple {
    u32 v[3];
};

struct NameTable {
    const char *names[12];
};

struct Obj {
    u32 connection;
    u32 state;
    u32 userId;
    String nick;
    String user;
    String realName;
    String gameName;
    String gameKey;
    String channelPrefix;
    u32 unk54, lastError, connectedFrames, disconnected;
    Triple callbacks;
    u32 userMapCount, userMapRoot, unk78;
    void *userMapFirst;
    u32 channelMapCount, channelMapRoot, unk88;
    void *channelMapFirst;
    u32 requestMapCount, requestMapRoot;
    void *requestMapLeftmost;
    u32 timerMapCount, timerMapRoot, unkA4;
    void *timerMapLeftmost;
};

extern "C" NameTable gLobbyChatPrefixNames;
extern "C" char sStr_GSP[];
extern "C" char sStr_Bang_2[];
extern "C" Obj *LobbyChatManager_Init(Obj *obj, const String &gameName, const String &gameKey, int prefix, u32 userId, const String &nick, const String &userName, const String &realName, const Triple *config);
#pragma opt_propagation off

extern "C" Obj *LobbyChatManager_Init(Obj *obj, const String &gameName, const String &gameKey, int prefix, u32 userId, const String &nick, const String &userName, const String &realName, const Triple *config) {
    obj->connection = 0;
    obj->state = 0;
    obj->userId = userId;
    LobbyString_InitCopy(&obj->nick, &nick);
    LobbyString_InitCopy(&obj->user, &userName);
    LobbyString_InitCopy(&obj->realName, &realName);
    LobbyString_InitCopy(&obj->gameName, &gameName);
    LobbyString_InitCopy(&obj->gameKey, &gameKey);
    u32 *w = obj->channelPrefix.w;
    u32 i = 0;
    do {
        w[i] = 0;
        i++;
    } while (i < 3);
    obj->unk54 = 0;
    obj->lastError = 0;
    obj->connectedFrames = 0;
    obj->disconnected = 0;
    obj->callbacks = *config;
    obj->userMapCount = 0;
    obj->userMapRoot = 0;
    obj->userMapFirst = &obj->userMapRoot;
    obj->channelMapCount = 0;
    obj->channelMapRoot = 0;
    obj->channelMapFirst = &obj->channelMapRoot;
    obj->requestMapCount = 0;
    obj->requestMapRoot = 0;
    obj->requestMapLeftmost = &obj->requestMapRoot;
    obj->timerMapCount = 0;
    obj->timerMapRoot = 0;
    obj->timerMapLeftmost = &obj->timerMapRoot;
    NameTable prefixNames = gLobbyChatPrefixNames;
    String gsp(sStr_GSP);
    String gspSep = LobbyString_ConcatCStr(gsp, sStr_Bang_2);
    String withGame = LobbyString_Concat(gspSep, obj->gameName);
    String withGameSep = LobbyString_ConcatCStr(withGame, sStr_Bang_2);
    String prefixName(prefixNames.names[prefix]);
    String withPrefix = LobbyString_Concat(withGameSep, prefixName);
    String channelPrefix = LobbyString_ConcatCStr(withPrefix, sStr_Bang_2);
    const String *pr = &channelPrefix;
    LobbyString_Assign(&obj->channelPrefix, pr);
    channelPrefix.destroy();
    withPrefix.destroy();
    prefixName.destroy();
    withGameSep.destroy();
    withGame.destroy();
    gspSep.destroy();
    gsp.destroy();
    return obj;
}

#pragma opt_propagation reset
