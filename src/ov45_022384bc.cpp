#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

class RangeDispatch {
public:
    RangeDispatch() {
    }
};

class String {
public:
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

    u32 Size() const {
        return !raw.l.flag ? raw.s.size : raw.l.size;
    }

    String *Assign(char *first, char *last);
};

extern "C" String *LobbyString_ReplaceRange(String *, u32, u32, char *, char *, RangeDispatch);
extern "C" String *LobbyString_AssignRange(String *, char *, char *);

inline String *String::Assign(char *first, char *last) {
    return LobbyString_ReplaceRange(this, 0, Size(), first, last, RangeDispatch());
}

extern "C" String *LobbyString_AssignRange(String *s, char *first, char *last) {
    return s->Assign(first, last);
}
