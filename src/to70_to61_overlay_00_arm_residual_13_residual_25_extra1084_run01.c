#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <string.h>

typedef struct {
    u8 data[4];
} Bytes4;

typedef struct {
    u8 data[2];
} Bytes2;

typedef struct {
    u8 pad[0x4a8];
    u16 port;
} Work;

extern int ov00_02214C60(Work *, void *, const u8 *, int, int);
extern void *ov00_022132C4(Work *, u32, u16);
extern int ov00_0221339C(void *);
extern int ov00_02214A44(u8);
extern int ov00_02214AF4(Work *, const u8 *, int);
extern int ov00_02214A70(const u8 *, int);
extern void ov00_02213C6C(Work *, void *);
extern u8 ov00_0221A63C[];

void ov00_02214BF0(Work *w, const u8 *p, int length, Bytes4 *out, Bytes2 *port);
int ov00_02214F2C(Work *w, const u8 *p, int length);

typedef struct {
    char *name;
    int type;
} Key;

extern int ArrayLength(void *);
extern Key *ov00_021FE6EC(void *, int);
extern void ov00_02213370(void *, u8);
extern void ov00_02213378(void *, u32, u16);
extern void ov00_02213384(void *, u32);
extern void ov00_02212D10(void *, const char *, int);
extern void ov00_02212CD0(void *, const char *, const char *);
extern u8 ov00_02213394(void *);
extern void ov00_0221338C(void *, u8);

void ov00_02214BF0(Work *w, const u8 *p, int length, Bytes4 *out, Bytes2 *port) {
    u8 flags;
    if (length < 5) {
        return;
    }
    flags = p[0];
    *out = *(const Bytes4 *)(p + 1);
    if (flags & 0x10) {
        if (length - 5 < 2) {
            return;
        }
        *port = *(const Bytes2 *)(p + 5);
    } else {
        *(u16 *)port = w->port;
    }
}
