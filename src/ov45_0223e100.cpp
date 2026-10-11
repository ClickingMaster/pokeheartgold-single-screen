#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern "C" int memcmp(const void *, const void *, u32);

struct RTCDate {
    u32 year, month, day, week;
};

struct RTCTime {
    u32 hour, minute, second;
};

extern "C" void ov00_021ECB94(RTCDate *, RTCTime *);
extern "C" s64 RTC_ConvertDateTimeToSecond(const RTCDate *, const RTCTime *);

struct Tag {};

struct Alloc {};

struct TA {};

struct TB {};

struct TC {};

struct Iter {
    const u8 *p;
    u32 extra;

    Iter(const u8 *q)
        : p(q) {
    }
};
struct Vec;
extern "C" Vec *ov45_022379D4(Vec *);
extern "C" void ov45_022327B8(Vec *, Iter, Iter, TC);
extern "C" void ov45_022383E0(Vec *, char *, char *, Alloc);
extern "C" void ov45_02232760(Vec *, char *, char *, Tag);
extern "C" const char *ov45_02231C78(const Vec *);
extern "C" char ov45_02254FB0[];

struct Vec {
    char *ptr;
    u32 size, cap;

    Vec() {
    }

    Vec(int)
        : ptr(0)
        , size(0)
        , cap(0) {
    }

    void Assign(char *first, char *last, Alloc a) {
        ov45_022383E0(this, first, last, a);
    }

    Vec(char *first, char *last, Alloc a)
        : ptr(0)
        , size(0)
        , cap(0) {
        ov45_022383E0(this, first, last, a);
    }

    Vec(const u8 *first, const u8 *last)
        : ptr(0)
        , size(0)
        , cap(0) {
        Init(first, last, TA(), TB());
    }

    void Init(const u8 *first, const u8 *last, TA, TB) {
        ov45_022327B8(this, Iter(first), Iter(last), TC());
    }

    void Destroy() {
        ov45_022379D4(this);
    }

    char *begin() const {
        return ptr;
    }

    char *end() const {
        return ptr + size;
    }

    Alloc get_allocator() const {
        Alloc a;
        return a;
    }

    const char *Data() const {
        return ov45_02231C78(this);
    }
};

inline int Compare(const Vec &a, const Vec &b, u32 n) {
    const char *pa = a.Data();
    return memcmp(pa, b.Data(), n);
}

inline bool operator==(const Vec &a, const Vec &b) {
    u32 n = a.size;
    bool eq = false;
    if (n == b.size) {
        if (n == 0 || Compare(a, b, n) == 0) {
            eq = true;
        }
    }
    return eq;
}

inline bool Same(const Vec &a, const Vec &b) {
    bool eq = false;
    u32 n = a.size;
    if (n == b.size) {
        if (n == 0 || Compare(a, b, n) == 0) {
            eq = true;
        }
    }
    return eq;
}

struct Field {
    Vec v;
    int dirty;

    Field() {
    }

    void Init() {
        v.ptr = 0;
        v.size = 0;
        v.cap = 0;
        dirty = 1;
    }

    Field(char *first, char *last, Alloc a)
        : v(0) {
        Field *const fs = this;
        fs->v.Assign(first, last, a);
        fs->dirty = 1;
    }

    Field(int)
        : v(0) {
    }

    void Assign(char *first, char *last, Alloc a) {
        v.Assign(first, last, a);
        dirty = 1;
    }

    Field &operator=(const Field &o) {
        if (this != &o) {
            ov45_02232760(&v, o.v.begin(), o.v.end(), Tag());
        }
        return *this;
    }

    void Set(const Field &o) {
        if (!(v == o.v)) {
            *this = o;
            dirty = 1;
        }
    }

    void SetFrom(const Field &o) {
        if (!Same(v, o.v)) {
            *this = o;
            dirty = 1;
        }
    }
};

struct Info {
    int id;
    u64 time;
    int z;

    Info() {
    }

    void Init(int k) {
        Info *self = this;
        self->id = k;
        self->time = 0;
        self->z = 0;
        RTCTime t;
        RTCDate d;
        ov00_021ECB94(&d, &t);
        self->time = RTC_ConvertDateTimeToSecond(&d, &t);
    }

    Info(int k) {
        Info *self = this;
        self->id = k;
        self->time = 0;
        self->z = 0;
        RTCTime t;
        RTCDate d;
        ov00_021ECB94(&d, &t);
        self->time = RTC_ConvertDateTimeToSecond(&d, &t);
    }

    bool operator==(const Info &o) const {
        return memcmp(this, &o, sizeof(Info)) == 0;
    }
};

struct Entry {
    Info info;
    int valid;

    Entry() {
    }

    Entry(int k)
        : info(k) {
        Entry *e = this;
        e->valid = 1;
    }

    void Init(int k) {
        info.Init(k);
        Entry *e = this;
        e->valid = 1;
    }

    Entry(const Info &i)
        : info(i)
        , valid(1) {
    }

    void Set(const Entry &e) {
        if (!(info == e.info)) {
            info = e.info;
            valid = 1;
        }
    }
};

struct Sub {
    const void *vtbl;
    Entry entry;
    Field field;
    int a28, id, a30, a34, a38;

    Sub(int i)
        : vtbl(ov45_02254FB0) {
        Entry *const pe = &entry;
        Sub *const t = this;
        pe->Init(5);
        t->field.Init();
        t->a28 = 0;
        t->id = i;
        t->a30 = 0;
        t->a34 = 0;
        t->a38 = 0;
    }

    Sub &operator=(const Sub &o) {
        entry.Set(o.entry);
        field.Set(o.field);
        a28 = o.a28;
        id = o.id;
        a30 = o.a30;
        a34 = o.a34;
        a38 = o.a38;
        return *this;
    }

    void Destroy() {
        field.v.Destroy();
    }

    void SetInfo(const Info &i) {
        entry.Set(Entry(i));
    }

    void SetField(const Vec &src) {
        Field f(src.begin(), src.end(), src.get_allocator());
        Field *pv = &f;
        field.SetFrom(*pv);
        f.v.Destroy();
    }

    void SetData(const u8 *data, u32 len) {
        Vec v(data, data + len);
        SetField(v);
        a34 = 1;
        v.Destroy();
    }
};

struct Obj {
    u8 pad[0x10];
    Sub sub;
};

extern "C" void ov45_0223E100(Obj *obj, int id, const u8 *data, u32 len);
#pragma optimization_level 2

extern "C" void ov45_0223E100(Obj *obj, int id, const u8 *data, u32 len) {
    Sub s(id);
    Sub *ps = &s;
    obj->sub = *ps;
    s.Destroy();
    obj->sub.SetInfo(Info(5));
    obj->sub.a30 = 1;
    obj->sub.SetData(data, len);
    obj->sub.a38 = 1;
}
