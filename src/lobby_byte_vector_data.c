#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
int LobbyByteVector_Data(int *);

typedef union {
    struct {
        u32 flag : 1;
        u32 rest : 31;
        u8 raw[8];
    };

    char bytes[12];
} String;

typedef struct {
    int mode;
    String text;
    int a, b;
} Result;

typedef struct {
    int state, error;
    u8 p[0xd0 - 8];
    int mode;
    String text;
    int a, b;
} Work;

static inline int badMode(Work *w) {
    if (w) {
        if (w->mode == 5) {
            return 0;
        }
        return 1;
    }
    return 0;
}

extern void LobbyString_InitCopy(String *, String *);

static inline void initResult(Result *r, int mode, String *s) {
    r->mode = mode;
    LobbyString_InitCopy(&r->text, s);
    r->a = 0;
    r->b = 0;
}

int LobbyByteVector_Data(int *p) {
    if (p[1] == 0) {
        return 0;
    }
    return p[0];
}
