#pragma force_active on
#include "global.h"
#include "pokemon_types_def.h"
#include "gf_gfx_loader.h"
typedef struct {
    u16 species;
    u16 form;
    u8 pad4[2];
    u8 shiny;
    u8 gender;
    u32 x8;
    u32 personality;
} MonInfo;
typedef struct {
    enum HeapID heapId;
    u8 pad4[0x10];
    void *x14;
    u8 pad18[0x74 - 0x18];
    void *x74[3];
} Heap;
void *PokeathlonCourse_GetHeapAllocPtr4(void *course);
u8 *PokeathlonCourse_GetParticipantUnk04(void *course, int index);
void *PokeathlonCourse_GetParticipantData(void *course, int index);
void ov96_021E6168(void *course, int index, int slot, MonInfo *info);
void ov96_021EEBF8(void *a, MonInfo *info, int b, int c, enum HeapID heapId, int d);
void GetMonSpriteCharAndPlttNarcIdsEx(PokepicTemplate *pokepicTemplate, u16 species, u8 gender, u8 whichFacing, u8 shiny, u8 form, u32 personality);
void ov96_021EAF60(void *a, u8 b, int c, void *data);
void Heap_Free(void *ptr);
void ov96_021EC790(void *course, int index);
// @fn ov96_021EC790
void ov96_021EC790(void *course, int index) {
    Heap *heap = PokeathlonCourse_GetHeapAllocPtr4(course);
    NNSG2dPaletteData *pltt;
    MonInfo info;
    PokepicTemplate tmpl;
    int i;
    void *data;
    PokeathlonCourse_GetParticipantUnk04(course, index);
    PokeathlonCourse_GetParticipantData(course, index);
    for (i = 0; i < 3; i++) {
        ov96_021E6168(course, index, i, &info);
        ov96_021EEBF8(heap->x74[i], &info, 1, 0, heap->heapId, 0);
        GetMonSpriteCharAndPlttNarcIdsEx(&tmpl, info.species, info.gender, 2, info.shiny, info.form, info.personality);
        data = GfGfxLoader_GetPlttData((NarcId)tmpl.narcID, tmpl.palDataID, &pltt, heap->heapId);
        ov96_021EAF60(heap->x14, i * 3 + 1, 1, pltt->pRawData);
        Heap_Free(data);
    }
}
