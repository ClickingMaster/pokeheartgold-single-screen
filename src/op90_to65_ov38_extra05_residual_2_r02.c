#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
int ov38_02224B80(const char *s, int n);
// @fn ov38_02224B80
int ov38_02224B80(const char *s, int n) {
    int i;
    int val;
    BOOL started;
    if (n > 10) {
        return -1;
    }
    i = 0;
    val = 0;
    started = FALSE;
    for (; i < n; i++) {
        char c = s[i];
        if (started && (c == ' ' || c == '\0')) {
            break;
        }
        if (!started && c == ' ') {
            continue;
        }
        if (c < '0' || c > '9') {
            return -1;
        }
        started = TRUE;
        {
            int prev = val;
            val = val * 10 + c - '0';
            if (prev > val) {
                return -1;
            }
        }
    }
    return val;
}
