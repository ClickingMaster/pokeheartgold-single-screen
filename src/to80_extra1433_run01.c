#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
int ov45_02231C78(int *);

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

extern Work *volatile _022577C4;

static inline int badMode(Work *w) {
    if (w) {
        if (w->mode == 5) {
            return 0;
        }
        return 1;
    }
    return 0;
}

extern u8 ov45_02254D5C[];
extern u8 ov45_02254D1C[];
extern u8 ov45_02254D3C[];
extern void ov45_02235C40(void);
extern void ov45_02235E4C(void);
extern void ov45_02236090(void);
extern void ov45_022362D0(void);
extern void ov45_02236A44(void);
extern void ov45_02235A94(void);
extern void ov45_02231EB0(String *);
extern void ov45_02233994(String *, int, String *);
extern void ov45_02237BC8(String *);
extern void ov45_02237A24(String *, String *);
extern void ov45_02237D48(String *, String *);
extern int ov45_02241048(char *, int, void *, void *, void (*)(void), int);

static inline void initResult(Result *r, int mode, String *s) {
    r->mode = mode;
    ov45_02237A24(&r->text, s);
    r->a = 0;
    r->b = 0;
}

int ov45_02231C8C(int);

int ov45_02231C78(int *p) {
    if (p[1] == 0) {
        return 0;
    }
    return p[0];
}
