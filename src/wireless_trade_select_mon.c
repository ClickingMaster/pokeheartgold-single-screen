#include "wireless_trade_select_mon.h"

#include "global.h"

#include "constants/game_stats.h"
#include "constants/items.h"
#include "constants/pokemon.h"
#include "constants/sndseq.h"
#include "constants/species.h"

#include "msgdata/msg.naix"

#include "bag.h"
#include "bg_window.h"
#include "brightness.h"
#include "field_bgm.h"
#include "filesystem.h"
#include "font.h"
#include "game_stats.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "gf_rtc.h"
#include "heap.h"
#include "item.h"
#include "launch_application_internal.h"
#include "list_menu.h"
#include "list_menu_items.h"
#include "math_util.h"
#include "message_format.h"
#include "msgdata.h"
#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "options.h"
#include "party.h"
#include "player_data.h"
#include "pm_string.h"
#include "pokemon.h"
#include "pokemon_icon_idx.h"
#include "render_window.h"
#include "sav_chatot.h"
#include "save_palpad.h"
#include "save_special_ribbons.h"
#include "save_vars_flags.h"
#include "sound_chatot.h"
#include "sound_playback.h"
#include "sound_system.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "string_util.h"
#include "sys_vars.h"
#include "system.h"
#include "text.h"
#include "touchscreen.h"
#include "touchscreen_list_menu.h"
#include "trainer_memo.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_0200B150.h"
#include "unk_0200FA24.h"
#include "unk_02013FDC.h"
#include "unk_0202C034.h"
#include "unk_02030A98.h"
#include "unk_02034354.h"
#include "unk_02035900.h"
#include "unk_020379A0.h"
#include "unk_02037C94.h"
#include "unk_0203A3B0.h"
#include "unk_02088288.h"
#include "update_dex_received.h"
#include "vram_transfer_manager.h"
#include "yes_no_prompt.h"

#define WIRELESS_TRADE_NUM_SLOTS   12 // 6 own party slots, then the partner's 6
#define WIRELESS_TRADE_SLOT_CANCEL 12 // the cursor position of the "Quit" button
#define WIRELESS_TRADE_NUM_WINDOWS 34

// Party data travels in chunks of this size (the receive side is in trade_communication.c).
#define WIRELESS_TRADE_PARTY_CHUNK_SIZE 0x590
#define WIRELESS_TRADE_MON_PIC_SIZE     0xC80

// Communication commands (handled by the receive table in trade_communication.c).
#define WIRELESS_TRADE_CMD_PARTY_CHUNK 0x16
#define WIRELESS_TRADE_CMD_CURSOR      0x17
#define WIRELESS_TRADE_CMD_ANSWER      0x18
#define WIRELESS_TRADE_CMD_PALPAD      0x1C
#define WIRELESS_TRADE_CMD_CHATOT      0x1D
#define WIRELESS_TRADE_CMD_START_DELAY 0x1F
#define WIRELESS_TRADE_CMD_RIBBONS     0x20

// Answers both players exchange with WIRELESS_TRADE_CMD_ANSWER.
#define WIRELESS_TRADE_ANSWER_QUIT    1
#define WIRELESS_TRADE_ANSWER_OFFER   2
#define WIRELESS_TRADE_ANSWER_CONFIRM 3
#define WIRELESS_TRADE_ANSWER_REFUSE  4

// What the screen shows of one party slot.
typedef struct WirelessTradeMonInfo {
    u16 species;        // 0x0, 0: empty slot
    u16 heldItem;       // 0x2, 0: none, 1: item, 2: mail
    u8 ball;            // 0x4
    u8 isEgg;           // 0x5
    u8 form;            // 0x6
    u8 padding_7;       // 0x7
    u16 gender;         // 0x8
    u16 flip;           // 0xA, BASE_FLIP of the species
    u32 capsule;        // 0xC, ball capsule id
} WirelessTradeMonInfo; // size: 0x10

// A straight-line slide of a party icon to its place above the trade question.
typedef struct WirelessTradeSlide {
    fx32 startX;      // 0x00
    fx32 startY;      // 0x04
    u8 filler_08[4];  // 0x08
    fx32 endX;        // 0x0C
    fx32 endY;        // 0x10
    u8 filler_14[4];  // 0x14
} WirelessTradeSlide; // size: 0x18

// The partner's Chatot cry as received.
typedef struct WirelessTradeChatotData {
    u32 data[0xFB];
} WirelessTradeChatotData; // size: 0x3EC

typedef struct WirelessTradeSelectMon WirelessTradeSelectMon;
typedef int (*WirelessTradeSelectMonFunc)(WirelessTradeSelectMon *work);

struct WirelessTradeSelectMon {
    FieldSystem *fieldSystem;                                    // 0x0000
    SaveData *saveData;                                          // 0x0004
    WirelessTradeSelectMonArgs *args;                            // 0x0008
    PokemonSummaryArgs summaryArgs;                              // 0x000C
    u32 summaryOfPartner;                                        // 0x0048
    OverlayManager *summaryApp;                                  // 0x004C
    u32 summaryActive;                                           // 0x0050
    u32 state;                                                   // 0x0054
    int exchangeState;                                           // 0x0058
    int sentChunks;                                              // 0x005C
    int commState;                                               // 0x0060, written by trade_communication.c
    int receivedChunks;                                          // 0x0064
    u32 tradeAccepted;                                           // 0x0068
    u32 answers[2];                                              // 0x006C, per player, written by trade_communication.c
    u32 resultMsg;                                               // 0x0074
    u8 filler_78[0x1C];                                          // 0x0078
    int cursor;                                                  // 0x0094
    int partnerCursor;                                           // 0x0098
    u16 cursorBlinkAngle;                                        // 0x009C
    u8 filler_9E[0xAE];                                          // 0x009E
    u32 selectResult;                                            // 0x014C
    u8 filler_150[8];                                            // 0x0150
    int dpadDir;                                                 // 0x0158
    int partnerDpadDir;                                          // 0x015C
    u8 filler_160[0x18];                                         // 0x0160
    u32 unk_178;                                                 // 0x0178
    u32 unk_17C;                                                 // 0x017C
    BgConfig *bgConfig;                                          // 0x0180
    MessageFormat *msgFormat;                                    // 0x0184
    MessageFormat *nicknameFormat;                               // 0x0188
    MessageFormat *unk_18C;                                      // 0x018C
    MsgData *msgData;                                            // 0x0190
    u8 filler_194[4];                                            // 0x0194
    String *playerName;                                          // 0x0198
    String *itemLabel;                                           // 0x019C
    SpriteList *spriteList;                                      // 0x01A0
    G2dRenderer renderer;                                        // 0x01A4
    GF_2DGfxResMan *resMan[4];                                   // 0x02CC
    SpriteResource *resObjs[2][4];                               // 0x02DC, char/pltt/cell/anim, main and sub screen
    SpriteResourcesHeader resHeaders[2];                         // 0x02FC
    Sprite *cursorSprites[2];                                    // 0x0344
    u8 filler_34C[0x30];                                         // 0x034C
    Sprite *iconSprites[WIRELESS_TRADE_NUM_SLOTS];               // 0x037C
    Sprite *itemSprites[WIRELESS_TRADE_NUM_SLOTS];               // 0x03AC
    Sprite *capsuleSprites[WIRELESS_TRADE_NUM_SLOTS];            // 0x03DC
    Sprite *detailSprites[6];                                    // 0x040C, mon pic, ball, gender; own and partner
    Sprite *tradeSprite;                                         // 0x0424
    u8 filler_428[0x1C];                                         // 0x0428
    Window windows[WIRELESS_TRADE_NUM_WINDOWS];                  // 0x0444
    u8 unk_664[4];                                               // 0x0664
    ListMenuItem *menuItems;                                     // 0x0668
    u8 filler_66C[4];                                            // 0x066C
    u32 yesNoState;                                              // 0x0670
    u32 unk_674;                                                 // 0x0674
    u8 filler_678[0x24];                                         // 0x0678
    WirelessTradeMonInfo mons[WIRELESS_TRADE_NUM_SLOTS + 1];     // 0x069C, the last one is the "Quit" button
    u8 filler_76C[0x30];                                         // 0x076C
    void *iconCharBuffers[WIRELESS_TRADE_NUM_SLOTS];             // 0x079C
    NNSG2dCharacterData *iconCharData[WIRELESS_TRADE_NUM_SLOTS]; // 0x07CC
    u8 monPics[2][WIRELESS_TRADE_MON_PIC_SIZE];                  // 0x07FC
    PokepicTemplate monPicTemplates[2];                          // 0x20FC
    int pendingMonPic;                                           // 0x211C, 1 + side of the pic to upload in vblank
    u8 filler_2120[0x100];                                       // 0x2120
    WirelessTradeSelectMonFunc callback;                         // 0x2220
    Party *party;                                                // 0x2224
    Party *partnerParty;                                         // 0x2228
    u32 unk_222C;                                                // 0x222C
    SavePalPad *palPad;                                          // 0x2230
    SavePalPad palPadPacket;                                     // 0x2234
    u8 filler_22BC[8];                                           // 0x22BC
    int bgScrollY;                                               // 0x22C4
    u32 slideStep;                                               // 0x22C8
    int startDelay;                                              // 0x22CC, written by trade_communication.c
    WirelessTradeSlide slides[2];                                // 0x22D0
    u8 partyChunks[2][WIRELESS_TRADE_PARTY_CHUNK_SIZE];          // 0x2300, receive buffers
    WirelessTradeChatotData chatot[2];                           // 0x2E20
    u32 friendStates[32];                                        // 0x35F8
    int friendNetId;                                             // 0x3678
    int chosenRosterSlot;                                        // 0x367C
    MessageFormat *rosterMsgFormat;                              // 0x3680
    MsgData *rosterMsgData;                                      // 0x3684
    ListMenuItem *rosterItems;                                   // 0x3688
    ListMenu *rosterMenu;                                        // 0x368C
    Window rosterWindow;                                         // 0x3690
    UnkStruct_021D2230 *roster;                                  // 0x36A0
    u32 tradesDone;                                              // 0x36A4
    s32 unk_36A8;                                                // 0x36A8
    int lastSentValue;                                           // 0x36AC
    int lastSentCommand;                                         // 0x36B0
    WaitingIcon *waitingIcon;                                    // 0x36B4
    u32 timeoutActive;                                           // 0x36B8
    int timeoutCounter;                                          // 0x36BC
    YesNoPrompt *yesNoPrompt;                                    // 0x36C0
    TouchscreenListMenuSpawner *touchMenuSpawner;                // 0x36C4
    TouchscreenListMenu *touchMenu;                              // 0x36C8
}; // size: 0x36CC

typedef struct WirelessTradeSlotPos {
    int x;
    int y;
} WirelessTradeSlotPos;

typedef struct WirelessTradeDetailSpritePos {
    int x;
    int y;
    int animSeq;
} WirelessTradeDetailSpritePos;

typedef struct WirelessTradeWindowPos {
    u16 x;
    u16 y;
} WirelessTradeWindowPos;

typedef struct WirelessTradeWindowTemplate {
    u16 x;
    u16 y;
    u16 width;
    u16 height;
    u16 baseTile;
} WirelessTradeWindowTemplate;

// Defined in trade_communication.c, whose internal header declares them with its own view of the work.
void sub_0208F658(FieldSystem *fieldSystem);
void sub_0208F7E0(FieldSystem *fieldSystem, WirelessTradeSelectMon *work);

// Defined in friend_roster.c, frontier_save.c, communication_player_state.c, communication_manager_9.c and
// network_communication_state_full.c; not declared in their public headers.
BOOL sub_0202C2DC(UnkStruct_021D2230 *roster, int index);
void sub_0202C338(UnkStruct_021D2230 *roster, int index);
void sub_020311AC(FrontierSave *frontier, int row);
void sub_02034AEC(SaveData *saveData, int a1);
int sub_02036FD8(int command, void *data, int size);
void sub_020399FC(enum HeapID heapID, BgConfig *bgConfig);
void sub_02039EAC(s32 *a0);
s32 sub_02039EB4(SaveData *saveData, s32 a1, s32 *a2);
void sub_0203A280(SaveData *saveData, s32 netId, s32 slot, enum HeapID heapID, u32 a4);

extern const u8 sWirelessTradeSelectMonSummaryPages[];
extern const TouchscreenListMenuTemplate sWirelessTradeSelectMonTouchMenuTemplate;
extern const ObjCharTransferTemplate sWirelessTradeSelectMonObjCharTransferTemplate;
extern const GraphicsModes sWirelessTradeSelectMonGraphicsModes;
// The anim sequence table is [side][own slot, partner slot, quit]; ov65_0221FD6C/70 label its later columns.
extern const u32 sWirelessTradeSelectMonCursorAnims[][3];
extern const u32 ov65_0221FD6C[][3];
extern const u32 ov65_0221FD70[][3];
extern const BgTemplate sWirelessTradeSelectMonBgTemplate_Sub0;
extern const BgTemplate sWirelessTradeSelectMonBgTemplate_Sub1;
extern const BgTemplate sWirelessTradeSelectMonBgTemplate_Main1;
extern const BgTemplate sWirelessTradeSelectMonBgTemplate_Sub2;
extern const BgTemplate sWirelessTradeSelectMonBgTemplate_Main0;
extern const BgTemplate sWirelessTradeSelectMonBgTemplate_Main2;
extern const BgTemplate sWirelessTradeSelectMonBgTemplate_Main3;
extern const GraphicsBanks sWirelessTradeSelectMonGraphicsBanks;
extern const TouchscreenHitbox sWirelessTradeSelectMonSlotHitboxes[];
// ov65_0221FEA8 labels the y member of sWirelessTradeSelectMonDetailSpritePos[0].
extern const WirelessTradeDetailSpritePos sWirelessTradeSelectMonDetailSpritePos[];
extern const WirelessTradeDetailSpritePos ov65_0221FEA8[];
extern const u32 sWirelessTradeSelectMonBallPalettes[];
// ov65_0221FF50 labels the y member of sWirelessTradeSelectMonSlotPos[0].
extern const WirelessTradeSlotPos sWirelessTradeSelectMonSlotPos[];
extern const WirelessTradeSlotPos ov65_0221FF50[];
extern const u8 sWirelessTradeSelectMonCursorMoves[][4][6];
extern const ListMenuTemplate sWirelessTradeSelectMonRosterListTemplate;
extern const WirelessTradeWindowPos sWirelessTradeSelectMonNameWindowPos[];
extern const WirelessTradeWindowTemplate sWirelessTradeSelectMonDetailWindows[];

void WirelessTradeSelectMon_LoadMonIconGfx(NNSG2dCharacterData *charData, u32 species, u32 form, u32 isEgg, u32 slot, Sprite *sprite);
void WirelessTradeSelectMon_PrintPlayerNames(WirelessTradeSelectMon *work);
void WirelessTradeSelectMon_ReloadScreen(WirelessTradeSelectMon *work);
void WirelessTradeSelectMon_PrintPartyNicknames(WirelessTradeSelectMon *work);
void WirelessTradeSelectMon_BufferPartyNicknames(MessageFormat *msgFormat, Party *party, u32 firstSlot);
void WirelessTradeSelectMon_PrintNicknameList(Window *window, MessageFormat *msgFormat, MsgData *msgData, Party *party, int firstSlot);
void WirelessTradeSelectMon_LoadPartyIcons(Party *party, int firstSlot, WirelessTradeSelectMon *work);
BOOL WirelessTradeSelectMon_ExchangeData(WirelessTradeSelectMon *work);
void WirelessTradeSelectMon_GetMonInfo(Pokemon *mon, WirelessTradeMonInfo *info);
void WirelessTradeSelectMon_SetBallSprite(WirelessTradeSelectMon *work, int index, int ball);
void WirelessTradeSelectMon_ShowPartnerMon(WirelessTradeSelectMon *work, int slot);
void WirelessTradeSelectMon_ShowSelectedMon(WirelessTradeSelectMon *work);
int WirelessTradeSelectMon_RunSelect(WirelessTradeSelectMon *work);
void WirelessTradeSelectMon_FreeSprites(WirelessTradeSelectMon *work);
void WirelessTradeSelectMon_FreeIconCharData(WirelessTradeSelectMon *work);
void WirelessTradeSelectMon_VBlankCB(WirelessTradeSelectMon *work);
void WirelessTradeSelectMon_SetGfxBanks(void);
void WirelessTradeSelectMon_InitBgs(BgConfig *bgConfig);
void WirelessTradeSelectMon_InitWork(WirelessTradeSelectMon *work, OverlayManager *man);
void WirelessTradeSelectMon_FreeBgs(BgConfig *bgConfig);
void WirelessTradeSelectMon_LoadScreenData(NARC *narc, int memberNo, BgConfig *bgConfig, int layer, int unused, u32 size, BOOL isCompressed, enum HeapID heapID);
void WirelessTradeSelectMon_CommitNameBg(BgConfig *bgConfig);
void WirelessTradeSelectMon_LoadBgGraphics(WirelessTradeSelectMon *work, NARC *narc);
void WirelessTradeSelectMon_InitObjTransfer(void);
void WirelessTradeSelectMon_InitSpriteResources(WirelessTradeSelectMon *work, NARC *narc);
u32 WirelessTradeSelectMon_LoadMonPic(u32 side, Pokemon *mon, void *dest, PokepicTemplate *pokepic);
void WirelessTradeSelectMon_PrintMonNickname(Window *window, Party *party, u32 slot, u32 unused, u32 x);
int WirelessTradeSelectMon_GetGenderSymbol(WirelessTradeMonInfo *info, Party *party, u32 slot, int gender);
void WirelessTradeSelectMon_ShowMonDetails(Window *windows, int side, Party *party, int slot, WirelessTradeSelectMon *work);
void WirelessTradeSelectMon_HideMonDetails(Window *windows, int side, WirelessTradeSelectMon *work);
void WirelessTradeSelectMon_CreateSprites(WirelessTradeSelectMon *work);
void WirelessTradeSelectMon_ReadDpad(int *dir);
void WirelessTradeSelectMon_PlayCryOnTouch(WirelessTradeSelectMon *work);
void WirelessTradeSelectMon_UpdateCursorBlink(u16 *angle);
u8 WirelessTradeSelectMon_GetNextCursorPos(int cursor, int dir, WirelessTradeMonInfo *mons);
void WirelessTradeSelectMon_SetCursorSprite(int cursor, Sprite *sprite, int side);
int WirelessTradeSelectMon_MoveCursor(int *dpadDir, int *cursor, Sprite *sprite, WirelessTradeMonInfo *mons, int side);
int WirelessTradeSelectMon_SendCommand(u32 unused, u32 command, u8 value);
void WirelessTradeSelectMon_SendCommandIfChanged(WirelessTradeSelectMon *work, int command, int value);
void *WirelessTradeSelectMon_GetPartyChunk(void *party, u32 chunk);
void WirelessTradeSelectMon_SendPartyChunk(u32 netId, void *party, u32 chunk);
int WirelessTradeSelectMon_SendSpecialRibbons(SaveData *saveData);
s32 WirelessTradeSelectMon_SendPalPad(PlayerProfile *profile, SavePalPad *palPad, SavePalPad *packet);
int WirelessTradeSelectMon_SendChatot(SOUND_CHATOT *chatot);
s32 WirelessTradeSelectMon_HandleKeys(WirelessTradeSelectMon *work);
s32 WirelessTradeSelectMon_HandleTouch(WirelessTradeSelectMon *work);
int WirelessTradeSelectMon_SelectMon(WirelessTradeSelectMon *work);
void WirelessTradeSelectMon_OpenSummary(WirelessTradeSelectMon *work, u32 ofPartner);
int WirelessTradeSelectMon_AskQuit(WirelessTradeSelectMon *work);
int WirelessTradeSelectMon_HandleQuitPrompt(WirelessTradeSelectMon *work);
int WirelessTradeSelectMon_ShowResultMessage(WirelessTradeSelectMon *work);
int WirelessTradeSelectMon_WaitResultMessage(WirelessTradeSelectMon *work);
void WirelessTradeSelectMon_PrintRosterMessage(WirelessTradeSelectMon *work, u32 msgId);
int WirelessTradeSelectMon_HandleGiveUpPrompt(WirelessTradeSelectMon *work);
int WirelessTradeSelectMon_HandleDeletePrompt(WirelessTradeSelectMon *work);
int WirelessTradeSelectMon_HandleRosterListInput(WirelessTradeSelectMon *work);
int WirelessTradeSelectMon_ShowRosterList(WirelessTradeSelectMon *work);
int WirelessTradeSelectMon_HandleRosterFullPrompt(WirelessTradeSelectMon *work);
int WirelessTradeSelectMon_HandleRegisterPrompt(WirelessTradeSelectMon *work);
int WirelessTradeSelectMon_WaitQuitSync(WirelessTradeSelectMon *work);
int WirelessTradeSelectMon_RegisterNextFriend(WirelessTradeSelectMon *work);
int WirelessTradeSelectMon_StartFriendRegistration(WirelessTradeSelectMon *work);
int WirelessTradeSelectMon_WaitQuitAnswers(WirelessTradeSelectMon *work);
TouchscreenListMenu *WirelessTradeSelectMon_CreateTouchMenu(WirelessTradeSelectMon *work, u32 numItems, u32 y);
int WirelessTradeSelectMon_ShowOwnMonMenu(WirelessTradeSelectMon *work);
int WirelessTradeSelectMon_HandleOwnMonMenu(WirelessTradeSelectMon *work);
int WirelessTradeSelectMon_OfferTrade(WirelessTradeSelectMon *work);
int WirelessTradeSelectMon_WaitOfferAnswers(WirelessTradeSelectMon *work);
int WirelessTradeSelectMon_StartTradeSlide(WirelessTradeSelectMon *work);
void WirelessTradeSelectMon_InitSlide(WirelessTradeSlide *slide, s32 startX, s32 startY, s32 endX, s32 endY);
void WirelessTradeSelectMon_SlideMonIcon(Sprite *icon, Sprite *item, Sprite *capsule, s32 step, const WirelessTradeSlide *slide, const WirelessTradeMonInfo *info);
int WirelessTradeSelectMon_UpdateTradeSlide(WirelessTradeSelectMon *work);
void WirelessTradeSelectMon_RestoreIcons(WirelessTradeSelectMon *work);
int WirelessTradeSelectMon_AskTradeConfirm(WirelessTradeSelectMon *work);
void WirelessTradeSelectMon_AcceptTrade(WirelessTradeSelectMon *work);
void WirelessTradeSelectMon_PrintMessageInWindow(WirelessTradeSelectMon *work, u32 windowIndex, u32 msgId);
int WirelessTradeSelectMon_HandleTradeConfirm(WirelessTradeSelectMon *work);
int WirelessTradeSelectMon_AskDetachCapsule(WirelessTradeSelectMon *work);
int WirelessTradeSelectMon_HandleCapsulePrompt(WirelessTradeSelectMon *work);
int WirelessTradeSelectMon_WaitConfirmAnswers(WirelessTradeSelectMon *work);
int WirelessTradeSelectMon_FinishTrade(WirelessTradeSelectMon *work);
int WirelessTradeSelectMon_ShowPartnerMonMenu(WirelessTradeSelectMon *work);
void WirelessTradeSelectMon_DestroyTouchMenu(WirelessTradeSelectMon *work);
int WirelessTradeSelectMon_HandlePartnerMonMenu(WirelessTradeSelectMon *work);
void WirelessTradeSelectMon_TradeMons(Party *party, Party *partnerParty, u32 slot, u32 partnerSlot, WirelessTradeSelectMonArgs *args);
void WirelessTradeSelectMon_ScrollBg(WirelessTradeSelectMon *work);
void WirelessTradeSelectMon_SetSpritePos(Sprite *sprite, s32 x, s32 y);
void WirelessTradeSelectMon_ShowWaitingIcon(WirelessTradeSelectMon *work);
void WirelessTradeSelectMon_HideWaitingIcon(WirelessTradeSelectMon *work);
s32 WirelessTradeSelectMon_HasOtherNonEggMon(WirelessTradeSelectMon *work);
s32 WirelessTradeSelectMon_FindBadEggParty(WirelessTradeSelectMon *work);
void WirelessTradeSelectMon_StartTimeout(WirelessTradeSelectMon *work);
void WirelessTradeSelectMon_UpdateTimeout(WirelessTradeSelectMon *work);
void WirelessTradeSelectMon_StartMenuDim(void);
void WirelessTradeSelectMon_EndMenuDim(void);
void WirelessTradeSelectMon_AddWindows(BgConfig *bgConfig, Window *windows, Options *options);
void WirelessTradeSelectMon_RemoveWindows(Window *windows);
void WirelessTradeSelectMon_PrintString(Window *window, String *string, int unused, int textSpeed, int x, int y);
u32 WirelessTradeSelectMon_PrintMessage(Window *window, u32 msgId, u32 frame, MsgData *msgData, MessageFormat *msgFormat);
YesNoPrompt *WirelessTradeSelectMon_CreateYesNoPrompt(BgConfig *bgConfig);
s32 WirelessTradeSelectMon_HandleYesNoPrompt(BgConfig *bgConfig, void *unused, u32 *state, YesNoPrompt **prompt, u32 dim);
ListMenu *WirelessTradeSelectMon_CreateRosterListMenu(ListMenuItem *items, u32 count, Window *window, BgConfig *bgConfig);
void WirelessTradeSelectMon_RosterListCursorCB(ListMenu *list, s32 index, u8 onInit);

BOOL WirelessTradeSelectMon_Init(OverlayManager *man, int *state) {
    NARC *narc;
    WirelessTradeSelectMon *work;

    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    GX_SetVisiblePlane(0);
    GXS_SetVisiblePlane(0);
    Heap_Create(HEAP_ID_3, HEAP_ID_26, 0x70FA0);
    FontID_Alloc(4, HEAP_ID_26);
    narc = NARC_New(NARC_a_0_5_2, HEAP_ID_26);
    work = OverlayManager_CreateAndGetData(man, sizeof(WirelessTradeSelectMon), HEAP_ID_26);
    MI_CpuClearFast(work, sizeof(WirelessTradeSelectMon));
    work->bgConfig = BgConfig_Alloc(HEAP_ID_26);
    work->msgFormat = MessageFormat_New_Custom(12, 22, HEAP_ID_26);
    work->nicknameFormat = MessageFormat_New(HEAP_ID_26);
    work->unk_18C = MessageFormat_New(HEAP_ID_26);
    work->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0183_bin, HEAP_ID_26);
    work->summaryApp = NULL;
    WirelessTradeSelectMon_InitWork(work, man);
    SetKeyRepeatTimers(4, 8);
    WirelessTradeSelectMon_SetGfxBanks();
    WirelessTradeSelectMon_InitBgs(work->bgConfig);
    BeginNormalPaletteFade(0, 1, 1, RGB_BLACK, 16, 1, HEAP_ID_26);
    WirelessTradeSelectMon_LoadBgGraphics(work, narc);
    Main_SetVBlankIntrCB((GFIntrCB)WirelessTradeSelectMon_VBlankCB, work);
    WirelessTradeSelectMon_InitObjTransfer();
    WirelessTradeSelectMon_InitSpriteResources(work, narc);
    WirelessTradeSelectMon_CreateSprites(work);
    WirelessTradeSelectMon_AddWindows(work->bgConfig, work->windows, work->args->options);
    {
        FieldSystem *fieldSystem = work->args->fieldSystem;
        work->fieldSystem = fieldSystem;
        sub_0208F7E0(fieldSystem, work);
    }
    WirelessTradeSelectMon_PrintMessage(&work->windows[23], 20, 1, work->msgData, work->msgFormat); // "Communicating. Please stand by..."
    sub_0208F658(work->fieldSystem);
    ToggleBgLayer(GF_BG_LYR_MAIN_0, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    sub_020399FC(HEAP_ID_26, work->bgConfig);
    WirelessStatusIcon_Create();
    IsNighttime();
    sub_02055198(NULL, SEQ_GS_POKESEN);
    sub_02005448(SEQ_GS_POKESEN);
    NARC_Delete(narc);
    return TRUE;
}

void WirelessTradeSelectMon_LoadMonIconGfx(NNSG2dCharacterData *charData, u32 species, u32 form, u32 isEgg, u32 slot, Sprite *sprite) {
    GX_LoadOBJ(charData->pRawData, (slot * 16 + 0x92) * 32, 0x200);
    Sprite_SetPalOffset(sprite, GetMonIconPaletteEx(species, form, isEgg) + 10);
}

void WirelessTradeSelectMon_PrintPlayerNames(WirelessTradeSelectMon *work) {
    PlayerProfile *profile = sub_02034818(Comm_GetLocalNetId());
    PlayerProfile *partnerProfile = sub_02034818(Comm_GetLocalNetId() ^ 1);
    String *name = PlayerProfile_GetPlayerName_NewString(profile, HEAP_ID_26);
    String *partnerName = PlayerProfile_GetPlayerName_NewString(partnerProfile, HEAP_ID_26);
    String *quit = NewString_ReadMsgData(work->msgData, 50); // "Quit"

    WirelessTradeSelectMon_PrintString(&work->windows[0], name, 10, 0, 1, 1);
    WirelessTradeSelectMon_PrintString(&work->windows[1], partnerName, 10, 0, 1, 1);
    WirelessTradeSelectMon_PrintString(&work->windows[6], quit, 5, 0, 1, 1);
    String_Delete(quit);
    String_Delete(partnerName);
    String_Delete(name);
}

void WirelessTradeSelectMon_ReloadScreen(WirelessTradeSelectMon *work) {
    NARC *narc = NARC_New(NARC_a_0_5_2, HEAP_ID_26);

    // The original call passes the narc to the parameterless function.
    ((void (*)(NARC *))WirelessTradeSelectMon_SetGfxBanks)(narc);
    WirelessTradeSelectMon_InitBgs(work->bgConfig);
    WirelessTradeSelectMon_LoadBgGraphics(work, narc);
    WirelessTradeSelectMon_PrintPartyNicknames(work);
    WirelessTradeSelectMon_PrintPlayerNames(work);
    LoadUserFrameGfx2(work->bgConfig, GF_BG_LYR_MAIN_0, 985, 10, Options_GetFrame(work->args->options), HEAP_ID_26);
    LoadUserFrameGfx1(work->bgConfig, GF_BG_LYR_MAIN_0, 1015, 11, 0, HEAP_ID_26);
    WirelessTradeSelectMon_PrintMessage(&work->windows[21], 15, 1, work->msgData, work->msgFormat); // "Choose a Pokémon."
    ToggleBgLayer(GF_BG_LYR_SUB_0, GF_PLANE_TOGGLE_ON);
    ToggleBgLayer(GF_BG_LYR_SUB_1, GF_PLANE_TOGGLE_ON);
    ToggleBgLayer(GF_BG_LYR_SUB_2, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    ToggleBgLayer(GF_BG_LYR_MAIN_0, GF_PLANE_TOGGLE_ON);
    ToggleBgLayer(GF_BG_LYR_MAIN_1, GF_PLANE_TOGGLE_ON);
    ToggleBgLayer(GF_BG_LYR_MAIN_2, GF_PLANE_TOGGLE_ON);
    ToggleBgLayer(GF_BG_LYR_MAIN_3, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    WirelessTradeSelectMon_InitObjTransfer();
    WirelessTradeSelectMon_InitSpriteResources(work, narc);
    WirelessTradeSelectMon_CreateSprites(work);
    WirelessTradeSelectMon_LoadPartyIcons(work->party, 0, work);
    WirelessTradeSelectMon_LoadPartyIcons(work->partnerParty, 6, work);
    Sprite_SetDrawFlag(work->cursorSprites[0], TRUE);
    Sprite_SetDrawFlag(work->cursorSprites[1], TRUE);
    Main_SetVBlankIntrCB((GFIntrCB)WirelessTradeSelectMon_VBlankCB, work);
    NARC_Delete(narc);
}

void WirelessTradeSelectMon_PrintPartyNicknames(WirelessTradeSelectMon *work) {
    int count;
    int partnerCount;

    WirelessTradeSelectMon_BufferPartyNicknames(work->msgFormat, work->party, 0);
    WirelessTradeSelectMon_BufferPartyNicknames(work->msgFormat, work->partnerParty, 6);
    WirelessTradeSelectMon_PrintNicknameList(&work->windows[7], work->msgFormat, work->msgData, work->party, 0);
    WirelessTradeSelectMon_PrintNicknameList(&work->windows[13], work->msgFormat, work->msgData, work->partnerParty, 6);
    count = Party_GetCount(work->party);
    partnerCount = Party_GetCount(work->partnerParty);
    // The original call passes both party counts, which the function ignores.
    ((void (*)(BgConfig *, int, int))WirelessTradeSelectMon_CommitNameBg)(work->bgConfig, count, partnerCount);
}

BOOL WirelessTradeSelectMon_Main(OverlayManager *man, int *state) {
    WirelessTradeSelectMon *work = OverlayManager_GetData(man);
    int ret = FALSE;

    switch (*state) {
    case 0:
        if (IsPaletteFadeFinished()) {
            *state = 1;
            WirelessTradeSelectMon_PrintPlayerNames(work);
        }
        break;
    case 1:
        switch (work->state) {
        case 0:
            work->state = WirelessTradeSelectMon_ExchangeData(work);
            break;
        case 1:
            work->state = WirelessTradeSelectMon_RunSelect(work);
            break;
        case 2:
            BeginNormalPaletteFade(0, 0, 0, RGB_BLACK, 8, 1, HEAP_ID_26);
            *state = 2;
            break;
        case 3:
            BeginNormalPaletteFade(0, 0, 0, RGB_BLACK, 8, 1, HEAP_ID_26);
            work->state = 4;
            break;
        case 4:
            if (IsPaletteFadeFinished()) {
                WirelessTradeSelectMon_FreeIconCharData(work);
                WirelessTradeSelectMon_FreeSprites(work);
                WirelessTradeSelectMon_FreeBgs(work->bgConfig);
                work->summaryActive = TRUE;
                work->state = 5;
                WirelessTradeSelectMon_OpenSummary(work, work->cursor / 6);
            }
            break;
        case 5:
            if (OverlayManager_Run(work->summaryApp)) {
                OverlayManager_Delete(work->summaryApp);
                WirelessTradeSelectMon_ReloadScreen(work);
                work->summaryActive = FALSE;
                work->cursor = work->summaryArgs.partySlot + 6 * work->summaryOfPartner;
                WirelessTradeSelectMon_ShowSelectedMon(work);
                WirelessTradeSelectMon_SetCursorSprite(work->cursor, work->cursorSprites[0], 0);
                WirelessStatusIcon_Create();
                work->state = 6;
            }
            break;
        case 6:
            BeginNormalPaletteFade(0, 1, 1, RGB_BLACK, 8, 1, HEAP_ID_26);
            work->state = 7;
            break;
        case 7:
            if (IsPaletteFadeFinished()) {
                work->state = 1;
            }
            break;
        }
        break;
    case 2:
        if (IsPaletteFadeFinished()) {
            ret = TRUE;
        }
        break;
    }
    if (work->summaryActive == FALSE) {
        WirelessTradeSelectMon_ScrollBg(work);
        SpriteList_RenderAndAnimateSprites(work->spriteList);
    }
    sub_020399FC(HEAP_ID_26, work->bgConfig);
    return ret;
}

void WirelessTradeSelectMon_BufferPartyNicknames(MessageFormat *msgFormat, Party *party, u32 firstSlot) {
    int i;

    for (i = 0; i < Party_GetCount(party); i++) {
        BufferBoxMonNickname(msgFormat, i + firstSlot, Mon_GetBoxMon(Party_GetMonByIndex(party, i)));
    }
}

void WirelessTradeSelectMon_PrintNicknameList(Window *window, MessageFormat *msgFormat, MsgData *msgData, Party *party, int firstSlot) {
    int i;
    String *string;

    for (i = 0; i < Party_GetCount(party); i++) {
        string = ReadMsgData_ExpandPlaceholders(msgFormat, msgData, firstSlot + 1 + i, HEAP_ID_26);
        WirelessTradeSelectMon_PrintString(window, string, 8, 0, 1, 0);
        String_Delete(string);
        window++;
    }
}

#pragma force_active on

void WirelessTradeSelectMon_LoadPartyIcons(Party *party, int firstSlot, WirelessTradeSelectMon *work) {
    int i;

    for (i = 0; i < Party_GetCount(party); i++) {
        int slot = i + firstSlot;
        work->iconCharBuffers[slot] = GfGfxLoader_GetCharData(NARC_poketool_icongra_poke_icon, GetMonIconNaixEx(work->mons[slot].species, work->mons[slot].isEgg, work->mons[slot].form), FALSE, &work->iconCharData[slot], HEAP_ID_26);
        DC_FlushRange(work->iconCharData[slot]->pRawData, 0x200);
        WirelessTradeSelectMon_LoadMonIconGfx(work->iconCharData[slot], work->mons[slot].species, work->mons[slot].form, work->mons[slot].isEgg, slot, work->iconSprites[slot]);
        Sprite_SetDrawFlag(work->iconSprites[slot], TRUE);
        if (work->mons[slot].heldItem == 0) {
            Sprite_SetDrawFlag(work->itemSprites[slot], FALSE);
        } else {
            Sprite_SetDrawFlag(work->itemSprites[slot], TRUE);
            Sprite_SetAnimCtrlSeq(work->itemSprites[slot], work->mons[slot].heldItem + 2);
        }
        if (work->mons[slot].capsule == 0) {
            Sprite_SetDrawFlag(work->capsuleSprites[slot], FALSE);
        } else {
            Sprite_SetDrawFlag(work->capsuleSprites[slot], TRUE);
            Sprite_SetAnimCtrlSeq(work->capsuleSprites[slot], 21);
        }
    }
    for (; i < PARTY_SIZE; i++) {
        Sprite_SetDrawFlag(work->iconSprites[firstSlot + i], FALSE);
        Sprite_SetDrawFlag(work->itemSprites[firstSlot + i], FALSE);
        Sprite_SetDrawFlag(work->capsuleSprites[firstSlot + i], FALSE);
    }
}

#pragma force_active reset

BOOL WirelessTradeSelectMon_ExchangeData(WirelessTradeSelectMon *work) {
    int i;

    WirelessTradeSelectMon_UpdateTimeout(work);
    switch (work->exchangeState) {
    case 0:
        sub_02037AC0(0x50);
        sub_0201A728(2);
        WirelessTradeSelectMon_ShowWaitingIcon(work);
        i = 0;
        if (Party_GetCount(work->args->party) > 0) {
            do {
                Pokemon *mon = Party_GetMonByIndex(work->args->party, i);
                if (GetMonData(mon, MON_DATA_SPECIES_OR_EGG, NULL) == SPECIES_SHAYMIN && GetMonData(mon, MON_DATA_FORM, NULL) != 0) {
                    Mon_UpdateShayminForm(mon, 0);
                }
                i++;
            } while (i < Party_GetCount(work->args->party));
        }
        work->exchangeState++;
        break;
    case 1:
        if (sub_02037B38(0x50)) {
            if (work->args->unk30 == 0) {
                work->exchangeState = 6;
            } else {
                work->exchangeState = 2;
            }
            if (Comm_GetLocalNetId() == 0) {
                // The parent picks a random delay before the data exchange.
                u16 delay = (s32)LCRandom() % 60;
                ((int (*)(int, int, int))WirelessTradeSelectMon_SendCommand)(Comm_GetLocalNetId(), WIRELESS_TRADE_CMD_START_DELAY, delay + 3);
            }
            WirelessTradeSelectMon_SendSpecialRibbons(work->saveData);
            WirelessTradeSelectMon_StartTimeout(work);
        }
        break;
    case 2:
        if (work->startDelay != 0) {
            work->exchangeState++;
        }
        break;
    case 3:
        work->startDelay--;
        if (work->startDelay == 0) {
            work->exchangeState = 4;
        }
        break;
    case 4:
        sub_02039EAC(&work->unk_36A8);
        work->exchangeState++;
        break;
    case 5:
        if (sub_02039EB4(work->saveData, 2, &work->unk_36A8)) {
            work->exchangeState++;
        }
        break;
    case 6:
        sub_020378E4(1);
        sub_02037AC0(0x51);
        work->exchangeState++;
        break;
    case 7:
        if (sub_02037B38(0x51)) {
            work->exchangeState++;
        }
        break;
    case 8:
        work->sentChunks = 0;
        work->commState = 0;
        work->receivedChunks = 0;
        if (Comm_GetLocalNetId() == 1) {
            WirelessTradeSelectMon_SendPartyChunk(Comm_GetLocalNetId(), work->party, work->sentChunks);
            work->sentChunks++;
        }
        work->exchangeState++;
        break;
    case 9:
        if (work->commState != 0) {
            work->exchangeState = 10;
        }
        break;
    case 10:
        work->exchangeState++;
        WirelessTradeSelectMon_PrintPartyNicknames(work);
        break;
    case 11: {
        int j;
        u16 species;
        u8 *shifted;
        // The work pointer shifted by j entries: mons[0] through it is mons[j].
        shifted = (u8 *)work;
        j = 0;
        species = j;
        do {
            j++;
            ((WirelessTradeSelectMon *)shifted)->mons[0].species = species;
            shifted += sizeof(WirelessTradeMonInfo);
        } while (j < WIRELESS_TRADE_NUM_SLOTS + 1);
        i = species;
    }
        if (Party_GetCount(work->party) > 0) {
            WirelessTradeMonInfo *info = work->mons;
            do {
                WirelessTradeSelectMon_GetMonInfo(Party_GetMonByIndex(work->party, i), info);
                info++;
                i++;
            } while (i < Party_GetCount(work->party));
        }
        i = 0;
        if (Party_GetCount(work->partnerParty) > 0) {
            do {
                WirelessTradeSelectMon_GetMonInfo(Party_GetMonByIndex(work->partnerParty, i), &work->mons[i + 6]);
                i++;
            } while (i < Party_GetCount(work->partnerParty));
        }
        work->mons[WIRELESS_TRADE_SLOT_CANCEL].species = 1;
        work->exchangeState++;
        break;
    case 12:
        WirelessTradeSelectMon_SendPalPad(sub_02034818(Comm_GetLocalNetId()), work->palPad, &work->palPadPacket);
        work->exchangeState++;
        break;
    case 13:
        if (work->commState == 3) {
            work->exchangeState++;
        }
        break;
    case 14:
        WirelessTradeSelectMon_SendChatot(Save_Chatot_Get(work->saveData));
        work->exchangeState++;
        break;
    case 15:
        if (work->commState == 4) {
            work->exchangeState++;
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_OFF);
            WirelessTradeSelectMon_LoadPartyIcons(work->party, 0, work);
            WirelessTradeSelectMon_LoadPartyIcons(work->partnerParty, 6, work);
            // Walks the work pointer by one sprite slot: cursorSprites[0] through it is cursorSprites[i].
            for (i = 0; i < 2; i++, work = (WirelessTradeSelectMon *)((u8 *)work + sizeof(Sprite *))) {
                Sprite_SetDrawFlag(work->cursorSprites[0], TRUE);
            }
        }
        break;
    case 16:
        StartBrightnessTransition(8, 0, -16, (GXBlendPlaneMask)0x1E, 1);
        ToggleBgLayer(GF_BG_LYR_MAIN_1, GF_PLANE_TOGGLE_ON);
        ToggleBgLayer(GF_BG_LYR_MAIN_2, GF_PLANE_TOGGLE_ON);
        ToggleBgLayer(GF_BG_LYR_MAIN_3, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
        work->exchangeState++;
        break;
    case 17:
        if (IsBrightnessTransitionActive(1)) {
            work->pendingMonPic = WirelessTradeSelectMon_LoadMonPic(0, Party_GetMonByIndex(work->party, 0), work->monPics[0], &work->monPicTemplates[0]);
            WirelessTradeSelectMon_ShowMonDetails(work->windows, 0, work->party, 0, work);
            work->exchangeState++;
        }
        break;
    case 18:
        StartBrightnessTransition(8, 0, -16, (GXBlendPlaneMask)0x17, 2);
        ToggleBgLayer(GF_BG_LYR_SUB_0, GF_PLANE_TOGGLE_ON);
        ToggleBgLayer(GF_BG_LYR_SUB_1, GF_PLANE_TOGGLE_ON);
        ToggleBgLayer(GF_BG_LYR_SUB_2, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
        work->exchangeState++;
        WirelessTradeSelectMon_HideWaitingIcon(work);
        break;
    case 19:
        if (IsBrightnessTransitionActive(2)) {
            ClearFrameAndWindow2(&work->windows[23], TRUE);
            FillBgTilemapRect(work->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, 0, 32, 24, 0);
            WirelessTradeSelectMon_PrintMessage(&work->windows[21], 15, 1, work->msgData, work->msgFormat); // "Choose a Pokémon."
            sub_0201A738(2);
            return TRUE;
        }
        break;
    }
    return FALSE;
}

void WirelessTradeSelectMon_GetMonInfo(Pokemon *mon, WirelessTradeMonInfo *info) {
    BOOL decrypted;
    u32 heldItem;

    decrypted = AcquireMonLock(mon);
    info->ball = GetMonData(mon, MON_DATA_POKEBALL, NULL);
    info->species = GetMonData(mon, MON_DATA_SPECIES, NULL);
    info->form = GetMonData(mon, MON_DATA_FORM, NULL);
    info->isEgg = GetMonData(mon, MON_DATA_IS_EGG, NULL);
    info->gender = GetMonData(mon, MON_DATA_GENDER, NULL);
    info->capsule = GetMonData(mon, MON_DATA_BALL_CAPSULE_ID, NULL);
    heldItem = GetMonData(mon, MON_DATA_HELD_ITEM, NULL);
    ReleaseMonLock(mon, decrypted);

    if (info->species != SPECIES_NONE) {
        info->flip = GetMonBaseStat_HandleAlternateForm(info->species, info->form, BASE_FLIP);
    }
    if (heldItem != ITEM_NONE) {
        info->heldItem = ItemIdIsMail((u16)heldItem) + 1;
    }
}

void WirelessTradeSelectMon_SetBallSprite(WirelessTradeSelectMon *work, int index, int ball) {
    Sprite **sprites = work->detailSprites;

    Sprite_SetAnimCtrlSeq(sprites[index], ball + 7);
    if (index == 2) {
        GfGfxLoader_GXLoadPal(NARC_a_1_6_2, sWirelessTradeSelectMonBallPalettes[ball - 1], GF_PAL_LOCATION_SUB_OBJ, GF_PAL_SLOT_11_OFFSET, 0x20, HEAP_ID_26);
        Sprite_SetPaletteOverride(sprites[index], 11);
    } else if (index == 3) {
        GfGfxLoader_GXLoadPal(NARC_a_1_6_2, sWirelessTradeSelectMonBallPalettes[ball - 1], GF_PAL_LOCATION_SUB_OBJ, GF_PAL_SLOT_12_OFFSET, 0x20, HEAP_ID_26);
        Sprite_SetPaletteOverride(sprites[index], 12);
    }
}

void WirelessTradeSelectMon_ShowPartnerMon(WirelessTradeSelectMon *work, int slot) {
    int partySlot;
    int side;
    Pokemon *mon;

    partySlot = slot % 6;
    mon = Party_GetMonByIndex(work->partnerParty, partySlot);
    side = slot / 6;
    work->pendingMonPic = WirelessTradeSelectMon_LoadMonPic(1, mon, work->monPics[side], &work->monPicTemplates[side]);
    WirelessTradeSelectMon_ShowMonDetails(work->windows, 1, work->partnerParty, partySlot, work);
    WirelessTradeSelectMon_SetBallSprite(work, 3, work->mons[slot].ball);
}

void WirelessTradeSelectMon_ShowSelectedMon(WirelessTradeSelectMon *work) {
    int cursor = work->cursor;

    if (cursor == WIRELESS_TRADE_SLOT_CANCEL) {
        return;
    }
    if (cursor < 6) {
        Pokemon *mon = Party_GetMonByIndex(work->party, cursor);
        int side = work->cursor / 6;

        work->pendingMonPic = WirelessTradeSelectMon_LoadMonPic(0, mon, work->monPics[side], &work->monPicTemplates[side]);
        WirelessTradeSelectMon_ShowMonDetails(work->windows, 0, work->party, work->cursor, work);
        WirelessTradeSelectMon_HideMonDetails(work->windows, 1, work);
        Sprite_SetDrawFlag(work->detailSprites[3], FALSE);
    } else {
        WirelessTradeSelectMon_ShowPartnerMon(work, cursor);
        WirelessTradeSelectMon_HideMonDetails(work->windows, 0, work);
        Sprite_SetDrawFlag(work->detailSprites[2], FALSE);
    }
}

int WirelessTradeSelectMon_RunSelect(WirelessTradeSelectMon *work) {
    WirelessTradeSelectMonFunc callback = work->callback;

    if (callback != NULL) {
        work->selectResult = callback(work);
    }

    switch (work->selectResult) {
    case 0:
    case 1:
        break;
    case 2:
        return 2;
    case 3:
        return 3;
    default:
        break;
    }

    if (WirelessTradeSelectMon_MoveCursor(&work->dpadDir, &work->cursor, work->cursorSprites[0], work->mons, 0)) {
        WirelessTradeSelectMon_ShowSelectedMon(work);
    }
    WirelessTradeSelectMon_MoveCursor(&work->partnerDpadDir, &work->partnerCursor, work->cursorSprites[1], work->mons, 1);
    WirelessTradeSelectMon_UpdateCursorBlink(&work->cursorBlinkAngle);
    WirelessTradeSelectMon_SendCommandIfChanged(work, WIRELESS_TRADE_CMD_CURSOR, work->cursor);
    return 1;
}

void WirelessTradeSelectMon_FreeSprites(WirelessTradeSelectMon *work) {
    int i;
    u8 *shifted;
    u32 offset;

    SpriteTransfer_DeleteCharTransferTask(work->resObjs[0][GF_GFX_RES_TYPE_CHAR]);
    SpriteTransfer_DeleteCharTransferTask(work->resObjs[1][GF_GFX_RES_TYPE_CHAR]);
    SpriteTransfer_DeletePlttTransferTask(work->resObjs[0][GF_GFX_RES_TYPE_PLTT]);
    SpriteTransfer_DeletePlttTransferTask(work->resObjs[1][GF_GFX_RES_TYPE_PLTT]);
    // resMan[i], addressed through the work pointer walked by i slots.
    offset = 0xB3 * 4;
    i = 0;
    shifted = (u8 *)work;
    while (i < 4) {
        Destroy2DGfxResObjMan(*(GF_2DGfxResMan **)(shifted + offset));
        i++;
        shifted += sizeof(GF_2DGfxResMan *);
    }
    SpriteList_Delete(work->spriteList);
    OamManager_Free();
    ObjCharTransfer_Destroy();
    ObjPlttTransfer_Destroy();
}

void WirelessTradeSelectMon_FreeIconCharData(WirelessTradeSelectMon *work) {
    int i;

    i = 0;
    if (Party_GetCount(work->party) > 0) {
        do {
            Heap_Free(work->iconCharBuffers[i]);
            i++;
        } while (i < Party_GetCount(work->party));
    }
    i = 0;
    if (Party_GetCount(work->partnerParty) > 0) {
        do {
            Heap_Free(work->iconCharBuffers[6 + i]);
            i++;
        } while (i < Party_GetCount(work->partnerParty));
    }
}

BOOL WirelessTradeSelectMon_Exit(OverlayManager *man, int *state) {
    WirelessTradeSelectMon *work = OverlayManager_GetData(man);
    WirelessTradeSelectMonArgs *args = OverlayManager_GetArgs(man);

    args->unk24 = work->tradeAccepted;
    WirelessTradeSelectMon_FreeIconCharData(work);
    String_Delete(work->itemLabel);
    Heap_Free(work->partnerParty);
    WirelessTradeSelectMon_FreeSprites(work);
    WirelessTradeSelectMon_RemoveWindows(work->windows);
    WirelessTradeSelectMon_FreeBgs(work->bgConfig);
    Heap_FreeExplicit(HEAP_ID_26, work->bgConfig);
    DestroyMsgData(work->msgData);
    MessageFormat_Delete(work->unk_18C);
    MessageFormat_Delete(work->nicknameFormat);
    MessageFormat_Delete(work->msgFormat);
    String_Delete(work->playerName);
    OverlayManager_FreeData(man);
    Main_SetVBlankIntrCB(NULL, NULL);
    FontID_Release(4);
    Heap_Destroy(HEAP_ID_26);
    return TRUE;
}

#pragma force_active on

void WirelessTradeSelectMon_VBlankCB(WirelessTradeSelectMon *work) {
    vu32 *dtcm = (vu32 *)0x027E0000;

    DoScheduledBgGpuUpdates(work->bgConfig);
    if (work->pendingMonPic) {
        int side = work->pendingMonPic - 1;
        GXS_LoadOBJ(work->monPics[side], side * WIRELESS_TRADE_MON_PIC_SIZE, WIRELESS_TRADE_MON_PIC_SIZE);
        GfGfxLoader_GXLoadPal((NarcId)work->monPicTemplates[side].narcID, work->monPicTemplates[side].palDataID, GF_PAL_LOCATION_SUB_OBJ, (enum GFPalSlotOffset)((side + 2) * 32), 32, HEAP_ID_26);
        work->pendingMonPic = 0;
    }
    GF_RunVramTransferTasks();
    OamManager_ApplyAndResetBuffers();
    dtcm[(HW_INTR_CHECK_BUF - HW_DTCM) / 4] |= OS_IE_V_BLANK; // OS_SetIrqCheckFlag, through the DTCM base address
}

#pragma force_active reset

void WirelessTradeSelectMon_SetGfxBanks(void) {
    GraphicsBanks banks = sWirelessTradeSelectMonGraphicsBanks;
    GfGfx_SetBanks(&banks);
}

void WirelessTradeSelectMon_InitBgs(BgConfig *bgConfig) {
    GraphicsModes modes;
    BgTemplate main0;
    BgTemplate main1;
    BgTemplate main2;
    BgTemplate main3;
    BgTemplate sub0;
    BgTemplate sub1;
    BgTemplate sub2;
    int i;

    modes = sWirelessTradeSelectMonGraphicsModes;
    SetBothScreensModesAndDisable(&modes);

    main0 = sWirelessTradeSelectMonBgTemplate_Main0;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_0, &main0, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_0);
    main1 = sWirelessTradeSelectMonBgTemplate_Main1;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_1, &main1, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_1);
    main2 = sWirelessTradeSelectMonBgTemplate_Main2;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_2, &main2, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_2);
    main3 = sWirelessTradeSelectMonBgTemplate_Main3;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_3, &main3, GF_BG_TYPE_TEXT);
    sub0 = sWirelessTradeSelectMonBgTemplate_Sub0;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_0, &sub0, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_0);
    sub1 = sWirelessTradeSelectMonBgTemplate_Sub1;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_1, &sub1, GF_BG_TYPE_TEXT);
    sub2 = sWirelessTradeSelectMonBgTemplate_Sub2;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_2, &sub2, GF_BG_TYPE_TEXT);

    BG_ClearCharDataRange(GF_BG_LYR_MAIN_0, 32, 0, HEAP_ID_26);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_1, 32, 0, HEAP_ID_26);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_0, 32, 0, HEAP_ID_26);
    for (i = 0; i < 4; i++) {
        BgSetPosTextAndCommit(bgConfig, (u8)i, BG_POS_OP_SET_X, 0);
        BgSetPosTextAndCommit(bgConfig, (u8)i, BG_POS_OP_SET_Y, 0);
        BgSetPosTextAndCommit(bgConfig, (u8)(i + 4), BG_POS_OP_SET_X, 0);
        BgSetPosTextAndCommit(bgConfig, (u8)(i + 4), BG_POS_OP_SET_Y, 0);
    }
    ToggleBgLayer(GF_BG_LYR_MAIN_0, GF_PLANE_TOGGLE_OFF);
    ToggleBgLayer(GF_BG_LYR_MAIN_1, GF_PLANE_TOGGLE_OFF);
    ToggleBgLayer(GF_BG_LYR_MAIN_2, GF_PLANE_TOGGLE_OFF);
    ToggleBgLayer(GF_BG_LYR_MAIN_3, GF_PLANE_TOGGLE_OFF);
    ToggleBgLayer(GF_BG_LYR_SUB_0, GF_PLANE_TOGGLE_OFF);
    ToggleBgLayer(GF_BG_LYR_SUB_1, GF_PLANE_TOGGLE_OFF);
    ToggleBgLayer(GF_BG_LYR_SUB_2, GF_PLANE_TOGGLE_OFF);

    GX_SetVisibleWnd(GX_WNDMASK_NONE);
    G2_SetWnd0InsidePlane(GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG2 | GX_WND_PLANEMASK_BG3 | GX_WND_PLANEMASK_OBJ, FALSE);
    G2_SetWndOutsidePlane(GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG2 | GX_WND_PLANEMASK_BG3 | GX_WND_PLANEMASK_OBJ, TRUE);
    G2_SetWnd0Position(240, 0, 0, 16);
    GX_SetDispSelect(GX_DISP_SELECT_SUB_MAIN);
}

void WirelessTradeSelectMon_InitWork(WirelessTradeSelectMon *work, OverlayManager *man) {
    WirelessTradeSelectMonArgs *args = OverlayManager_GetArgs(man);
    u32 size;

    work->args = args;
    work->unk_674 = 4;
    work->summaryActive = FALSE;
    work->yesNoState = 0;
    work->unk_178 = 6;
    work->unk_17C = 6;
    work->cursor = 0;
    work->partnerCursor = 6;
    work->selectResult = 0;
    work->state = 0;
    work->exchangeState = 0;
    work->callback = WirelessTradeSelectMon_SelectMon;
    work->answers[0] = 0;
    work->answers[1] = 0;
    work->tradeAccepted = FALSE;
    work->pendingMonPic = 0;
    work->lastSentValue = -1;
    work->lastSentCommand = -1;
    work->startDelay = 0;
    work->timeoutActive = FALSE;
    work->timeoutCounter = 0;
    work->party = args->party;
    work->palPad = args->palPad;
    work->saveData = args->saveData;

    size = SaveArray_Party_sizeof();
    work->partnerParty = Heap_Alloc(HEAP_ID_26, size);
    Party_InitWithMaxSize(work->partnerParty, PARTY_SIZE);
    size = SaveArray_Party_sizeof();
    memset(work->partnerParty, 0xFF, size);
    work->playerName = PlayerProfile_GetPlayerName_NewString(args->profile, HEAP_ID_26);
    work->itemLabel = NewString_ReadMsgData(work->msgData, 42); // "ITEM"
    sub_02039F68(args->wifiHistory);
}

void WirelessTradeSelectMon_FreeBgs(BgConfig *bgConfig) {
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_2);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_1);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_0);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_3);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_0);
}

#pragma force_active on

void WirelessTradeSelectMon_LoadScreenData(NARC *narc, int memberNo, BgConfig *bgConfig, int layer, int unused, u32 size, BOOL isCompressed, enum HeapID heapID) {
    NNSG2dScreenData *screen;
    void *raw = GfGfxLoader_LoadFromOpenNarc(narc, memberNo, isCompressed, heapID, TRUE);

    if (raw) {
        if (NNS_G2dGetUnpackedScreenData(raw, &screen)) {
            if (size == 0) {
                size = screen->szByte;
            }
            if (GetBgTilemapBuffer(bgConfig, (u8)layer)) {
                BG_LoadScreenTilemapData(bgConfig, (u8)layer, screen->rawData, size);
            }
        }
        Heap_Free(raw);
    }
}

#pragma force_active reset

void WirelessTradeSelectMon_CommitNameBg(BgConfig *bgConfig) {
    BgCopyOrUncompressTilemapBufferRangeToVram(bgConfig, GF_BG_LYR_MAIN_2, GetBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_2), 0x600, 0);
}

void WirelessTradeSelectMon_LoadBgGraphics(WirelessTradeSelectMon *work, NARC *narc) {
    BgConfig *bgConfig = work->bgConfig;

    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 0, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x120, HEAP_ID_26);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 0, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x120, HEAP_ID_26);
    BG_SetMaskColor(GF_BG_LYR_MAIN_0, RGB_BLACK);
    BG_SetMaskColor(GF_BG_LYR_SUB_0, RGB_BLACK);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_13_OFFSET, HEAP_ID_26);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 1, bgConfig, GF_BG_LYR_MAIN_2, 0, 0x2400, TRUE, HEAP_ID_26);
    WirelessTradeSelectMon_LoadScreenData(narc, 3, bgConfig, GF_BG_LYR_MAIN_2, 0, 0x600, TRUE, HEAP_ID_26);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 4, bgConfig, GF_BG_LYR_MAIN_3, 0, 0x600, TRUE, HEAP_ID_26);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_2_OFFSET, HEAP_ID_26);
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_3_OFFSET, HEAP_ID_26);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 2, bgConfig, GF_BG_LYR_SUB_1, 0, 0x800, TRUE, HEAP_ID_26);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 5, bgConfig, GF_BG_LYR_SUB_1, 0, 0x600, TRUE, HEAP_ID_26);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 6, bgConfig, GF_BG_LYR_SUB_2, 0, 0x600, TRUE, HEAP_ID_26);
    LoadFontPal1(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_2_OFFSET, HEAP_ID_26);
    LoadFontPal0(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_3_OFFSET, HEAP_ID_26);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_0);
}

void WirelessTradeSelectMon_InitObjTransfer(void) {
    ObjCharTransferTemplate transferTemplate = sWirelessTradeSelectMonObjCharTransferTemplate;

    ObjCharTransfer_Init(&transferTemplate);
    ObjPlttTransfer_Init(20, HEAP_ID_26);
    ObjCharTransfer_ClearBuffers();
    ObjPlttTransfer_Reset();
}

void WirelessTradeSelectMon_InitSpriteResources(WirelessTradeSelectMon *work, NARC *narc) {
    int i;

    NNS_G2dInitOamManagerModule();
    OamManager_Create(0, 100, 0, 32, 0, 100, 0, 32, HEAP_ID_26);
    work->spriteList = G2dRenderer_Init(45, &work->renderer, HEAP_ID_26);
    for (i = 0; i < 4; i++) {
        work->resMan[i] = Create2DGfxResObjMan(2, (GfGfxResType)i, HEAP_ID_26);
    }
    work->resObjs[0][GF_GFX_RES_TYPE_CHAR] = AddCharResObjFromOpenNarc(work->resMan[GF_GFX_RES_TYPE_CHAR], narc, 7, TRUE, 0, NNS_G2D_VRAM_TYPE_2DMAIN, HEAP_ID_26);
    work->resObjs[0][GF_GFX_RES_TYPE_PLTT] = AddPlttResObjFromOpenNarc(work->resMan[GF_GFX_RES_TYPE_PLTT], narc, 0, FALSE, 0, NNS_G2D_VRAM_TYPE_2DMAIN, 9, HEAP_ID_26);
    work->resObjs[0][GF_GFX_RES_TYPE_CELL] = AddCellOrAnimResObjFromOpenNarc(work->resMan[GF_GFX_RES_TYPE_CELL], narc, 8, TRUE, 0, GF_GFX_RES_TYPE_CELL, HEAP_ID_26);
    work->resObjs[0][GF_GFX_RES_TYPE_ANIM] = AddCellOrAnimResObjFromOpenNarc(work->resMan[GF_GFX_RES_TYPE_ANIM], narc, 9, TRUE, 0, GF_GFX_RES_TYPE_ANIM, HEAP_ID_26);
    work->resObjs[1][GF_GFX_RES_TYPE_CHAR] = AddCharResObjFromOpenNarc(work->resMan[GF_GFX_RES_TYPE_CHAR], narc, 10, TRUE, 1, NNS_G2D_VRAM_TYPE_2DSUB, HEAP_ID_26);
    work->resObjs[1][GF_GFX_RES_TYPE_PLTT] = AddPlttResObjFromOpenNarc(work->resMan[GF_GFX_RES_TYPE_PLTT], narc, 0, FALSE, 1, NNS_G2D_VRAM_TYPE_2DSUB, 10, HEAP_ID_26);
    work->resObjs[1][GF_GFX_RES_TYPE_CELL] = AddCellOrAnimResObjFromOpenNarc(work->resMan[GF_GFX_RES_TYPE_CELL], narc, 11, TRUE, 1, GF_GFX_RES_TYPE_CELL, HEAP_ID_26);
    work->resObjs[1][GF_GFX_RES_TYPE_ANIM] = AddCellOrAnimResObjFromOpenNarc(work->resMan[GF_GFX_RES_TYPE_ANIM], narc, 12, TRUE, 1, GF_GFX_RES_TYPE_ANIM, HEAP_ID_26);
    SpriteTransfer_CreateCharTransferTask(work->resObjs[0][GF_GFX_RES_TYPE_CHAR]);
    SpriteTransfer_CreateCharTransferTask(work->resObjs[1][GF_GFX_RES_TYPE_CHAR]);
    SpriteTransfer_CreateExtPlttTransferTask(work->resObjs[0][GF_GFX_RES_TYPE_PLTT]);
    SpriteTransfer_CreateExtPlttTransferTask(work->resObjs[1][GF_GFX_RES_TYPE_PLTT]);
    GfGfxLoader_GXLoadPal(NARC_poketool_icongra_poke_icon, sub_02074490(), GF_PAL_LOCATION_MAIN_OBJ, GF_PAL_SLOT_10_OFFSET, 0x80, HEAP_ID_26);
}

u32 WirelessTradeSelectMon_LoadMonPic(u32 side, Pokemon *mon, void *dest, PokepicTemplate *pokepic) {
    u32 personality;
    u32 species;

    GetPokemonSpriteCharAndPlttNarcIds(pokepic, mon, 2);
    personality = GetMonData(mon, MON_DATA_PERSONALITY, NULL);
    species = GetMonData(mon, MON_DATA_SPECIES, NULL);
    if (GetMonData(mon, MON_DATA_IS_EGG, NULL) == TRUE && species == SPECIES_SPINDA) {
        species = SPECIES_EGG;
    }
    sub_02014494((NarcId)pokepic->narcID, pokepic->charDataID, HEAP_ID_26, 0, 0, 10, 10, dest, personality, FALSE, 2, species);
    DC_FlushRange(dest, WIRELESS_TRADE_MON_PIC_SIZE);
    return side + 1;
}

void WirelessTradeSelectMon_PrintMonNickname(Window *window, Party *party, u32 slot, u32 unused, u32 x) {
    String *string = String_New(20, HEAP_ID_26);
    Pokemon *mon = Party_GetMonByIndex(party, slot);

    GetMonData(mon, MON_DATA_NICKNAME_STRING, string);
    FillWindowPixelBuffer(window, 0);
    WirelessTradeSelectMon_PrintString(window, string, unused, 0, x, 1);
    String_Delete(string);
}

// Nidoran without a nickname shows no gender symbol: the symbol is part of its species name.
int WirelessTradeSelectMon_GetGenderSymbol(WirelessTradeMonInfo *info, Party *party, u32 slot, int gender) {
    if ((info->species == SPECIES_NIDORAN_F || info->species == SPECIES_NIDORAN_M) && !GetMonData(Party_GetMonByIndex(party, slot), MON_DATA_HAS_NICKNAME, NULL)) {
        return MON_GENDERLESS;
    }
    return gender;
}

#pragma force_active on

void WirelessTradeSelectMon_ShowMonDetails(Window *windows, int side, Party *party, int slot, WirelessTradeSelectMon *work) {
    // Kept in one struct: matches the original stack layout.
    struct {
        Party *party;
        int slot;
        int yOffset;
        int sideOffset;
        u8 *isEgg;
        Sprite **sprites0;
        int offset0;
        Sprite **sprites1;
        int offset1;
        int windowOffset;
        u8 *shifted;
        int row;
    } f;

    String *string;
    int genderSymbol;
    u16 value;
    Sprite **sprites;

    f.party = party;
    f.slot = slot;
    // Pokemon starts with its BoxPokemon.
    f.yOffset = sub_0207083C((BoxPokemon *)Party_GetMonByIndex(party, slot), 2);
    sprites = work->detailSprites;
    f.sideOffset = side * sizeof(Sprite *);
    Sprite_SetDrawFlag(*(Sprite **)((u8 *)sprites + f.sideOffset), TRUE);
    // ov65_0221FEA8 is sWirelessTradeSelectMonDetailSpritePos[0].y.
    WirelessTradeSelectMon_SetSpritePos(*(Sprite **)((u8 *)sprites + f.sideOffset), sWirelessTradeSelectMonDetailSpritePos[side].x, f.yOffset + ov65_0221FEA8[side].x + 192);
    if (side == 0) {
        // The original call passes the flip value untruncated (sprite.h declares the parameter u8).
        ((void (*)(Sprite *, int))Sprite_SetFlipMode)(*(Sprite **)((u8 *)sprites + f.sideOffset), work->mons[f.slot].flip);
    }
    WirelessTradeSelectMon_SetBallSprite(work, side + 2, work->mons[f.slot].ball);
    Sprite_SetDrawFlag(work->detailSprites[side + 2], TRUE);
    WirelessTradeSelectMon_PrintMonNickname(&windows[side + 26], f.party, f.slot, 9, 6);

    // The work pointer shifted by the mon's row: mons[0] through it is mons[f.slot + side * 6].
    f.row = (f.slot + side * 6) * sizeof(WirelessTradeMonInfo);
    f.shifted = (u8 *)work + f.row;
    genderSymbol = WirelessTradeSelectMon_GetGenderSymbol((WirelessTradeMonInfo *)((u8 *)work->mons + f.row), f.party, f.slot, ((WirelessTradeSelectMon *)f.shifted)->mons[0].gender);
    f.isEgg = &work->mons[0].isEgg;
    if (f.isEgg[f.row]) {
        genderSymbol = MON_GENDERLESS;
    }
    switch (genderSymbol) {
    case MON_GENDERLESS:
        Sprite_SetDrawFlag(work->detailSprites[side + 4], FALSE);
        break;
    case MON_MALE:
        f.sprites0 = work->detailSprites;
        f.offset0 = (side + 4) * sizeof(Sprite *);
        Sprite_SetDrawFlag(*(Sprite **)((u8 *)f.sprites0 + f.offset0), TRUE);
        Sprite_SetAnimCtrlSeq(*(Sprite **)((u8 *)f.sprites0 + f.offset0), 7);
        break;
    case MON_FEMALE:
        f.sprites1 = work->detailSprites;
        f.offset1 = (side + 4) * sizeof(Sprite *);
        Sprite_SetDrawFlag(*(Sprite **)((u8 *)f.sprites1 + f.offset1), TRUE);
        Sprite_SetAnimCtrlSeq(*(Sprite **)((u8 *)f.sprites1 + f.offset1), 6);
        break;
    }

    if (f.isEgg[f.row] == FALSE) {
        string = String_New(10, HEAP_ID_26);
        f.windowOffset = (side + 28) * sizeof(Window);
        FillWindowPixelBuffer((Window *)((u8 *)windows + f.windowOffset), 0);
        ReadMsgDataIntoString(work->msgData, 41, string); // "Lv."
        WirelessTradeSelectMon_PrintString((Window *)((u8 *)windows + f.windowOffset), string, 9, 255, 6, 0);
        value = (u16)GetMonData(Party_GetMonByIndex(f.party, f.slot), MON_DATA_LEVEL, NULL);
        String16_FormatInteger(string, value, 3, PRINTING_MODE_LEFT_ALIGN, TRUE);
        WirelessTradeSelectMon_PrintString((Window *)((u8 *)windows + f.windowOffset), string, 9, 0, 30, 0);
        String_Delete(string);
    } else {
        ClearWindowTilemapAndCopyToVram(&windows[side + 28]);
    }

    WirelessTradeSelectMon_PrintString(&windows[side + 30], work->itemLabel, 7, 0, 3, 0);
    value = (u16)GetMonData(Party_GetMonByIndex(f.party, f.slot), MON_DATA_HELD_ITEM, NULL);
    side += 32;
    FillWindowPixelBuffer(&windows[side], 0);
    string = String_New(20, HEAP_ID_26);
    GetItemNameIntoString(string, value, HEAP_ID_26);
    WirelessTradeSelectMon_PrintString(&windows[side], string, 9, 0, 3, 0);
    String_Delete(string);
}

#pragma force_active reset

void WirelessTradeSelectMon_HideMonDetails(Window *windows, int side, WirelessTradeSelectMon *work) {
    Sprite_SetDrawFlag(work->detailSprites[side], FALSE);
    Sprite_SetDrawFlag(work->detailSprites[side + 2], FALSE);
    Sprite_SetDrawFlag(work->detailSprites[side + 4], FALSE);
    ClearWindowTilemapAndCopyToVram(&windows[side + 26]);
    ClearWindowTilemapAndCopyToVram(&windows[side + 28]);
    ClearWindowTilemapAndCopyToVram(&windows[side + 30]);
    ClearWindowTilemapAndCopyToVram(&windows[side + 32]);
}

#pragma force_active on

void WirelessTradeSelectMon_CreateSprites(WirelessTradeSelectMon *work) {
    SpriteTemplate spriteTemplate;
    int i;

    CreateSpriteResourcesHeader(&work->resHeaders[0], 0, 0, 0, 0, -1, -1, FALSE, 2, work->resMan[GF_GFX_RES_TYPE_CHAR], work->resMan[GF_GFX_RES_TYPE_PLTT], work->resMan[GF_GFX_RES_TYPE_CELL], work->resMan[GF_GFX_RES_TYPE_ANIM], NULL, NULL);
    CreateSpriteResourcesHeader(&work->resHeaders[1], 1, 1, 1, 1, -1, -1, FALSE, 0, work->resMan[GF_GFX_RES_TYPE_CHAR], work->resMan[GF_GFX_RES_TYPE_PLTT], work->resMan[GF_GFX_RES_TYPE_CELL], work->resMan[GF_GFX_RES_TYPE_ANIM], NULL, NULL);
    spriteTemplate.spriteList = work->spriteList;
    spriteTemplate.header = &work->resHeaders[0];
    spriteTemplate.position.x = 32 * FX32_ONE;
    spriteTemplate.position.y = 96 * FX32_ONE;
    spriteTemplate.position.z = 0;
    spriteTemplate.scale.x = FX32_ONE;
    spriteTemplate.scale.y = FX32_ONE;
    spriteTemplate.scale.z = FX32_ONE;
    spriteTemplate.rotation = 0;
    spriteTemplate.drawPriority = 0;
    spriteTemplate.whichScreen = NNS_G2D_VRAM_TYPE_2DMAIN;
    spriteTemplate.heapID = HEAP_ID_26;

    for (i = 0; i < 2; i++) {
        spriteTemplate.position.x = sWirelessTradeSelectMonSlotPos[i].x * FX32_ONE;
        spriteTemplate.position.y = sWirelessTradeSelectMonSlotPos[i].y * FX32_ONE;
        work->cursorSprites[i] = Sprite_CreateAffine(&spriteTemplate);
        Sprite_SetAnimActiveFlag(work->cursorSprites[i], TRUE);
        WirelessTradeSelectMon_SetCursorSprite((&work->cursor)[i], work->cursorSprites[i], i); // cursor, partnerCursor
        Sprite_SetDrawPriority(work->cursorSprites[i], 100);
        Sprite_SetDrawFlag(work->cursorSprites[i], FALSE);
        Sprite_SetOamMode(work->cursorSprites[i], GX_OAM_MODE_XLU);
    }
    for (i = 0; i < WIRELESS_TRADE_NUM_SLOTS; i++) {
        spriteTemplate.position.x = (sWirelessTradeSelectMonSlotPos[i].x + 16) * FX32_ONE;
        spriteTemplate.position.y = (sWirelessTradeSelectMonSlotPos[i].y - 6) * FX32_ONE;
        work->iconSprites[i] = Sprite_CreateAffine(&spriteTemplate);
        Sprite_SetAnimActiveFlag(work->iconSprites[i], TRUE);
        Sprite_SetAnimCtrlSeq(work->iconSprites[i], i + 5);
        Sprite_SetDrawPriority(work->iconSprites[i], 5);
        Sprite_SetDrawFlag(work->iconSprites[i], FALSE);
        Sprite_SetOamMode(work->iconSprites[i], GX_OAM_MODE_XLU);
    }
    for (i = 0; i < WIRELESS_TRADE_NUM_SLOTS; i++) {
        spriteTemplate.position.x = (sWirelessTradeSelectMonSlotPos[i].x + 36) * FX32_ONE;
        spriteTemplate.position.y = (sWirelessTradeSelectMonSlotPos[i].y + 16) * FX32_ONE;
        spriteTemplate.drawPriority = 0;
        work->itemSprites[i] = Sprite_CreateAffine(&spriteTemplate);
        Sprite_SetDrawPriority(work->itemSprites[i], 3);
        Sprite_SetDrawFlag(work->itemSprites[i], FALSE);
        Sprite_SetOamMode(work->itemSprites[i], GX_OAM_MODE_XLU);
    }
    for (i = 0; i < WIRELESS_TRADE_NUM_SLOTS; i++) {
        spriteTemplate.position.x = (sWirelessTradeSelectMonSlotPos[i].x + 45) * FX32_ONE;
        spriteTemplate.position.y = (sWirelessTradeSelectMonSlotPos[i].y + 16) * FX32_ONE;
        spriteTemplate.drawPriority = 0;
        work->capsuleSprites[i] = Sprite_CreateAffine(&spriteTemplate);
        Sprite_SetDrawPriority(work->capsuleSprites[i], 3);
        Sprite_SetDrawFlag(work->capsuleSprites[i], FALSE);
        Sprite_SetOamMode(work->capsuleSprites[i], GX_OAM_MODE_XLU);
    }
    spriteTemplate.position.x = 96 * FX32_ONE;
    spriteTemplate.position.y = 66 * FX32_ONE;
    work->tradeSprite = Sprite_CreateAffine(&spriteTemplate);
    Sprite_SetAnimActiveFlag(work->tradeSprite, TRUE);
    Sprite_SetAnimCtrlSeq(work->tradeSprite, 20);
    Sprite_SetDrawFlag(work->tradeSprite, FALSE);
    for (i = 0; i < 6; i++) {
        spriteTemplate.header = &work->resHeaders[1];
        spriteTemplate.position.x = sWirelessTradeSelectMonDetailSpritePos[i].x * FX32_ONE;
        spriteTemplate.position.y = sWirelessTradeSelectMonDetailSpritePos[i].y * FX32_ONE + 192 * FX32_ONE;
        spriteTemplate.whichScreen = NNS_G2D_VRAM_TYPE_2DSUB;
        work->detailSprites[i] = Sprite_CreateAffine(&spriteTemplate);
        Sprite_SetDrawFlag(work->detailSprites[i], FALSE);
        Sprite_SetAnimActiveFlag(work->detailSprites[i], FALSE);
        Sprite_SetAnimCtrlSeq(work->detailSprites[i], sWirelessTradeSelectMonDetailSpritePos[i].animSeq);
    }
}

#pragma force_active reset

void WirelessTradeSelectMon_ReadDpad(int *dir) {
    int count;
    int newDir;
    u32 keys;

    count = 0;
    newDir = 0;
    keys = gSystem.newAndRepeatedKeys;
    if (keys & PAD_KEY_UP) {
        newDir = 1;
        count++;
    }
    if (keys & PAD_KEY_DOWN) {
        newDir = 2;
        count++;
    }
    if (keys & PAD_KEY_LEFT) {
        newDir = 3;
        count++;
    }
    if (keys & PAD_KEY_RIGHT) {
        newDir = 4;
        count++;
    }
    if (count != 0) {
        *dir = newDir;
    }
}

void WirelessTradeSelectMon_PlayCryOnTouch(WirelessTradeSelectMon *work) {
    int hit;
    int form;
    u16 species;

    if (work->cursor >= 6) {
        return;
    }
    hit = TouchscreenHitbox_FindRectAtTouchNew(sWirelessTradeSelectMonSlotHitboxes);
    if (hit == -1) {
        return;
    }
    if (hit >= 6) {
        return;
    }
    if (work->mons[hit].species == SPECIES_NONE) {
        return;
    }
    if (work->mons[work->cursor].isEgg != FALSE) {
        return;
    }
    species = work->mons[work->cursor].species;
    form = work->mons[work->cursor].form;
    if (species == SPECIES_CHATOT) {
        sub_02006E4C(Save_Chatot_Get(work->saveData), 0, 100, 0);
    } else {
        PlayCry(species, (u8)form);
    }
    Sprite_SetAnimActiveFlag(work->detailSprites[0], TRUE);
    Sprite_SetAnimCtrlSeq(work->detailSprites[0], 4);
}

void WirelessTradeSelectMon_UpdateCursorBlink(u16 *angle) {
    u16 color;

    *angle += 20;
    if (*angle > 360) {
        *angle = 0;
    }
    color = (u16)(((GF_SinDeg(*angle) * 10 / FX32_ONE) + 15) * 32 | 29);
    GX_LoadOBJPltt(&color, 0x3A, sizeof(color));
}

#pragma force_active on

u8 WirelessTradeSelectMon_GetNextCursorPos(int cursor, int dir, WirelessTradeMonInfo *mons) {
    int i;
    u8 ret = 0;

    for (i = 0; i < 6; i++) {
        if (mons[sWirelessTradeSelectMonCursorMoves[cursor][dir][i]].species) {
            ret = sWirelessTradeSelectMonCursorMoves[cursor][dir][i];
            break;
        }
    }
    return ret;
}

#pragma force_active reset

void WirelessTradeSelectMon_SetCursorSprite(int cursor, Sprite *sprite, int side) {
    VecFx32 pos;

    GF_ASSERT(cursor < WIRELESS_TRADE_NUM_SLOTS + 1);
    // ov65_0221FF50 is sWirelessTradeSelectMonSlotPos[0].y.
    pos.x = sWirelessTradeSelectMonSlotPos[cursor].x * FX32_ONE;
    pos.y = ov65_0221FF50[cursor].x * FX32_ONE;
    if (cursor == WIRELESS_TRADE_SLOT_CANCEL) {
        Sprite_SetMatrix(sprite, &pos);
        Sprite_SetAnimCtrlSeq(sprite, ov65_0221FD70[side][0]);
        return;
    }
    Sprite_SetMatrix(sprite, &pos);
    if (cursor < 6) {
        Sprite_SetAnimCtrlSeq(sprite, sWirelessTradeSelectMonCursorAnims[side][0]);
    } else {
        Sprite_SetAnimCtrlSeq(sprite, ov65_0221FD6C[side][0]);
    }
}

int WirelessTradeSelectMon_MoveCursor(int *dpadDir, int *cursor, Sprite *sprite, WirelessTradeMonInfo *mons, int side) {
    int ret;
    int next;
    int dir;
    int dirIndex;

    dir = *dpadDir;
    dirIndex = dir - 1;
    ret = FALSE;
    if (side == 0) {
        if (dir != 0) {
            next = ((int (*)(int, int, WirelessTradeMonInfo *))WirelessTradeSelectMon_GetNextCursorPos)(*cursor, dirIndex, mons);
            WirelessTradeSelectMon_SetCursorSprite(next, sprite, side);
            if (*cursor != next) {
                PlaySE(SEQ_SE_DP_SELECT);
                *cursor = next;
                ret = TRUE;
            }
        }
        *dpadDir = 0;
    } else {
        WirelessTradeSelectMon_SetCursorSprite(*cursor, sprite, side);
    }
    return ret;
}

int WirelessTradeSelectMon_SendCommand(u32 unused, u32 command, u8 value) {
    u8 data = value;
    return Comm_SendCommand(command, &data, sizeof(data));
}

#pragma force_active on

void WirelessTradeSelectMon_SendCommandIfChanged(WirelessTradeSelectMon *work, int command, int value) {
    if (value != work->lastSentValue || command != work->lastSentCommand) {
        ((int (*)(int, int, int))WirelessTradeSelectMon_SendCommand)(Comm_GetLocalNetId(), command, value);
        work->lastSentValue = value;
        work->lastSentCommand = command;
    }
}

#pragma force_active reset

void *WirelessTradeSelectMon_GetPartyChunk(void *party, u32 chunk) {
    return (u8 *)party + chunk * WIRELESS_TRADE_PARTY_CHUNK_SIZE;
}

void WirelessTradeSelectMon_SendPartyChunk(u32 netId, void *party, u32 chunk) {
    if (Comm_IsNetIdConnected((u16)netId)) {
        sub_02036FD8(WIRELESS_TRADE_CMD_PARTY_CHUNK, WirelessTradeSelectMon_GetPartyChunk(party, chunk), WIRELESS_TRADE_PARTY_CHUNK_SIZE);
    }
}

int WirelessTradeSelectMon_SendSpecialRibbons(SaveData *saveData) {
    return Comm_SendCommand(WIRELESS_TRADE_CMD_RIBBONS, Save_SpecialRibbons_Get(saveData), 14);
}

// Sends the player's own Pal Pad entry, made from the profile and the trainers on the Pal Pad.
s32 WirelessTradeSelectMon_SendPalPad(PlayerProfile *profile, SavePalPad *palPad, SavePalPad *packet) {
    s32 i;

    CopyU16StringArray(packet->name, PlayerProfile_GetNamePtr(profile));
    packet->otid = PlayerProfile_GetTrainerID(profile);
    // filler_14 holds the language, version and gender.
    packet->filler_14[0] = PlayerProfile_GetLanguage(profile);
    packet->filler_14[1] = PlayerProfile_GetVersion(profile);
    packet->filler_14[2] = PlayerProfile_GetTrainerGender(profile);
    for (i = 0; i < NUM_FRIENDS_PER_ENTRY; i++, palPad++) {
        packet->unk_18[i] = palPad->otid;
        packet->unk_58[i] = palPad->filler_14[1];
        packet->unk_68[i] = palPad->filler_14[0];
        packet->filler_78[i] = palPad->filler_14[2];
    }
    return sub_02036FD8(WIRELESS_TRADE_CMD_PALPAD, packet, sizeof(*packet));
}

int WirelessTradeSelectMon_SendChatot(SOUND_CHATOT *chatot) {
    return sub_02036FD8(WIRELESS_TRADE_CMD_CHATOT, chatot, 1000);
}

s32 WirelessTradeSelectMon_HandleKeys(WirelessTradeSelectMon *work) {
    if (gSystem.newKeys & PAD_BUTTON_B) {
        PlaySE(SEQ_SE_DP_SELECT);
        work->cursor = WIRELESS_TRADE_SLOT_CANCEL;
        WirelessTradeSelectMon_ShowSelectedMon(work);
        WirelessTradeSelectMon_SetCursorSprite(work->cursor, work->cursorSprites[0], 0);
    } else {
        WirelessTradeSelectMon_ReadDpad(&work->dpadDir);
        if ((gSystem.newKeys & PAD_BUTTON_A) && work->dpadDir == 0) {
            if (work->cursor == WIRELESS_TRADE_SLOT_CANCEL) {
                work->callback = WirelessTradeSelectMon_AskQuit;
            } else if (work->cursor < 6) {
                work->callback = WirelessTradeSelectMon_ShowOwnMonMenu;
            } else if (work->cursor >= 6 && work->cursor < WIRELESS_TRADE_NUM_SLOTS) {
                work->callback = WirelessTradeSelectMon_ShowPartnerMonMenu;
            }
        }
    }
    return 0;
}

s32 WirelessTradeSelectMon_HandleTouch(WirelessTradeSelectMon *work) {
    s32 hit = TouchscreenHitbox_FindRectAtTouchNew(sWirelessTradeSelectMonSlotHitboxes);

    if (hit != -1) {
        if (hit == WIRELESS_TRADE_SLOT_CANCEL) {
            PlaySE(SEQ_SE_DP_SELECT);
            work->callback = WirelessTradeSelectMon_AskQuit;
            work->cursor = WIRELESS_TRADE_SLOT_CANCEL;
        } else if (work->mons[hit].species != SPECIES_NONE) {
            work->cursor = hit;
            if (work->cursor < 6) {
                work->callback = WirelessTradeSelectMon_ShowOwnMonMenu;
            } else if (work->cursor >= 6 && work->cursor < WIRELESS_TRADE_NUM_SLOTS) {
                work->callback = WirelessTradeSelectMon_ShowPartnerMonMenu;
            }
        }
        WirelessTradeSelectMon_SetCursorSprite(work->cursor, work->cursorSprites[0], 0);
        WirelessTradeSelectMon_ShowSelectedMon(work);
        return 1;
    }
    return 0;
}

int WirelessTradeSelectMon_SelectMon(WirelessTradeSelectMon *work) {
    if (!WirelessTradeSelectMon_HandleTouch(work)) {
        WirelessTradeSelectMon_HandleKeys(work);
    }
    WirelessTradeSelectMon_PlayCryOnTouch(work);
    return 0;
}

void WirelessTradeSelectMon_OpenSummary(WirelessTradeSelectMon *work, u32 ofPartner) {
    if (ofPartner == FALSE) {
        work->summaryArgs.party = work->party;
        work->summaryArgs.partyCount = Party_GetCount(work->args->party);
        work->summaryArgs.unk28 = 0;
        sub_0208AD34(&work->summaryArgs, sub_02034818(Comm_GetLocalNetId()));
    } else {
        work->summaryArgs.party = work->partnerParty;
        work->summaryArgs.partyCount = Party_GetCount(work->partnerParty);
        // The partner's Chatot cry.
        work->summaryArgs.unk28 = (int)&work->chatot[Comm_GetLocalNetId() ^ 1];
        sub_0208AD34(&work->summaryArgs, sub_02034818(Comm_GetLocalNetId() ^ 1));
    }

    work->summaryArgs.unk11 = 1;
    work->summaryArgs.partySlot = work->cursor % 6;
    work->summaryArgs.unk12 = 1;
    work->summaryArgs.moveToLearn = 0;
    work->summaryArgs.unk2C = sub_02088288(work->args->saveData);
    work->summaryArgs.natDexEnabled = work->args->natDexEnabled;
    work->summaryArgs.options = work->args->options;
    work->summaryArgs.ribbons = Save_SpecialRibbons_Get(work->args->saveData);
    sub_02089D40(&work->summaryArgs, sWirelessTradeSelectMonSummaryPages);
    work->summaryApp = OverlayManager_New(&gOverlayTemplate_PokemonSummary, &work->summaryArgs, HEAP_ID_26);
    work->summaryOfPartner = ofPartner;
}

int WirelessTradeSelectMon_AskQuit(WirelessTradeSelectMon *work) {
    FillBgTilemapRect(work->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, 0, 32, 24, 0);
    WirelessTradeSelectMon_PrintMessage(&work->windows[23], 25, 1, work->msgData, work->msgFormat); // "Cancel trading?"
    work->callback = WirelessTradeSelectMon_HandleQuitPrompt;
    return 0;
}

int WirelessTradeSelectMon_HandleQuitPrompt(WirelessTradeSelectMon *work) {
    switch (WirelessTradeSelectMon_HandleYesNoPrompt(work->bgConfig, work->unk_664, &work->yesNoState, &work->yesNoPrompt, 1)) {
    case YESNORESPONSE_YES:
    case YESNORESPONSE_YES_TOUCH:
        FillBgTilemapRect(work->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, 0, 32, 24, 0);
        WirelessTradeSelectMon_PrintMessage(&work->windows[23], 28, 1, work->msgData, work->msgFormat); // "Waiting for your friend to finish..."
        WirelessTradeSelectMon_SendCommandIfChanged(work, WIRELESS_TRADE_CMD_ANSWER, WIRELESS_TRADE_ANSWER_QUIT);
        work->callback = WirelessTradeSelectMon_WaitQuitAnswers;
        work->tradeAccepted = FALSE;
        break;
    case YESNORESPONSE_NO:
    case YESNORESPONSE_NO_TOUCH:
        FillBgTilemapRect(work->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, 0, 32, 24, 0);
        WirelessTradeSelectMon_PrintMessage(&work->windows[21], 15, 1, work->msgData, work->msgFormat); // "Choose a Pokémon."
        work->callback = WirelessTradeSelectMon_SelectMon;
        break;
    }
    return 0;
}

int WirelessTradeSelectMon_ShowResultMessage(WirelessTradeSelectMon *work) {
    FillBgTilemapRect(work->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, 0, 32, 24, 0);
    WirelessTradeSelectMon_PrintMessage(&work->windows[23], work->resultMsg, 1, work->msgData, work->msgFormat);
    work->callback = WirelessTradeSelectMon_WaitResultMessage;
    return 0;
}

int WirelessTradeSelectMon_WaitResultMessage(WirelessTradeSelectMon *work) {
    if ((gSystem.newKeys & PAD_BUTTON_A) || gSystem.touchNew != 0) {
        FillBgTilemapRect(work->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, 0, 32, 24, 0);
        WirelessTradeSelectMon_PrintMessage(&work->windows[21], 15, 1, work->msgData, work->msgFormat); // "Choose a Pokémon."
        work->callback = WirelessTradeSelectMon_SelectMon;
        WirelessTradeSelectMon_RestoreIcons(work);
    }
    return 0;
}

void WirelessTradeSelectMon_PrintRosterMessage(WirelessTradeSelectMon *work, u32 msgId) {
    WirelessTradeSelectMon_PrintMessage(&work->windows[23], msgId, 1, work->rosterMsgData, work->rosterMsgFormat);
}

int WirelessTradeSelectMon_HandleGiveUpPrompt(WirelessTradeSelectMon *work) {
    int result = WirelessTradeSelectMon_HandleYesNoPrompt(work->bgConfig, work->unk_664, &work->yesNoState, &work->yesNoPrompt, 1);

    switch (result) {
    case YESNORESPONSE_YES:
    case YESNORESPONSE_YES_TOUCH:
        work->callback = WirelessTradeSelectMon_RegisterNextFriend;
        break;
    case YESNORESPONSE_NO:
    case YESNORESPONSE_NO_TOUCH:
        WirelessTradeSelectMon_PrintRosterMessage(work, 63); // "Your Friend Roster is full. Will you delete someone?"
        work->callback = WirelessTradeSelectMon_HandleRosterFullPrompt;
        break;
    }
    return 0;
}

int WirelessTradeSelectMon_HandleDeletePrompt(WirelessTradeSelectMon *work) {
    int result = WirelessTradeSelectMon_HandleYesNoPrompt(work->bgConfig, work->unk_664, &work->yesNoState, &work->yesNoPrompt, 1);

    switch (result) {
    case YESNORESPONSE_YES:
    case YESNORESPONSE_YES_TOUCH:
        sub_020311AC(Save_Frontier_GetStatic(work->saveData), work->chosenRosterSlot);
        sub_0202C338(work->roster, work->chosenRosterSlot);
        sub_0203A280(work->saveData, work->friendNetId, 31, HEAP_ID_26, 0);
        work->callback = WirelessTradeSelectMon_RegisterNextFriend;
        break;
    case YESNORESPONSE_NO:
    case YESNORESPONSE_NO_TOUCH:
        BufferPlayersName(work->rosterMsgFormat, 0, sub_02034818(work->friendNetId));
        WirelessTradeSelectMon_PrintRosterMessage(work, 64); // "Give up on registering {name}?"
        work->callback = WirelessTradeSelectMon_HandleGiveUpPrompt;
        break;
    }
    return 0;
}

#pragma force_active on

int WirelessTradeSelectMon_HandleRosterListInput(WirelessTradeSelectMon *work) {
    int choice = ListMenu_ProcessInput(work->rosterMenu);

    switch (choice) {
    case LIST_NOTHING_CHOSEN:
        return 0;
    case LIST_CANCEL:
        PlaySE(SEQ_SE_DP_SELECT);
        BufferPlayersName(work->rosterMsgFormat, 0, sub_02034818(work->friendNetId));
        WirelessTradeSelectMon_PrintRosterMessage(work, 64); // "Give up on registering {name}?"
        work->callback = WirelessTradeSelectMon_HandleGiveUpPrompt;
        break;
    default: {
        PlaySE(SEQ_SE_DP_SELECT);
        work->chosenRosterSlot = choice;
        PlayerProfile *profile = PlayerProfile_New(HEAP_ID_26);
        Save_Profile_PlayerName_Set(profile, sub_0202C254(work->roster, choice));
        BufferPlayersName(work->rosterMsgFormat, 0, profile);
        Heap_Free(profile);
        WirelessTradeSelectMon_PrintRosterMessage(work, 65); // "Delete {name}'s data from your Friend Roster?"
        work->callback = WirelessTradeSelectMon_HandleDeletePrompt;
        break;
    }
    }
    EraseFrameAndWindow1(&work->rosterWindow, FALSE);
    RemoveWindow(&work->rosterWindow);
    DestroyListMenu(work->rosterMenu, NULL, NULL);
    ListMenuItems_Delete(work->rosterItems);
    return 0;
}

int WirelessTradeSelectMon_ShowRosterList(WirelessTradeSelectMon *work) {
    int count = sub_0202C2F8(work->roster);
    work->rosterItems = ListMenuItems_New(count + 1, HEAP_ID_26);
    String *name = String_New(100, HEAP_ID_26);
    for (int i = 0; i < 32; i++) {
        if (sub_0202C2DC(work->roster, i) != FALSE) {
            CopyU16ArrayToString(name, sub_0202C254(work->roster, i));
            ListMenuItems_AddItem(work->rosterItems, name, i);
        }
    }
    ListMenuItems_AppendFromMsgData(work->rosterItems, work->rosterMsgData, 11, LIST_CANCEL); // "CANCEL"
    String_Delete(name);
    work->rosterMenu = WirelessTradeSelectMon_CreateRosterListMenu(work->rosterItems, count, &work->rosterWindow, work->bgConfig);
    work->callback = WirelessTradeSelectMon_HandleRosterListInput;
    return 0;
}

#pragma force_active reset

int WirelessTradeSelectMon_HandleRosterFullPrompt(WirelessTradeSelectMon *work) {
    int result = WirelessTradeSelectMon_HandleYesNoPrompt(work->bgConfig, work->unk_664, &work->yesNoState, &work->yesNoPrompt, 1);

    switch (result) {
    case YESNORESPONSE_YES:
    case YESNORESPONSE_YES_TOUCH:
        work->callback = WirelessTradeSelectMon_ShowRosterList;
        break;
    case YESNORESPONSE_NO:
    case YESNORESPONSE_NO_TOUCH:
        BufferPlayersName(work->rosterMsgFormat, 0, sub_02034818(work->friendNetId));
        WirelessTradeSelectMon_PrintRosterMessage(work, 64); // "Give up on registering {name}?"
        work->callback = WirelessTradeSelectMon_HandleGiveUpPrompt;
        break;
    }
    return 0;
}

int WirelessTradeSelectMon_HandleRegisterPrompt(WirelessTradeSelectMon *work) {
    int result = WirelessTradeSelectMon_HandleYesNoPrompt(work->bgConfig, work->unk_664, &work->yesNoState, &work->yesNoPrompt, 1);

    switch (result) {
    case YESNORESPONSE_YES:
    case YESNORESPONSE_YES_TOUCH: {
        int i;
        work->callback = WirelessTradeSelectMon_RegisterNextFriend;
        for (i = 0; i < 32; i++) {
            if (!sub_0202C2DC(work->roster, i)) {
                sub_0203A280(work->saveData, work->friendNetId, i, HEAP_ID_26, 0);
                break;
            }
        }
        if (i == 32) {
            WirelessTradeSelectMon_PrintRosterMessage(work, 63); // "Your Friend Roster is full. Will you delete someone?"
            work->callback = WirelessTradeSelectMon_HandleRosterFullPrompt;
            return 0;
        }
        break;
    }
    case YESNORESPONSE_NO:
    case YESNORESPONSE_NO_TOUCH:
        work->callback = WirelessTradeSelectMon_RegisterNextFriend;
        break;
    }
    return 0;
}

int WirelessTradeSelectMon_WaitQuitSync(WirelessTradeSelectMon *work) {
    return sub_02037B38(0x13) ? 2 : 0;
}

int WirelessTradeSelectMon_RegisterNextFriend(WirelessTradeSelectMon *work) {
    int i;
    int j;
    UnkStruct_021D2230 *roster;

    work->friendNetId = -1;
    for (i = 0; i < Comm_CountConnected(); i++) {
        if (work->friendStates[i] == 2) {
            work->friendNetId = i;
            work->friendStates[i] = 0;
            break;
        }
    }
    if (work->friendNetId == -1) {
        DestroyMsgData(work->rosterMsgData);
        MessageFormat_Delete(work->rosterMsgFormat);
        sub_02037AC0(0x13);
        WirelessTradeSelectMon_PrintMessage(&work->windows[23], 28, 1, work->msgData, work->msgFormat); // "Waiting for your friend to finish..."
        work->callback = WirelessTradeSelectMon_WaitQuitSync;
        return 0;
    }
    if (Bag_HasItem(Save_Bag_Get(work->saveData), ITEM_PAL_PAD, 1, HEAP_ID_26) == TRUE) {
        BufferPlayersName(work->rosterMsgFormat, 0, sub_02034818(work->friendNetId));
        WirelessTradeSelectMon_PrintRosterMessage(work, 62); // "Register {name} in your Pal Pad?"
        work->callback = WirelessTradeSelectMon_HandleRegisterPrompt;
        return 0;
    }
    roster = sub_0202C6F4(work->saveData);
    for (j = 0; j < 32; j++) {
        if (!sub_0202C2DC(roster, j)) {
            sub_0203A280(work->saveData, work->friendNetId, j, HEAP_ID_26, 0);
            break;
        }
    }
    work->callback = WirelessTradeSelectMon_RegisterNextFriend;
    return 0;
}

int WirelessTradeSelectMon_StartFriendRegistration(WirelessTradeSelectMon *work) {
    // unk_02037C94.h declares sub_0203A1C4 as void, but it returns whether anyone can be registered.
    if (!((s32(*)(SaveData *, void *, enum HeapID))sub_0203A1C4)(work->saveData, work->friendStates, HEAP_ID_26)) {
        sub_02037AC0(0x13);
        WirelessTradeSelectMon_PrintMessage(&work->windows[23], 28, 1, work->msgData, work->msgFormat); // "Waiting for your friend to finish..."
        work->callback = WirelessTradeSelectMon_WaitQuitSync;
        return 0;
    }
    work->rosterMsgFormat = MessageFormat_New(HEAP_ID_26);
    work->rosterMsgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0779_bin, HEAP_ID_26);
    work->roster = sub_0202C6F4(work->saveData);
    work->callback = WirelessTradeSelectMon_RegisterNextFriend;
    return 0;
}

int WirelessTradeSelectMon_WaitQuitAnswers(WirelessTradeSelectMon *work) {
    if (work->answers[0] != 0 && work->answers[1] != 0) {
        if (work->answers[0] == WIRELESS_TRADE_ANSWER_QUIT && work->answers[1] == WIRELESS_TRADE_ANSWER_QUIT) {
            work->callback = WirelessTradeSelectMon_StartFriendRegistration;
        } else {
            work->callback = WirelessTradeSelectMon_ShowResultMessage;
        }
        work->answers[0] = 0;
        work->answers[1] = 0;
        work->resultMsg = 29; // "Your friend apparently wants to trade a Pokémon with you."
    }
    return 0;
}

TouchscreenListMenu *WirelessTradeSelectMon_CreateTouchMenu(WirelessTradeSelectMon *work, u32 numItems, u32 y) {
    TouchscreenListMenuHeader header;
    TouchscreenListMenu *menu;

    work->touchMenuSpawner = TouchscreenListMenuSpawner_Create(HEAP_ID_26, NULL);
    MI_CpuFill8(&header, 0, sizeof(header));
    header.template = sWirelessTradeSelectMonTouchMenuTemplate;
    header.listMenuItems = work->menuItems;
    header.bgConfig = work->bgConfig;
    header.numWindows = (u8)numItems;
    menu = TouchscreenListMenu_Create(work->touchMenuSpawner, &header, 0, 17, (u8)y, 13, 0);
    WirelessTradeSelectMon_StartMenuDim();
    return menu;
}

int WirelessTradeSelectMon_ShowOwnMonMenu(WirelessTradeSelectMon *work) {
    BufferBoxMonNickname(work->nicknameFormat, 0, Mon_GetBoxMon(Party_GetMonByIndex(work->party, work->cursor)));
    FillBgTilemapRect(work->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, 0, 32, 24, 0);
    WirelessTradeSelectMon_PrintMessage(&work->windows[22], 16, 1, work->msgData, work->nicknameFormat); // "{nickname} is selected."
    work->menuItems = ListMenuItems_New(3, HEAP_ID_26);
    ListMenuItems_AppendFromMsgData(work->menuItems, work->msgData, 17, 0); // "SUMMARY"
    ListMenuItems_AppendFromMsgData(work->menuItems, work->msgData, 18, 1); // "TRADE"
    ListMenuItems_AppendFromMsgData(work->menuItems, work->msgData, 19, 2); // "CANCEL"
    work->touchMenu = WirelessTradeSelectMon_CreateTouchMenu(work, 3, 9);
    work->callback = WirelessTradeSelectMon_HandleOwnMonMenu;
    return 0;
}

int WirelessTradeSelectMon_HandleOwnMonMenu(WirelessTradeSelectMon *work) {
    u32 input = TouchscreenListMenu_HandleInput(work->touchMenu);

    switch (input) {
    case 0: // summary
        FillBgTilemapRect(work->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, 0, 32, 24, 0);
        WirelessTradeSelectMon_PrintMessage(&work->windows[21], 15, 1, work->msgData, work->msgFormat); // "Choose a Pokémon."
        WirelessTradeSelectMon_DestroyTouchMenu(work);
        ListMenuItems_Delete(work->menuItems);
        work->callback = WirelessTradeSelectMon_SelectMon;
        return 3;
    case 1: // trade
        FillBgTilemapRect(work->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, 0, 32, 24, 0);
        WirelessTradeSelectMon_PrintMessage(&work->windows[23], 20, 1, work->msgData, work->msgFormat); // "Communicating. Please stand by..."
        WirelessTradeSelectMon_DestroyTouchMenu(work);
        ListMenuItems_Delete(work->menuItems);
        work->callback = WirelessTradeSelectMon_OfferTrade;
        break;
    case 2: // cancel
    case (u32)LIST_CANCEL:
        FillBgTilemapRect(work->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, 0, 32, 24, 0);
        WirelessTradeSelectMon_PrintMessage(&work->windows[21], 15, 1, work->msgData, work->msgFormat); // "Choose a Pokémon."
        WirelessTradeSelectMon_DestroyTouchMenu(work);
        ListMenuItems_Delete(work->menuItems);
        work->callback = WirelessTradeSelectMon_SelectMon;
        break;
    }
    return 0;
}

int WirelessTradeSelectMon_OfferTrade(WirelessTradeSelectMon *work) {
    FillBgTilemapRect(work->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, 0, 32, 24, 0);
    WirelessTradeSelectMon_PrintMessage(&work->windows[23], 20, 1, work->msgData, work->msgFormat); // "Communicating. Please stand by..."
    WirelessTradeSelectMon_SendCommandIfChanged(work, WIRELESS_TRADE_CMD_ANSWER, WIRELESS_TRADE_ANSWER_OFFER);
    work->callback = WirelessTradeSelectMon_WaitOfferAnswers;
    return 0;
}

int WirelessTradeSelectMon_WaitOfferAnswers(WirelessTradeSelectMon *work) {
    if (work->answers[0] != 0 && work->answers[1] != 0) {
        if (work->answers[0] == WIRELESS_TRADE_ANSWER_OFFER && work->answers[1] == WIRELESS_TRADE_ANSWER_OFFER) {
            work->callback = WirelessTradeSelectMon_StartTradeSlide;
        } else {
            work->callback = WirelessTradeSelectMon_ShowResultMessage;
        }
        work->resultMsg = 24; // "The trade has been canceled."
        work->answers[0] = 0;
        work->answers[1] = 0;
    }
    return 0;
}

#pragma force_active on

int WirelessTradeSelectMon_StartTradeSlide(WirelessTradeSelectMon *work) {
    int i;

    for (i = 0; i < 6; i++) {
        if (i != work->cursor) {
            Sprite_SetDrawFlag(work->iconSprites[i], FALSE);
            if (work->mons[i].heldItem != 0) {
                Sprite_SetDrawFlag(work->itemSprites[i], FALSE);
            }
            if (work->mons[i].capsule != 0) {
                Sprite_SetDrawFlag(work->capsuleSprites[i], FALSE);
            }
        }
        ClearWindowTilemapAndScheduleTransfer(&work->windows[i + 7]);
        if (i != work->partnerCursor - 6) {
            Sprite_SetDrawFlag(work->iconSprites[i + 6], FALSE);
            if (work->mons[i + 6].heldItem != 0) {
                Sprite_SetDrawFlag(work->itemSprites[i + 6], FALSE);
            }
            if (work->mons[i + 6].capsule != 0) {
                Sprite_SetDrawFlag(work->capsuleSprites[i + 6], FALSE);
            }
        }
        ClearWindowTilemapAndScheduleTransfer(&work->windows[i + 13]);
    }
    Sprite_SetDrawFlag(work->cursorSprites[0], FALSE);
    Sprite_SetDrawFlag(work->cursorSprites[1], FALSE);
    WirelessTradeSelectMon_InitSlide(&work->slides[0], sWirelessTradeSelectMonSlotPos[work->cursor].x + 16, ov65_0221FF50[work->cursor].x - 6, 48, 32);
    WirelessTradeSelectMon_InitSlide(&work->slides[1], sWirelessTradeSelectMonSlotPos[work->partnerCursor].x + 16, ov65_0221FF50[work->partnerCursor].x - 6, 176, 32);
    work->slideStep = 0;
    work->callback = WirelessTradeSelectMon_UpdateTradeSlide;
    return 0;
}

#pragma force_active reset

void WirelessTradeSelectMon_InitSlide(WirelessTradeSlide *slide, s32 startX, s32 startY, s32 endX, s32 endY) {
    slide->startX = startX * FX32_ONE;
    slide->startY = startY * FX32_ONE;
    slide->endX = endX * FX32_ONE;
    slide->endY = endY * FX32_ONE;
}

void WirelessTradeSelectMon_SlideMonIcon(Sprite *icon, Sprite *item, Sprite *capsule, s32 step, const WirelessTradeSlide *slide, const WirelessTradeMonInfo *info) {
    VecFx32 pos;
    VecFx32 itemPos;
    VecFx32 capsulePos;

    pos.x = slide->startX + ((slide->endX - slide->startX) / 20) * step;
    pos.y = slide->startY + ((slide->endY - slide->startY) / 20) * step;
    Sprite_SetMatrix(icon, &pos);
    if (info->heldItem != 0) {
        itemPos.x = pos.x + 20 * FX32_ONE;
        itemPos.y = pos.y + 22 * FX32_ONE;
        Sprite_SetMatrix(item, &itemPos);
    }
    if (info->capsule != 0) {
        capsulePos.x = pos.x + 28 * FX32_ONE;
        capsulePos.y = pos.y + 22 * FX32_ONE;
        Sprite_SetMatrix(capsule, &capsulePos);
    }
}

int WirelessTradeSelectMon_UpdateTradeSlide(WirelessTradeSelectMon *work) {
    u32 slot;
    u32 partnerSlot;

    work->slideStep++;
    slot = work->cursor;
    WirelessTradeSelectMon_SlideMonIcon(work->iconSprites[slot], work->itemSprites[slot], work->capsuleSprites[slot], work->slideStep, &work->slides[0], &work->mons[slot]);
    partnerSlot = work->partnerCursor;
    WirelessTradeSelectMon_SlideMonIcon(work->iconSprites[partnerSlot], work->itemSprites[partnerSlot], work->capsuleSprites[partnerSlot], work->slideStep, &work->slides[1], &work->mons[partnerSlot]);
    if (work->slideStep == 21) {
        WirelessTradeSelectMon_PrintMonNickname(&work->windows[19], work->party, work->cursor, 8, 1);
        WirelessTradeSelectMon_PrintMonNickname(&work->windows[20], work->partnerParty, work->partnerCursor - 6, 8, 1);
        WirelessTradeSelectMon_ShowPartnerMon(work, work->partnerCursor);
        work->callback = WirelessTradeSelectMon_AskTradeConfirm;
        Sprite_SetDrawFlag(work->tradeSprite, TRUE);
    }
    return 0;
}

void WirelessTradeSelectMon_RestoreIcons(WirelessTradeSelectMon *work) {
    s32 i;
    u8 *shiftedMon;
    const WirelessTradeSlotPos *pos;
    u8 *shiftedSprite;
    Window *windows;

    ClearWindowTilemapAndScheduleTransfer(&work->windows[19]);
    ClearWindowTilemapAndScheduleTransfer(&work->windows[20]);
    // Both work pointers are walked: one by a mon info, one by a sprite slot per slot.
    shiftedMon = (u8 *)work;
    pos = sWirelessTradeSelectMonSlotPos;
    i = 0;
    shiftedSprite = (u8 *)work;
    windows = work->windows;
    for (; i < WIRELESS_TRADE_NUM_SLOTS; i++, shiftedMon += sizeof(WirelessTradeMonInfo), pos++, shiftedSprite += sizeof(Sprite *)) {
        if (((WirelessTradeSelectMon *)shiftedMon)->mons[0].species != SPECIES_NONE) {
            WirelessTradeSelectMon_SetSpritePos(((WirelessTradeSelectMon *)shiftedSprite)->iconSprites[0], pos->x + 16, pos->y - 6);
            Sprite_SetDrawFlag(((WirelessTradeSelectMon *)shiftedSprite)->iconSprites[0], TRUE);
            if (((WirelessTradeSelectMon *)shiftedMon)->mons[0].heldItem != 0) {
                WirelessTradeSelectMon_SetSpritePos(((WirelessTradeSelectMon *)shiftedSprite)->itemSprites[0], pos->x + 36, pos->y + 16);
                Sprite_SetDrawFlag(((WirelessTradeSelectMon *)shiftedSprite)->itemSprites[0], TRUE);
            }
            if (((WirelessTradeSelectMon *)shiftedMon)->mons[0].capsule != 0) {
                WirelessTradeSelectMon_SetSpritePos(((WirelessTradeSelectMon *)shiftedSprite)->capsuleSprites[0], pos->x + 44, pos->y + 16);
                Sprite_SetDrawFlag(((WirelessTradeSelectMon *)shiftedSprite)->capsuleSprites[0], TRUE);
            }
            ScheduleWindowCopyToVram(&windows[i + 7]);
        }
    }
    Sprite_SetDrawFlag(work->cursorSprites[0], TRUE);
    Sprite_SetDrawFlag(work->cursorSprites[1], TRUE);
    WirelessTradeSelectMon_HideMonDetails(work->windows, 1, work);
    Sprite_SetDrawFlag(work->detailSprites[3], FALSE);
    Sprite_SetDrawFlag(work->tradeSprite, FALSE);
}

int WirelessTradeSelectMon_AskTradeConfirm(WirelessTradeSelectMon *work) {
    Pokemon *mon;

    FillBgTilemapRect(work->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, 0, 32, 24, 0);
    mon = Party_GetMonByIndex(work->party, work->cursor);
    BufferBoxMonNickname(work->nicknameFormat, 0, Mon_GetBoxMon(mon));
    mon = Party_GetMonByIndex(work->partnerParty, work->partnerCursor - 6);
    BufferBoxMonNickname(work->nicknameFormat, 1, Mon_GetBoxMon(mon));
    WirelessTradeSelectMon_PrintMessage(&work->windows[23], 21, 1, work->msgData, work->nicknameFormat); // "Trade {0} in return for {1}?"
    work->callback = WirelessTradeSelectMon_HandleTradeConfirm;
    return 0;
}

void WirelessTradeSelectMon_AcceptTrade(WirelessTradeSelectMon *work) {
    WirelessTradeSelectMon_SendCommandIfChanged(work, WIRELESS_TRADE_CMD_ANSWER, WIRELESS_TRADE_ANSWER_CONFIRM);
    work->tradeAccepted = TRUE;
}

void WirelessTradeSelectMon_PrintMessageInWindow(WirelessTradeSelectMon *work, u32 windowIndex, u32 msgId) {
    FillBgTilemapRect(work->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, 0, 32, 24, 0);
    WirelessTradeSelectMon_PrintMessage(&work->windows[windowIndex], msgId, 1, work->msgData, work->msgFormat);
}

int WirelessTradeSelectMon_HandleTradeConfirm(WirelessTradeSelectMon *work) {
    u32 result = WirelessTradeSelectMon_HandleYesNoPrompt(work->bgConfig, work->unk_664, &work->yesNoState, &work->yesNoPrompt, 0);

    switch (result) {
    case YESNORESPONSE_YES:
    case YESNORESPONSE_YES_TOUCH:
        if (WirelessTradeSelectMon_HasOtherNonEggMon(work)) {
            switch (WirelessTradeSelectMon_FindBadEggParty(work)) {
            case 0:
                if (work->mons[work->cursor].capsule == 0) {
                    WirelessTradeSelectMon_AcceptTrade(work);
                    WirelessTradeSelectMon_PrintMessageInWindow(work, 23, 20); // "Communicating. Please stand by..."
                    work->callback = WirelessTradeSelectMon_WaitConfirmAnswers;
                } else {
                    work->callback = WirelessTradeSelectMon_AskDetachCapsule;
                }
                break;
            case 1:
                WirelessTradeSelectMon_PrintMessageInWindow(work, 23, 37); // "There is a Pokémon that cannot be traded in your party."
                work->callback = WirelessTradeSelectMon_WaitConfirmAnswers;
                WirelessTradeSelectMon_SendCommandIfChanged(work, WIRELESS_TRADE_CMD_ANSWER, WIRELESS_TRADE_ANSWER_REFUSE);
                break;
            case 2:
                WirelessTradeSelectMon_PrintMessageInWindow(work, 23, 38); // "There is a Pokémon that cannot be traded in the other Trainer's party."
                work->callback = WirelessTradeSelectMon_WaitConfirmAnswers;
                WirelessTradeSelectMon_SendCommandIfChanged(work, WIRELESS_TRADE_CMD_ANSWER, WIRELESS_TRADE_ANSWER_REFUSE);
                break;
            }
        } else {
            work->callback = WirelessTradeSelectMon_WaitConfirmAnswers;
            WirelessTradeSelectMon_SendCommandIfChanged(work, WIRELESS_TRADE_CMD_ANSWER, WIRELESS_TRADE_ANSWER_REFUSE);
        }
        break;
    case YESNORESPONSE_NO:
    case YESNORESPONSE_NO_TOUCH:
        FillBgTilemapRect(work->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, 0, 32, 24, 0);
        WirelessTradeSelectMon_PrintMessage(&work->windows[23], 20, 1, work->msgData, work->msgFormat); // "Communicating. Please stand by..."
        work->callback = WirelessTradeSelectMon_WaitConfirmAnswers;
        WirelessTradeSelectMon_SendCommandIfChanged(work, WIRELESS_TRADE_CMD_ANSWER, WIRELESS_TRADE_ANSWER_REFUSE);
        break;
    }
    return 0;
}

int WirelessTradeSelectMon_AskDetachCapsule(WirelessTradeSelectMon *work) {
    FillBgTilemapRect(work->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, 0, 32, 24, 0);
    WirelessTradeSelectMon_PrintMessage(&work->windows[23], 36, 1, work->msgData, work->nicknameFormat); // "The Ball Capsule will become detached. Is that OK?"
    work->callback = WirelessTradeSelectMon_HandleCapsulePrompt;
    return 0;
}

int WirelessTradeSelectMon_HandleCapsulePrompt(WirelessTradeSelectMon *work) {
    u32 result = WirelessTradeSelectMon_HandleYesNoPrompt(work->bgConfig, work->unk_664, &work->yesNoState, &work->yesNoPrompt, 0);

    switch (result) {
    case YESNORESPONSE_YES:
    case YESNORESPONSE_YES_TOUCH:
        WirelessTradeSelectMon_AcceptTrade(work);
        WirelessTradeSelectMon_PrintMessageInWindow(work, 23, 20); // "Communicating. Please stand by..."
        work->callback = WirelessTradeSelectMon_WaitConfirmAnswers;
        break;
    case YESNORESPONSE_NO:
    case YESNORESPONSE_NO_TOUCH:
        FillBgTilemapRect(work->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, 0, 32, 24, 0);
        WirelessTradeSelectMon_PrintMessage(&work->windows[23], 20, 1, work->msgData, work->msgFormat); // "Communicating. Please stand by..."
        work->callback = WirelessTradeSelectMon_WaitConfirmAnswers;
        WirelessTradeSelectMon_SendCommandIfChanged(work, WIRELESS_TRADE_CMD_ANSWER, WIRELESS_TRADE_ANSWER_REFUSE);
        break;
    }
    return 0;
}

int WirelessTradeSelectMon_WaitConfirmAnswers(WirelessTradeSelectMon *work) {
    if (work->answers[0] != 0 && work->answers[1] != 0) {
        if (work->answers[0] == WIRELESS_TRADE_ANSWER_CONFIRM && work->answers[1] == WIRELESS_TRADE_ANSWER_CONFIRM) {
            work->tradesDone++;
            work->callback = WirelessTradeSelectMon_FinishTrade;
        } else {
            work->callback = WirelessTradeSelectMon_ShowResultMessage;
        }
        work->answers[0] = 0;
        work->answers[1] = 0;
        if (WirelessTradeSelectMon_HasOtherNonEggMon(work)) {
            work->resultMsg = 24; // "The trade has been canceled."
        } else {
            work->resultMsg = 34; // "If you trade that Pokémon, you'll get stuck in your adventure!"
        }
    }
    return 0;
}

int WirelessTradeSelectMon_FinishTrade(WirelessTradeSelectMon *work) {
    sub_02034AEC(work->saveData, 1);
    WirelessTradeSelectMon_TradeMons(work->party, work->partnerParty, work->cursor, work->partnerCursor - 6, work->args);
    work->callback = WirelessTradeSelectMon_ShowResultMessage;
    return 2;
}

int WirelessTradeSelectMon_ShowPartnerMonMenu(WirelessTradeSelectMon *work) {
    Pokemon *mon = Party_GetMonByIndex(work->partnerParty, work->cursor - 6);

    BufferBoxMonNickname(work->nicknameFormat, 0, Mon_GetBoxMon(mon));
    FillBgTilemapRect(work->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, 0, 32, 24, 0);
    WirelessTradeSelectMon_PrintMessage(&work->windows[22], 16, 1, work->msgData, work->nicknameFormat); // "{nickname} is selected."
    work->menuItems = ListMenuItems_New(2, HEAP_ID_26);
    ListMenuItems_AppendFromMsgData(work->menuItems, work->msgData, 17, 0); // "SUMMARY"
    ListMenuItems_AppendFromMsgData(work->menuItems, work->msgData, 19, 1); // "CANCEL"
    work->touchMenu = WirelessTradeSelectMon_CreateTouchMenu(work, 2, 12);
    work->callback = WirelessTradeSelectMon_HandlePartnerMonMenu;
    return 0;
}

void WirelessTradeSelectMon_DestroyTouchMenu(WirelessTradeSelectMon *work) {
    TouchscreenListMenu_Destroy(work->touchMenu);
    TouchscreenListMenuSpawner_Destroy(work->touchMenuSpawner);
    WirelessTradeSelectMon_EndMenuDim();
}

int WirelessTradeSelectMon_HandlePartnerMonMenu(WirelessTradeSelectMon *work) {
    u32 input = TouchscreenListMenu_HandleInput(work->touchMenu);

    switch (input) {
    case 0: // summary
        FillBgTilemapRect(work->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, 0, 32, 24, 0);
        WirelessTradeSelectMon_PrintMessage(&work->windows[21], 15, 1, work->msgData, work->msgFormat); // "Choose a Pokémon."
        work->callback = WirelessTradeSelectMon_SelectMon;
        WirelessTradeSelectMon_DestroyTouchMenu(work);
        ListMenuItems_Delete(work->menuItems);
        return 3;
    case 1: // cancel
    case (u32)LIST_CANCEL:
        FillBgTilemapRect(work->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, 0, 32, 24, 0);
        WirelessTradeSelectMon_PrintMessage(&work->windows[21], 15, 1, work->msgData, work->msgFormat); // "Choose a Pokémon."
        WirelessTradeSelectMon_DestroyTouchMenu(work);
        ListMenuItems_Delete(work->menuItems);
        work->callback = WirelessTradeSelectMon_SelectMon;
        break;
    }
    return 0;
}

void WirelessTradeSelectMon_TradeMons(Party *party, Party *partnerParty, u32 slot, u32 partnerSlot, WirelessTradeSelectMonArgs *args) {
    Pokemon *sent;
    Pokemon *received;
    SaveVarsFlags *varsFlags;
    u8 data[2];

    sent = AllocMonZeroed(HEAP_ID_26);
    received = AllocMonZeroed(HEAP_ID_26);
    CopyPokemonToPokemon(Party_GetMonByIndex(party, slot), sent);
    CopyPokemonToPokemon(Party_GetMonByIndex(partnerParty, partnerSlot), received);
    if (GetMonData(received, MON_DATA_SPECIES, NULL) == SPECIES_ARCEUS) {
        if (GetMonData(received, MON_DATA_FATEFUL_ENCOUNTER, NULL) != 0 || (GetMonData(received, MON_DATA_MET_LOCATION, NULL) == 86 && GetMonData(received, MON_DATA_FATEFUL_ENCOUNTER, NULL) == 0)) {
            varsFlags = Save_VarsFlags_Get(args->saveData);
            if (Save_VarsFlags_GetVar404C(varsFlags) == 0) {
                Save_VarsFlags_SetVar404C(varsFlags, 1);
            }
        }
    }
    SetMonData(received, MON_DATA_GENDER, NULL);
    if (GetMonData(received, MON_DATA_IS_EGG, NULL) == FALSE) {
        data[1] = 70;
        data[0] = 0;
        SetMonData(received, MON_DATA_FRIENDSHIP, &data[1]);
        SetMonData(received, MON_DATA_MOOD, &data[0]);
    }
    MonSetTrainerMemo(received, sub_02034818(Comm_GetLocalNetId()), 5, 0, HEAP_ID_FIELD2);
    Pokemon_RemoveCapsule(received);
    CopyPokemonToPokemon(sent, args->unk38);
    CopyPokemonToPokemon(received, args->unk3C);
    PlayerProfile_Copy(sub_02034818(Comm_GetLocalNetId() ^ 1), args->partnerProfile);
    args->unk28 = slot;
    if (Party_HasMon(party, SPECIES_CHATOT) == FALSE) {
        Chatot_Invalidate(Save_Chatot_Get(args->saveData));
    }
    UpdatePokedexWithReceivedSpecies(args->saveData, received);
    CopyPokemonToPokemon(received, Party_GetMonByIndex(party, slot));
    CopyPokemonToPokemon(sent, Party_GetMonByIndex(partnerParty, partnerSlot));
    Party_ResetMonAprijuiceModifiers(party, slot);
    Party_ResetMonAprijuiceModifiers(partnerParty, partnerSlot);
    GameStats_Inc(args->gameStats, GAME_STAT_LOCAL_LINK_TRADES);
    Heap_Free(sent);
    Heap_Free(received);
}

void WirelessTradeSelectMon_ScrollBg(WirelessTradeSelectMon *work) {
    work->bgScrollY += 2;
    BgSetPosTextAndCommit(work->bgConfig, GF_BG_LYR_SUB_2, BG_POS_OP_SET_X, work->bgScrollY);
    BgSetPosTextAndCommit(work->bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_X, work->bgScrollY);
}

void WirelessTradeSelectMon_SetSpritePos(Sprite *sprite, s32 x, s32 y) {
    VecFx32 pos;
    pos.x = x * FX32_ONE;
    pos.y = y * FX32_ONE;
    Sprite_SetMatrix(sprite, &pos);
}

void WirelessTradeSelectMon_ShowWaitingIcon(WirelessTradeSelectMon *work) {
    work->waitingIcon = WaitingIcon_New(&work->windows[23], 985);
}

void WirelessTradeSelectMon_HideWaitingIcon(WirelessTradeSelectMon *work) {
    if (work->waitingIcon != NULL) {
        WaitingIcon_Delete(work->waitingIcon);
        work->waitingIcon = NULL;
    }
}

// FALSE if every other mon of the party is an Egg.
s32 WirelessTradeSelectMon_HasOtherNonEggMon(WirelessTradeSelectMon *work) {
    s32 eggs = 0;
    s32 count = Party_GetCount(work->party);
    s32 i = 0;
    s32 cursor;

    if (count > 0) {
        cursor = work->cursor;
        // The work pointer is walked by one mon info per slot.
        do {
            if (i != cursor) {
                eggs += work->mons[0].isEgg;
            }
            i++;
            work = (WirelessTradeSelectMon *)((u8 *)work + sizeof(WirelessTradeMonInfo));
        } while (i < count);
    }
    if (eggs == count - 1) {
        return FALSE;
    }
    return TRUE;
}

// 1 if the player's party holds a Bad Egg, 2 if the partner's does, else 0.
s32 WirelessTradeSelectMon_FindBadEggParty(WirelessTradeSelectMon *work) {
    s32 count;
    s32 i;

    count = Party_GetCount(work->party);
    for (i = 0; i < count; i++) {
        if (GetMonData(Party_GetMonByIndex(work->party, i), MON_DATA_CHECKSUM_FAILED, NULL) != 0) {
            return 1;
        }
    }
    count = Party_GetCount(work->partnerParty);
    for (i = 0; i < count; i++) {
        if (GetMonData(Party_GetMonByIndex(work->partnerParty, i), MON_DATA_CHECKSUM_FAILED, NULL) != 0) {
            return 2;
        }
    }
    return 0;
}

void WirelessTradeSelectMon_StartTimeout(WirelessTradeSelectMon *work) {
    work->timeoutActive = TRUE;
    work->timeoutCounter = 0;
}

void WirelessTradeSelectMon_UpdateTimeout(WirelessTradeSelectMon *work) {
    if (work->timeoutActive != FALSE) {
        work->timeoutCounter++;
        if (work->timeoutCounter > 1800) {
            sub_02039AD8(4);
        }
    }
}

// Darkens the main screen outside window 0 while a touch menu is open.
void WirelessTradeSelectMon_StartMenuDim(void) {
    GX_SetVisibleWnd(GX_WNDMASK_W0);
    G2_SetBlendBrightness(GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ, 7);
}

void WirelessTradeSelectMon_EndMenuDim(void) {
    G2_BlendNone();
    GX_SetVisibleWnd(GX_WNDMASK_NONE);
}

#pragma force_active on

void WirelessTradeSelectMon_AddWindows(BgConfig *bgConfig, Window *windows, Options *options) {
    int i;

    AddWindowParameterized(bgConfig, &windows[0], GF_BG_LYR_MAIN_1, 2, 1, 10, 2, 8, 1);
    FillWindowPixelBuffer(&windows[0], 0);
    AddWindowParameterized(bgConfig, &windows[1], GF_BG_LYR_MAIN_1, 20, 1, 10, 2, 8, 21);
    FillWindowPixelBuffer(&windows[1], 0);
    AddWindowParameterized(bgConfig, &windows[6], GF_BG_LYR_MAIN_1, 26, 21, 5, 2, 8, 41);
    FillWindowPixelBuffer(&windows[6], 0);
    LoadUserFrameGfx2(bgConfig, GF_BG_LYR_MAIN_0, 985, 10, Options_GetFrame(options), HEAP_ID_26);
    LoadUserFrameGfx1(bgConfig, GF_BG_LYR_MAIN_0, 1015, 11, 0, HEAP_ID_26);
    AddWindowParameterized(bgConfig, &windows[21], GF_BG_LYR_MAIN_0, 2, 21, 20, 2, 13, 1);
    FillWindowPixelBuffer(&windows[21], 0);
    AddWindowParameterized(bgConfig, &windows[22], GF_BG_LYR_MAIN_0, 2, 21, 27, 2, 13, 40);
    FillWindowPixelBuffer(&windows[22], 0);
    AddWindowParameterized(bgConfig, &windows[23], GF_BG_LYR_MAIN_0, 2, 19, 27, 4, 13, 54);
    FillWindowPixelBuffer(&windows[23], 0);
    AddWindowParameterized(bgConfig, &windows[25], GF_BG_LYR_MAIN_0, 20, 19, 11, 4, 13, 162);
    FillWindowPixelBuffer(&windows[25], 0);
    for (i = 0; i < 14; i++) {
        AddWindowParameterized(bgConfig, &windows[i + 7], GF_BG_LYR_MAIN_1, sWirelessTradeSelectMonNameWindowPos[i].x, sWirelessTradeSelectMonNameWindowPos[i].y, 8, 2, 8, 51 + i * 16);
        FillWindowPixelBuffer(&windows[i + 7], 0);
    }
    for (i = 0; i < 8; i++) {
        AddWindowParameterized(bgConfig, &windows[i + 26], GF_BG_LYR_SUB_0, sWirelessTradeSelectMonDetailWindows[i].x, sWirelessTradeSelectMonDetailWindows[i].y, sWirelessTradeSelectMonDetailWindows[i].width, sWirelessTradeSelectMonDetailWindows[i].height, 8, sWirelessTradeSelectMonDetailWindows[i].baseTile);
        FillWindowPixelBuffer(&windows[i + 26], 0);
    }
}

#pragma force_active reset

void WirelessTradeSelectMon_RemoveWindows(Window *windows) {
    int i;

    for (i = 0; i < 14; i++) {
        RemoveWindow(&windows[i + 7]);
    }
    for (i = 0; i < 8; i++) {
        RemoveWindow(&windows[i + 26]);
    }
    RemoveWindow(&windows[0]);
    RemoveWindow(&windows[1]);
    RemoveWindow(&windows[6]);
    RemoveWindow(&windows[21]);
    RemoveWindow(&windows[22]);
    RemoveWindow(&windows[23]);
    RemoveWindow(&windows[25]);
}

#pragma force_active on

// x == 1 centers the string in the window.
void WirelessTradeSelectMon_PrintString(Window *window, String *string, int unused, int textSpeed, int x, int y) {
    int printX = x;

    if (x == 1) {
        int width = FontID_String_GetWidth(0, string, 0);
        printX = (window->width * 8 - width) / 2;
    }
    AddTextPrinterParameterizedWithColor(window, 0, string, printX, y, textSpeed, MAKE_TEXT_COLOR(11, 12, 0), NULL);
}

#pragma force_active reset

u32 WirelessTradeSelectMon_PrintMessage(Window *window, u32 msgId, u32 frame, MsgData *msgData, MessageFormat *msgFormat) {
    String *string = ReadMsgData_ExpandPlaceholders(msgFormat, msgData, msgId, HEAP_ID_26);
    u32 printerId;

    if (frame == 1) {
        DrawFrameAndWindow2(window, TRUE, 985, 10);
    } else {
        DrawFrameAndWindow1(window, TRUE, 1015, 11);
    }
    FillWindowPixelBuffer(window, 15);
    // The font id is the frame argument (1), passed without truncation.
    printerId = ((u32(*)(Window *, u32, String *, u32, u32, u32, u32, PrinterCallback_t))AddTextPrinterParameterizedWithColor)(window, frame, string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 15), NULL);
    ScheduleWindowCopyToVram(window);
    String_Delete(string);
    return printerId;
}

YesNoPrompt *WirelessTradeSelectMon_CreateYesNoPrompt(BgConfig *bgConfig) {
    YesNoPromptTemplate promptTemplate;
    YesNoPrompt *prompt;

    MI_CpuFill8(&promptTemplate, 0, sizeof(promptTemplate));
    promptTemplate.bgConfig = bgConfig;
    promptTemplate.bgId = GF_BG_LYR_MAIN_0;
    promptTemplate.tileStart = 0x180;
    promptTemplate.plttSlot = 14;
    promptTemplate.x = 26;
    promptTemplate.y = 10;
    promptTemplate.ignoreTouchFlag = 0;
    promptTemplate.initialCursorPos = 0;
    prompt = YesNoPrompt_Create(HEAP_ID_26);
    YesNoPrompt_InitFromTemplate(prompt, &promptTemplate);
    return prompt;
}

s32 WirelessTradeSelectMon_HandleYesNoPrompt(BgConfig *bgConfig, void *unused, u32 *state, YesNoPrompt **prompt, u32 dim) {
    s32 result = -1;

    switch (*state) {
    case 0:
        *prompt = WirelessTradeSelectMon_CreateYesNoPrompt(bgConfig);
        if (dim != 0) {
            // The original call passes the flag to the parameterless function.
            ((void (*)(u32))WirelessTradeSelectMon_StartMenuDim)(dim);
        }
        (*state)++;
        break;
    case 1:
        result = YesNoPrompt_HandleInput(*prompt);
        if (result != YESNORESPONSE_WAIT) {
            YesNoPrompt_Destroy(*prompt);
            WirelessTradeSelectMon_EndMenuDim();
            *state = 0;
        }
        break;
    }
    return result;
}

ListMenu *WirelessTradeSelectMon_CreateRosterListMenu(ListMenuItem *items, u32 count, Window *window, BgConfig *bgConfig) {
    ListMenuTemplate listTemplate;

    AddWindowParameterized(bgConfig, window, GF_BG_LYR_MAIN_0, 19, 1, 12, 10, 13, 845);
    DrawFrameAndWindow1(window, FALSE, 1015, 11);
    listTemplate = sWirelessTradeSelectMonRosterListTemplate;
    listTemplate.totalItems = count + 1;
    listTemplate.maxShowed = 5;
    listTemplate.moveCursorFunc = WirelessTradeSelectMon_RosterListCursorCB;
    listTemplate.items = items;
    listTemplate.window = window;
    return ListMenuInit(&listTemplate, 0, 0, HEAP_ID_26);
}

void WirelessTradeSelectMon_RosterListCursorCB(ListMenu *list, s32 index, u8 onInit) {
    if (!onInit) {
        PlaySE(SEQ_SE_DP_SELECT);
    }
}
