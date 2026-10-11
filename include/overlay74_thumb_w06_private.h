#include "global.h"

struct OverlayManager;

#define U8(p, o) (*(u8 *)((u8 *)(p) + (o)))
#define U16(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define U32(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))

extern u8 *ov74_0223D0A4;
extern u8 _0223B368[];
extern u16 sRangerCharTable[][2];
extern u16 gConnectToRanger_PlayerName[];
extern u16 gConnectToRanger_TrainerId[];

extern void ov74_02229BC0(void (*func)(), u16 value);
extern void ov74_02229C60(void);
extern void ov74_02229C80(void);
extern void ov74_02229CB4(void);
extern void ov74_02229C5C(void);
extern void ov74_02229BD0(void);
extern void ov74_02229C44(void);
extern void ov74_02229E28(void *p, int enable);
extern int sub_02034DB8(void);
extern void sub_02037E1C(int);
extern int sub_02037EC8(void);
extern void sub_02034434(void);
extern void sub_02037AC0(int);
extern void SysTask_Destroy(void *task);
extern int sub_02037474(void);
extern void sub_02038114(void *, void *);
extern void ov74_02229B68(void *);
extern void *OverlayManager_GetArgs(void *);
extern void ov74_0222CD7C(void);
extern void sub_02038148(void *, void *);
extern int sub_02037088(int, void *, void *);
extern void *sub_020358D0(int);
extern void *memcpy(void *, const void *, u32);
extern void Heap_FreeExplicit(int, void *);
extern void sub_02037E9C(void);
extern void Comm_RegisterCommandTable(void *, int, void *);
extern void *WaitingIcon_New(void *, int);
extern void WaitingIcon_DeleteWithoutRestore(void *);
extern void *OverlayManager_GetData(void *);
extern void ov74_022359BC(void);
extern void ListMenuItems_Delete(void *);
extern void DestroyListMenu(void *, int, int);
extern void ClearWindowTilemapAndCopyToVram(void *);
extern void RemoveWindow(void *);
extern int WindowIsInUse(void *);
extern void FreeBgTilemapBuffer(void *, int);
extern void Heap_Free(void *);
extern void ov74_0223563C(void);
extern void ov74_02235690(void);
extern void ov74_02235728(int, int, int, int, int, int);
extern void EraseFrameAndWindow1(void *, int);
extern void ClearFrameAndWindow2(void *, int);
extern void *ov74_02235930(int, void *, int, int, int);
extern void Sprite_SetDrawFlag(void *, int);
extern const u16 *PlayerProfile_GetNamePtr(void *);
extern int PlayerProfile_GetTrainerID_VisibleHalf(void *);
extern void *Heap_Alloc(int, u32);
extern void MI_CpuFill8(void *, u8, u32);
extern void *SysTask_CreateOnMainQueue(void (*)(), int);
extern void sub_0203778C(void *);
extern void Comm_GetLocalNetId(void);
extern int Comm_IsNetIdConnected(int);
extern void *sub_02034818(int);

u16 ConnectToRanger_ConvertCharToRanger(u16 value);
void ConnectToRanger_FormatNumber5Digits(u16 *dst, int value);
void ov74_02229C0C(void);
void ov74_02229CB8(void *);
void ConnectToRanger_EncodePlayerData(void *);
void ov74_02229B68(void *);
void ov74_02229BD0(void);
void ov74_02229C44(void);
void ov74_02229C5C(void);
void ov74_02229C60(void);
void ov74_02229C80(void);
void ov74_02229CB4(void);
void ov74_02229CE0(void *, void *, void *);
int ov74_02229D0C(void *, void *);
void ov74_02229D1C(void *, void *);
void ov74_02229D50(u8);
int ov74_02229D6C(void *);
void ov74_02229DA0(u8);
u8 ov74_02229DBC(void);
int ov74_02229DCC(void);
void *ov74_02229DD4(void);
void ov74_02229DE0(int);
void ov74_02229DF8(void);
void ov74_02229E14(void *);
void ov74_02229E28(void *, int);
void ov74_02229E60(void *, int);
void ov74_02229E68(struct OverlayManager *);
void ov74_02229F04(void);
void ov74_02229F60(void *, int);
void ov74_02229F78(void *, int);
void ov74_02229F28(void *, int);
