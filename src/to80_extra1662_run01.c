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

int ov00_0220F7D4(const char *text, u32 *out, u16 *portOut) {
    char local[260];
    u32 address;
    u16 port;
    char *colon;
    if (!text || !*text) {
        address = 0;
        port = 0;
    } else {
        colon = strchr(text, ':');
        if (!colon) {
            port = 0;
        } else {
            if (colon == text) {
                text = 0;
                address = 0;
            } else {
                int length = colon - text;
                if (length >= 256) {
                    __msl_assertion_failed(ov00_02219CF4, ov00_02219D14, ov00_02219CD0, 129);
                }
                memcpy(local, text, length);
                local[length] = 0;
                text = local;
            }
            {
                char *p = colon + 1;
                while (*p) {
                    if (!isDigit(*p)) {
                        return 0;
                    }
                    p++;
                }
            }
            {
                long value = atol(colon + 1);
                if (value < 0 || value > 65535) {
                    return 0;
                }
                port = (u16)value;
            }
        }
        if (text) {
            address = ov00_021FFA44(text);
            if (address == 0xffffffff) {
                Host *h = SOC_GetHostByName(text);
                if (!h) {
                    return 0;
                }
                address = *h->addresses[0];
            }
        }
    }
    if (out) {
        *out = address;
    }
    if (portOut) {
        *portOut = port;
    }
    return 1;
}
