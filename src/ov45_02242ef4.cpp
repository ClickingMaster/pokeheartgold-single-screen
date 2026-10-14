#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
struct String;
extern "C" void LobbyString_InitFromCStr(String *s, const char *t);
extern "C" void LobbyString_Destroy(String *s);
extern "C" int LobbyString_CompareCStr(const String *s, const char *t);

enum Res {
    RES_NO,
    RES_YES
};

struct String {
    u32 w[3];

    String() {
    }

    String(const char *t) {
        LobbyString_InitFromCStr(this, t);
    }

    void destroy() {
        LobbyString_Destroy(this);
    }

    int compare(const char *t) const {
        return LobbyString_CompareCStr(this, t);
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
extern "C" void LobbyU32Vector_Reserve(Vec *v, u32 n);
extern "C" void LobbyU32Vector_PushBackSlow(Vec *v, const int *x, Tag t);

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
            LobbyU32Vector_Reserve(this, n);
        }
    }

    void push_back(const int &x) {
        Alloc a = Alloc();
        if (size < cap) {
            size++;
            a.construct(&ptr[size - 1], x);
        } else {
            LobbyU32Vector_PushBackSlow(this, &x, Tag());
        }
    }
};

struct Map {
    u32 unk0, unk4;
    int id;
};

extern "C" int LobbyChat_GetUserIdFromNick(String *s);
extern "C" void LobbyChatManager_GetNick(String *out, Map *m);
extern "C" int LobbyChat_AddChannelUser(Map *m, const String &name, const String &key);

static inline bool SameId(int id, String *s) {
    return id == LobbyChat_GetUserIdFromNick(s);
}

extern "C" Vec LobbyChat_AddChannelUsers(Map *map, int count, const char **names, const String &key);

extern "C" Vec LobbyChat_AddChannelUsers(Map *map, int count, const char **names, const String &key) {
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
            int id = LobbyChat_GetUserIdFromNick(&s);
            if (id == self) {
                const char *n = names[i];
                LobbyChatManager_GetNick(&name, map);
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
            int id = LobbyChat_AddChannelUser(map, t, key);
            t.destroy();
            if (id != -1) {
                out.push_back(id);
            }
        }
    }
    return out;
}
