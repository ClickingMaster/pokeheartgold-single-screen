#pragma force_active on
#include "global.h"
#include <nitro/code32.h>

typedef struct {
    u8 data[0x100];
    int count;
} ByteList;
void ov00_02210D9C(ByteList *list, int value);
// @fn ov00_02210D9C
void ov00_02210D9C(ByteList *list, int value) {
    if (list->count >= 0xfe) {
        return;
    }
    if (value < 1 || value > 0xfe) {
        return;
    }
    list->data[list->count++] = value;
}
