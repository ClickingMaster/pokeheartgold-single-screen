#pragma force_active on
#include "global.h"
#include <nitro/code32.h>

typedef struct {
    int type;
    int sign;
    int value;
} Spec;

extern char ov45_022566E0[], ov45_022565D0[], ov45_02255E78[];
extern void __msl_assertion_failed(const char *, const char *, const char *, int);
extern void *ov00_021EC3BC(void *, u32);
extern void ov00_021EC3D8(void *);
extern void *memset(void *, int, u32);
Spec *ov45_0224AD24(const char *fmt, int *args, int nargs);

// @fn ov45_0224AD24
#pragma opt_strength_reduction off
Spec *ov45_0224AD24(const char *fmt, int *args, int nargs) {
    Spec *list = NULL;
    char c;
    int type;
    int sign;
    int hasArg;
    int count;

    count = 0;
    hasArg = 0;

    if (fmt == NULL) {
        __msl_assertion_failed(ov45_022566E0, ov45_022565D0, ov45_02255E78, 0x494);
    }
    if (*fmt == '+') {
        sign = 1;
    } else if (*fmt == '-') {
        sign = 0;
    } else {
        return NULL;
    }
    fmt++;
    do {
        c = *fmt++;
        switch (c) {
        case '+': sign = 1; type = -1; break;
        case '-': sign = 0; type = -1; break;
        case '\0': type = 0; hasArg = 0; break;
        case 'i': type = 2; hasArg = 0; break;
        case 'l': type = 3; hasArg = 1; break;
        case 'p': type = 4; hasArg = 0; break;
        case 's': type = 5; hasArg = 0; break;
        case 'k': type = 6; hasArg = 1; break;
        case 'm': type = 7; hasArg = 0; break;
        case 'n': type = 8; hasArg = 0; break;
        case 't': type = 9; hasArg = 0; break;
        case 'o': type = 10; hasArg = 1; break;
        case 'v': type = 11; hasArg = 1; break;
        case 'b': type = 1; hasArg = 1; break;
        case 'u': type = 12; hasArg = 0; break;
        case 'w': type = 13; hasArg = 0; break;
        case 'e': type = 14; hasArg = 0; break;
        default: type = -1; break;
        }
        if (type != -1) {
            Spec *e;
            Spec *n = ov00_021EC3BC(list, (count + 1) * sizeof(Spec));
            if (n == NULL) {
                ov00_021EC3D8(list);
                return NULL;
            }
            list = n;
            e = &list[count++];
            memset(e, 0, sizeof(Spec));
            e->sign = sign;
            e->type = type;
            if (hasArg) {
                if (nargs > 0) {
                    nargs--;
                    e->value = *args++;
                } else {
                    e->value = 0;
                }
            }
        }
    } while (c != '\0');
    return list;
}
#pragma opt_strength_reduction reset
