#pragma force_active on
#include "ov01_21e90c0_opus_partial_internal.h"
#include "system.h"
const s8 *ov01_021EA578(const s8 *p, u16 *color, s16 *dir);
const s8 *ov01_021EA668(const s8 *p, u16 *out);
// @fn ov01_021EA3E0
int ov01_021EA3E0(const char *path, AreaLightEntry **entriesOut) {
    s8 line[256];
    s8 tok[256];
    const s8 *p;
    long i;
    const s8 *data;
    AreaLightEntry *entry;
    int size;
    int count;
    int j;

    data = Sys_AllocAndReadFile(HEAP_ID_FIELD1, path);
    p = data;
    count = 0;
    do {
        p = Ascii_GetDelim(p, line, '\r');
        if (line[0] != 'E' || line[1] != 'O' || line[2] != 'F') {
            p = Ascii_GetDelim(p, line, '\r');
            p = Ascii_GetDelim(p, line, '\r');
            p = Ascii_GetDelim(p, line, '\r');
            p = Ascii_GetDelim(p, line, '\r');
            p = Ascii_GetDelim(p, line, '\r');
            p = Ascii_GetDelim(p, line, '\r');
            p = Ascii_GetDelim(p, line, '\r');
            p = Ascii_GetDelim(p, line, '\r');
            p = Ascii_GetDelim(p, line, '\r');
            count++;
        }
    } while (line[0] != 'E' || line[1] != 'O' || line[2] != 'F');
    size = count * sizeof(AreaLightEntry);
    *entriesOut = Heap_Alloc(HEAP_ID_FIELD1, size);
    MI_CpuFill8(*entriesOut, 0, size);
    p = data;
    for (i = 0; i < count; i++) {
        entry = &(*entriesOut)[i];
        p = Ascii_GetDelim(p, line, '\r');
        Ascii_GetDelim(line, tok, ',');
        entry->endTime = Ascii_StrToL(tok);
        for (j = 0; j < 4; j++) {
            p = ov01_021EA578(p, &entry->color[j], entry->dir[j]);
            if (entry->color[j] != 0xFFFF) {
                entry->mask |= 1 << j;
            } else {
                entry->color[j] = 0;
            }
        }
        p = ov01_021EA668(p, &entry->unk26);
        p = ov01_021EA668(p, &entry->unk28);
        p = ov01_021EA668(p, &entry->unk2A);
        p = ov01_021EA668(p, &entry->unk2C);
        p = Ascii_GetDelim(p, line, '\r');
    }
    Heap_FreeExplicit(HEAP_ID_FIELD1, (void *)data);
    return count;
}
