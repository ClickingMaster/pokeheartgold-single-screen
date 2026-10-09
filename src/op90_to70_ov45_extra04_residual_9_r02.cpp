#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
#include <size_t.h>
inline void *operator new(std::size_t, void *p) { return p; }
extern "C" void OS_GetMacAddress(u8 *);
struct Str;
extern "C" Str *ov45_02237B58(Str *, const char *);
struct Str {
    union { struct { u32 flag:1; u32 capacity:31; u32 size; char *data; } l; struct { u8 flag:1; u8 size:7; char data[11]; } s; } raw;
    Str(const char *c) { ov45_02237B58(this, c); }
    const char *CStr() const { return !raw.l.flag ? raw.s.data : raw.l.data; }
};
enum ReqMethod { REQ_METHOD_GET, REQ_METHOD_POST };
struct TA {}; struct TB {}; struct TD {};
struct Iter { const u8 *p; u32 extra; Iter(const u8 *q) : p(q) {} };
struct Vec;
extern "C" void ov45_022327B8(Vec *, Iter, Iter, TD);
extern "C" const u8 *ov45_02231C78(const Vec *);
struct Vec {
    u8 *ptr; u32 size, cap;
    Vec() : ptr(0), size(0), cap(0) {}
    void Assign(Iter first, Iter last) { Assign(first, last, TA(), TB()); }
    void Assign(Iter first, Iter last, const TA &, const TB &) { ov45_022327B8(this, first, last, TD()); }
};
struct Info {
    int id; u16 one; u8 a; u8 b; u8 mac[6]; u16 pad; int pad2;
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
struct Out { u32 a, b; u8 rest[0x2d0]; };
struct Op;
struct OpVtbl { void (*f0)(Op *); void (*del)(Op *); };
extern "C" const OpVtbl ov45_02254FC0;
extern "C" void ov45_022354AC(void);
struct Op {
    const OpVtbl *vtbl; int kind; int flag; Str a; Str b; int id; Vec vec; void (*cb)(void); int x38; int x3c;
    Op(int f, const char *n, const char *h, int i, const u8 *first, const u8 *last)
        : vtbl(&ov45_02254FC0), kind(3), flag(f), a(n), b(h), id(i), vec(), cb(ov45_022354AC), x38(0), x3c(0) {
        vec.Assign(Iter(first), Iter(last));
    }
};
extern "C" void *ov45_02242394(u32);
extern "C" int ov45_0223AEDC(void *, void *);
extern "C" void ov45_0225443C(int, const char *);
extern "C" int ov45_02254A04(const char *, int, const u8 *, u32, int);
extern "C" int ov45_022549E0(const char *, int, const u8 *, u32, int);
extern "C" void ov45_022546F8(void);
extern "C" const char *_02254F58[6];
struct Data { u8 pad0[4]; u32 x4; u8 pad8[0x38 - 8]; u8 x38; u8 x39; u8 pad3a[2]; u16 x3c; u8 x3e; u8 pad3f[3]; u8 x42; };
struct Work { int state; u8 pad4[0x4c - 4]; int x4c; int x50; u8 pad54[0x6c - 0x54]; int x6c; u8 pad70[0x110 - 0x70]; Vec v110; u8 pad11c[0x144 - 0x11c]; u8 x144[4]; };
extern "C" Work *_022577C4;
extern "C" Op *ov45_022577D4;
inline const Data *GetData(Work *w) { return (const Data *)ov45_02231C78(&w->v110); }
#define PNEW(o, a, b, c, d, e, f) new (o) Op(a, b, c, d, e, f)
extern "C" int ov45_02232E60(int mode);
// @fn ov45_02232E60
extern "C" int ov45_02232E60(int mode) {
    const char *name;
    const char *host;
    Op *o;
    int x;
    ReqMethod flag;
    Work *w = _022577C4;
    if (w == 0) return 4;
    if (w != 0 && w->state == 10) return 7;
    if (mode < 0 || mode > 2) return 3;
    Out out;
    if (!ov45_0223AEDC(w->x144, &out) || _022577C4->x6c != 0) return 4;
    const Data *d = GetData(_022577C4);
    Packet p = {0};
    {
        Info info;
        const Data *d2 = GetData(_022577C4);
        info.id = _022577C4->x4c;
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
    x = _022577C4->x4c;
    host = _02254F58[1];
    name = _02254F58[3];
    flag = _022577C4->x50 == 0 ? REQ_METHOD_GET : REQ_METHOD_POST;
    if (ov45_022577D4 == 0) {
        o = (Op *)ov45_02242394(sizeof(Op));
        PNEW(o, flag, name, host, x, (const u8 *)&p, (const u8 *)&p + sizeof(p));
        ov45_022577D4 = o;
        ov45_0225443C(o->flag, o->a.CStr());
        int ret;
        if (o->x3c) {
            u32 n = o->vec.size;
            ret = ov45_02254A04(o->b.CStr(), o->id, ov45_02231C78(&o->vec), n, 0);
        } else {
            u32 n = o->vec.size;
            ret = ov45_022549E0(o->b.CStr(), o->id, ov45_02231C78(&o->vec), n, 0);
        }
        int ok;
        if (ret == 0) ok = 1;
        else { ov45_022546F8(); ok = 0; }
        if (!ok) {
            if (ov45_022577D4 != 0) {
                Op *q = ov45_022577D4;
                if (q != 0) q->vtbl->del(q);
                ov45_022577D4 = 0;
            }
        }
    }
    _022577C4->x6c = 1;
    return 0;
}
