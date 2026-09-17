#include "to50_overlay59_gap2_private.h"
#define U8(w, off)  (*(u8 *)((u8 *)(w) + (off)))
#define PTR(w, off) (*(void **)((u8 *)(w) + (off)))

void ov59_02239D08(Ov59R8Work *work, int msgNo) {
    switch (msgNo) {
    case 6: {
        int count = sub_02031C08(PTR(work, 0x10));
        BufferIntegerAsString(PTR(work, 0x60), 0, 5 - count, 1, 0, 1);
        break;
    }
    case 12:
        if (U8(work, 0x4E) > 1) {
            BufferItemNamePlural(PTR(work, 0x60), 0, 0x1E5 + U8(work, 0x4D));
        } else {
            BufferItemName(PTR(work, 0x60), 0, 0x1E5 + U8(work, 0x4D));
        }
        BufferIntegerAsString(PTR(work, 0x60), 1, U8(work, 0x4E), 2, 0, 1);
        break;
    case 21: {
        int box = sub_02032004((u8 *)work + 0x22);
        ReadMsgDataIntoString(PTR(work, 0x5C), box + 0x13, PTR(work, 0x64));
        BufferString(PTR(work, 0x60), 0, PTR(work, 0x64), 2, 0, 2);
        BufferIntegerAsString(PTR(work, 0x60), 1, U8(work, 0x15), 3, 0, 1);
        BufferIntegerAsString(PTR(work, 0x60), 2, U8(work, 0x16), 3, 0, 1);
        BufferIntegerAsString(PTR(work, 0x60), 3, 5 - U8(work, 0x18), 1, 0, 1);
        break;
    }
    default:
        ReadMsgDataIntoString(PTR(work, 0x5C), msgNo + 0x1B, PTR(work, 0x68));
        return;
    }
    ReadMsgDataIntoString(PTR(work, 0x5C), msgNo + 0x1B, PTR(work, 0x64));
    StringExpandPlaceholders(PTR(work, 0x60), PTR(work, 0x68), PTR(work, 0x64));
}
