#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on

typedef void (*StateFunc)(void);

extern void ov13_0222B8E8(void *func);
extern void ov13_0222B8F8(int a, int b);
extern int ov13_02240394(int a);
extern void ov13_022403B8(int a, int b, int c, int d);
extern void ov13_02240EA8(int a, int b);
extern void ov13_02240EFC(int a, int b);
extern void ov13_02240B04(int a);
extern void ov13_022405B8(int a);
extern int ov13_02241694(int a);
extern void ov13_0222BC84(void *func);
extern int ov13_0222BC68(void);
extern void ov13_0222BC0C(void);
extern void ov13_02227F68(void);
extern void ov13_02227028(void);
extern void ov13_0223A840(void);
extern void ov13_0223AC20(void);
extern void ov13_0223ABB8(int a);
extern void ov13_022279EC(int a);
extern void ov13_022279B4(int a);
extern int ov13_02227258(void);
extern void ov13_022272CC(void);
extern void ov13_0222726C(int a);
extern void ov13_022272E4(void);
extern void ov13_02227228(void);
extern int ov13_022272A0(void);
extern void ov13_02227DE4(int a);
extern int ov13_022277B8(int a, int b, int c);
extern void ov13_02227924(void *a, void *b);
extern void ov13_0222D454();
extern void ov13_0222DB6C();
extern void ov13_0222788C(void);

extern u8 ov13_0224F514;
extern u8 ov13_0224F518;
extern u8 ov13_0224F51C;
extern u8 ov13_0224F520[];
extern u8 ov13_0224F524[];
extern u8 ov13_0224F528;
struct UnkF52C {
    u8 flag;
    u16 counter;
    void *task;
};
extern struct UnkF52C ov13_0224F52C;
extern u8 ov13_0224F534;
struct UnkOam {
    u16 attr0;
    u16 attr1;
    u16 attr2;
};
struct UnkBig {
    u8 unk0[4];
    void *unk4;
    void *unk8;
    int unkC;
    struct UnkOam *unk10[5];
    struct UnkOam *unk24[5];
    u8 unk38[4];
    void *unk3C;
    u16 unk40;
    u16 unk42[3];
    u16 unk48[4];
    u8 unk50;
    u8 unk51;
    u8 unk52;
    u8 unk53;
};
struct UnkF538 {
    u8 unk0;
    u16 unk2;
    struct UnkBig *big;
};
extern struct UnkF538 ov13_0224F538;
struct Str25 {
    char s[25];
};
struct Str22 {
    char s[22];
};
extern struct Str25 ov13_022453E3;
extern struct Str22 ov13_022453CD;
extern u8 ov13_02246358[];
extern u8 ov13_0224544C[];
extern u8 ov13_02245450[];
extern void ov13_0222B934(int a, int b);
extern void ov13_02238440();
extern void *ov13_0224128C(int size, int align);
extern void ov13_0222B948(int *out, int a);
extern u8 ov13_02239D04(void *p);
extern void ov13_02230C68();
extern void ov13_022307AC(void);
extern void ov13_02230CC4(void);
extern void *ov13_022278C4(void *a);
extern void *ov13_0223F95C(void *a, int b, int c);
extern void ov13_02239228(void *a);
extern void ov13_022392B4(void);
extern void ov13_0223943C(int a, int b, int c, int d, int e);
extern struct UnkOam *ov13_02240184(int a, int b);
extern void ov13_022400F0(int a, int b, struct UnkOam *c);
extern void ov13_0222FF50(void);
extern void ov13_022301A4(void);
extern void ov13_02230414(void);
extern s32 FX_DivS32(s32 a, s32 b);
extern u8 ov13_02246300[];
extern u8 ov13_02246318[];
extern u8 ov13_02246330[];
extern u8 ov13_02246344[];
extern void ov13_02227E40(void);
extern void ov13_02227964(int a);
extern void ov13_0222B254(void);
extern void *ov13_02242528(int a, void *b, int c, int d);
extern int ov13_0222B3B4(void);
extern void ov13_022425F0(int a, void *b);
extern void ov13_022425D0(int a, void *b);
extern void ov13_0222B364(int a);
extern void ov13_02237618();
extern void ov13_022413E4(void);
extern void ov13_02242478(int a);
extern u8 ov13_022462D8[];
extern u8 ov13_022462EC[];
extern void *ov13_0222BCA8(void);
extern int ov13_022408A0(int a, int b);
extern void ov13_02240CC0(int a, int b, int c, int d, int e, int f, int g, void *h);
extern void ov13_02240E2C(int a);
extern void ov13_0223BE6C(int a, int b, int c, int d, int e);
extern int ov13_0223C1DC(void);
extern void ov13_0223C198(void);
extern int ov13_0223C1F0(void);
extern void ov13_0222BC9C(void);
extern void ov13_0222BB20(void *func);
extern void ov13_02226F58(int a);
extern u8 ov13_0224629C[];
extern u8 ov13_022462B0[];
extern u8 ov13_022462C4[];

void ov13_0222DC24();
void ov13_0222DC58();
void ov13_0222DC88();
void ov13_0222DC98();
void ov13_0222DC9C();
void ov13_0222DCA0();
void ov13_0222DCC8();
void ov13_0222DD5C();
void ov13_0222DDE4();
void ov13_0222DE04();
void ov13_0222DE6C();
void ov13_0222DEA0();
void ov13_0222DECC();
void ov13_0222DEF4();
void ov13_0222DF08();
void ov13_0222DF28();
void ov13_0222DF2C();
void ov13_0222DF54();
void ov13_0222DF74();
void ov13_0222DFC0();
void ov13_0222DFF4();
void ov13_0222E014();
void ov13_0222E07C();
void ov13_0222E0B0();
void ov13_0222E0EC();
void ov13_0222E114();
void ov13_0222E128();
void ov13_0222E148();
void ov13_0222E14C();
void ov13_0222E174();
void ov13_0222E194();
void ov13_0222E1D0();
void ov13_0222E220();
void ov13_0222E268();
void ov13_0222E2E8();
void ov13_0222E33C();
void ov13_0222E368();
void ov13_0222E390();
void ov13_0222E3A4();
void ov13_0222E3DC();
void ov13_0222E3E0();
void ov13_0222E430();
void ov13_0222E450();
void ov13_0222E4B4();
void ov13_0222E56C();
void ov13_0222E59C();
void ov13_0222E608();
void ov13_0222E63C();
void ov13_0222E66C();
void ov13_0222E67C();
void ov13_0222E680();
void ov13_0222E6BC();
void ov13_0222E720();
void ov13_0222E754();
void ov13_0222E790();
void ov13_0222E810();
void ov13_0222E878();
void ov13_0222E8B4();
void ov13_0222E8E8();
void ov13_0222E8FC();
void ov13_0222E934();
void ov13_0222E938();
void ov13_0222E990();
void ov13_0222E9B0();
void ov13_0222EA14();
void ov13_0222EABC();
void ov13_0222EB8C();
void ov13_0222EC0C();
void ov13_0222EC40();
void ov13_0222EC64();
void ov13_0222EC88();
void ov13_0222ECF8();
void ov13_0222ECFC();
void ov13_0222ED00();
void ov13_0222ED5C();
void ov13_0222EDB8();
void ov13_0222EE80();
int ov13_0222EE94();
int ov13_0222F28C();
void ov13_0222EEB4();
void ov13_0222EF00();
void ov13_0222EF80();
void ov13_0222EFB4();
void ov13_0222EFE0();
void ov13_0222F004();
void ov13_0222F018();
void ov13_0222F050();
void ov13_0222F054();
void ov13_0222F080();
void ov13_0222F0B8();
void ov13_0222F138();
void ov13_0222F230();
void ov13_0222F2AC();
void ov13_0222F30C();
void ov13_0222F3B4();
void ov13_0222F408();
void ov13_0222F444();
void ov13_0222F48C();
void ov13_0222F560();
void ov13_0222F598();
void ov13_0222F59C();
void ov13_0222F5E8();
void ov13_0222F624();
void ov13_0222F688();
void ov13_0222F764();
void ov13_0222F780();
void ov13_0222F7AC();
void ov13_0222F7CC();
void ov13_0222F818();
void ov13_0222F848();
void ov13_0222F884();
void ov13_0222F8F0();
void ov13_0222F924();
void ov13_0222F948();
void ov13_0222F958();
void ov13_0222F95C();
void ov13_0222F998();
void ov13_0222FA08();
void ov13_0222FA3C();
void ov13_0222FB14();
void ov13_0222FC68();
void ov13_0222FD10();
void ov13_0222FE84();
void ov13_0222FED8();
void ov13_0222FF14();
void ov13_0222FF3C();
void ov13_0222FF50();

#define REG16(a) (*(vu16 *)(a))
#define SET_PRI3(a) REG16(a) = (u16)((REG16(a) & ~3) | 3)

// @fn ov13_0222FE84
void ov13_0222FE84(void) {
    ov13_022403B8(2, 1, 1, 8);
    ov13_022403B8(2, 0, 0x1d, 8);
    ov13_02240EA8(1, 1);
    ov13_02240EA8(0, 0x1d);
    ov13_0222B8E8(ov13_0222FED8);
}

// @fn ov13_0222FED8
void ov13_0222FED8(void) {
    if (ov13_02240394(1) != 0) {
        return;
    }
    if (ov13_02240394(0) != 0) {
        return;
    }
    ov13_022279B4(0);
    ov13_0222B8E8(ov13_0222FF14);
}

// @fn ov13_0222FF14
void ov13_0222FF14(void) {
    if (ov13_02227258() == -2) {
        return;
    }
    ov13_022272CC();
    ov13_0222B8E8(ov13_0222FF3C);
}

// @fn ov13_0222FF3C
void ov13_0222FF3C(void) {
    ov13_0222FF50();
    ov13_022301A4();
    ov13_02230414();
}
