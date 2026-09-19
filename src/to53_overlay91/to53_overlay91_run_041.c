#include "private_w19.h"

void ov91_0225EDB0(W19Queue *q) {
    memset(q->point, 0, sizeof(q->point));
    q->read = 0;
    q->write = 0;
}
