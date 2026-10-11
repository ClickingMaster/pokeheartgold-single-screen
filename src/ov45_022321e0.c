#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef union {
    struct {
        u32 flag : 1, rest : 31;
        u8 raw[8];
    };

    char bytes[12];
} String;

typedef struct {
    int state;
    u8 p[0xd0 - 4];
    int mode;
} Work;

extern Work *volatile _022577C4;
extern void ov45_022320B4(String *, void *);
extern void ov45_02237BC8(String *);
extern int ov45_02240A80(char *);

static inline BOOL active(Work *w) {
    return w->mode == 5 ? FALSE : TRUE;
}

int ov45_022321E0(void);
#pragma opt_propagation off

int ov45_022321E0(void) {
    String text;
    Work *w = _022577C4;
    int result;
    if (!w) {
        return 3;
    }
    if (w && w->state == 10) {
        return 3;
    }
    if (w && _022577C4->state != 8) {
        return 3;
    }
    {
        BOOL enabled = active(w);
        if (enabled == 0) {
            return 0;
        }
    }
    ov45_022320B4(&text, &w->mode);
    result = ov45_02240A80(!text.flag ? &text.bytes[1] : *(char **)&text.raw[4]);
    ov45_02237BC8(&text);
    if (result) {
        return 2;
    }
    return 1;
}
