#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

inline void *operator new(unsigned long, void *p) {
    return p;
}

struct Tag {};
struct String;
extern "C" void LobbyString_InitCopy(String *dst, const String *src);
extern "C" void LobbyString_Destroy(String *s);
extern "C" void LobbyString_Assign(String *dst, const String *src);
extern "C" String ov45_02236680(const String *s, u32 pos, u32 n);
extern "C" int ov45_02242254(const String *s, const String &str, int pos);

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

    String(const String &o) {
        LobbyString_InitCopy(this, &o);
    }

    void destroy() {
        LobbyString_Destroy(this);
    }

    String &operator=(const String &o) {
        LobbyString_Assign(this, &o);
        return *this;
    }

    u32 size() const {
        return !raw.l.flag ? raw.s.size : raw.l.size;
    }

    bool empty() const {
        return size() == 0;
    }

    String substr(u32 pos, u32 n) const {
        return ov45_02236680(this, pos, n);
    }

    int find(const String &str, int pos) const {
        return ov45_02242254(this, str, pos);
    }
};
struct Vec;
extern "C" void ov45_02237634(Vec *v, const String *x, Tag t);

struct Alloc {
    void construct(String *p, const String &x) {
        new (p) String(x);
    }
};

struct Vec {
    String *ptr;
    u32 size;
    u32 cap;

    Vec()
        : ptr(0)
        , size(0)
        , cap(0) {
    }

    Alloc alloc() const {
        return Alloc();
    }

    void push_back(const String &x) {
        Alloc a = alloc();
        if (size < cap) {
            a.construct(ptr + size, x);
            ++size;
        } else {
            ov45_02237634(this, &x, Tag());
        }
    }
};

extern "C" Vec ov45_0223F000(const String &src, const String &sep, int max);

extern "C" Vec ov45_0223F000(const String &src, const String &sep, int max) {
    Vec out;
    int count = 1;
    String cur(src);
    int pos;
    while (count != max && (pos = cur.find(sep, 0)) != -1) {
        if (pos != 0) {
            String piece = ov45_02236680(&cur, 0, pos);
            out.push_back(piece);
            piece.destroy();
            count++;
        }
        String rest = ov45_02236680(&cur, pos + 1, -1);
        cur = rest;
        rest.destroy();
    }
    if (!cur.empty()) {
        out.push_back(cur);
    }
    cur.destroy();
    return out;
}
