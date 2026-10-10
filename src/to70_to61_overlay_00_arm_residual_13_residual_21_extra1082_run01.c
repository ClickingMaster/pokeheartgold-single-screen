#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <string.h>
extern int strncmp(const char *, const char *, u32);

typedef struct Work {
    u8 p[0x7c];
    char *buffer;
    int length;
    u8 q[0x488 - 0x84];
    void (*callback)(struct Work *, int, u32, void *);
    u8 r[8];
    void *userdata;
} Work;

extern char *ov00_0221A52C;
extern u32 ov00_0221B9D4;
extern void ov00_0221505C(Work *, char *);
extern void ov00_02214908(Work *);
void ov00_02214130(Work *);
extern u8 __lower_mapC[];
u32 ov00_022141EC(char *, u32);

void ov00_02214130(Work *w) {
    int length = w->length;
    if (length > 0) {
        char *first = ov00_0221A52C;
        if ((u32)length > strlen(first)) {
            char *buffer = w->buffer;
            char *prefix = ov00_0221A52C;
            if (strncmp(buffer, prefix, strlen(prefix)) == 0) {
                ov00_0221505C(w, buffer + strlen(first));
                w->callback(w, 5, ov00_0221B9D4, w->userdata);
            }
        }
    }
    w->callback(w, 4, ov00_0221B9D4, w->userdata);
    ov00_02214908(w);
}
