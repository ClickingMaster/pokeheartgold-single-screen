#include "overlay59_apricorn_result_apply_private.h"

void ov59_0223BAE8(Ov59R12Work *work) {
    int i;
    Ov59R12Entry *entry = &work->entries[work->selected];

    ov58_022379C0(&work->record, work->party, entry->slot);
    if (work->mode->mode == 8) {
        sub_02032024(work->box);
    }
    Party_GetMonAprijuiceModifiers(work->party, &entry->aprijuice, entry->slot);
    work->oldStars[0] = entry->stars.stars;
    work->oldStars[1] = *(u16 *)&entry->stars.color[0];
    work->oldStars[2] = *(u16 *)&entry->stars.color[2];
    work->oldStars[3] = *(u16 *)&entry->stars.color[4];
    CalcMonPokeathlonStars(&entry->stars, entry->mon, entry->aprijuice.unk_00, work->heap);
    work->changedCount = 0;
    for (i = 0; i < 5; i++) {
        work->starDelta[i] = (u8)((entry->stars.stars >> (ov59_0223C99C[i] * 3)) & 7) - (u8)((work->oldStars[0] >> (ov59_0223C99C[i] * 3)) & 7);
        if (work->starDelta[i] != 0) {
            work->changedCount++;
        }
    }
    work->ready = 1;
}
