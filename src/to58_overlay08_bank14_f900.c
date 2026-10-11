#include "../include/to57_overlay08_bank14_private.h"
extern void BattlePartyMenu_PrintMonName(void *, int, int, int, int, int);
extern void BattlePartyMenu_PrintHPStat(void *, int);
extern void BattlePartyMenu_PrintLevelAndNextExp(void *, int);
extern void BattlePartyMenu_PrintAttack(void *, int);
extern void BattlePartyMenu_PrintDefense(void *, int);
extern void BattlePartyMenu_PrintSpeed(void *, int);
extern void BattlePartyMenu_PrintSpAttack(void *, int);
extern void BattlePartyMenu_PrintSpDefense(void *, int);
extern void BattlePartyMenu_PrintAbilityName(void *, int, int);
extern void BattlePartyMenu_PrintHeldItemName(void *, int, int);
extern void BattlePartyMenu_PrintAbilityDescription(void *, int);
typedef struct Work { u8 pad[0x2070]; Window *windows; u8 count; u8 page; } Work;
#define CLEAR(n) FillWindowPixelBuffer(&w->windows[(n)], 0)

void BattlePartyMenu_PrintSummaryPage(void *arg) {
    Work *w = arg;
    u32 base = (u32)w->page * 22;
    CLEAR(0x13);
    CLEAR(0x14);
    CLEAR(0xE);
    CLEAR(0xF);
    CLEAR(0x10);
    CLEAR(0x11);
    CLEAR(0x12);
    CLEAR(0xD);
    CLEAR(0x15);
    CLEAR(base);
    CLEAR(base + 10);
    CLEAR(base + 11);
    CLEAR(base + 12);
    CLEAR(base + 5);
    CLEAR(base + 6);
    CLEAR(base + 7);
    CLEAR(base + 8);
    CLEAR(base + 9);
    CLEAR(base + 4);
    CLEAR(base + 1);
    CLEAR(base + 2);
    CLEAR(base + 3);
    BattlePartyMenu_PrintMonName(w, base, 0, *((u8 *)*(void **)w + 0x11), 0, 0);
    BattlePartyMenu_PrintHPStat(w, *((u8 *)*(void **)w + 0x11));
    BattlePartyMenu_DrawHPBar(w, base + 10, *((u8 *)*(void **)w + 0x11), 0, 0);
    BattlePartyMenu_PrintLevelAndNextExp(w, *((u8 *)*(void **)w + 0x11));
    BattlePartyMenu_PrintAttack(w, *((u8 *)*(void **)w + 0x11));
    BattlePartyMenu_PrintDefense(w, *((u8 *)*(void **)w + 0x11));
    BattlePartyMenu_PrintSpeed(w, *((u8 *)*(void **)w + 0x11));
    BattlePartyMenu_PrintSpAttack(w, *((u8 *)*(void **)w + 0x11));
    BattlePartyMenu_PrintSpDefense(w, *((u8 *)*(void **)w + 0x11));
    BattlePartyMenu_PrintAbilityName(w, base + 1, *((u8 *)*(void **)w + 0x11));
    BattlePartyMenu_PrintHeldItemName(w, base + 3, *((u8 *)*(void **)w + 0x11));
    BattlePartyMenu_PrintAbilityDescription(w, *((u8 *)*(void **)w + 0x11));
    BattlePartyMenu_PrintButtonLabel(w, 21, 19);
    w->page ^= 1;
}
