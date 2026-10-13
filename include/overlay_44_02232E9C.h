#ifndef POKEHEARTGOLD_OVY_44_02232E9C_H
#define POKEHEARTGOLD_OVY_44_02232E9C_H

#include "global.h"

#include "heap.h"
#include "list_menu.h"
#include "list_menu_2d.h"
#include "main.h"
#include "obj_char_transfer.h"
#include "overlay_42.h"
#include "render_window.h"
#include "sprite.h"
#include "unk_02009D48.h"
#include "font_sprite.h"
#include "friend_roster.h"
#include "yes_no_prompt.h"

typedef struct WifiClubRoomLights {
    void *unk0;
    NNSG2dPaletteData *unk4;
    u8 unk8;
    u8 unk9;
    s16 unkA;
    u8 unkC;
    u8 unkD;
    s16 unkE;
    u8 unk10;
} WifiClubRoomLights;

typedef struct UnkStruct_ov44_0222DD64 {
    u32 strno;
    u32 value;
} UnkStruct_ov44_0222DD64;

typedef struct WifiClubRoom WifiClubRoom;
typedef struct WifiClubRoomPerson WifiClubRoomPerson;
typedef struct UnkStruct_ov44_022319EC UnkStruct_ov44_022319EC;

typedef s32 (*WifiClubRoomPersonUpdateFunc)(WifiClubRoom *, WifiClubRoomPerson *);
typedef void (*WifiClubRoomPersonAnimFunc)(WifiClubRoom *, WifiClubRoomPerson *);
typedef void (*func_type_02236680)(UnkStruct_ov44_022319EC *, enum HeapID);
typedef s32 (*func_type_0222A60C)(UnkStruct_ov44_022319EC *, s32);

struct WifiClubRoomPerson {
    UnkStruct_ov42_02228110 *unk0;
    UnkStruct_ov42_0222903C *unk4;
    s16 unk8;
    u8 unkA;
    u8 unkB;
    u16 unkC;
    u16 unkE;
    WifiClubRoomPersonUpdateFunc unk10;
    void *unk14;
    WifiClubRoomPersonAnimFunc unk18;
};

typedef struct UnkStruct_ov44_02232C4C {
    s16 unk0;
    u8 unk2;
    u8 unk3;
} UnkStruct_ov44_02232C4C;

typedef struct WifiClubRoomGfx {
    SpriteList *unk0;
    G2dRenderer unk4;
    GF_2DGfxResMan *unk12C[4];
    SpriteResource *unk13C[4];
    SpriteResourcesHeader unk14C;
    Sprite *unk170;
    Sprite *unk174;
} WifiClubRoomGfx;

struct WifiClubRoom {
    s32 unk0;
    enum HeapID unk4;
    u8 unk8;
    u8 unk9;
    s16 unkA;
    u32 unkC;
    BgConfig *unk10;
    WifiClubRoomGfx unk14;
    WifiClubRoomPerson unk18C[48];
    WifiClubRoomPerson *unk6CC;
    WifiClubRoomLights unk6D0;
    UnkStruct_ov42_02227F68 *unk6E4;
    UnkStruct_ov42_022280A8 *unk6E8;
    UnkStruct_ov42_02228EDC *unk6EC;
    UnkStruct_ov44_02232914 unk6F0;
    UnkStruct_ov42_022293B8 *unk6F4;
    UnkStruct_ov42_02229A40 *unk6F8;
};

typedef struct UnkStruct_ov44_02231800 {
    MessageFormat *unk0;
    void *unk4;
    NNSG2dScreenData *unk8;
    void *unkC[7];
    NNSG2dScreenData *unk28[7];
    void *unk44;
    NNSG2dScreenData *unk48;
    TouchHitboxController *unk4C;
    u8 unk50[0x20];
    u8 unk70;
    u8 unk71;
    u8 unk72;
    u8 unk73;
    s8 unk74;
    u8 unk75;
    u8 unk76;
    u8 unk77;
    u32 unk78;
    Window unk7C[8];
    Window unkFC[8][2];
    Window unk1FC;
    SpriteResource *unk20C[4];
    Sprite *unk21C[3];
    ObjCharSpaceReservation unk228;
    TextOBJ *unk234;
    u32 unk238;
    u32 unk23C;
    u32 unk240;
    TouchHitboxController *unk244;
} UnkStruct_ov44_02231800;

typedef struct UnkStruct_ov44_02231958 {
    u16 unk0[6];
    u16 unkC[6];
    u8 unk18[3];
    u8 unk1B;
    u8 unk1C[5];
    u8 unk21;
    u8 unk22;
    u8 unk23;
} UnkStruct_ov44_02231958;

typedef struct UnkStruct_ov44_0223197C {
    UnkStruct_ov44_02231958 unk0;
    UnkStruct_ov44_02231958 unk24[0x20];
} UnkStruct_ov44_0223197C;

typedef struct UnkStruct_ov44_0222A40C {
    SysTask *unk0;
    u32 unk4;
    u16 unk8[4][0x10];
    u16 unk88[4][0x10];
    u8 unk108[0x220];
    s16 unk328;
    s8 unk32A;
    u8 unk32B;
    u32 unk32C;
} UnkStruct_ov44_0222A40C;

typedef struct UnkStruct_ov44_0222AB24 {
    void *unk0;
    NNSG2dScreenData *unk4;
    void *unk8;
    NNSG2dCharacterData *unkC;
} UnkStruct_ov44_0222AB24;

struct UnkStruct_ov44_022319EC {
    FriendRoster *unk0;
    UnkStruct_ov44_0223197C *unk4;
    u8 unk8[4];
    u8 unkC[0x20];
    u8 unk2C[0x20];
    u32 unk4C[0x20];
    u32 unkCC[0x20];
    ListMenuItem *unk14C;
    struct ListMenu *unk150;
    ListMenuItem *unk154;
    struct ListMenu *unk158;
    BgConfig *unk15C;
    SaveData *saveData;
    MessageFormat *unk164;
    MsgData *unk168;
    MsgData *unk16C;
    String *unk170;
    String *unk174;
    String *unk178;
    String *unk17C;
    u32 unk180;
    struct ListMenu2D *unk184;
    YesNoPrompt *unk188;
    WaitingIcon *unk18C;
    SpriteList *unk190;
    G2dRenderer unk194;
    GF_2DGfxResMan *unk2BC[4];
    void *unk2CC;
    Window unk2D0;
    Window unk2E0;
    Window unk2F0;
    Window unk300;
    Window unk310;
    Window unk320;
    Window unk330;
    s32 unk340;
    u32 unk344;
    u32 unk348;
    u32 unk34C;
    u8 unk350[0x4];
    u32 unk354;
    u8 unk358[4];
    s32 unk35C;
    u8 unk360[4];
    u8 unk364;
    u8 unk365[3];
    u32 unk368;
    u8 unk36C[4];
    s32 unk370;
    u16 unk374;
    u16 unk376;
    u8 unk378[4];
    u16 unk37C;
    u16 unk37E;
    u16 unk380;
    u16 unk382;
    u32 unk384;
    u8 unk388[4];
    WifiClubRoom unk38C;
    WifiClubRoomPerson *unkA88[0x21];
    UnkStruct_ov44_0222AB24 unkB0C;
    UnkStruct_ov44_02231800 unkB1C;
    u32 unkD64;
    UnkStruct_ov44_0222A40C unkD68;
    UnkStruct_ov44_02231958 unk1098;
    u16 unk10BC;
    u16 unk10BE;
};

typedef struct UnkStruct_ov44_0222A758_args {
    SaveData *unk0;
    u32 unk4;
    u32 unk8;
} UnkStruct_ov44_0222A758_args;

s32 ov44_02229EE0(UnkStruct_ov44_022319EC *arg0);
s32 ov44_02229EF8(UnkStruct_ov44_022319EC *arg0);
s32 ov44_02229EFC(UnkStruct_ov44_022319EC *arg0);
u32 ov44_02229F00(UnkStruct_ov44_022319EC *arg0, UnkStruct_ov44_02231958 *arg1);
s32 ov44_02229F44(s32 arg0);
u32 ov44_02229F74(u32 arg0);
s32 ov44_02229FB4(u32 arg0);
s32 ov44_02229FDC(u32 arg0);
s32 ov44_0222A020(u32 arg0);
s32 ov44_0222A048(u16 arg0);
s32 ov44_0222A090(u32 arg0);
u32 ov44_0222A0B4(u32 arg0);
s32 ov44_0222A168(UnkStruct_ov44_022319EC *arg0);
void ov44_0222A1B4(u32 arg0);
void ov44_0222A1FC(UnkStruct_ov44_022319EC *arg0);

s32 ov44_02229FDC(u32);
void ov44_0222A40C(s32 arg0, UnkStruct_ov44_0222A40C *arg1);
s32 ov44_0222A4B4(OverlayManager *overlayMananger, s32 *arg1);
s32 ov44_0222A60C(OverlayManager *overlayManager, s32 *arg1);
void ov44_0222A68C(UnkStruct_ov44_022319EC *arg0);
s32 ov44_0222A758(OverlayManager *overlayManager);
void ov44_0222A7F8(UnkStruct_ov44_022319EC *arg0);
void ov44_0222A830(void);
void ov44_0222A850(BgConfig *arg0);
void ov44_0222AA94(UnkStruct_ov44_022319EC *arg0, WifiClubRoomPerson *arg1);
void ov44_0222AAC0(UnkStruct_ov44_022319EC *arg0, WifiClubRoomPerson *arg1);
WifiClubRoomPerson *ov44_0222AAEC(UnkStruct_ov44_022319EC *arg0, u32 arg1);
void ov44_0222AB24(BgConfig *arg0, UnkStruct_ov44_0222AB24 *arg1, NARC *arg2, enum HeapID arg3);
void ov44_0222ABB8(UnkStruct_ov44_0222AB24 *arg0);
void ov44_0222ABDC(BgConfig *arg0, UnkStruct_ov44_0222AB24 *arg1, s32 arg2, u8 arg3, u8 arg4, s32 arg5, s32 arg6);
void ov44_0222AC54(BgConfig *arg0, UnkStruct_ov44_0222AB24 *arg1, s32 arg2, u8 arg3, u8 arg4, u32 arg5);
void ov44_0222ACE8(Window *arg0, UnkStruct_ov44_0222AB24 *arg1, u8 arg2, u16 arg3, s32 arg4, s32 arg5);
void ov44_0222AD34(Window *arg0, UnkStruct_ov44_0222AB24 *arg1, u8 arg2, u16 arg3, u32 arg4);
s32 ov44_0222ADC4(u32 arg0);
u32 ov44_0222ADD0(void);
void ov44_0222ADF8(UnkStruct_ov44_022319EC *arg0);
void ov44_0222AE44(UnkStruct_ov44_022319EC *arg0);
void ov44_0222AE74(BgConfig *arg0);
void ov44_0222AEC0(UnkStruct_ov44_022319EC *arg0, NARC *arg1);
void ov44_0222AFE8(void);
void ov44_0222B030(UnkStruct_ov44_022319EC *arg0, NARC *arg1);
void ov44_0222B0A4(UnkStruct_ov44_022319EC *arg0);
void ov44_0222B0B0(UnkStruct_ov44_022319EC *arg0);
void ov44_0222B164(UnkStruct_ov44_022319EC *arg0);
s32 ov44_0222B228(UnkStruct_ov44_022319EC *arg0, s32 arg1);
s32 ov44_0222B36C(UnkStruct_ov44_022319EC *arg0, s32 arg1);
s32 ov44_0222B3A8(UnkStruct_ov44_022319EC *arg0, s32 arg1);
s32 ov44_0222B42C(UnkStruct_ov44_022319EC *arg0, s32 arg1);
s32 ov44_0222B494(UnkStruct_ov44_022319EC *arg0, s32 arg1);
s32 ov44_0222B528(UnkStruct_ov44_022319EC *arg0, s32 arg1);
s32 ov44_0222B64C(UnkStruct_ov44_022319EC *arg0, s32 arg1);
s32 ov44_0222B744(UnkStruct_ov44_022319EC *arg0, s32 arg1);
void ov44_0222B778(UnkStruct_ov44_022319EC *arg0, u32 arg1);
s32 ov44_0222B870(UnkStruct_ov44_022319EC *arg0);
s32 ov44_0222B8E0(UnkStruct_ov44_022319EC *arg0);
void ov44_0222B9A0(UnkStruct_ov44_022319EC *arg0);
s32 ov44_0222BA6C(UnkStruct_ov44_022319EC *arg0, s32 arg1);
s32 ov44_0222BAB8(UnkStruct_ov44_022319EC *arg0, s32 arg1);
s32 ov44_0222BAC4(UnkStruct_ov44_022319EC *arg0, s32 arg1);
s32 ov44_0222BAD0(UnkStruct_ov44_022319EC *arg0, s32 arg1);
s32 ov44_0222BB38(UnkStruct_ov44_022319EC *arg0, s32 arg1);
s32 ov44_0222BBA4(UnkStruct_ov44_022319EC *arg0, s32 arg1);
s32 ov44_0222BC78(UnkStruct_ov44_022319EC *arg0, s32 arg1);
s32 ov44_0222BE3C(UnkStruct_ov44_022319EC *arg0, s32 arg1);
s32 ov44_0222BEE0(UnkStruct_ov44_022319EC *arg0, s32 arg1);
s32 ov44_0222BF6C(UnkStruct_ov44_022319EC *arg0, s32 arg1);
s32 ov44_0222BFF0(UnkStruct_ov44_022319EC *arg0, s32 arg1);
s32 ov44_0222C060(UnkStruct_ov44_022319EC *arg0, s32 arg1);
s32 ov44_0222C084(UnkStruct_ov44_022319EC *arg0, s32 arg1);
void ov44_0222C120(UnkStruct_ov44_022319EC *arg0);
void ov44_0222C288(struct ListMenu *list, s32 index, u8 onInit);
void ov44_0222C29C(UnkStruct_ov44_022319EC *arg0);
s32 ov44_0222C35C(UnkStruct_ov44_022319EC *arg0, s32 arg1);
s32 ov44_0222C4EC(UnkStruct_ov44_022319EC *arg0);
s32 ov44_0222C500(UnkStruct_ov44_022319EC *arg0);
s32 ov44_0222C524(UnkStruct_ov44_022319EC *arg0);
void ov44_0222C658(UnkStruct_ov44_022319EC *arg0, enum HeapID heapID);
void ov44_0222C670(UnkStruct_ov44_022319EC *arg0, enum HeapID heapID);
s32 ov44_0222C684(UnkStruct_ov44_022319EC *arg0, s32 arg1);
s32 ov44_0222C9AC(UnkStruct_ov44_022319EC *arg0, s32 arg1);
s32 ov44_0222C9F4(UnkStruct_ov44_022319EC *arg0, s32 arg1);
s32 ov44_0222CB34(UnkStruct_ov44_022319EC *arg0, s32 arg1);
s32 ov44_0222CB7C(UnkStruct_ov44_022319EC *arg0, s32 arg1);
s32 ov44_0222CC34(UnkStruct_ov44_022319EC *arg0, s32 arg1);
s32 ov44_0222CDAC(UnkStruct_ov44_022319EC *arg0, s32 arg1);

s32 ov44_0222CE40(UnkStruct_ov44_022319EC *arg0, s32 arg1);
s32 ov44_0222CFE0(UnkStruct_ov44_022319EC *arg0, s32 arg1);
s32 ov44_0222D0A4(UnkStruct_ov44_022319EC *arg0, s32 arg1);
s32 ov44_0222D10C(UnkStruct_ov44_022319EC *arg0, s32 arg1);
s32 ov44_0222D1BC(UnkStruct_ov44_022319EC *arg0, s32 arg1);
s32 ov44_0222D1C0(UnkStruct_ov44_022319EC *arg0, s32 arg1);
s32 ov44_0222D1C4(UnkStruct_ov44_022319EC *arg0, s32 arg1);
s32 ov44_0222D1C8(UnkStruct_ov44_022319EC *arg0, s32 arg1);
s32 ov44_0222D214(UnkStruct_ov44_022319EC *arg0, s32 arg1);
s32 ov44_0222D23C(UnkStruct_ov44_022319EC *arg0, s32 arg1);
s32 ov44_0222D2B0(UnkStruct_ov44_022319EC *arg0, s32 arg1);
s32 ov44_0222D3DC(UnkStruct_ov44_022319EC *arg0, s32 arg1);
s32 ov44_0222D594(UnkStruct_ov44_022319EC *arg0, s32 arg1);
s32 ov44_0222D824(UnkStruct_ov44_022319EC *arg0, s32 arg1);
s32 ov44_0222D8B0(UnkStruct_ov44_022319EC *arg0, s32 arg1);
s32 ov44_0222DA64(UnkStruct_ov44_022319EC *arg0, s32 arg1);
s32 ov44_0222DC18(UnkStruct_ov44_022319EC *arg0, s32 arg1);
s32 ov44_0222DD64(UnkStruct_ov44_022319EC *arg0, s32 arg1);
s32 ov44_0222DFEC(UnkStruct_ov44_02231958 *arg0);
s32 ov44_0222E02C(UnkStruct_ov44_022319EC *arg0);
void ov44_0222E030(void);
BOOL ov44_0222E074(void);
s32 ov44_0222E090(UnkStruct_ov44_022319EC *arg0, s32 arg1);
s32 ov44_0222E45C(UnkStruct_ov44_022319EC *arg0, s32 arg1);
s32 ov44_0222E5D8(UnkStruct_ov44_022319EC *arg0, s32 arg1);
s32 ov44_0222E5DC(UnkStruct_ov44_022319EC *arg0, s32 arg1);
s32 ov44_0222E5E0(UnkStruct_ov44_022319EC *arg0, s32 arg1);
s32 ov44_0222E62C(UnkStruct_ov44_022319EC *arg0, s32 arg1);
s32 ov44_0222E630(UnkStruct_ov44_022319EC *arg0, s32 arg1);
s32 ov44_0222E634(UnkStruct_ov44_022319EC *arg0, s32 arg1);
s32 ov44_0222E7C4(UnkStruct_ov44_022319EC *arg0);
s32 ov44_0222E860(UnkStruct_ov44_022319EC *arg0, s32 arg1);
s32 ov44_0222E908(UnkStruct_ov44_022319EC *arg0, s32 arg1);
s32 ov44_0222E948(UnkStruct_ov44_022319EC *arg0, s32 arg1);
s32 ov44_0222E9C4(UnkStruct_ov44_022319EC *arg0, s32 arg1);
s32 ov44_0222E9FC(UnkStruct_ov44_022319EC *arg0);
s32 ov44_0222EA2C(UnkStruct_ov44_022319EC *arg0, s32 arg1);
s32 ov44_0222EB90(UnkStruct_ov44_022319EC *arg0, s32 arg1);
s32 ov44_0222EBB8(UnkStruct_ov44_022319EC *arg0, s32 arg1);
s32 ov44_0222EBC4(UnkStruct_ov44_022319EC *arg0, s32 arg1);
s32 ov44_0222EC14(UnkStruct_ov44_022319EC *arg0, s32 arg1);

s32 ov44_0222EC2C(UnkStruct_ov44_022319EC *arg0, s32 arg1);
s32 ov44_0222EC98(UnkStruct_ov44_022319EC *arg0, s32 arg1);
s32 ov44_0222EDB8(UnkStruct_ov44_022319EC *arg0, s32 arg1);
s32 ov44_0222EE10(UnkStruct_ov44_022319EC *arg0, s32 arg1);
s32 ov44_0222EE54(UnkStruct_ov44_022319EC *arg0, s32 arg1);
s32 ov44_0222EED4(UnkStruct_ov44_022319EC *arg0, s32 arg1);
s32 ov44_0222F0AC(UnkStruct_ov44_022319EC *arg0, s32 arg1);
s32 ov44_0222F194(UnkStruct_ov44_022319EC *arg0, s32 arg1);
s32 ov44_0222F4E0(UnkStruct_ov44_022319EC *arg0, s32 arg1);
void ov44_0222F510(UnkStruct_ov44_022319EC *arg0, s32 arg1, s32 arg2);
void ov44_0222F66C(UnkStruct_ov44_022319EC *arg0, s32 arg1);
void ov44_0222F780(UnkStruct_ov44_022319EC *arg0, s32 arg1);
void ov44_0222F7BC(UnkStruct_ov44_022319EC *arg0);
void ov44_0222F818(UnkStruct_ov44_022319EC *arg0, u32 arg1);
void ov44_0222F830(UnkStruct_ov44_022319EC *arg0, u32 arg1);
u8 ov44_0222F89C(UnkStruct_ov44_022319EC *arg0);
u8 ov44_0222F8C0(UnkStruct_ov44_022319EC *arg0);
u8 ov44_0222F8F0(UnkStruct_ov44_022319EC *arg0);
void ov44_0222F910(UnkStruct_ov44_022319EC *arg0);
void ov44_0222F950(UnkStruct_ov44_022319EC *arg0, enum HeapID heapID);
void ov44_0222F970(UnkStruct_ov44_022319EC *arg0, s32 arg1, enum HeapID heapID);
s32 ov44_0222F98C(UnkStruct_ov44_022319EC *arg0);
void ov44_0222F9A0(UnkStruct_ov44_022319EC *arg0, NARC *arg1, enum HeapID arg2);
void ov44_0222FA28(UnkStruct_ov44_022319EC *arg0);
u8 ov44_0222FA80(UnkStruct_ov44_022319EC *arg0, enum HeapID heapID);
void ov44_0222FBA0(UnkStruct_ov44_022319EC *arg0);
s32 ov44_0222FBC8(UnkStruct_ov44_022319EC *arg0, s32 arg1);
u8 ov44_0222FC00(UnkStruct_ov44_022319EC *arg0);
void ov44_0222FC0C(UnkStruct_ov44_022319EC *arg0);
void ov44_0222FC3C(UnkStruct_ov44_022319EC *arg0, s32 arg1);
void ov44_0222FC84(s32 arg0, enum HeapID arg1, UnkStruct_ov44_022319EC *arg2);
void ov44_0222FCBC(UnkStruct_ov44_022319EC *arg0, NARC *arg1, enum HeapID arg2);
void ov44_0222FF30(UnkStruct_ov44_022319EC *arg0);
void ov44_0222FFB4(NNSG2dScreenData *arg0, s32 arg1);
void ov44_0222FFF4(UnkStruct_ov44_022319EC *arg0, s32 arg1, u32 arg2);
void ov44_02230028(UnkStruct_ov44_022319EC *arg0, s32 arg1);
u8 ov44_02230048(UnkStruct_ov44_022319EC *arg0, s32 arg1);
void ov44_02230060(UnkStruct_ov44_022319EC *arg0, enum HeapID heapID);
void ov44_02230080(UnkStruct_ov44_022319EC *arg0);
void ov44_02230090(UnkStruct_ov44_022319EC *arg0);
void ov44_022300C8(UnkStruct_ov44_022319EC *arg0);
void ov44_02230234(UnkStruct_ov44_022319EC *arg0, enum HeapID heapID);
void ov44_02230300(UnkStruct_ov44_022319EC *arg0, enum HeapID heapID);
void ov44_022307E0(UnkStruct_ov44_022319EC *arg0, enum HeapID heapID);
void ov44_022308B0(UnkStruct_ov44_022319EC *arg0, enum HeapID heapID);
void ov44_02230B2C(UnkStruct_ov44_022319EC *arg0, enum HeapID heapID);
void ov44_02230C68(UnkStruct_ov44_022319EC *arg0, enum HeapID heapID);
void ov44_02230D8C(UnkStruct_ov44_022319EC *arg0, enum HeapID heapID);
void ov44_02230E5C(UnkStruct_ov44_022319EC *arg0, enum HeapID heapID);
void ov44_02230FE8(UnkStruct_ov44_022319EC *arg0);
void ov44_02231054(UnkStruct_ov44_022319EC *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
void ov44_02231084(UnkStruct_ov44_022319EC *arg0, String *arg1, u32 arg2, s32 arg3);
void ov44_022310C8(UnkStruct_ov44_022319EC *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void ov44_02231148(UnkStruct_ov44_022319EC *arg0, u8 arg1, u8 arg2, u8 arg3, u8 arg4);
s32 ov44_0223120C(u32 arg0, u32 *color);
void ov44_022312B8(UnkStruct_ov44_022319EC *arg0, Window *arg1, s32 arg2, s32 arg3, s32 arg4);
void ov44_02231344(UnkStruct_ov44_022319EC *arg0, Window *arg1, s32 arg2, u32 arg3, u32 arg4);
void ov44_022313C8(UnkStruct_ov44_022319EC *arg0);
void ov44_02231420(UnkStruct_ov44_022319EC *arg0, NARC *arg1, enum HeapID arg2);
void ov44_022316B0(UnkStruct_ov44_022319EC *arg0);
void ov44_02231720(UnkStruct_ov44_022319EC *arg0);
void ov44_02231754(UnkStruct_ov44_022319EC *arg0);
s32 ov44_02231788(UnkStruct_ov44_022319EC *arg0);
void ov44_022317F0(u32 arg0, u32 arg1, UnkStruct_ov44_022319EC *arg2);
void ov44_02231800(UnkStruct_ov44_02231800 *arg0, s32 arg1);
s32 ov44_0223183C(UnkStruct_ov44_02231800 *arg0, s32 arg1, s32 arg2, s32 arg3);

// 1
s32 ov44_02229F44(s32);
void ov44_0222F510(UnkStruct_ov44_022319EC *, s32, s32);
void ov44_0222F7BC(UnkStruct_ov44_022319EC *);
s32 WifiClubRoom_GetTileBelowPlayer(WifiClubRoom *arg0);

// The following functions are static
void ov44_02231918(UnkStruct_ov44_022319EC *arg0, s32 arg1);
UnkStruct_ov44_02231958 *ov44_02231958(UnkStruct_ov44_022319EC *arg0, u32 arg1);
s32 ov44_02231974(s32 arg0);
s32 ov44_0223197C(UnkStruct_ov44_022319EC *arg0, s32 arg1, s32 arg2);
void ov44_022319BC(UnkStruct_ov44_022319EC *arg0, s32 arg1);
void ov44_022319EC(UnkStruct_ov44_022319EC *arg0);
YesNoPrompt *WifiClub_CreateYesNoPromptIgnoreTouch(BgConfig *arg0, const WindowTemplate *arg1, u32 arg2);
YesNoPrompt *WifiClub_CreateYesNoPrompt(BgConfig *arg0, const WindowTemplate *arg1, u32 arg2);
void WifiClubRoom_Init(WifiClubRoom *arg0, enum HeapID heapID, NARC *arg2, BgConfig *arg3, s32 arg4, u32 arg5);
void WifiClubRoom_Free(WifiClubRoom *arg0);
s32 WifiClubRoom_IsInitialized(WifiClubRoom *arg0);
s32 WifiClubRoom_Update(WifiClubRoom *arg0);
void WifiClubRoom_Draw(WifiClubRoom *arg0);
u8 WifiClubRoom_GetTalkPartnerId(WifiClubRoom *arg0);
s32 WifiClubRoom_GetTileUnderPlayer(WifiClubRoom *arg0);
void WifiClubRoom_ShowFriendMarker(WifiClubRoom *arg0, WifiClubRoomPerson *arg1);
void WifiClubRoom_HideFriendMarker(WifiClubRoom *arg0);
void WifiClubRoom_SetInputLocked(WifiClubRoom *arg0, s32 arg1);
s32 WifiClubRoom_IsInputLocked(WifiClubRoom *arg0);
WifiClubRoomPerson *WifiClubRoom_AddPlayer(WifiClubRoom *arg0, s16 arg1);
WifiClubRoomPerson *WifiClubRoom_AddFriend(WifiClubRoom *arg0, s16 arg1, s16 arg2);
void WifiClubRoom_StartPersonLeave(WifiClubRoom *arg0, WifiClubRoomPerson *arg1);
void WifiClubRoomPerson_Delete(WifiClubRoomPerson *arg0);
u8 WifiClubRoomPerson_GetId(WifiClubRoomPerson *arg0);
s32 WifiClubRoomPerson_GetOppositeDirection(WifiClubRoomPerson *arg0);
void WifiClubRoom_StartTalk(WifiClubRoom *arg0, WifiClubRoomPerson *arg1, s32 arg2);
void WifiClubRoom_EndTalk(WifiClubRoom *arg0, WifiClubRoomPerson *arg1);
void WifiClubRoom_SetPersonBusy(WifiClubRoom *arg0, WifiClubRoomPerson *arg1, u8 arg2);
void WifiClubRoom_StartCounterLight(WifiClubRoom *arg0);
void WifiClubRoom_ContinueLightSequence(WifiClubRoom *arg0);
void WifiClubRoom_UpdateLights(WifiClubRoom *arg0);
void WifiClubRoom_DeleteAllPersons(WifiClubRoom *arg0);
s32 WifiClubRoom_IsPlayerIdle(WifiClubRoom *arg0);
s32 WifiClubRoom_IsPlayerFacingCounter(WifiClubRoom *arg0);
void WifiClubRoom_ProcessMoveCommands(WifiClubRoom *arg0);
void WifiClubRoom_UpdatePersons(WifiClubRoom *arg0);
void WifiClubRoom_AnimatePersons(WifiClubRoom *arg0);
WifiClubRoomPerson *WifiClubRoom_GetPersonInDirection(WifiClubRoom *arg0, WifiClubRoomPerson *arg1, u32 arg2);
void WifiClubRoom_CreateDrawers(WifiClubRoom *arg0, s32 arg1, enum HeapID arg2);
void WifiClubRoom_DeleteDrawers(WifiClubRoom *arg0);
void WifiClubRoom_CreateMapBg(WifiClubRoom *arg0, enum HeapID arg1, NARC *arg2, s32 arg3);
void WifiClubRoom_DeleteMapBg(WifiClubRoom *arg0);
void WifiClubRoomGfx_Init(WifiClubRoomGfx *arg0, enum HeapID heapID, NARC *narc);
void WifiClubRoomGfx_Free(WifiClubRoomGfx *arg0);
void WifiClubRoomGfx_LoadResources(WifiClubRoomGfx *arg0, enum HeapID heapID, NARC *narc);
void WifiClubRoomGfx_FreeResources(WifiClubRoomGfx *arg0);
void WifiClubRoomGfx_CreateSprites(WifiClubRoomGfx *arg1, enum HeapID heapID);
void WifiClubRoomGfx_DeleteSprites(WifiClubRoomGfx *arg0);
void WifiClubRoom_InitBgs(BgConfig *arg0, enum HeapID heapID);
void WifiClubRoom_FreeBgs(BgConfig *arg0);
void WifiClubRoom_LoadBgGfx(BgConfig *bgConfig, enum HeapID heapID, NARC *narc);
void WifiClubRoomGfx_LoadSpriteResources(WifiClubRoomGfx *arg0, enum HeapID heapID, NARC *narc);
void WifiClubRoomGfx_FreeSpriteResources(WifiClubRoomGfx *arg0);
void WifiClubRoomGfx_CreateMarkerSprites(WifiClubRoomGfx *arg0, enum HeapID heapID);
void WifiClubRoomGfx_DeleteMarkerSprites(WifiClubRoomGfx *arg0);
void WifiClubRoom_ShowExitArrow(WifiClubRoom *arg0);
void WifiClubRoom_HideExitArrow(WifiClubRoom *arg0);
void WifiClubRoom_ShowFriendMarkerAt(WifiClubRoom *arg0, UnkStruct_ov44_02232914 arg1, s32 arg2);
void WifiClubRoom_HideFriendMarkerSprite(WifiClubRoom *arg0);
s32 WifiClubRoom_IsPlayerAtEntrance(WifiClubRoom *arg0);
void WifiClubRoom_FindFriendSpawnPos(WifiClubRoom *arg0, s16 *arg1, u32 arg2);
void WifiClubRoom_FindEntrancePos(WifiClubRoom *arg0, s16 *arg1);
WifiClubRoomPerson *WifiClubRoom_AllocPerson(WifiClubRoom *arg0);
void WifiClubRoom_CreatePersonDrawer(WifiClubRoom *arg0, WifiClubRoomPerson *arg1, s32 arg2);
void WifiClubRoom_CreatePersonObject(WifiClubRoom *arg0, WifiClubRoomPerson *arg1, s16 *arg2, s16 arg3, u16 arg4, u8 arg5, s32 arg6);
s32 WifiClubRoomPerson_CanTalk(WifiClubRoomPerson *arg0);
void WifiClubRoom_SetPersonModePlayer(WifiClubRoom *arg0, WifiClubRoomPerson *arg1);
void WifiClubRoom_SetPersonModeWarpIn(WifiClubRoom *arg0, WifiClubRoomPerson *arg1);
void WifiClubRoom_SetPersonModeWander(WifiClubRoom *arg0, WifiClubRoomPerson *arg1);
void WifiClubRoom_SetPersonModeBusy(WifiClubRoom *arg0, WifiClubRoomPerson *arg1);
void WifiClubRoomPerson_AnimateNone(WifiClubRoom *arg0, WifiClubRoomPerson *arg1);
void WifiClubRoomPerson_AnimateWarp(WifiClubRoom *arg0, WifiClubRoomPerson *arg1);
void WifiClubRoomPerson_AnimateHop(WifiClubRoom *arg0, WifiClubRoomPerson *arg1);
s32 WifiClubRoomPerson_UpdatePlayer(WifiClubRoom *arg0, WifiClubRoomPerson *arg1);
s32 WifiClubRoomPerson_UpdateWarpIn(WifiClubRoom *arg0, WifiClubRoomPerson *arg1);
s32 WifiClubRoomPerson_UpdateWarpOut(WifiClubRoom *arg0, WifiClubRoomPerson *arg1);
s32 WifiClubRoomPerson_UpdateWander(WifiClubRoom *arg0, WifiClubRoomPerson *arg1);
s32 WifiClubRoomPerson_UpdateBusy(WifiClubRoom *arg0, WifiClubRoomPerson *arg1);
s32 WifiClubRoomPerson_UpdateTalking(WifiClubRoom *arg0, WifiClubRoomPerson *arg1);
void WifiClubRoom_QueueMoveCommand(WifiClubRoom *arg0, s16 arg1, u32 arg2, u32 arg3);
void WifiClubRoom_QueuePersonCommand(WifiClubRoom *arg0, s16 arg1, WifiClubRoomPerson *arg2);
s32 WifiClubRoomPerson_IsNotWarping(WifiClubRoomPerson *arg0);
void WifiClubRoom_SetPersonActivityMode(WifiClubRoom *arg0, WifiClubRoomPerson *arg1, u8 arg2);
void WifiClubRoomLights_Init(WifiClubRoom *arg0, WifiClubRoomLights *arg1, NARC *arg2);
void WifiClubRoomLights_Free(WifiClubRoom *arg0, WifiClubRoomLights *arg1);
void WifiClubRoomLights_Update(WifiClubRoom *arg0, WifiClubRoomLights *arg1);
void WifiClubRoomLights_UpdateBlink(WifiClubRoomLights *arg0);
void WifiClubRoomLights_UpdateSequence(WifiClubRoomLights *arg0);
void *WifiClubRoomLights_GetColor(WifiClubRoomLights *arg0, s32 arg1);
void WifiClubRoomLights_EndSequence(WifiClubRoomLights *arg0);
void WifiClubRoomLights_Trigger(WifiClubRoomLights *arg0);
void WifiClubRoomLights_Start(WifiClubRoomLights *arg0, u8 arg1);
void WifiClubRoomLights_Continue(WifiClubRoomLights *arg0);
UnkStruct_ov44_02232914 ov44_02232E90(s32 arg0);
const u32 *ov44_02232E9C(u32 arg0);

#endif // POKEHEARTGOLD_OVY_44_02232E9C_H
