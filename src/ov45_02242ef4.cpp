#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
struct String;
extern "C" void ov45_02237B58(String *s, const char *t);
extern "C" void ov45_02237BC8(String *s);
extern "C" int ov45_02237FD8(const String *s, const char *t);

enum Res {
    RES_NO,
    RES_YES
};

struct String {
    u32 w[3];

    String() {
    }

    String(const char *t) {
        ov45_02237B58(this, t);
    }

    void destroy() {
        ov45_02237BC8(this);
    }

    int compare(const char *t) const {
        return ov45_02237FD8(this, t);
    }

    Res ne(const char *t) const {
        return compare(t) != 0 ? RES_YES : RES_NO;
    }

    Res ne2(const char *t) const {
        return compare(t) == 0 ? RES_NO : RES_YES;
    }

    bool operator!=(const char *t) const {
        return compare(t) != 0;
    }

    bool operator==(const char *t) const {
        return compare(t) == 0;
    }
};

class Tag {};

struct Alloc {
    void construct(int *p, const int &v) {
        *p = v;
    }
};
struct Vec;
extern "C" void ov45_0223E830(Vec *v, u32 n);
extern "C" void ov45_0223E910(Vec *v, const int *x, Tag t);

struct Vec {
    int *ptr;
    u32 size;
    u32 cap;

    Vec()
        : ptr(0)
        , size(0)
        , cap(0) {
    }

    void reserve(u32 n) {
        if (n > cap) {
            ov45_0223E830(this, n);
        }
    }

    void push_back(const int &x) {
        Alloc a = Alloc();
        if (size < cap) {
            size++;
            a.construct(&ptr[size - 1], x);
        } else {
            ov45_0223E910(this, &x, Tag());
        }
    }
};

struct Map {
    u32 unk0, unk4;
    int id;
};

extern "C" int ov45_02242A2C(String *s);
extern "C" void ov45_0223FC5C(String *out, Map *m);
extern "C" int ov45_02242D30(Map *m, const String &name, const String &key);

static inline bool SameId(int id, String *s) {
    return id == ov45_02242A2C(s);
}

extern "C" Vec ov45_02242EF4(Map *map, int count, const char **names, const String &key);

extern "C" Vec ov45_02242EF4(Map *map, int count, const char **names, const String &key) {
    Vec out;
    int constructed;
    out.reserve(count);
    for (int i = 0; i < count; i++) {
        bool skip;
        {
            String s(names[i]);
            String name;
            constructed = 0;
            int self = map->id;
            skip = false;
            int id = ov45_02242A2C(&s);
            if (id == self) {
                const char *n = names[i];
                ov45_0223FC5C(&name, map);
                constructed = 1;
                if (name.ne(n)) {
                    skip = true;
                }
            }
            if (constructed) {
                name.destroy();
            }
            s.destroy();
        }
        if (!skip) {
            String t(names[i]);
            int id = ov45_02242D30(map, t, key);
            t.destroy();
            if (id != -1) {
                out.push_back(id);
            }
        }
    }
    return out;
}
