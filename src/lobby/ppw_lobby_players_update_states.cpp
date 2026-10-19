#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

struct Triple {
    u32 a, b, c;
};

struct Obj {
    u32 unk0, channel, timeLow, timeHigh, unk10, systemProfileChanged;
    u8 userProfile[4];
    u32 userProfileSize, unk20, userProfileChanged, state, id, unk30, unk34, entered;
};

struct Node {
    Node *a, *b, *c;
    Obj *value;
};
struct It;
extern "C" void LobbyTreeIterator_Advance(It *it);

struct It {
    Node *n;

    It() {
    }

    It(Node *p)
        : n(p) {
    }

    It &operator++() {
        LobbyTreeIterator_Advance(this);
        return *this;
    }

    Obj *&operator*() const {
        return n->value;
    }

    Obj *operator->() const {
        return n->value;
    }

    bool operator==(const It &o) const {
        return n == o.n;
    }

    bool operator!=(const It &o) const {
        return n != o.n;
    }
};

struct Handler {
    void *unk0, *unk4, *unk8;
    void (*onPlayerEnter)(u32, Triple *, void *, u32);
};

struct Tree {
    u32 size;
    Node header;
};

struct Map {
    u32 unk0;
    Tree tree;

    It begin() {
        return It(tree.header.b);
    }

    It end() {
        return It(&tree.header);
    }
};

extern "C" void PPWLobbyPlayer_UpdateState(Obj *obj);
extern "C" Handler *PPWLobby_GetWork(void);
extern "C" void *LobbyByteVector_Data(void *p);

extern "C" void PPWLobbyPlayers_UpdateStates(Map *m);
#pragma opt_propagation off

extern "C" void PPWLobbyPlayers_UpdateStates(Map *m) {
    Obj *o2;
    u32 zero;
    u32 x;
    u32 y;
    It it;
    Node *end;
    it.n = m->tree.header.b;
    end = &m->tree.header;
    if (it.n != end) {
        zero = 0;
        do {
            PPWLobbyPlayer_UpdateState(it.n->value);
            switch (it.n->value->state) {
            case 0:
            case 1:
            case 3:
                break;
            case 2: {
                Obj *obj = it.n->value;
                obj->systemProfileChanged = zero;
                o2 = it.n->value;
                o2->userProfileChanged = zero;
                const Triple &t = (Triple) { .c = obj->channel, .a = obj->timeLow, .b = obj->timeHigh };
                Triple b = t;
                x = o2->userProfileSize;
                y = it.n->value->id;
                Handler *h = PPWLobby_GetWork();
                h->onPlayerEnter(y, &b, LobbyByteVector_Data(o2->userProfile), x);
                it.n->value->entered = 1;
                break;
            }
            }
            LobbyTreeIterator_Advance(&it);
        } while (it.n != end);
    }
}

#pragma opt_propagation reset
