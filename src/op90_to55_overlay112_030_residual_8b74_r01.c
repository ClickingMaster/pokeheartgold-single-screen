#pragma force_active on
#include "global.h"
#include "gf_rtc.h"
#include "math_util.h"
#include "pokemon.h"

typedef struct WalkerRouteMon {
    u16 species;
    u16 item;
    u16 level;
    u8 form;
    u8 gender;
    u16 moves[4];
    u16 steps;
    u16 chance;
} WalkerRouteMon;

typedef struct WalkerRoute {
    u8 header[8];
    WalkerRouteMon mons[6];
    u8 rest[0xC0 - 8 - 6 * sizeof(WalkerRouteMon)];
} WalkerRoute;

typedef struct WalkerMon {
    u16 species;
    u16 level;
    u16 moves[4];
    u8 item;
    u8 form : 5;
    u8 gender : 2;
    u8 flag : 1;
    u8 abilityBit : 1;
    u8 pad : 7;
    u8 padF;
} WalkerMon;

extern WalkerRoute ov112_021F4138[];
void ov112_021E8B74(WalkerMon *out, u16 *steps, u8 *chances, int route, u8 *chosen);

// @fn ov112_021E8B74
void ov112_021E8B74(WalkerMon *out, u16 *steps, u8 *chances, int route, u8 *chosen) {
    int i, k;
    WalkerRoute *r = &ov112_021F4138[route];
    BASE_STATS *personal;
    SetLCRNGSeed(GF_RTC_TimeToSec());
    for (i = 0; i < 3; i++) {
        int idx = i * 2 + LCRandom() % 2;
        WalkerRouteMon *mon;
        mon = &r->mons[idx];
        chosen[i] = idx;
        personal = AllocAndLoadMonPersonal_HandleAlternateForm(mon->species, mon->form, HEAP_ID_154);
        out[i].species = mon->species;
        out[i].level = mon->level;
        for (k = 0; k < 4; k++) {
            out[i].moves[k] = mon->moves[k];
        }
        out[i].item = mon->item;
        out[i].form = mon->form;
        out[i].gender = mon->gender;
        out[i].flag = 0;
        out[i].abilityBit = GetPersonalAttr(personal, 0x1C);
        steps[i] = mon->steps;
        chances[i] = mon->chance;
        FreeMonPersonal(personal);
    }
}
