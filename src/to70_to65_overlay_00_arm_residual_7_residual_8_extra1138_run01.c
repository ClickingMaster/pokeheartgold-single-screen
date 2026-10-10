#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <string.h>
extern int sprintf(char *, const char *, ...);

typedef struct {
    u8 pad[0x18];
    int value;
} Lookup;

typedef struct {
    int state;
    u32 a, b;
    int id;
    u8 pad[0x28];
    void *handle;
} Work;

typedef struct {
    volatile int a, b, c, d, id, end;
} Request;

extern int ov00_0220657C(void *, int, int, Lookup **, int, int, int);
extern int ov00_02205E50(void *, int, int);
extern int ov00_022020EC(void *, Request *, char *);
extern int ov00_0220211C(void *, Request *, int);
extern int ov00_02201FE0(void *, Request *, const char *, int);
extern int ov00_02201F48(void *, Request *, int);
extern void ov00_021FE754(void *, Request *);
extern u32 ov00_021FFA70(int);
extern int ov00_02202424(void *, Work *, char *);
extern void __msl_assertion_failed(char *, char *, char *, int);
extern char ov00_02219118[];
extern char ov00_022190A8[];
extern char ov00_02218F70[];
extern char ov00_022191D0[];
extern char ov00_022191E0[];
extern char ov00_022191E4[];
extern char ov00_022191EC[];
extern char ov00_022191F4[];
extern char ov00_02218F84[];
extern char ov00_02219200[];

typedef struct {
    u8 pad[16];
    u32 address, port;
} Peer;

typedef struct {
    u8 pad[8];
    Peer *peer;
} Resolved;

typedef struct {
    u8 len, family;
    u16 port;
    u32 address;
} SocketAddress;

extern int ov00_02207DD0(void *, int, Resolved **);
extern void ov00_0220A890(void *, char *);
extern int ov00_021FF804(int, int, int);
extern void ov00_0220A86C(void *, int, char *);
extern void ov00_02202990(void *, int, int);
extern int ov00_021FF44C(int, int);
extern void ov00_02207370(int);
extern int ov00_021FF8A0(int, SocketAddress *, int);
extern int ov00_021FFA60(int);
extern char ov00_02219008[];
extern char ov00_0221914C[];
extern char ov00_02219174[];
extern char ov00_022191A8[];
int ov00_02207610(void *, Work *);

static inline void InitRequest(Request *r) {
    r->id = 0;
    r->a = 0;
    r->b = 0;
    r->c = 0;
    r->d = 0;
    r->end = 0;
}

int ov00_022075AC(void *, Work *);
int ov00_0220779C(void *, Work *, int, const char *);
int ov00_0220792C(void *, Work *, int, int *);

int ov00_022075AC(void *context, Work *w) {
    Lookup *lookup;
    int result = ov00_0220657C(context, 2, 0, &lookup, 0, 0, 0);
    if (result != 0) {
        return result;
    }
    result = ov00_02205E50(context, w->id, lookup->value);
    if (result == 0) {
        w->state = 101;
        result = 0;
    }
    return result;
}

int ov00_02207610(void *context, Work *w) {
    struct {
        Resolved *resolved;
        SocketAddress address;
    } frame;

    if (ov00_02207DD0(context, w->id, &frame.resolved) == 0) {
        ov00_0220A890(context, ov00_02219008);
        return 3;
    }
    w->b = ov00_021FF804(2, 1, 0);
    if ((int)w->b == -1) {
        ov00_0220A86C(context, 5, ov00_0221914C);
        ov00_02202990(context, 3, 0);
        return 3;
    }
    if (ov00_021FF44C(w->b, 0) == 0) {
        ov00_0220A86C(context, 5, ov00_02219174);
        ov00_02202990(context, 3, 0);
        return 3;
    }
    ov00_02207370(w->b);
    {
        SocketAddress *p = &frame.address;
        *(u32 *)p = 0;
        p->address = 0;
    }
    frame.address.family = 2;
    frame.address.address = frame.resolved->peer->address;
    frame.address.port = frame.resolved->peer->port;
    if (ov00_021FF8A0(w->b, &frame.address, 8) == -1) {
        int error = ov00_021FFA60(w->b);
        if (error != -6 && error != -26 && error != -76) {
            ov00_0220A86C(context, 5, ov00_022191A8);
            ov00_02202990(context, 3, 1);
            return 3;
        }
    }
    w->state = 103;
    return 0;
}
