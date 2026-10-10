#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern void *ov00_021EC3A8(u32);
extern int ov00_021FF804(int, int, int);
extern int ov00_021FF44C(int, int);
extern int ov00_021FF8A0(int, void *, int);
extern int ov00_021FFA60(int);
extern void ov00_0220A890(void *, char *);
extern void ov00_0220A86C(void *, int, char *);
extern void ov00_02202990(void *, int, int);
extern void __msl_assertion_failed(char *, char *, char *, int);
extern char ov00_02219328[];
extern char ov00_02219338[];
extern char ov00_02219360[];
extern char ov00_022192E8[];
extern char ov00_02219394[];
extern char ov00_022193C4[];
extern char ov00_022193E4[];
extern char ov00_022192D0[];
extern char ov00_022193F0[];

typedef struct {
    u32 a, b, c;
    u32 **addresses;
} Host;

extern Host *SOC_GetHostByName(char *);

typedef struct {
    int a, socket;
    void *buffer;
    u32 size;
} Data;

typedef struct {
    int a;
    Data *data;
    u8 p[12];
    int state;
} Work;

typedef union {
    struct {
        u8 len, family;
        u16 port;
        u32 address;
    } fields;

    u32 words[2];
} Address;

int ov00_02207FF0(void *, Work *);

int ov00_02207FF0(void *ctx, Work *w) {
    Data *d = w->data;
    Host *h;
    int error;
    d->size = 4096;
    d->buffer = ov00_021EC3A8(4097);
    if (!d->buffer) {
        ov00_0220A890(ctx, ov00_02219328);
        return 1;
    }
    d->socket = ov00_021FF804(2, 1, 0);
    if (d->socket == -1) {
        ov00_0220A86C(ctx, 5, ov00_02219338);
        ov00_02202990(ctx, 3, 1);
        return 3;
    }
    if (ov00_021FF44C(d->socket, 0) == 0) {
        ov00_0220A86C(ctx, 5, ov00_02219360);
        ov00_02202990(ctx, 3, 1);
        return 3;
    }
    h = SOC_GetHostByName(ov00_022192E8);
    if (!h) {
        ov00_0220A86C(ctx, 5, ov00_02219394);
        ov00_02202990(ctx, 3, 1);
        return 3;
    }
    {
        Address addr = { 0 };
        addr.fields.family = 2;
        addr.fields.address = **h->addresses;
        if (!addr.fields.address) {
            __msl_assertion_failed(ov00_022193C4, ov00_022193E4, ov00_022192D0, 89);
        }
        addr.fields.port = 0xcd74;
        if (ov00_021FF8A0(d->socket, &addr, 8) == -1) {
            error = ov00_021FFA60(d->socket);
            if (error != -6 && error != -26 && error != -76) {
                ov00_0220A86C(ctx, 5, ov00_022193F0);
                ov00_02202990(ctx, 3, 1);
                return 3;
            }
        }
        w->state = 1;
        return 0;
    }
}
