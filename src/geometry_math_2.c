#include "unk_02020B8C.h"

BOOL sub_02020EB0(const void *a, const void *b, const void *c, const void *d) {
    if ((sub_02020E80(a, b, c) ^ sub_02020E80(a, b, d)) == 1) {
        if ((sub_02020E80(c, d, a) ^ sub_02020E80(c, d, b)) == 1) {
            return TRUE;
        }
    }
    return FALSE;
}
