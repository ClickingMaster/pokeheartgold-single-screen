#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef struct {
    volatile u32 tag;
    u32 value;
    char *text;
} Entry;

extern char ov45_02256690[];
extern char ov45_022565D0[];
extern char ov45_02256128[];
extern long atol(const char *);
extern void __msl_assertion_failed(const char *, const char *, const char *, int);
void ov45_0224AFD8(u32 *, Entry *);

void ov45_0224AFD8(u32 *out, Entry *entries) {
    if (entries->tag == 0) {
        return;
    }
    do {
        switch (entries->tag) {
        case 1:
            break;
        case 2:
            out[0] = entries->value;
            break;
        case 3:
            if (entries->value && entries->text) {
                out[7] = atol(entries->text);
            } else {
                out[7] = 0;
            }
            break;
        case 4:
            out[1] = entries->value;
            break;
        case 5:
            out[2] = entries->value;
            break;
        case 6:
            break;
        case 7:
            out[3] = entries->value;
            break;
        case 8:
            out[4] = entries->value;
            break;
        case 9:
            out[5] = entries->value;
            break;
        case 10:
        case 11:
        case 12:
        case 13:
            break;
        case 14:
            out[6] = entries->value;
            break;
        default:
            __msl_assertion_failed(ov45_02256690, ov45_022565D0, ov45_02256128, 0x56c);
            break;
        }
        entries++;
    } while (entries->tag);
}
