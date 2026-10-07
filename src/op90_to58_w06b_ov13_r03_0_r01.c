#pragma force_active on
#include "global.h"
typedef struct { u32 length; u8 data[0x4c]; u32 flags; } Entry;
typedef struct { u32 count; Entry entries[64]; } List;
extern char ov13_02245A14[];
extern int ov13_02222A9C(const char *);
extern int ov13_02222948(const u8 *, const u8 *, int);
int ov13_022216E0(List *list);
// @fn ov13_022216E0
int ov13_022216E0(List *list) {
 int result = 0; int matches = 0; int i; int count;
 if (list->count == 0) return 5;
 count = list->count;
 if ((u32)count > 64) count = 64;
    for (i = 0; i < count; i++) {
        if (list->entries[i].flags & 1) {
            if (list->entries[i].length == ov13_02222A9C(ov13_02245A14) && ov13_02222948(list->entries[i].data, (const u8 *)ov13_02245A14, ov13_02222A9C(ov13_02245A14)) == 0) {
                matches++;
            }
        }
    }
    if (matches > 1) result = 4;
    if (matches == 0) result = 5;
    return result;
}
