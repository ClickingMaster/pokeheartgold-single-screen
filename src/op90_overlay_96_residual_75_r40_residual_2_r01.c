#pragma force_active on
#include "global.h"
typedef struct {
    int x0;
    void *obj;
} Entry;
typedef struct {
    Entry entries[5];
    u8 pad[4];
} Row;
void *ov96_021EB3E4(void *g, int index, int a, int b, int c);
void ov96_021EB52C(void *obj, int a, int b);
void ov96_021EB630(void *obj, int a);
void ov96_021FC630(Row *rows, void *g, u16 index);
// @fn ov96_021FC630
void ov96_021FC630(Row *rows, void *g, u16 index) {
    u8 i;
    u8 j;
    for (i = 0; i < 3; i++) {
        Row *row = &rows[i];
        for (j = 0; j < 5; j++) {
            Entry *entry = &row->entries[j];
            entry->obj = ov96_021EB3E4(g, index, 1, 0x6a, 10);
            ov96_021EB52C(entry->obj, 1, 0);
            ov96_021EB630(entry->obj, 5);
        }
    }
}
