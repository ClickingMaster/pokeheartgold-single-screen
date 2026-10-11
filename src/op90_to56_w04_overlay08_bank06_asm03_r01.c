#pragma force_active on
#include "global.h"
#include "party.h"
#include "pokemon.h"
#include "move.h"
typedef struct {
    u16 move;
    u8 pp;
    u8 maxPP;
    u8 type;
    u8 power;
    u8 category;
    u8 accuracy;
} MoveSummary;
typedef struct {
    Pokemon *mon;
    u16 species;
    u16 stat6;
    u16 stat8;
    u16 statA;
    u16 statC;
    u16 statE;
    u16 hp;
    u16 maxHp;
    u8 type1;
    u8 type2;
    u8 level : 7;
    u8 noOT : 1;
    u8 gender : 3;
    u8 status : 4;
    u8 isEgg : 1;
    u16 item;
    u16 unk1A;
    u32 exp;
    u32 levelExp;
    u32 nextLevelExp;
    u8 contest[7];
    u8 pad2F;
    MoveSummary moves[4];
} MonSummary;
typedef struct {
    Party *party;
} SummaryCtx;
typedef struct {
    SummaryCtx *ctx;
    MonSummary mons[6];
} SummaryWork;
u32 Pokemon_GetStatusIconId(Pokemon *mon);
void BattlePartyMenu_LoadMonData(SummaryWork *w);
// @fn BattlePartyMenu_LoadMonData
void BattlePartyMenu_LoadMonData(SummaryWork *w) {
    u16 i, j;
    for (i = 0; i < Party_GetCount(w->ctx->party); i++) {
        w->mons[i].mon = Party_GetMonByIndex(w->ctx->party, i);
        w->mons[i].species = GetMonData(w->mons[i].mon, MON_DATA_SPECIES, NULL);
        if (w->mons[i].species == 0) {
            continue;
        }
        w->mons[i].stat6 = GetMonData(w->mons[i].mon, 165, NULL);
        w->mons[i].stat8 = GetMonData(w->mons[i].mon, 166, NULL);
        w->mons[i].statA = GetMonData(w->mons[i].mon, 167, NULL);
        w->mons[i].statC = GetMonData(w->mons[i].mon, 168, NULL);
        w->mons[i].statE = GetMonData(w->mons[i].mon, 169, NULL);
        w->mons[i].hp = GetMonData(w->mons[i].mon, MON_DATA_HP, NULL);
        w->mons[i].maxHp = GetMonData(w->mons[i].mon, MON_DATA_MAX_HP, NULL);
        w->mons[i].type1 = GetMonData(w->mons[i].mon, 177, NULL);
        w->mons[i].type2 = GetMonData(w->mons[i].mon, 178, NULL);
        w->mons[i].level = GetMonData(w->mons[i].mon, MON_DATA_LEVEL, NULL);
        if (GetMonData(w->mons[i].mon, 176, NULL) == 1) {
            w->mons[i].noOT = 0;
        } else {
            w->mons[i].noOT = 1;
        }
        w->mons[i].gender = GetMonGender(w->mons[i].mon);
        w->mons[i].status = Pokemon_GetStatusIconId(w->mons[i].mon);
        w->mons[i].isEgg = GetMonData(w->mons[i].mon, MON_DATA_IS_EGG, NULL);
        w->mons[i].item = GetMonData(w->mons[i].mon, 10, NULL);
        w->mons[i].unk1A = GetMonData(w->mons[i].mon, MON_DATA_HELD_ITEM, NULL);
        w->mons[i].exp = GetMonData(w->mons[i].mon, MON_DATA_EXPERIENCE, NULL);
        w->mons[i].levelExp = GetMonExpBySpeciesAndLevel(w->mons[i].species, w->mons[i].level);
        if (w->mons[i].level == 100) {
            w->mons[i].nextLevelExp = w->mons[i].levelExp;
        } else {
            w->mons[i].nextLevelExp = GetMonExpBySpeciesAndLevel(w->mons[i].species, w->mons[i].level + 1);
        }
        w->mons[i].contest[0] = GetMonData(w->mons[i].mon, 19, NULL);
        w->mons[i].contest[1] = GetMonData(w->mons[i].mon, 20, NULL);
        w->mons[i].contest[2] = GetMonData(w->mons[i].mon, 21, NULL);
        w->mons[i].contest[3] = GetMonData(w->mons[i].mon, 22, NULL);
        w->mons[i].contest[4] = GetMonData(w->mons[i].mon, 23, NULL);
        w->mons[i].contest[5] = GetMonData(w->mons[i].mon, 162, NULL);
        w->mons[i].contest[6] = GetMonData(w->mons[i].mon, 112, NULL);
        for (j = 0; j < 4; j++) {
            MoveSummary *m = &w->mons[i].moves[j];
            m->move = GetMonData(w->mons[i].mon, MON_DATA_MOVE1 + j, NULL);
            if (m->move == 0) {
                continue;
            }
            m->pp = GetMonData(w->mons[i].mon, MON_DATA_MOVE1_PP + j, NULL);
            m->maxPP = GetMonData(w->mons[i].mon, MON_DATA_MOVE1_PP_UPS + j, NULL);
            m->maxPP = GetMoveMaxPP(m->move, m->maxPP);
            m->type = GetMoveAttr(m->move, MOVEATTR_TYPE);
            m->power = GetMoveAttr(m->move, MOVEATTR_CLASS);
            m->category = GetMoveAttr(m->move, MOVEATTR_ACCURACY);
            m->accuracy = GetMoveAttr(m->move, MOVEATTR_POWER);
        }
    }
}
