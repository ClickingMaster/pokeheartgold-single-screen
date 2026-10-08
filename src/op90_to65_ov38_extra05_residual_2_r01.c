#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
int ov38_02224A98(const char *s, int n);

static inline BOOL IsUpper(int c) {
    BOOL ret = FALSE;
    if (c >= 'A' && c <= 'Z') {
        ret = TRUE;
    }
    return ret;
}

// @fn ov38_02224A98
int ov38_02224A98(const char *s, int n) {
    int i;
    int val;
    BOOL started;
    if (n > 8) {
        return -1;
    }
    if (n == 8 && s[0] > '7') {
        return -1;
    }
    i = 0;
    val = 0;
    started = FALSE;
    for (; i < n; i++) {
        char c = s[i];
        c = IsUpper(c) ? c + 0x20 : c;
        if (c >= '0' && c <= '9') {
            started = TRUE;
            val = val * 16 + c - '0';
        } else if (c >= 'a' && c <= 'f') {
            started = TRUE;
            val = val * 16 + c - 'a' + 10;
        } else {
            if (started && (c == ' ' || c == '\0')) {
                break;
            }
            if (!started && c == ' ') {
                continue;
            }
            return -1;
        }
    }
    return val;
}
