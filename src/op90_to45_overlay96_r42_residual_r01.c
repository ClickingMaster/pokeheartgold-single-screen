#pragma force_active on
#include "global.h"
typedef struct {
    u32 f0;
    u32 f4;
    u32 f8;
    u32 fc;
    u32 f10;
    u32 f14;
    u32 f18;
    u32 f1c;
} FieldRecord;
typedef struct {
    u8 order[4];
    u32 mode : 3;
    u32 best : 12;
    u32 zero : 12;
    u32 pad1 : 5;
    u32 maximum : 16;
    u32 winners : 12;
    u32 pad2 : 4;
    u8 unused;
    u8 rank[4];
} RankResult;
FieldRecord *PokeathlonCourse_GetFieldData(void *course);
u8 *PokeathlonCourse_GetDataCopyArea(void *course);
RankResult *ov96_021E8A20(void *data);
u8 ov96_021ED954(void *course, u8 row);
int ov96_021ED9CC(void *course);
int ov96_021EDCEC(void *course, u8 mode, u8 row, u8 column);
u32 MTRandom(void);
void ov96_021EDA58(void *course);
// @fn ov96_021EDA58
void ov96_021EDA58(void *course) {
    int k;
    int i;
    int maximum;
    int mode;
    int rowValue;
    int value;
    int bit;
    FieldRecord *fields;
    u32 largest;
    FieldRecord *entry;
    RankResult *result;
    int j;
    int index;

    fields = PokeathlonCourse_GetFieldData(course);
    result = ov96_021E8A20(PokeathlonCourse_GetDataCopyArea(course) + 0x28);
    result->zero = 0;
    result->best = 0;
    result->winners = 0;
    for (i = 0; i < 4; i++) {
        result->order[i] = ov96_021ED954(course, i);
    }
    result->mode = ov96_021ED9CC(course);
    maximum = 0;
    largest = 0;
    mode = result->mode;
    for (k = 0, entry = fields; k < 4; k++) {
        for (j = 0; j < 3; j++) {
            if (mode != 4) {
                rowValue = ov96_021EDCEC(course, mode, k, j);
                if (maximum < rowValue) {
                    maximum = rowValue;
                }
            }
            if (largest < entry[j].f8) {
                largest = entry[j].f8;
            }
        }
        entry += 3;
    }
    for (i = 0, bit = 0; i < 4; i++) {
        for (j = 0; j < 3; j++) {
            u8 sel = j + bit;
            if (fields[j].f0 == 0 && fields[j].f10 == 0 && fields[j].f1c == 0) {
                result->zero += 1 << sel;
            }
            if (largest == fields[j].f8) {
                result->winners += 1 << sel;
            }
            if (mode != 4) {
                value = ov96_021EDCEC(course, mode, i, j);
                if (value > maximum) {
                    GF_AssertFail();
                }
                if (maximum == value) {
                    result->best += 1 << sel;
                }
            }
        }
        bit += 3;
        fields += 3;
    }
    result->maximum = maximum;
    index = MTRandom() & 3;
    for (i = 0; i < 4; i++) {
        result->rank[index] = i;
        index = (index + 1) % 4;
    }
}
