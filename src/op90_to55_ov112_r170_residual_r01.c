#pragma force_active on
#include "global.h"
extern void *String_New(u32, int);
extern void String_Delete(void *);
extern void CopyU16ArrayToString(void *, const u16 *);
extern void BufferString(void *, int, void *, int, int, int);
extern const u16 ov112_021FF2EC[];
void ov112_021F1488(void *work, const u16 *name, u8 idx, int len);

// @fn ov112_021F1488
void ov112_021F1488(void *work, const u16 *name, u8 idx, int len) {
    BOOL invalid = TRUE;
    void *str = String_New(len, *(int *)((u8 *)work + 4));
    int i;
    if (name[0] != 0) {
        for (i = 0; i < len; i++) {
            if (name[i] == 0xFFFF) {
                invalid = FALSE;
                break;
            }
        }
    }
    if (invalid) {
        CopyU16ArrayToString(str, ov112_021FF2EC);
    } else {
        CopyU16ArrayToString(str, name);
    }
    BufferString(*(void **)((u8 *)work + 0x58), idx, str, 2, 1, 2);
    String_Delete(str);
}
