#ifndef SOL37_OV70_R44_PRIVATE_H
#define SOL37_OV70_R44_PRIVATE_H

#include "global.h"

typedef u8 Ov70Work;

#define PTR(w, o) (*(void **)((w) + (o)))
#define U32(w, o) (*(u32 *)((w) + (o)))
#define S32(w, o) (*(s32 *)((w) + (o)))
#define S16(w, o) (*(s16 *)((w) + (o)))

extern u8 gSystem[];
extern const u32 ov70_0224600C[];

int WifiConnection_ClassifyErrorCode(int, int);
int ov00_021ECDC8(void);
int ov00_021EC11C(int *, int *);
void ov00_021EC210(void);
int ov00_021EC8D8(void);
void ov00_021FA0D8(void);
void ov00_021ED9B4(void);
void sub_020399EC(void);
void GtsServer_RequestInfo(Ov70Work *);
BOOL GtsServer_IsRequestDone(void);
int GtsServer_GetResult(void);
void sub_0203189C(void *, void *);
void GtsServer_RequestSetProfile(void *, void *);
u32 FriendRoster_GetUserData(void *);
u32 Save_SysInfo_GetDwcProfileId(void *);
void Save_SysInfo_SetDwcProfileId(void *, u32);
u64 DWC_CreateFriendKey(u32);
void GtsServer_Init(u32, u32, u32);
void ov70_022451A8(Ov70Work *, int, int);
void EraseFrameAndWindow1(void *, int);
void WirelessStatusIcon_Destroy(void);
void Gts_DeleteWaitingIcon(Ov70Work *);
void BeginNormalPaletteFade(int, int, int, int, int, int, int);
int Gts_HandleYesNoPrompt(Ov70Work *);
void YesNoPrompt_Destroy(void *);
void Gts_SetNextScreen(Ov70Work *, int, int);
BOOL TextPrinterCheckActive(u8);
void sub_0203946C(void);
void *Gts_CreateYesNoPrompt(void *, int, int, int, int);
void *NewString_ReadMsgData(void *, int);
void StringExpandPlaceholders(void *, void *, void *);
void String_Delete(void *);
void FillWindowPixelBuffer(void *, int);
void DrawFrameAndWindow2(void *, int, int, int);
u8 AddTextPrinterParameterized(void *, int, void *, int, int, int, void *);
int FontID_String_GetWidth(int, void *, int);
u8 AddTextPrinterParameterizedWithColor(void *, int, void *, int, int, int, int, void *);

int ov70_02244CA8(Ov70Work *);
int ov70_02244954(Ov70Work *);
int ov70_02244A04(Ov70Work *);
int ov70_02244A4C(Ov70Work *);
int ov70_02244A68(Ov70Work *);
int ov70_02244B20(Ov70Work *);
int ov70_02244B54(Ov70Work *);
int ov70_02244CCC(Ov70Work *);
int ov70_02244D08(Ov70Work *);
int ov70_02244D44(Ov70Work *);
int ov70_02244D80(Ov70Work *);
int ov70_02244E44(Ov70Work *);
void Gts_SetState(Ov70Work *, int, int);
int ov70_02244F14(Ov70Work *);
int ov70_02244F34(Ov70Work *);
int ov70_02244F68(Ov70Work *);
void ov70_02244FA4(Ov70Work *, void *, int, int, int);
int ov70_02245018(void *, void *, int, int, int, int);
void ov70_02245050(void *, void *, int, int, int, int);
void ov70_02245084(void *, void *, int, int, int, int);
void ov70_022450B8(void *, void *, int, int, int, int);
void ov70_022450EC(void *, void *, int);

#endif
