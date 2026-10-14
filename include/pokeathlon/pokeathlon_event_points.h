#ifndef POKEHEARTGOLD_POKEATHLON_POKEATHLON_EVENT_POINTS_H
#define POKEHEARTGOLD_POKEATHLON_POKEATHLON_EVENT_POINTS_H

#include "global.h"

#include "pokeathlon/pokeathlon_course_types_def.h"

// The course points of an event (pokeathlon_event_points.c, pokeathlon_event_rank_points.c).

// The part of a course ranking entry (PokeathlonCourseScoreList_GetRanked) the points are made from.
typedef struct PokeathlonEventPointsEntry {
    u8 unk00[9]; // 0x00
    u8 rank;     // 0x09
    u16 points;  // 0x0A
    u32 score;   // 0x0C: the event score
} PokeathlonEventPointsEntry;

// pokeathlon_event_points.c
void PokeathlonEvent_CalcPoints(void *list, int event);
void PokeathlonEvent_CalcPointsFromScores(void *list, int event);
void PokeathlonEvent_CalcPointsFromRanks(void *list, int event);
float PokeathlonEvent_Saturate(u32 score, float rate);
u8 PokeathlonEvent_ClampPoints(float points);
u8 PokeathlonEvent_HurdleDashPoints(u32 frames);
u8 PokeathlonEvent_PennantCapturePoints(int score);
u8 PokeathlonEvent_CirclePushPoints(int score);
u8 PokeathlonEvent_BlockSmashPoints(u32 score);
u8 PokeathlonEvent_DiscCatchPoints(float score);
u8 PokeathlonEvent_LampJumpPoints(u32 score);
u8 PokeathlonEvent_RelayRunPoints(u32 score);
u8 PokeathlonEvent_RingDropPoints(u32 score);
u8 PokeathlonEvent_SnowThrowPoints(int score);

// pokeathlon_event_rank_points.c
void PokeathlonEvent_CalcRankPoints(PokeathlonCourseScoreList *list);

#endif // POKEHEARTGOLD_POKEATHLON_POKEATHLON_EVENT_POINTS_H
