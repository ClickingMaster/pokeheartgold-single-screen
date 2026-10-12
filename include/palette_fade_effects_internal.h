#ifndef POKEHEARTGOLD_PALETTE_FADE_EFFECTS_INTERNAL_H
#define POKEHEARTGOLD_PALETTE_FADE_EFFECTS_INTERNAL_H
#include "global.h"

#include "sys_task_api.h"

typedef struct FadeWork {
    u8 padding00[0xC];
    u32 state;
    u8 padding10[0x14];
    u16 color;
    u16 padding26;
    u32 topScreen;
    u32 bottomScreen;
} FadeWork;

typedef struct IntVector4 {
    int values[4];
} IntVector4;

typedef struct Interpolation {
    int value;
    int start;
    int delta;
} Interpolation;

typedef struct AllocationList {
    void *allocation;
    int count;
} AllocationList;

typedef struct PointerAt0C {
    u8 padding00[0xC];
    void *allocation;
} PointerAt0C;

typedef struct Fade16Param {
    const void *first;
    const void *second;
    u16 value08;
    u16 value0A;
    u8 value0C;
    u8 value0D;
    u16 value0E;
} Fade16Param;

typedef struct ShortVector4 {
    u16 values[4];
} ShortVector4;

typedef struct VectorListWork {
    u8 padding00[0xC];
    u8 *vectors;
    int count;
} VectorListWork;

typedef struct RepeatingFadeA {
    u8 padding00[8];
    int screen;
    int current;
    u8 padding10[8];
    int step;
    int repeats;
    int delay;
    int counter;
    int heapId;
    u8 padding2C[8];
    void *blank;
} RepeatingFadeA;
typedef struct RepeatingFadeB {
    u8 padding00[8];
    int screen;
    int current;
    int step;
    int repeats;
    int delay;
    int counter;
    u8 padding20[8];
    void *blank;
    int heapId;
} RepeatingFadeB;
typedef struct RepeatingFadeC {
    u8 padding00[8];
    int screen;
    u8 padding0C[4];
    int current;
    int step;
    int repeats;
    int delay;
    int counter;
    u8 padding24[8];
    void *blank;
    int heapId;
} RepeatingFadeC;

extern void sub_0200FCDC(u16 color);
extern void BrightnessTransition_Start(FadeWork *work);
extern BOOL BrightnessTransition_Run(FadeWork *work);
extern void RectWipe_Begin(FadeWork *work, const void *param);
extern BOOL RectWipe_Update(FadeWork *work);
extern void DoubleRectWipe_Begin(FadeWork *work, const void *a, const void *b);
extern BOOL DoubleRectWipe_Update(FadeWork *work);
extern void CircleWipe_Begin(FadeWork *work, const void *param);
extern BOOL CircleWipe_Update(FadeWork *work);
extern void WedgeWipe_Begin(FadeWork *work, const void *param);
extern BOOL WedgeWipe_Update(FadeWork *work);
extern void SlopeWipe_Begin(FadeWork *work, const void *param);
extern BOOL SlopeWipe_Update(FadeWork *work);
extern void ClockWipe_Begin(FadeWork *work, const void *param);
extern BOOL ClockWipe_Update(FadeWork *work);
extern void LineShutterWipe_Begin(FadeWork *work, const void *param);
extern BOOL LineShutterWipe_Update(FadeWork *work);
extern void RectShutterWipe_Begin(FadeWork *work, const void *param);
extern BOOL RectShutterWipe_Update(FadeWork *work);
extern void MultiRectWipe_Begin(FadeWork *work, const Fade16Param *param);
extern BOOL MultiRectWipe_Update(FadeWork *work);
extern void SectorWipe_Begin(FadeWork *work, const ShortVector4 *param);
extern BOOL SectorWipe_Update(FadeWork *work);
extern void MirroredSectorWipe_Begin(FadeWork *work, const ShortVector4 *param);
extern BOOL MirroredSectorWipe_Update(FadeWork *work);
extern void Heap_Free(void *ptr);
extern void GF_AssertFail(void);
extern const short FX_SinCosTable_[];
extern void ScreenWindow_SetVisible(int a, int b);
extern void ScreenWindow_QueueSetVisible(void *work, int value);
extern void sub_0200FF88(void *a0, u32 a1, u32 a2, u32 a3, u32 heapId);
extern void sub_0200FFB4(void *a0, int index, u32 heapId);
extern void sub_02011130(void);
extern void CircleWipe_BuildScanlines(RepeatingFadeA *work);
extern void WedgeWipe_BuildScanlines(RepeatingFadeB *work);
extern void SlopeWipe_BuildScanlines(RepeatingFadeC *work);

extern const u8 _020F5DA0[];
extern const u8 _020F5DA8[];
extern const u8 _020F5DC8[];
extern const u8 _020F5DD0[];
extern const u8 _020F5DD8[];
extern const u8 _020F5DE4[];
extern const u8 _020F5DFC[];
extern const u8 _020F5E08[];
extern const u8 _020F5E14[];
extern const u8 _020F5E20[];
extern const u8 _020F5E2C[];
extern const u8 _020F5E38[];
extern const u8 _020F5E44[];
extern const u8 _020F5E50[];
extern const u8 _020F5E5C[];
extern const u8 _020F5E68[];
extern const u8 _020F5E74[];
extern const u8 _020F5E80[];
extern const u8 _020F5E8C[];
extern const u8 _020F5E98[];
extern const u8 _020F5EA4[];
extern const u8 _020F5EB0[];
extern const u8 _020F5EC8[];
extern const u8 _020F5ED4[];
extern const u8 _020F5EE0[];
extern const u8 _020F5EEC[];
extern const u8 _020F5EFC[];
extern const u8 _020F5F0C[];
extern const u8 _020F5F1C[];
extern const u8 _020F5D58[];
extern const u8 _020F5D5C[];
extern const u8 _020F5D60[];
extern const u8 _020F5D64[];
extern const u8 _020F5D68[];
extern const u8 _020F5D80[];
extern const u8 _020F5D90[];
extern const u8 _020F5D98[];
extern const u8 _020F5DB0[];
extern const u8 _020F5DB8[];
extern const u8 _020F5DF0[];
extern const u8 _020F5EBC[];
extern u8 _0210F64C[];

BOOL FadeFunc_00(FadeWork *work);
BOOL FadeFunc_01(FadeWork *work);
BOOL FadeFunc_02(FadeWork *work);
BOOL FadeFunc_03(FadeWork *work);
BOOL FadeFunc_04(FadeWork *work);
BOOL FadeFunc_05(FadeWork *work);
BOOL FadeFunc_06(FadeWork *work);
BOOL FadeFunc_07(FadeWork *work);
BOOL FadeFunc_08(FadeWork *work);
BOOL FadeFunc_09(FadeWork *work);
BOOL FadeFunc_10(FadeWork *work);
BOOL FadeFunc_11(FadeWork *work);
BOOL FadeFunc_12(FadeWork *work);
BOOL FadeFunc_13(FadeWork *work);
BOOL FadeFunc_14(FadeWork *work);
BOOL FadeFunc_15(FadeWork *work);
BOOL FadeFunc_16(FadeWork *work);
BOOL FadeFunc_17(FadeWork *work);
BOOL FadeFunc_18(FadeWork *work);
BOOL FadeFunc_19(FadeWork *work);
BOOL FadeFunc_20(FadeWork *work);
BOOL FadeFunc_21(FadeWork *work);
BOOL FadeFunc_22(FadeWork *work);
BOOL FadeFunc_23(FadeWork *work);
BOOL FadeFunc_24(FadeWork *work);
BOOL FadeFunc_25(FadeWork *work);
BOOL FadeFunc_26(FadeWork *work);
BOOL FadeFunc_27(FadeWork *work);
BOOL FadeFunc_28(FadeWork *work);
BOOL FadeFunc_29(FadeWork *work);
BOOL FadeFunc_30(FadeWork *work);
BOOL FadeFunc_31(FadeWork *work);
BOOL FadeFunc_32(FadeWork *work);
BOOL FadeFunc_33(FadeWork *work);
BOOL FadeFunc_34(FadeWork *work);
BOOL FadeFunc_35(FadeWork *work);
BOOL FadeFunc_36(FadeWork *work);
BOOL FadeFunc_37(FadeWork *work);
BOOL FadeFunc_38(FadeWork *work);
BOOL FadeFunc_39(FadeWork *work);
BOOL FadeFunc_40(FadeWork *work);
BOOL FadeFunc_41(FadeWork *work);
BOOL FadeFunc_42(FadeWork *work);
int sub_020109BC(int angle);
int sub_020109D8(int angle, int value);
int sub_02010A54(int angle, int value);
int sub_02010A6C(int start, int end, int divisor);
int sub_02010A7C(int value, int increment);
void sub_02010A8C(IntVector4 *dst, const IntVector4 *src);
void sub_02010EC8(AllocationList *list);
void sub_02010ED0(AllocationList *list);
void *sub_02010EE0(AllocationList *list, int index);
void MultiRectWipe_FreeRects(PointerAt0C *work);
void SectorWipe_Finish(void *unused);
void SectorWipe_Interpolate(Interpolation *interp, int numerator, int denominator);
void MirroredSectorWipe_Finish(void *unused);
void MirroredSectorWipe_Interpolate(Interpolation *interp, int numerator, int denominator);
void LineShutterWipe_Finish(void *unused);
void sub_02010F00(SysTask *task, AllocationList *list);
void sub_02011068(void *work, int a, int b, int useHardware);
SysTask *sub_020110C4(void *work);
void sub_020110DC(void *a0, u8 *work, u32 heapId);
void sub_020110F4(void *a0, u8 *work, u32 heapId);
void sub_02011104(SysTask *task, u8 *work);
void MultiRectWipe_Advance(VectorListWork *work);
BOOL CircleWipe_Step(RepeatingFadeA *work);
BOOL WedgeWipe_Step(RepeatingFadeB *work);
BOOL SlopeWipe_Step(RepeatingFadeC *work);

#include <string.h>

BOOL FadeFunc_00(FadeWork *work);
BOOL FadeFunc_03(FadeWork *work);
BOOL FadeFunc_04(FadeWork *work);
BOOL FadeFunc_05(FadeWork *work);
BOOL FadeFunc_06(FadeWork *work);
BOOL FadeFunc_07(FadeWork *work);
BOOL FadeFunc_08(FadeWork *work);
BOOL FadeFunc_09(FadeWork *work);
BOOL FadeFunc_10(FadeWork *work);
BOOL FadeFunc_11(FadeWork *work);
BOOL FadeFunc_12(FadeWork *work);
BOOL FadeFunc_13(FadeWork *work);
BOOL FadeFunc_14(FadeWork *work);
BOOL FadeFunc_15(FadeWork *work);
BOOL FadeFunc_16(FadeWork *work);
BOOL FadeFunc_17(FadeWork *work);
BOOL FadeFunc_18(FadeWork *work);
BOOL FadeFunc_19(FadeWork *work);
BOOL FadeFunc_20(FadeWork *work);
BOOL FadeFunc_21(FadeWork *work);
BOOL FadeFunc_22(FadeWork *work);
BOOL FadeFunc_23(FadeWork *work);
BOOL FadeFunc_24(FadeWork *work);
BOOL FadeFunc_25(FadeWork *work);
BOOL FadeFunc_26(FadeWork *work);
BOOL FadeFunc_27(FadeWork *work);
BOOL FadeFunc_28(FadeWork *work);
BOOL FadeFunc_29(FadeWork *work);
BOOL FadeFunc_30(FadeWork *work);
BOOL FadeFunc_31(FadeWork *work);
BOOL FadeFunc_32(FadeWork *work);
BOOL FadeFunc_33(FadeWork *work);
BOOL FadeFunc_34(FadeWork *work);
BOOL FadeFunc_35(FadeWork *work);
BOOL FadeFunc_36(FadeWork *work);
BOOL FadeFunc_37(FadeWork *work);
BOOL FadeFunc_38(FadeWork *work);
BOOL FadeFunc_39(FadeWork *work);
BOOL FadeFunc_40(FadeWork *work);
BOOL FadeFunc_41(FadeWork *work);
BOOL FadeFunc_42(FadeWork *work);
int sub_020109BC(int angle);
int sub_02010A54(int angle, int value);
int sub_02010A6C(int start, int end, int divisor);
int sub_02010A7C(int value, int increment);
void sub_02010A8C(IntVector4 *dst, const IntVector4 *src);
void sub_02010EC8(AllocationList *list);
void sub_02010ED0(AllocationList *list);
void *sub_02010EE0(AllocationList *list, int index);
void MultiRectWipe_FreeRects(PointerAt0C *work);
void SectorWipe_Finish(void *unused);
void MirroredSectorWipe_Finish(void *unused);
void LineShutterWipe_Finish(void *unused);
void sub_02010F00(SysTask *task, AllocationList *list);
void sub_02011068(void *work, int a, int b, int useHardware);
SysTask *sub_020110C4(void *work);
void sub_020110DC(void *a0, u8 *work, u32 heapId);
void sub_020110F4(void *a0, u8 *work, u32 heapId);
void sub_02011104(SysTask *task, u8 *work);

#endif
