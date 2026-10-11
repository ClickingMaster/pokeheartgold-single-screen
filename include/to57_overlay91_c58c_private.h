#ifndef TO57_NEW29_PRIVATE_H
#define TO57_NEW29_PRIVATE_H
#include "global.h"
typedef struct OverlayManager OverlayManager;
typedef struct Ov91Tail3 {
    u8 a, b, c;
} Ov91Tail3;
typedef struct Ov91Tail4 {
    u8 first;
    Ov91Tail3 rest;
} Ov91Tail4;
typedef struct Ov91Work {
    void *unk00;
    void *unk04;
    u8 unk08[8];
    u8 unk10;
    u8 pad11[7];
    u8 unk18[0x18];
    void *unk30;
    void *unk34;
    u32 unk38;
    u32 unk3C;
    u32 unk40;
    u16 unk44;
    u16 unk46;
    u8 unk48[0x28];
    u32 unk70;
    u32 unk74;
    u32 unk78;
    u32 unk7C;
    u32 unk80;
    Ov91Tail4 unk84;
} Ov91Work;
typedef struct Ov91Block16 {
    u32 w[4];
} Ov91Block16;
extern void *OverlayManager_GetData(OverlayManager *);
extern void *OverlayManager_GetArgs(OverlayManager *);
extern int PlazaGameRoster_CheckConnectionLost(void *);
extern BOOL IsPaletteFadeFinished(void);
extern void sub_0200FB70(void);
extern void sub_0200FC20(int);
extern int PlazaGameRoster_ProcessConnectionLost(void *);
extern void ov91_0225CC4C(Ov91Work *, void *);
extern void GF_AssertFail(void);
extern void *PlazaGameEntry_NewSwalotPlop(void *, u32);
extern BOOL PlazaGameEntry_IsDone(void *);
extern u32 PlazaGameEntry_GetSelection(void *);
extern void PlazaGameEntry_Free(void *);
extern struct {
    u8 pad[0x44];
    u32 unk44;
} gSystem;
extern void ov91_0225CB58(void);
extern void Main_SetVBlankIntrCB(void (*)(void), void *);
extern void HBlankInterruptDisable(void);
extern void GF_CreateVramTransferManager(u32, u32);
extern void ov91_02261B2C(Ov91Work *);
extern u32 sub_020347A0(void);
extern u16 Comm_GetLocalNetId(void);
extern u16 PlazaGameRoster_GetIndexOfNetId(void *, u16);
extern void *ov91_0225CCC4(u32, u32, u32, void *);
extern void *ov91_0225CDF4(u32, u32, u32, u16, void *);
extern void WirelessStatusIcon_Create(void);
extern void WifiConnection_StartVoiceChat(u32);
extern void ov91_0225D3C4(void *, void *);
extern void sub_02037AC0(u32);
extern BOOL sub_02037B38(u32);
extern void BeginNormalPaletteFade(u32, u32, u32, u16, u32, u32, u32);
extern BOOL ov91_0225CEB4(void *, u32);
extern BOOL Comm_SendCommand(u32, void *, u32);
extern u32 ov91_0225D37C(void *);
extern u32 ov91_0225CCF4(void *);
extern BOOL ov91_0225CD5C(void *);
extern u32 ov91_0225CD6C(void *);
extern BOOL ov91_0225CCA8(Ov91Work *, u32, void *, u32);
extern void ov91_0225CD64(void *);
extern void ov91_0225D1DC(void *);
extern void ov91_0225CB64(Ov91Work *);
extern void ov91_0225CB98(Ov91Work *);
extern BOOL ov91_0225D078(void *, u32);
extern BOOL ov91_0225CDAC(void *);
extern void ov91_0225CDC4(void *, Ov91Block16 *);
extern void ov91_0225CCEC(void *);
extern void ov91_0225CE80(void *);
extern void GF_DestroyVramTransferManager(void);
extern void PlazaGame_CalcRanks(void *, u8);
extern void *PlazaGameResults_NewSwalotPlop(void *, void *, u32);
extern BOOL PlazaGameResults_IsDone(void *);
extern u32 PlazaGameResults_GetContinue(void *);
extern void PlazaGameResults_Free(void *);
int ov91_0225C58C(OverlayManager *, int *);
#endif
