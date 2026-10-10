#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
#include <string.h>

typedef struct {
    int phase;
    void *queue;
    int status;
    char first[36], second[36], third[36];
    u8 pad78[4];
    u32 f7c, f80;
    u8 pad84[0x480 - 0x84];
    int f480, f484;
    void *callback;
    u32 f48c;
    u8 pad490[4];
    void *argument;
    char *message;
    u8 pad49c[4];
    u32 f4a0, f4a4, f4a8, messageId;
    int attempts;
    u8 pad4b4[4];
    int value;
    u8 pad4bc[0x5cc - 0x4bc];
    u32 f5cc, owned;
    u8 trailing[0x5ec - 0x5d4];
} State;

extern char ov00_0221A5D8[];
extern char ov00_0221A5C8[];
extern char ov00_0221A560[];
extern char ov00_0221A5E8[];
extern char ov00_0221A5B8[];
extern int ov00_0221B0D0;
extern void __msl_assertion_failed(const char *, const char *, const char *, int);
extern void ov00_02213EC0(void *);
extern void ov00_0221505C(void *, u32);
extern void ov00_021FF3CC(void);
extern void srand(unsigned);
extern void *ov00_02212C08(void *unusedState);
extern u32 ov00_021FF368(void);
void ov00_02214004(void *, const char *, const char *, const char *, int, int, void *, void *);

void ov00_02214004(void *parent, const char *a, const char *b, const char *c, int value, int force, void *callback, void *arg) {
    State *s = parent;
    if (!s) {
        __msl_assertion_failed(ov00_0221A5D8, ov00_0221A5C8, ov00_0221A560, 0x11c);
    }
    if (!force && ov00_0221B0D0 != 1) {
        return;
    }
    s->phase = 1;
    ov00_02213EC0(s);
    ov00_02212C08(s);
    strcpy(s->first, a);
    strcpy(s->second, b);
    strcpy(s->third, c);
    s->callback = callback;
    s->f48c = 0;
    if (!callback) {
        __msl_assertion_failed(ov00_0221A5E8, ov00_0221A5C8, ov00_0221A560, 0x132);
    }
    s->argument = arg;
    s->message = ov00_0221A5B8;
    s->f4a0 = 0;
    s->attempts = -1;
    s->f7c = 0;
    s->f80 = 0;
    s->status = 0;
    s->f484 = -1;
    s->f480 = 0;
    s->f4a4 = 0;
    s->value = value;
    ov00_0221505C(s, (u32)ov00_0221A5B8);
    s->f5cc = 0;
    srand(ov00_021FF368());
    ov00_021FF3CC();
}
