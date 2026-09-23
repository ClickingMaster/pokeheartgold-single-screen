#include "global.h"

void ov41_022463B0(void *work, void *data);

void ov41_02246D54(void *, int, int, int);
void ov41_02246B68(void *, void *);
void ov41_02246BEC(void *, void *);

void ov41_022463B0(void *work, void *data)
{
    ov41_02246D54(data, 0x76, 0x13, 0xe);
    ov41_02246B68(work, data);
    ov41_02246BEC(work, data);
}
