#include "slop_r1_apricorn_private.h"

void sub_020321A0(SaveApricornBox *box, ApricornRecord *records, int count, int skip) {
    volatile int found;
    int numRecords = 0;
    int outerIndex;
    int initialIndex = 0;
    int foundIndex;
    int j;
    u32 destIndex;

    do {
        if (!sub_02031D6C((UnkStruct_02031CEC *)&box->unk_20[initialIndex][0x18])) {
            break;
        }
        numRecords++;
        initialIndex++;
    } while (initialIndex < 3);

    outerIndex = 0;
    if (count <= 0) {
        return;
    }
    do {
        if (outerIndex == skip) {
            goto next_record;
        }
        if (!sub_02031D6C(&records->data)) {
            goto next_record;
        }
        foundIndex = 0;
        found = FALSE;
        {
            ApricornRecord *boxRecord = (ApricornRecord *)box->unk_20;
            j = found;
            do {
                if (sub_02032158(records, boxRecord)) {
                    foundIndex = (u8)j;
                    found = TRUE;
                    break;
                }
                j++;
                boxRecord++;
            } while (j < 3);
        }
        if (numRecords >= 3) {
            destIndex = 2;
            found = TRUE;
        } else if (found) {
            destIndex = (u8)(numRecords - 1);
        } else {
            destIndex = (u8)numRecords;
            numRecords++;
        }
        if (found && (u32)foundIndex < destIndex) {
            int limit = numRecords - 1;
            if (foundIndex < limit) {
                u8 *cursor = (u8 *)box + foundIndex * 0x20;
                do {
                    *(ApricornRecord *)(cursor + 0x20) = *(ApricornRecord *)(cursor + 0x40);
                    foundIndex++;
                    cursor += 0x20;
                } while (foundIndex < limit);
            }
        }
        *(ApricornRecord *)((u8 *)box + 0x20 + destIndex * 0x20) = *records;
next_record:
        records++;
        outerIndex++;
    } while (outerIndex < count);
}
