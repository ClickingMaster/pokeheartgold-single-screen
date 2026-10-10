#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
#include <string.h>

typedef struct W {
    u8 pad[0x80c];
    void *list;
} W;

typedef struct Payload {
    W *w;
    const char *oldname, *newname;
} Payload;

extern const char ov45_022559B8[];
extern const char ov45_02255918[];
extern const char ov45_022556C4[];
extern const char ov45_02255C98[];
extern const char ov45_02255CA8[];
extern const char ov45_02255CBC[];
extern const char ov45_02255CD8[];
extern const char ov45_02255CE8[];
extern const char ov45_02255CFC[];
extern void __msl_assertion_failed(const char *, const char *, const char *, int);
extern void ov45_02248E00(void *, void *);
extern void ov00_021FF160(void *, void (*)(void *, void *), void *);
void ov45_02249054(W *, const char *, const char *);
#define REQUIRE(c, s, line) \
    if (!(c))               \
    __msl_assertion_failed(s, ov45_02255918, ov45_022556C4, line)

void ov45_02249054(W *w, const char *oldname, const char *newname) {
    Payload p;
    REQUIRE(w, ov45_022559B8, 0x386);
    REQUIRE(oldname, ov45_02255C98, 0x388);
    REQUIRE(*oldname, ov45_02255CA8, 0x388);
    REQUIRE(strlen(oldname) < 128, ov45_02255CBC, 0x388);
    REQUIRE(newname, ov45_02255CD8, 0x389);
    REQUIRE(*newname, ov45_02255CE8, 0x389);
    REQUIRE(strlen(newname) < 128, ov45_02255CFC, 0x389);
    p.w = w;
    p.oldname = oldname;
    p.newname = newname;
    ov00_021FF160(w->list, ov45_02248E00, &p);
}
