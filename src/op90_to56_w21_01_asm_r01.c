#pragma force_active on
#include "global.h"
typedef struct PartyOrder {
    u8 slots[6];
} PartyOrder;
typedef struct PartySlotInfo {
    u8 unk00[0x1B];
    u8 unk1B_0 : 7;
    u8 isEgg : 1;
    u8 unk1C[0x34];
} PartySlotInfo;
typedef struct Ov08Work {
    PartySlotInfo slots[6];
} Ov08Work;
extern const PartyOrder sBattlePartyMultiBattleOrder;
BOOL BattlePartyMenu_IsMultiBattle(Ov08Work *work);
BOOL BattlePartyMenu_GetSlotType(Ov08Work *work, int slot);
u8 BattlePartyMenu_FindNextMon(Ov08Work *work, int pos, int dir);

// @fn BattlePartyMenu_FindNextMon
u8 BattlePartyMenu_FindNextMon(Ov08Work *work, int pos, int dir) {
    int cur = pos;
    if (BattlePartyMenu_IsMultiBattle(work) == TRUE) {
        PartyOrder order = sBattlePartyMultiBattleOrder;
        for (pos = 0; pos < 6; pos++) {
            if (cur == order.slots[pos]) {
                break;
            }
        }
        while (TRUE) {
            pos += dir;
            if (pos < 0) {
                pos = 5;
            } else if (pos >= 6) {
                pos = 0;
            }
            if (cur == order.slots[pos]) {
                break;
            }
            if (BattlePartyMenu_GetSlotType(work, order.slots[pos]) && !work->slots[order.slots[pos]].isEgg) {
                return order.slots[pos];
            }
        }
    } else {
        while (TRUE) {
            pos += dir;
            if (pos < 0) {
                pos = 5;
            } else if (pos >= 6) {
                pos = 0;
            }
            if (cur == pos) {
                break;
            }
            if (BattlePartyMenu_GetSlotType(work, pos) && !work->slots[pos].isEgg) {
                return pos;
            }
        }
    }
    return 0xFF;
}
