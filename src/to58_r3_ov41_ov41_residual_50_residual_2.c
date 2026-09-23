#include "global.h"

void GF_AssertFail(void);

typedef struct {
    void *value;
    void *arg;
    u32 pad[2];
} Ov41Slot;

typedef struct {
    Ov41Slot *slots;
    int capacity;
} Ov41Slots;

Ov41Slot *ov41_022499F0(Ov41Slots *array, void *value, void *arg);

Ov41Slot *ov41_022499F0(Ov41Slots *array, void *value, void *arg)
{
    int i;
    if (array->slots == NULL) {
        GF_AssertFail();
    }
    if (array->capacity == 0) {
        GF_AssertFail();
    }
    for (i = 0; i < array->capacity; i++) {
        if (array->slots[i].value == NULL) {
            break;
        }
    }
    if (array->capacity <= i) {
        GF_AssertFail();
    }
    array->slots[i].value = value;
    array->slots[i].arg = arg;
    return &array->slots[i];
}
