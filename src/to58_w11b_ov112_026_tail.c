#include "global.h"
extern void *NewMsgDataFromNarc(int, int, int, int);
extern void *MessageFormat_New_Custom(int, int, int);
extern void *String_New(int, int);
void ov112_021E7C18(void *w);
void ov112_021E7C18(void *w) {
    *(void **)((u8 *)w + 0x1e44c) = NewMsgDataFromNarc(0, 0x1b, 0x112, 0x9a);
    *(void **)((u8 *)w + 0x1e450) = NewMsgDataFromNarc(1, 0x1b, 0xde, 0x9a);
    *(void **)((u8 *)w + 0x1e448) = MessageFormat_New_Custom(0x10, 0x20, 0x9a);
    *(void **)((u8 *)w + 0x1e454) = String_New(0x190, 0x9a);
}
