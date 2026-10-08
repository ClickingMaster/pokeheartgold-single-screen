#include <nitro/types.h>
#include <nitro/code32.h>
extern "C" int memcmp(const void *, const void *, unsigned);
struct Str;
extern "C" Str *ov45_02237B58(Str *, char *);
extern "C" Str *ov45_02237BC8(Str *);
struct Str {
    u32 words[3];
    void Destroy() { ov45_02237BC8(this); }
};
struct InfoHead { int id; };
struct Info : InfoHead {
    u64 mid;
    int z;
    Info() {}
    Info(const Info &o) : InfoHead(o) { *this = o; }
    bool operator==(const Info &o) const { return memcmp(this, &o, sizeof(Info)) == 0; }
};
struct Entry {
    Info info;
    int valid;
    Entry(const Info &i) : info(i), valid(1) {}
    void Assign(const Info &i) { info = i; valid = 1; }
};
struct SummaryData {
    u64 mid;
    int id;
};
struct Summary {
    SummaryData d;
    Summary(const Info &i) { d.id = i.id; d.mid = i.mid; }

    Summary() {}
    Summary(const Summary &o) : d(o.d) {}
};
struct Session { u32 unk0; Entry entry; u8 pad18[0x30 - 0x18]; int dirty; u8 pad34[4]; int active; };
struct Channel { int id; };
struct Work {
    u8 pad0[0x10]; void (*left)(int); u8 pad14[0x20 - 0x14]; void (*update)(int, const Summary &);
    u8 pad24[0xb8 - 0x24]; Channel a; u8 padbc[0xd0 - 0xbc]; Channel b; u8 padd4[0xe8 - 0xd4]; u8 sessions[4];
};
extern "C" Work *_022577C4;
extern "C" void ov45_022320B4(Str *, Channel *);
extern "C" int ov45_022340F8(const Str *, const Str *);
extern "C" Session *ov45_0223DEC4(void *, int);
extern "C" void ov45_0223DE38(void *, int);
extern "C" void ov45_02235C40(char *, int);
extern "C" void ov45_0223DD3C(void *, int);
inline int Match(Channel *c, const Str &name) {
    Str t;
    int r;
    ov45_022320B4(&t, c);
    r = ov45_022340F8(&t, &name);
    t.Destroy();
    return r;
}
inline int Find(Work *w, const Str &name) {
    const Str &n = name;
    if (Match(&w->a, n)) return w->a.id;
    if (Match(&w->b, n)) return w->b.id;
    return 5;
}
inline void Refresh2(Session *s, Info &info, Summary &arg, int kind) {
    s->entry.valid = 0;
    info = s->entry.info;
    info.id = kind;
    Entry e(info);
    const Info &ni = e.info;
    if (!(s->entry.info == ni)) s->entry.Assign(ni);
    s->dirty = 1;
    s->entry.valid = 0;
}
inline void MakeSum(Session *s, Summary &arg) {
    Summary sum(s->entry.info);
    arg = sum;
}
#pragma force_active on
// @fn ov45_02235C40
#pragma opt_common_subs off
extern "C" void ov45_02235C40(char *channel, int id) {
    Info info;
    Summary arg;
    int kind;
    {
        Str name;
        ov45_02237B58(&name, channel);
        kind = Find(_022577C4, name);
        name.Destroy();
    }
    if (kind == 5) return;
    if (kind == 0) {
        ov45_0223DD3C(_022577C4->sessions, id);
        return;
    }
    Session *s = ov45_0223DEC4(_022577C4->sessions, id);
    if (s == 0) {
        ov45_0223DD3C(_022577C4->sessions, id);
        s = ov45_0223DEC4(_022577C4->sessions, id);
    }
    Refresh2(s, info, arg, kind);
    MakeSum(s, arg);
    if (s->active) _022577C4->update(id, arg);
}
#pragma opt_common_subs reset
