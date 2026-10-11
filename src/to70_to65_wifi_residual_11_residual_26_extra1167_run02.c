#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
u32 parse_time(const u8 *p, int type);
#include <string.h>

typedef struct {
    u32 a;
    int length;
    void *modulus;
    int explen;
    void *exponent;
} Key;

extern void *(*gCpsAlloc)(int);
extern void (*gCpsFree)(void *);
extern void CPSi_big_from_char(u16 *, void *, int, int);
extern void CPSi_big_power(u16 *, u16 *, u16 *, int, u16 *);
extern void exit_computebound(int);
extern void CPSi_char_from_big(u8 *, u16 *, int, int);
extern int enter_computebound(void);
int validate_signature(u8 *, Key *);

int validate_signature(u8 *w, Key *k) {
    u16 *buf, *sig, *exp, *mod;
    int words, result, state;
    if (!*(void **)(w + 0x5a4) || !*(int *)(w + 0x5a8) || !k->exponent || !k->explen || !k->modulus || !k->length) {
        return 2;
    }
    words = (k->length * 2) / 2;
    buf = gCpsAlloc(words * 8);
    if (!buf) {
        return 2;
    }
    sig = buf + words;
    exp = sig + words;
    mod = exp + words;
    CPSi_big_from_char(sig, *(void **)(w + 0x5a4), *(int *)(w + 0x5a8), words);
    CPSi_big_from_char(exp, k->exponent, k->explen, words);
    CPSi_big_from_char(mod, k->modulus, k->length, words);
    state = enter_computebound();
    CPSi_big_power(buf, sig, exp, words, mod);
    exit_computebound(state);
    CPSi_char_from_big((u8 *)sig, buf, k->length, words);
    result = 0;
    if (((u8 *)sig)[0] != 0 || ((u8 *)sig)[1] != 1) {
        result = 2;
    } else {
        int len = k->length, i = 2;
        while (i < len) {
            if (*(u8 *)((u32)sig + i) != 255) {
                break;
            }
            i++;
        }
        if (i + 1 >= len || ((u8 *)sig)[i] != 0 || ((u8 *)sig)[i + 1] != 48 || memcmp(w + 0x468, (u8 *)sig + len - *(int *)(w + 0x47c), *(int *)(w + 0x47c))) {
            result = 2;
        }
    }
    gCpsFree(buf);
    return result;
}
