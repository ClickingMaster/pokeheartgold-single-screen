#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u8 pad[24];
    void *map;
} Work;

typedef struct {
    void *a;
    void *value;
} Pair;

extern char ov00_0221A500[];
extern char ov00_0221A508[];
extern char ov00_0221A4E4[];
extern char ov00_0221A514[];
extern u16 __msl_digit[];
extern void __msl_assertion_failed(char *, char *, char *, int);
extern Pair *ov00_021FF0C0(void *, char **);
extern int strcmp(const char *, const char *);
extern long atol(const char *);
extern int ov00_022132BC(Work *);
char *ov00_02212D48(Work *, char *, char *);
int ov00_02212DB4(Work *, char *, int);

char *ov00_02212D48(Work *w, char *key, char *fallback) {
    struct {
        char *key;
        u32 extra;
    } local;

    Pair *pair;
    if (!w) {
        __msl_assertion_failed(ov00_0221A500, ov00_0221A508, ov00_0221A4E4, 151);
    }
    if (!w) {
        return 0;
    }
    local.key = key;
    pair = ov00_021FF0C0(w->map, &local.key);
    if (pair) {
        fallback = pair->value;
    }
    return fallback;
}

int ov00_02212DB4(Work *w, char *key, int fallback) {
    char *value;
    int ch, digit;
    if (!strcmp(key, ov00_0221A514)) {
        return ov00_022132BC(w);
    }
    value = ov00_02212D48(w, key, 0);
    if (value) {
        ch = (u8)*value;
        if (ch < 0 || ch >= 128) {
            digit = 0;
        } else {
            digit = __msl_digit[ch] & 8;
        }
        if (digit) {
            goto success;
        }
    }
    return fallback;
success:
    return atol(value);
}
