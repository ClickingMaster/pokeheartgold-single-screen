#include "global.h"

typedef struct {
    u32 value;
    u32 pad[2];
} Ov41Entry;

Ov41Entry *ov41_02248D7C(Ov41Entry *entries, int count);

Ov41Entry *ov41_02248D7C(Ov41Entry *entries, int count)
{
    int i;
    i = 0;
    if (count > 0) {
        Ov41Entry *it = entries;
        do {
            if (it->value == 0) {
                return &entries[i];
            }
            i++;
            it++;
        } while (i < count);
    }
    return NULL;
}
