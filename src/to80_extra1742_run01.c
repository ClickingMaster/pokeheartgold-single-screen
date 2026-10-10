#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
#include <string.h>

typedef struct W {
    u8 pad[0x114];
    void *callback;
    u8 p118[0x18];
    void *context, *list;
} W;

typedef struct P {
    void *root;
    const char *oldname, *newname;
} P;

extern const char ov45_02255938[];
extern const char ov45_02255918[];
extern const char ov45_0225578C[];
extern const char ov45_02255A30[];
extern const char ov45_02255B7C[];
extern const char ov45_02255BDC[];
extern const char ov45_02255BF4[];
extern const char ov45_02255C10[];
extern const char ov45_02255C34[];
extern const char ov45_02255C4C[];
extern const char ov45_02255C68[];
extern const char ov45_02255C8C[];
extern void __msl_assertion_failed(const char *, const char *, const char *, int);
extern void *ov00_021FF0C0(void *, const char *);
extern int ov00_021FF01C(void *, void *);
extern void ov00_021FEF74(void *, void *);
extern int ov45_0224861C(void *, W *);
extern void ov45_02244D30(void *, int, void *, void *, void *, int, W *, int);
void ov45_02248E00(W *, P *);
#define REQUIRE(c, s, line) \
    if (!(c))               \
    __msl_assertion_failed(s, ov45_02255918, ov45_0225578C, line)

void ov45_02248E00(W *w, P *p) {
    u8 entry[224];

    struct {
        W *w;
        const char *oldname, *newname;
    } payload;

    void *found;
    REQUIRE(w, ov45_02255938, 0x34f);
    REQUIRE(p, ov45_02255A30, 0x350);
    REQUIRE(w->list, ov45_02255B7C, 0x355);
    REQUIRE(p->newname, ov45_02255BDC, 0x35a);
    REQUIRE(*p->newname, ov45_02255BF4, 0x35a);
    REQUIRE(strlen(p->newname) < 128, ov45_02255C10, 0x35a);
    REQUIRE(p->oldname, ov45_02255C34, 0x35b);
    REQUIRE(*p->oldname, ov45_02255C4C, 0x35b);
    REQUIRE(strlen(p->oldname) < 128, ov45_02255C68, 0x35b);
    found = ov00_021FF0C0(w->list, p->oldname);
    if (!found) {
        return;
    }
    memcpy(entry, found, sizeof(entry));
    REQUIRE(ov00_021FF01C(w->list, found), ov45_02255C8C, 0x367);
    strncpy((char *)entry, p->newname, 128);
    entry[127] = 0;
    ov00_021FEF74(w->list, entry);
    if (ov45_0224861C(p->root, w) && w->callback) {
        payload.w = w;
        payload.oldname = p->oldname;
        payload.newname = p->newname;
        ov45_02244D30(p->root, 8, w->callback, &payload, w->context, 0, w, 12);
    }
}
