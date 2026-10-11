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
void *PokeathlonCourseSpritePool_CreateAndTake(void *g, int index, int a, int b, int c);
void PokeathlonCourseSpritePoolSprite_Show(void *obj, int a, int b);
void PokeathlonCourseSpritePoolSprite_SetDrawPriority(void *obj, int a);
void ov96_021FC630(Row *rows, void *g, u16 index);
// @fn ov96_021FC630
void ov96_021FC630(Row *rows, void *g, u16 index) {
    u8 i;
    u8 j;
    for (i = 0; i < 3; i++) {
        Row *row = &rows[i];
        for (j = 0; j < 5; j++) {
            Entry *entry = &row->entries[j];
            entry->obj = PokeathlonCourseSpritePool_CreateAndTake(g, index, 1, 0x6a, 10);
            PokeathlonCourseSpritePoolSprite_Show(entry->obj, 1, 0);
            PokeathlonCourseSpritePoolSprite_SetDrawPriority(entry->obj, 5);
        }
    }
}
