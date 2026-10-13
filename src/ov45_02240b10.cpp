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
extern "C" void ov45_022429E0(String *s, void *user);
extern "C" void ov45_02233ABC(String *out, const String *a, const char *b);
struct Root;
extern "C" void ov45_02240F6C(String *, Root *);
extern "C" void ov45_0223FC5C(String *, Root *);
extern "C" void ov45_02240F7C(String *, Root *);
extern "C" void ov45_02240F8C(String *, Root *);

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
extern "C" void ov45_02236BA8(InsertResult *out, Map *m, const Pair<u32, Task> *v);

struct Map {
    u32 words[4];

    InsertResult insert(const Pair<u32, Task> &v) {
        InsertResult r;
        ov45_02236BA8(&r, this, &v);
        return r;
    }
};

struct Root {
    u32 connection, state;
    u8 pad[0x94];
    Map queue;
};

extern "C" Pair<int, Task> ov45_02232AE8(int k, const Task &t);

struct Callbacks {
    void (*f[4])(void);
    void *data;
};

extern Root *ov45_022577DC;
extern "C" const u8 _version_NINTENDO_DWC_LOBBY[];
extern "C" void OSi_ReferSymbol(const void *);
extern "C" const char ov45_02255308[], ov45_02255310[], ov45_02255318[], ov45_0225531C[];
extern "C" void *LobbyMem_Alloc(u32);
extern "C" Root *ov45_0224268C(Root *, String *, String *, int, void *, String *, String *, String *, void *);
extern "C" int ov45_0224246C(void);
extern "C" void ov45_0223ED00(void), ov45_0223ED1C(void), ov45_0223F1C8(void), ov45_0223EDD8(void), ov45_0223FA3C(void), ov45_0223F9C4(void), ov45_0223F92C(void);
extern "C" int ov45_02240850(void);
extern "C" u32 chatConnectSpecialA(const char *, int, const char *, const char *, const char *, const char *, Callbacks *, void (*)(void), void (*)(void), void (*)(void), void *, int);
extern "C" void DWC_GetDateTime(RTCDate *, RTCTime *);
extern "C" void ov45_022424DC(void), ov45_02242858(Root *), DWC_Free(int, void *, int);

inline void Free(void *p) {
    if (p) {
        DWC_Free(0, p, 0);
    }
}

inline void DeleteRoot(Root *r) {
    if (r) {
        ov45_02242858(r);
        Free(r);
    }
}

extern "C" void MI_CpuFill8(void *, u8, u32);
extern "C" int ov45_02240B10(const char *a, const char *b, int n, u8 *config, void *user);
#pragma force_active on

extern "C" int ov45_02240B10(const char *a, const char *b, int n, u8 *config, void *user) {
    String outer1, outer2, outer3;
    Callbacks callbacks;
    String name2, name1;
    String s7, s6, s5, s4, s3, s2, s1;
    RTCDate date;
    RTCTime time;
    Root *root;
    int error;
    OSi_ReferSymbol(_version_NINTENDO_DWC_LOBBY);
    if (ov45_022577DC) {
        return 4;
    }
    ov45_022429E0(&outer1, user);
    LobbyString_InitFromCStr(&outer2, ov45_02255308);
    LobbyString_InitFromCStr(&outer3, ov45_02255310);
    {
        int firstReady = 0, secondReady = firstReady;
        Root *created = (Root *)LobbyMem_Alloc(0xac);
        if (created) {
            LobbyString_InitFromCStr(&name2, b);
            firstReady = 1;
            LobbyString_InitFromCStr(&name1, a);
            secondReady = firstReady;
            created = ov45_0224268C(created, &name1, &name2, n, user, &outer1, &outer2, &outer3, config);
        }
        ov45_022577DC = created;
        if (secondReady) {
            name1.destroy();
        }
        if (firstReady) {
            name2.destroy();
        }
    }
    if (!ov45_022577DC) {
        outer3.destroy();
        outer2.destroy();
        outer1.destroy();
        return 1;
    }
    if (!ov45_0224246C()) {
        error = 1;
        goto failure;
    }
    {
        MI_CpuFill8(&callbacks, 0, 20);
        callbacks.f[0] = ov45_0223ED00;
        callbacks.f[1] = ov45_0223ED1C;
        callbacks.f[2] = ov45_0223EDD8;
        callbacks.f[3] = ov45_0223F1C8;
        callbacks.data = *(void **)(config + 8);
        ov45_02240F6C(&s7, ov45_022577DC);
        ov45_02233ABC(&s6, &s7, ov45_02255318);
        ov45_02233ABC(&s5, &s6, ov45_0225531C);
        ov45_0223FC5C(&s4, ov45_022577DC);
        ov45_02240F7C(&s3, ov45_022577DC);
        ov45_02240F6C(&s2, ov45_022577DC);
        ov45_02240F8C(&s1, ov45_022577DC);
        u32 connection = chatConnectSpecialA(s5.c_str(), 0x1a0b, s4.c_str(), s3.c_str(), s2.c_str(), s1.c_str(), &callbacks, ov45_0223FA3C, ov45_0223F9C4, ov45_0223F92C, *(void **)(config + 8), 0);
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
        ov45_022577DC->connection = connection;
        if (ov45_022577DC->state != 5) {
            ov45_022577DC->state = 1;
        }
    }
    {
        root = ov45_022577DC;
        DWC_GetDateTime(&date, &time);
        root->queue.insert(ov45_02232AE8(0, Task(RTC_ConvertDateTimeToSecond(&date, &time), root, ov45_02240850)));
    }
    outer3.destroy();
    outer2.destroy();
    outer1.destroy();
    return 0;
failure:
    ov45_022424DC();
    root = ov45_022577DC;
    if (root) {
        DeleteRoot(root);
        ov45_022577DC = 0;
    }
    outer3.destroy();
    outer2.destroy();
    outer1.destroy();
    return error;
}
