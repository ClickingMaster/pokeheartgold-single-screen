#pragma force_active on
#include "global.h"
typedef struct Ov87Pos {
    s16 x;
    s16 y;
} Ov87Pos;
typedef struct Ov87App {
    u8 pad0[0x34];
    void *x34;
    void *msgData;
    void *msgFmt;
    void *str1;
    void *str2;
    u8 pad48[0x58 - 0x48];
    void *x58;
    u8 x5C[0x160 - 0x5C];
    void *printer;
    u8 pad164[0x16C - 0x164];
    u8 renderer[0x2F8 - 0x16C];
    void *spritesA[4];
    void *spritesB[9];
    void *spritesC[4];
    u8 pad33C[0x35C - 0x33C];
    void *spriteD;
    u8 pad360[0x380 - 0x360];
    void *narc;
} Ov87App;
extern const Ov87Pos ov87_021E82E4[];
extern const Ov87Pos ov87_021E81A0[];
void *NARC_New(int, int);
void ov87_021E6B38(Ov87App *);
void ov87_021E6BA8(Ov87App *);
void *NewMsgDataFromNarc(int, int, int, int);
void *MessageFormat_New(int);
void *String_New(int, int);
void LoadFontPal0(int, int, int);
void LoadFontPal1(int, int, int);
void FontID_Alloc(int, int);
void *MessagePrinter_New(int, int, int, int);
void ov87_021E80C0(void *, void *);
void *sub_020163E0(int, int, int, int);
void ov87_021E79C4(Ov87App *);
void ov87_021E7A04(Ov87App *);
void ov87_021E7A2C(Ov87App *);
void GfGfx_BothDispOn(void);
void *ov87_021E7F6C(void *, int, int, u16, u16, int, int, int);
void ov87_021E7FD4(void *, int);
void ov87_021E7FEC(void *, int, int);
void ov87_021E7FE0(void *, int);
void ov87_021E6704(Ov87App *);
void sub_020210BC(void);
void sub_02021148(int);
void ov87_021E6C04(void *);
void Main_SetVBlankIntrCB(void (*)(void *), void *);
void ov87_021E68DC(Ov87App *app);
// @fn ov87_021E68DC
void ov87_021E68DC(Ov87App *app) {
    int i;
    app->narc = NARC_New(0xCF, 0x7A);
    ov87_021E6B38(app);
    ov87_021E6BA8(app);
    app->msgData = NewMsgDataFromNarc(1, 0x1B, 0x1B0, 0x7A);
    app->msgFmt = MessageFormat_New(0x7A);
    app->str1 = String_New(600, 0x7A);
    app->str2 = String_New(600, 0x7A);
    LoadFontPal0(0, 0x1A0, 0x7A);
    LoadFontPal0(4, 0x1A0, 0x7A);
    LoadFontPal1(0, 0x180, 0x7A);
    LoadFontPal1(4, 0x180, 0x7A);
    FontID_Alloc(2, 0x7A);
    app->printer = MessagePrinter_New(15, 14, 0, 0x7A);
    ov87_021E80C0(app->x58, app->x5C);
    app->x34 = sub_020163E0(0, 1, 12, 0x7A);
    ov87_021E79C4(app);
    ov87_021E7A04(app);
    ov87_021E7A2C(app);
    GfGfx_BothDispOn();
    for (i = 0; i < 4; i++) {
        app->spritesA[i] = ov87_021E7F6C(app->renderer, 0, i, 0x3C, 0x1A + i * 0x2A, 1, 0, 0);
        ov87_021E7FD4(app->spritesA[i], 0);
    }
    for (i = 0; i < 9; i++) {
        app->spritesB[i] = ov87_021E7F6C(app->renderer, 1, 0, ov87_021E82E4[i].x, ov87_021E82E4[i].y, 0, 2, 10);
        ov87_021E7FD4(app->spritesB[i], 0);
    }
    ov87_021E6704(app);
    for (i = 0; i < 4; i++) {
        app->spritesC[i] = ov87_021E7F6C(app->renderer, 2, i, ov87_021E81A0[i].x, ov87_021E81A0[i].y, 0, 1, 0);
        ov87_021E7FEC(app->spritesC[i], ov87_021E81A0[i].x - 256, ov87_021E81A0[i].y);
        ov87_021E7FE0(app->spritesC[i], i + 20);
    }
    app->spriteD = ov87_021E7F6C(app->renderer, 4, 0, 0x26, 0x38, 0, 0, 10);
    ov87_021E7FD4(app->spriteD, 0);
    reg_GX_POWCNT &= ~0x8000;
    sub_020210BC();
    sub_02021148(1);
    Main_SetVBlankIntrCB(ov87_021E6C04, app);
}
