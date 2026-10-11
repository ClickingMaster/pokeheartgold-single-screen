#include "gf_rtc.h"
#define U8(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define U16(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define U32(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))
typedef struct WorkF90 {
    u8 pad000[0x80];
    u32 field80;
    u32 field84;
    u8 pad088[0x3ac];
    u32 field434;
    u32 field438;
    u8 pad43C[0x1198];
    u32 field15D4;
} WorkF90;
typedef struct GiftSlot {
    u16 kind;
    u16 pad2;
    u32 id;
    u8 mon[0x50];
} GiftSlot;
extern void *OverlayManager_GetData(void *);
extern void *OverlayManager_GetArgs(void *);
extern void ov74_0222AA18(void *, void *, int);
extern void ov74_02229F60(void *, int);
extern void ov74_02229D1C(void *, int);
extern void WirelessStatusIcon_Create(void);
extern void ov74_02229E28(void *, int);
extern void ov74_02229D50(int);
extern void ov74_02229F28(void *, int);
extern void sub_02037AC0(int);
extern void *MysteryGiftComm_GetRecvBuffer(int, int, int);
extern void *memcpy(void *, const void *, u32);
extern void ov74_02235F58(void *, void *, int);
extern int GetMonData(void *, int, void *);
extern int ov74_022360B0(void);
extern void PlaySE(int);
extern void *Save_MysteryGift_Get(void *);
extern void SaveMysteryGift_ReceivedFlagSet(void *, int);
extern void SaveMysteryGift_TrySetSpecialCard(void *, void *);
extern void SaveMysteryGift_TryInsertGift(void *, void *, int);
extern void SaveMysteryGift_TryInsertCard(void *, void *);
extern void ov74_022360A0(void *);
extern void AddWindowParameterized(void *, void *, int, int, int, int, int, int, int);
extern void DrawFrameAndWindow1(void *, int, int, int);
extern void ov74_0222A89C(void *, void *, int, void *, int);
extern void ov74_0223C260(void);
void ov74_0222A130(void *);

int ov74_02229F90(void *);
int ov74_02229FF4(void *);
void *ov74_0222A078(void *);
int ov74_0222A0E4(GiftSlot *);
void ov74_0222A174(void *);
void ov74_0222A240(void *, int);
