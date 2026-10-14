#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

union Str {
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

    u32 words[3];

    u32 Size() const {
        return !l.flag ? s.size : l.size;
    }

    const char *Data() const {
        return !l.flag ? s.data : l.data;
    }
};

extern "C" void LobbyString_FindFirstOfChars(Str *self, const char *data, int pos, u32 n);
extern "C" void LobbyString_FindFirstOf(Str *self, const Str &str, int pos);

extern "C" void LobbyString_FindFirstOf(Str *self, const Str &str, int pos) {
    const char *p;
    u32 n;
    if (!str.l.flag) {
        p = str.s.data;
        n = str.s.size;
    } else {
        p = str.l.data;
        n = str.l.size;
    }
    LobbyString_FindFirstOfChars(self, p, pos, n);
}
