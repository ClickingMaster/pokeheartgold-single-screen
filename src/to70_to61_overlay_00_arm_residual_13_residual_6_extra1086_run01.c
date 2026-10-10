#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
extern void ov00_022115B0(void *, void *, int, u8 *, int, u8 *, int, u8 *);
void ov00_02211608(void *, void *, u8 *, int);

typedef struct {
    u8 entries[256];
    int count;
} List;

typedef struct {
    char data[0x800];
    int length;
} Buffer;

typedef struct {
    u8 p[0x88];
    void (*base)(u8, Buffer *, void *);
    void (*first)(u8, int, Buffer *, void *);
    void (*second)(u8, int, Buffer *, void *);
    void (*list)(int, List *, void *);
    int (*count)(int, void *);
    u8 q[0x10c - 0x9c];
    void *context;
} Work;

extern char *ov00_0221A0B8[];
extern char ov00_02219EFC[];
extern char ov00_02219F04[];
extern char ov00_02219F08[];
extern void ov00_02210DFC(Buffer *, char *);
extern int sprintf(char *, const char *, ...);
void ov00_022116C0(Work *, Buffer *, int);

void ov00_022116C0(Work *w, Buffer *b, int type) {
    char text[128];
    List list;
    int count, i, old, j;
    list.count = 0;
    if ((u32)(type - 1) > 1) {
        count = 1;
    } else {
        count = w->count(type, w->context);
    }
    w->list(type, &list, w->context);
    for (i = 0; i < list.count; i++) {
        char *name = ov00_0221A0B8[list.entries[i]];
        if (name == 0) {
            name = ov00_02219EFC;
        }
        if (type == 0) {
            ov00_02210DFC(b, name);
            b->data[b->length - 1] = '\\';
            old = b->length;
            w->base(list.entries[i], b, w->context);
            if (old == b->length) {
                ov00_02210DFC(b, ov00_02219F04);
            }
            b->data[b->length - 1] = '\\';
        } else {
            for (j = 0; j < count; j++) {
                sprintf(text, ov00_02219F08, name, j);
                ov00_02210DFC(b, text);
                b->data[b->length - 1] = '\\';
                old = b->length;
                if (type == 1) {
                    w->first(list.entries[i], j, b, w->context);
                } else if (type == 2) {
                    w->second(list.entries[i], j, b, w->context);
                }
                if (old == b->length) {
                    ov00_02210DFC(b, ov00_02219F04);
                }
                b->data[b->length - 1] = '\\';
            }
        }
    }
}
