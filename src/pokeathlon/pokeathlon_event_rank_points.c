#include "global.h"

#include "pokeathlon/pokeathlon_event_points.h"

// Points by rank (100, 80, 70, 60); tied participants get the points of the lowest of their places. Every point
// of the event score adds 5. At most 200.
extern const u8 gPokeathlonEventRankPoints[4];

// PokeathlonCourseScoreList_GetRanked (pokeathlon_course_internal.h)
PokeathlonEventPointsEntry *PokeathlonCourseScoreList_GetRanked(void *list, int index);

#pragma force_active on

void PokeathlonEvent_CalcRankPoints(void *list) {
    u8 ranks[4];
    u8 ties[4];
    u8 table[4];
    u8 i, j;
    PokeathlonEventPointsEntry *entry;
    u16 points;
    u8 last;

    table[0] = gPokeathlonEventRankPoints[0];
    table[1] = gPokeathlonEventRankPoints[1];
    last = gPokeathlonEventRankPoints[3];
    table[2] = gPokeathlonEventRankPoints[2];
    table[3] = last;
    for (i = 0; i < 4; i++) {
        ranks[i] = PokeathlonCourseScoreList_GetRanked(list, i)->rank;
        ties[i] = 0;
    }
    for (i = 0; i < 4; i++) {
        u8 rank = ranks[i];
        entry = PokeathlonCourseScoreList_GetRanked(list, i);
        for (j = 0; j < 4; j++) {
            if (i != j && rank == ranks[j]) {
                ties[i]++;
            }
        }
        points = table[rank + ties[i]] + entry->score * 5;
        if (points > 200.0) {
            points = 200;
        }
        entry->points = points;
    }
}

#pragma force_active reset
