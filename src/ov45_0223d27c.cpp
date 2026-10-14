#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern "C" unsigned long strtoul(const char *, char **, int);
extern "C" long strtol(const char *, char **, int);

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

    const char *c_str() const {
        return !l.flag ? s.data : l.data;
    }

    u32 size() const {
        return !l.flag ? s.size : l.size;
    }
};

struct TA {};

struct TB {};

struct TD {};

struct TP {};

struct TQ {};

struct Iter {
    const char *p;
    u32 extra;

    Iter(const char *q)
        : p(q) {
    }
};
struct CVec;
extern "C" void LobbyCharVector_AssignIterRange(CVec *, Iter, Iter, TD);
extern "C" void LobbyCharVector_PushBackSlow(CVec *, const char *, TQ);

struct CVec {
    char *ptr;
    u32 size, cap;

    void Assign(const char *first, const char *last) {
        Assign(Iter(first), Iter(last));
    }

    void Assign(Iter first, Iter last) {
        Assign(first, last, TA(), TB());
    }

    void Assign(Iter first, Iter last, const TA &, const TB &) {
        LobbyCharVector_AssignIterRange(this, first, last, TD());
    }

    void PushBack(const char &c, const char *p) {
        PushBack(c, p, TP());
    }

    void PushBack(const char &c, const char *p, const TP &) {
        if (size < cap) {
            u32 n = size + 1;
            size = n;
            ptr[n - 1] = c;
        } else {
            LobbyCharVector_PushBackSlow(this, p, TQ());
        }
    }
};

struct SVec {
    Str *ptr;
    u32 size, cap;
};

struct Out {
    u32 x0;
    u32 x4;
    int decode;
    int flag;
    long value;
    u32 x14;
    CVec payload;
};

extern "C" Str *LobbyString_InitFromCStr(Str *, const char *);
extern "C" void LobbyString_Destroy(Str *);
extern "C" void LobbyString_Split(SVec *, const Str *, const Str *, int);
extern "C" void LobbyStringVector_Destroy(SVec *);
extern "C" int LobbyString_CompareNoCase(Str *, Str *, int);
extern "C" int PPWLobby_DecodeMessageData(const char *, CVec *);
extern "C" const char sStr_SpaceSpace[], sStr_Space_2[], sStr_B[], sStr_S[], ov45_022551D9[];
extern "C" char ov45_022551D8[];
extern "C" int PPWLobby_ParseMessage(const Str *, Out *);
#pragma force_active on
#pragma opt_common_subs off

extern "C" int PPWLobby_ParseMessage(const Str *src, Out *out) {
    SVec parts;
    SVec fields;
    Str delim;
    Str delim2;
    Str b;
    Str a;
    Str b2;
    Str a2;
    char *end;
    const Str *p;
    LobbyString_InitFromCStr(&delim, sStr_SpaceSpace);
    LobbyString_Split(&parts, src, &delim, 2);
    LobbyString_Destroy(&delim);
    if (parts.size != 2) {
        LobbyStringVector_Destroy(&parts);
        return 0;
    }
    {
        p = parts.ptr;
        if (p[0].size() > 40) {
            LobbyStringVector_Destroy(&parts);
            return 0;
        }
        if (p[1].size() > 400) {
            LobbyStringVector_Destroy(&parts);
            return 0;
        }
    }
    LobbyString_InitFromCStr(&delim2, sStr_Space_2);
    LobbyString_Split(&fields, &parts.ptr[0], &delim2, 0);
    LobbyString_Destroy(&delim2);
    if (fields.size < 6) {
        LobbyStringVector_Destroy(&fields);
        LobbyStringVector_Destroy(&parts);
        return 0;
    }
    out->x0 = strtoul(fields.ptr[0].c_str(), &end, 16);
    out->x4 = strtoul(fields.ptr[1].c_str(), &end, 16);
    {
        const char *k = sStr_B;
        LobbyString_InitFromCStr(&a, fields.ptr[2].c_str());
        LobbyString_InitFromCStr(&b, k);
        int r = LobbyString_CompareNoCase(&a, &b, 1);
        LobbyString_Destroy(&b);
        LobbyString_Destroy(&a);
        out->decode = r == 0;
    }
    {
        const char *k = sStr_S;
        LobbyString_InitFromCStr(&a2, fields.ptr[3].c_str());
        LobbyString_InitFromCStr(&b2, k);
        int r = LobbyString_CompareNoCase(&a2, &b2, 1);
        LobbyString_Destroy(&b2);
        LobbyString_Destroy(&a2);
        if (r == 0) {
            out->flag = 0;
        } else {
            out->flag = 1;
        }
    }
    out->value = strtol(fields.ptr[4].c_str(), &end, 16);
    LobbyStringVector_Destroy(&fields);
    if (out->decode == 1) {
        if (!PPWLobby_DecodeMessageData(parts.ptr[1].c_str(), &out->payload)) {
            LobbyStringVector_Destroy(&parts);
            return 0;
        }
    } else {
        p = parts.ptr;
        out->payload.Assign(p[1].c_str(), p[1].c_str() + p[1].size());
        out->payload.PushBack(ov45_022551D8[1], ov45_022551D9);
    }
    LobbyStringVector_Destroy(&parts);
    return 1;
}
