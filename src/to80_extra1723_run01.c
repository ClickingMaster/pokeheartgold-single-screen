#pragma force_active on
#include "global.h"
#pragma force_active on

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <string.h>
long atol(const char *);

typedef struct Callback {
    void *fn;
    void *arg;
} Callback;

typedef struct Profile {
    int a, b;
    void *info;
    int unused;
    char *token;
    int updates;
} Profile;

typedef struct Info {
    int id, value;
    char *name, *text;
    u32 address;
    int port;
} Info;

typedef struct Event {
    int id, value;
    char *text;
} Event;

#define FIELD(p, n, t) (*(t *)((char *)(p) + (n)))
extern char ov00_02218268[];
extern char ov00_02218270[];
extern char ov00_022182A0[];
extern char ov00_022182A4[];
extern char ov00_022182AC[];
extern char ov00_022182BC[];
extern char ov00_022182C4[];
extern char ov00_022182D0[];
extern char ov00_022182D4[];
extern char ov00_022182DC[];
extern char ov00_022182E4[];
extern char ov00_022182EC[];
extern char ov00_022182F0[];
extern char ov00_022182F4[];
int ov00_0220A4A0(char *, char *, char *, int);
void ov00_0220A86C(void *, int, char *);
void ov00_02202990(void *, int, int);
void ov00_0220A890(void *, char *);
int ov00_021FFA70(int);
void *ov00_021EC3A8(int);
void ov00_021EC3D8(void *);
char *ov00_021FF3D4(char *);
Profile *ov00_02207CEC(void *, int);
void ov00_0220A348(char *, char *, int);
int ov00_02202A84(void *, Callback, void *, int, int);
void ov00_02201BC4(void *, int, int, char *);
#define BAD()                                 \
    do {                                      \
        ov00_0220A86C(ctx, 1, ov00_02218270); \
        ov00_02202990(ctx, 3, 1);             \
        return 3;                             \
    } while (0)
#define OOM()                              \
    do {                                   \
        ov00_0220A890(ctx, ov00_022182AC); \
        return 1;                          \
    } while (0)
int ov00_02201134(void *, char *);

int ov00_02201134(void *ctx, char *input) {
    char big[4096], small[256], num[16];
    int id, kind, value, result;
    Callback cb1, cb4, cb3, cb2;
    Profile *p;
    Info *info;
    Event *e;
    char *split;
    void *state = FIELD(ctx, 0, void *);
    if (!ov00_0220A4A0(input, ov00_02218268, big, 4096)) {
        BAD();
    }
    kind = atol(big);
    if (!ov00_0220A4A0(input, ov00_022182A0, big, 4096)) {
        BAD();
    }
    id = atol(big);
    if (ov00_0220A4A0(input, ov00_022182A4, big, 4096)) {
        value = atol(big);
    } else {
        value = ov00_021FFA70(0);
    }
    switch (kind) {
    case 1: {
        cb1 = FIELD(state, 0x1bc, Callback);
        if (!cb1.fn) {
            break;
        }
        e = ov00_021EC3A8(12);
        if (!e) {
            OOM();
        }
        if (!ov00_0220A4A0(input, ov00_022182BC, big, 4096)) {
            BAD();
        }
        e->text = ov00_021EC3A8(strlen(big) + 1);
        if (!e->text) {
            OOM();
        }
        strcpy(e->text, big);
        e->id = id;
        e->value = value;
        result = ov00_02202A84(ctx, cb1, e, 0, 2);
        if (result) {
            return result;
        }
        break;
    }
    case 2: {
        p = ov00_02207CEC(ctx, id);
        if (!p) {
            OOM();
        }
        if (!ov00_0220A4A0(input, ov00_022182BC, big, 4096)) {
            BAD();
        }
        split = strstr(big, ov00_022182C4);
        if (!split) {
            BAD();
        }
        *split = 0;
        if (strlen(split + 8) != 32) {
            BAD();
        }
        ov00_021EC3D8(p->token);
        p->token = 0;
        p->token = ov00_021FF3D4(split + 8);
        p->updates++;
        cb2 = FIELD(state, 0x1ac, Callback);
        if (!cb2.fn) {
            break;
        }
        e = ov00_021EC3A8(0x40c);
        if (!e) {
            OOM();
        }
        ov00_0220A348((char *)e + 8, big, 0x401);
        e->id = id;
        e->value = value;
        result = ov00_02202A84(ctx, cb2, e, 0, 6);
        if (result) {
            return result;
        }
        break;
    }
    case 100: {
        p = ov00_02207CEC(ctx, id);
        if (!p) {
            OOM();
        }
        if (!p->info) {
            p->info = ov00_021EC3A8(24);
            if (!p->info) {
                OOM();
            }
            memset(p->info, 0, 24);
            FIELD(p->info, 0, int) = FIELD(state, 0x430, int)++;
        }
        info = p->info;
        if (!ov00_0220A4A0(input, ov00_022182BC, big, 4096)) {
            BAD();
        }
        if (!ov00_0220A4A0(big, ov00_022182D0, num, 16)) {
            BAD();
        }
        info->value = atol(num);
        ov00_021EC3D8(info->name);
        info->name = 0;
        if (!ov00_0220A4A0(big, ov00_022182D4, small, 256)) {
            small[0] = 0;
        }
        info->name = ov00_021FF3D4(small);
        if (!info->name) {
            OOM();
        }
        ov00_021EC3D8(info->text);
        info->text = 0;
        if (!ov00_0220A4A0(big, ov00_022182DC, small, 256)) {
            small[0] = 0;
        }
        info->text = ov00_021FF3D4(small);
        if (!info->text) {
            OOM();
        }
        if (ov00_0220A4A0(big, ov00_022182E4, num, 16)) {
            u32 a = atol(num), b = atol(num), c = atol(num), d = atol(num);
            info->address = ((c >> 24) & 255) | ((d >> 8) & 0xff00) | ((b << 8) & 0xff0000) | ((a << 24) & 0xff000000);
        } else {
            info->address = 0;
        }
        if (ov00_0220A4A0(big, ov00_022182EC, num, 16)) {
            u16 port = atol(num);
            info->port = (u16)(((port >> 8) & 255) | ((port << 8) & 0xff00));
        } else {
            info->port = 0;
        }
        cb3 = FIELD(state, 0x1b4, Callback);
        if (!cb3.fn) {
            break;
        }
        e = ov00_021EC3A8(12);
        if (!e) {
            OOM();
        }
        e->id = id;
        e->text = (char *)info->id;
        e->value = value;
        result = ov00_02202A84(ctx, cb3, e, 0, 5);
        if (result) {
            return result;
        }
        break;
    }
    case 101: {
        int port;
        if (!ov00_0220A4A0(input, ov00_022182BC, big, 4096)) {
            BAD();
        }
        split = strstr(big, ov00_022182EC);
        if (!split) {
            BAD();
        }
        if (!split[3]) {
            BAD();
        }
        port = atol(split + 3);
        split = strstr(big, ov00_022182F0);
        if (!split) {
            small[0] = 0;
        } else {
            ov00_0220A348(small, split + 3, 256);
        }
        cb4 = FIELD(state, 0x1c4, Callback);
        if (!cb4.fn) {
            break;
        }
        e = ov00_021EC3A8(0x108);
        if (!e) {
            OOM();
        }
        e->id = id;
        e->value = port;
        strcpy((char *)e + 8, small);
        result = ov00_02202A84(ctx, cb4, e, 0, 0);
        if (result) {
            return result;
        }
        break;
    }
    case 102:
        if (!ov00_0220A4A0(input, ov00_022182BC, big, 4096)) {
            BAD();
        }
        ov00_02201BC4(ctx, id, 0x67, ov00_022182F4);
        break;
    default:
        break;
    }
    return 0;
}
