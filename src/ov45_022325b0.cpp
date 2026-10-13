#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern "C" int memcmp(const void *, const void *, u32);

struct Tag {};

struct Alloc {
    Alloc() {
    }
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
extern "C" Vec *ov45_022379D4(Vec *v);
extern "C" void ov45_022327B8(Vec *v, Iter first, Iter last, TD);
extern "C" void ov45_022383E0(Vec *v, char *first, char *last, Alloc);
extern "C" void ov45_02232760(Vec *v, char *first, char *last, Tag);
extern "C" const char *ov45_02231C78(const Vec *v);

struct Vec {
    char *ptr;
    u32 size, cap;

    void Assign(Iter first, Iter last) {
        Assign(first, last, TA(), TB());
    }

    void Assign(Iter first, Iter last, const TA &, const TB &) {
        ov45_022327B8(this, first, last, TD());
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

    const char *Data() const {
        return ov45_02231C78(this);
    }
};

// Storage with an out-of-line destructor (ov45_022379D4).
struct VecStorage : Vec {
    ~VecStorage();
};

// Byte buffer built from a [first,last) range; destroyed by ov45_022379D4.
struct Bytes {
    VecStorage v;

    Bytes(const u8 *first, const u8 *last) {
        v.ptr = 0;
        v.size = 0;
        v.cap = 0;
        v.Assign(Iter(first), Iter(last));
    }

    ~Bytes();

    char *begin() const {
        return v.ptr;
    }

    char *end() const {
        return v.ptr + v.size;
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

struct Field {
    Vec v;
    int dirty;

    Field() {
    }

    Field &Init(char *b, char *e, Alloc a) {
        v.ptr = 0;
        v.size = 0;
        v.cap = 0;
        Field *self = (Field *)(void *)this;
        ov45_022383E0(&self->v, b, e, a);
        self->dirty = 1;
        return *self;
    }
};

struct Work {
    int state;
    int sub;
    u8 pad[0x110 - 8];
    Field field;
    u8 pad2[0x12c - 0x120];
    int changed;
};

extern "C" Work *_022577C4;
extern "C" int PPW_LobbyUpdateMyProfile(const u8 *data, u32 len, int flag);

extern "C" int PPW_LobbyUpdateMyProfile(const u8 *data, u32 len, int flag) {
    Work *w = _022577C4;
    if (w == 0) {
        return 4;
    }
    if (w != 0 && w->state == 10) {
        return 7;
    }
    if (w != 0 && (*(Work *volatile *)&_022577C4)->state != 8) {
        return 4;
    }
    Field f;
    Bytes raw(data, data + len);
    f.Init(raw.begin(), raw.end(), Alloc());
    const Field &nf = f;
    if (flag) {
        w->field.dirty = 1;
    }
    if (!(w->field.v == nf.v)) {
        if (&w->field != &nf) {
            ov45_02232760(&w->field.v, nf.v.begin(), nf.v.end(), Tag());
        }
        w->field.dirty = 1;
    }
    f.v.Destroy();
    w->changed = 1;
    return 0;
}
