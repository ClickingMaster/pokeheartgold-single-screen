#pragma force_active on
#include "Z:/home/art/projects/pokeheartgold/.local-tools/to90/opus/r12/10c4/hdr.h"
#include "party.h"
#include "pokemon.h"
#include "save_vars_flags.h"
#include "sys_vars.h"
extern void *Save_Frontier_GetHallChallenge(void *save);
extern void *Save_Frontier_GetHallFlags(void *save);
extern void HallChallenge_Clear(void *rec);
extern u32 HallFlags_GetField(void *aux, int a, u8 kind, int c, int d);
extern u32 HallChallenge_GetField(void *rec, int a, u8 b, int c, int d);
extern u32 sub_0205C0CC(u8 kind);
extern u8 ov80_0223787C(u8 kind);
extern void ov80_02231930(void *save, u8 kind, u8 i, int d);
extern BOOL ov80_0223792C(u8 kind);
extern void ov80_0222A840(void *save);
TrHouse *ov80_022310C4(void *save, u32 resume, u8 kind, u8 a3, u8 a4);
// @fn ov80_022310C4
TrHouse *ov80_022310C4(void *save, u32 resume, u8 kind, u8 a3, u8 a4) {
    TrHouse *s;
    TrHouse *s0;
    TrHouse *s1;
    TrHouse *s2;
    TrHouse *s5;
    TrHouse *s6;
    void *rec;
    void *aux;
    u8 n;
    u16 i;
    u8 flag;
    void *frontier;
    u32 stat;
    u16 out1;
    u16 out2;
    static TrHouse *ov80_0223DD44;

    ov80_0223DD44 = Heap_Alloc(HEAP_ID_FIELD2, sizeof(TrHouse));
    MI_CpuFill8(ov80_0223DD44, 0, sizeof(TrHouse));
    s0 = ov80_0223DD44;
    s0->unk6F8 = Save_Frontier_GetHallChallenge(save);
    s0->save = save;
    s0->heapId = HEAP_ID_FIELD2;
    s1 = ov80_0223DD44;
    s1->party = SaveArray_Party_Alloc(HEAP_ID_FIELD2);
    s1->unkD8C = AllocMonZeroed(HEAP_ID_FIELD2);
    rec = s1->unk6F8;
    aux = Save_Frontier_GetHallFlags(save);
    if (resume == 0) {
        s2 = ov80_0223DD44;
        s2->kind = kind;
        n = ov80_0223787C(s2->kind);
        ov80_0223DD44->idx = 0;
        HallChallenge_Clear(rec);
        if (ov80_0223DD44->kind == 3) {
            flag = Save_VarsFlags_GetVar4052(Save_VarsFlags_Get((SaveData *)ov80_0223DD44->save));
        } else {
            flag = HallFlags_GetField(aux, 5, ov80_0223DD44->kind, 0, 0);
        }
        if (flag == 1) {
            ov80_0223DD44->unk8 = FrontierSave_GetStat(Save_Frontier_GetStatic(ov80_0223DD44->save), sub_0205C0CC(ov80_0223DD44->kind), sub_0205C268(sub_0205C0CC(ov80_0223DD44->kind)));
        } else {
            ov80_0223DD44->unk8 = 0;
            for (i = 0; i < 18; i++) {
                ov80_02231930(ov80_0223DD44->save, ov80_0223DD44->kind, i, 0);
            }
        }
        s = ov80_0223DD44;
        s->unk260[0] = a3;
        s->unk260[1] = a4;
    } else {
        s = ov80_0223DD44;
        s->kind = HallChallenge_GetField(rec, 0, 0, 0, 0);
        n = ov80_0223787C(s->kind);
        s5 = ov80_0223DD44;
        s5->idx = HallChallenge_GetField(rec, 1, 0, 0, 0);
        frontier = Save_Frontier_GetStatic(ov80_0223DD44->save);
        stat = sub_0205C0CC(ov80_0223DD44->kind);
        ov80_0223DD44->unk8 = FrontierSave_GetStat(frontier, stat, sub_0205C268(sub_0205C0CC(s5->kind)));
        for (i = 0; i < n; i++) {
            ov80_0223DD44->unk260[i] = HallChallenge_GetField(rec, 3, i, 0, 0);
        }
        for (i = 0; i < 20; i++) {
            ov80_0223DD44->unk18[i] = HallChallenge_GetField(rec, 2, i, 0, 0);
        }
        for (i = 0; i < 20; i++) {
            s6 = ov80_0223DD44;
            s6->unk268[i] = (u8)HallChallenge_GetField(rec, 4, i, 0, 0);
        }
    }
    for (i = 0; i < n; i++) {
        Pokemon *mon;
        s = ov80_0223DD44;
        mon = Party_GetMonByIndex(SaveArray_Party_Get((SaveData *)s->save), s->unk260[i]);
        s = ov80_0223DD44;
        s->unk728[i] = GetMonData(mon, MON_DATA_HELD_ITEM, NULL);
    }
    s = ov80_0223DD44;
    s->unk10 = 0;
    s->unkA = s->unk8 / 10;
    if (s->kind == 2) {
        for (i = 0; i < 18; i++) {
            sub_02030BF4(i, (u8 *)ov80_0223DD44 + 0x716, 9);
        }
    } else {
        for (i = 0; i < 18; i++) {
            u16 v = ov80_022318D0(save, ov80_0223DD44->kind, i, &out1, &out2);
            sub_02030BF4(i, ov80_0223DD44->unk704[ov80_0223DD44->kind], v);
        }
    }
    if (ov80_0223792C(ov80_0223DD44->kind) == TRUE) {
        ov80_0222A840(ov80_0223DD44->save);
    }
    return ov80_0223DD44;
}
