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
    u32 userData;
    String strC;
    String str18;
    String str24;
    String str30;
    String str3C;
    String str48;
    u32 unk54, lastError, connectedFrames, disconnected;
    Triple config;
    u32 unk70, unk74, unk78;
    void *unk7C;
    u32 unk80, unk84, unk88;
    void *unk8C;
    u32 unk90, unk94;
    void *unk98;
    u32 unk9C, unkA0, unkA4;
    void *unkA8;
};

extern "C" NameTable gLobbyChatPrefixNames;
extern "C" char sStr_GSP[];
extern "C" char sStr_Bang_2[];
extern "C" Obj *LobbyChatManager_Init(Obj *obj, const String &gameName, const String &gameKey, int prefix, u32 userId, const String &nick, const String &userName, const String &realName, const Triple *config);
#pragma opt_propagation off

extern "C" Obj *LobbyChatManager_Init(Obj *obj, const String &gameName, const String &gameKey, int prefix, u32 userId, const String &nick, const String &userName, const String &realName, const Triple *config) {
    obj->connection = 0;
    obj->state = 0;
    obj->userData = userId;
    LobbyString_InitCopy(&obj->strC, &nick);
    LobbyString_InitCopy(&obj->str18, &userName);
    LobbyString_InitCopy(&obj->str24, &realName);
    LobbyString_InitCopy(&obj->str30, &gameName);
    LobbyString_InitCopy(&obj->str3C, &gameKey);
    u32 *w = obj->str48.w;
    u32 i = 0;
    do {
        w[i] = 0;
        i++;
    } while (i < 3);
    obj->unk54 = 0;
    obj->lastError = 0;
    obj->connectedFrames = 0;
    obj->disconnected = 0;
    obj->config = *config;
    obj->unk70 = 0;
    obj->unk74 = 0;
    obj->unk7C = &obj->unk74;
    obj->unk80 = 0;
    obj->unk84 = 0;
    obj->unk8C = &obj->unk84;
    obj->unk90 = 0;
    obj->unk94 = 0;
    obj->unk98 = &obj->unk94;
    obj->unk9C = 0;
    obj->unkA0 = 0;
    obj->unkA8 = &obj->unkA0;
    NameTable prefixNames = gLobbyChatPrefixNames;
    String gsp(sStr_GSP);
    String gspSep = LobbyString_ConcatCStr(gsp, sStr_Bang_2);
    String withGame = LobbyString_Concat(gspSep, obj->str30);
    String withGameSep = LobbyString_ConcatCStr(withGame, sStr_Bang_2);
    String prefixName(prefixNames.names[prefix]);
    String withPrefix = LobbyString_Concat(withGameSep, prefixName);
    String channelPrefix = LobbyString_ConcatCStr(withPrefix, sStr_Bang_2);
    const String *pr = &channelPrefix;
    LobbyString_Assign(&obj->str48, pr);
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
