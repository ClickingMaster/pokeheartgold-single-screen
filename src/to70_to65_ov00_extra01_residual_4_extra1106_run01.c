#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern int sprintf(char *, const char *, ...);
#include <string.h>
extern long atol(const char *);
extern char *strchr(const char *, int);
extern int ov00_0221B634;
extern char ov00_0221B638[];
extern const char ov00_02219CE4[];
extern const char ov00_02219CEC[];
extern const char ov00_02219CF0[];
extern const char ov00_02219CF4[];
extern const char ov00_02219D14[];
extern const char ov00_02219CD0[];
extern u16 __msl_digit[];

typedef struct {
    u32 addr;
} Addr;

extern char *SOC_InetNtoA(Addr);
extern u32 ov00_021FFA44(const char *);

typedef struct {
    u8 p[12];
    u32 **addresses;
} Host;

extern Host *SOC_GetHostByName(const char *);
extern void __msl_assertion_failed(const char *, const char *, const char *, int);

static inline int isDigit(int c) {
    return (c < 0 || c >= 128) ? 0 : (__msl_digit[c] & 8);
}

char *ov00_0220F724(u32, u16, char *);
int ov00_0220F7D4(const char *, u32 *, u16 *);

char *ov00_0220F724(u32 addr, u16 port, char *buf) {
    if (buf == 0) {
        ov00_0221B634 ^= 1;
        buf = ov00_0221B638 + ov00_0221B634 * 22;
    }
    if (addr) {
        Addr localAddr;
        localAddr.addr = addr;
        if (port) {
            sprintf(buf, ov00_02219CE4, SOC_InetNtoA(localAddr), port);
        } else {
            sprintf(buf, ov00_02219CEC, SOC_InetNtoA(localAddr));
        }
    } else {
        if (port) {
            sprintf(buf, ov00_02219CF0, port);
        } else {
            *buf = 0;
        }
    }
    return buf;
}
