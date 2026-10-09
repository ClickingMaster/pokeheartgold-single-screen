#include <nitro/types.h>
#include <nitro/code32.h>
extern "C" int memcmp(const void *, const void *, unsigned);
class Str;
extern "C" Str *ov45_02237BC8(Str *);
extern "C" Str *ov45_02237D48(Str *, const Str *);
class Str {
public:
    union {
        u32 words[3];
        struct { u32 allocated : 1, capacity : 31; u32 length; char *pointer; } heap;
        char bytes[12];
    } s;
    Str() { u32 i; u32 *p = s.words; for (i = 0; i < 3; i++) p[i] = 0; }
    void Destroy() { ov45_02237BC8(this); }
    Str &operator=(const Str &o) { ov45_02237D48(this, &o); return *this; }
    const char *text() const { return s.heap.allocated == 0 ? s.bytes + 1 : s.heap.pointer; }
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
struct Peer {
    int kind; Str name; int a; int b;
    BOOL IsValid() const { if (kind == 5) return FALSE; return TRUE; }

    void Reset() { Str empty; kind = 5; name = empty; a = 0; b = 0; empty.Destroy(); }
};
struct Work {
    int state; int sub; u8 pad8[0xd0 - 0x8]; Peer peer; u8 pade8[0xfc - 0xe8]; Entry entry; u8 pad110[0x128 - 0x110]; int dirty;
};
class ErrorMap { public: int Map(int r) const; };
inline int ErrorMap::Map(int r) const {
    int state = 0;
    switch (r) { case 1: state = 2; break; case 2: state = 3; break; }
    return state;
}
extern "C" Work *_022577C4;
extern "C" Str ov45_022320B4(Peer *);
extern "C" int ov45_022412CC(const char *);
extern "C" int ov45_02231EC0(void);
inline Work *GetWork() { return _022577C4; }
inline bool IsError(Work *w) { return w != 0 && w->state == 10; }
inline void Refresh(Work *w, Info &info) {
    Work *ww = w;
    ww->entry.valid = 0;
    info = ww->entry.info;
    info.id = 5;
    Entry e(info);
    const Info &ni = e.info;
    if (!(ww->entry.info == ni)) ww->entry.Assign(ni);
    ww->dirty = 1;
}
#pragma force_active on
// @fn ov45_02231EC0
#pragma opt_common_subs off
extern "C" int ov45_02231EC0(void) {
    Info info;
    Work *w = GetWork();
    if (w == 0) return 4;
    if (IsError(w)) return 7;
    if (w != 0 && GetWork()->state != 8) return 4;
    if (!(w != 0 && w->peer.IsValid())) return 4;
    {
        Str t(ov45_022320B4(&w->peer));
        int parsed;
        int state;
        parsed = ov45_022412CC(t.text());
        w = GetWork();
        state = ErrorMap().Map(parsed);
        if (state != 0) { w->sub = state; w->state = 10; }
        t.Destroy();
    }
    w = GetWork();
    w->peer.Reset();
    w = GetWork();
    Refresh(w, info);
    return 0;
}
#pragma opt_common_subs reset
