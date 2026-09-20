#include "to56_r2_w18_battle_choice_cleanup_private.h"

#define PTR8(p, o)  (*(void **)((u8 *)(p) + (o)))
#define BYTE8(p, o) (*(u8 *)((u8 *)(p) + (o)))

void ov12_0225F8AC(SysTask *task, void *data) {
    u8 *work = data;
    int battler;
    int selected;
    Party *party;
    int partner;
    int i;
    u32 battleType = BattleSystem_GetBattleType(PTR8(work, 0));
    battler = work[9];

    if ((battleType & 0x10) != 0 || (battleType & 8) != 0) {
        partner = battler;
    } else {
        partner = BattleSystem_GetBattlerIdPartner(PTR8(work, 0), battler);
    }

    selected = ov12_02258BA0(PTR8(work, 0), battler);
    if (selected == 6) {
        selected = ov12_02258800(PTR8(work, 0), battler);
        if (selected == 6) {
            party = BattleSystem_GetParty(PTR8(work, 0), work[9]);
            for (i = 0; i < Party_GetCount(party); i++) {
                Pokemon *mon = BattleSystem_GetPartyMon(PTR8(work, 0), work[9], i);
                if (GetMonData(mon, 0xA3, NULL) != 0 && i != work[battler + 0xC] && i != work[partner + 0xC]) {
                    break;
                }
            }
            selected = i;
        }
    }
    ov12_02263360(PTR8(work, 0), work[9], selected + 1);
    ov12_0226430C(PTR8(work, 0), work[9], work[8]);
    Heap_Free(work);
    SysTask_Destroy(task);
}
