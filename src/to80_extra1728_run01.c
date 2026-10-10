#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <string.h>
#pragma force_active on

typedef struct {
    u8 pad[12];
    char name[0x4b0 - 12];
    int socket;
} Work;

typedef struct {
    u8 len, family;
    u16 port;
    u32 address;
} Address;

typedef struct {
    u8 bytes[4];
} Bytes;

typedef struct {
    void *name, *aliases;
    int type;
    u8 **addresses;
} Host;

extern void *ov00_0221B9DC[];
extern const char ov00_0221A5FC[];
extern int ov00_022141EC(const char *, int);
extern int sprintf(char *, const char *, ...);
extern u32 ov00_021FFA44(const char *);
extern Host *SOC_GetHostByName(const char *);
extern int ov00_021FF804(int, int, int);
extern int ov00_021FF8A0(int, Address *, int);
extern void ov00_021FF818(int);
int ov00_02214244(Work *);

int ov00_02214244(Work *w) {
    struct {
        Address address;
        char host[128];
    } f;

    int result = ov00_022141EC(w->name, 20);
    if (ov00_0221B9DC[1]) {
        strcpy(f.host, ov00_0221B9DC[1]);
    } else {
        sprintf(f.host, ov00_0221A5FC, w->name, result);
    }
    f.address.family = 2;
    f.address.port = 0xee70;
    f.address.address = ov00_021FFA44(f.host);
    if (f.address.address == (u32)-1) {
        Host *h = SOC_GetHostByName(f.host);
        if (!h) {
            return 2;
        }
        memcpy(&f.address.address, h->addresses[0], 4);
    }
    if (w->socket == -1) {
        w->socket = ov00_021FF804(2, 1, 0);
        if (w->socket == -1) {
            return 1;
        }
    }
    if (ov00_021FF8A0(w->socket, &f.address, 8) == 0) {
        return 0;
    }
    ov00_021FF818(w->socket);
    w->socket = -1;
    return 3;
}
