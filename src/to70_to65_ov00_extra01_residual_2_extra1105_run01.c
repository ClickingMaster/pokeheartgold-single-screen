#pragma force_active on
#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <string.h>

typedef struct {
    int socket;
    u32 address;
    u16 port, pad;
    void *a, *b;
    u8 p[16];
    void *user;
    u8 p2[16];
    int receive, send;
    u32 last;
} Work;

typedef struct {
    u8 length, family;
    u16 port;
    u32 address;
} Address;

extern void ov00_021FF3CC(void);
extern int ov00_0220F7D4(char *, u32 *, u16 *);
extern void *ov00_021EC3A8(u32);
extern void ov00_021EC3D8(void *);
extern void ov00_0220EEA8(void);
extern void ov00_0220EE88(void);
extern void ov00_0220EEDC(void);
extern void *ov00_021FED4C(int, int, int, void *, void *, int);
extern void *ov00_021FE5B4(int, int, void *);
extern void ov00_021FEE8C(void *);
extern void ov00_021FE678(void *);
extern int ov00_021FF804(int, int, int);
extern int ov00_021FF840(int, void *, int);
extern int ov00_021FFA18(int, void *, int *);
extern void ov00_021FF818(int);
int ov00_0220EF24(Work **, char *, int, int, void *);

static inline u16 swap(u16 value) {
    int v = value;
    return ((v >> 8) & 255) | ((v << 8) & 0xff00);
}

int ov00_0220EF24(Work **out, char *name, int receive, int send, void *user) {
    struct {
        u16 port, pad;
        int length;
        u32 host;
        Address address;
    } local;

    Work *w;
    ov00_021FF3CC();
    if (send == 0) {
        send = 65536;
    }
    if (receive == 0) {
        receive = 65536;
    }
    if (ov00_0220F7D4(name, &local.host, &local.port) == 0) {
        return 4;
    }
    w = ov00_021EC3A8(68);
    if (w == 0) {
        return 1;
    }
    memset(w, 0, 68);
    w->socket = -1;
    w->send = send;
    w->receive = receive;
    w->user = user;
    w->a = ov00_021FED4C(4, 32, 2, ov00_0220EE88, ov00_0220EEA8, 0);
    if (w->a == 0) {
        ov00_021EC3D8(w);
        return 1;
    }
    w->b = ov00_021FE5B4(4, 4, ov00_0220EEDC);
    if (w->b == 0) {
        ov00_021FEE8C(w->a);
        ov00_021EC3D8(w);
        return 1;
    }
    w->socket = ov00_021FF804(2, 2, 0);
    if (w->socket == -1) {
        ov00_021FEE8C(w->a);
        ov00_021FE678(w->b);
        ov00_021EC3D8(w);
        return 3;
    }
    memset(&local.address, 0, 8);
    local.address.family = 2;
    local.address.address = local.host;
    local.address.port = swap(local.port);
    if (ov00_021FF840(w->socket, &local.address, 8) == -1) {
        ov00_021FF818(w->socket);
        ov00_021FEE8C(w->a);
        ov00_021FE678(w->b);
        ov00_021EC3D8(w);
        return 3;
    }
    local.length = 8;
    ov00_021FFA18(w->socket, &local.address, &local.length);
    w->address = local.address.address;
    w->port = swap(local.address.port);
    *out = w;
    return 0;
}
