#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
struct String;
struct TreeHdr;
extern "C" void LobbyString_InitCopy(String *dst, const String *src);
extern "C" void LobbyString_Destroy(String *s);
extern "C" void LobbyUserIdSet_Assign(TreeHdr *dst, const TreeHdr *src);
extern "C" void LobbyUserIdSet_Destroy(TreeHdr *t);

struct Blob24 {
    u32 w[6];
};

struct Alloc {};

struct String {
    u32 w[3];

    String(const String &s) {
        LobbyString_InitCopy(this, &s);
    }

    String(const String &s, int) {
        String *const d = this;
        Alloc al = Alloc();
        (void)al;
        LobbyString_InitCopy(d, &s);
    }
};

struct TreeHdr {
    int count;
    void *root;
    void *leftmost;

    void init() {
        TreeHdr *t = this;
        t->count = 0;
        t->leftmost = &t->root;
        t->root = 0;
    }

    void copyFrom(const TreeHdr &s, const Alloc &) {
        TreeHdr *t = this;
        t->count = 0;
        t->leftmost = &t->root;
        t->root = 0;
        LobbyUserIdSet_Assign(t, &s);
    }
};

struct Entry {
    u32 a;
    TreeHdr t;
    u32 x10;
    Blob24 b;

    Entry() {
        a = 0;
        t.init();
    }

    Entry(const Entry &s) {
        Alloc al = Alloc();
        Entry *d;
        const Entry *const p = &s;
        d = this;
        d->a = p->a;
        d->t.copyFrom(p->t, al);
        d->x10 = p->x10;
        d->b = p->b;
    }
};

struct NamedEntry {
    String name;
    Entry e;

    NamedEntry(const NamedEntry &s)
        : name(s.name, 0)
        , e(s.e) {
    }
};

struct InsertResult {
    void *it;
    u8 inserted;
};

struct Obj {
    u8 pad[0x80];
    TreeHdr tree;
};

extern "C" InsertResult LobbyChannelMap_Insert(TreeHdr *t, NamedEntry *e);
extern "C" NamedEntry LobbyChannelMap_MakeEntry(String *name, Entry *src);
extern "C" int LobbyChat_AddChannel(Obj *o, const String *name, const Blob24 *b, u32 x);
#pragma opt_propagation off

extern "C" int LobbyChat_AddChannel(Obj *o, const String *name, const Blob24 *b, u32 x) {
    String a(*name);
    Entry e;
    e.x10 = x;
    e.b = *b;
    NamedEntry ne = LobbyChannelMap_MakeEntry(&a, &e);
    NamedEntry cp(ne);
    u8 ins = LobbyChannelMap_Insert(&o->tree, &cp).inserted;
    LobbyUserIdSet_Destroy(&cp.e.t);
    LobbyString_Destroy(&cp.name);
    LobbyUserIdSet_Destroy(&ne.e.t);
    LobbyString_Destroy(&ne.name);
    LobbyUserIdSet_Destroy(&e.t);
    LobbyString_Destroy(&a);
    return ins;
}

#pragma opt_propagation reset
