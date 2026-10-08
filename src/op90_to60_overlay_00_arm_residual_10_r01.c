#pragma force_active on
#include "global.h"
#include <nitro/code32.h>

extern const char ov00_02219A08[];
extern const char ov00_02219A10[];
extern const char ov00_022199D0[];
void __msl_assertion_failed(const char *expr, const char *file, const char *func, int line);
u32 ov00_021FF368(void);
void srand(unsigned int seed);
int rand(void);
BOOL ov00_0220BC50(const u8 *buf);
u8 *ov00_0220BCEC(u8 *buf);

// @fn ov00_0220BC50
BOOL ov00_0220BC50(const u8 *buf) {
    int parity;
    int i;
    parity = 0;
    i = 1;
    for (; i < 32; i++) {
        parity = ((buf[0] & 1) ^ (parity ^ ((i ^ buf[i - 1]) & 1))) ^ (buf[0] < 0x4f ? 1 : 0) ^ (buf[i - 1] < buf[0] ? 1 : 0);
        if ((parity != 0 && (buf[i] & 1) == 0) || (parity == 0 && (buf[i] & 1) == 1)) {
            return FALSE;
        }
    }
    return TRUE;
}

// @fn ov00_0220BCEC
u8 *ov00_0220BCEC(u8 *buf) {
    int i;
    int parity;
    if (buf == NULL) {
        __msl_assertion_failed(ov00_02219A08, ov00_02219A10, ov00_022199D0, 0x2b);
    }
    srand(ov00_021FF368());
    buf[0] = rand() % 93 + 33;
    parity = 0;
    i = 1;
    for (; i < 32; i++) {
        parity = ((buf[0] & 1) ^ (parity ^ ((i ^ buf[i - 1]) & 1))) ^ (buf[0] < 0x4f ? 1 : 0) ^ (buf[i - 1] < buf[0] ? 1 : 0);
        buf[i] = rand() % 93 + 33;
        if ((parity != 0 && (buf[i] & 1) == 0) || (parity == 0 && (buf[i] & 1) == 1)) {
            buf[i]++;
        }
    }
    return buf;
}
