#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
int ov38_02224CD4(const char *a, const char *b);

static inline BOOL IsUpper(int c) {
    BOOL ret = FALSE;
    if (c >= 'A' && c <= 'Z') {
        ret = TRUE;
    }
    return ret;
}

static inline int ToLower(char c) {
    return IsUpper(c) ? c + 0x20 : c;
}

static inline int ToLowerAt(const char *p) {
    int upper = 0;
    if (*p >= 'A' && *p <= 'Z') {
        upper = 1;
    }
    return upper ? *p + 0x20 : *p;
}

// @fn ov38_02224CD4
int ov38_02224CD4(const char *a, const char *b) {
    int lb, la;
    while (lb = ToLower(*b), la = ToLowerAt(a), la == lb) {
        if (*a == '\0' || *a == ' ') {
            return 0;
        }
        a++;
        b++;
    }
    return -1;
}
