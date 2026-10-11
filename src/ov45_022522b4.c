#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef struct {
    void *data;
    int used, capacity;
} Buffer;

typedef struct {
    int handle;
    u32 state;
    u8 pad[0x108 - 8];
    Buffer first, second;
    u8 pad2[0x328 - 0x120];
    void *owned[8];
    void **parts;
    int count;
} Work;

extern char ov45_02257334[];
extern char ov45_02257344[];
extern char ov45_02257258[];
extern char ov45_02257234[];
extern char ov45_022573C4[];
extern char ov45_022573E4[];
extern char ov45_02257408[];
extern char ov45_02257438[];
extern char ov45_0225745C[];
extern char ov45_0225749C[];
extern char ov45_022574BC[];
extern char ov45_022574E0[];
extern char ov45_02257514[];
extern char ov45_02257538[];
extern void __msl_assertion_failed(const char *, const char *, const char *, int);
extern void ov00_021FF82C(int, int);
extern void ov00_021FF818(int);
extern void ov45_02251C40(Buffer *);
extern void ov00_021EC3D8(void *);

static inline void validate(Buffer *b, char *ptr, char *cap, char *align, char *used, char *size) {
    if (!b) {
        __msl_assertion_failed(ptr, ov45_02257258, ov45_02257234, 283);
    }
    if (b->capacity < 0) {
        __msl_assertion_failed(cap, ov45_02257258, ov45_02257234, 283);
    }
    if (b->capacity % 8192 != 0) {
        __msl_assertion_failed(align, ov45_02257258, ov45_02257234, 283);
    }
    if (b->used < 0) {
        __msl_assertion_failed(used, ov45_02257258, ov45_02257234, 283);
    }
    if (b->used > b->capacity) {
        __msl_assertion_failed(size, ov45_02257258, ov45_02257234, 283);
    }
}

void ov45_022522B4(Work *);

void ov45_022522B4(Work *w) {
    int i;
    if (!w) {
        __msl_assertion_failed(ov45_02257334, ov45_02257258, ov45_02257234, 283);
    }
    if (w->state > 2) {
        __msl_assertion_failed(ov45_02257344, ov45_02257258, ov45_02257234, 283);
    }
    validate(&w->first, ov45_022573C4, ov45_022573E4, ov45_02257408, ov45_02257438, ov45_0225745C);
    validate(&w->second, ov45_0225749C, ov45_022574BC, ov45_022574E0, ov45_02257514, ov45_02257538);
    if (w->handle != -1) {
        ov00_021FF82C(w->handle, 2);
        ov00_021FF818(w->handle);
    }
    w->state = 2;
    ov45_02251C40(&w->first);
    ov45_02251C40(&w->second);
    ov00_021EC3D8(w->owned[0]);
    ov00_021EC3D8(w->owned[1]);
    ov00_021EC3D8(w->owned[2]);
    ov00_021EC3D8(w->owned[3]);
    ov00_021EC3D8(w->owned[4]);
    ov00_021EC3D8(w->owned[5]);
    ov00_021EC3D8(w->owned[6]);
    ov00_021EC3D8(w->owned[7]);
    for (i = 0; i < w->count; i++) {
        ov00_021EC3D8(w->parts[i]);
    }
    ov00_021EC3D8(w->parts);
}
