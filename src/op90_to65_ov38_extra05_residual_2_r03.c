#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
typedef struct {
    u32 v[9];
} DivTable;
extern DivTable ov38_02225000;
int ov38_02224C18(char *out, u32 val);
// @fn ov38_02224C18
int ov38_02224C18(char *out, u32 val) {
    DivTable d = ov38_02225000;
    int i;
    int len = 0;
    BOOL started;
    i = 0;
    started = FALSE;
    for (; i < 9; i++) {
        u32 p = d.v[i];
        if (val >= p) {
            u32 q = val / p;
            started = TRUE;
            val -= q * p;
            if (out != NULL) {
                out[len] = q + '0';
            }
            len++;
        } else if (started) {
            if (out != NULL) {
                out[len] = '0';
            }
            len++;
        }
    }
    if (out != NULL) {
        out[len] = val + '0';
    }
    return len + 1;
}
