#ifndef TO50_OV59_GAP7_PRIVATE_H
#define TO50_OV59_GAP7_PRIVATE_H
#include "global.h"
typedef struct Ov59Gap7Work {
    u8 raw[0x254];
} Ov59Gap7Work;
void ov59_0223A7FC(Ov59Gap7Work *, int direction);
#endif
