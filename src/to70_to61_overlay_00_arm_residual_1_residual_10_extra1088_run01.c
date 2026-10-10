#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
extern unsigned long strtoul(const char *, char **, int);

typedef struct {
    u8 pad[0x1c];
    u8 connection[8];
    int state;
    u8 pad2[0x88 - 0x28];
    void (*callback)(int, int, int, int, int, void *);
    void *context;
} Work;

typedef struct {
    int a, b;
    char text[520];
} Meta;

extern Work *ov00_0221A784;
extern char ov00_022179EC[];
extern char ov00_022179F0[];
extern char ov00_022179F4[];
extern int ov00_021EC230(void);
extern int ov00_021F03BC(void *);
extern int ov00_022005B0(void *, int);
extern void ov00_021EE6FC(void), ov00_021EE800(int), ov00_02200540(void *, int, int *), ov00_02200400(void *, int, Meta *), ov00_021ED4E8(char *, char *, char *, int), ov00_021F1BCC(int, void (*)(void), int, void *, void *), ov00_021EEB1C(void), ov00_021EC24C(int, int), ov00_021F04A0(int, char *, int);

int ov00_021EE054(void *name, void (*callback)(int, int, int, int, int, void *), void *arg, void *context, void *user);

int ov00_021EE054(void *name, void (*callback)(int, int, int, int, int, void *), void *arg, void *context, void *user) {
    struct {
        char val[4];
        int id;
        Meta meta;
    } local;

    local.id = -1;
    int peer, code;
    u8 a, b;
    if (ov00_021EC230() != 0 || ov00_0221A784->state != 3) {
        return 0;
    }
    ov00_021EE6FC();
    ov00_0221A784->callback = callback;
    ov00_0221A784->context = arg;
    ov00_021EE800(5);
    peer = ov00_021F03BC(name);
    if (peer == 0 || ov00_022005B0(ov00_0221A784->connection, peer) == 0) {
        code = 11;
        goto error;
    }
    ov00_02200540(ov00_0221A784->connection, peer, &local.id);
    ov00_02200400(ov00_0221A784->connection, local.id, &local.meta);
    if (local.meta.b != 6) {
        code = 11;
        goto error;
    }
    local.val[0] = '0';
    ov00_021ED4E8(ov00_022179EC, local.val, local.meta.text, 47);
    a = strtoul(local.val, 0, 10);
    ov00_021ED4E8(ov00_022179F0, local.val, local.meta.text, 47);
    b = strtoul(local.val, 0, 10);
    if (b == a) {
        code = 13;
        goto error;
    }
    ov00_021F1BCC(peer, ov00_021EEB1C, 0, context, user);
    return 1;
error:
    ov00_021EC24C(code, 0);
    ov00_0221A784->callback(code, 0, 1, 0, 0, ov00_0221A784->context);
    if (ov00_0221A784 && ov00_0221A784->state == 5) {
        ov00_021EE800(3);
        ov00_021F04A0(1, ov00_022179F4, 0);
    }
    return 1;
}
