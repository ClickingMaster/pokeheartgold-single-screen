#pragma force_active on
#include "global.h"
typedef struct Record {
    u8 data[0x48];
} Record;
void *ov73_021E7D54(int index, Record **players, int a2);
int ov73_021E7CD8(Record *records, int count, Record *record, int idx);
void SaveSubstruct_UpdateCRC(int id);
void ov73_021E7D74(Record *records, int count, int numPlayers, int myId, Record **players, int a5, int a6);

// @fn ov73_021E7D74
void ov73_021E7D74(Record *records, int count, int numPlayers, int myId, Record **players, int a5, int a6) {
    u8 given[16];
    int j;
    int i;
    int k;
    Record **list;
    MI_CpuFill8(given, 0, sizeof(given));
    j = 0;
    for (i = 0; i < numPlayers; i++) {
        if (players[i] == NULL) {
            continue;
        }
        for (k = 0; k < 4; k++) {
            while (players[j] == NULL || j == i || given[j] == 4) {
                j = (j + 1) % numPlayers;
            }
            given[j]++;
            if (j != myId) {
                list = ov73_021E7D54(j, players, a5);
                if (list != NULL) {
                    ov73_021E7CD8(records, count, &list[j][k], a6);
                }
            }
        }
    }
    SaveSubstruct_UpdateCRC(23);
}
