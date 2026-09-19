#include "private_w19.h"

BOOL ov91_0225EE64(W19Queue *q, u16 *out) {
    int n;
    if (q->write == q->read) return FALSE;
    n = q->write - 1;
    if (n < 0) n += 8;
    out[0] = q->point[n][0];
    out[1] = q->point[n][1];
    return TRUE;
}


u32 ov91_0225EE88(W19Queue *q) {
    if (q->read > q->write) return 8 - q->read + q->write;
    return q->write - q->read;
}
