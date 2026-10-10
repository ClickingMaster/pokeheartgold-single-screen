#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern int chars_till_end(s8 *);

int compare_fqdn(s8 *a, s8 *b);

int compare_fqdn(s8 *a, s8 *b) {
    int ch, other;
    for (;;) {
        int first, second;
        while ((other = *a++) == (ch = *b++)) {
            if (other == 0) {
                return 0;
            }
        }
        if (ch != '*') {
            return 1;
        }
        a--;
        first = chars_till_end(a);
        second = chars_till_end(b);
        if (second > first) {
            return 1;
        }
        a += first - second;
    }
}

typedef struct Ctx Ctx;

struct Ctx {
    u8 pad[0x455];
    u8 state;
    u8 pad2[2];
    int error, valid;
    u8 pad3[0x20];
    u8 signature[0x114];
    u32 items;
    u8 pad4[8];
    void *root;
    u8 pad5[8];
    u8 f5ac, f5ad, f5ae, f5af, f5b0;
    u8 pad6[0xff];
    u8 flag;
    u8 pad7[0xff];
    s8 name[0x50];
    s8 *host;
    u8 *certificate;
    u32 length, date;
    u32 (*callback)(u32, Ctx *, int);
};

extern int RTC_GetDate(RTCDate *);
extern int cert_item(Ctx *, u8 **, int, int, int);
extern u32 auth_cert(Ctx *);
extern u32 validate_signature(Ctx *, void *);
void rcv_certificate(Ctx *, u8 *);
