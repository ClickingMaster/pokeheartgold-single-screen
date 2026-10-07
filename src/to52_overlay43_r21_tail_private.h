#ifndef TO50_OVERLAY43_R21_PRIVATE_H
#define TO50_OVERLAY43_R21_PRIVATE_H
#include "global.h"

extern const s16 ov43_0222EFA0[];
extern const s16 ov43_0222EFA2[];
extern const u32 ov43_0222ED94[];
int sub_0202C090(void *, int, int);
const u16 *sub_0202C254(void *, int);
void *ov42_022272BC(void *, s16 *, int, int);
void *String_New(int, int);
void CopyU16ArrayToString(void *, const u16 *);
void AddTextPrinterParameterizedWithColor(void *, int, void *, int, int, int, u32, void *);
void String_Delete(void *);
void CopyToBgTilemapRect(void *, int, int, int, int, int, const void *, int, int, int, int);
void ScheduleBgTilemapBufferTransfer(void *, int);
void ov43_0222CD3C(void *);

void ov43_0222CB34(s16 *, void *, void *, int);
void ov43_0222CBAC(void *);
void ov43_0222CA50(s16 *, void *, void *, void *, int, void *, int, int);


typedef struct R21Runner {
    void *manager;
    void *namingArgs;
    void *secondaryArgs;
} R21Runner;

typedef struct R21State {
    u32 unk0;
    void *saveData;
    u8 state;
} R21State;

void *ov43_0222CD44(R21Runner *, R21State *, int);
void *Save_PlayerData_GetOptionsAddr(void *);
void *sub_02085400(int, int, const void *, void *, int, int);
void ov43_0222A1DC(void *);
extern const u8 gOverlayTemplate_NamingScreen[];
extern const u8 _02102620[];
void *OverlayManager_New(const void *, void *, int);
int OverlayManager_Run(void *);
void OverlayManager_Delete(void *);
void ov43_0222A318(R21State *, int, int);
void ov43_0222A338(R21State *, int, int);
void NamingScreen_DeleteArgs(void *);
void sub_02085438(void *);
void ov43_0222A154(void *, int);
int ov43_0222CBB4(R21Runner *, void *, R21State *, int, int);

#endif // TO50_OVERLAY43_R21_PRIVATE_H
