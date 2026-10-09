#include <nitro/types.h>
#include <nitro/code32.h>
struct TA {}; struct TB {}; struct TD {};
struct Str;
struct SetNode;
struct SetIt {
    SetNode *n;
    SetIt(SetNode *p) : n(p) {}
};
struct Iter8 { SetNode *n; u32 extra; Iter8(const SetIt &i) : n(i.n) {} };
struct Vec;
extern "C" void ov45_022434C0(Vec *, Iter8, Iter8, TD);
extern "C" Vec *ov45_022376BC(Vec *);
struct VIt {
    int *p;
    VIt(int *q) : p(q) {}
    int &operator*() const { return *p; }
    VIt &operator++() { ++p; return *this; }
};
inline bool operator==(const VIt &a, const VIt &b) { return a.p == b.p; }
inline bool operator!=(const VIt &a, const VIt &b) { return !(a == b); }
struct Vec {
    int *ptr; u32 size, cap;
    Vec() : ptr(0), size(0), cap(0) {}
    void Build(SetIt f, SetIt l) { Init(f, l); }
    void Init(SetIt f, SetIt l) { Assign(f, l, TA(), TB()); }
    void Assign(SetIt f, SetIt l, const TA &ta, const TB &tb) { Assign(Iter8(f), Iter8(l), ta, tb, TD()); }
    void Assign(Iter8 f, Iter8 l, const TA &, const TB &, const TD &td) { ov45_022434C0(this, f, l, td); }
    void Destroy() { ov45_022376BC(this); }
    VIt begin() { return VIt(ptr); }
    VIt end() { return VIt(ptr + size); }
};
struct MapNode { u8 pad[0x1c]; u32 isize; SetNode *hdr; SetNode *first; };
struct MapIt { MapNode *n; };
struct Root;
typedef void (Root::*Method)(int);
struct MemberTable { u32 header[2]; Method method; };
extern "C" const MemberTable ov45_02255358;
struct Root { u8 pad[0x80]; u32 msize; MapNode *mhdr; };
extern "C" void ov45_02242C74(MapIt *, void *, Str *);
extern "C" void ov45_022435A8(void *, Str *);
struct Bound {
    Method m; Root *obj;
    Bound(Method mm, Root *o) : m(mm), obj(o) {}
    void operator()(int v) const { (obj->*m)(v); }
};
template <class It, class F> inline F ForEach(It first, It last, F f) {
    for (; first != last; ++first) f(*first);
    return f;
}
extern "C" int ov45_0224335C(Root *root, Str *key);
#pragma force_active on
// @fn ov45_0224335C
extern "C" int ov45_0224335C(Root *root, Str *key) {
    MapIt it;
    ov45_02242C74(&it, (u8 *)root + 0x80, key);
    if (it.n == (MapNode *)((u8 *)root + 0x84)) {
        return 0;
    }
    Vec values;
    values.Build(SetIt(it.n->first), SetIt((SetNode *)&it.n->hdr));
    ov45_022435A8((u8 *)root + 0x80, key);
    ForEach(values.begin(), values.end(), Bound(ov45_02255358.method, root));
    values.Destroy();
    return 1;
}
