#include "global.h"

#include "msgdata/msg.naix"
#include "pokeathlon/pokeathlon.h"
#include "pokeathlon/pokeathlon_course_internal.h"

#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "player_data.h"
#include "pm_string.h"

void *PokeathlonCourseScoreList_New(enum HeapID heapId) {
    PokeathlonCourseScoreList *list = Heap_Alloc(heapId, sizeof(PokeathlonCourseScoreList));
    PokeathlonCourseScoreList *entryBase;
    int i;

    MI_CpuFill8(list, 0, sizeof(PokeathlonCourseScoreList));
    // entryBase: the list pointer moved on by one entry per step, entries[0] is the current one (needed to match)
    entryBase = list;
    for (i = 0; i < POKEATHLON_COURSE_SCORE_ENTRY_MAX; i++, entryBase = (PokeathlonCourseScoreList *)((u8 *)entryBase + sizeof(PokeathlonCourseScore))) {
        entryBase->entries[0].text = String_New(37, heapId);
        entryBase->entries[0].name = String_New(11, heapId);
    }
    return list;
}

void PokeathlonCourseScoreList_Free(void *ptr) {
    int i = 0;
    // entryBase: as in PokeathlonCourseScoreList_New
    PokeathlonCourseScoreList *entryBase = ptr;

    do {
        String_Delete(entryBase->entries[0].text);
        String_Delete(entryBase->entries[0].name);
        i++;
        entryBase = (PokeathlonCourseScoreList *)((u8 *)entryBase + sizeof(PokeathlonCourseScore));
    } while (i < POKEATHLON_COURSE_SCORE_ENTRY_MAX);
    Heap_Free(ptr);
}

PokeathlonCourseScore *PokeathlonCourseScoreList_AddEntry(PokeathlonCourseScoreList *list) {
    if (list->next >= 4) {
        GF_AssertFail();
    }
    if (list->next >= list->count) {
        GF_AssertFail();
    }
    return &list->entries[list->next++];
}

PokeathlonCourseScore *PokeathlonCourseScoreList_GetEntry(PokeathlonCourseScoreList *list, u32 index) {
    if (index >= 4) {
        GF_AssertFail();
    }
    if ((s32)index >= list->count) {
        GF_AssertFail();
    }
    return &list->entries[index];
}

void PokeathlonCourseScoreList_Reset(PokeathlonCourseScoreList *list) {
    int i;
    list->count = 0;
    list->unk04 = 0;
    list->next = 0;
    list->sortOrder = 0;
    list->totalsAdded = FALSE;
    for (i = 0; i < POKEATHLON_COURSE_SCORE_ENTRY_MAX; i++) {
        list->ranking[i] = NULL;
    }
}

#pragma force_active on

// Sorts the entries by score (descending for mode 0/1, ascending for mode 2; ties go to the lower participant)
// into ranking[] and gives each entry its rank (tied entries share one).
void PokeathlonCourseScoreList_Rank(PokeathlonCourseScoreList *list, int mode) {
    int order;
    PokeathlonCourseScoreNode nodes[POKEATHLON_COURSE_SCORE_ENTRY_MAX + 1];
    int i;
    int j;
    PokeathlonCourseScoreNode *node;
    PokeathlonCourseScoreNode *cur;
    PokeathlonCourseScoreNode *p;
    int prevScore;
    int rank;
    int tie;
    nodes[0].next = nodes[0].prev = &nodes[1];
    nodes[0].entry = NULL;
    switch (mode) {
    case 0:
        order = 1;
        break;
    case 1:
        order = 1;
        break;
    case 2:
        order = 2;
        break;
    default:
        GF_AssertFail();
        break;
    }
    list->sortOrder = order;
    for (i = 0; i < list->count; i++) {
        node = &nodes[i + 1];
        node->entry = &list->entries[i];
        node->next = NULL;
        node->prev = NULL;
        cur = &nodes[0];
        for (j = 0; j < i; j++) {
            cur = cur->next;
            if (order == 1) {
                if (cur->entry->score < node->entry->score) {
                    cur->prev->next = node;
                    node->prev = cur->prev;
                    node->next = cur;
                    cur->prev = node;
                    break;
                } else if (cur->entry->score == node->entry->score && cur->entry->participant > node->entry->participant) {
                    cur->prev->next = node;
                    node->prev = cur->prev;
                    node->next = cur;
                    cur->prev = node;
                }
            } else {
                if (cur->entry->score > node->entry->score) {
                    cur->prev->next = node;
                    node->prev = cur->prev;
                    node->next = cur;
                    cur->prev = node;
                    break;
                } else if (cur->entry->score == node->entry->score && cur->entry->participant > node->entry->participant) {
                    cur->prev->next = node;
                    node->prev = cur->prev;
                    node->next = cur;
                    cur->prev = node;
                }
            }
        }
        if (j == i) {
            cur->next = node;
            node->prev = cur;
        }
    }
    p = &nodes[0];
    prevScore = -1;
    tie = 0;
    rank = -1;
    for (i = 0; i < list->count; i++) {
        p = p->next;
        list->ranking[i] = p->entry;
        if (prevScore != p->entry->score) {
            prevScore = p->entry->score;
            if (tie) {
                rank = i;
                tie = 0;
            } else {
                rank++;
            }
        } else {
            tie = 1;
        }
        list->ranking[i]->rank = rank;
    }
}

#pragma force_active reset

PokeathlonCourseScore *PokeathlonCourseScoreList_GetRanked(PokeathlonCourseScoreList *list, u32 index) {
    if (index >= 4) {
        GF_AssertFail();
    }
    if ((s32)index >= list->count) {
        GF_AssertFail();
    }
    return list->ranking[index];
}

void PokeathlonCourseScoreList_Init(PokeathlonCourseScoreList *list) {
    PokeathlonCourseScoreList_Reset(list);
    list->count = 4;
    list->unk04 = 0;
}

s32 PokeathlonCourseScoreList_GetCount(PokeathlonCourseScoreList *list) {
    return list->count;
}

u32 ov96_021E9528(PokeathlonCourseScoreList *list) {
    return list->unk04;
}

void PokeathlonCourseScoreList_AddPointsToTotals(PokeathlonCourseScoreList *list) {
    int i;
    if (list->totalsAdded != FALSE) {
        GF_AssertFail();
    }
    if (list->totalsAdded == FALSE) {
        for (i = 0; i < list->count; i++) {
            list->totals[list->ranking[i]->participant] += list->ranking[i]->points;
        }
        list->totalsAdded = TRUE;
    }
}

void PokeathlonCourseScoreList_SaveEventPoints(PokeathlonCourseScoreList *list, int event) {
    int i;
    u32 points[4] = { 0, 0, 0, 0 };
    if (event >= POKEATHLON_COURSE_EVENT_POINTS_MAX) {
        GF_AssertFail();
    }
    for (i = 0; i < list->count; i++) {
        points[list->ranking[i]->participant] += list->ranking[i]->points;
    }
    for (i = 0; i < 4; i++) {
        list->eventPoints[event][i] = points[i];
    }
}

u32 PokeathlonCourseScoreList_GetEventPoints(PokeathlonCourseScoreList *list, int event, int participant) {
    if (event >= POKEATHLON_COURSE_EVENT_POINTS_MAX) {
        GF_AssertFail();
    }
    return list->eventPoints[event][participant];
}

// Adds the next score entry and prints its score text (msg 309 #msgno with the score in placeholder idx).
void PokeathlonCourse_AddScoreEntry(PokeathlonCourseData *course, PokeathlonCourseScoreList *list, u32 participant, u32 score, u8 idx, u8 digits, u32 msgno) {
    enum HeapID heapId = PokeathlonCourse_GetHeapID(course);
    MsgData *msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0309_bin, heapId);
    PokeathlonCourseScore *entry = PokeathlonCourseScoreList_AddEntry(list);
    MessageFormat *msgFormat;
    String *string;
    entry->participant = participant;
    entry->score = score;
    msgFormat = MessageFormat_New(heapId);
    BufferIntegerAsString(msgFormat, idx, score, digits, PRINTING_MODE_RIGHT_ALIGN, TRUE);
    string = ReadMsgData_ExpandPlaceholders(msgFormat, msgData, msgno, heapId);
    MessageFormat_Delete(msgFormat);
    String_Copy(entry->text, string);
    String_Delete(string);
    DestroyMsgData(msgData);
}

// As PokeathlonCourse_AddScoreEntry, with two numbers in the text.
void PokeathlonCourse_AddScoreEntry2(PokeathlonCourseData *course, PokeathlonCourseScoreList *list, u32 participant, u32 score, u32 value1, u32 value2, u8 idx1, u8 idx2, u8 digits1, u8 digits2, u32 msgno) {
    enum HeapID heapId = PokeathlonCourse_GetHeapID(course);
    MsgData *msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0309_bin, heapId);
    PokeathlonCourseScore *entry = PokeathlonCourseScoreList_AddEntry(list);
    MessageFormat *msgFormat;
    String *string;
    entry->participant = participant;
    entry->score = score;
    msgFormat = MessageFormat_New(heapId);
    BufferIntegerAsString(msgFormat, idx1, value1, digits1, PRINTING_MODE_RIGHT_ALIGN, TRUE);
    BufferIntegerAsString(msgFormat, idx2, value2, digits2, PRINTING_MODE_RIGHT_ALIGN, TRUE);
    string = ReadMsgData_ExpandPlaceholders(msgFormat, msgData, msgno, heapId);
    MessageFormat_Delete(msgFormat);
    String_Copy(entry->text, string);
    String_Delete(string);
    DestroyMsgData(msgData);
}

void PokeathlonCourseCmd_RecvEvents(int netId, int size, void *data, void *context) {
    PokeathlonCourseData *course = context;
    PokeathlonCourse_SetEvents(course, data);
    PokeathlonCourse_SetStateTransitionType(course, 1);
    PokeathlonCourse_SetStateField07(course, 1);
}

#pragma force_active on

// Host: collects every player's participant entry; once all arrived, fills the free slots with NPC teams.
void PokeathlonCourseCmd_RecvParticipant(int netId, int size, void *data, void *context) {
    int count;
    if (PokeathlonCourse_GetNetId(context) == 0) {
        memcpy(PokeathlonCourse_GetParticipantData(context, netId), data, size);
        PokeathlonCourse_IncrementField1EF(context);
        count = PokeathlonCourse_GetParticipantCount(context);
        if (count == PokeathlonCourse_GetField1EF(context)) {
            if ((u32)PokeathlonCourse_GetField1EF(context) < 4) {
                u32 missing = 4 - PokeathlonCourse_GetField1EF(context);
                PokeathlonCourse_SetupNpcParticipants(context, (u8)missing);
            }
            PokeathlonCourse_ResetField1EF(context);
            PokeathlonCourse_SetStateField07(context, 4);
        }
    }
}

#pragma force_active reset

void PokeathlonCourseCmd_RecvAllParticipants(int netId, int size, void *data, void *context) {
    PokeathlonCourseData *course = context;
    memcpy(PokeathlonCourse_GetParticipantData(course, netId), data, size);
    PokeathlonCourse_LoadPerformanceData(course);
    PokeathlonCourse_SetStateTransitionType(course, 3);
    PokeathlonCourse_SetStateField07(course, 5);
}

void PokeathlonCourseCmd_RecvProfiles(int netId, int size, void *data, void *context) {
    PokeathlonCourseData *course = context;
    if (PokeathlonCourse_GetNetId(course)) {
        memcpy(PokeathlonCourse_GetPlayerProfileFromData(course, 0), data, size);
    }
    PokeathlonCourse_SetStateTransitionType(course, 4);
    PokeathlonCourse_SetStateField07(course, 6);
}

void PokeathlonCourseCmd_RecvRecord(int netId, int size, void *data, void *context) {
    PokeathlonCourseData *course = context;
    PokeathlonCourse_IncrementField1EF(course);
    if (PokeathlonCourse_GetParticipantCount(course) == PokeathlonCourse_GetField1EF(course)) {
        PokeathlonCourse_ResetField1EF(course);
        PokeathlonCourse_SetStateTransitionType(course, 5);
        PokeathlonCourse_SetStateField07(course, 7);
    }
}

void PokeathlonCourseCmd_RecvPacket(int netId, int size, void *data, void *context) {
    PokeathlonCourseData *course = context;
    u8 *dataCopy = PokeathlonCourse_GetDataCopyArea(course);
    if (((PokeathlonCoursePacket *)data)->isHostPacket) {
        memcpy(dataCopy + 6 * sizeof(PokeathlonCoursePacket), data, size);
    } else {
        memcpy(dataCopy + 2 * sizeof(PokeathlonCoursePacket) + netId * sizeof(PokeathlonCoursePacket), data, size);
    }
}

void PokeathlonCourseCmd_RecvEventData(int netId, int size, void *data, void *context) {
    PokeathlonCourseData *course = context;
    PokeathlonCourse_SetStateTransitionType(course, 10);
    PokeathlonCourse_SetStateField07(course, 18);
}

void PokeathlonCourseCmd_RecvEventResult(int netId, int size, void *data, void *context) {
    PokeathlonCourseData *course = context;
    u8 *dataCopy = PokeathlonCourse_GetDataCopyArea(course);
    if (!PokeathlonCourse_GetNetId(course)) {
        u32 resultSize = PokeathlonCourse_GetEventResultSize(course);
        memcpy(PokeathlonCourse_ToBytes(dataCopy + sizeof(PokeathlonCoursePacket)) + netId * resultSize, data, resultSize);
        PokeathlonCourse_IncrementField1EF(course);
        if (PokeathlonCourse_GetParticipantCount(course) == PokeathlonCourse_GetField1EF(course)) {
            PokeathlonCourse_ResetField1EF(course);
            PokeathlonCourse_SetStateField07(course, 21);
        }
    }
}

void PokeathlonCourseCmd_RecvEventResults(int netId, int size, void *data, void *context) {
    PokeathlonCourseData *course = context;
    memcpy(PokeathlonCourse_GetDataCopyArea(course) + 6 * sizeof(PokeathlonCoursePacket), data, size);
    PokeathlonCourse_SetStateField07(course, 22);
}

// Host: adds a player's per-mon event counters to its counters in the course results.
void PokeathlonCourseCmd_RecvMonCounters(int netId, int size, void *data, void *context) {
    PokeathlonCourseData *course = context;
    u32 *src = data;
    u32 *dst;
    u8 i;
    PokeathlonCourse_GetDataCopyArea(course);
    if (!PokeathlonCourse_GetNetId(course)) {
        dst = (u32 *)PokeathlonCourse_GetFieldData_AtIndex(course, (u8)netId);
        for (i = 0; i < 3; i++) {
            dst[i * 8 + 0] += src[i * 8 + 0];
            dst[i * 8 + 1] += src[i * 8 + 1];
            dst[i * 8 + 2] += src[i * 8 + 2];
            dst[i * 8 + 3] += src[i * 8 + 3];
            dst[i * 8 + 4] += src[i * 8 + 4];
            dst[i * 8 + 5] += src[i * 8 + 5];
            dst[i * 8 + 6] += src[i * 8 + 6];
            dst[i * 8 + 7] += src[i * 8 + 7];
        }
        PokeathlonCourse_IncrementField1EF(course);
        if (PokeathlonCourse_GetParticipantCount(course) == PokeathlonCourse_GetField1EF(course)) {
            PokeathlonCourse_ResetField1EF(course);
            PokeathlonCourse_SetStateField07(course, 30);
        }
    }
}

void PokeathlonCourseCmd_RecvResults(int netId, int size, void *data, void *context) {
    PokeathlonCourseData *course = context;
    PokeathlonCourse_GetDataCopyArea(course);
    PokeathlonCourse_GetNetId(course);
    PokeathlonCourse_SetStateTransitionType(course, 13);
    PokeathlonCourse_SetStateField07(course, 31);
}

void PokeathlonCourseCmd_RecvJuiceRecord(int netId, int size, void *data, void *context) {
    PokeathlonCourseData *course = context;
    PokeathlonCourse_GetDataCopyArea(course);
    PokeathlonCourse_IncrementField1EF(course);
    if (PokeathlonCourse_GetParticipantCount(course) == PokeathlonCourse_GetField1EF(course)) {
        PokeathlonCourse_ResetField1EF(course);
        PokeathlonCourse_SetStateTransitionType(course, 15);
        PokeathlonCourse_SetStateField07(course, 36);
    }
}

int PokeathlonCourseCmd_EventsSize(void) {
    return 4;
}

int PokeathlonCourseCmd_ParticipantSize(void) {
    return sizeof(PokeathlonCourseParticipant);
}

int PokeathlonCourseCmd_AllParticipantsSize(void) {
    return sizeof(PokeathlonCourseParticipant) * 4;
}

u32 PokeathlonCourseCmd_ProfilesSize(void) {
    return PlayerProfile_sizeof() * 4;
}

int PokeathlonCourseCmd_RecordSize(void) {
    return sizeof(PokeathlonStatistics);
}

// pokeathlon.h declares this size as a pointer, as PokeathlonCourseLink_SendCommand takes it
void *PokeathlonCourseCmd_PacketSize(void) {
    return (void *)0x26;
}

int PokeathlonCourseCmd_MonCountersSize(void) {
    return 0x60;
}

int PokeathlonCourseCmd_ResultsSize(void) {
    return 0x180;
}

u32 PokeathlonCourseCmd_JuiceRecordSize(void) {
    return ApricornJuiceRecord_sizeof();
}

void *PokeathlonCourseCmd_GetEventDataBuffer(int netId, void *context) {
    return (void *)PokeathlonCourse_GetField3A4(context);
}

void *PokeathlonCourseCmd_GetRecordBuffer(int netId, void *context) {
    return PokeathlonCourse_GetField974_AtIndex(context, netId);
}

void *PokeathlonCourseCmd_GetResultsBuffer(int netId, void *context) {
    return PokeathlonCourse_GetFieldData_AtIndex(context, 0);
}

void *PokeathlonCourseCmd_GetMonCountersBuffer(int netId, void *context, int size) {
    return (u8 *)PokeathlonCourse_GetFieldBA4(context) + netId * size;
}

void *PokeathlonCourseCmd_GetJuiceRecordBuffer(int netId, void *context) {
    return PokeathlonCourse_GetJuiceRecord(context, (u8)netId);
}
