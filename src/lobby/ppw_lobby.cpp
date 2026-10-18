// PPW lobby core: sending and receiving lobby messages, main and sub channel handling, player tracking and the
// questionnaire, schedule, VIP and profile web requests. Kept as one translation unit because its functions share
// one block of file-scope data.
#include <nitro/types.h>

#include "ppw_lobby_state.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <size_t.h>
#include <stdarg.h>
#include <string.h>
using std::va_list;
extern "C" {
#include <nitro/os/printf.h>
}
#include <nitro/rtc.h>

#include "overlay45_format_buffer_private.h"
#include "overlay45_string_search.h"
#include "ppw_lobby_work.h"

#include "../../lib/NitroDWC/src/tree-iterator4.h"
using std::memcmp;
using std::memmove;

inline void *operator new(std::size_t, void *p) {
    return p;
}

extern "C" char sStr_WebEncLobbySubmitQuestionnaireAsp[], sStr_WebEncLobbyGetQuestionnaireAsp[], sStr_StatsUploadKey[], sStr_WebEncLobbyGetScheduleAsp[], sStr_WebEncLobbyGetVIP_Asp[], sStr_WebEncLobbyCheckProfileAsp[];
#pragma explicit_zero_data on
extern "C" u8 gLobbyTimerMapInserted = 1;
extern "C" const char *gPPWLobbyGetScheduleUrl = sStr_WebEncLobbyGetScheduleAsp;
extern "C" const char *gPPWLobbyGetVipUrl = sStr_WebEncLobbyGetVIP_Asp;
extern "C" const char *gPPWLobbyGetQuestionnaireUrl = sStr_WebEncLobbyGetQuestionnaireAsp;
extern "C" const char *gPPWLobbySubmitQuestionnaireUrl = sStr_WebEncLobbySubmitQuestionnaireAsp;
extern "C" signed char gLobbyStringTerminator = 0;
extern "C" const char *gPPWLobbyCheckProfileUrl = sStr_WebEncLobbyCheckProfileAsp;
extern "C" const char *gPPWLobbyStatsUploadKey = sStr_StatsUploadKey;
extern "C" signed char gLobbyStringInPlaceTerminator = 0;
extern "C" u8 gLobbyTimerMapNotInserted = 0;
#pragma explicit_zero_data reset

namespace SubmitQuestionnaire {
extern "C" void OS_GetMacAddress(u8 *);
struct Str;
extern "C" Str *LobbyString_InitFromCStr(Str *, const char *);

struct Str {
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

    Str(const char *c) {
        LobbyString_InitFromCStr(this, c);
    }

    const char *CStr() const {
        return !raw.l.flag ? raw.s.data : raw.l.data;
    }
};

enum ReqMethod {
    REQ_METHOD_GET,
    REQ_METHOD_POST
};

struct TA {};

struct TB {};

struct TD {};

struct Iter {
    const u8 *p;
    u32 extra;

    Iter(const u8 *q)
        : p(q) {
    }
};
struct Vec;
extern "C" void LobbyCharVector_AssignIterRange(Vec *, Iter, Iter, TD);
extern "C" const u8 *LobbyByteVector_Data(const void *);

struct Vec {
    u8 *ptr;
    u32 size, cap;

    Vec()
        : ptr(0)
        , size(0)
        , cap(0) {
    }

    void Assign(Iter first, Iter last) {
        Assign(first, last, TA(), TB());
    }

    void Assign(Iter first, Iter last, const TA &, const TB &) {
        LobbyCharVector_AssignIterRange(this, first, last, TD());
    }
};

struct Info {
    int id;
    u16 one;
    u8 a;
    u8 b;
    u8 mac[6];
    u16 pad;
    int pad2;
};

struct Packet {
    Info info;
    u32 out0, out1;
    int mode;
    u32 d4;
    u32 d38;
    u16 d3c;
    u16 d3e;
};

struct Out {
    u32 a, b;
    u8 rest[0x2d0];
};
struct Op;

struct OpVtbl {
    void (*f0)(Op *);
    void (*del)(Op *);
};

extern "C" const OpVtbl gPPWLobbyWebRequestVtable;
extern "C" void PPWLobby_OnSubmitQuestionnaire(void);

struct Op {
    const OpVtbl *vtbl;
    int kind;
    int flag;
    Str a;
    Str b;
    int id;
    Vec vec;
    void (*cb)(void);
    int x38;
    int x3c;

    Op(int f, const char *n, const char *h, int i, const u8 *first, const u8 *last)
        : vtbl(&gPPWLobbyWebRequestVtable)
        , kind(3)
        , flag(f)
        , a(n)
        , b(h)
        , id(i)
        , vec()
        , cb(PPWLobby_OnSubmitQuestionnaire)
        , x38(0)
        , x3c(0) {
        vec.Assign(Iter(first), Iter(last));
    }
};

extern "C" void *LobbyMem_Alloc(u32);
extern "C" int PPWLobbySchedule_GetQuestionnaire(void *, void *);
extern "C" void StatsUpload_Start(int, const char *);
extern "C" int StatsUpload_UploadWithLastHash(const char *, int, const u8 *, u32, int);
extern "C" int StatsUpload_Upload(const char *, int, const u8 *, u32, int);
extern "C" void StatsUpload_Stop(void);

struct Data {
    u8 pad0[4];
    u32 x4;
    u8 pad8[0x38 - 8];
    u8 x38;
    u8 x39;
    u8 pad3a[2];
    u16 x3c;
    u8 x3e;
    u8 pad3f[3];
    u8 x42;
};

extern "C" PPWLobbyWork *gPPWLobbyWork;
extern "C" Op *gPPWLobbyCurrentRequest;

inline const Data *GetData(PPWLobbyWork *w) {
    return (const Data *)LobbyByteVector_Data(&w->players.self.userProfile);
}

#define PNEW(o, a, b, c, d, e, f) new (o) Op(a, b, c, d, e, f)
extern "C" int PPW_LobbySubmitQuestionnaire(int mode);

extern "C" int PPW_LobbySubmitQuestionnaire(int mode) {
    const char *name;
    const char *host;
    Op *o;
    int x;
    ReqMethod flag;
    PPWLobbyWork *w = gPPWLobbyWork;
    if (w == 0) {
        return 4;
    }
    if (w != 0 && w->state == PPW_LOBBY_STATE_ERROR) {
        return 7;
    }
    if (mode < 0 || mode > 2) {
        return 3;
    }
    Out out;
    if (!PPWLobbySchedule_GetQuestionnaire(&w->schedule, &out) || gPPWLobbyWork->submittingQuestionnaire != 0) {
        return 4;
    }
    const Data *d = GetData(gPPWLobbyWork);
    Packet p = { 0 };
    {
        Info info;
        const Data *d2 = GetData(gPPWLobbyWork);
        info.id = gPPWLobbyWork->userId;
        info.one = 1;
        info.a = d2->x42;
        info.b = d2->x39;
        OS_GetMacAddress(info.mac);
        p.info = info;
    }
    p.out0 = out.a;
    p.out1 = out.b;
    p.mode = mode;
    p.d4 = d->x4;
    p.d38 = d->x38;
    p.d3c = d->x3c;
    p.d3e = d->x3e;
    x = gPPWLobbyWork->userId;
    host = gPPWLobbySubmitQuestionnaireUrl;
    name = gPPWLobbyStatsUploadKey;
    flag = gPPWLobbyWork->unk_50 == 0 ? REQ_METHOD_GET : REQ_METHOD_POST;
    if (gPPWLobbyCurrentRequest == 0) {
        o = (Op *)LobbyMem_Alloc(sizeof(Op));
        PNEW(o, flag, name, host, x, (const u8 *)&p, (const u8 *)&p + sizeof(p));
        gPPWLobbyCurrentRequest = o;
        StatsUpload_Start(o->flag, o->a.CStr());
        int ret;
        if (o->x3c) {
            u32 n = o->vec.size;
            ret = StatsUpload_UploadWithLastHash(o->b.CStr(), o->id, LobbyByteVector_Data(&o->vec), n, 0);
        } else {
            u32 n = o->vec.size;
            ret = StatsUpload_Upload(o->b.CStr(), o->id, LobbyByteVector_Data(&o->vec), n, 0);
        }
        int ok;
        if (ret == 0) {
            ok = 1;
        } else {
            StatsUpload_Stop();
            ok = 0;
        }
        if (!ok) {
            if (gPPWLobbyCurrentRequest != 0) {
                Op *q = gPPWLobbyCurrentRequest;
                if (q != 0) {
                    q->vtbl->del(q);
                }
                gPPWLobbyCurrentRequest = 0;
            }
        }
    }
    gPPWLobbyWork->submittingQuestionnaire = 1;
    return 0;
}

}

#undef PNEW

namespace LastError {
extern "C" {

typedef union S {
    struct {
        u32 flag : 1;
        u32 cap : 31;
        u32 len;
        char *ptr;
    } l;

    struct {
        u8 flag : 1;
        u8 len : 7;
        char data[11];
    } s;
} S;

typedef struct Small {
    u8 x;
} Small;

typedef struct Vec {
    char *data;
    u32 len, cap;
} Vec;

typedef struct Node {
    void *left, *right;
    u32 parent;
    u32 key;
} Node;

typedef struct Body {
    u64 a, b;
    u32 c, d;
} Body;

typedef struct Record {
    int type;
    u32 words[6];
} Record;

extern PPWLobbyWork *volatile gPPWLobbyWork;
extern u8 gPPWLobbyPlayersVtable[];
extern void *LobbyString_InitCopy(void *, void *);
extern void *LobbyCharVector_Data(Vec *);
extern int DWC_Free();
extern int DWC_GetDateTime();
extern int PPWLobby_SendMessage();
extern int PPWLobby_SetChannelData();
extern int PPWLobbyPlayerSet_Destroy();
extern int LobbyTimerMap_Clear();
extern int LobbyCharVector_Grow();
extern int LobbyCharVector_Reallocate();
extern int LobbyCharVector_RecommendSize();
extern int LobbyCharVector_Destroy();
extern int LobbyString_InitFromCStr();
extern int LobbyString_Destroy();
extern int PPWLobbyPlayers_Clear();
extern int LobbyChat_ToErrorCode(int);
extern int LobbyChat_IsInChannel();
void *PPWLobby_GetChannelBaseName(u8 *p, u8 *q);
void *PPWLobbyChannel_GetName(u8 *p, u8 *q);
void LobbyCharVector_AssignRange(Vec *p, char *first, char *last);
void LobbyTimerMap_MakeEntry(Record *p, int type, Body *q);
int LobbyTimerMap_EraseKey(u8 *p, u32 *key);
int PPW_LobbyGetLastError(void);
int PPW_LobbyToErrorCode(int x);
int PPW_LobbyInitResultToLastError(int x);
void PPWLobby_GetChannelName(S *p, u8 *q, int x);

int PPW_LobbyGetLastError(void) {
    PPWLobbyWork *work = gPPWLobbyWork;
    if (work) {
        return work->error;
    }
    return 0;
}

int PPW_LobbyToErrorCode(int x) {
    if (x >= 4) {
        return x + 0x297c;
    }
    return LobbyChat_ToErrorCode(x);
}

int PPW_LobbyInitResultToLastError(int x) {
    switch (x) {
    case 0:
        return 0;
    case 1:
        return 2;
    case 2:
        return 3;
    default:
        return 1;
    }
}
}
}

namespace SendMessage {
class Str;
extern "C" Str *LobbyString_Destroy(Str *);
extern "C" Str *LobbyString_InitCopy(Str *, const Str *);
extern "C" int LobbyString_CompareCStr(const Str *, const char *);

class Str {
public:
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

        u32 words[3];
    } raw;

    Str() {
        u32 i;
        u32 *p = raw.words;
        for (i = 0; i < 3; i++) {
            p[i] = 0;
        }
    }

    Str(const Str &o) {
        LobbyString_InitCopy(this, &o);
    }

    void Destroy() {
        LobbyString_Destroy(this);
    }

    const char *CStr() const {
        return !raw.l.flag ? raw.s.data : raw.l.data;
    }

    bool operator==(const char *t) const {
        return LobbyString_CompareCStr(this, t) == 0;
    }
};

extern "C" PPWLobbyWork *gPPWLobbyWork;
extern "C" char sStr_Empty_15[];
extern "C" Str PPWLobby_GetChannelName(PPWLobbyChannel *, int);
extern "C" int PPWLobby_EncodeMessage(int, int, int, int, int, Str *);
extern "C" int LobbyChat_SendUserMessage(int, const char *);
extern "C" int LobbyChat_SendChannelMessage(const char *, const char *);
extern "C" int PPWLobby_SendMessage(int, int, int, int, int, int, int);

class ErrorMap {
public:
    int Map(int r) const;
};

inline int ErrorMap::Map(int r) const {
    int state = 0;
    switch (r) {
    case 1:
        state = 2;
        break;
    case 2:
        state = 3;
        break;
    }
    return state;
}

inline void SetError(int r) {
    PPWLobbyWork *g = gPPWLobbyWork;
    int state = ErrorMap().Map(r);
    if (state != 0) {
        g->error = state;
        g->state = PPW_LOBBY_STATE_ERROR;
    }
}

extern "C" int PPWLobby_SendMessage(int id, int key, int a, int b, int c, int d, int e) {
    int result;
    if (id == -1 && key == 5) {
        return 3;
    }
    Str value;
    if (!PPWLobby_EncodeMessage(a, b, c, d, e, &value)) {
        value.Destroy();
        return 3;
    }
    if (key == 5) {
        result = LobbyChat_SendUserMessage(id, value.CStr());
        SetError(result);
        value.Destroy();
        return result;
    }
    Str name = PPWLobby_GetChannelName(&gPPWLobbyWork->mainChannel, key);
    if (name == sStr_Empty_15) {
        name.Destroy();
        value.Destroy();
        return 5;
    }
    result = LobbyChat_SendChannelMessage(name.CStr(), value.CStr());
    SetError(result);
    name.Destroy();
    value.Destroy();
    return result;
}

}

namespace SetChannelData {
extern "C" {

typedef union {
    u32 words[3];

    struct {
        u32 heap : 1;
        u32 unused : 31;
    } bits;

    struct {
        char *ptr;
        u32 size;
        u32 capacity;
    } vector;

    u8 bytes[12];
} String;

extern PPWLobbyWork *gPPWLobbyWork;
extern char sStr_Empty_15[];
extern int PPWLobby_EncodeMessage(void *, void *, void *, void *, void *, String *), LobbyChat_SendUserMessage(int, char *), LobbyChat_SendChannelMessage(char *, char *), LobbyString_CompareCStr(String *, char *), LobbyChat_SetChannelKey(char *, int, int, void *);
extern void LobbyString_Destroy(String *), PPWLobby_GetChannelName(String *, void *, int), LobbyCharVector_Grow(String *, u32), LobbyCharVector_Destroy(String *);
extern void *LobbyCharVector_Data(String *);
extern int DWC_Base64Encode(void *, u32, void *, u32);

static inline char *Text(String *s) {
    if (s->bits.heap == 0) {
        return (char *)&s->bytes[1];
    } else {
        return (char *)s->words[2];
    }
}

static inline int Empty(String *s) {
    return LobbyString_CompareCStr(s, sStr_Empty_15) == 0;
}

static inline void Status(int result) {
    volatile u8 z = 0;
    PPWLobbyWork *g = gPPWLobbyWork;
    int e = 0;
    (void)&z;
    switch (result) {
    case 1:
        e = 2;
        break;
    case 2:
        e = 3;
        break;
    }
    if (e) {
        g->error = e;
        g->state = PPW_LOBBY_STATE_ERROR;
    }
}

int PPWLobby_SendMessage(int, int, void *, void *, void *, void *, void *);
int PPWLobby_SetChannelData(int, int, int, void *, u32);

int PPWLobby_SetChannelData(int id, int key, int mode, void *data, u32 length) {
    String name, encoded;
    int result;
    char *text;
    PPWLobby_GetChannelName(&name, &gPPWLobbyWork->mainChannel, key);
    if (Empty(&name)) {
        LobbyString_Destroy(&name);
        return 5;
    }
    if (length > 300) {
        LobbyString_Destroy(&name);
        return 3;
    }
    encoded.words[0] = 0;
    encoded.words[1] = 0;
    encoded.words[2] = 0;
    LobbyCharVector_Grow(&encoded, (length + 2) / 3 * 4 + 1);
    {
        void *out = LobbyCharVector_Data(&encoded);
        int end = DWC_Base64Encode(data, length, out, encoded.vector.size);
        encoded.vector.ptr[end] = 0;
    }
    text = Text(&name);
    result = LobbyChat_SetChannelKey(text, id, mode, LobbyCharVector_Data(&encoded));
    Status(result);
    LobbyCharVector_Destroy(&encoded);
    LobbyString_Destroy(&name);
    return result;
}
}
}

namespace StartCheckProfile {
extern "C" void OS_GetMacAddress(u8 *);
struct Str;
extern "C" Str *LobbyString_InitFromCStr(Str *, const char *);

struct Str {
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

        u32 words[3];
    } raw;

    Str(const char *c) {
        LobbyString_InitFromCStr(this, c);
    }

    const char *CStr() const {
        return !raw.l.flag ? raw.s.data : raw.l.data;
    }
};

enum ReqMethod {
    REQ_METHOD_GET,
    REQ_METHOD_POST
};

struct Al {};

struct TA {};

struct TB {};

struct TC {};

struct TD {};

struct Iter {
    const u8 *p;
    u32 extra;

    Iter(const u8 *q)
        : p(q) {
    }
};
struct Vec;
extern "C" void LobbyCharVector_AssignIterRange(Vec *, Iter, Iter, TD);
extern "C" const char *LobbyByteVector_Data(const void *);

struct Vec {
    char *ptr;
    u32 size, cap;

    Vec()
        : ptr(0)
        , size(0)
        , cap(0) {
    }

    void Assign(Iter first, Iter last) {
        Assign(first, last, TA(), TB());
    }

    void Assign(Iter first, Iter last, const TA &, const TB &) {
        LobbyCharVector_AssignIterRange(this, first, last, TD());
    }
};

typedef u64 W8;

struct B48 {
    u32 w[12];
};

struct Config {
    u32 a, b;
    u16 h08[8];
    u32 h18, h1C;
    u16 h20[6];
    u8 b2C[6];
    u8 b32[6];
    u8 b38, b39;
    u16 h3A, h3C;
    u8 b3E, b3F, b40, b41, b42, b43;
    W8 b44;
    u8 b4C[12];
    B48 b58;

    struct {
        u16 h88, h8A;
    } t1;

    struct {
        u32 h8C, h90;
    } t2;
};

struct Info {
    u32 x;
    u16 one;
    u8 a;
    u8 b;
    u8 mac[6];
    u8 rest[6];
    Config cfg;
};
struct Obj;

struct ObjVtbl {
    void (*f0)(Obj *);
    void (*del)(Obj *);
};

extern "C" const ObjVtbl gPPWLobbyWebRequestVtable;
extern "C" void PPWLobby_OnCheckProfile(int, int, int);

struct Obj {
    const ObjVtbl *vtbl;
    int kind;
    int flag;
    Str name;
    Str host;
    u32 x;
    Vec data;
    void (*cb)(int, int, int);
    int x38;
    int x3c;

    Obj(int f, const char *n, const char *h, u32 x_, const Info &info)
        : vtbl(&gPPWLobbyWebRequestVtable)
        , kind(3)
        , flag(f)
        , name(n)
        , host(h)
        , x(x_)
        , data()
        , cb(PPWLobby_OnCheckProfile)
        , x38(0)
        , x3c(0) {
        data.Assign(Iter((const u8 *)&info), Iter((const u8 *)&info + sizeof(Info)));
    }
};

extern "C" PPWLobbyWork *gPPWLobbyWork;
extern "C" Obj *gPPWLobbyCurrentRequest;
extern "C" void *LobbyMem_Alloc(u32);
extern "C" void StatsUpload_Start(int, const char *);
extern "C" int StatsUpload_UploadWithLastHash(const char *, u32, const char *, u32, int);
extern "C" int StatsUpload_Upload(const char *, u32, const char *, u32, int);
extern "C" void StatsUpload_Stop(int);
extern "C" void PPWLobby_StartCheckProfile(void);

extern "C" void PPWLobby_StartCheckProfile(void) {
    Info info;
    const Config *c = (const Config *)LobbyByteVector_Data(&gPPWLobbyWork->players.self.userProfile);
    const PPWLobbyUserProfile *f = (const PPWLobbyUserProfile *)LobbyByteVector_Data(&gPPWLobbyWork->players.self.userProfile);
    const char *name;
    const char *host;
    Obj *o;
    u32 x;
    ReqMethod flag;
    info.x = gPPWLobbyWork->userId;
    info.one = 1;
    info.a = f->version;
    info.b = f->language;
    OS_GetMacAddress(info.mac);
    info.cfg = *c;
    x = gPPWLobbyWork->userId;
    host = gPPWLobbyCheckProfileUrl;
    name = gPPWLobbyStatsUploadKey;
    flag = gPPWLobbyWork->unk_50 == 0 ? REQ_METHOD_GET : REQ_METHOD_POST;
    if (gPPWLobbyCurrentRequest != 0) {
        return;
    }
    o = (Obj *)LobbyMem_Alloc(sizeof(Obj));
    new (o) Obj(flag, name, host, x, info);
    gPPWLobbyCurrentRequest = o;
    StatsUpload_Start(o->flag, o->name.CStr());
    int ret;
    if (o->x3c) {
        u32 n = o->data.size;
        ret = StatsUpload_UploadWithLastHash(o->host.CStr(), o->x, LobbyByteVector_Data(&o->data), n, 0);
    } else {
        u32 n = o->data.size;
        ret = StatsUpload_Upload(o->host.CStr(), o->x, LobbyByteVector_Data(&o->data), n, 0);
    }
    int ok;
    if (ret == 0) {
        ok = 1;
    } else {
        StatsUpload_Stop(ret);
        ok = 0;
    }
    if (ok) {
        return;
    }
    if (gPPWLobbyCurrentRequest != 0) {
        Obj *p = gPPWLobbyCurrentRequest;
        if (p != 0) {
            p->vtbl->del(p);
        }
        gPPWLobbyCurrentRequest = 0;
    }
}

}

namespace MakeChannelName {
extern "C" {
extern char *gPPWLobbyChannelNamePrefixes[];
extern void PPWLobbyChannel_GetName(Str *, void *), LobbyString_Reserve(Str *, u32);
extern int LobbyString_Compare(Str *, Str *), LobbyChat_IsInChannel(const char *), PPWLobbyPlayers_AllHaveProfiles(void *), PPWLobby_SetChannelData(u32, u32, const void *, const void *, u32);
extern void *LobbyByteVector_Data(void *);
extern u32 current_time(void);
extern void LobbyString_Format(Str *, const char *, ...);
extern void PPWLobby_SendMessage(u32, u32, u32, u32, u32, const void *, u32), LobbyChat_GetChannelModeAsync(const char *, void (*)(void), s32), LobbyChat_GetChannelKeysAsync(const char *, s32, const char *, void (*)(void), s32);
extern void PPWLobby_OnGetChannelMode(void), PPWLobby_OnGetChannelTime(void), PPWLobby_OnGetLobbyData(void), PPWLobbySchedule_SetQuestionnaire(void *, void *);
extern Str *LobbyString_ConcatCStr(Str *, Str *, char *);
extern Str *LobbyString_Concat(Str *, Str *, Str *);
extern char sStr_Bang[];
extern char sFmt_C_IntUnderscoreUint[];
extern char sStr_b_lib_u_user[];
extern char sStr_b_lib_u_system[];
extern char sStr_b_lib_c_time[];
extern char sStr_b_lib_c_lobby[];
void PPWLobby_MakeChannelName(Str *out, u32 id, Str *s);
void PPWLobby_GenerateChannelBaseName(Str *out);
int LobbyString_Equals(Str *a, Str *b);
int PPWLobby_IsMainChannelReady(void);
int PPWLobby_TimerUploadUserProfile(void);
int PPWLobby_TimerUploadSystemProfile(void);
int PPWLobby_TimerSendRecruitInfo(void);
int PPWLobby_TimerPollChannelMode(void);
int PPWLobby_TimerRequestChannelTime(void);
int PPWLobby_TimerRequestLobbyData(void);
int PPWLobby_GetWebResultError(u32 type);
int PPWLobby_ParseWebResponse(int valid, u8 *data, u32 size, void **output, u32 *length);
void PPWLobby_OnGetQuestionnaire(int valid, u8 *data, u32 size);
void PPWLobby_OnSubmitQuestionnaire(int status);

extern Str *LobbyString_ReplaceRange(Str *, u32, u32, const char *, const char *, u8);

static inline u32 short_length(Str *p) {
    if (p->l.flag) {
        return p->l.size;
    }
    return p->s.size;
}

void PPWLobby_MakeChannelName(Str *out, u32 id, Str *s) {
    Str b, a;
    LobbyString_InitFromCStr(&b, gPPWLobbyChannelNamePrefixes[id]);
    LobbyString_ConcatCStr(&a, &b, sStr_Bang);
    LobbyString_Concat(out, &a, s);
    LobbyString_Destroy(&a);
    LobbyString_Destroy(&b);
}
}
}

namespace StringConcat {

namespace LobbyStringTypes {
    union Str {
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

        u32 words[3];
    };

    class RangeDispatch {};

    typedef char ActualString12[sizeof(Str) == 12 ? 1 : -1];
    typedef char ActualCarrier1[sizeof(RangeDispatch) == 1 ? 1 : -1];
}

using namespace LobbyStringTypes;
extern "C" void LobbyString_Reserve(Str *, u32);
extern "C" Str *LobbyString_ReplaceRange(Str *, u32, u32, char *, char *, RangeDispatch);
extern "C" Str *LobbyString_Concat(Str *, Str *, Str *);

extern "C" Str *LobbyString_Concat(Str *out, Str *left, Str *right) {
    u32 i, n1, n2;
    char *p1, *p2;
    RangeDispatch first, second;
    for (i = 0; i < 3; i++) {
        out->words[i] = 0;
    }
    if (left->l.flag) {
        n1 = left->l.size;
        p1 = left->l.data;
    } else {
        n1 = left->s.size;
        p1 = left->s.data;
    }
    if (right->l.flag) {
        p2 = right->l.data;
        n2 = right->l.size;
    } else {
        p2 = right->s.data;
        n2 = right->s.size;
    }
    LobbyString_Reserve(out, n1 + n2);
    out = LobbyString_ReplaceRange(out, 0, 0, p1, p1 + n1, first);
    return LobbyString_ReplaceRange(out, n1, 0, p2, p2 + n2, second);
}

}

namespace StringConcatCStr {

namespace LobbyStringTypes {
    union Str {
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

        u32 words[3];
    };

    class RangeDispatch {};

    typedef char ActualString12[sizeof(Str) == 12 ? 1 : -1];
    typedef char ActualCarrier1[sizeof(RangeDispatch) == 1 ? 1 : -1];
}

using namespace LobbyStringTypes;
extern "C" void LobbyString_Reserve(Str *, u32);
extern "C" Str *LobbyString_ReplaceRange(Str *, u32, u32, char *, char *, RangeDispatch);
extern "C" unsigned strlen(const char *);
extern "C" Str *LobbyString_ConcatCStr(Str *, Str *, char *);

extern "C" Str *LobbyString_ConcatCStr(Str *out, Str *left, char *right) {
    u32 i, n1;
    char *p1;
    RangeDispatch first, second;
    for (i = 0; i < 3; i++) {
        out->words[i] = 0;
    }
    const u32 n2 = strlen(right);
    if (left->l.flag) {
        p1 = left->l.data;
        n1 = left->l.size;
    } else {
        p1 = left->s.data;
        n1 = left->s.size;
    }
    LobbyString_Reserve(out, n1 + n2);
    out = LobbyString_ReplaceRange(out, 0, 0, p1, p1 + n1, first);
    return LobbyString_ReplaceRange(out, n1, 0, right, right + n2, second);
}

}

namespace GenerateChannelBaseName {
extern "C" {
extern PPWLobbyWork *gPPWLobbyWork;
extern char *gPPWLobbyChannelNamePrefixes[];
extern void PPWLobbyChannel_GetName(Str *, void *), LobbyString_Reserve(Str *, u32);
extern int LobbyString_Compare(Str *, Str *), LobbyChat_IsInChannel(const char *), PPWLobbyPlayers_AllHaveProfiles(void *), PPWLobby_SetChannelData(u32, u32, const void *, const void *, u32);
extern void *LobbyByteVector_Data(void *);
extern u32 current_time(void);
extern void LobbyString_Format(Str *, const char *, ...);
extern void PPWLobby_SendMessage(u32, u32, u32, u32, u32, const void *, u32), LobbyChat_GetChannelModeAsync(const char *, void (*)(void), s32), LobbyChat_GetChannelKeysAsync(const char *, s32, const char *, void (*)(void), s32);
extern void PPWLobby_OnGetChannelMode(void), PPWLobby_OnGetChannelTime(void), PPWLobby_OnGetLobbyData(void), PPWLobbySchedule_SetQuestionnaire(void *, void *);
extern Str *LobbyString_ConcatCStr(Str *, Str *, char *);
extern Str *LobbyString_Concat(Str *, Str *, Str *);
extern char sStr_Bang[];
extern char sFmt_C_IntUnderscoreUint[];
extern char sStr_b_lib_u_user[];
extern char sStr_b_lib_u_system[];
extern char sStr_b_lib_c_time[];
extern char sStr_b_lib_c_lobby[];
void PPWLobby_MakeChannelName(Str *out, u32 id, Str *s);
void PPWLobby_GenerateChannelBaseName(Str *out);
int LobbyString_Equals(Str *a, Str *b);
int PPWLobby_IsMainChannelReady(void);
int PPWLobby_TimerUploadUserProfile(void);
int PPWLobby_TimerUploadSystemProfile(void);
int PPWLobby_TimerSendRecruitInfo(void);
int PPWLobby_TimerPollChannelMode(void);
int PPWLobby_TimerRequestChannelTime(void);
int PPWLobby_TimerRequestLobbyData(void);
int PPWLobby_GetWebResultError(u32 type);
int PPWLobby_ParseWebResponse(int valid, u8 *data, u32 size, void **output, u32 *length);
void PPWLobby_OnGetQuestionnaire(int valid, u8 *data, u32 size);
void PPWLobby_OnSubmitQuestionnaire(int status);

extern Str *LobbyString_ReplaceRange(Str *, u32, u32, const char *, const char *, u8);

static inline u32 short_length(Str *p) {
    if (p->l.flag) {
        return p->l.size;
    }
    return p->s.size;
}

void PPWLobby_GenerateChannelBaseName(Str *out) {
    u32 id = gPPWLobbyWork->userId;
    u32 tick = current_time();
    LobbyString_Format(out, sFmt_C_IntUnderscoreUint, id, tick % 1000);
}
}
}

using std::va_list;
extern "C" {
}
using namespace LobbyCharVectorTypes;

namespace StringFormat {
struct Vector {
    Vec repr;

    Vector() {
        repr.ptr = 0;
        repr.size = 0;
        repr.cap = 0;
    }

    ~Vector() {
        LobbyCharVector_Destroy(&repr);
    }

    void shrink(u32 count, ResizeDispatch) {
        repr.size -= count;
    }

    void resize(u32 count) {
        if (count > repr.size) {
            LobbyCharVector_AppendZeros(&repr, count - repr.size, ResizeDispatch());
        } else if (count < repr.size) {
            shrink(repr.size - count, ResizeDispatch());
        }
    }

    char *text() {
        return LobbyCharVector_Data(&repr);
    }
};

typedef char StringSize[sizeof(Str) == 12 ? 1 : -1];
typedef char VectorSize[sizeof(Vector) == 12 ? 1 : -1];
typedef char DispatchSize[sizeof(ResizeDispatch) == 1 ? 1 : -1];
}

using namespace StringFormat;
extern "C" void LobbyString_Format(Str *, const char *, ...);

extern "C" void LobbyString_Format(Str *output, const char *format, ...) {
    Vector vector;
    va_list args;
    u32 count;
    va_start(args, format);
    count = OS_VSNPrintf(0, 0, format, args) + 1;
    vector.resize(count);
    OS_VSNPrintf(vector.text(), count, format, args);
    vector.repr.ptr[count - 1] = 0;
    va_end(args);
    new (output) Str;
    LobbyString_InitFromCStr(output, vector.text());
}

namespace EnterMainChannel {
class Str;
extern "C" Str *LobbyString_Destroy(Str *);
extern "C" Str *LobbyString_InitCopy(Str *, const Str *);
extern "C" Str *LobbyString_Assign(void *, const Str *);

class Str {
public:
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

        u32 words[3];
    } raw;

    Str(const Str &o) {
        LobbyString_InitCopy(this, &o);
    }

    void Destroy() const {
        LobbyString_Destroy(const_cast<Str *>(this));
    }

    Str &operator=(const Str &o) {
        LobbyString_Assign(this, &o);
        return *this;
    }

    const char *CStr() const {
        return !raw.l.flag ? raw.s.data : raw.l.data;
    }
};

struct Callbacks {
    void (*a)();
    void (*b)();
    void (*c)();
    void (*d)();
    void (*e)();
    u32 f;

    Callbacks(void (*a_)(), void (*b_)(), void (*c_)(), void (*d_)(), void (*e_)())
        : a(a_)
        , b(b_)
        , c(c_)
        , d(d_)
        , e(e_)
        , f(0) {
    }
};

extern "C" PPWLobbyWork *gPPWLobbyWork;
extern "C" void PPWLobby_OnPlayerJoined(), PPWLobby_OnPlayerLeft(), PPWLobby_OnMessage(), PPWLobby_OnChannelDataChanged(), PPWLobby_OnTopicChanged(), PPWLobby_OnEnterMainChannel();
extern "C" Str PPWLobby_GetChannelBaseName(PPWLobbyWork *, int);
extern "C" Str PPWLobby_MakeChannelName(int, const Str &);
extern "C" int LobbyChat_EnterChannelAsync(const char *, int, int, Callbacks *, void (*)(), int);
extern "C" int PPWLobby_EnterMainChannel(void);

class ErrorMap {
public:
    int Map(int r) const;
};

inline int ErrorMap::Map(int r) const {
    int state = 0;
    switch (r) {
    case 1:
        state = 2;
        break;
    case 2:
        state = 3;
        break;
    }
    return state;
}

inline Str MakeMainChannelName(Str t) {
    Str n = PPWLobby_MakeChannelName(0, t);
    t.Destroy();
    return n;
}

struct Channel {
    int kind;
    u32 nm[3];
    int usersEnumerated;
    int modeReady;

    Str &name() {
        return *(Str *)nm;
    }

    void Destroy() {
        name().Destroy();
    }
};

inline void SetSelfChannel(const Str &n) {
    Channel c;
    PPWLobbyWork *w;
    Channel *p = &c;
    c.kind = 0;
    LobbyString_InitCopy(&c.name(), &n);
    p->usersEnumerated = 0;
    p->modeReady = 0;
    w = gPPWLobbyWork;
    w->mainChannel.kind = c.kind;
    LobbyString_Assign(&w->mainChannel.name, &c.name());
    w->mainChannel.usersEnumerated = c.usersEnumerated;
    w->mainChannel.modeReady = c.modeReady;
    c.Destroy();
}

#pragma push
#pragma opt_propagation off

extern "C" int PPWLobby_EnterMainChannel(void) {
    Callbacks cb(PPWLobby_OnPlayerJoined, PPWLobby_OnPlayerLeft, PPWLobby_OnMessage, PPWLobby_OnChannelDataChanged, PPWLobby_OnTopicChanged);
    Str name = MakeMainChannelName(PPWLobby_GetChannelBaseName(gPPWLobbyWork, 0));
    int result = LobbyChat_EnterChannelAsync(name.CStr(), 0, 0x14, &cb, PPWLobby_OnEnterMainChannel, 0);
    {
        PPWLobbyWork *w = gPPWLobbyWork;
        int state = ErrorMap().Map(result);
        if (state != 0) {
            w->error = state;
            w->state = PPW_LOBBY_STATE_ERROR;
        }
    }
    if (result != 0) {
        name.Destroy();
        return result;
    }
    {
        SetSelfChannel(name);
    }
    name.Destroy();
    return result;
}

#pragma pop

}

namespace HandleReceivedData {
struct Str;
extern "C" Str *LobbyString_InitFromCStr(Str *, char *);
extern "C" Str *LobbyString_Destroy(Str *);
extern "C" Str *LobbyString_InitCopy(Str *, const Str *);

struct Str {
    u32 words[3];

    Str(char *s) {
        LobbyString_InitFromCStr(this, s);
    }

    Str(const Str &o) {
        LobbyString_InitCopy(this, &o);
    }

    void Destroy() {
        LobbyString_Destroy(this);
    }
};
struct Vec;
extern "C" Vec *LobbyCharVector_Destroy(Vec *);

struct Vec {
    char *ptr;
    u32 size, cap;

    void Init() {
        ptr = 0;
        size = 0;
        cap = 0;
    }

    void Destroy() {
        LobbyCharVector_Destroy(this);
    }
};

extern "C" PPWLobbyWork *gPPWLobbyWork;
extern "C" Str PPWLobbyChannel_GetName(PPWLobbyChannel *);
extern "C" int LobbyString_Equals(const Str &, const Str &);
extern "C" int PPWLobby_DecodeMessageData(const char *, Vec *);
extern "C" char *LobbyByteVector_Data(Vec *);
extern "C" int PPWLobby_HandleLibraryData(int, int, int, int, int, char *, u32, int);
extern "C" PPWLobbyPlayer *PPWLobbyPlayers_Find(void *, int);
extern "C" void PPWLobby_HandleReceivedData(int, int, char *, int, int, const char *, int);

inline int Find(PPWLobbyWork *w, const Str &name) {
    const Str &n = name;
    int eq;
    {
        Str t = PPWLobbyChannel_GetName(&w->mainChannel);
        eq = LobbyString_Equals(t, n);
        t.Destroy();
    }
    if (eq) {
        return w->mainChannel.kind;
    }
    {
        Str t = PPWLobbyChannel_GetName(&w->subChannel);
        eq = LobbyString_Equals(t, n);
        t.Destroy();
    }
    if (eq) {
        return w->subChannel.kind;
    }
    return 5;
}

inline int IsActive(PPWLobbyPlayer *s) {
    return s != 0 && s->entered != 0;
}

#pragma opt_common_subs off

extern "C" void PPWLobby_HandleReceivedData(int a, int b, char *channel, int id, int arg, const char *text, int extra) {
    int kind;
    Vec data;
    {
        Str name(channel);
        kind = Find(gPPWLobbyWork, name);
        name.Destroy();
    }
    if (kind == 5) {
        return;
    }
    data.Init();
    if (!PPWLobby_DecodeMessageData(text, &data)) {
        data.Destroy();
        return;
    }
    u32 len = data.size;
    if (len == 0) {
        data.Destroy();
        return;
    }
    if (PPWLobby_HandleLibraryData(a, b, kind, id, arg, LobbyByteVector_Data(&data), len, extra)) {
        data.Destroy();
        return;
    }
    if (id != -1) {
        if (!IsActive(PPWLobbyPlayers_Find(&gPPWLobbyWork->players, id))) {
            data.Destroy();
            return;
        }
    }
    PPWLobbyWork *w = gPPWLobbyWork;
    w->callbacks.onLocationsChanged(a, b, kind, id, arg, LobbyByteVector_Data(&data), data.size, extra);
    data.Destroy();
}

#pragma opt_common_subs reset

}

namespace DecodeMessageData {
extern "C" unsigned strlen(const char *);

class ResizeDispatch {};
struct Vec;

class Decoder {
public:
    int Decode(const char *src, u32 len, Vec *dst) const;
};

struct Vec {
    char *ptr;
    u32 size, cap;
};

extern "C" void LobbyCharVector_AppendZeros(Vec *, u32, ResizeDispatch);
extern "C" char *LobbyByteVector_Data(Vec *);
extern "C" int DWC_Base64Decode(const char *, u32, char *, u32);
extern "C" int PPWLobby_DecodeMessageData(const char *, Vec *);

inline void Shrink(Vec *v, u32 count, ResizeDispatch) {
    v->size -= count;
}

inline void Resize(Vec *v, u32 count) {
    u32 size = v->size;
    if (count > size) {
        LobbyCharVector_AppendZeros(v, count - size, ResizeDispatch());
    } else if (count < size) {
        Shrink(v, size - count, ResizeDispatch());
    }
}

inline int Decoder::Decode(const char *src, u32 len, Vec *dst) const {
    return DWC_Base64Decode(src, len, LobbyByteVector_Data(dst), dst->size);
}

extern "C" int PPWLobby_DecodeMessageData(const char *text, Vec *out) {
    Decoder codec = Decoder();
    u32 len = strlen(text);
    int n;
    Resize(out, len);
    if (len == 0) {
        return 1;
    }
    n = codec.Decode(text, strlen(text), out);
    if (n == -1) {
        return 0;
    }
    Resize(out, n);
    return 1;
}

}

namespace StringEquals {
class String {
public:
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

    u32 Size() const {
        return !raw.l.flag ? raw.s.size : raw.l.size;
    }
};

extern "C" int LobbyString_Compare(const String *, const String *);
extern "C" int LobbyString_Equals(const String &, const String &);

extern "C" int LobbyString_Equals(const String &a, const String &b) {
    return a.Size() == b.Size() && LobbyString_Compare(&a, &b) == 0;
}

}

namespace HandleLibraryData {
extern "C" int strcmp(const char *, const char *);
extern "C" int memcmp(const void *, const void *, unsigned long);

struct TA {};

struct TB {};

struct TD {};

struct TE {};

struct TU {};

struct Iter {
    const u8 *p;
    u32 extra;

    Iter(const u8 *q)
        : p(q) {
    }
};
struct Vec;
extern "C" void LobbyCharVector_AssignIterRange(Vec *, Iter, Iter, TD);
extern "C" void LobbyCharVector_InitRange(Vec *, const u8 *, const u8 *, TU);
extern "C" void LobbyCharVector_AssignRange(Vec *, const u8 *, const u8 *, TE);
extern "C" const u8 *LobbyByteVector_Data(const Vec *);
extern "C" void LobbyCharVector_Destroy(const Vec *);

struct Vec {
    u8 *ptr;
    u32 size, cap;

    void Init() {
        ptr = 0;
        size = 0;
        cap = 0;
    }

    void Assign(const u8 *first, const u8 *last) {
        Assign(Iter(first), Iter(last));
    }

    void Assign(Iter first, Iter last) {
        Assign(first, last, TA(), TB());
    }

    void Assign(Iter first, Iter last, const TA &, const TB &) {
        LobbyCharVector_AssignIterRange(this, first, last, TD());
    }

    void InitCopy(const Vec &o, TU al) {
        ptr = 0;
        size = 0;
        cap = 0;
        LobbyCharVector_InitRange(this, o.ptr, o.ptr + o.size, al);
    }

    void AssignVec(const Vec &o) {
        if (this != &o) {
            LobbyCharVector_AssignRange(this, o.ptr, o.ptr + o.size, TE());
        }
    }
};

inline bool operator==(const Vec &a, const Vec &b) {
    return a.size == b.size && (a.size == 0 || memcmp(LobbyByteVector_Data(&a), LobbyByteVector_Data(&b), a.size) == 0);
}

struct Quad {
    u32 a;
    u64 b;
    u32 c;
};

struct Triple {
    s64 b;
    u32 a;
};

struct VecBox {
    Vec v;
    int changed;

    void Init(const Vec &x, TU al) {
        v.InitCopy(x, al);
        changed = 1;
    }

    void InitM(const Vec &x, TU al) {
        v.InitCopy(x, al);
        Mark();
    }

    void Mark() {
        if (this) {
            changed = 1;
        }
    }

    void Update(const VecBox &o) {
        if (!(v == o.v)) {
            v.AssignVec(o.v);
            changed = 1;
        }
    }
};

inline bool operator==(const Quad &a, const Quad &b) {
    return memcmp(&a, &b, 16) == 0;
}

struct QuadBox {
    Quad q;
    int changed;

    void Init(const Quad &x) {
        q = x;
        changed = 1;
    }

    void Update(const QuadBox &o) {
        if (!(q == o.q)) {
            q = o.q;
            changed = 1;
        }
    }
};

struct Rec {
    u32 x0;
    QuadBox q;
    VecBox vec;
    u32 x28, x2c;
    int qRecv;
    int vecRecv;
    int valid;
    void SetVec(const u8 *d, u32 n, TU al);
    void SetQuad(const u8 *d);

    Triple GetTriple() {
        Triple t;
        t.a = q.q.a;
        t.b = q.q.b;
        return t;
    }
};

struct Val : PPWLobbyTime {
    Val(const u8 *d) {
        ((u32 *)&seconds)[0] = ((const u32 *)d)[0];
        ((u32 *)&seconds)[1] = ((const u32 *)d)[1];
    }

    Val(const Val &o) {
        seconds = o.seconds;
    }
};

extern "C" PPWLobbyWork *gPPWLobbyWork;
extern "C" const char sStr_b_lib_u_user[], sStr_b_lib_u_system[], sStr_b_lib_c_time[], sStr_b_lib_c_lobby[];
extern "C" Rec *PPWLobbyPlayers_Find(void *, int);
extern "C" int PPWLobbySchedule_Load(PPWLobbySchedule *, const u8 *, u32);
extern "C" int PPWLobby_HandleLibraryData(int, int, int, int, const char *, const u8 *, u32);

inline void Rec::SetVec(const u8 *d, u32 n, TU al) {
    Vec v;
    VecBox box;
    v.Init();
    v.Assign(Iter(d), Iter(d + n));
    VecBox *pb = &box;
    pb->InitM(v, al);
    VecBox *pc = &box;
    vec.Update(*pc);
    LobbyCharVector_Destroy(&box.v);
    vecRecv = 1;
    LobbyCharVector_Destroy(&v);
}

inline void Rec::SetQuad(const u8 *d) {
    QuadBox qbox;
    QuadBox *pq = &qbox;
    pq->Init(*(const Quad *)d);
    q.Update(*pq);
    qRecv = 1;
}

static inline void SetChannelTime(PPWLobbyWork *work, Val time) {
    work->schedule.hasTime = 1;
    work->schedule.time = time;
}

static inline void OnPairFn(const u8 *d) {
    SetChannelTime(gPPWLobbyWork, Val(d));
}

extern "C" int PPWLobby_HandleLibraryData(int a0, int a1, int sel, int id, const char *cmd, const u8 *data, u32 len) {
    if (sel) {
        return 0;
    }
    if (strcmp(cmd, sStr_b_lib_u_user) == 0) {
        if (id == gPPWLobbyWork->userId) {
            return 1;
        }
        Rec *r = PPWLobbyPlayers_Find(&gPPWLobbyWork->players, id);
        if (!r) {
            return 1;
        }
        TU al;
        r->SetVec(data, len, al);
        if (r->valid) {
            gPPWLobbyWork->callbacks.onPlayerUpdate(id, data, len);
        }
        return 1;
    }
    if (strcmp(cmd, sStr_b_lib_u_system) == 0) {
        if (id == gPPWLobbyWork->userId) {
            return 1;
        }
        if (len != 16) {
            return 1;
        }
        Rec *r = PPWLobbyPlayers_Find(&gPPWLobbyWork->players, id);
        if (!r) {
            return 1;
        }
        r->SetQuad(data);
        if (r->valid && r->q.changed) {
            r->q.changed = 0;
            Triple t = r->GetTriple();
            gPPWLobbyWork->callbacks.onPlayerRecordChanged(id, &t);
        }
        return 1;
    }
    if (strcmp(cmd, sStr_b_lib_c_time) == 0) {
        if (len != 8) {
            return 1;
        }
        OnPairFn(data);
        return 1;
    }
    if (strcmp(cmd, sStr_b_lib_c_lobby) == 0) {
        if (!PPWLobbySchedule_Load(&gPPWLobbyWork->schedule, data, len)) {
            return 1;
        }
        gPPWLobbyWork->schedule.state = 4;
        return 1;
    }
    return 0;
}

}

namespace HandleSystemMessage {
extern "C" {

typedef struct {
    u32 pad[2];
    int mode;
    u32 gap;
    int selector;
} Obj;

extern PPWLobbyWork *gPPWLobbyWork;
PPWLobbyPlayer *PPWLobbyPlayers_Find(void *, int);
void PPWLobbySchedule_Start(void *);

static inline BOOL RecordValid(PPWLobbyPlayer *r) {
    return r && r->entered != 0;
}

int PPWLobby_HandleSystemMessage(Obj *, int, int, void *, int);

int PPWLobby_HandleSystemMessage(Obj *obj, int id, int flag, void *payload, int message) {
    PPWLobbyPlayer *r;
    BOOL valid;
    if (flag) {
        return 1;
    }
    switch (obj->selector) {
    case 1:
        if (id == gPPWLobbyWork->userId) {
            break;
        }
        r = PPWLobbyPlayers_Find(&gPPWLobbyWork->players, id);
        valid = RecordValid(r);
        switch (valid) {
        case 0:
            goto done;
        default:
            break;
        }
        if (obj->mode == 1 && message == 0x44) {
            gPPWLobbyWork->callbacks.onMatchInfo(id, payload);
        }
        break;
    case 2:
        if (id == gPPWLobbyWork->userId) {
            break;
        }
        r = PPWLobbyPlayers_Find(&gPPWLobbyWork->players, id);
        valid = RecordValid(r);
        switch (valid) {
        case 0:
            goto done;
        default:
            break;
        }
        if (obj->mode == 1 && message == 0x44) {
            gPPWLobbyWork->callbacks.onMatchClosed(id, payload);
        }
        break;
    case 0:
        if (obj->mode == 1 && message == 4) {
            PPWLobbySchedule_Start(&gPPWLobbyWork->schedule);
        }
        break;
    }
done:
    return 1;
}
}
}

namespace IsMainChannelReady {
extern "C" {

typedef union {
    struct {
        u32 large : 1, other : 31;
        u32 size;
        char *ptr;
    } heap;

    struct {
        u8 header;
        char data[11];
    } small;
} String;

extern PPWLobbyWork *gPPWLobbyWork;
extern void PPWLobbyChannel_GetName(String *, void *), LobbyString_Destroy(String *);
extern int LobbyChat_IsInChannel(char *), PPWLobbyPlayers_AllHaveProfiles(void *);
int PPWLobby_IsMainChannelReady(void);

static inline int enough(s32 n) {
    int result;
    if (n >= 4) {
        result = 1;
    } else {
        result = 0;
    }
    return result;
}

int PPWLobby_IsMainChannelReady(void) {
    String local;
    int empty;
    PPWLobbyWork *c;
    PPWLobbyChannel_GetName(&local, &gPPWLobbyWork->mainChannel);
    empty = LobbyChat_IsInChannel(!local.heap.large ? local.small.data : local.heap.ptr) == 0;
    LobbyString_Destroy(&local);
    if (empty) {
        return 0;
    }
    c = gPPWLobbyWork;
    if (c->mainChannel.usersEnumerated == 0) {
        return 0;
    }
    if (c->mainChannel.modeReady == 0) {
        return 0;
    }
    if (c->schedule.hasTime == 0) {
        return 0;
    }
    if (!enough(c->schedule.state)) {
        return 0;
    }
    return PPWLobbyPlayers_AllHaveProfiles(&c->players) != 0;
}
}
}

#pragma push

namespace TimerUploadUserProfile {
extern "C" {
#pragma opt_common_subs off

typedef union Str {
    struct {
        u32 large : 1;
        u32 other : 31;
        u32 length;
        char *ptr;
    } f;

    struct {
        u8 bit : 1;
        u8 size : 7;
        u8 rest[11];
    } small;

    struct {
        u8 header;
        char data[11];
    } shortData;

    u8 bytes[12];
} Str;

extern PPWLobbyWork *gPPWLobbyWork;
extern char sStr_b_lib_u_user[];
extern int LobbyString_Compare(Str *, Str *);
extern void *LobbyByteVector_Data(void *);
extern void PPWLobby_SetChannelData(u32, u32, char *, void *, u32);
int LobbyString_Equals(Str *, Str *);
int PPWLobby_TimerUploadUserProfile(void);

static inline int flush(PPWLobbyWork *work) {
    u32 userId;
    void *buf;
    if (work->players.self.userProfileChanged == 0) {
        return 1;
    }
    work->players.self.userProfileChanged = 0;
    userId = gPPWLobbyWork->userId;
    buf = LobbyByteVector_Data(&work->players.self.userProfile);
    PPWLobby_SetChannelData(userId, 0, sStr_b_lib_u_user, buf, work->players.self.userProfile.size);
    return 1;
}

extern void PPWLobbyChannel_GetName(Str *, void *);
extern int LobbyChat_IsInChannel(char *);
extern void LobbyString_Destroy(Str *);
extern int PPWLobbyPlayers_AllHaveProfiles(void *);
int PPWLobby_IsMainChannelReady(void);

static inline BOOL ready(int n) {
    return n >= 4;
}

int PPWLobby_TimerUploadUserProfile(void) {
    return flush(gPPWLobbyWork);
}
}
}

#pragma pop

namespace ChannelTimers {
extern "C" {
extern PPWLobbyWork *gPPWLobbyWork;
extern char *gPPWLobbyChannelNamePrefixes[];
extern void PPWLobbyChannel_GetName(Str *, void *), LobbyString_Reserve(Str *, u32);
extern int LobbyString_Compare(Str *, Str *), LobbyChat_IsInChannel(const char *), PPWLobbyPlayers_AllHaveProfiles(void *), PPWLobby_SetChannelData(u32, u32, const void *, const void *, u32);
extern void *LobbyByteVector_Data(void *);
extern u32 current_time(void);
extern void LobbyString_Format(Str *, const char *, ...);
extern void PPWLobby_SendMessage(u32, u32, u32, u32, u32, const void *, u32), LobbyChat_GetChannelModeAsync(const char *, void (*)(void), s32), LobbyChat_GetChannelKeysAsync(const char *, s32, const char *, void (*)(void), s32);
extern void PPWLobby_OnGetChannelMode(void), PPWLobby_OnGetChannelTime(void), PPWLobby_OnGetLobbyData(void), PPWLobbySchedule_SetQuestionnaire(void *, void *);
extern Str *LobbyString_ConcatCStr(Str *, Str *, char *);
extern Str *LobbyString_Concat(Str *, Str *, Str *);
extern char sStr_Bang[];
extern char sFmt_C_IntUnderscoreUint[];
extern char sStr_b_lib_u_user[];
extern char sStr_b_lib_u_system[];
extern char sStr_b_lib_c_time[];
extern char sStr_b_lib_c_lobby[];
void PPWLobby_MakeChannelName(Str *out, u32 id, Str *s);
void PPWLobby_GenerateChannelBaseName(Str *out);
int LobbyString_Equals(Str *a, Str *b);
int PPWLobby_IsMainChannelReady(void);
int PPWLobby_TimerUploadUserProfile(void);
int PPWLobby_TimerUploadSystemProfile(void);
int PPWLobby_TimerSendRecruitInfo(void);
int PPWLobby_TimerPollChannelMode(void);
int PPWLobby_TimerRequestChannelTime(void);
int PPWLobby_TimerRequestLobbyData(void);
int PPWLobby_GetWebResultError(u32 type);
int PPWLobby_ParseWebResponse(int valid, u8 *data, u32 size, void **output, u32 *length);
void PPWLobby_OnGetQuestionnaire(int valid, u8 *data, u32 size);
void PPWLobby_OnSubmitQuestionnaire(int status);

extern Str *LobbyString_ReplaceRange(Str *, u32, u32, const char *, const char *, u8);

static inline u32 short_length(Str *p) {
    if (p->l.flag) {
        return p->l.size;
    }
    return p->s.size;
}

int PPWLobby_TimerUploadSystemProfile(void) {
    PPWLobbyWork *work = gPPWLobbyWork;
    if (!work->players.self.systemProfileChanged) {
        return 1;
    }
    work->players.self.systemProfileChanged = 0;
    PPWLobby_SetChannelData(gPPWLobbyWork->userId, 0, sStr_b_lib_u_system, &work->players.self.systemProfile, 16);
    return 1;
}

int PPWLobby_TimerSendRecruitInfo(void) {
    PPWLobbyWork *work = gPPWLobbyWork;
    if (!work->recruiting) {
        return 0;
    }
    PPWLobby_SendMessage(work->userId, 0, 1, 0, 1, work->recruitInfo, 0x44);
    return 1;
}

int PPWLobby_TimerPollChannelMode(void) {
    Str s;
    PPWLobbyChannel_GetName(&s, &gPPWLobbyWork->mainChannel);
    LobbyChat_GetChannelModeAsync(!s.l.flag ? s.s.data : s.l.data, PPWLobby_OnGetChannelMode, 0);
    LobbyString_Destroy(&s);
    return 1;
}

int PPWLobby_TimerRequestChannelTime(void) {
    Str s;
    if (gPPWLobbyWork->schedule.hasTime != 0) {
        return 0;
    }
    PPWLobbyChannel_GetName(&s, &gPPWLobbyWork->mainChannel);
    LobbyChat_GetChannelKeysAsync(!s.l.flag ? s.s.data : s.l.data, -1, sStr_b_lib_c_time, PPWLobby_OnGetChannelTime, 0);
    LobbyString_Destroy(&s);
    return 1;
}
}
}

namespace TimerRequestLobbyData {
extern "C" {
extern PPWLobbyWork *gPPWLobbyWork;
extern u8 sStr_b_lib_c_lobby[];
extern void PPWLobby_OnGetLobbyData(void), PPWLobbyChannel_GetName(void *, void *), LobbyString_Destroy(void *), LobbyChat_GetChannelKeysAsync(void *, int, void *, void (*)(void), int);

typedef union {
    struct {
        u32 flag : 1, other : 31;
        u32 a;
        void *pointer;
    } longform;

    struct {
        u8 prefix;
        u8 data[11];
    } shortform;
} String;

static inline BOOL limit(int value) {
    return value >= 4;
}

int PPWLobby_TimerRequestLobbyData(void);

int PPWLobby_TimerRequestLobbyData(void) {
    String s;
    switch (limit(gPPWLobbyWork->schedule.state)) {
    case 0:
        break;
    default:
        return 0;
    }
    PPWLobbyChannel_GetName(&s, &gPPWLobbyWork->mainChannel);
    LobbyChat_GetChannelKeysAsync(s.longform.flag == 0 ? s.shortform.data : s.longform.pointer, -1, sStr_b_lib_c_lobby, PPWLobby_OnGetLobbyData, 0);
    LobbyString_Destroy(&s);
    return 1;
}
}
}

namespace WebResponse {
extern "C" {
extern char *gPPWLobbyChannelNamePrefixes[];
extern void PPWLobbyChannel_GetName(Str *, void *), LobbyString_Reserve(Str *, u32);
extern int LobbyString_Compare(Str *, Str *), LobbyChat_IsInChannel(const char *), PPWLobbyPlayers_AllHaveProfiles(void *), PPWLobby_SetChannelData(u32, u32, const void *, const void *, u32);
extern void *LobbyByteVector_Data(void *);
extern u32 current_time(void);
extern void LobbyString_Format(Str *, const char *, ...);
extern void PPWLobby_SendMessage(u32, u32, u32, u32, u32, const void *, u32), LobbyChat_GetChannelModeAsync(const char *, void (*)(void), s32), LobbyChat_GetChannelKeysAsync(const char *, s32, const char *, void (*)(void), s32);
extern void PPWLobby_OnGetChannelMode(void), PPWLobby_OnGetChannelTime(void), PPWLobby_OnGetLobbyData(void), PPWLobbySchedule_SetQuestionnaire(void *, void *);
extern Str *LobbyString_ConcatCStr(Str *, Str *, char *);
extern Str *LobbyString_Concat(Str *, Str *, Str *);
extern char sStr_Bang[];
extern char sFmt_C_IntUnderscoreUint[];
extern char sStr_b_lib_u_user[];
extern char sStr_b_lib_u_system[];
extern char sStr_b_lib_c_time[];
extern char sStr_b_lib_c_lobby[];
void PPWLobby_MakeChannelName(Str *out, u32 id, Str *s);
void PPWLobby_GenerateChannelBaseName(Str *out);
int LobbyString_Equals(Str *a, Str *b);
int PPWLobby_IsMainChannelReady(void);
int PPWLobby_TimerUploadUserProfile(void);
int PPWLobby_TimerUploadSystemProfile(void);
int PPWLobby_TimerSendRecruitInfo(void);
int PPWLobby_TimerPollChannelMode(void);
int PPWLobby_TimerRequestChannelTime(void);
int PPWLobby_TimerRequestLobbyData(void);
int PPWLobby_GetWebResultError(u32 type);
int PPWLobby_ParseWebResponse(int valid, u8 *data, u32 size, void **output, u32 *length);
void PPWLobby_OnGetQuestionnaire(int valid, u8 *data, u32 size);
void PPWLobby_OnSubmitQuestionnaire(int status);

extern Str *LobbyString_ReplaceRange(Str *, u32, u32, const char *, const char *, u8);

static inline u32 short_length(Str *p) {
    if (p->l.flag) {
        return p->l.size;
    }
    return p->s.size;
}

int PPWLobby_GetWebResultError(u32 type) {
    switch (type) {
    case 0:
        return 0;
    case 6:
        return 1;
    case 7:
    case 8:
    case 9:
    case 10:
        return 2;
    default:
        return 2;
    }
}

int PPWLobby_ParseWebResponse(int valid, u8 *data, u32 size, void **output, u32 *length) {
    int e;
    *output = 0;
    *length = 0;
    if (!valid) {
        return 2;
    }
    if (size < 4) {
        return 2;
    }
    e = PPWLobby_GetWebResultError(*data);
    if (e == 2) {
        return e;
    }
    if (e == 1) {
        return e;
    }
    *output = data + 4;
    *length = size - 4;
    return e;
}
}
}

namespace OnCheckProfile {
extern "C" void OS_GetMacAddress(u8 *);
struct Str;
extern "C" Str *LobbyString_InitFromCStr(Str *, const char *);

struct Str {
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

        u32 words[3];
    } raw;

    Str(const char *c) {
        LobbyString_InitFromCStr(this, c);
    }

    const char *CStr() const {
        return !raw.l.flag ? raw.s.data : raw.l.data;
    }
};

enum ReqMethod {
    REQ_METHOD_GET,
    REQ_METHOD_POST
};

struct Al {};

struct TA {};

struct TB {};

struct TC {};

struct TD {};

struct Iter {
    const u8 *p;
    u32 extra;

    Iter(const u8 *q)
        : p(q) {
    }
};
struct Vec;
extern "C" void LobbyCharVector_AssignIterRange(Vec *, Iter, Iter, TD);
extern "C" const char *LobbyByteVector_Data(const void *);

struct Vec {
    char *ptr;
    u32 size, cap;

    Vec()
        : ptr(0)
        , size(0)
        , cap(0) {
    }

    void Assign(Iter first, Iter last) {
        Assign(first, last, TA(), TB());
    }

    void Assign(Iter first, Iter last, const TA &, const TB &) {
        LobbyCharVector_AssignIterRange(this, first, last, TD());
    }

    void Assign(const u8 *first, const u8 *last) {
        Assign(Iter(first), Iter(last));
    }
};

struct Info {
    u32 x;
    u16 one;
    u8 a;
    u8 b;
    u8 mac[6];
    u8 rest[6];
};
struct Obj;

struct ObjVtbl {
    void (*f0)(Obj *);
    void (*del)(Obj *);
};

extern "C" const ObjVtbl gPPWLobbyWebRequestVtable;
extern "C" void PPWLobby_OnGetSchedule(void);

struct Obj {
    const ObjVtbl *vtbl;
    int kind;
    int flag;
    Str name;
    Str host;
    u32 x;
    Vec data;
    void (*cb)(void);
    int x38;
    int x3c;

    Obj(int f, const char *n, const char *h, u32 x_, const Info &info)
        : vtbl(&gPPWLobbyWebRequestVtable)
        , kind(3)
        , flag(f)
        , name(n)
        , host(h)
        , x(x_)
        , data()
        , cb(PPWLobby_OnGetSchedule)
        , x38(0)
        , x3c(1) {
        data.Assign(Iter((const u8 *)&info), Iter((const u8 *)&info + sizeof(Info)));
    }
};

extern "C" PPWLobbyWork *gPPWLobbyWork;
extern "C" Obj *gPPWLobbyCurrentRequest;
extern "C" int PPWLobby_ParseWebResponse(int, int, int, void **, u32 *);
extern "C" void *LobbyMem_Alloc(u32);
extern "C" void StatsUpload_Start(int, const char *);
extern "C" int StatsUpload_UploadWithLastHash(const char *, u32, const char *, u32, int);
extern "C" int StatsUpload_Upload(const char *, u32, const char *, u32, int);
extern "C" void StatsUpload_Stop(int);
extern "C" int PPW_LobbyUpdateMyProfile(const void *, u32, int);
extern "C" void PPWLobby_OnCheckProfile(int a0, int a1, int a2);

extern "C" void PPWLobby_OnCheckProfile(int a0, int a1, int a2) {
    Info info;
    const char *name;
    const char *host;
    Obj *o;
    u32 x;
    ReqMethod flag;
    void *buf;
    u32 len;
    PPWLobbyWork *w = gPPWLobbyWork;
    if (w == 0 || w->state == PPW_LOBBY_STATE_CLOSED) {
        return;
    }
    buf = 0;
    len = 0;
    int r = PPWLobby_ParseWebResponse(a0, a1, a2, &buf, &len);
    if (r == 0 && len != 0x94) {
        r = 2;
    }
    if (r == 0) {
        PPW_LobbyUpdateMyProfile(buf, len, 0);
    }
    gPPWLobbyWork->callbacks.onOwnRecord(r, buf, len);
    if (r != 0) {
        PPWLobbyWork *g = gPPWLobbyWork;
        g->error = 6;
        g->state = PPW_LOBBY_STATE_ERROR;
        return;
    }
    const PPWLobbyUserProfile *f = (const PPWLobbyUserProfile *)LobbyByteVector_Data(&gPPWLobbyWork->players.self.userProfile);
    info.x = gPPWLobbyWork->userId;
    info.one = 1;
    info.a = f->version;
    info.b = f->language;
    OS_GetMacAddress(info.mac);
    x = gPPWLobbyWork->userId;
    host = gPPWLobbyGetScheduleUrl;
    name = gPPWLobbyStatsUploadKey;
    flag = gPPWLobbyWork->unk_50 == 0 ? REQ_METHOD_GET : REQ_METHOD_POST;
    if (gPPWLobbyCurrentRequest != 0) {
        return;
    }
    o = (Obj *)LobbyMem_Alloc(sizeof(Obj));
    new (o) Obj(flag, name, host, x, info);
    gPPWLobbyCurrentRequest = o;
    StatsUpload_Start(o->flag, o->name.CStr());
    int ret;
    if (o->x3c) {
        u32 n = o->data.size;
        ret = StatsUpload_UploadWithLastHash(o->host.CStr(), o->x, LobbyByteVector_Data(&o->data), n, 0);
    } else {
        u32 n = o->data.size;
        ret = StatsUpload_Upload(o->host.CStr(), o->x, LobbyByteVector_Data(&o->data), n, 0);
    }
    int ok;
    if (ret == 0) {
        ok = 1;
    } else {
        StatsUpload_Stop(ret);
        ok = 0;
    }
    if (ok) {
        return;
    }
    if (gPPWLobbyCurrentRequest != 0) {
        Obj *p = gPPWLobbyCurrentRequest;
        if (p != 0) {
            p->vtbl->del(p);
        }
        gPPWLobbyCurrentRequest = 0;
    }
}

}

#pragma push

namespace OnGetSchedule {
extern "C" void OS_GetMacAddress(u8 *);
struct Str;
extern "C" Str *LobbyString_InitFromCStr(Str *, const char *);

struct Str {
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

        u32 words[3];
    } raw;

    Str(const char *c) {
        LobbyString_InitFromCStr(this, c);
    }

    const char *CStr() const {
        return !raw.l.flag ? raw.s.data : raw.l.data;
    }
};

enum ReqMethod {
    REQ_METHOD_GET,
    REQ_METHOD_POST
};

struct Al {};

struct TA {};

struct TB {};

struct TC {};

struct TD {};

struct Iter {
    const u8 *p;
    u32 extra;

    Iter(const u8 *q)
        : p(q) {
    }
};
struct Vec;
extern "C" void LobbyCharVector_AssignIterRange(Vec *, Iter, Iter, TD);
extern "C" const char *LobbyByteVector_Data(const void *);

struct Vec {
    char *ptr;
    u32 size, cap;

    Vec()
        : ptr(0)
        , size(0)
        , cap(0) {
    }

    void Assign(const Iter &first, const Iter &last) {
        Assign(first, last, TA(), TB());
    }

    void Assign(const Iter &first, const Iter &last, const TA &, const TB &) {
        LobbyCharVector_AssignIterRange(this, first, last, TD());
    }

    void Assign(const u8 *first, const u8 *last) {
        Assign(Iter(first), Iter(last));
    }
};

struct Info {
    u32 x;
    u16 one;
    u8 a;
    u8 b;
    u8 mac[6];
    u16 r14;
    u32 r16;
};

struct Info2 {
    Info info;
    u32 extra;
};
struct Obj;
extern "C" void PPWLobby_OnGetVip(int, int, int);

struct ObjVtbl {
    void (*f0)(Obj *);
    void (*del)(Obj *);
};

extern "C" const ObjVtbl gPPWLobbyWebRequestVtable;

struct Obj {
    const ObjVtbl *vtbl;
    int kind;
    int flag;
    Str name;
    Str host;
    u32 x;
    Vec data;
    void (*cb)(int, int, int);
    int x38;
    int x3c;

    Obj(int f, const char *n, const char *h, u32 x_, const Info2 &info)
        : vtbl(&gPPWLobbyWebRequestVtable)
        , kind(3)
        , flag(f)
        , name(n)
        , host(h)
        , x(x_)
        , data()
        , cb(PPWLobby_OnGetVip)
        , x38(0)
        , x3c(1) {
        data.Assign(Iter((const u8 *)&info), Iter((const u8 *)&info + sizeof(Info2)));
    }
};

extern "C" PPWLobbyWork *gPPWLobbyWork;
extern "C" Obj *gPPWLobbyCurrentRequest;
extern "C" int PPWLobby_ParseWebResponse(int, int, int, void **, u32 *);
extern "C" int PPW_LobbyUpdateMyProfile(const void *, u32, int);
extern "C" void *LobbyMem_Alloc(u32);
extern "C" void StatsUpload_Start(int, const char *);
extern "C" int StatsUpload_UploadWithLastHash(const char *, u32, const char *, u32, int);
extern "C" int StatsUpload_Upload(const char *, u32, const char *, u32, int);
extern "C" void StatsUpload_Stop(int);
extern "C" int PPWLobbySchedule_IsSizeValid(void *, void *, u32);
extern "C" void PPWLobbySchedule_Load(void *, void *, u32);
extern "C" void PPWLobby_OnGetSchedule(int a0, int a1, int a2);
#pragma opt_dead_assignments off

extern "C" void PPWLobby_OnGetSchedule(int a0, int a1, int a2) {
    Info2 i2;
    Info info;
    const char *name;
    const char *host;
    Obj *o;
    u32 x;
    ReqMethod flag;
    void *buf;
    u32 len;
    PPWLobbyWork *w = gPPWLobbyWork;
    if (w == 0 || w->state == PPW_LOBBY_STATE_CLOSED) {
        return;
    }
    buf = 0;
    len = 0;
    int r = PPWLobby_ParseWebResponse(a0, a1, a2, &buf, &len);
    if (r == 0 && !PPWLobbySchedule_IsSizeValid(&gPPWLobbyWork->schedule, buf, len)) {
        r = 2;
    }
    int ok2 = gPPWLobbyWork->callbacks.onEvent12Result(r, buf);
    if (r != 0) {
        PPWLobbyWork *g = gPPWLobbyWork;
        g->error = 4;
        g->state = PPW_LOBBY_STATE_ERROR;
        return;
    }
    if (ok2) {
        PPWLobbySchedule_Load(&gPPWLobbyWork->schedule, buf, len);
    }
    const PPWLobbyUserProfile *p = (const PPWLobbyUserProfile *)LobbyByteVector_Data(&gPPWLobbyWork->players.self.userProfile);
    int ret = 0;
    std::memset(&i2, 0, sizeof(i2));
    const PPWLobbyUserProfile *f = (const PPWLobbyUserProfile *)LobbyByteVector_Data(&gPPWLobbyWork->players.self.userProfile);
    info.x = gPPWLobbyWork->userId;
    info.one = 1;
    info.a = f->version;
    info.b = f->language;
    OS_GetMacAddress(info.mac);
    i2.info = info;
    i2.extra = p->trainerId;
    x = gPPWLobbyWork->userId;
    host = gPPWLobbyGetVipUrl;
    name = gPPWLobbyStatsUploadKey;
    flag = gPPWLobbyWork->unk_50 == 0 ? REQ_METHOD_GET : REQ_METHOD_POST;
    if (gPPWLobbyCurrentRequest != 0) {
        return;
    }
    o = (Obj *)LobbyMem_Alloc(sizeof(Obj));
    new (o) Obj(flag, name, host, x, i2);
    gPPWLobbyCurrentRequest = o;
    StatsUpload_Start(o->flag, o->name.CStr());
    if (o->x3c) {
        u32 n = o->data.size;
        ret = StatsUpload_UploadWithLastHash(o->host.CStr(), o->x, LobbyByteVector_Data(&o->data), n, 0);
    } else {
        u32 n = o->data.size;
        ret = StatsUpload_Upload(o->host.CStr(), o->x, LobbyByteVector_Data(&o->data), n, 0);
    }
    int ok;
    if (ret == 0) {
        ok = 1;
    } else {
        StatsUpload_Stop(ret);
        ok = 0;
    }
    if (ok) {
        return;
    }
    if (gPPWLobbyCurrentRequest != 0) {
        Obj *q = gPPWLobbyCurrentRequest;
        if (q != 0) {
            q->vtbl->del(q);
        }
        gPPWLobbyCurrentRequest = 0;
    }
}

}

#pragma pop

namespace OnGetVip {
extern "C" void OS_GetMacAddress(u8 *);
struct Str;
extern "C" Str *LobbyString_InitFromCStr(Str *, const char *);

struct Str {
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

        u32 words[3];
    } raw;

    Str(const char *c) {
        LobbyString_InitFromCStr(this, c);
    }

    const char *CStr() const {
        return !raw.l.flag ? raw.s.data : raw.l.data;
    }
};

enum ReqMethod {
    REQ_METHOD_GET,
    REQ_METHOD_POST
};

struct Al {};

struct TA {};

struct TB {};

struct TC {};

struct TD {};

struct Iter {
    const u8 *p;
    u32 extra;

    Iter(const u8 *q)
        : p(q) {
    }
};
struct Vec;
extern "C" void LobbyCharVector_AssignIterRange(Vec *, Iter, Iter, TD);
extern "C" const char *LobbyByteVector_Data(const void *);

struct Vec {
    char *ptr;
    u32 size, cap;

    Vec()
        : ptr(0)
        , size(0)
        , cap(0) {
    }

    void Assign(Iter first, Iter last) {
        Assign(first, last, TA(), TB());
    }

    void Assign(Iter first, Iter last, const TA &, const TB &) {
        LobbyCharVector_AssignIterRange(this, first, last, TD());
    }

    void Assign(const u8 *first, const u8 *last) {
        Assign(Iter(first), Iter(last));
    }
};

struct Info {
    u32 x;
    u16 one;
    u8 a;
    u8 b;
    u8 mac[6];
    u8 rest[6];
};
struct Obj;

struct ObjVtbl {
    void (*f0)(Obj *);
    void (*del)(Obj *);
};

extern "C" const ObjVtbl gPPWLobbyWebRequestVtable;
extern "C" void PPWLobby_OnGetQuestionnaire(void);

struct Obj {
    const ObjVtbl *vtbl;
    int kind;
    int flag;
    Str name;
    Str host;
    u32 x;
    Vec data;
    void (*cb)(void);
    int x38;
    int x3c;

    Obj(int f, const char *n, const char *h, u32 x_, const Info &info)
        : vtbl(&gPPWLobbyWebRequestVtable)
        , kind(3)
        , flag(f)
        , name(n)
        , host(h)
        , x(x_)
        , data()
        , cb(PPWLobby_OnGetQuestionnaire)
        , x38(0)
        , x3c(1) {
        data.Assign(Iter((const u8 *)&info), Iter((const u8 *)&info + sizeof(Info)));
    }
};

extern "C" PPWLobbyWork *gPPWLobbyWork;
extern "C" Obj *gPPWLobbyCurrentRequest;
extern "C" int PPWLobby_ParseWebResponse(int, int, int, void **, u32 *);
extern "C" void *LobbyMem_Alloc(u32);
extern "C" void StatsUpload_Start(int, const char *);
extern "C" int StatsUpload_UploadWithLastHash(const char *, u32, const char *, u32, int);
extern "C" int StatsUpload_Upload(const char *, u32, const char *, u32, int);
extern "C" void StatsUpload_Stop(int);
extern "C" int PPWLobbySchedule_ValidateVipRecords(void *, void *, u32);
extern "C" void PPWLobbySchedule_SetVipRecords(void *, void *, u32);
extern "C" void PPWLobby_OnGetVip(int a0, int a1, int a2);

extern "C" void PPWLobby_OnGetVip(int a0, int a1, int a2) {
    Info info;
    const char *name;
    const char *host;
    Obj *o;
    u32 x;
    ReqMethod flag;
    void *buf;
    u32 len;
    PPWLobbyWork *w = gPPWLobbyWork;
    if (w == 0 || w->state == PPW_LOBBY_STATE_CLOSED) {
        return;
    }
    buf = 0;
    len = 0;
    int r = PPWLobby_ParseWebResponse(a0, a1, a2, &buf, &len);
    if (!PPWLobbySchedule_ValidateVipRecords(&gPPWLobbyWork->schedule, buf, len)) {
        r = 2;
    }
    if (r != 0) {
        PPWLobbyWork *g = gPPWLobbyWork;
        g->error = 5;
        g->state = PPW_LOBBY_STATE_ERROR;
        return;
    }
    PPWLobbySchedule_SetVipRecords(&gPPWLobbyWork->schedule, buf, len);
    gPPWLobbyWork->callbacks.onSpecialVisitorsReceived(r, buf, len >> 3);
    const PPWLobbyUserProfile *f = (const PPWLobbyUserProfile *)LobbyByteVector_Data(&gPPWLobbyWork->players.self.userProfile);
    info.x = gPPWLobbyWork->userId;
    info.one = 1;
    info.a = f->version;
    info.b = f->language;
    OS_GetMacAddress(info.mac);
    x = gPPWLobbyWork->userId;
    host = gPPWLobbyGetQuestionnaireUrl;
    name = gPPWLobbyStatsUploadKey;
    flag = gPPWLobbyWork->unk_50 == 0 ? REQ_METHOD_GET : REQ_METHOD_POST;
    if (gPPWLobbyCurrentRequest != 0) {
        return;
    }
    o = (Obj *)LobbyMem_Alloc(sizeof(Obj));
    new (o) Obj(flag, name, host, x, info);
    gPPWLobbyCurrentRequest = o;
    StatsUpload_Start(o->flag, o->name.CStr());
    int ret;
    if (o->x3c) {
        u32 n = o->data.size;
        ret = StatsUpload_UploadWithLastHash(o->host.CStr(), o->x, LobbyByteVector_Data(&o->data), n, 0);
    } else {
        u32 n = o->data.size;
        ret = StatsUpload_Upload(o->host.CStr(), o->x, LobbyByteVector_Data(&o->data), n, 0);
    }
    int ok;
    if (ret == 0) {
        ok = 1;
    } else {
        StatsUpload_Stop(ret);
        ok = 0;
    }
    if (ok) {
        return;
    }
    if (gPPWLobbyCurrentRequest != 0) {
        Obj *p = gPPWLobbyCurrentRequest;
        if (p != 0) {
            p->vtbl->del(p);
        }
        gPPWLobbyCurrentRequest = 0;
    }
}

}

namespace OnGetQuestionnaire {
extern "C" {

extern PPWLobbyWork *gPPWLobbyWork;
extern int PPWLobby_ParseWebResponse(void *, void *, void *, void **, u32 *);
extern void PPWLobbySchedule_SetQuestionnaire(void *, void *);
void PPWLobby_OnGetQuestionnaire(void *, void *, void *);

void PPWLobby_OnGetQuestionnaire(void *a, void *b, void *c) {
    void *data;
    u32 size;
    int result;
    PPWLobbyWork *w = gPPWLobbyWork;
    if (!w || w->state == PPW_LOBBY_STATE_CLOSED) {
        return;
    }
    data = 0;
    size = 0;
    result = PPWLobby_ParseWebResponse(a, b, c, &data, &size);
    if (size != 0x2d8) {
        result = 2;
    }
    if (result) {
        {
            PPWLobbyWork *target = gPPWLobbyWork;
            target->error = 7;
            target->state = PPW_LOBBY_STATE_ERROR;
        }
        return;
    }
    PPWLobbySchedule_SetQuestionnaire(&gPPWLobbyWork->schedule, data);
    gPPWLobbyWork->callbacks.onSurveyReceived(result, data);
    gPPWLobbyWork->schedule.state = 2;
}
}
}

namespace OnSubmitQuestionnaire {
extern "C" {
extern PPWLobbyWork *gPPWLobbyWork;
extern char *gPPWLobbyChannelNamePrefixes[];
extern void PPWLobbyChannel_GetName(Str *, void *), LobbyString_Reserve(Str *, u32);
extern int LobbyString_Compare(Str *, Str *), LobbyChat_IsInChannel(const char *), PPWLobbyPlayers_AllHaveProfiles(void *), PPWLobby_SetChannelData(u32, u32, const void *, const void *, u32);
extern void *LobbyByteVector_Data(void *);
extern u32 current_time(void);
extern void LobbyString_Format(Str *, const char *, ...);
extern void PPWLobby_SendMessage(u32, u32, u32, u32, u32, const void *, u32), LobbyChat_GetChannelModeAsync(const char *, void (*)(void), s32), LobbyChat_GetChannelKeysAsync(const char *, s32, const char *, void (*)(void), s32);
extern void PPWLobby_OnGetChannelMode(void), PPWLobby_OnGetChannelTime(void), PPWLobby_OnGetLobbyData(void), PPWLobbySchedule_SetQuestionnaire(void *, void *);
extern Str *LobbyString_ConcatCStr(Str *, Str *, char *);
extern Str *LobbyString_Concat(Str *, Str *, Str *);
extern char sStr_Bang[];
extern char sFmt_C_IntUnderscoreUint[];
extern char sStr_b_lib_u_user[];
extern char sStr_b_lib_u_system[];
extern char sStr_b_lib_c_time[];
extern char sStr_b_lib_c_lobby[];
void PPWLobby_MakeChannelName(Str *out, u32 id, Str *s);
void PPWLobby_GenerateChannelBaseName(Str *out);
int LobbyString_Equals(Str *a, Str *b);
int PPWLobby_IsMainChannelReady(void);
int PPWLobby_TimerUploadUserProfile(void);
int PPWLobby_TimerUploadSystemProfile(void);
int PPWLobby_TimerSendRecruitInfo(void);
int PPWLobby_TimerPollChannelMode(void);
int PPWLobby_TimerRequestChannelTime(void);
int PPWLobby_TimerRequestLobbyData(void);
int PPWLobby_GetWebResultError(u32 type);
int PPWLobby_ParseWebResponse(int valid, u8 *data, u32 size, void **output, u32 *length);
void PPWLobby_OnGetQuestionnaire(int valid, u8 *data, u32 size);
void PPWLobby_OnSubmitQuestionnaire(int status);

extern Str *LobbyString_ReplaceRange(Str *, u32, u32, const char *, const char *, u8);

static inline u32 short_length(Str *p) {
    if (p->l.flag) {
        return p->l.size;
    }
    return p->s.size;
}

void PPWLobby_OnSubmitQuestionnaire(int status) {
    if (!gPPWLobbyWork || gPPWLobbyWork->state == PPW_LOBBY_STATE_CLOSED) {
        return;
    }
    gPPWLobbyWork->submittingQuestionnaire = 0;
    gPPWLobbyWork->callbacks.onSurveySubmitted(status ? 0 : 2);
}
}
}

namespace OnEnterMainChannel {
using std::memcmp;

typedef struct RTCDate {
    u32 year, month, day, week;
} RTCDate;

typedef struct RTCTime {
    u32 hour, minute, second;
} RTCTime;

extern "C" s64 RTC_ConvertDateTimeToSecond(const RTCDate *date, const RTCTime *time);
extern "C" void DWC_GetDateTime(RTCDate *date, RTCTime *time);

struct TrueType {};

struct FalseType {};

template <class T, class U>
inline void Assign(T &d, const U &s, TrueType, FalseType) {
    d = s;
}

template <class T, class U>
inline void Assign(T &d, const U &s) {
    Assign(d, s, TrueType(), FalseType());
}

struct Str {
    u32 words[3];

    Str() {
        u32 *p = words;
        u32 i;
        for (i = 0; i < 3; i++) {
            p[i] = 0;
        }
    }
};

extern "C" void LobbyString_Destroy(Str *s);
extern "C" void LobbyString_Assign(Str *dst, const Str *src);

typedef struct Vec {
    void *ptr;
    u32 size, cap;
} Vec;

extern "C" void *LobbyByteVector_Data(Vec *v);

static inline s64 GetNow(void) {
    RTCDate date;
    RTCTime time;
    DWC_GetDateTime(&date, &time);
    return RTC_ConvertDateTimeToSecond(&date, &time);
}

struct Pair {
    s64 v;

    Pair() {
    }

    Pair(s64 x)
        : v(x) {
    }
};

struct Stamp {
    u32 code;
    Pair pair;
    u32 extra;

    Stamp(u32 c)
        : code(c)
        , pair(0)
        , extra(0) {
        Pair *d = &pair;
        *d = Pair(GetNow());
    }

    Stamp(const Stamp &o)
        : code(o.code) {
        Pair *d = &pair;
        *d = o.pair;
        extra = o.extra;
    }

    Stamp &operator=(const Stamp &o) {
        code = o.code;
        pair = o.pair;
        extra = o.extra;
        return *this;
    }

    bool operator==(const Stamp &o) const {
        return memcmp(this, &o, sizeof(Stamp)) == 0;
    }
};

struct Tracked {
    Stamp stamp;
    int active;

    Tracked(u32 c, int a)
        : stamp(Stamp(c))
        , active(a) {
    }

    void Set(const Tracked &t) {
        const Stamp *s = &t.stamp;
        if (!(stamp == *s)) {
            stamp = *s;
            active = 1;
        }
    }
};

struct Task {
    s64 time;
    s64 period;
    void *arg;
    int (*func)(void);

    Task() {
    }

    Task(s64 t, u32 p, int (*f)(void)) {
        time = t;
        period = p;
        arg = NULL;
        func = f;
        time -= period;
    }
};

template <class A, class B>
struct KV {
    A first;
    B second;

    template <class C, class D>
    KV(const KV<C, D> &p) {
        Assign(first, p.first);
        const D *src = &p.second;
        B *dst = &second;
        *dst = *src;
    }
};

struct InsertResult {
    void *node;
    bool inserted;
};
struct Map;
extern "C" void LobbyTimerMap_Insert(InsertResult *out, Map *m, const KV<u32, Task> *v);

struct Map {
    u32 words[4];

    InsertResult insert(const KV<u32, Task> &v) {
        InsertResult r;
        LobbyTimerMap_Insert(&r, this, &v);
        return r;
    }
};

extern "C" KV<int, Task> LobbyTimerMap_MakeEntry(int k, const Task &t);

struct Arguments {
    s64 time;
    u32 code;
};

struct Sub {
    void *vt;
    u32 tree[4];
    Tracked tracked;
    Vec vec;
    u8 pad34[0x3c - 0x34];
    u32 key;
    int flag;

    void SetFlag(int f) {
        flag = 0;
        if (f) {
            flag = 1;
        }
    }
};

struct Callbacks {
    void *cb8;
    void (*notify)(u32 key, Arguments *args, void *data, u32 size);
};

struct Peer {
    int kind;
    Str name;
    int a;
    int b;

    void Reset() {
        Str empty;
        kind = 5;
        LobbyString_Assign(&name, &empty);
        a = 0;
        b = 0;
        LobbyString_Destroy(&empty);
    }
};

typedef struct Lobby {
    int state;
    int err;
    Callbacks cbs;
    u8 pad10[0x70 - 0x10];
    int retry;
    u8 pad74[0xb8 - 0x74];
    Peer peer;
    u8 padd0[0xe8 - 0xd0];
    Sub sub;
    u8 pad12c[0x134 - 0x12c];
    Map timers;
} Lobby;

extern "C" Lobby *gPPWLobbyWork;
extern "C" void LobbyChat_EnumUsersAsync(void *handle, void (*cb)(void), u32 code);
extern "C" void PPWLobby_OnEnumUsers(void);
extern "C" int PPWLobby_TimerUploadSystemProfile(void);
extern "C" int PPWLobby_TimerUploadUserProfile(void);
extern "C" int PPWLobby_TimerRequestChannelTime(void);
extern "C" int PPWLobby_TimerPollChannelMode(void);
extern "C" int PPWLobby_TimerRequestLobbyData(void);

extern "C" void PPWLobby_OnEnterMainChannel(int success, void *unused, void *handle, u32 code);

static inline void Lobby_ResetStamp(void) {
    gPPWLobbyWork->retry = 0;
    Tracked t(5, 1);
    Sub *sub = &gPPWLobbyWork->sub;
    sub->tracked.Set(t);
    sub->SetFlag(1);
}

static inline void Lobby_AddTimer(int kind, u32 delay, int (*cb)(void)) {
    Map *m = &gPPWLobbyWork->timers;
    Task task(GetNow(), delay, cb);
    KV<int, Task> d = LobbyTimerMap_MakeEntry(kind, task);
    m->insert(d);
}

static inline Arguments Lobby_GetArgs(void) {
    const Stamp &s = gPPWLobbyWork->sub.tracked.stamp;
    Arguments a;
    a.code = s.code;
    a.time = s.pair.v;
    return a;
}

static inline void Lobby_Notify(Arguments *args) {
    Callbacks *cbs = &gPPWLobbyWork->cbs;
    Sub *sub = &gPPWLobbyWork->sub;
    u32 key = sub->key;
    cbs->notify(key, args, LobbyByteVector_Data(&sub->vec), sub->vec.size);
}

static inline int Lobby_Retry(Lobby *w) {
    if (w->retry >= 3) {
        return 0;
    }
    w->retry++;
    return 1;
}

static inline void Lobby_SetError(int code) {
    Lobby *w = gPPWLobbyWork;
    if (code != 0) {
        w->err = code;
        w->state = PPW_LOBBY_STATE_ERROR;
    }
}

static inline void Lobby_OnFail(void) {
    gPPWLobbyWork->peer.Reset();
    if (Lobby_Retry(gPPWLobbyWork)) {
        gPPWLobbyWork->state = PPW_LOBBY_STATE_ENUM_CHANNELS;
    } else {
        Lobby_SetError(3);
    }
}

extern "C" void PPWLobby_OnEnterMainChannel(int success, void *unused, void *handle, u32 code) {
    if (code == 5) {
        return;
    }
    if (success) {
        Lobby_ResetStamp();
        LobbyChat_EnumUsersAsync(handle, PPWLobby_OnEnumUsers, code);
        Lobby_AddTimer(1, 3, PPWLobby_TimerUploadSystemProfile);
        Lobby_AddTimer(2, 4, PPWLobby_TimerUploadUserProfile);
        Lobby_AddTimer(5, 4, PPWLobby_TimerRequestChannelTime);
        Lobby_AddTimer(4, 3, PPWLobby_TimerPollChannelMode);
        Lobby_AddTimer(6, 4, PPWLobby_TimerRequestLobbyData);
        Arguments args = Lobby_GetArgs();
        Lobby_Notify(&args);
    } else {
        Lobby_OnFail();
    }
}

}

namespace OnEnterSubChannel {
extern "C" int memcmp(const void *, const void *, unsigned);
class Str;
extern "C" Str *LobbyString_Destroy(Str *);
extern "C" Str *LobbyString_Assign(Str *, const Str *);

class Str {
public:
    u32 words[3];

    Str() {
        u32 i;
        u32 *p = words;
        for (i = 0; i < 3; i++) {
            p[i] = 0;
        }
    }

    void Destroy() {
        LobbyString_Destroy(this);
    }

    Str &operator=(const Str &o) {
        LobbyString_Assign(this, &o);
        return *this;
    }
};

struct InfoHead {
    int id;
};

struct Info : InfoHead {
    u64 mid;
    int z;

    Info() {
    }

    Info(const Info &o)
        : InfoHead(o) {
        *this = o;
    }

    bool operator==(const Info &o) const {
        return memcmp(this, &o, sizeof(Info)) == 0;
    }
};

struct Entry {
    Info info;
    int valid;

    Entry(const Info &i)
        : info(i)
        , valid(1) {
    }

    void Assign(const Info &i) {
        info = i;
        valid = 1;
    }
};

struct Peer {
    int kind;
    Str name;
    int a;
    int b;

    void Reset() {
        Str empty;
        kind = 5;
        name = empty;
        a = 0;
        b = 0;
        empty.Destroy();
    }
};

struct Work {
    int state;
    int sub;
    void (*callback)(int, int);
    u8 padc[0xd0 - 0xc];
    Peer peer;
    u8 pade8[0xfc - 0xe8];
    Entry entry;
    u8 pad110[0x128 - 0x110];
    int dirty;
};

extern "C" Work *gPPWLobbyWork;
extern "C" char gPPWLobbyFootChannelMode[], gPPWLobbyClockChannelMode[], gPPWLobbyNewsChannelMode[];
extern "C" void LobbyChat_SetChannelMode(int, const char *);
extern "C" void PPWLobby_OnEnterSubChannel(int, int, int, int);
#pragma opt_common_subs off

extern "C" void PPWLobby_OnEnterSubChannel(int result, int unused, int arg, int kind) {
    if (kind == 5) {
        return;
    }
    if (result != 0) {
        const char *name;
        switch (kind) {
        case 1:
        case 2:
            name = gPPWLobbyFootChannelMode;
            break;
        case 3:
            name = gPPWLobbyClockChannelMode;
            break;
        case 4:
            name = gPPWLobbyNewsChannelMode;
            break;
        default:
            name = 0;
            break;
        }
        if (name == 0) {
            Work *w = gPPWLobbyWork;
            w->sub = 1;
            w->state = PPW_LOBBY_STATE_ERROR;
        }
        LobbyChat_SetChannelMode(arg, name);
        {
            Work *w = gPPWLobbyWork;
            w->entry.valid = 0;
            Info info;
            info = w->entry.info;
            info.id = kind;
            Entry e(info);
            const Info &ni = e.info;
            if (!(w->entry.info == ni)) {
                w->entry.Assign(ni);
            }
            w->dirty = 1;
        }
    } else {
        gPPWLobbyWork->peer.Reset();
    }
    gPPWLobbyWork->callback(result, kind);
}

#pragma opt_common_subs reset

}

namespace OnPlayerJoined {
extern "C" int memcmp(const void *, const void *, unsigned);
struct Str;
extern "C" Str *LobbyString_InitFromCStr(Str *, char *);
extern "C" Str *LobbyString_Destroy(Str *);

struct Str {
    u32 words[3];

    void Destroy() {
        LobbyString_Destroy(this);
    }
};

struct InfoHead {
    int id;
};

struct Info : InfoHead {
    u64 mid;
    int z;

    Info() {
    }

    Info(const Info &o)
        : InfoHead(o) {
        *this = o;
    }

    bool operator==(const Info &o) const {
        return memcmp(this, &o, sizeof(Info)) == 0;
    }
};

struct Entry {
    Info info;
    int valid;

    Entry(const Info &i)
        : info(i)
        , valid(1) {
    }

    void Assign(const Info &i) {
        info = i;
        valid = 1;
    }
};

struct SummaryData {
    u64 mid;
    int id;
};

struct Summary {
    SummaryData d;

    Summary(const Info &i) {
        d.id = i.id;
        d.mid = i.mid;
    }

    Summary() {
    }

    Summary(const Summary &o)
        : d(o.d) {
    }
};

struct Session {
    u32 unk0;
    Entry entry;
    u8 pad18[0x30 - 0x18];
    int dirty;
    u8 pad34[4];
    int active;
};

extern "C" PPWLobbyWork *gPPWLobbyWork;
extern "C" void PPWLobbyChannel_GetName(Str *, PPWLobbyChannel *);
extern "C" int LobbyString_Equals(const Str *, const Str *);
extern "C" Session *PPWLobbyPlayers_Find(void *, int);
extern "C" void PPWLobbyPlayers_Remove(void *, int);
extern "C" void PPWLobby_OnPlayerJoined(char *, int);
extern "C" void PPWLobbyPlayers_Add(void *, int);

inline int Match(PPWLobbyChannel *c, const Str &name) {
    Str t;
    int r;
    PPWLobbyChannel_GetName(&t, c);
    r = LobbyString_Equals(&t, &name);
    t.Destroy();
    return r;
}

inline int Find(PPWLobbyWork *w, const Str &name) {
    const Str &n = name;
    if (Match(&w->mainChannel, n)) {
        return w->mainChannel.kind;
    }
    if (Match(&w->subChannel, n)) {
        return w->subChannel.kind;
    }
    return 5;
}

inline void UpdatePlayerChannel(Session *s, Info &info, Summary &arg, int kind) {
    s->entry.valid = 0;
    info = s->entry.info;
    info.id = kind;
    Entry e(info);
    const Info &ni = e.info;
    if (!(s->entry.info == ni)) {
        s->entry.Assign(ni);
    }
    s->dirty = 1;
    s->entry.valid = 0;
}

inline void MakeSummary(Session *s, Summary &arg) {
    Summary sum(s->entry.info);
    arg = sum;
}

#pragma opt_common_subs off

extern "C" void PPWLobby_OnPlayerJoined(char *channel, int id) {
    Info info;
    Summary arg;
    int kind;
    {
        Str name;
        LobbyString_InitFromCStr(&name, channel);
        kind = Find(gPPWLobbyWork, name);
        name.Destroy();
    }
    if (kind == 5) {
        return;
    }
    if (kind == 0) {
        PPWLobbyPlayers_Add(&gPPWLobbyWork->players, id);
        return;
    }
    Session *s = PPWLobbyPlayers_Find(&gPPWLobbyWork->players, id);
    if (s == 0) {
        PPWLobbyPlayers_Add(&gPPWLobbyWork->players, id);
        s = PPWLobbyPlayers_Find(&gPPWLobbyWork->players, id);
    }
    UpdatePlayerChannel(s, info, arg, kind);
    MakeSummary(s, arg);
    if (s->active) {
        gPPWLobbyWork->callbacks.onPlayerRecordChanged(id, &arg);
    }
}

#pragma opt_common_subs reset

}

namespace OnPlayerLeft {
extern "C" int memcmp(const void *, const void *, unsigned);
struct Str;
extern "C" Str *LobbyString_InitFromCStr(Str *, char *);
extern "C" Str *LobbyString_Destroy(Str *);

struct Str {
    u32 words[3];

    void Destroy() {
        LobbyString_Destroy(this);
    }
};

struct InfoHead {
    int id;
};

struct Info : InfoHead {
    u64 mid;
    int z;

    Info() {
    }

    Info(const Info &o)
        : InfoHead(o) {
        *this = o;
    }

    bool operator==(const Info &o) const {
        return memcmp(this, &o, sizeof(Info)) == 0;
    }
};

struct Entry {
    Info info;
    int valid;

    Entry(const Info &i)
        : info(i)
        , valid(1) {
    }

    void Assign(const Info &i) {
        info = i;
        valid = 1;
    }
};

struct SummaryData {
    u64 mid;
    int id;
};

struct Summary {
    SummaryData d;

    Summary(const Info &i) {
        d.id = i.id;
        d.mid = i.mid;
    }

    Summary() {
    }

    Summary(const Summary &o)
        : d(o.d) {
    }
};

struct Session {
    u32 unk0;
    Entry entry;
    u8 pad18[0x30 - 0x18];
    int dirty;
    u8 pad34[4];
    int active;
};

extern "C" PPWLobbyWork *gPPWLobbyWork;
extern "C" void PPWLobbyChannel_GetName(Str *, PPWLobbyChannel *);
extern "C" int LobbyString_Equals(const Str *, const Str *);
extern "C" Session *PPWLobbyPlayers_Find(void *, int);
extern "C" void PPWLobbyPlayers_Remove(void *, int);
extern "C" void PPWLobby_OnPlayerLeft(char *, int);

inline int Match(PPWLobbyChannel *c, const Str &name) {
    Str t;
    int r;
    PPWLobbyChannel_GetName(&t, c);
    r = LobbyString_Equals(&t, &name);
    t.Destroy();
    return r;
}

inline int Find(PPWLobbyWork *w, const Str &name) {
    const Str &n = name;
    if (Match(&w->mainChannel, n)) {
        return w->mainChannel.kind;
    }
    if (Match(&w->subChannel, n)) {
        return w->subChannel.kind;
    }
    return 5;
}

inline void UpdatePlayerChannel(Session *s, Info &info, Summary &arg) {
    s->entry.valid = 0;
    info = s->entry.info;
    info.id = 5;
    Entry e(info);
    const Info &ni = e.info;
    if (!(s->entry.info == ni)) {
        s->entry.Assign(ni);
    }
    s->dirty = 1;
    s->entry.valid = 0;
}

inline void MakeSummary(Session *s, Summary &arg) {
    Summary sum(s->entry.info);
    arg = sum;
}

#pragma opt_common_subs off

extern "C" void PPWLobby_OnPlayerLeft(char *channel, int id) {
    Info info;
    Summary arg;
    int kind;
    {
        Str name;
        LobbyString_InitFromCStr(&name, channel);
        kind = Find(gPPWLobbyWork, name);
        name.Destroy();
    }
    if (kind == 5) {
        return;
    }
    if (kind == 0) {
        Session *s = PPWLobbyPlayers_Find(&gPPWLobbyWork->players, id);
        if (s == 0 || s->active == 0) {
            return;
        }
        gPPWLobbyWork->callbacks.onPlayerLeave(id);
        PPWLobbyPlayers_Remove(&gPPWLobbyWork->players, id);
        return;
    }
    Session *s = PPWLobbyPlayers_Find(&gPPWLobbyWork->players, id);
    if (s == 0) {
        return;
    }
    UpdatePlayerChannel(s, info, arg);
    MakeSummary(s, arg);
    if (s->active) {
        gPPWLobbyWork->callbacks.onPlayerRecordChanged(id, &arg);
    }
}

#pragma opt_common_subs reset

}

namespace OnPlayerMessage {
extern "C" {
void ov45_02236E58(Iterator *);
extern int printf(const char *, ...);
extern void abort(void);

struct Str {
    union {
        u32 flags;

        struct {
            u32 large : 1;
            u32 cap : 31;
        } bits;
    } u;

    u32 size;
    char *ptr;
};

struct Vec {
    void *ptr;
    u32 size, cap;
};

struct TreeNode {
    void *left, *right;
    u32 parent;
    u32 key;
    u32 data[6];
};

struct Tree {
    u32 count;
    void *root;
    u32 unk;
    void *end;
};

struct Alloc {
    u8 tag;
};

extern u8 gPPWLobbyPlayersVtable[], sStr_b_lib_u_user[], sStr_b_lib_u_system[], gPPWLobbyMainChannelMode[];
extern void DWC_Free(int, void *, int);
extern void *LobbyMem_Alloc(u32);
extern struct Str *LobbyString_Destroy(struct Str *);
extern struct Vec *LobbyCharVector_Destroy(struct Vec *);
extern struct Str *LobbyString_InitSubstring(struct Str *, struct Str *, u32, u32);
extern void PPWLobby_OnMessage(int, int, int, int);
extern void PPWLobby_HandleReceivedData(int, int, int, int, int, int, int);
extern void PPWLobbyPlayers_Clear(void *), LobbyEventDequeMap_Destroy(void *), PPWLobbyPlayerSet_DestroyNodes(void *, void *);
extern void LobbyStringVector_RecommendSize(struct Vec *, u32, int), LobbyStringVector_AppendCopiesSlow(struct Vec *, u32, void *), LobbyStringVector_EraseAtEnd(struct Vec *, u32, struct Alloc), LobbyCharVector_AppendZerosSlow(struct Vec *, u32);
extern void LobbyCharVector_Allocate(struct Vec *, u32, struct Alloc);
extern void LobbyTree_InsertRebalance(struct TreeNode *, void *), LobbyTimerMap_DestroyNodes(struct Tree *, struct TreeNode *);
extern struct TreeNode *LobbyTree_EraseRebalance(struct TreeNode *, void *);
extern void LobbyTimerMap_EraseKey(void *, int *), LobbyChat_SetChannelMode(int, char *);
extern void PPWLobbyPlayers_Add(void *, int), LobbyChat_GetChannelKeysAsync(int, int, char *, void (*)(), int);
extern void PPWLobbyChannel_GetName(struct Str *, void *);
extern struct Str *LobbyString_InitFromCStr(struct Str *, char *);
extern int LobbyString_Equals(struct Str *, struct Str *);
extern void LobbyString_Reserve(struct Str *, u32);
extern u32 LobbyCharVector_RecommendSize(struct Vec *, u32);
extern struct Str *LobbyString_Replace(struct Str *, u32, u32, struct Str *);
extern struct Str *LobbyString_ReplaceRange(struct Str *, u32, u32, char *, char *, struct Alloc);
extern void _ZNSt15__vector_commonILb1EE18throw_length_errorEv(void);
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *PPWLobbyWebRequest_Destroy(u8 *a);
void *PPWLobbyPlayers_Delete(u8 *a);
void *PPWLobbyPlayer_Delete(u8 *a);
void *PPWLobbyPlayerSet_Destroy(struct Tree *a);
void LobbyTimerMap_Clear(struct Tree *a);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void LobbyStringVector_PushBackSlow(struct Vec *a, void *b, struct Alloc unused);
struct Vec *LobbyStringVector_Destroy(struct Vec *a);
struct Vec *LobbyU32Vector_Destroy(struct Vec *a);
struct Vec *LobbyVipVector_Destroy(struct Vec *a);
struct Vec *LobbyEventVector_Destroy(struct Vec *a);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
void LobbyCharVector_Allocate(struct Vec *a, u32 b, struct Alloc unused);
u32 LobbyCharVector_RecommendSize(struct Vec *a, u32 n);
struct Str *LobbyString_Destroy(struct Str *a);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);

extern void LobbyEventDequeMap_Destroy(void *), PPWLobbyPlayerSet_DestroyNodes(void *, void *);
extern struct TreeNode *LobbyTimerMap_FindInsertPos(struct Tree *, u32 *, struct TreeNode **, u8 *, u8 *);
extern struct TreeNode *LobbyTimerMap_InsertNode(struct Tree *, struct TreeNode *, int, int, u32 *);
extern void LobbyString_ReplaceSubstring(struct Str *, u32, u32, struct Str *, u32, u32);
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *PPWLobbyWebRequest_Destroy(u8 *a);
void *PPWLobbyPlayers_Delete(u8 *a);
void *PPWLobbyPlayer_Delete(u8 *a);
void *PPWLobbyPlayerSet_Destroy(struct Tree *a);
void LobbyTimerMap_Clear(struct Tree *a);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void LobbyStringVector_PushBackSlow(struct Vec *a, void *b, struct Alloc unused);
struct Vec *LobbyStringVector_Destroy(struct Vec *a);
struct Vec *LobbyU32Vector_Destroy(struct Vec *a);
struct Vec *LobbyVipVector_Destroy(struct Vec *a);
struct Vec *LobbyEventVector_Destroy(struct Vec *a);
void LobbyCharVector_Allocate(struct Vec *a, u32 b, struct Alloc unused);
u32 LobbyCharVector_RecommendSize(struct Vec *a, u32 n);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
struct Str *LobbyString_Destroy(struct Str *a);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);
void *LobbyEventDeque_Destroy(u32 *a);
void LobbyTimerMap_Insert(u32 *result, struct Tree *a, u32 *value);
void LobbyTimerMap_Erase(Iterator *result, struct Tree *a, Iterator node);
void LobbyCharVector_Grow(struct Vec *a, u32 n);
void LobbyCharVector_Reallocate(struct Vec *a, u32 unused, u32 n);
struct Str *LobbyString_InitCopy(struct Str *a, struct Str *b);
struct Str *LobbyString_InitSubstring(struct Str *a, struct Str *b, u32 offset, u32 length);
struct Str *LobbyString_InitFromCStr(struct Str *a, char *b);
extern char sStr_BasicStringReserveLengthError[], sStr_TreeInsertLengthError[];
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *PPWLobbyWebRequest_Destroy(u8 *a);
void *PPWLobbyPlayers_Delete(u8 *a);
void *PPWLobbyPlayer_Delete(u8 *a);
void *LobbyEventDeque_Destroy(u32 *a);
void *PPWLobbyPlayerSet_Destroy(struct Tree *a);
void LobbyTimerMap_Insert(u32 *result, struct Tree *a, u32 *value);
void LobbyTimerMap_Erase(Iterator *result, struct Tree *a, Iterator node);
void LobbyTimerMap_Clear(struct Tree *a);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void LobbyStringVector_PushBackSlow(struct Vec *a, void *b, struct Alloc unused);
struct Vec *LobbyStringVector_Destroy(struct Vec *a);
struct Vec *LobbyU32Vector_Destroy(struct Vec *a);
struct Vec *LobbyVipVector_Destroy(struct Vec *a);
struct Vec *LobbyEventVector_Destroy(struct Vec *a);
void LobbyCharVector_Grow(struct Vec *a, u32 n);
void LobbyCharVector_Allocate(struct Vec *a, u32 b, struct Alloc unused);
void LobbyCharVector_Reallocate(struct Vec *a, u32 unused, u32 n);
u32 LobbyCharVector_RecommendSize(struct Vec *a, u32 n);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
struct Str *LobbyString_InitCopy(struct Str *a, struct Str *b);
struct Str *LobbyString_InitSubstring(struct Str *a, struct Str *b, u32 offset, u32 length);
struct Str *LobbyString_InitFromCStr(struct Str *a, char *b);
struct Str *LobbyString_Destroy(struct Str *a);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);
void LobbyString_Reserve(struct Str *a, u32 requested);
struct TreeNode *LobbyTimerMap_InsertNode(struct Tree *a, struct TreeNode *parent, int left, int end, u32 *value);
extern int PPWLobby_DecodeMessageData(void *, struct Vec *);
extern void *LobbyByteVector_Data(struct Vec *);
extern int PPWLobbySchedule_Load(void *, void *, u32);
extern void PPWLobbySchedule_CopyToVector(struct Vec *, void *), PPWLobby_SetChannelData(int, int, char *, void *, u32);
extern char sStr_b_lib_c_lobby[];
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *PPWLobbyWebRequest_Destroy(u8 *a);
void *PPWLobbyPlayers_Delete(u8 *a);
void *PPWLobbyPlayer_Delete(u8 *a);
void *LobbyEventDeque_Destroy(u32 *a);
void *PPWLobbyPlayerSet_Destroy(struct Tree *a);
void LobbyTimerMap_Insert(u32 *result, struct Tree *a, u32 *value);
void LobbyTimerMap_Erase(Iterator *result, struct Tree *a, Iterator node);
void LobbyTimerMap_Clear(struct Tree *a);
struct TreeNode *LobbyTimerMap_InsertNode(struct Tree *a, struct TreeNode *parent, int left, int end, u32 *value);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void LobbyStringVector_PushBackSlow(struct Vec *a, void *b, struct Alloc unused);
struct Vec *LobbyStringVector_Destroy(struct Vec *a);
struct Vec *LobbyU32Vector_Destroy(struct Vec *a);
struct Vec *LobbyVipVector_Destroy(struct Vec *a);
struct Vec *LobbyEventVector_Destroy(struct Vec *a);
void LobbyCharVector_Grow(struct Vec *a, u32 n);
void LobbyCharVector_Allocate(struct Vec *a, u32 b, struct Alloc unused);
void LobbyCharVector_Reallocate(struct Vec *a, u32 unused, u32 n);
u32 LobbyCharVector_RecommendSize(struct Vec *a, u32 n);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
struct Str *LobbyString_InitCopy(struct Str *a, struct Str *b);
struct Str *LobbyString_InitSubstring(struct Str *a, struct Str *b, u32 offset, u32 length);
struct Str *LobbyString_InitFromCStr(struct Str *a, char *b);
struct Str *LobbyString_Destroy(struct Str *a);
void LobbyString_Reserve(struct Str *a, u32 requested);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);
void PPWLobby_OnEnumUsers(char *name, int id, int n, int *list);
void PPWLobby_OnGetLobbyData(int ok, int unused1, int unused2, int unused3, void *value);

void PPWLobby_OnPlayerMessage(int a, int b, int c) {
    PPWLobby_OnMessage(0, a, b, c);
}
}
}

#pragma push

namespace OnMessage {
extern "C" {

typedef struct {
    u32 a, b, c;
} String;

typedef struct {
    int a, b, type, kind, arg, spare;
    String text;
} Message;

extern PPWLobbyWork *gPPWLobbyWork;
extern void LobbyString_InitFromCStr(String *, void *), PPWLobbyChannel_GetName(String *, PPWLobbyChannel *), LobbyString_Destroy(String *), LobbyCharVector_Destroy(String *), PPWLobby_HandleSystemMessage(Message *, int, int, void *, u32);
extern int LobbyString_Equals(String *, String *), PPWLobby_ParseMessage(String *, Message *);
extern PPWLobbyPlayer *PPWLobbyPlayers_Find(void *, int);
extern void *LobbyByteVector_Data(String *);
void PPWLobby_OnMessage(void *, int, void *);

static inline void zeroString(String *s) {
    u32 *p = (u32 *)s;
    *p++ = 0;
    *p++ = 0;
    *p = 0;
}

#pragma opt_common_subs off

static inline int validEntry(PPWLobbyPlayer *p) {
    if (p && p->entered) {
        return 1;
    }
    return 0;
}

void PPWLobby_OnMessage(void *key, int index, void *name) {
    Message m;
    String k, tmp, b, a;
    int channel = 5;
    if (key) {
        PPWLobbyWork *c;
        String *kp;
        int match;
        LobbyString_InitFromCStr(&k, key);
        c = gPPWLobbyWork;
        kp = &k;
        PPWLobbyChannel_GetName(&a, &c->mainChannel);
        match = LobbyString_Equals(&a, kp);
        LobbyString_Destroy(&a);
        if (match) {
            channel = c->mainChannel.kind;
        } else {
            PPWLobbyChannel_GetName(&b, &c->subChannel);
            match = LobbyString_Equals(&b, kp);
            LobbyString_Destroy(&b);
            if (match) {
                channel = c->subChannel.kind;
            } else {
                channel = 5;
            }
        }
        LobbyString_Destroy(&k);
    }
    if (key && channel == 5) {
        return;
    }
    {
        int match;
        int zero = 0;
        m.a = zero;
        m.b = 6;
        m.type = 0;
        m.kind = 0;
        m.arg = 0;
        m.spare = 0;
        zeroString(&m.text);
        LobbyString_InitFromCStr(&tmp, name);
        match = PPWLobby_ParseMessage(&tmp, &m) == 0 ? 1 : zero;
        LobbyString_Destroy(&tmp);
        if (match) {
            LobbyCharVector_Destroy(&m.text);
            return;
        }
        switch (m.kind) {
        case 1: {
            PPWLobbyPlayer *p = PPWLobbyPlayers_Find(&gPPWLobbyWork->players, index);
            if (!validEntry(p)) {
                LobbyCharVector_Destroy(&m.text);
                return;
            }
            switch (m.type) {
            case 1: {
                PPWLobbyWork *c = gPPWLobbyWork;
                void *t = LobbyByteVector_Data(&m.text);
                c->callbacks.onMessage(index, channel, m.arg, t, m.text.b);
                break;
            }
            case 0: {
                PPWLobbyWork *c = gPPWLobbyWork;
                void *t = LobbyByteVector_Data(&m.text);
                c->callbacks.onEvent3(index, channel, m.arg, t);
                break;
            }
            }
            break;
        }
        case 0: {
            void *t = LobbyByteVector_Data(&m.text);
            PPWLobby_HandleSystemMessage(&m, index, channel, t, m.text.b);
            break;
        }
        }
        LobbyCharVector_Destroy(&m.text);
    }
}
}
}

#pragma pop

namespace OnChannelEvents {
extern "C" {
void ov45_02236E58(Iterator *);
extern int printf(const char *, ...);
extern void abort(void);

struct Str {
    union {
        u32 flags;

        struct {
            u32 large : 1;
            u32 cap : 31;
        } bits;
    } u;

    u32 size;
    char *ptr;
};

struct Vec {
    void *ptr;
    u32 size, cap;
};

struct TreeNode {
    void *left, *right;
    u32 parent;
    u32 key;
    u32 data[6];
};

struct Tree {
    u32 count;
    void *root;
    u32 unk;
    void *end;
};

struct Alloc {
    u8 tag;
};

extern PPWLobbyWork *gPPWLobbyWork;
extern u8 gPPWLobbyPlayersVtable[], sStr_b_lib_u_user[], sStr_b_lib_u_system[], gPPWLobbyMainChannelMode[];
extern void DWC_Free(int, void *, int);
extern void *LobbyMem_Alloc(u32);
extern struct Str *LobbyString_Destroy(struct Str *);
extern struct Vec *LobbyCharVector_Destroy(struct Vec *);
extern struct Str *LobbyString_InitSubstring(struct Str *, struct Str *, u32, u32);
extern void PPWLobby_OnMessage(int, int, int, int);
extern void PPWLobby_HandleReceivedData(int, int, int, int, int, int, int);
extern void PPWLobbyPlayers_Clear(void *), LobbyEventDequeMap_Destroy(void *), PPWLobbyPlayerSet_DestroyNodes(void *, void *);
extern void LobbyStringVector_RecommendSize(struct Vec *, u32, int), LobbyStringVector_AppendCopiesSlow(struct Vec *, u32, void *), LobbyStringVector_EraseAtEnd(struct Vec *, u32, struct Alloc), LobbyCharVector_AppendZerosSlow(struct Vec *, u32);
extern void LobbyCharVector_Allocate(struct Vec *, u32, struct Alloc);
extern void LobbyTree_InsertRebalance(struct TreeNode *, void *), LobbyTimerMap_DestroyNodes(struct Tree *, struct TreeNode *);
extern struct TreeNode *LobbyTree_EraseRebalance(struct TreeNode *, void *);
extern void LobbyTimerMap_EraseKey(void *, int *), LobbyChat_SetChannelMode(int, char *);
extern void PPWLobbyPlayers_Add(void *, int), LobbyChat_GetChannelKeysAsync(int, int, char *, void (*)(), int);
extern void PPWLobbyChannel_GetName(struct Str *, void *);
extern struct Str *LobbyString_InitFromCStr(struct Str *, char *);
extern int LobbyString_Equals(struct Str *, struct Str *);
extern void LobbyString_Reserve(struct Str *, u32);
extern u32 LobbyCharVector_RecommendSize(struct Vec *, u32);
extern struct Str *LobbyString_Replace(struct Str *, u32, u32, struct Str *);
extern struct Str *LobbyString_ReplaceRange(struct Str *, u32, u32, char *, char *, struct Alloc);
extern void _ZNSt15__vector_commonILb1EE18throw_length_errorEv(void);
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *PPWLobbyWebRequest_Destroy(u8 *a);
void *PPWLobbyPlayers_Delete(u8 *a);
void *PPWLobbyPlayer_Delete(u8 *a);
void *PPWLobbyPlayerSet_Destroy(struct Tree *a);
void LobbyTimerMap_Clear(struct Tree *a);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void LobbyStringVector_PushBackSlow(struct Vec *a, void *b, struct Alloc unused);
struct Vec *LobbyStringVector_Destroy(struct Vec *a);
struct Vec *LobbyU32Vector_Destroy(struct Vec *a);
struct Vec *LobbyVipVector_Destroy(struct Vec *a);
struct Vec *LobbyEventVector_Destroy(struct Vec *a);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
void LobbyCharVector_Allocate(struct Vec *a, u32 b, struct Alloc unused);
u32 LobbyCharVector_RecommendSize(struct Vec *a, u32 n);
struct Str *LobbyString_Destroy(struct Str *a);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);

extern void LobbyEventDequeMap_Destroy(void *), PPWLobbyPlayerSet_DestroyNodes(void *, void *);
extern struct TreeNode *LobbyTimerMap_FindInsertPos(struct Tree *, u32 *, struct TreeNode **, u8 *, u8 *);
extern struct TreeNode *LobbyTimerMap_InsertNode(struct Tree *, struct TreeNode *, int, int, u32 *);
extern void LobbyString_ReplaceSubstring(struct Str *, u32, u32, struct Str *, u32, u32);
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *PPWLobbyWebRequest_Destroy(u8 *a);
void *PPWLobbyPlayers_Delete(u8 *a);
void *PPWLobbyPlayer_Delete(u8 *a);
void *PPWLobbyPlayerSet_Destroy(struct Tree *a);
void LobbyTimerMap_Clear(struct Tree *a);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void LobbyStringVector_PushBackSlow(struct Vec *a, void *b, struct Alloc unused);
struct Vec *LobbyStringVector_Destroy(struct Vec *a);
struct Vec *LobbyU32Vector_Destroy(struct Vec *a);
struct Vec *LobbyVipVector_Destroy(struct Vec *a);
struct Vec *LobbyEventVector_Destroy(struct Vec *a);
void LobbyCharVector_Allocate(struct Vec *a, u32 b, struct Alloc unused);
u32 LobbyCharVector_RecommendSize(struct Vec *a, u32 n);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
struct Str *LobbyString_Destroy(struct Str *a);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);
void *LobbyEventDeque_Destroy(u32 *a);
void LobbyTimerMap_Insert(u32 *result, struct Tree *a, u32 *value);
void LobbyTimerMap_Erase(Iterator *result, struct Tree *a, Iterator node);
void LobbyCharVector_Grow(struct Vec *a, u32 n);
void LobbyCharVector_Reallocate(struct Vec *a, u32 unused, u32 n);
struct Str *LobbyString_InitCopy(struct Str *a, struct Str *b);
struct Str *LobbyString_InitSubstring(struct Str *a, struct Str *b, u32 offset, u32 length);
struct Str *LobbyString_InitFromCStr(struct Str *a, char *b);
extern char sStr_BasicStringReserveLengthError[], sStr_TreeInsertLengthError[];
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *PPWLobbyWebRequest_Destroy(u8 *a);
void *PPWLobbyPlayers_Delete(u8 *a);
void *PPWLobbyPlayer_Delete(u8 *a);
void *LobbyEventDeque_Destroy(u32 *a);
void *PPWLobbyPlayerSet_Destroy(struct Tree *a);
void LobbyTimerMap_Insert(u32 *result, struct Tree *a, u32 *value);
void LobbyTimerMap_Erase(Iterator *result, struct Tree *a, Iterator node);
void LobbyTimerMap_Clear(struct Tree *a);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void LobbyStringVector_PushBackSlow(struct Vec *a, void *b, struct Alloc unused);
struct Vec *LobbyStringVector_Destroy(struct Vec *a);
struct Vec *LobbyU32Vector_Destroy(struct Vec *a);
struct Vec *LobbyVipVector_Destroy(struct Vec *a);
struct Vec *LobbyEventVector_Destroy(struct Vec *a);
void LobbyCharVector_Grow(struct Vec *a, u32 n);
void LobbyCharVector_Allocate(struct Vec *a, u32 b, struct Alloc unused);
void LobbyCharVector_Reallocate(struct Vec *a, u32 unused, u32 n);
u32 LobbyCharVector_RecommendSize(struct Vec *a, u32 n);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
struct Str *LobbyString_InitCopy(struct Str *a, struct Str *b);
struct Str *LobbyString_InitSubstring(struct Str *a, struct Str *b, u32 offset, u32 length);
struct Str *LobbyString_InitFromCStr(struct Str *a, char *b);
struct Str *LobbyString_Destroy(struct Str *a);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);
void LobbyString_Reserve(struct Str *a, u32 requested);
struct TreeNode *LobbyTimerMap_InsertNode(struct Tree *a, struct TreeNode *parent, int left, int end, u32 *value);
extern int PPWLobby_DecodeMessageData(void *, struct Vec *);
extern void *LobbyByteVector_Data(struct Vec *);
extern int PPWLobbySchedule_Load(void *, void *, u32);
extern void PPWLobbySchedule_CopyToVector(struct Vec *, void *), PPWLobby_SetChannelData(int, int, char *, void *, u32);
extern char sStr_b_lib_c_lobby[];
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *PPWLobbyWebRequest_Destroy(u8 *a);
void *PPWLobbyPlayers_Delete(u8 *a);
void *PPWLobbyPlayer_Delete(u8 *a);
void *LobbyEventDeque_Destroy(u32 *a);
void *PPWLobbyPlayerSet_Destroy(struct Tree *a);
void LobbyTimerMap_Insert(u32 *result, struct Tree *a, u32 *value);
void LobbyTimerMap_Erase(Iterator *result, struct Tree *a, Iterator node);
void LobbyTimerMap_Clear(struct Tree *a);
struct TreeNode *LobbyTimerMap_InsertNode(struct Tree *a, struct TreeNode *parent, int left, int end, u32 *value);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void LobbyStringVector_PushBackSlow(struct Vec *a, void *b, struct Alloc unused);
struct Vec *LobbyStringVector_Destroy(struct Vec *a);
struct Vec *LobbyU32Vector_Destroy(struct Vec *a);
struct Vec *LobbyVipVector_Destroy(struct Vec *a);
struct Vec *LobbyEventVector_Destroy(struct Vec *a);
void LobbyCharVector_Grow(struct Vec *a, u32 n);
void LobbyCharVector_Allocate(struct Vec *a, u32 b, struct Alloc unused);
void LobbyCharVector_Reallocate(struct Vec *a, u32 unused, u32 n);
u32 LobbyCharVector_RecommendSize(struct Vec *a, u32 n);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
struct Str *LobbyString_InitCopy(struct Str *a, struct Str *b);
struct Str *LobbyString_InitSubstring(struct Str *a, struct Str *b, u32 offset, u32 length);
struct Str *LobbyString_InitFromCStr(struct Str *a, char *b);
struct Str *LobbyString_Destroy(struct Str *a);
void LobbyString_Reserve(struct Str *a, u32 requested);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);
void PPWLobby_OnEnumUsers(char *name, int id, int n, int *list);
void PPWLobby_OnGetLobbyData(int ok, int unused1, int unused2, int unused3, void *value);

void PPWLobby_OnExcessFlood(int a) {
    gPPWLobbyWork->callbacks.onExcessFlood(a);
}

void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d) {
    PPWLobby_HandleReceivedData(1, 1, a, b, c, d, 0);
}

void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f) {
    PPWLobby_HandleReceivedData(a, 0, b, c, d, e, f);
}
}
}

namespace OnEnumChannels {
extern "C" {

typedef union {
    struct {
        u32 allocated : 1;
        u32 capacity : 31;
        u32 size;
        char *data;
    } heap;

    struct {
        u8 header;
        char data[11];
    } inplace;
} String;

typedef struct {
    String *data;
    u32 size, cap;
} Vec;

extern PPWLobbyWork *gPPWLobbyWork;
extern char sStr_Bang[];
extern void LobbyString_InitFromCStr(String *, char *), LobbyString_Destroy(String *), LobbyStringVector_Destroy(Vec *), LobbyString_Assign(void *, String *), PPWLobby_GenerateChannelBaseName(String *);
extern void LobbyString_Split(Vec *, String *, String *, int);
void PPWLobby_OnEnumChannels(int, int, char **, int, int *);

void PPWLobby_OnEnumChannels(int valid, int count, char **names, int unused, int *types) {
    Vec parts;
    String item;
    String delimiter;
    String text;
    String empty;
    int i;
    if (!valid) {
        return;
    }
    for (i = 0; i < count; i++) {
        if (types[i] >= 20) {
            continue;
        }
        LobbyString_InitFromCStr(&item, names[i]);
        LobbyString_InitFromCStr(&delimiter, sStr_Bang);
        LobbyString_Split(&parts, &item, &delimiter, 0);
        LobbyString_Destroy(&delimiter);
        LobbyString_Destroy(&item);
        if (parts.size < 2) {
            LobbyStringVector_Destroy(&parts);
            continue;
        }
        {
            String *s = parts.data + 1;
            char *p = s->heap.allocated == 0 ? s->inplace.data : s->heap.data;
            LobbyString_InitFromCStr(&text, p);
        }
        LobbyString_Assign(&gPPWLobbyWork->channelBaseName, &text);
        LobbyString_Destroy(&text);
        gPPWLobbyWork->isNewChannel = 0;
        gPPWLobbyWork->state = PPW_LOBBY_STATE_ENTER_MAIN_CHANNEL;
        LobbyStringVector_Destroy(&parts);
        return;
    }
    PPWLobby_GenerateChannelBaseName(&empty);
    LobbyString_Assign(&gPPWLobbyWork->channelBaseName, &empty);
    LobbyString_Destroy(&empty);
    gPPWLobbyWork->isNewChannel = 1;
    gPPWLobbyWork->state = PPW_LOBBY_STATE_ENTER_MAIN_CHANNEL;
}
}
}

namespace StringSplit {
struct Str;
extern "C" Str *LobbyString_InitCopy(Str *, const Str *);
extern "C" void LobbyString_Destroy(Str *);
extern "C" void LobbyString_Assign(Str *, const Str *);
extern "C" int LobbyString_Find(const Str *, const Str *, int);
extern "C" void LobbyString_Substr(Str *, const Str *, int, int);

struct Str {
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

        u32 words[3];
    } raw;

    Str() {
    }

    Str(const Str &o) {
        LobbyString_InitCopy(this, &o);
    }

    u32 size() const {
        return !raw.l.flag ? raw.s.size : raw.l.size;
    }

    void Destroy() {
        LobbyString_Destroy(this);
    }
};

class Alloc {};
struct Vec;
extern "C" void LobbyStringVector_PushBackSlow(Vec *, const Str &, Alloc);

struct Vec {
    Str *data;
    u32 count, cap;

    Alloc get_alloc() const {
        return Alloc();
    }

    Vec()
        : data(0)
        , count(0)
        , cap(0) {
    }

    void push_back(const Str &x) {
        Alloc a = get_alloc();
        (void)a;
        if (count < cap) {
            new (data + count) Str(x);
            count++;
        } else {
            LobbyStringVector_PushBackSlow(this, x, Alloc());
        }
    }
};

extern "C" Vec LobbyString_Split(const Str &s, const Str &delim, int max);

extern "C" Vec LobbyString_Split(const Str &s, const Str &delim, int max) {
    Vec out;
    int n = 1;
    Str rest(s);
    int pos;
    while (n != max && (pos = LobbyString_Find(&rest, &delim, 0)) != -1) {
        if (pos != 0) {
            Str piece;
            LobbyString_Substr(&piece, &rest, 0, pos);
            out.push_back(piece);
            piece.Destroy();
            n++;
        }
        Str tmp;
        LobbyString_Substr(&tmp, &rest, pos + delim.size(), -1);
        LobbyString_Assign(&rest, &tmp);
        tmp.Destroy();
    }
    if (rest.size() != 0) {
        out.push_back(rest);
    }
    rest.Destroy();
    return out;
}

}

namespace StringSubstr {
extern "C" {
void LobbyString_InitSubstring(void *, void *, int, int);
void LobbyString_Substr(void *, void *, int, int);

void LobbyString_Substr(void *a, void *b, int c, int d) {
    void *volatile copy = b;
    LobbyString_InitSubstring(a, b, c, d);
}
}
}

namespace OnEnumUsers {
struct Str;
extern "C" Str *LobbyString_InitFromCStr(Str *, char *);
extern "C" Str *LobbyString_Destroy(Str *);
extern "C" Str *LobbyString_InitCopy(Str *, const Str *);

struct Str {
    u32 words[3];

    Str(char *s) {
        LobbyString_InitFromCStr(this, s);
    }

    Str(const Str &o) {
        LobbyString_InitCopy(this, &o);
    }

    void Destroy() {
        LobbyString_Destroy(this);
    }
};

extern "C" PPWLobbyWork *gPPWLobbyWork;
extern "C" char sStr_b_lib_u_user[], sStr_b_lib_u_system[];
extern "C" Str PPWLobbyChannel_GetName(PPWLobbyChannel *);
extern "C" int LobbyString_Equals(const Str &, const Str &);
extern "C" void PPWLobbyPlayers_Add(void *, int);
extern "C" void LobbyChat_GetChannelKeysAsync(char *, int, char *, void (*)(), int);
extern "C" void PPWLobby_OnGetChannelKeys();
extern "C" void PPWLobby_OnEnumUsers(int, char *, int, int *);

inline int Find(PPWLobbyWork *w, const Str &name) {
    const Str &n = name;
    int eq;
    {
        Str t = PPWLobbyChannel_GetName(&w->mainChannel);
        eq = LobbyString_Equals(t, n);
        t.Destroy();
    }
    if (eq) {
        return w->mainChannel.kind;
    }
    {
        Str t = PPWLobbyChannel_GetName(&w->subChannel);
        eq = LobbyString_Equals(t, n);
        t.Destroy();
    }
    if (eq) {
        return w->subChannel.kind;
    }
    return 5;
}

inline bool IsSelf(int id) {
    return id == gPPWLobbyWork->userId;
}

#pragma opt_common_subs off

extern "C" void PPWLobby_OnEnumUsers(int success, char *channel, int count, int *ids) {
    int result, i;
    if (success == 0) {
        return;
    }
    {
        Str name(channel);
        PPWLobbyWork *w = gPPWLobbyWork;
        result = Find(w, name);
        name.Destroy();
    }
    if (result == 5) {
        return;
    }
    if (result != 0) {
        return;
    }
    for (i = 0; i < count; i++) {
        if (!IsSelf(ids[i])) {
            PPWLobbyPlayers_Add(&gPPWLobbyWork->players, ids[i]);
            LobbyChat_GetChannelKeysAsync(channel, ids[i], sStr_b_lib_u_user, PPWLobby_OnGetChannelKeys, 0);
            LobbyChat_GetChannelKeysAsync(channel, ids[i], sStr_b_lib_u_system, PPWLobby_OnGetChannelKeys, 0);
        }
    }
    gPPWLobbyWork->mainChannel.usersEnumerated = 1;
}

#pragma opt_common_subs reset

}

namespace OnGetChannelMode {
extern "C" {
void ov45_02236E58(Iterator *);
extern int printf(const char *, ...);
extern void abort(void);

struct Str {
    union {
        u32 flags;

        struct {
            u32 large : 1;
            u32 cap : 31;
        } bits;
    } u;

    u32 size;
    char *ptr;
};

struct Vec {
    void *ptr;
    u32 size, cap;
};

struct TreeNode {
    void *left, *right;
    u32 parent;
    u32 key;
    u32 data[6];
};

struct Tree {
    u32 count;
    void *root;
    u32 unk;
    void *end;
};

struct Alloc {
    u8 tag;
};

extern PPWLobbyWork *gPPWLobbyWork;
extern u8 gPPWLobbyPlayersVtable[], sStr_b_lib_u_user[], sStr_b_lib_u_system[], gPPWLobbyMainChannelMode[];
extern void DWC_Free(int, void *, int);
extern void *LobbyMem_Alloc(u32);
extern struct Str *LobbyString_Destroy(struct Str *);
extern struct Vec *LobbyCharVector_Destroy(struct Vec *);
extern struct Str *LobbyString_InitSubstring(struct Str *, struct Str *, u32, u32);
extern void PPWLobby_OnMessage(int, int, int, int);
extern void PPWLobby_HandleReceivedData(int, int, int, int, int, int, int);
extern void PPWLobbyPlayers_Clear(void *), LobbyEventDequeMap_Destroy(void *), PPWLobbyPlayerSet_DestroyNodes(void *, void *);
extern void LobbyStringVector_RecommendSize(struct Vec *, u32, int), LobbyStringVector_AppendCopiesSlow(struct Vec *, u32, void *), LobbyStringVector_EraseAtEnd(struct Vec *, u32, struct Alloc), LobbyCharVector_AppendZerosSlow(struct Vec *, u32);
extern void LobbyCharVector_Allocate(struct Vec *, u32, struct Alloc);
extern void LobbyTree_InsertRebalance(struct TreeNode *, void *), LobbyTimerMap_DestroyNodes(struct Tree *, struct TreeNode *);
extern struct TreeNode *LobbyTree_EraseRebalance(struct TreeNode *, void *);
extern void LobbyTimerMap_EraseKey(void *, int *), LobbyChat_SetChannelMode(int, char *);
extern void PPWLobbyPlayers_Add(void *, int), LobbyChat_GetChannelKeysAsync(int, int, char *, void (*)(), int);
extern void PPWLobbyChannel_GetName(struct Str *, void *);
extern struct Str *LobbyString_InitFromCStr(struct Str *, char *);
extern int LobbyString_Equals(struct Str *, struct Str *);
extern void LobbyString_Reserve(struct Str *, u32);
extern u32 LobbyCharVector_RecommendSize(struct Vec *, u32);
extern struct Str *LobbyString_Replace(struct Str *, u32, u32, struct Str *);
extern struct Str *LobbyString_ReplaceRange(struct Str *, u32, u32, char *, char *, struct Alloc);
extern void _ZNSt15__vector_commonILb1EE18throw_length_errorEv(void);
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *PPWLobbyWebRequest_Destroy(u8 *a);
void *PPWLobbyPlayers_Delete(u8 *a);
void *PPWLobbyPlayer_Delete(u8 *a);
void *PPWLobbyPlayerSet_Destroy(struct Tree *a);
void LobbyTimerMap_Clear(struct Tree *a);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void LobbyStringVector_PushBackSlow(struct Vec *a, void *b, struct Alloc unused);
struct Vec *LobbyStringVector_Destroy(struct Vec *a);
struct Vec *LobbyU32Vector_Destroy(struct Vec *a);
struct Vec *LobbyVipVector_Destroy(struct Vec *a);
struct Vec *LobbyEventVector_Destroy(struct Vec *a);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
void LobbyCharVector_Allocate(struct Vec *a, u32 b, struct Alloc unused);
u32 LobbyCharVector_RecommendSize(struct Vec *a, u32 n);
struct Str *LobbyString_Destroy(struct Str *a);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);

extern void LobbyEventDequeMap_Destroy(void *), PPWLobbyPlayerSet_DestroyNodes(void *, void *);
extern struct TreeNode *LobbyTimerMap_FindInsertPos(struct Tree *, u32 *, struct TreeNode **, u8 *, u8 *);
extern struct TreeNode *LobbyTimerMap_InsertNode(struct Tree *, struct TreeNode *, int, int, u32 *);
extern void LobbyString_ReplaceSubstring(struct Str *, u32, u32, struct Str *, u32, u32);
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *PPWLobbyWebRequest_Destroy(u8 *a);
void *PPWLobbyPlayers_Delete(u8 *a);
void *PPWLobbyPlayer_Delete(u8 *a);
void *PPWLobbyPlayerSet_Destroy(struct Tree *a);
void LobbyTimerMap_Clear(struct Tree *a);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void LobbyStringVector_PushBackSlow(struct Vec *a, void *b, struct Alloc unused);
struct Vec *LobbyStringVector_Destroy(struct Vec *a);
struct Vec *LobbyU32Vector_Destroy(struct Vec *a);
struct Vec *LobbyVipVector_Destroy(struct Vec *a);
struct Vec *LobbyEventVector_Destroy(struct Vec *a);
void LobbyCharVector_Allocate(struct Vec *a, u32 b, struct Alloc unused);
u32 LobbyCharVector_RecommendSize(struct Vec *a, u32 n);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
struct Str *LobbyString_Destroy(struct Str *a);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);
void *LobbyEventDeque_Destroy(u32 *a);
void LobbyTimerMap_Insert(u32 *result, struct Tree *a, u32 *value);
void LobbyTimerMap_Erase(Iterator *result, struct Tree *a, Iterator node);
void LobbyCharVector_Grow(struct Vec *a, u32 n);
void LobbyCharVector_Reallocate(struct Vec *a, u32 unused, u32 n);
struct Str *LobbyString_InitCopy(struct Str *a, struct Str *b);
struct Str *LobbyString_InitSubstring(struct Str *a, struct Str *b, u32 offset, u32 length);
struct Str *LobbyString_InitFromCStr(struct Str *a, char *b);
extern char sStr_BasicStringReserveLengthError[], sStr_TreeInsertLengthError[];
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *PPWLobbyWebRequest_Destroy(u8 *a);
void *PPWLobbyPlayers_Delete(u8 *a);
void *PPWLobbyPlayer_Delete(u8 *a);
void *LobbyEventDeque_Destroy(u32 *a);
void *PPWLobbyPlayerSet_Destroy(struct Tree *a);
void LobbyTimerMap_Insert(u32 *result, struct Tree *a, u32 *value);
void LobbyTimerMap_Erase(Iterator *result, struct Tree *a, Iterator node);
void LobbyTimerMap_Clear(struct Tree *a);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void LobbyStringVector_PushBackSlow(struct Vec *a, void *b, struct Alloc unused);
struct Vec *LobbyStringVector_Destroy(struct Vec *a);
struct Vec *LobbyU32Vector_Destroy(struct Vec *a);
struct Vec *LobbyVipVector_Destroy(struct Vec *a);
struct Vec *LobbyEventVector_Destroy(struct Vec *a);
void LobbyCharVector_Grow(struct Vec *a, u32 n);
void LobbyCharVector_Allocate(struct Vec *a, u32 b, struct Alloc unused);
void LobbyCharVector_Reallocate(struct Vec *a, u32 unused, u32 n);
u32 LobbyCharVector_RecommendSize(struct Vec *a, u32 n);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
struct Str *LobbyString_InitCopy(struct Str *a, struct Str *b);
struct Str *LobbyString_InitSubstring(struct Str *a, struct Str *b, u32 offset, u32 length);
struct Str *LobbyString_InitFromCStr(struct Str *a, char *b);
struct Str *LobbyString_Destroy(struct Str *a);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);
void LobbyString_Reserve(struct Str *a, u32 requested);
struct TreeNode *LobbyTimerMap_InsertNode(struct Tree *a, struct TreeNode *parent, int left, int end, u32 *value);
extern int PPWLobby_DecodeMessageData(void *, struct Vec *);
extern void *LobbyByteVector_Data(struct Vec *);
extern int PPWLobbySchedule_Load(void *, void *, u32);
extern void PPWLobbySchedule_CopyToVector(struct Vec *, void *), PPWLobby_SetChannelData(int, int, char *, void *, u32);
extern char sStr_b_lib_c_lobby[];
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *PPWLobbyWebRequest_Destroy(u8 *a);
void *PPWLobbyPlayers_Delete(u8 *a);
void *PPWLobbyPlayer_Delete(u8 *a);
void *LobbyEventDeque_Destroy(u32 *a);
void *PPWLobbyPlayerSet_Destroy(struct Tree *a);
void LobbyTimerMap_Insert(u32 *result, struct Tree *a, u32 *value);
void LobbyTimerMap_Erase(Iterator *result, struct Tree *a, Iterator node);
void LobbyTimerMap_Clear(struct Tree *a);
struct TreeNode *LobbyTimerMap_InsertNode(struct Tree *a, struct TreeNode *parent, int left, int end, u32 *value);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void LobbyStringVector_PushBackSlow(struct Vec *a, void *b, struct Alloc unused);
struct Vec *LobbyStringVector_Destroy(struct Vec *a);
struct Vec *LobbyU32Vector_Destroy(struct Vec *a);
struct Vec *LobbyVipVector_Destroy(struct Vec *a);
struct Vec *LobbyEventVector_Destroy(struct Vec *a);
void LobbyCharVector_Grow(struct Vec *a, u32 n);
void LobbyCharVector_Allocate(struct Vec *a, u32 b, struct Alloc unused);
void LobbyCharVector_Reallocate(struct Vec *a, u32 unused, u32 n);
u32 LobbyCharVector_RecommendSize(struct Vec *a, u32 n);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
struct Str *LobbyString_InitCopy(struct Str *a, struct Str *b);
struct Str *LobbyString_InitSubstring(struct Str *a, struct Str *b, u32 offset, u32 length);
struct Str *LobbyString_InitFromCStr(struct Str *a, char *b);
struct Str *LobbyString_Destroy(struct Str *a);
void LobbyString_Reserve(struct Str *a, u32 requested);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);
void PPWLobby_OnEnumUsers(char *name, int id, int n, int *list);
void PPWLobby_OnGetLobbyData(int ok, int unused1, int unused2, int unused3, void *value);

void PPWLobby_OnGetChannelMode(int a, int b, u32 *c) {
    int v;
    if (!a) {
        return;
    }
    if (c[7] == 20) {
        gPPWLobbyWork->mainChannel.modeReady = 1;
        v = 4;
        LobbyTimerMap_EraseKey(gPPWLobbyWork->timers, &v);
        return;
    }
    LobbyChat_SetChannelMode(b, (char *)gPPWLobbyMainChannelMode);
}
}
}

namespace OnGetChannelTime {
struct Vec;
extern "C" Vec *LobbyCharVector_Destroy(Vec *);

struct Vec {
    char *ptr;
    u32 size, cap;

    Vec()
        : ptr(0)
        , size(0)
        , cap(0) {
    }

    void Destroy() {
        LobbyCharVector_Destroy(this);
    }
};

struct Date {
    u32 year, month, day, week;
};

struct Time {
    u32 hour, minute, second;
};

struct Stamp {
    s64 v;
};

struct DateTime {
    Date date;
    Time time;
};

extern "C" PPWLobbyWork *gPPWLobbyWork;
extern "C" char sStr_b_lib_c_time[];
extern "C" int PPWLobby_DecodeMessageData(const char *, Vec *);
extern "C" void DWC_GetDateTime(Date *, Time *);
extern "C" s64 RTC_ConvertDateTimeToSecond(const Date *, const Time *);
extern "C" void PPWLobby_SetChannelData(int, int, char *, Stamp *, int);
extern "C" char *LobbyByteVector_Data(Vec *);
extern "C" void LobbyTimerMap_EraseKey(void *, int *);
extern "C" void PPWLobby_OnGetChannelTime(int, int, int, int, const char *);

extern "C" void PPWLobby_OnGetChannelTime(int result, int unused1, int unused2, int unused3, const char *value) {
    if (result == 0) {
        return;
    }
    Vec data;
    if (PPWLobby_DecodeMessageData(value, &data) == 0 || data.size != 8) {
        PPWLobbyWork *w = gPPWLobbyWork;
        DateTime now;
        DWC_GetDateTime(&now.date, &now.time);
        w->schedule.time.seconds = RTC_ConvertDateTimeToSecond(&now.date, &now.time);
        Stamp s;
        s.v = w->schedule.time.seconds;
        PPWLobby_SetChannelData(-1, 0, sStr_b_lib_c_time, &s, 8);
        data.Destroy();
        return;
    }
    {
        Stamp s = *(Stamp *)LobbyByteVector_Data(&data);
        PPWLobbyWork *w = gPPWLobbyWork;
        w->schedule.hasTime = 1;
        w->schedule.time.seconds = s.v;
        int state = 5;
        LobbyTimerMap_EraseKey(gPPWLobbyWork->timers, &state);
    }
    data.Destroy();
}

}

namespace OnGetLobbyData {
extern "C" {

typedef struct {
    void *ptr;
    u32 size, capacity;
} Blob;

typedef struct {
    u32 lo, hi;
} Pair;

extern PPWLobbyWork *gPPWLobbyWork;
extern u8 sStr_b_lib_c_time[], sStr_b_lib_c_lobby[];
extern int PPWLobby_DecodeMessageData(void *, Blob *), PPWLobbySchedule_Load(PPWLobbySchedule *, void *, u32);
extern void *LobbyByteVector_Data(Blob *);
extern void LobbyCharVector_Destroy(Blob *), PPWLobby_SetChannelData(int, int, void *, void *, int), LobbyTimerMap_EraseKey(void *, int *), PPWLobbySchedule_CopyToVector(Blob *, PPWLobbySchedule *), DWC_GetDateTime(RTCDate *, RTCTime *);

static inline int appendBlob(Blob *blob) {
    PPWLobbyWork *w = gPPWLobbyWork;
    return PPWLobbySchedule_Load(&w->schedule, LobbyByteVector_Data(blob), blob->size);
}

void PPWLobby_OnGetChannelTime(int, int, int, int, void *), PPWLobby_OnGetLobbyData(int, int, int, int, void *);

void PPWLobby_OnGetLobbyData(int ok, int a, int b, int c, void *data) {
    Blob blob, copy;
    int state;
    if (ok == 0) {
        return;
    }
    blob.ptr = 0;
    blob.size = 0;
    blob.capacity = 0;
    if (!PPWLobby_DecodeMessageData(data, &blob) || blob.size == 0 || !appendBlob(&blob)) {
        PPWLobbySchedule_CopyToVector(&copy, &gPPWLobbyWork->schedule);
        PPWLobby_SetChannelData(-1, 0, sStr_b_lib_c_lobby, LobbyByteVector_Data(&copy), copy.size);
        LobbyCharVector_Destroy(&copy);
        LobbyCharVector_Destroy(&blob);
        return;
    }
    gPPWLobbyWork->schedule.state = 4;
    state = 6;
    LobbyTimerMap_EraseKey(gPPWLobbyWork->timers, &state);
    LobbyCharVector_Destroy(&blob);
}
}
}

namespace Destructors {
extern "C" {
void ov45_02236E58(Iterator *);
extern int printf(const char *, ...);
extern void abort(void);

struct Str {
    union {
        u32 flags;

        struct {
            u32 large : 1;
            u32 cap : 31;
        } bits;
    } u;

    u32 size;
    char *ptr;
};

struct Vec {
    void *ptr;
    u32 size, cap;
};

struct TreeNode {
    void *left, *right;
    u32 parent;
    u32 key;
    u32 data[6];
};

struct Tree {
    u32 count;
    void *root;
    u32 unk;
    void *end;
};

struct Alloc {
    u8 tag;
};

struct PlayerSet {
    u32 count;
    void *root;
    void *leftmost;
};

struct Player {
    void *vtable;
    PPWLobbySystemProfile systemProfile;
    int systemProfileChanged;
    struct Vec userProfile;
};

struct Players {
    u8 *vtable;
    struct PlayerSet playerSet;
    struct Player self;
};

struct WebRequest {
    void *vtable;
    int kind;
    int method;
    struct Str key;
    struct Str url;
    u32 userId;
    struct Vec data;
};

extern u8 gPPWLobbyPlayersVtable[], sStr_b_lib_u_user[], sStr_b_lib_u_system[], gPPWLobbyMainChannelMode[];
extern void DWC_Free(int, void *, int);
extern void *LobbyMem_Alloc(u32);
extern struct Str *LobbyString_Destroy(struct Str *);
extern struct Vec *LobbyCharVector_Destroy(struct Vec *);
extern struct Str *LobbyString_InitSubstring(struct Str *, struct Str *, u32, u32);
extern void PPWLobby_OnMessage(int, int, int, int);
extern void PPWLobby_HandleReceivedData(int, int, int, int, int, int, int);
extern void PPWLobbyPlayers_Clear(void *), LobbyEventDequeMap_Destroy(void *), PPWLobbyPlayerSet_DestroyNodes(void *, void *);
extern void LobbyStringVector_RecommendSize(struct Vec *, u32, int), LobbyStringVector_AppendCopiesSlow(struct Vec *, u32, void *), LobbyStringVector_EraseAtEnd(struct Vec *, u32, struct Alloc), LobbyCharVector_AppendZerosSlow(struct Vec *, u32);
extern void LobbyCharVector_Allocate(struct Vec *, u32, struct Alloc);
extern void LobbyTree_InsertRebalance(struct TreeNode *, void *), LobbyTimerMap_DestroyNodes(struct Tree *, struct TreeNode *);
extern struct TreeNode *LobbyTree_EraseRebalance(struct TreeNode *, void *);
extern void LobbyTimerMap_EraseKey(void *, int *), LobbyChat_SetChannelMode(int, char *);
extern void PPWLobbyPlayers_Add(void *, int), LobbyChat_GetChannelKeysAsync(int, int, char *, void (*)(), int);
extern void PPWLobbyChannel_GetName(struct Str *, void *);
extern struct Str *LobbyString_InitFromCStr(struct Str *, char *);
extern int LobbyString_Equals(struct Str *, struct Str *);
extern void LobbyString_Reserve(struct Str *, u32);
extern u32 LobbyCharVector_RecommendSize(struct Vec *, u32);
extern struct Str *LobbyString_Replace(struct Str *, u32, u32, struct Str *);
extern struct Str *LobbyString_ReplaceRange(struct Str *, u32, u32, char *, char *, struct Alloc);
extern void _ZNSt15__vector_commonILb1EE18throw_length_errorEv(void);
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *PPWLobbyWebRequest_Destroy(struct WebRequest *request);
void *PPWLobbyPlayers_Delete(struct Players *players);
void *PPWLobbyPlayer_Delete(struct Player *player);
void *PPWLobbyPlayerSet_Destroy(struct PlayerSet *a);
void LobbyTimerMap_Clear(struct Tree *a);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void LobbyStringVector_PushBackSlow(struct Vec *a, void *b, struct Alloc unused);
struct Vec *LobbyStringVector_Destroy(struct Vec *a);
struct Vec *LobbyU32Vector_Destroy(struct Vec *a);
struct Vec *LobbyVipVector_Destroy(struct Vec *a);
struct Vec *LobbyEventVector_Destroy(struct Vec *a);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
void LobbyCharVector_Allocate(struct Vec *a, u32 b, struct Alloc unused);
u32 LobbyCharVector_RecommendSize(struct Vec *a, u32 n);
struct Str *LobbyString_Destroy(struct Str *a);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);

extern void LobbyEventDequeMap_Destroy(void *), PPWLobbyPlayerSet_DestroyNodes(void *, void *);
extern struct TreeNode *LobbyTimerMap_FindInsertPos(struct Tree *, u32 *, struct TreeNode **, u8 *, u8 *);
extern struct TreeNode *LobbyTimerMap_InsertNode(struct Tree *, struct TreeNode *, int, int, u32 *);
extern void LobbyString_ReplaceSubstring(struct Str *, u32, u32, struct Str *, u32, u32);
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *PPWLobbyWebRequest_Destroy(struct WebRequest *request);
void *PPWLobbyPlayers_Delete(struct Players *players);
void *PPWLobbyPlayer_Delete(struct Player *player);
void *PPWLobbyPlayerSet_Destroy(struct PlayerSet *a);
void LobbyTimerMap_Clear(struct Tree *a);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void LobbyStringVector_PushBackSlow(struct Vec *a, void *b, struct Alloc unused);
struct Vec *LobbyStringVector_Destroy(struct Vec *a);
struct Vec *LobbyU32Vector_Destroy(struct Vec *a);
struct Vec *LobbyVipVector_Destroy(struct Vec *a);
struct Vec *LobbyEventVector_Destroy(struct Vec *a);
void LobbyCharVector_Allocate(struct Vec *a, u32 b, struct Alloc unused);
u32 LobbyCharVector_RecommendSize(struct Vec *a, u32 n);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
struct Str *LobbyString_Destroy(struct Str *a);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);
void *LobbyEventDeque_Destroy(u32 *a);
void LobbyTimerMap_Insert(u32 *result, struct Tree *a, u32 *value);
void LobbyTimerMap_Erase(Iterator *result, struct Tree *a, Iterator node);
void LobbyCharVector_Grow(struct Vec *a, u32 n);
void LobbyCharVector_Reallocate(struct Vec *a, u32 unused, u32 n);
struct Str *LobbyString_InitCopy(struct Str *a, struct Str *b);
struct Str *LobbyString_InitSubstring(struct Str *a, struct Str *b, u32 offset, u32 length);
struct Str *LobbyString_InitFromCStr(struct Str *a, char *b);
extern char sStr_BasicStringReserveLengthError[], sStr_TreeInsertLengthError[];
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *PPWLobbyWebRequest_Destroy(struct WebRequest *request);
void *PPWLobbyPlayers_Delete(struct Players *players);
void *PPWLobbyPlayer_Delete(struct Player *player);
void *LobbyEventDeque_Destroy(u32 *a);
void *PPWLobbyPlayerSet_Destroy(struct PlayerSet *a);
void LobbyTimerMap_Insert(u32 *result, struct Tree *a, u32 *value);
void LobbyTimerMap_Erase(Iterator *result, struct Tree *a, Iterator node);
void LobbyTimerMap_Clear(struct Tree *a);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void LobbyStringVector_PushBackSlow(struct Vec *a, void *b, struct Alloc unused);
struct Vec *LobbyStringVector_Destroy(struct Vec *a);
struct Vec *LobbyU32Vector_Destroy(struct Vec *a);
struct Vec *LobbyVipVector_Destroy(struct Vec *a);
struct Vec *LobbyEventVector_Destroy(struct Vec *a);
void LobbyCharVector_Grow(struct Vec *a, u32 n);
void LobbyCharVector_Allocate(struct Vec *a, u32 b, struct Alloc unused);
void LobbyCharVector_Reallocate(struct Vec *a, u32 unused, u32 n);
u32 LobbyCharVector_RecommendSize(struct Vec *a, u32 n);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
struct Str *LobbyString_InitCopy(struct Str *a, struct Str *b);
struct Str *LobbyString_InitSubstring(struct Str *a, struct Str *b, u32 offset, u32 length);
struct Str *LobbyString_InitFromCStr(struct Str *a, char *b);
struct Str *LobbyString_Destroy(struct Str *a);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);
void LobbyString_Reserve(struct Str *a, u32 requested);
struct TreeNode *LobbyTimerMap_InsertNode(struct Tree *a, struct TreeNode *parent, int left, int end, u32 *value);
extern int PPWLobby_DecodeMessageData(void *, struct Vec *);
extern void *LobbyByteVector_Data(struct Vec *);
extern int PPWLobbySchedule_Load(void *, void *, u32);
extern void PPWLobbySchedule_CopyToVector(struct Vec *, void *), PPWLobby_SetChannelData(int, int, char *, void *, u32);
extern char sStr_b_lib_c_lobby[];
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *PPWLobbyWebRequest_Destroy(struct WebRequest *request);
void *PPWLobbyPlayers_Delete(struct Players *players);
void *PPWLobbyPlayer_Delete(struct Player *player);
void *LobbyEventDeque_Destroy(u32 *a);
void *PPWLobbyPlayerSet_Destroy(struct PlayerSet *a);
void LobbyTimerMap_Insert(u32 *result, struct Tree *a, u32 *value);
void LobbyTimerMap_Erase(Iterator *result, struct Tree *a, Iterator node);
void LobbyTimerMap_Clear(struct Tree *a);
struct TreeNode *LobbyTimerMap_InsertNode(struct Tree *a, struct TreeNode *parent, int left, int end, u32 *value);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void LobbyStringVector_PushBackSlow(struct Vec *a, void *b, struct Alloc unused);
struct Vec *LobbyStringVector_Destroy(struct Vec *a);
struct Vec *LobbyU32Vector_Destroy(struct Vec *a);
struct Vec *LobbyVipVector_Destroy(struct Vec *a);
struct Vec *LobbyEventVector_Destroy(struct Vec *a);
void LobbyCharVector_Grow(struct Vec *a, u32 n);
void LobbyCharVector_Allocate(struct Vec *a, u32 b, struct Alloc unused);
void LobbyCharVector_Reallocate(struct Vec *a, u32 unused, u32 n);
u32 LobbyCharVector_RecommendSize(struct Vec *a, u32 n);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
struct Str *LobbyString_InitCopy(struct Str *a, struct Str *b);
struct Str *LobbyString_InitSubstring(struct Str *a, struct Str *b, u32 offset, u32 length);
struct Str *LobbyString_InitFromCStr(struct Str *a, char *b);
struct Str *LobbyString_Destroy(struct Str *a);
void LobbyString_Reserve(struct Str *a, u32 requested);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);
void PPWLobby_OnEnumUsers(char *name, int id, int n, int *list);
void PPWLobby_OnGetLobbyData(int ok, int unused1, int unused2, int unused3, void *value);

void PPWLobby_OnTopicChanged(void) {
}

void *PPWLobbyWebRequest_Destroy(struct WebRequest *request) {
    LobbyCharVector_Destroy(&request->data);
    LobbyString_Destroy(&request->url);
    LobbyString_Destroy(&request->key);
    return request;
}

void *PPWLobbyPlayers_Delete(struct Players *players) {
    players->vtable = gPPWLobbyPlayersVtable;
    PPWLobbyPlayers_Clear(players);
    LobbyCharVector_Destroy(&players->self.userProfile);
    PPWLobbyPlayerSet_Destroy(&players->playerSet);
    if (players) {
        DWC_Free(0, players, 0);
    }
    return players;
}

void *PPWLobbyPlayer_Delete(struct Player *player) {
    LobbyCharVector_Destroy(&player->userProfile);
    if (player) {
        DWC_Free(0, player, 0);
    }
    return player;
}
}
}

namespace EventDequeDestroy {
extern "C" {

typedef struct Queue {
    u32 a;
    volatile u32 b;
    void **base;
    u32 start;
    u32 count, capacity;
} Queue;

extern void DWC_Free(int, void *, int), LobbyEventDequeMap_Destroy(void *);
Queue *LobbyEventDeque_Destroy(Queue *);

Queue *LobbyEventDeque_Destroy(Queue *p) {
    void **end, **cursor, **base, **limit;
    u32 capacity, start;
    u32 index = 0;
    volatile u8 tag = 0;
    p->b = 0;
    if (p->count) {
        if (p->capacity) {
            index = (p->start + ((volatile Queue *)p)->count) % p->capacity;
        }
        base = p->base;
        start = p->start;
        capacity = ((volatile Queue *)p)->capacity;
        end = base + index;
        cursor = base + start;
        limit = base + capacity;
        while (cursor != end) {
            if (*cursor) {
                DWC_Free(0, *cursor, 0);
            }
            if (++cursor == limit) {
                cursor = base;
            }
        }
    }
    LobbyEventDequeMap_Destroy(&p->base);
    return p;
}
}
}

namespace PlayerSetDestroy {
extern "C" {
void ov45_02236E58(Iterator *);
extern int printf(const char *, ...);
extern void abort(void);

struct Str {
    union {
        u32 flags;

        struct {
            u32 large : 1;
            u32 cap : 31;
        } bits;
    } u;

    u32 size;
    char *ptr;
};

struct Vec {
    void *ptr;
    u32 size, cap;
};

struct TreeNode {
    void *left, *right;
    u32 parent;
    u32 key;
    u32 data[6];
};

struct Tree {
    u32 count;
    void *root;
    u32 unk;
    void *end;
};

struct Alloc {
    u8 tag;
};

extern u8 gPPWLobbyPlayersVtable[], sStr_b_lib_u_user[], sStr_b_lib_u_system[], gPPWLobbyMainChannelMode[];
extern void DWC_Free(int, void *, int);
extern void *LobbyMem_Alloc(u32);
extern struct Str *LobbyString_Destroy(struct Str *);
extern struct Vec *LobbyCharVector_Destroy(struct Vec *);
extern struct Str *LobbyString_InitSubstring(struct Str *, struct Str *, u32, u32);
extern void PPWLobby_OnMessage(int, int, int, int);
extern void PPWLobby_HandleReceivedData(int, int, int, int, int, int, int);
extern void PPWLobbyPlayers_Clear(void *), LobbyEventDequeMap_Destroy(void *), PPWLobbyPlayerSet_DestroyNodes(void *, void *);
extern void LobbyStringVector_RecommendSize(struct Vec *, u32, int), LobbyStringVector_AppendCopiesSlow(struct Vec *, u32, void *), LobbyStringVector_EraseAtEnd(struct Vec *, u32, struct Alloc), LobbyCharVector_AppendZerosSlow(struct Vec *, u32);
extern void LobbyCharVector_Allocate(struct Vec *, u32, struct Alloc);
extern void LobbyTree_InsertRebalance(struct TreeNode *, void *), LobbyTimerMap_DestroyNodes(struct Tree *, struct TreeNode *);
extern struct TreeNode *LobbyTree_EraseRebalance(struct TreeNode *, void *);
extern void LobbyTimerMap_EraseKey(void *, int *), LobbyChat_SetChannelMode(int, char *);
extern void PPWLobbyPlayers_Add(void *, int), LobbyChat_GetChannelKeysAsync(int, int, char *, void (*)(), int);
extern void PPWLobbyChannel_GetName(struct Str *, void *);
extern struct Str *LobbyString_InitFromCStr(struct Str *, char *);
extern int LobbyString_Equals(struct Str *, struct Str *);
extern void LobbyString_Reserve(struct Str *, u32);
extern u32 LobbyCharVector_RecommendSize(struct Vec *, u32);
extern struct Str *LobbyString_Replace(struct Str *, u32, u32, struct Str *);
extern struct Str *LobbyString_ReplaceRange(struct Str *, u32, u32, char *, char *, struct Alloc);
extern void _ZNSt15__vector_commonILb1EE18throw_length_errorEv(void);
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *PPWLobbyWebRequest_Destroy(u8 *a);
void *PPWLobbyPlayers_Delete(u8 *a);
void *PPWLobbyPlayer_Delete(u8 *a);
void *PPWLobbyPlayerSet_Destroy(struct Tree *a);
void LobbyTimerMap_Clear(struct Tree *a);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void LobbyStringVector_PushBackSlow(struct Vec *a, void *b, struct Alloc unused);
struct Vec *LobbyStringVector_Destroy(struct Vec *a);
struct Vec *LobbyU32Vector_Destroy(struct Vec *a);
struct Vec *LobbyVipVector_Destroy(struct Vec *a);
struct Vec *LobbyEventVector_Destroy(struct Vec *a);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
void LobbyCharVector_Allocate(struct Vec *a, u32 b, struct Alloc unused);
u32 LobbyCharVector_RecommendSize(struct Vec *a, u32 n);
struct Str *LobbyString_Destroy(struct Str *a);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);

extern void LobbyEventDequeMap_Destroy(void *), PPWLobbyPlayerSet_DestroyNodes(void *, void *);
extern struct TreeNode *LobbyTimerMap_FindInsertPos(struct Tree *, u32 *, struct TreeNode **, u8 *, u8 *);
extern struct TreeNode *LobbyTimerMap_InsertNode(struct Tree *, struct TreeNode *, int, int, u32 *);
extern void LobbyString_ReplaceSubstring(struct Str *, u32, u32, struct Str *, u32, u32);
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *PPWLobbyWebRequest_Destroy(u8 *a);
void *PPWLobbyPlayers_Delete(u8 *a);
void *PPWLobbyPlayer_Delete(u8 *a);
void *PPWLobbyPlayerSet_Destroy(struct Tree *a);
void LobbyTimerMap_Clear(struct Tree *a);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void LobbyStringVector_PushBackSlow(struct Vec *a, void *b, struct Alloc unused);
struct Vec *LobbyStringVector_Destroy(struct Vec *a);
struct Vec *LobbyU32Vector_Destroy(struct Vec *a);
struct Vec *LobbyVipVector_Destroy(struct Vec *a);
struct Vec *LobbyEventVector_Destroy(struct Vec *a);
void LobbyCharVector_Allocate(struct Vec *a, u32 b, struct Alloc unused);
u32 LobbyCharVector_RecommendSize(struct Vec *a, u32 n);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
struct Str *LobbyString_Destroy(struct Str *a);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);
void *LobbyEventDeque_Destroy(u32 *a);
void LobbyTimerMap_Insert(u32 *result, struct Tree *a, u32 *value);
void LobbyTimerMap_Erase(Iterator *result, struct Tree *a, Iterator node);
void LobbyCharVector_Grow(struct Vec *a, u32 n);
void LobbyCharVector_Reallocate(struct Vec *a, u32 unused, u32 n);
struct Str *LobbyString_InitCopy(struct Str *a, struct Str *b);
struct Str *LobbyString_InitSubstring(struct Str *a, struct Str *b, u32 offset, u32 length);
struct Str *LobbyString_InitFromCStr(struct Str *a, char *b);
extern char sStr_BasicStringReserveLengthError[], sStr_TreeInsertLengthError[];
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *PPWLobbyWebRequest_Destroy(u8 *a);
void *PPWLobbyPlayers_Delete(u8 *a);
void *PPWLobbyPlayer_Delete(u8 *a);
void *LobbyEventDeque_Destroy(u32 *a);
void *PPWLobbyPlayerSet_Destroy(struct Tree *a);
void LobbyTimerMap_Insert(u32 *result, struct Tree *a, u32 *value);
void LobbyTimerMap_Erase(Iterator *result, struct Tree *a, Iterator node);
void LobbyTimerMap_Clear(struct Tree *a);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void LobbyStringVector_PushBackSlow(struct Vec *a, void *b, struct Alloc unused);
struct Vec *LobbyStringVector_Destroy(struct Vec *a);
struct Vec *LobbyU32Vector_Destroy(struct Vec *a);
struct Vec *LobbyVipVector_Destroy(struct Vec *a);
struct Vec *LobbyEventVector_Destroy(struct Vec *a);
void LobbyCharVector_Grow(struct Vec *a, u32 n);
void LobbyCharVector_Allocate(struct Vec *a, u32 b, struct Alloc unused);
void LobbyCharVector_Reallocate(struct Vec *a, u32 unused, u32 n);
u32 LobbyCharVector_RecommendSize(struct Vec *a, u32 n);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
struct Str *LobbyString_InitCopy(struct Str *a, struct Str *b);
struct Str *LobbyString_InitSubstring(struct Str *a, struct Str *b, u32 offset, u32 length);
struct Str *LobbyString_InitFromCStr(struct Str *a, char *b);
struct Str *LobbyString_Destroy(struct Str *a);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);
void LobbyString_Reserve(struct Str *a, u32 requested);
struct TreeNode *LobbyTimerMap_InsertNode(struct Tree *a, struct TreeNode *parent, int left, int end, u32 *value);
extern int PPWLobby_DecodeMessageData(void *, struct Vec *);
extern void *LobbyByteVector_Data(struct Vec *);
extern int PPWLobbySchedule_Load(void *, void *, u32);
extern void PPWLobbySchedule_CopyToVector(struct Vec *, void *), PPWLobby_SetChannelData(int, int, char *, void *, u32);
extern char sStr_b_lib_c_lobby[];
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *PPWLobbyWebRequest_Destroy(u8 *a);
void *PPWLobbyPlayers_Delete(u8 *a);
void *PPWLobbyPlayer_Delete(u8 *a);
void *LobbyEventDeque_Destroy(u32 *a);
void *PPWLobbyPlayerSet_Destroy(struct Tree *a);
void LobbyTimerMap_Insert(u32 *result, struct Tree *a, u32 *value);
void LobbyTimerMap_Erase(Iterator *result, struct Tree *a, Iterator node);
void LobbyTimerMap_Clear(struct Tree *a);
struct TreeNode *LobbyTimerMap_InsertNode(struct Tree *a, struct TreeNode *parent, int left, int end, u32 *value);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void LobbyStringVector_PushBackSlow(struct Vec *a, void *b, struct Alloc unused);
struct Vec *LobbyStringVector_Destroy(struct Vec *a);
struct Vec *LobbyU32Vector_Destroy(struct Vec *a);
struct Vec *LobbyVipVector_Destroy(struct Vec *a);
struct Vec *LobbyEventVector_Destroy(struct Vec *a);
void LobbyCharVector_Grow(struct Vec *a, u32 n);
void LobbyCharVector_Allocate(struct Vec *a, u32 b, struct Alloc unused);
void LobbyCharVector_Reallocate(struct Vec *a, u32 unused, u32 n);
u32 LobbyCharVector_RecommendSize(struct Vec *a, u32 n);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
struct Str *LobbyString_InitCopy(struct Str *a, struct Str *b);
struct Str *LobbyString_InitSubstring(struct Str *a, struct Str *b, u32 offset, u32 length);
struct Str *LobbyString_InitFromCStr(struct Str *a, char *b);
struct Str *LobbyString_Destroy(struct Str *a);
void LobbyString_Reserve(struct Str *a, u32 requested);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);
void PPWLobby_OnEnumUsers(char *name, int id, int n, int *list);
void PPWLobby_OnGetLobbyData(int ok, int unused1, int unused2, int unused3, void *value);

void *PPWLobbyPlayerSet_Destroy(struct Tree *a) {
    if (a->root) {
        PPWLobbyPlayerSet_DestroyNodes(a, a->root);
    }
    return a;
}
}
}

namespace TimerMapInsert {
extern "C" {

typedef struct Node {
    char pad[12];
    u32 value;
} Node;

typedef struct Result {
    Node *node;
    u8 flag;
    char pad[3];
} Result;

extern int LobbyTimerMap_FindInsertPos(void *, u32 *, Node **, u8 *, u8 *);
extern Node *LobbyTimerMap_InsertNode(void *, int, u8, u8, u32 *);
void LobbyTimerMap_Insert(Result *, void *, u32 *);

void LobbyTimerMap_Insert(Result *r, void *p, u32 *v) {
    u8 second, first;
    Node *node;
    int result = LobbyTimerMap_FindInsertPos(p, v, &node, &second, &first);
    if (!node || node->value < *v) {
        r->node = LobbyTimerMap_InsertNode(p, result, second, first, v);
        r->flag = gLobbyTimerMapInserted;
    } else {
        r->node = node;
        r->flag = gLobbyTimerMapNotInserted;
    }
}
}
}

namespace TimerMapErase {
extern "C" {
void ov45_02236E58(Iterator *);
extern int printf(const char *, ...);
extern void abort(void);

struct Str {
    union {
        u32 flags;

        struct {
            u32 large : 1;
            u32 cap : 31;
        } bits;
    } u;

    u32 size;
    char *ptr;
};

struct Vec {
    void *ptr;
    u32 size, cap;
};

struct TreeNode {
    void *left, *right;
    u32 parent;
    u32 key;
    u32 data[6];
};

struct Tree {
    u32 count;
    void *root;
    u32 unk;
    void *end;
};

struct Alloc {
    u8 tag;
};

extern u8 gPPWLobbyPlayersVtable[], sStr_b_lib_u_user[], sStr_b_lib_u_system[], gPPWLobbyMainChannelMode[];
extern void DWC_Free(int, void *, int);
extern void *LobbyMem_Alloc(u32);
extern struct Str *LobbyString_Destroy(struct Str *);
extern struct Vec *LobbyCharVector_Destroy(struct Vec *);
extern struct Str *LobbyString_InitSubstring(struct Str *, struct Str *, u32, u32);
extern void PPWLobby_OnMessage(int, int, int, int);
extern void PPWLobby_HandleReceivedData(int, int, int, int, int, int, int);
extern void PPWLobbyPlayers_Clear(void *), LobbyEventDequeMap_Destroy(void *), PPWLobbyPlayerSet_DestroyNodes(void *, void *);
extern void LobbyStringVector_RecommendSize(struct Vec *, u32, int), LobbyStringVector_AppendCopiesSlow(struct Vec *, u32, void *), LobbyStringVector_EraseAtEnd(struct Vec *, u32, struct Alloc), LobbyCharVector_AppendZerosSlow(struct Vec *, u32);
extern void LobbyCharVector_Allocate(struct Vec *, u32, struct Alloc);
extern void LobbyTree_InsertRebalance(struct TreeNode *, void *), LobbyTimerMap_DestroyNodes(struct Tree *, struct TreeNode *);
extern struct TreeNode *LobbyTree_EraseRebalance(struct TreeNode *, void *);
extern void LobbyTimerMap_EraseKey(void *, int *), LobbyChat_SetChannelMode(int, char *);
extern void PPWLobbyPlayers_Add(void *, int), LobbyChat_GetChannelKeysAsync(int, int, char *, void (*)(), int);
extern void PPWLobbyChannel_GetName(struct Str *, void *);
extern struct Str *LobbyString_InitFromCStr(struct Str *, char *);
extern int LobbyString_Equals(struct Str *, struct Str *);
extern void LobbyString_Reserve(struct Str *, u32);
extern u32 LobbyCharVector_RecommendSize(struct Vec *, u32);
extern struct Str *LobbyString_Replace(struct Str *, u32, u32, struct Str *);
extern struct Str *LobbyString_ReplaceRange(struct Str *, u32, u32, char *, char *, struct Alloc);
extern void _ZNSt15__vector_commonILb1EE18throw_length_errorEv(void);
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *PPWLobbyWebRequest_Destroy(u8 *a);
void *PPWLobbyPlayers_Delete(u8 *a);
void *PPWLobbyPlayer_Delete(u8 *a);
void *PPWLobbyPlayerSet_Destroy(struct Tree *a);
void LobbyTimerMap_Clear(struct Tree *a);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void LobbyStringVector_PushBackSlow(struct Vec *a, void *b, struct Alloc unused);
struct Vec *LobbyStringVector_Destroy(struct Vec *a);
struct Vec *LobbyU32Vector_Destroy(struct Vec *a);
struct Vec *LobbyVipVector_Destroy(struct Vec *a);
struct Vec *LobbyEventVector_Destroy(struct Vec *a);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
void LobbyCharVector_Allocate(struct Vec *a, u32 b, struct Alloc unused);
u32 LobbyCharVector_RecommendSize(struct Vec *a, u32 n);
struct Str *LobbyString_Destroy(struct Str *a);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);

extern void LobbyEventDequeMap_Destroy(void *), PPWLobbyPlayerSet_DestroyNodes(void *, void *);
extern struct TreeNode *LobbyTimerMap_FindInsertPos(struct Tree *, u32 *, struct TreeNode **, u8 *, u8 *);
extern struct TreeNode *LobbyTimerMap_InsertNode(struct Tree *, struct TreeNode *, int, int, u32 *);
extern void LobbyString_ReplaceSubstring(struct Str *, u32, u32, struct Str *, u32, u32);
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *PPWLobbyWebRequest_Destroy(u8 *a);
void *PPWLobbyPlayers_Delete(u8 *a);
void *PPWLobbyPlayer_Delete(u8 *a);
void *PPWLobbyPlayerSet_Destroy(struct Tree *a);
void LobbyTimerMap_Clear(struct Tree *a);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void LobbyStringVector_PushBackSlow(struct Vec *a, void *b, struct Alloc unused);
struct Vec *LobbyStringVector_Destroy(struct Vec *a);
struct Vec *LobbyU32Vector_Destroy(struct Vec *a);
struct Vec *LobbyVipVector_Destroy(struct Vec *a);
struct Vec *LobbyEventVector_Destroy(struct Vec *a);
void LobbyCharVector_Allocate(struct Vec *a, u32 b, struct Alloc unused);
u32 LobbyCharVector_RecommendSize(struct Vec *a, u32 n);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
struct Str *LobbyString_Destroy(struct Str *a);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);
void *LobbyEventDeque_Destroy(u32 *a);
void LobbyTimerMap_Insert(u32 *result, struct Tree *a, u32 *value);
void LobbyTimerMap_Erase(Iterator *result, struct Tree *a, Iterator node);
void LobbyCharVector_Grow(struct Vec *a, u32 n);
void LobbyCharVector_Reallocate(struct Vec *a, u32 unused, u32 n);
struct Str *LobbyString_InitCopy(struct Str *a, struct Str *b);
struct Str *LobbyString_InitSubstring(struct Str *a, struct Str *b, u32 offset, u32 length);
struct Str *LobbyString_InitFromCStr(struct Str *a, char *b);
extern char sStr_BasicStringReserveLengthError[], sStr_TreeInsertLengthError[];
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *PPWLobbyWebRequest_Destroy(u8 *a);
void *PPWLobbyPlayers_Delete(u8 *a);
void *PPWLobbyPlayer_Delete(u8 *a);
void *LobbyEventDeque_Destroy(u32 *a);
void *PPWLobbyPlayerSet_Destroy(struct Tree *a);
void LobbyTimerMap_Insert(u32 *result, struct Tree *a, u32 *value);
void LobbyTimerMap_Erase(Iterator *result, struct Tree *a, Iterator node);
void LobbyTimerMap_Clear(struct Tree *a);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void LobbyStringVector_PushBackSlow(struct Vec *a, void *b, struct Alloc unused);
struct Vec *LobbyStringVector_Destroy(struct Vec *a);
struct Vec *LobbyU32Vector_Destroy(struct Vec *a);
struct Vec *LobbyVipVector_Destroy(struct Vec *a);
struct Vec *LobbyEventVector_Destroy(struct Vec *a);
void LobbyCharVector_Grow(struct Vec *a, u32 n);
void LobbyCharVector_Allocate(struct Vec *a, u32 b, struct Alloc unused);
void LobbyCharVector_Reallocate(struct Vec *a, u32 unused, u32 n);
u32 LobbyCharVector_RecommendSize(struct Vec *a, u32 n);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
struct Str *LobbyString_InitCopy(struct Str *a, struct Str *b);
struct Str *LobbyString_InitSubstring(struct Str *a, struct Str *b, u32 offset, u32 length);
struct Str *LobbyString_InitFromCStr(struct Str *a, char *b);
struct Str *LobbyString_Destroy(struct Str *a);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);
void LobbyString_Reserve(struct Str *a, u32 requested);
struct TreeNode *LobbyTimerMap_InsertNode(struct Tree *a, struct TreeNode *parent, int left, int end, u32 *value);
extern int PPWLobby_DecodeMessageData(void *, struct Vec *);
extern void *LobbyByteVector_Data(struct Vec *);
extern int PPWLobbySchedule_Load(void *, void *, u32);
extern void PPWLobbySchedule_CopyToVector(struct Vec *, void *), PPWLobby_SetChannelData(int, int, char *, void *, u32);
extern char sStr_b_lib_c_lobby[];
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *PPWLobbyWebRequest_Destroy(u8 *a);
void *PPWLobbyPlayers_Delete(u8 *a);
void *PPWLobbyPlayer_Delete(u8 *a);
void *LobbyEventDeque_Destroy(u32 *a);
void *PPWLobbyPlayerSet_Destroy(struct Tree *a);
void LobbyTimerMap_Insert(u32 *result, struct Tree *a, u32 *value);
void LobbyTimerMap_Erase(Iterator *result, struct Tree *a, Iterator node);
void LobbyTimerMap_Clear(struct Tree *a);
struct TreeNode *LobbyTimerMap_InsertNode(struct Tree *a, struct TreeNode *parent, int left, int end, u32 *value);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void LobbyStringVector_PushBackSlow(struct Vec *a, void *b, struct Alloc unused);
struct Vec *LobbyStringVector_Destroy(struct Vec *a);
struct Vec *LobbyU32Vector_Destroy(struct Vec *a);
struct Vec *LobbyVipVector_Destroy(struct Vec *a);
struct Vec *LobbyEventVector_Destroy(struct Vec *a);
void LobbyCharVector_Grow(struct Vec *a, u32 n);
void LobbyCharVector_Allocate(struct Vec *a, u32 b, struct Alloc unused);
void LobbyCharVector_Reallocate(struct Vec *a, u32 unused, u32 n);
u32 LobbyCharVector_RecommendSize(struct Vec *a, u32 n);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
struct Str *LobbyString_InitCopy(struct Str *a, struct Str *b);
struct Str *LobbyString_InitSubstring(struct Str *a, struct Str *b, u32 offset, u32 length);
struct Str *LobbyString_InitFromCStr(struct Str *a, char *b);
struct Str *LobbyString_Destroy(struct Str *a);
void LobbyString_Reserve(struct Str *a, u32 requested);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);
void PPWLobby_OnEnumUsers(char *name, int id, int n, int *list);
void PPWLobby_OnGetLobbyData(int ok, int unused1, int unused2, int unused3, void *value);

void LobbyTimerMap_Erase(Iterator *result, struct Tree *a, Iterator node) {
    struct TreeNode *old = (struct TreeNode *)node.current;
    if (old == a->end) {
        ov45_02236E58(&node);
        a->end = node.current;
    }
    result->current = LobbyTree_EraseRebalance(old, a->root);
    if (old) {
        DWC_Free(0, old, 0);
    }
    a->count--;
}

void LobbyTimerMap_Clear(struct Tree *a) {
    if (!a->root) {
        return;
    }
    LobbyTimerMap_DestroyNodes(a, (struct TreeNode *)a->root);
    a->count = 0;
    a->root = 0;
    a->end = (struct TreeNode *)&a->root;
}
}
}

namespace TimerMapInsertNode {
extern "C" int printf(const char *, ...);
extern "C" void abort(void);
extern "C" char sStr_TreeInsertLengthError[];
extern "C" void *LobbyMem_Alloc(u32);
extern "C" void DWC_Free(int, void *, int);

class Alloc {};

struct Pair8 {
    u64 v;
};

struct Inner {
    u64 x;
    u64 y;
    u32 p;
    u32 q;
};

template <class A, class B>
struct Pair {
    A first;
    B second;

    Pair(const A &a, const B &b)
        : first(a) {
        B *dst = &second;
        *dst = b;
    }
};

typedef Pair<u32, Inner> Value;

struct Node {
    Node *left, *right, *parent;
    Value val;
};

struct Tree {
    u32 count;
    Node *root;
    u32 unk8;
    Node *leftmost;
};

class Holder {
public:
    Node *a;
    Node *b;
    Node **alloc;
    Node *volatile ptr;

    Holder(Node *n, Node **al)
        : a(n)
        , b(n)
        , alloc(al)
        , ptr(n) {
    }

    void Destroy() {
        Node *p = ptr;
        if (p) {
            DWC_Free(0, p, 0);
        }
    }

    void Release() {
        ptr = 0;
    }
};

inline Node *Allocate(Alloc) {
    return (Node *)LobbyMem_Alloc(sizeof(Node));
}

extern "C" void LobbyTree_InsertRebalance(Node *, Node *);
extern "C" Node *LobbyTimerMap_InsertNode(Tree *, Node *, int, int, Value *);

extern "C" Node *LobbyTimerMap_InsertNode(Tree *t, Node *parent, int left, int leftmost, Value *v) {
    if (t->count == (u32)-1) {
        printf(sStr_TreeInsertLengthError);
        abort();
    }
    Node *n = Allocate(Alloc());
    Holder h(n, &t->root);
    new (&n->val) Value(v->first, v->second);
    n->right = 0;
    h.Release();
    n->left = 0;
    new (&n->parent) Node *(parent);
    if (left) {
        parent->left = n;
    } else {
        parent->right = n;
    }
    t->count++;
    LobbyTree_InsertRebalance(n, t->root);
    if (leftmost) {
        t->leftmost = n;
    }
    h.Destroy();
    return n;
}

}

namespace TimerMapDestroy {
extern "C" {
void ov45_02236E58(Iterator *);
extern int printf(const char *, ...);
extern void abort(void);

struct Str {
    union {
        u32 flags;

        struct {
            u32 large : 1;
            u32 cap : 31;
        } bits;
    } u;

    u32 size;
    char *ptr;
};

struct Vec {
    void *ptr;
    u32 size, cap;
};

struct TreeNode {
    void *left, *right;
    u32 parent;
    u32 key;
    u32 data[6];
};

struct Tree {
    u32 count;
    void *root;
    u32 unk;
    void *end;
};

struct Alloc {
    u8 tag;
};

extern u8 gPPWLobbyPlayersVtable[], sStr_b_lib_u_user[], sStr_b_lib_u_system[], gPPWLobbyMainChannelMode[];
extern void DWC_Free(int, void *, int);
extern void *LobbyMem_Alloc(u32);
extern struct Str *LobbyString_Destroy(struct Str *);
extern struct Vec *LobbyCharVector_Destroy(struct Vec *);
extern struct Str *LobbyString_InitSubstring(struct Str *, struct Str *, u32, u32);
extern void PPWLobby_OnMessage(int, int, int, int);
extern void PPWLobby_HandleReceivedData(int, int, int, int, int, int, int);
extern void PPWLobbyPlayers_Clear(void *), LobbyEventDequeMap_Destroy(void *), PPWLobbyPlayerSet_DestroyNodes(void *, void *);
extern void LobbyStringVector_RecommendSize(struct Vec *, u32, int), LobbyStringVector_AppendCopiesSlow(struct Vec *, u32, void *), LobbyStringVector_EraseAtEnd(struct Vec *, u32, struct Alloc), LobbyCharVector_AppendZerosSlow(struct Vec *, u32);
extern void LobbyCharVector_Allocate(struct Vec *, u32, struct Alloc);
extern void LobbyTree_InsertRebalance(struct TreeNode *, void *), LobbyTimerMap_DestroyNodes(struct Tree *, struct TreeNode *);
extern struct TreeNode *LobbyTree_EraseRebalance(struct TreeNode *, void *);
extern void LobbyTimerMap_EraseKey(void *, int *), LobbyChat_SetChannelMode(int, char *);
extern void PPWLobbyPlayers_Add(void *, int), LobbyChat_GetChannelKeysAsync(int, int, char *, void (*)(), int);
extern void PPWLobbyChannel_GetName(struct Str *, void *);
extern struct Str *LobbyString_InitFromCStr(struct Str *, char *);
extern int LobbyString_Equals(struct Str *, struct Str *);
extern void LobbyString_Reserve(struct Str *, u32);
extern u32 LobbyCharVector_RecommendSize(struct Vec *, u32);
extern struct Str *LobbyString_Replace(struct Str *, u32, u32, struct Str *);
extern struct Str *LobbyString_ReplaceRange(struct Str *, u32, u32, char *, char *, struct Alloc);
extern void _ZNSt15__vector_commonILb1EE18throw_length_errorEv(void);
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *PPWLobbyWebRequest_Destroy(u8 *a);
void *PPWLobbyPlayers_Delete(u8 *a);
void *PPWLobbyPlayer_Delete(u8 *a);
void *PPWLobbyPlayerSet_Destroy(struct Tree *a);
void LobbyTimerMap_Clear(struct Tree *a);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void LobbyStringVector_PushBackSlow(struct Vec *a, void *b, struct Alloc unused);
struct Vec *LobbyStringVector_Destroy(struct Vec *a);
struct Vec *LobbyU32Vector_Destroy(struct Vec *a);
struct Vec *LobbyVipVector_Destroy(struct Vec *a);
struct Vec *LobbyEventVector_Destroy(struct Vec *a);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
void LobbyCharVector_Allocate(struct Vec *a, u32 b, struct Alloc unused);
u32 LobbyCharVector_RecommendSize(struct Vec *a, u32 n);
struct Str *LobbyString_Destroy(struct Str *a);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);

extern void LobbyEventDequeMap_Destroy(void *), PPWLobbyPlayerSet_DestroyNodes(void *, void *);
extern struct TreeNode *LobbyTimerMap_FindInsertPos(struct Tree *, u32 *, struct TreeNode **, u8 *, u8 *);
extern struct TreeNode *LobbyTimerMap_InsertNode(struct Tree *, struct TreeNode *, int, int, u32 *);
extern void LobbyString_ReplaceSubstring(struct Str *, u32, u32, struct Str *, u32, u32);
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *PPWLobbyWebRequest_Destroy(u8 *a);
void *PPWLobbyPlayers_Delete(u8 *a);
void *PPWLobbyPlayer_Delete(u8 *a);
void *PPWLobbyPlayerSet_Destroy(struct Tree *a);
void LobbyTimerMap_Clear(struct Tree *a);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void LobbyStringVector_PushBackSlow(struct Vec *a, void *b, struct Alloc unused);
struct Vec *LobbyStringVector_Destroy(struct Vec *a);
struct Vec *LobbyU32Vector_Destroy(struct Vec *a);
struct Vec *LobbyVipVector_Destroy(struct Vec *a);
struct Vec *LobbyEventVector_Destroy(struct Vec *a);
void LobbyCharVector_Allocate(struct Vec *a, u32 b, struct Alloc unused);
u32 LobbyCharVector_RecommendSize(struct Vec *a, u32 n);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
struct Str *LobbyString_Destroy(struct Str *a);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);
void *LobbyEventDeque_Destroy(u32 *a);
void LobbyTimerMap_Insert(u32 *result, struct Tree *a, u32 *value);
void LobbyTimerMap_Erase(Iterator *result, struct Tree *a, Iterator node);
void LobbyCharVector_Grow(struct Vec *a, u32 n);
void LobbyCharVector_Reallocate(struct Vec *a, u32 unused, u32 n);
struct Str *LobbyString_InitCopy(struct Str *a, struct Str *b);
struct Str *LobbyString_InitSubstring(struct Str *a, struct Str *b, u32 offset, u32 length);
struct Str *LobbyString_InitFromCStr(struct Str *a, char *b);
extern char sStr_BasicStringReserveLengthError[], sStr_TreeInsertLengthError[];
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *PPWLobbyWebRequest_Destroy(u8 *a);
void *PPWLobbyPlayers_Delete(u8 *a);
void *PPWLobbyPlayer_Delete(u8 *a);
void *LobbyEventDeque_Destroy(u32 *a);
void *PPWLobbyPlayerSet_Destroy(struct Tree *a);
void LobbyTimerMap_Insert(u32 *result, struct Tree *a, u32 *value);
void LobbyTimerMap_Erase(Iterator *result, struct Tree *a, Iterator node);
void LobbyTimerMap_Clear(struct Tree *a);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void LobbyStringVector_PushBackSlow(struct Vec *a, void *b, struct Alloc unused);
struct Vec *LobbyStringVector_Destroy(struct Vec *a);
struct Vec *LobbyU32Vector_Destroy(struct Vec *a);
struct Vec *LobbyVipVector_Destroy(struct Vec *a);
struct Vec *LobbyEventVector_Destroy(struct Vec *a);
void LobbyCharVector_Grow(struct Vec *a, u32 n);
void LobbyCharVector_Allocate(struct Vec *a, u32 b, struct Alloc unused);
void LobbyCharVector_Reallocate(struct Vec *a, u32 unused, u32 n);
u32 LobbyCharVector_RecommendSize(struct Vec *a, u32 n);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
struct Str *LobbyString_InitCopy(struct Str *a, struct Str *b);
struct Str *LobbyString_InitSubstring(struct Str *a, struct Str *b, u32 offset, u32 length);
struct Str *LobbyString_InitFromCStr(struct Str *a, char *b);
struct Str *LobbyString_Destroy(struct Str *a);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);
void LobbyString_Reserve(struct Str *a, u32 requested);
struct TreeNode *LobbyTimerMap_InsertNode(struct Tree *a, struct TreeNode *parent, int left, int end, u32 *value);
extern int PPWLobby_DecodeMessageData(void *, struct Vec *);
extern void *LobbyByteVector_Data(struct Vec *);
extern int PPWLobbySchedule_Load(void *, void *, u32);
extern void PPWLobbySchedule_CopyToVector(struct Vec *, void *), PPWLobby_SetChannelData(int, int, char *, void *, u32);
extern char sStr_b_lib_c_lobby[];
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *PPWLobbyWebRequest_Destroy(u8 *a);
void *PPWLobbyPlayers_Delete(u8 *a);
void *PPWLobbyPlayer_Delete(u8 *a);
void *LobbyEventDeque_Destroy(u32 *a);
void *PPWLobbyPlayerSet_Destroy(struct Tree *a);
void LobbyTimerMap_Insert(u32 *result, struct Tree *a, u32 *value);
void LobbyTimerMap_Erase(Iterator *result, struct Tree *a, Iterator node);
void LobbyTimerMap_Clear(struct Tree *a);
struct TreeNode *LobbyTimerMap_InsertNode(struct Tree *a, struct TreeNode *parent, int left, int end, u32 *value);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void LobbyStringVector_PushBackSlow(struct Vec *a, void *b, struct Alloc unused);
struct Vec *LobbyStringVector_Destroy(struct Vec *a);
struct Vec *LobbyU32Vector_Destroy(struct Vec *a);
struct Vec *LobbyVipVector_Destroy(struct Vec *a);
struct Vec *LobbyEventVector_Destroy(struct Vec *a);
void LobbyCharVector_Grow(struct Vec *a, u32 n);
void LobbyCharVector_Allocate(struct Vec *a, u32 b, struct Alloc unused);
void LobbyCharVector_Reallocate(struct Vec *a, u32 unused, u32 n);
u32 LobbyCharVector_RecommendSize(struct Vec *a, u32 n);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
struct Str *LobbyString_InitCopy(struct Str *a, struct Str *b);
struct Str *LobbyString_InitSubstring(struct Str *a, struct Str *b, u32 offset, u32 length);
struct Str *LobbyString_InitFromCStr(struct Str *a, char *b);
struct Str *LobbyString_Destroy(struct Str *a);
void LobbyString_Reserve(struct Str *a, u32 requested);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);
void PPWLobby_OnEnumUsers(char *name, int id, int n, int *list);
void PPWLobby_OnGetLobbyData(int ok, int unused1, int unused2, int unused3, void *value);

void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b) {
    if (b->left) {
        LobbyTimerMap_DestroyNodes(a, (struct TreeNode *)b->left);
    }
    if (b->right) {
        LobbyTimerMap_DestroyNodes(a, (struct TreeNode *)b->right);
    }
    if (b) {
        DWC_Free(0, b, 0);
    }
}

void *LobbyTimerMap_Destroy(struct Tree *a) {
    if (a->root) {
        LobbyTimerMap_DestroyNodes(a, (struct TreeNode *)a->root);
    }
    return a;
}
}
}

namespace TreeIterator {

struct TreeNode {
    void *left;
    void *right;
    u32 parent;
};

static inline void *Left(void *node) {
    return static_cast<TreeNode *>(node)->left;
}

static inline void *Right(void *node) {
    return static_cast<TreeNode *>(node)->right;
}

static inline void *Parent(void *node) {
    return reinterpret_cast<void *>(static_cast<TreeNode *>(node)->parent & ~1UL);
}

extern "C" void ov45_02236E58(Iterator *it);

extern "C" void ov45_02236E58(Iterator *it) {
    if (Right(it->current)) {
        void *next = Right(it->current);
        void *left;
        while ((left = Left(next)) != 0) {
            next = left;
        }
        it->current = next;
        return;
    }
    void *node = it->current;
    void *parent = Parent(node);
    while (node != Left(parent)) {
        it->current = parent;
        node = it->current;
        parent = Parent(node);
    }
    it->current = parent;
}

}

namespace TreeInsertRebalance {
extern "C" {
void ov45_02236E58(Iterator *);

typedef struct Node {
    void *left, *right;
    u32 parent;
} Node;

static inline Node *Parent(Node *n) {
    return (Node *)(n->parent & ~1);
}

static inline void Set(Node **p, Node *n) {
    *p = n;
}

void ov45_02236E58(Iterator *);
extern void LobbyTree_RotateLeft(Node *, void **), LobbyTree_RotateRight(Node *, void **);

static inline int Red(Node *n) {
    return (n->parent & 1) ? 1 : 0;
}

static inline void Black(Node *n) {
    n->parent &= ~1;
}

static inline void Paint(Node *n) {
    n->parent |= 1;
}

void LobbyTree_InsertRebalance(Node *, void *);

void LobbyTree_InsertRebalance(Node *n, void *volatile root) {
    Paint(n);
    while (n != root && Red(Parent(n)) == 1) {
        Node *p = Parent(n), *g = Parent(p);
        if (p == g->left) {
            Node *u = (Node *)g->right;
            if (u && Red(u) == 1) {
                Black(p);
                Black(u);
                n = Parent(Parent(n));
                Paint(n);
            } else {
                if (n == p->right) {
                    n = p;
                    LobbyTree_RotateLeft(n, (void **)&root);
                }
                Black(Parent(n));
                Paint(Parent(Parent(n)));
                LobbyTree_RotateRight(Parent(Parent(n)), (void **)&root);
            }
        } else {
            Node *u = (Node *)g->left;
            if (u && Red(u) == 1) {
                Black(p);
                Black(u);
                n = Parent(Parent(n));
                Paint(n);
            } else {
                if (n == p->left) {
                    n = p;
                    LobbyTree_RotateRight(n, (void **)&root);
                }
                Black(Parent(n));
                Paint(Parent(Parent(n)));
                LobbyTree_RotateLeft(Parent(Parent(n)), (void **)&root);
            }
        }
    }
    Black((Node *)root);
}
}
}

namespace TreeEraseRebalance {
extern "C" {
void ov45_02236E58(Iterator *);

typedef struct Node {
    struct Node *left, *right;
    u32 tagged;
} Node;

extern void ov45_02236E58(Iterator *), LobbyTree_RotateLeft(Node *, void **), LobbyTree_RotateRight(Node *, void **);
Node *LobbyTree_EraseRebalance(Node *, void *);

static inline Node *parent(Node *n) {
    return (Node *)(n->tagged & ~1);
}

static inline int red(Node *n) {
    if (*(volatile u32 *)&n->tagged & 1) {
        return 1;
    }
    return 0;
}

static inline void setParent(Node *n, Node *p) {
    n->tagged = (u32)p | (n->tagged & 1);
}

static inline void setRed(Node *n, int r) {
    u32 t = n->tagged;
    if (r) {
        t |= 1;
    } else {
        t &= ~1;
    }
    n->tagged = t;
}

static inline int redLoaded(Node *n, u32 *tag) {
    u32 t = *(volatile u32 *)&n->tagged;
    *tag = t;
    if (t & 1) {
        return 1;
    }
    return 0;
}

Node *LobbyTree_EraseRebalance(Node *node, void *root) {
    Iterator iter = { node };
    Node *removed, *child, *up;
    int left, fix;
    ov45_02236E58(&iter);
    removed = node;
    if (node->left && node->right) {
        removed = (Node *)iter.current;
    }
    child = removed->left;
    if (!child) {
        child = removed->right;
    }
    up = parent(removed);
    if (child) {
        setParent(child, up);
    }
    {
        Node *p = parent(removed);
        if (removed == p->left) {
            p->left = child;
            left = 1;
        } else {
            p->right = child;
            left = 0;
        }
    }
    fix = !red(removed);
    if (removed != node) {
        Node *p = parent(node);
        setParent(removed, p);
        p = parent(removed);
        if (node == p->left) {
            p->left = removed;
        } else {
            p->right = removed;
        }
        {
            Node *c = node->left;
            removed->left = c;
            if (!c) {
                up = removed;
            } else {
                setParent(c, removed);
            }
        }
        {
            Node *c = node->right;
            removed->right = c;
            if (!c) {
                up = removed;
            } else {
                setParent(c, removed);
            }
        }
        setRed(removed, red(node));
        if (root == node) {
            root = removed;
        }
    } else if (!child && node == root) {
        fix = 0;
    }
    if (fix) {
        while (child != root && (!child || !red(child))) {
            Node *sibling;
            u32 colorword;
            if (left) {
                sibling = up->right;
                if (redLoaded(sibling, &colorword) == 1) {
                    sibling->tagged = colorword & ~1;
                    setRed(up, 1);
                    LobbyTree_RotateLeft(up, &root);
                    sibling = up->right;
                }
                if ((!sibling->left || !red(sibling->left)) && (!sibling->right || !red(sibling->right))) {
                    setRed(sibling, 1);
                    child = up;
                    up = parent(up);
                    left = (child == up->left);
                } else {
                    if (!sibling->right || !red(sibling->right)) {
                        setRed(sibling->left, 0);
                        setRed(sibling, 1);
                        LobbyTree_RotateRight(sibling, &root);
                        sibling = up->right;
                    }
                    setRed(sibling, red(up));
                    setRed(up, 0);
                    setRed(sibling->right, 0);
                    LobbyTree_RotateLeft(up, &root);
                    child = (Node *)root;
                }
            } else {
                sibling = up->left;
                if (redLoaded(sibling, &colorword) == 1) {
                    sibling->tagged = colorword & ~1;
                    setRed(up, 1);
                    LobbyTree_RotateRight(up, &root);
                    sibling = up->left;
                }
                if ((!sibling->left || !red(sibling->left)) && (!sibling->right || !red(sibling->right))) {
                    setRed(sibling, 1);
                    child = up;
                    up = parent(up);
                    left = (child == up->left);
                } else {
                    if (!sibling->left || !red(sibling->left)) {
                        setRed(sibling->right, 0);
                        setRed(sibling, 1);
                        LobbyTree_RotateLeft(sibling, &root);
                        sibling = up->left;
                    }
                    setRed(sibling, red(up));
                    setRed(up, 0);
                    setRed(sibling->left, 0);
                    LobbyTree_RotateRight(up, &root);
                    child = (Node *)root;
                }
            }
        }
        if (child) {
            setRed(child, 0);
        }
    }
    return (Node *)iter.current;
}
}
}

namespace TreeRotate {
extern "C" {
void ov45_02236E58(Iterator *);
extern int printf(const char *, ...);
extern void abort(void);

struct Str {
    union {
        u32 flags;

        struct {
            u32 large : 1;
            u32 cap : 31;
        } bits;
    } u;

    u32 size;
    char *ptr;
};

struct Vec {
    void *ptr;
    u32 size, cap;
};

struct TreeNode {
    void *left, *right;
    u32 parent;
    u32 key;
    u32 data[6];
};

struct Tree {
    u32 count;
    void *root;
    u32 unk;
    void *end;
};

struct Alloc {
    u8 tag;
};

extern u8 gPPWLobbyPlayersVtable[], sStr_b_lib_u_user[], sStr_b_lib_u_system[], gPPWLobbyMainChannelMode[];
extern void DWC_Free(int, void *, int);
extern void *LobbyMem_Alloc(u32);
extern struct Str *LobbyString_Destroy(struct Str *);
extern struct Vec *LobbyCharVector_Destroy(struct Vec *);
extern struct Str *LobbyString_InitSubstring(struct Str *, struct Str *, u32, u32);
extern void PPWLobby_OnMessage(int, int, int, int);
extern void PPWLobby_HandleReceivedData(int, int, int, int, int, int, int);
extern void PPWLobbyPlayers_Clear(void *), LobbyEventDequeMap_Destroy(void *), PPWLobbyPlayerSet_DestroyNodes(void *, void *);
extern void LobbyStringVector_RecommendSize(struct Vec *, u32, int), LobbyStringVector_AppendCopiesSlow(struct Vec *, u32, void *), LobbyStringVector_EraseAtEnd(struct Vec *, u32, struct Alloc), LobbyCharVector_AppendZerosSlow(struct Vec *, u32);
extern void LobbyCharVector_Allocate(struct Vec *, u32, struct Alloc);
extern void LobbyTree_InsertRebalance(struct TreeNode *, void *), LobbyTimerMap_DestroyNodes(struct Tree *, struct TreeNode *);
extern struct TreeNode *LobbyTree_EraseRebalance(struct TreeNode *, void *);
extern void LobbyTimerMap_EraseKey(void *, int *), LobbyChat_SetChannelMode(int, char *);
extern void PPWLobbyPlayers_Add(void *, int), LobbyChat_GetChannelKeysAsync(int, int, char *, void (*)(), int);
extern void PPWLobbyChannel_GetName(struct Str *, void *);
extern struct Str *LobbyString_InitFromCStr(struct Str *, char *);
extern int LobbyString_Equals(struct Str *, struct Str *);
extern void LobbyString_Reserve(struct Str *, u32);
extern u32 LobbyCharVector_RecommendSize(struct Vec *, u32);
extern struct Str *LobbyString_Replace(struct Str *, u32, u32, struct Str *);
extern struct Str *LobbyString_ReplaceRange(struct Str *, u32, u32, char *, char *, struct Alloc);
extern void _ZNSt15__vector_commonILb1EE18throw_length_errorEv(void);
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *PPWLobbyWebRequest_Destroy(u8 *a);
void *PPWLobbyPlayers_Delete(u8 *a);
void *PPWLobbyPlayer_Delete(u8 *a);
void *PPWLobbyPlayerSet_Destroy(struct Tree *a);
void LobbyTimerMap_Clear(struct Tree *a);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void LobbyStringVector_PushBackSlow(struct Vec *a, void *b, struct Alloc unused);
struct Vec *LobbyStringVector_Destroy(struct Vec *a);
struct Vec *LobbyU32Vector_Destroy(struct Vec *a);
struct Vec *LobbyVipVector_Destroy(struct Vec *a);
struct Vec *LobbyEventVector_Destroy(struct Vec *a);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
void LobbyCharVector_Allocate(struct Vec *a, u32 b, struct Alloc unused);
u32 LobbyCharVector_RecommendSize(struct Vec *a, u32 n);
struct Str *LobbyString_Destroy(struct Str *a);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);

extern void LobbyEventDequeMap_Destroy(void *), PPWLobbyPlayerSet_DestroyNodes(void *, void *);
extern struct TreeNode *LobbyTimerMap_FindInsertPos(struct Tree *, u32 *, struct TreeNode **, u8 *, u8 *);
extern struct TreeNode *LobbyTimerMap_InsertNode(struct Tree *, struct TreeNode *, int, int, u32 *);
extern void LobbyString_ReplaceSubstring(struct Str *, u32, u32, struct Str *, u32, u32);
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *PPWLobbyWebRequest_Destroy(u8 *a);
void *PPWLobbyPlayers_Delete(u8 *a);
void *PPWLobbyPlayer_Delete(u8 *a);
void *PPWLobbyPlayerSet_Destroy(struct Tree *a);
void LobbyTimerMap_Clear(struct Tree *a);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void LobbyStringVector_PushBackSlow(struct Vec *a, void *b, struct Alloc unused);
struct Vec *LobbyStringVector_Destroy(struct Vec *a);
struct Vec *LobbyU32Vector_Destroy(struct Vec *a);
struct Vec *LobbyVipVector_Destroy(struct Vec *a);
struct Vec *LobbyEventVector_Destroy(struct Vec *a);
void LobbyCharVector_Allocate(struct Vec *a, u32 b, struct Alloc unused);
u32 LobbyCharVector_RecommendSize(struct Vec *a, u32 n);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
struct Str *LobbyString_Destroy(struct Str *a);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);
void *LobbyEventDeque_Destroy(u32 *a);
void LobbyTimerMap_Insert(u32 *result, struct Tree *a, u32 *value);
void LobbyTimerMap_Erase(Iterator *result, struct Tree *a, Iterator node);
void LobbyCharVector_Grow(struct Vec *a, u32 n);
void LobbyCharVector_Reallocate(struct Vec *a, u32 unused, u32 n);
struct Str *LobbyString_InitCopy(struct Str *a, struct Str *b);
struct Str *LobbyString_InitSubstring(struct Str *a, struct Str *b, u32 offset, u32 length);
struct Str *LobbyString_InitFromCStr(struct Str *a, char *b);
extern char sStr_BasicStringReserveLengthError[], sStr_TreeInsertLengthError[];
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *PPWLobbyWebRequest_Destroy(u8 *a);
void *PPWLobbyPlayers_Delete(u8 *a);
void *PPWLobbyPlayer_Delete(u8 *a);
void *LobbyEventDeque_Destroy(u32 *a);
void *PPWLobbyPlayerSet_Destroy(struct Tree *a);
void LobbyTimerMap_Insert(u32 *result, struct Tree *a, u32 *value);
void LobbyTimerMap_Erase(Iterator *result, struct Tree *a, Iterator node);
void LobbyTimerMap_Clear(struct Tree *a);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void LobbyStringVector_PushBackSlow(struct Vec *a, void *b, struct Alloc unused);
struct Vec *LobbyStringVector_Destroy(struct Vec *a);
struct Vec *LobbyU32Vector_Destroy(struct Vec *a);
struct Vec *LobbyVipVector_Destroy(struct Vec *a);
struct Vec *LobbyEventVector_Destroy(struct Vec *a);
void LobbyCharVector_Grow(struct Vec *a, u32 n);
void LobbyCharVector_Allocate(struct Vec *a, u32 b, struct Alloc unused);
void LobbyCharVector_Reallocate(struct Vec *a, u32 unused, u32 n);
u32 LobbyCharVector_RecommendSize(struct Vec *a, u32 n);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
struct Str *LobbyString_InitCopy(struct Str *a, struct Str *b);
struct Str *LobbyString_InitSubstring(struct Str *a, struct Str *b, u32 offset, u32 length);
struct Str *LobbyString_InitFromCStr(struct Str *a, char *b);
struct Str *LobbyString_Destroy(struct Str *a);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);
void LobbyString_Reserve(struct Str *a, u32 requested);
struct TreeNode *LobbyTimerMap_InsertNode(struct Tree *a, struct TreeNode *parent, int left, int end, u32 *value);
extern int PPWLobby_DecodeMessageData(void *, struct Vec *);
extern void *LobbyByteVector_Data(struct Vec *);
extern int PPWLobbySchedule_Load(void *, void *, u32);
extern void PPWLobbySchedule_CopyToVector(struct Vec *, void *), PPWLobby_SetChannelData(int, int, char *, void *, u32);
extern char sStr_b_lib_c_lobby[];
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *PPWLobbyWebRequest_Destroy(u8 *a);
void *PPWLobbyPlayers_Delete(u8 *a);
void *PPWLobbyPlayer_Delete(u8 *a);
void *LobbyEventDeque_Destroy(u32 *a);
void *PPWLobbyPlayerSet_Destroy(struct Tree *a);
void LobbyTimerMap_Insert(u32 *result, struct Tree *a, u32 *value);
void LobbyTimerMap_Erase(Iterator *result, struct Tree *a, Iterator node);
void LobbyTimerMap_Clear(struct Tree *a);
struct TreeNode *LobbyTimerMap_InsertNode(struct Tree *a, struct TreeNode *parent, int left, int end, u32 *value);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void LobbyStringVector_PushBackSlow(struct Vec *a, void *b, struct Alloc unused);
struct Vec *LobbyStringVector_Destroy(struct Vec *a);
struct Vec *LobbyU32Vector_Destroy(struct Vec *a);
struct Vec *LobbyVipVector_Destroy(struct Vec *a);
struct Vec *LobbyEventVector_Destroy(struct Vec *a);
void LobbyCharVector_Grow(struct Vec *a, u32 n);
void LobbyCharVector_Allocate(struct Vec *a, u32 b, struct Alloc unused);
void LobbyCharVector_Reallocate(struct Vec *a, u32 unused, u32 n);
u32 LobbyCharVector_RecommendSize(struct Vec *a, u32 n);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
struct Str *LobbyString_InitCopy(struct Str *a, struct Str *b);
struct Str *LobbyString_InitSubstring(struct Str *a, struct Str *b, u32 offset, u32 length);
struct Str *LobbyString_InitFromCStr(struct Str *a, char *b);
struct Str *LobbyString_Destroy(struct Str *a);
void LobbyString_Reserve(struct Str *a, u32 requested);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);
void PPWLobby_OnEnumUsers(char *name, int id, int n, int *list);
void PPWLobby_OnGetLobbyData(int ok, int unused1, int unused2, int unused3, void *value);

void LobbyTree_RotateLeft(struct TreeNode *a, void **root) {
    struct TreeNode *b = (struct TreeNode *)a->right, *c, *parent;
    if (*root == a) {
        *root = b;
    }
    a->right = b->left;
    c = (struct TreeNode *)b->left;
    if (c) {
        c->parent = (u32)a | (c->parent & 1);
    }
    b->parent = (a->parent & ~1) | (b->parent & 1);
    parent = (struct TreeNode *)(a->parent & ~1);
    if (a == parent->left) {
        parent->left = b;
    } else {
        parent->right = b;
    }
    b->left = a;
    a->parent = (u32)b | (a->parent & 1);
}

void LobbyTree_RotateRight(struct TreeNode *a, void **root) {
    struct TreeNode *b = (struct TreeNode *)a->left, *c, *parent;
    if (*root == a) {
        *root = b;
    }
    a->left = b->right;
    c = (struct TreeNode *)b->right;
    if (c) {
        c->parent = (u32)a | (c->parent & 1);
    }
    b->parent = (a->parent & ~1) | (b->parent & 1);
    parent = (struct TreeNode *)(a->parent & ~1);
    if (a == parent->left) {
        parent->left = b;
    } else {
        parent->right = b;
    }
    b->right = a;
    a->parent = (u32)b | (a->parent & 1);
}

void LobbyStringVector_PushBackSlow(struct Vec *a, void *b, struct Alloc unused) {
    struct Alloc alloc = { 0 };
    LobbyStringVector_RecommendSize(a, 1, 0);
    LobbyStringVector_AppendCopiesSlow(a, 1, b);
}
}
}

namespace StringVectorDestroy {
extern "C" {

typedef struct Tag {
    u8 value;
} Tag;

void LobbyStringVector_EraseAtEnd(u32 *, u32, Tag, u32);
void DWC_Free(u32, u32, u32);
u32 *LobbyStringVector_Destroy(u32 *);

static inline Tag tag(void) {
    Tag t;
    t.value = 0;
    return t;
}

u32 *LobbyStringVector_Destroy(u32 *p) {
    if (p[0]) {
        u32 current = p[1];
        Tag t = { 0 };
        LobbyStringVector_EraseAtEnd(p, current, t, 0);
        if (p[0]) {
            DWC_Free(0, p[0], 0);
        }
    }
    return p;
}
}
}

namespace VectorDestroy {
extern "C" {
void ov45_02236E58(Iterator *);
extern int printf(const char *, ...);
extern void abort(void);

struct Str {
    union {
        u32 flags;

        struct {
            u32 large : 1;
            u32 cap : 31;
        } bits;
    } u;

    u32 size;
    char *ptr;
};

struct Vec {
    void *ptr;
    u32 size, cap;
};

struct TreeNode {
    void *left, *right;
    u32 parent;
    u32 key;
    u32 data[6];
};

struct Tree {
    u32 count;
    void *root;
    u32 unk;
    void *end;
};

struct Alloc {
    u8 tag;
};

extern u8 gPPWLobbyPlayersVtable[], sStr_b_lib_u_user[], sStr_b_lib_u_system[], gPPWLobbyMainChannelMode[];
extern void DWC_Free(int, void *, int);
extern void *LobbyMem_Alloc(u32);
extern struct Str *LobbyString_Destroy(struct Str *);
extern struct Vec *LobbyCharVector_Destroy(struct Vec *);
extern struct Str *LobbyString_InitSubstring(struct Str *, struct Str *, u32, u32);
extern void PPWLobby_OnMessage(int, int, int, int);
extern void PPWLobby_HandleReceivedData(int, int, int, int, int, int, int);
extern void PPWLobbyPlayers_Clear(void *), LobbyEventDequeMap_Destroy(void *), PPWLobbyPlayerSet_DestroyNodes(void *, void *);
extern void LobbyStringVector_RecommendSize(struct Vec *, u32, int), LobbyStringVector_AppendCopiesSlow(struct Vec *, u32, void *), LobbyStringVector_EraseAtEnd(struct Vec *, u32, struct Alloc), LobbyCharVector_AppendZerosSlow(struct Vec *, u32);
extern void LobbyCharVector_Allocate(struct Vec *, u32, struct Alloc);
extern void LobbyTree_InsertRebalance(struct TreeNode *, void *), LobbyTimerMap_DestroyNodes(struct Tree *, struct TreeNode *);
extern struct TreeNode *LobbyTree_EraseRebalance(struct TreeNode *, void *);
extern void LobbyTimerMap_EraseKey(void *, int *), LobbyChat_SetChannelMode(int, char *);
extern void PPWLobbyPlayers_Add(void *, int), LobbyChat_GetChannelKeysAsync(int, int, char *, void (*)(), int);
extern void PPWLobbyChannel_GetName(struct Str *, void *);
extern struct Str *LobbyString_InitFromCStr(struct Str *, char *);
extern int LobbyString_Equals(struct Str *, struct Str *);
extern void LobbyString_Reserve(struct Str *, u32);
extern u32 LobbyCharVector_RecommendSize(struct Vec *, u32);
extern struct Str *LobbyString_Replace(struct Str *, u32, u32, struct Str *);
extern struct Str *LobbyString_ReplaceRange(struct Str *, u32, u32, char *, char *, struct Alloc);
extern void _ZNSt15__vector_commonILb1EE18throw_length_errorEv(void);
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *PPWLobbyWebRequest_Destroy(u8 *a);
void *PPWLobbyPlayers_Delete(u8 *a);
void *PPWLobbyPlayer_Delete(u8 *a);
void *PPWLobbyPlayerSet_Destroy(struct Tree *a);
void LobbyTimerMap_Clear(struct Tree *a);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void LobbyStringVector_PushBackSlow(struct Vec *a, void *b, struct Alloc unused);
struct Vec *LobbyStringVector_Destroy(struct Vec *a);
struct Vec *LobbyU32Vector_Destroy(struct Vec *a);
struct Vec *LobbyVipVector_Destroy(struct Vec *a);
struct Vec *LobbyEventVector_Destroy(struct Vec *a);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
void LobbyCharVector_Allocate(struct Vec *a, u32 b, struct Alloc unused);
u32 LobbyCharVector_RecommendSize(struct Vec *a, u32 n);
struct Str *LobbyString_Destroy(struct Str *a);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);

extern void LobbyEventDequeMap_Destroy(void *), PPWLobbyPlayerSet_DestroyNodes(void *, void *);
extern struct TreeNode *LobbyTimerMap_FindInsertPos(struct Tree *, u32 *, struct TreeNode **, u8 *, u8 *);
extern struct TreeNode *LobbyTimerMap_InsertNode(struct Tree *, struct TreeNode *, int, int, u32 *);
extern void LobbyString_ReplaceSubstring(struct Str *, u32, u32, struct Str *, u32, u32);
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *PPWLobbyWebRequest_Destroy(u8 *a);
void *PPWLobbyPlayers_Delete(u8 *a);
void *PPWLobbyPlayer_Delete(u8 *a);
void *PPWLobbyPlayerSet_Destroy(struct Tree *a);
void LobbyTimerMap_Clear(struct Tree *a);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void LobbyStringVector_PushBackSlow(struct Vec *a, void *b, struct Alloc unused);
struct Vec *LobbyStringVector_Destroy(struct Vec *a);
struct Vec *LobbyU32Vector_Destroy(struct Vec *a);
struct Vec *LobbyVipVector_Destroy(struct Vec *a);
struct Vec *LobbyEventVector_Destroy(struct Vec *a);
void LobbyCharVector_Allocate(struct Vec *a, u32 b, struct Alloc unused);
u32 LobbyCharVector_RecommendSize(struct Vec *a, u32 n);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
struct Str *LobbyString_Destroy(struct Str *a);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);
void *LobbyEventDeque_Destroy(u32 *a);
void LobbyTimerMap_Insert(u32 *result, struct Tree *a, u32 *value);
void LobbyTimerMap_Erase(Iterator *result, struct Tree *a, Iterator node);
void LobbyCharVector_Grow(struct Vec *a, u32 n);
void LobbyCharVector_Reallocate(struct Vec *a, u32 unused, u32 n);
struct Str *LobbyString_InitCopy(struct Str *a, struct Str *b);
struct Str *LobbyString_InitSubstring(struct Str *a, struct Str *b, u32 offset, u32 length);
struct Str *LobbyString_InitFromCStr(struct Str *a, char *b);
extern char sStr_BasicStringReserveLengthError[], sStr_TreeInsertLengthError[];
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *PPWLobbyWebRequest_Destroy(u8 *a);
void *PPWLobbyPlayers_Delete(u8 *a);
void *PPWLobbyPlayer_Delete(u8 *a);
void *LobbyEventDeque_Destroy(u32 *a);
void *PPWLobbyPlayerSet_Destroy(struct Tree *a);
void LobbyTimerMap_Insert(u32 *result, struct Tree *a, u32 *value);
void LobbyTimerMap_Erase(Iterator *result, struct Tree *a, Iterator node);
void LobbyTimerMap_Clear(struct Tree *a);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void LobbyStringVector_PushBackSlow(struct Vec *a, void *b, struct Alloc unused);
struct Vec *LobbyStringVector_Destroy(struct Vec *a);
struct Vec *LobbyU32Vector_Destroy(struct Vec *a);
struct Vec *LobbyVipVector_Destroy(struct Vec *a);
struct Vec *LobbyEventVector_Destroy(struct Vec *a);
void LobbyCharVector_Grow(struct Vec *a, u32 n);
void LobbyCharVector_Allocate(struct Vec *a, u32 b, struct Alloc unused);
void LobbyCharVector_Reallocate(struct Vec *a, u32 unused, u32 n);
u32 LobbyCharVector_RecommendSize(struct Vec *a, u32 n);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
struct Str *LobbyString_InitCopy(struct Str *a, struct Str *b);
struct Str *LobbyString_InitSubstring(struct Str *a, struct Str *b, u32 offset, u32 length);
struct Str *LobbyString_InitFromCStr(struct Str *a, char *b);
struct Str *LobbyString_Destroy(struct Str *a);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);
void LobbyString_Reserve(struct Str *a, u32 requested);
struct TreeNode *LobbyTimerMap_InsertNode(struct Tree *a, struct TreeNode *parent, int left, int end, u32 *value);
extern int PPWLobby_DecodeMessageData(void *, struct Vec *);
extern void *LobbyByteVector_Data(struct Vec *);
extern int PPWLobbySchedule_Load(void *, void *, u32);
extern void PPWLobbySchedule_CopyToVector(struct Vec *, void *), PPWLobby_SetChannelData(int, int, char *, void *, u32);
extern char sStr_b_lib_c_lobby[];
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *PPWLobbyWebRequest_Destroy(u8 *a);
void *PPWLobbyPlayers_Delete(u8 *a);
void *PPWLobbyPlayer_Delete(u8 *a);
void *LobbyEventDeque_Destroy(u32 *a);
void *PPWLobbyPlayerSet_Destroy(struct Tree *a);
void LobbyTimerMap_Insert(u32 *result, struct Tree *a, u32 *value);
void LobbyTimerMap_Erase(Iterator *result, struct Tree *a, Iterator node);
void LobbyTimerMap_Clear(struct Tree *a);
struct TreeNode *LobbyTimerMap_InsertNode(struct Tree *a, struct TreeNode *parent, int left, int end, u32 *value);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void LobbyStringVector_PushBackSlow(struct Vec *a, void *b, struct Alloc unused);
struct Vec *LobbyStringVector_Destroy(struct Vec *a);
struct Vec *LobbyU32Vector_Destroy(struct Vec *a);
struct Vec *LobbyVipVector_Destroy(struct Vec *a);
struct Vec *LobbyEventVector_Destroy(struct Vec *a);
void LobbyCharVector_Grow(struct Vec *a, u32 n);
void LobbyCharVector_Allocate(struct Vec *a, u32 b, struct Alloc unused);
void LobbyCharVector_Reallocate(struct Vec *a, u32 unused, u32 n);
u32 LobbyCharVector_RecommendSize(struct Vec *a, u32 n);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
struct Str *LobbyString_InitCopy(struct Str *a, struct Str *b);
struct Str *LobbyString_InitSubstring(struct Str *a, struct Str *b, u32 offset, u32 length);
struct Str *LobbyString_InitFromCStr(struct Str *a, char *b);
struct Str *LobbyString_Destroy(struct Str *a);
void LobbyString_Reserve(struct Str *a, u32 requested);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);
void PPWLobby_OnEnumUsers(char *name, int id, int n, int *list);
void PPWLobby_OnGetLobbyData(int ok, int unused1, int unused2, int unused3, void *value);

struct Vec *LobbyU32Vector_Destroy(struct Vec *a) {
    if (a->ptr) {
        u32 size = a->size;
        struct Alloc alloc = { 0 };
        a->size -= size;
        if (a->ptr) {
            DWC_Free(0, a->ptr, 0);
        }
    }
    return a;
}

struct Vec *LobbyVipVector_Destroy(struct Vec *a) {
    if (a->ptr) {
        u32 size = a->size;
        struct Alloc alloc = { 0 };
        a->size -= size;
        if (a->ptr) {
            DWC_Free(0, a->ptr, 0);
        }
    }
    return a;
}

struct Vec *LobbyEventVector_Destroy(struct Vec *a) {
    if (a->ptr) {
        u32 size = a->size;
        struct Alloc alloc = { 0 };
        a->size -= size;
        if (a->ptr) {
            DWC_Free(0, a->ptr, 0);
        }
    }
    return a;
}
}
}

namespace CharVectorGrow {
extern "C" void *memset(void *, int, unsigned);

class Alloc {};

class TrueType {};

struct Vec {
    char *ptr;
    u32 size, cap;
};

extern "C" void LobbyCharVector_Allocate(Vec *, u32, Alloc);
extern "C" void LobbyCharVector_Grow(Vec *, u32);

inline void FillN(char *p, u32 n, char c, TrueType) {
    memset(p, c, n);
}

extern "C" void LobbyCharVector_Grow(Vec *v, u32 n) {
    if (n) {
        LobbyCharVector_Allocate(v, n, Alloc());
        FillN(v->ptr + v->size, n, char(), TrueType());
        v->size += n;
    }
}

}

namespace CharVectorAllocate {
extern "C" {
void ov45_02236E58(Iterator *);
extern int printf(const char *, ...);
extern void abort(void);

struct Str {
    union {
        u32 flags;

        struct {
            u32 large : 1;
            u32 cap : 31;
        } bits;
    } u;

    u32 size;
    char *ptr;
};

struct Vec {
    void *ptr;
    u32 size, cap;
};

struct TreeNode {
    void *left, *right;
    u32 parent;
    u32 key;
    u32 data[6];
};

struct Tree {
    u32 count;
    void *root;
    u32 unk;
    void *end;
};

struct Alloc {
    u8 tag;
};

extern u8 gPPWLobbyPlayersVtable[], sStr_b_lib_u_user[], sStr_b_lib_u_system[], gPPWLobbyMainChannelMode[];
extern void DWC_Free(int, void *, int);
extern void *LobbyMem_Alloc(u32);
extern struct Str *LobbyString_Destroy(struct Str *);
extern struct Vec *LobbyCharVector_Destroy(struct Vec *);
extern struct Str *LobbyString_InitSubstring(struct Str *, struct Str *, u32, u32);
extern void PPWLobby_OnMessage(int, int, int, int);
extern void PPWLobby_HandleReceivedData(int, int, int, int, int, int, int);
extern void PPWLobbyPlayers_Clear(void *), LobbyEventDequeMap_Destroy(void *), PPWLobbyPlayerSet_DestroyNodes(void *, void *);
extern void LobbyStringVector_RecommendSize(struct Vec *, u32, int), LobbyStringVector_AppendCopiesSlow(struct Vec *, u32, void *), LobbyStringVector_EraseAtEnd(struct Vec *, u32, struct Alloc), LobbyCharVector_AppendZerosSlow(struct Vec *, u32);
extern void LobbyCharVector_Allocate(struct Vec *, u32, struct Alloc);
extern void LobbyTree_InsertRebalance(struct TreeNode *, void *), LobbyTimerMap_DestroyNodes(struct Tree *, struct TreeNode *);
extern struct TreeNode *LobbyTree_EraseRebalance(struct TreeNode *, void *);
extern void LobbyTimerMap_EraseKey(void *, int *), LobbyChat_SetChannelMode(int, char *);
extern void PPWLobbyPlayers_Add(void *, int), LobbyChat_GetChannelKeysAsync(int, int, char *, void (*)(), int);
extern void PPWLobbyChannel_GetName(struct Str *, void *);
extern struct Str *LobbyString_InitFromCStr(struct Str *, char *);
extern int LobbyString_Equals(struct Str *, struct Str *);
extern void LobbyString_Reserve(struct Str *, u32);
extern u32 LobbyCharVector_RecommendSize(struct Vec *, u32);
extern struct Str *LobbyString_Replace(struct Str *, u32, u32, struct Str *);
extern struct Str *LobbyString_ReplaceRange(struct Str *, u32, u32, char *, char *, struct Alloc);
extern void _ZNSt15__vector_commonILb1EE18throw_length_errorEv(void);
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *PPWLobbyWebRequest_Destroy(u8 *a);
void *PPWLobbyPlayers_Delete(u8 *a);
void *PPWLobbyPlayer_Delete(u8 *a);
void *PPWLobbyPlayerSet_Destroy(struct Tree *a);
void LobbyTimerMap_Clear(struct Tree *a);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void LobbyStringVector_PushBackSlow(struct Vec *a, void *b, struct Alloc unused);
struct Vec *LobbyStringVector_Destroy(struct Vec *a);
struct Vec *LobbyU32Vector_Destroy(struct Vec *a);
struct Vec *LobbyVipVector_Destroy(struct Vec *a);
struct Vec *LobbyEventVector_Destroy(struct Vec *a);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
void LobbyCharVector_Allocate(struct Vec *a, u32 b, struct Alloc unused);
u32 LobbyCharVector_RecommendSize(struct Vec *a, u32 n);
struct Str *LobbyString_Destroy(struct Str *a);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);

extern void LobbyEventDequeMap_Destroy(void *), PPWLobbyPlayerSet_DestroyNodes(void *, void *);
extern struct TreeNode *LobbyTimerMap_FindInsertPos(struct Tree *, u32 *, struct TreeNode **, u8 *, u8 *);
extern struct TreeNode *LobbyTimerMap_InsertNode(struct Tree *, struct TreeNode *, int, int, u32 *);
extern void LobbyString_ReplaceSubstring(struct Str *, u32, u32, struct Str *, u32, u32);
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *PPWLobbyWebRequest_Destroy(u8 *a);
void *PPWLobbyPlayers_Delete(u8 *a);
void *PPWLobbyPlayer_Delete(u8 *a);
void *PPWLobbyPlayerSet_Destroy(struct Tree *a);
void LobbyTimerMap_Clear(struct Tree *a);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void LobbyStringVector_PushBackSlow(struct Vec *a, void *b, struct Alloc unused);
struct Vec *LobbyStringVector_Destroy(struct Vec *a);
struct Vec *LobbyU32Vector_Destroy(struct Vec *a);
struct Vec *LobbyVipVector_Destroy(struct Vec *a);
struct Vec *LobbyEventVector_Destroy(struct Vec *a);
void LobbyCharVector_Allocate(struct Vec *a, u32 b, struct Alloc unused);
u32 LobbyCharVector_RecommendSize(struct Vec *a, u32 n);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
struct Str *LobbyString_Destroy(struct Str *a);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);
void *LobbyEventDeque_Destroy(u32 *a);
void LobbyTimerMap_Insert(u32 *result, struct Tree *a, u32 *value);
void LobbyTimerMap_Erase(Iterator *result, struct Tree *a, Iterator node);
void LobbyCharVector_Grow(struct Vec *a, u32 n);
void LobbyCharVector_Reallocate(struct Vec *a, u32 unused, u32 n);
struct Str *LobbyString_InitCopy(struct Str *a, struct Str *b);
struct Str *LobbyString_InitSubstring(struct Str *a, struct Str *b, u32 offset, u32 length);
struct Str *LobbyString_InitFromCStr(struct Str *a, char *b);
extern char sStr_BasicStringReserveLengthError[], sStr_TreeInsertLengthError[];
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *PPWLobbyWebRequest_Destroy(u8 *a);
void *PPWLobbyPlayers_Delete(u8 *a);
void *PPWLobbyPlayer_Delete(u8 *a);
void *LobbyEventDeque_Destroy(u32 *a);
void *PPWLobbyPlayerSet_Destroy(struct Tree *a);
void LobbyTimerMap_Insert(u32 *result, struct Tree *a, u32 *value);
void LobbyTimerMap_Erase(Iterator *result, struct Tree *a, Iterator node);
void LobbyTimerMap_Clear(struct Tree *a);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void LobbyStringVector_PushBackSlow(struct Vec *a, void *b, struct Alloc unused);
struct Vec *LobbyStringVector_Destroy(struct Vec *a);
struct Vec *LobbyU32Vector_Destroy(struct Vec *a);
struct Vec *LobbyVipVector_Destroy(struct Vec *a);
struct Vec *LobbyEventVector_Destroy(struct Vec *a);
void LobbyCharVector_Grow(struct Vec *a, u32 n);
void LobbyCharVector_Allocate(struct Vec *a, u32 b, struct Alloc unused);
void LobbyCharVector_Reallocate(struct Vec *a, u32 unused, u32 n);
u32 LobbyCharVector_RecommendSize(struct Vec *a, u32 n);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
struct Str *LobbyString_InitCopy(struct Str *a, struct Str *b);
struct Str *LobbyString_InitSubstring(struct Str *a, struct Str *b, u32 offset, u32 length);
struct Str *LobbyString_InitFromCStr(struct Str *a, char *b);
struct Str *LobbyString_Destroy(struct Str *a);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);
void LobbyString_Reserve(struct Str *a, u32 requested);
struct TreeNode *LobbyTimerMap_InsertNode(struct Tree *a, struct TreeNode *parent, int left, int end, u32 *value);
extern int PPWLobby_DecodeMessageData(void *, struct Vec *);
extern void *LobbyByteVector_Data(struct Vec *);
extern int PPWLobbySchedule_Load(void *, void *, u32);
extern void PPWLobbySchedule_CopyToVector(struct Vec *, void *), PPWLobby_SetChannelData(int, int, char *, void *, u32);
extern char sStr_b_lib_c_lobby[];
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *PPWLobbyWebRequest_Destroy(u8 *a);
void *PPWLobbyPlayers_Delete(u8 *a);
void *PPWLobbyPlayer_Delete(u8 *a);
void *LobbyEventDeque_Destroy(u32 *a);
void *PPWLobbyPlayerSet_Destroy(struct Tree *a);
void LobbyTimerMap_Insert(u32 *result, struct Tree *a, u32 *value);
void LobbyTimerMap_Erase(Iterator *result, struct Tree *a, Iterator node);
void LobbyTimerMap_Clear(struct Tree *a);
struct TreeNode *LobbyTimerMap_InsertNode(struct Tree *a, struct TreeNode *parent, int left, int end, u32 *value);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void LobbyStringVector_PushBackSlow(struct Vec *a, void *b, struct Alloc unused);
struct Vec *LobbyStringVector_Destroy(struct Vec *a);
struct Vec *LobbyU32Vector_Destroy(struct Vec *a);
struct Vec *LobbyVipVector_Destroy(struct Vec *a);
struct Vec *LobbyEventVector_Destroy(struct Vec *a);
void LobbyCharVector_Grow(struct Vec *a, u32 n);
void LobbyCharVector_Allocate(struct Vec *a, u32 b, struct Alloc unused);
void LobbyCharVector_Reallocate(struct Vec *a, u32 unused, u32 n);
u32 LobbyCharVector_RecommendSize(struct Vec *a, u32 n);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
struct Str *LobbyString_InitCopy(struct Str *a, struct Str *b);
struct Str *LobbyString_InitSubstring(struct Str *a, struct Str *b, u32 offset, u32 length);
struct Str *LobbyString_InitFromCStr(struct Str *a, char *b);
struct Str *LobbyString_Destroy(struct Str *a);
void LobbyString_Reserve(struct Str *a, u32 requested);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);
void PPWLobby_OnEnumUsers(char *name, int id, int n, int *list);
void PPWLobby_OnGetLobbyData(int ok, int unused1, int unused2, int unused3, void *value);

void LobbyCharVector_Allocate(struct Vec *a, u32 b, struct Alloc unused) {
    a->ptr = LobbyMem_Alloc(b);
    a->cap = b;
}
}
}

namespace CharVectorReallocate {
extern "C" {

typedef struct {
    u8 byte;
} Allocator;

typedef struct {
    void *data;
    volatile u32 size;
    u32 capacity;
} Buffer;

extern void DWC_Free(int, void *, int), LobbyCharVector_Allocate(Buffer *, u32, Allocator);
void LobbyCharVector_Reallocate(Buffer *, Allocator, u32);

void LobbyCharVector_Reallocate(Buffer *b, Allocator incoming, u32 n) {
    u32 size = b->size;
    Allocator old = { 0 };
    volatile Allocator copy;
    (void)incoming;
    (void)old;
    b->size -= size;
    if (b->data) {
        if (b->data) {
            DWC_Free(0, b->data, 0);
        }
        b->data = 0;
        b->capacity = 0;
    }
    Allocator fresh = { 0 };
    copy.byte = 0;
    LobbyCharVector_Allocate(b, n, fresh);
}
}
}

namespace CharVectorRecommendSize {
extern "C" {
void ov45_02236E58(Iterator *);
extern int printf(const char *, ...);
extern void abort(void);

struct TreeNode {
    void *left, *right;
    u32 parent;
    u32 key;
    u32 data[6];
};

struct Tree {
    u32 count;
    void *root;
    u32 unk;
    void *end;
};

struct Alloc {
    u8 tag;
};

extern u8 gPPWLobbyPlayersVtable[], sStr_b_lib_u_user[], sStr_b_lib_u_system[], gPPWLobbyMainChannelMode[];
extern void DWC_Free(int, void *, int);
extern void *LobbyMem_Alloc(u32);
extern Str *LobbyString_Destroy(Str *);
extern Vec *LobbyCharVector_Destroy(Vec *);
extern Str *LobbyString_InitSubstring(Str *, Str *, u32, u32);
extern void PPWLobby_OnMessage(int, int, int, int);
extern void PPWLobby_HandleReceivedData(int, int, int, int, int, int, int);
extern void PPWLobbyPlayers_Clear(void *), LobbyEventDequeMap_Destroy(void *), PPWLobbyPlayerSet_DestroyNodes(void *, void *);
extern void LobbyStringVector_RecommendSize(Vec *, u32, int), LobbyStringVector_AppendCopiesSlow(Vec *, u32, void *), LobbyStringVector_EraseAtEnd(Vec *, u32, struct Alloc), LobbyCharVector_AppendZerosSlow(Vec *, u32);
extern void LobbyCharVector_Allocate(Vec *, u32, struct Alloc);
extern void LobbyTree_InsertRebalance(struct TreeNode *, void *), LobbyTimerMap_DestroyNodes(struct Tree *, struct TreeNode *);
extern struct TreeNode *LobbyTree_EraseRebalance(struct TreeNode *, void *);
extern void LobbyTimerMap_EraseKey(void *, int *), LobbyChat_SetChannelMode(int, char *);
extern void PPWLobbyPlayers_Add(void *, int), LobbyChat_GetChannelKeysAsync(int, int, char *, void (*)(), int);
extern void PPWLobbyChannel_GetName(Str *, void *);
extern Str *LobbyString_InitFromCStr(Str *, char *);
extern int LobbyString_Equals(Str *, Str *);
extern void LobbyString_Reserve(Str *, u32);
extern u32 LobbyCharVector_RecommendSize(Vec *, u32);
extern Str *LobbyString_Replace(Str *, u32, u32, Str *);
extern Str *LobbyString_ReplaceRange(Str *, u32, u32, char *, char *, struct Alloc);
extern void _ZNSt15__vector_commonILb1EE18throw_length_errorEv(void);
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(Str *a, Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *PPWLobbyWebRequest_Destroy(u8 *a);
void *PPWLobbyPlayers_Delete(u8 *a);
void *PPWLobbyPlayer_Delete(u8 *a);
void *PPWLobbyPlayerSet_Destroy(struct Tree *a);
void LobbyTimerMap_Clear(struct Tree *a);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void LobbyStringVector_PushBackSlow(Vec *a, void *b, struct Alloc unused);
Vec *LobbyStringVector_Destroy(Vec *a);
Vec *LobbyU32Vector_Destroy(Vec *a);
Vec *LobbyVipVector_Destroy(Vec *a);
Vec *LobbyEventVector_Destroy(Vec *a);
Vec *LobbyCharVector_Destroy(Vec *a);
void LobbyCharVector_Allocate(Vec *a, u32 b, struct Alloc unused);
u32 LobbyCharVector_RecommendSize(Vec *a, u32 n);
Str *LobbyString_Destroy(Str *a);
Str *LobbyString_Assign(Str *a, Str *b);
Str *LobbyString_Replace(Str *a, u32 b, u32 c, Str *d);

extern void LobbyEventDequeMap_Destroy(void *), PPWLobbyPlayerSet_DestroyNodes(void *, void *);
extern struct TreeNode *LobbyTimerMap_FindInsertPos(struct Tree *, u32 *, struct TreeNode **, u8 *, u8 *);
extern struct TreeNode *LobbyTimerMap_InsertNode(struct Tree *, struct TreeNode *, int, int, u32 *);
extern void LobbyString_ReplaceSubstring(Str *, u32, u32, Str *, u32, u32);
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(Str *a, Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *PPWLobbyWebRequest_Destroy(u8 *a);
void *PPWLobbyPlayers_Delete(u8 *a);
void *PPWLobbyPlayer_Delete(u8 *a);
void *PPWLobbyPlayerSet_Destroy(struct Tree *a);
void LobbyTimerMap_Clear(struct Tree *a);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void LobbyStringVector_PushBackSlow(Vec *a, void *b, struct Alloc unused);
Vec *LobbyStringVector_Destroy(Vec *a);
Vec *LobbyU32Vector_Destroy(Vec *a);
Vec *LobbyVipVector_Destroy(Vec *a);
Vec *LobbyEventVector_Destroy(Vec *a);
void LobbyCharVector_Allocate(Vec *a, u32 b, struct Alloc unused);
u32 LobbyCharVector_RecommendSize(Vec *a, u32 n);
Vec *LobbyCharVector_Destroy(Vec *a);
Str *LobbyString_Destroy(Str *a);
Str *LobbyString_Assign(Str *a, Str *b);
Str *LobbyString_Replace(Str *a, u32 b, u32 c, Str *d);
void *LobbyEventDeque_Destroy(u32 *a);
void LobbyTimerMap_Insert(u32 *result, struct Tree *a, u32 *value);
void LobbyTimerMap_Erase(Iterator *result, struct Tree *a, Iterator node);
void LobbyCharVector_Grow(Vec *a, u32 n);
void LobbyCharVector_Reallocate(Vec *a, u32 unused, u32 n);
Str *LobbyString_InitCopy(Str *a, Str *b);
Str *LobbyString_InitSubstring(Str *a, Str *b, u32 offset, u32 length);
Str *LobbyString_InitFromCStr(Str *a, char *b);
extern char sStr_BasicStringReserveLengthError[], sStr_TreeInsertLengthError[];
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(Str *a, Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *PPWLobbyWebRequest_Destroy(u8 *a);
void *PPWLobbyPlayers_Delete(u8 *a);
void *PPWLobbyPlayer_Delete(u8 *a);
void *LobbyEventDeque_Destroy(u32 *a);
void *PPWLobbyPlayerSet_Destroy(struct Tree *a);
void LobbyTimerMap_Insert(u32 *result, struct Tree *a, u32 *value);
void LobbyTimerMap_Erase(Iterator *result, struct Tree *a, Iterator node);
void LobbyTimerMap_Clear(struct Tree *a);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void LobbyStringVector_PushBackSlow(Vec *a, void *b, struct Alloc unused);
Vec *LobbyStringVector_Destroy(Vec *a);
Vec *LobbyU32Vector_Destroy(Vec *a);
Vec *LobbyVipVector_Destroy(Vec *a);
Vec *LobbyEventVector_Destroy(Vec *a);
void LobbyCharVector_Grow(Vec *a, u32 n);
void LobbyCharVector_Allocate(Vec *a, u32 b, struct Alloc unused);
void LobbyCharVector_Reallocate(Vec *a, u32 unused, u32 n);
u32 LobbyCharVector_RecommendSize(Vec *a, u32 n);
Vec *LobbyCharVector_Destroy(Vec *a);
Str *LobbyString_InitCopy(Str *a, Str *b);
Str *LobbyString_InitSubstring(Str *a, Str *b, u32 offset, u32 length);
Str *LobbyString_InitFromCStr(Str *a, char *b);
Str *LobbyString_Destroy(Str *a);
Str *LobbyString_Assign(Str *a, Str *b);
Str *LobbyString_Replace(Str *a, u32 b, u32 c, Str *d);
void LobbyString_Reserve(Str *a, u32 requested);
struct TreeNode *LobbyTimerMap_InsertNode(struct Tree *a, struct TreeNode *parent, int left, int end, u32 *value);
extern int PPWLobby_DecodeMessageData(void *, Vec *);
extern void *LobbyByteVector_Data(Vec *);
extern int PPWLobbySchedule_Load(void *, void *, u32);
extern void PPWLobbySchedule_CopyToVector(Vec *, void *), PPWLobby_SetChannelData(int, int, char *, void *, u32);
extern char sStr_b_lib_c_lobby[];
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(Str *a, Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *PPWLobbyWebRequest_Destroy(u8 *a);
void *PPWLobbyPlayers_Delete(u8 *a);
void *PPWLobbyPlayer_Delete(u8 *a);
void *LobbyEventDeque_Destroy(u32 *a);
void *PPWLobbyPlayerSet_Destroy(struct Tree *a);
void LobbyTimerMap_Insert(u32 *result, struct Tree *a, u32 *value);
void LobbyTimerMap_Erase(Iterator *result, struct Tree *a, Iterator node);
void LobbyTimerMap_Clear(struct Tree *a);
struct TreeNode *LobbyTimerMap_InsertNode(struct Tree *a, struct TreeNode *parent, int left, int end, u32 *value);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void LobbyStringVector_PushBackSlow(Vec *a, void *b, struct Alloc unused);
Vec *LobbyStringVector_Destroy(Vec *a);
Vec *LobbyU32Vector_Destroy(Vec *a);
Vec *LobbyVipVector_Destroy(Vec *a);
Vec *LobbyEventVector_Destroy(Vec *a);
void LobbyCharVector_Grow(Vec *a, u32 n);
void LobbyCharVector_Allocate(Vec *a, u32 b, struct Alloc unused);
void LobbyCharVector_Reallocate(Vec *a, u32 unused, u32 n);
u32 LobbyCharVector_RecommendSize(Vec *a, u32 n);
Vec *LobbyCharVector_Destroy(Vec *a);
Str *LobbyString_InitCopy(Str *a, Str *b);
Str *LobbyString_InitSubstring(Str *a, Str *b, u32 offset, u32 length);
Str *LobbyString_InitFromCStr(Str *a, char *b);
Str *LobbyString_Destroy(Str *a);
void LobbyString_Reserve(Str *a, u32 requested);
Str *LobbyString_Assign(Str *a, Str *b);
Str *LobbyString_Replace(Str *a, u32 b, u32 c, Str *d);
void PPWLobby_OnEnumUsers(char *name, int id, int n, int *list);
void PPWLobby_OnGetLobbyData(int ok, int unused1, int unused2, int unused3, void *value);

u32 LobbyCharVector_RecommendSize(Vec *a, u32 n) {
    u32 cap = a->cap;
    u32 growth;
    if (n > ~0u - cap) {
        _ZNSt15__vector_commonILb1EE18throw_length_errorEv();
    }
    if (cap < 0x55555555) {
        growth = (cap + 1) * 3 / 5;
        return cap + *(n > growth ? &n : &growth);
    }
    if (cap < 0xaaaaaaaa) {
        u32 g = (cap + 1) / 2;
        return cap + *(n > g ? &n : &g);
    }
    return ~0u;
}
}
}

using std::memset;
using namespace LobbyCharVectorTypes;

extern "C" void LobbyCharVector_AppendZeros(Vec *vector, u32 count, ResizeDispatch) {
    u32 capacity = vector->cap;
    if (count > capacity || vector->size > capacity - count) {
        u32 additional = vector->size + count - capacity;
        Allocator allocator = Allocator();
        allocator.recommend(vector, additional);
    } else {
        memset(vector->ptr + vector->size, 0, count);
        vector->size += count;
        return;
    }
    LobbyCharVector_AppendZerosSlow(vector, count);
}

using LobbyStringTypes::RangeDispatch;
using namespace LobbyCharVectorTypes;
extern "C" void DWC_Free(int, void *, u32);
extern "C" Str *LobbyString_InitCopy(Str *, Str *);

namespace LobbyCharVectorTypes {
struct CharacterAllocator {
    void release(char *storage) {
        if (storage) {
            DWC_Free(0, storage, 0);
        }
    }
};
}

extern "C" Vec *LobbyCharVector_Destroy(Vec *vector) {
    if (vector->ptr) {
        u32 count = vector->size;
        CharacterAllocator allocator = CharacterAllocator();
        vector->size -= count;
        allocator.release(vector->ptr);
    }
    return vector;
}

extern "C" Str *LobbyString_InitCopy(Str *output, Str *input) {
    if (!input->l.flag) {
        *output = *input;
    } else {
        u32 index;
        RangeDispatch dispatch;
        for (index = 0; index < 3; index++) {
            output->words[index] = 0;
        }
        LobbyString_Reserve(output, input->l.size);
        LobbyString_ReplaceRange(output, 0, 0, input->l.data, input->l.data + input->l.size, dispatch);
    }
    return output;
}

namespace StringInit {

class String {
public:
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

        u32 words[3];
    } raw;

    u32 Size() const {
        return !raw.l.flag ? raw.s.size : raw.l.size;
    }
};

extern "C" void LobbyString_Reserve(String *, u32);
extern "C" String *LobbyString_ReplaceSubstring(String *, u32, u32, String *, u32, u32);
extern "C" String *LobbyString_InitSubstring(String *, String *, u32, u32);

template <class T>
inline const T &Min(const T &a, const T &b) {
    return a < b ? a : b;
}

extern "C" String *LobbyString_InitSubstring(String *self, String *str, u32 pos, u32 n) {
    u32 i;
    for (i = 0; i < 3; i++) {
        self->raw.words[i] = 0;
    }
    u32 size = str->Size();
    LobbyString_Reserve(self, pos <= size ? Min(size - pos, n) : 0);
    LobbyString_ReplaceSubstring(self, 0, 0, str, pos, n);
    return self;
}

}

using LobbyStringTypes::RangeDispatch;
using std::strlen;

extern "C" Str *LobbyString_InitFromCStr(Str *output, char *text) {
    u32 index, length;
    RangeDispatch dispatch;
    for (index = 0; index < 3; index++) {
        output->words[index] = 0;
    }
    length = strlen(text);
    LobbyString_Reserve(output, length);
    LobbyString_ReplaceRange(output, 0, 0, text, text + length, dispatch);
    return output;
}

extern "C" void DWC_Free(int, void *, u32);

extern "C" Str *LobbyString_Destroy(Str *output) {
    if (output->l.flag && output->l.data) {
        DWC_Free(0, output->l.data, 0);
    }
    return output;
}

namespace StringReserve {
extern "C" {
extern char sStr_BasicStringReserveLengthError[];
int printf(const char *, ...);
void abort(void);
void *LobbyMem_Alloc(u32);
void DWC_Free(int, void *, int);

typedef union {
    u32 raw;

    struct {
        u32 heap : 1;
        u32 capacity : 31;
    } h;

    struct {
        u8 heap : 1;
        u8 smallSize : 7;
        u8 rest[3];
    } small;
} Header;

typedef union {
    struct {
        Header header;
        u32 length;
        u8 *ptr;
    } heap;

    struct {
        u8 header;
        u8 data[11];
    } inplace;
} Text;

void LobbyString_Reserve(Text *w, u32 size);

void LobbyString_Reserve(Text *w, u32 size) {
    u32 len, capacity;
    u8 *dst, *old;
    int wasHeap, makeHeap;
    if (size > 0x7ffffffe) {
        printf(sStr_BasicStringReserveLengthError);
        abort();
    }
    wasHeap = w->heap.header.h.heap != 0;
    if (wasHeap) {
        capacity = w->heap.header.raw >> 1;
        len = w->heap.length;
    } else {
        capacity = 11;
        len = w->heap.header.small.smallSize;
    }
    if (size < len) {
        size = len;
    }
    if (size + 1 <= 11) {
        size = 11;
    } else {
        size = (size + 16) & ~15;
    }
    if (size == capacity) {
        return;
    }
    if (size == 11) {
        dst = w->inplace.data;
        old = w->heap.ptr;
        makeHeap = 0;
    } else {
        if (size > capacity) {
            dst = (u8 *)LobbyMem_Alloc(size);
        } else {
            dst = (u8 *)LobbyMem_Alloc(size);
            if (!dst) {
                return;
            }
        }
        old = wasHeap ? w->heap.ptr : w->inplace.data;
        makeHeap = 1;
    }
    memmove(dst, old, len);
    dst[len] = 0;
    if (wasHeap && old) {
        DWC_Free(0, old, 0);
    }
    w->heap.header.h.heap = makeHeap;
    if (!makeHeap) {
        w->heap.header.small.smallSize = (u8)len;
    } else {
        w->heap.length = len;
        w->heap.ptr = dst;
        w->heap.header.raw = (w->heap.header.raw & 1) | (size << 1);
    }
}
}
}

namespace StringAssign {
extern "C" {

struct StrRaw {
    u32 w[3];
};

struct Str {
    union {
        u32 flags;

        struct {
            u32 large : 1;
            u32 cap : 31;
        } bits;
    } u;

    u32 size;
    char *ptr;
};
extern struct Str *LobbyString_Replace(struct Str *, u32, u32, struct Str *);
struct Str *LobbyString_Assign(struct Str *, struct Str *);

struct Str *LobbyString_Assign(struct Str *dst, struct Str *src) {
    BOOL isLong = dst->u.bits.large ? TRUE : FALSE;
    if (!isLong && !src->u.bits.large) {
        *(struct StrRaw *)dst = *(struct StrRaw *)src;
        return dst;
    }
    return LobbyString_Replace(dst, 0, isLong ? dst->size : ((u32)((const u8 *)dst)[0] << 24) >> 25, src);
}
}
}

namespace StringReplace {

namespace LobbyStringTypes {
    union Str {
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

        u32 words[3];
    };

    class RangeDispatch {};

    typedef char ActualString12[sizeof(Str) == 12 ? 1 : -1];
    typedef char ActualCarrier1[sizeof(RangeDispatch) == 1 ? 1 : -1];
}

using namespace LobbyStringTypes;
extern "C" Str *LobbyString_ReplaceRange(Str *, u32, u32, char *, char *, RangeDispatch);
extern "C" Str *LobbyString_Replace(Str *, u32, u32, Str *);

extern "C" Str *LobbyString_Replace(Str *out, u32 pos, u32 remove, Str *input) {
    char *data;
    u32 size;
    RangeDispatch dispatch;
    if (input->l.flag) {
        data = input->l.data;
        size = input->l.size;
    } else {
        data = input->s.data;
        size = input->s.size;
    }
    return LobbyString_ReplaceRange(out, pos, remove, data, data + size, dispatch);
}

}

namespace StringReplaceSubstring {
extern "C" int printf(const char *, ...);
extern "C" void abort(void);
extern "C" char sStr_BasicStringOutOfRange[];

class RangeDispatch {};

class String {
public:
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
};

extern "C" String *LobbyString_ReplaceRange(String *, u32, u32, char *, char *, RangeDispatch);
extern "C" String *LobbyString_ReplaceSubstring(String *, u32, u32, String *, u32, u32);

template <class T>
inline const T &Min(const T &a, const T &b) {
    return a < b ? a : b;
}

extern "C" String *LobbyString_ReplaceSubstring(String *self, u32 pos, u32 n1, String *str, u32 pos2, u32 n2) {
    char *data;
    u32 size;
    RangeDispatch dispatch;
    if (str->raw.l.flag) {
        data = str->raw.l.data;
        size = str->raw.l.size;
    } else {
        data = str->raw.s.data;
        size = str->raw.s.size;
    }
    if (pos2 > size) {
        printf(sStr_BasicStringOutOfRange);
        abort();
    }
    data += pos2;
    n2 = Min(size - pos2, n2);
    return LobbyString_ReplaceRange(self, pos, n1, data, data + n2, dispatch);
}

}

inline StringSearchRange StringSearch::GetRange() const {
    if (raw.longForm.flag) {
        return StringSearchRange(raw.longForm.data, raw.longForm.size);
    }
    return StringSearchRange(raw.shortForm.data, raw.shortForm.size);
}

inline int StringSearch::Find(const StringSearch &other, u32 pos) const {
    StringSearchRange needle = other.GetRange();
    return (int)LobbyString_FindChars(this, needle.data, pos, needle.size);
}

extern "C" int LobbyString_Find(const StringSearch *a, const StringSearch *b, u32 pos) {
    return a->Find(*b, pos);
}

extern "C" {

u32 LobbyString_FindChars(const StringSearch *s, const s8 *needle, u32 start, u32 count) {
    u32 len, left;
    const s8 *data, *end, *at, *p, *q;
    if (s->raw.longForm.flag) {
        data = s->raw.longForm.data;
        len = s->raw.longForm.size;
    } else {
        data = s->raw.shortForm.data;
        len = s->raw.shortForm.size;
    }
    if (start > len) {
        goto fail;
    }
    left = len - start;
    end = needle + count;
    at = data + start;
    if (left < count) {
        goto fail;
    }
    do {
        p = at;
        q = needle;
        while (q < end) {
            if (*q != *p) {
                goto next;
            }
            q++;
            p++;
        }
        return at - data;
next:
        left--;
        at++;
    } while (left >= count);
fail:
    return ~0U;
}
}

namespace StringCompare {
extern "C" unsigned strlen(const char *);

class String {
public:
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

    u32 Size() const {
        return !raw.l.flag ? raw.s.size : raw.l.size;
    }

    const char *Data() const {
        return !raw.l.flag ? raw.s.data : raw.l.data;
    }

    int Compare(const String &other) const;
    int Compare(const char *text) const;
};

extern "C" int LobbyString_CompareChars(const String *, u32, u32, const char *, u32);
extern "C" int LobbyString_Compare(const String *, const String *);
extern "C" int LobbyString_CompareCStr(const String *, const char *);

inline int String::Compare(const String &other) const {
    const char *d;
    u32 n;
    if (!other.raw.l.flag) {
        d = other.raw.s.data;
        n = other.raw.s.size;
    } else {
        d = other.raw.l.data;
        n = other.raw.l.size;
    }
    return LobbyString_CompareChars(this, 0, Size(), d, n);
}

inline int String::Compare(const char *text) const {
    u32 n = strlen(text);
    return LobbyString_CompareChars(this, 0, Size(), text, n);
}

extern "C" int LobbyString_Compare(const String *a, const String *b) {
    return a->Compare(*b);
}

extern "C" int LobbyString_CompareCStr(const String *a, const char *b) {
    return a->Compare(b);
}

}

namespace StringCompareChars {
extern "C" {
extern int printf(const char *, ...);
extern void abort(void);
extern const char sStr_StringComparePosGtLen[];
int LobbyString_CompareChars(const Str *, u32, u32, const void *, u32);

static inline const u32 *Min(const u32 *a, const u32 *b) {
    return *a < *b ? a : b;
}

int LobbyString_CompareChars(const Str *s, u32 pos, u32 length, const void *other, u32 otherLength) {
    const char *data;
    u32 size, n, remaining;
    int r;
    if (s->l.flag) {
        data = s->l.data;
        size = s->l.size;
    } else {
        data = s->s.data;
        size = s->s.size;
    }
    if (pos > size) {
        printf(sStr_StringComparePosGtLen);
        abort();
    }
    remaining = size - pos;
    n = *Min(&length, &remaining);
    r = memcmp(data + pos, other, *Min(&otherLength, &n));
    if (r) {
        return r;
    }
    if (n < otherLength) {
        return -1;
    }
    return n != otherLength;
}
}
}

namespace StringReplaceRange {
extern "C" void *memcpy(void *, const void *, unsigned);
extern "C" void *memmove(void *, const void *, unsigned);
extern "C" int printf(const char *, ...);
extern "C" void abort(void);
extern "C" void *LobbyMem_Alloc(u32);
extern "C" void DWC_Free(int, void *, int);
extern "C" const char sStr_BasicStringOutOfRange[], sStr_BasicStringLengthError[];

class Tag {};
struct Str;
extern "C" Str *LobbyString_Destroy(Str *);
extern "C" Str *LobbyString_AssignRange(Str *, const char *, const char *, int);

struct Str {
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

        u32 words[3];
    } raw;

    Str() {
        u32 i;
        u32 *p = raw.words;
        for (i = 0; i < 3; i++) {
            p[i] = 0;
        }
    }

    void Destroy() {
        LobbyString_Destroy(this);
    }

    const char *CStr() const {
        const Str *t = this;
        return !t->raw.l.flag ? t->raw.s.data : t->raw.l.data;
    }
};

inline const u32 &Min(const u32 &a, const u32 &b) {
    return b < a ? b : a;
}

extern "C" Str *LobbyString_ReplaceRange(Str *s, u32 pos, u32 n1, const char *first, const char *last, Tag);

#pragma opt_common_subs off

extern "C" Str *LobbyString_ReplaceRange(Str *s, u32 pos, u32 n1, const char *first, const char *last, Tag) {
    bool isLong = s->raw.l.flag;
    u32 cap;
    char *data;
    u32 size;
    if (!isLong) {
        cap = 11;
        data = s->raw.s.data;
        size = s->raw.s.size;
    } else {
        cap = s->raw.l.capacity;
        size = s->raw.l.size;
        data = s->raw.l.data;
    }
    if (pos > size) {
        printf(sStr_BasicStringOutOfRange);
        abort();
    }
    u32 rem = size - pos;
    u32 n = Min(n1, rem);
    u32 n2 = last - first;
    u32 len;
    if (n2 > 0x7ffffffe || (len = size - n) > 0x7ffffffe - n2) {
        printf(sStr_BasicStringLengthError);
        abort();
    }
    u32 newSize = n2 + len;
    u32 tail = size - (pos + n);
    if (newSize >= cap) {
        u32 newCap = (cap + 15) & ~15;
        while (newCap < newSize + 1) {
            newCap = (newCap * 2 + 15) & ~15;
        }
        char *p = (char *)LobbyMem_Alloc(newCap);
        if (pos) {
            memcpy(p, data, pos);
        }
        char *q = p + pos;
        memmove(q, first, last - first);
        if (tail) {
            memcpy(q + n2, data + pos + n, tail);
        }
        p[newSize] = gLobbyStringTerminator;
        if (isLong) {
            if (data) {
                DWC_Free(0, data, 0);
            }
        } else {
            s->raw.l.flag = 1;
        }
        s->raw.l.data = p;
        s->raw.l.size = newSize;
        s->raw.l.capacity = newCap;
    } else {
        Str tmp;
        if (tail && data + pos + n2 < last && last <= data + size) {
            LobbyString_AssignRange(&tmp, first, last, 0);
            first = tmp.CStr();
        }
        if (tail) {
            memmove(data + pos + n2, data + pos + n, tail);
        }
        memmove(data + pos, first, n2);
        data[newSize] = gLobbyStringInPlaceTerminator;
        if (isLong) {
            s->raw.l.size = newSize;
        } else {
            s->raw.s.size = newSize;
        }
        tmp.Destroy();
    }
    return s;
}

#pragma opt_common_subs reset

}
