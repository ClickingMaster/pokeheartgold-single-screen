#include "global.h"

#include "system.h"

typedef struct Ov112State {
    u8 raw[0x1f300];
} Ov112State;

#define W(p, o) (*(int *)((p)->raw + (o)))

int PokewalkerApp_HandleTwoChoiceMenu(Ov112State *);
void PokewalkerApp_HideCursor(Ov112State *);
void FillBgTilemapRect(void *, int, int, int, int, int, int, int);
void ClearWindowTilemapAndCopyToVram(void *);
int PokewalkerApp_StartMessageBox(Ov112State *, int, int);
void BgCommitTilemapBufferToVram(void *, int);
int TextPrinterCheckActive(u8);
void PokewalkerApp_PrintString(Ov112State *, int, void *, int, int);
void PokewalkerApp_LoadTilemap(Ov112State *, int, int);
void PokewalkerApp_PrintExit(Ov112State *, int);
void PokewalkerApp_SetCursorPosition(Ov112State *, int);
int PokewalkerApp_GetTouchedButton(int);
void ov112_021EC440(Ov112State *, int, int, int);
void PlaySE(int);

int ov112_021EBBD8(Ov112State *);
int ov112_021EBC3C(Ov112State *);
int ov112_021EBC7C(Ov112State *);
int ov112_021EBCD8(Ov112State *);
int ov112_021EBD9C(Ov112State *);
int ov112_021EBDE8(Ov112State *);
int ov112_021EBE4C(Ov112State *);

int ov112_021EBBD8(Ov112State *p) {
    switch (PokewalkerApp_HandleTwoChoiceMenu(p)) {
    case 0:
        PokewalkerApp_HideCursor(p);
        FillBgTilemapRect(*(void **)(p->raw + 0x18), 1, 0, 0, 16, 32, 8, 16);
        ClearWindowTilemapAndCopyToVram(p->raw + 0x1ea98);
        ClearWindowTilemapAndCopyToVram(p->raw + 0x1eaa8);
        return 3;
    case 1:
        W(p, 4) = 11;
        return 34;
    default:
        return 2;
    }
}

int ov112_021EBC3C(Ov112State *p) {
    W(p, 0x1e524) = PokewalkerApp_StartMessageBox(p, 0, 4);
    FillBgTilemapRect(*(void **)(p->raw + 0x18), 2, 0, 0, 0, 32, 24, 16);
    BgCommitTilemapBufferToVram(*(void **)(p->raw + 0x18), 2);
    return 4;
}

int ov112_021EBC7C(Ov112State *p) {
    if (!TextPrinterCheckActive((u8)W(p, 0x1e524))) {
        PokewalkerApp_PrintString(p, 5, *(void **)(p->raw + 0x1e458), 4, 0x000f0100);
        PokewalkerApp_LoadTilemap(p, 2, 10);
        PokewalkerApp_PrintExit(p, 1);
        PokewalkerApp_SetCursorPosition(p, 2);
        return 5;
    }
    return 4;
}

int ov112_021EBCD8(Ov112State *p) {
    int selection = PokewalkerApp_GetTouchedButton(1);
    if (selection != -1) {
        ov112_021EC440(p, selection, selection + 2, 6);
        PlaySE(1501);
        return 33;
    }
    if (gSystem.newKeys & 0x40) {
        if (W(p, 0x1ec50) != 0) {
            PlaySE(1500);
        }
        PokewalkerApp_SetCursorPosition(p, 2);
        W(p, 0x1ec50) = 0;
    } else if (gSystem.newKeys & 0x80) {
        if (W(p, 0x1ec50) != 1) {
            PlaySE(1500);
        }
        PokewalkerApp_SetCursorPosition(p, 3);
        W(p, 0x1ec50) = 1;
    } else if (gSystem.newKeys & 1) {
        PlaySE(1501);
        selection = W(p, 0x1ec50);
        ov112_021EC440(p, selection, selection + 2, 6);
        return 33;
    } else if (gSystem.newKeys & 2) {
        PlaySE(1500);
        ov112_021EC440(p, 1, 3, 6);
        return 33;
    }
    return 5;
}

int ov112_021EBD9C(Ov112State *p) {
    PokewalkerApp_HideCursor(p);
    ClearWindowTilemapAndCopyToVram(p->raw + 0x1eab8);
    FillBgTilemapRect(*(void **)(p->raw + 0x18), 1, 0, 0, 16, 32, 8, 16);
    if (W(p, 0x1ec50) == 0) {
        return 11;
    }
    W(p, 4) = 11;
    return 34;
}

int ov112_021EBDE8(Ov112State *p) {
    FillBgTilemapRect(*(void **)(p->raw + 0x18), 2, 0, 0, 0, 32, 24, 16);
    FillBgTilemapRect(*(void **)(p->raw + 0x18), 1, 0, 0, 0, 32, 24, 16);
    BgCommitTilemapBufferToVram(*(void **)(p->raw + 0x18), 1);
    BgCommitTilemapBufferToVram(*(void **)(p->raw + 0x18), 2);
    W(p, 0x1e524) = PokewalkerApp_StartMessageBox(p, 2, 9);
    return 8;
}
