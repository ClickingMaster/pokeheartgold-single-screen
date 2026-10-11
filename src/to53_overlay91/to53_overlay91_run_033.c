#include "private_w18.h"

/* ---- prototypes (source order) ---- */
void ov91_0225CA64(Ov91Work *work);
void ov91_0225CA6C(Ov91Work *work);
void ov91_0225CB50(Ov91Work *work);
void ov91_0225CC38(Ov91Work *work);
void ov91_0225CCEC(void *ptr);
u8 ov91_0225CD5C(Ov91Work *work);
void ov91_0225CD64(Ov91Work *work);
u8 ov91_0225CD6C(Ov91Work *work);
void ov91_0225CDC4(Ov91Work *work, Ov91Blk16 *dst);
u32 ov91_0225D2E8(Ov91Sub *p);
u32 ov91_0225D37C(Ov91Sub *p);
void ov91_0225DBD0(u8 *base);
void ov91_0225DBE0(void);
void ov91_0225DD20(u8 *p, u8 v);
void ov91_0225E1E0(u16 *p, u16 v);
BOOL ov91_0225DBAC(Ov91Sub *p);

/* callees we have not written: prototypes only, so the caller still emits the
 * ordinary bl + relocation the original has. */
void ov91_0225D750(void *p);
void ov91_0225D84C(void *p);
void ov91_0225D768(void *p);

/* batch 2 prototypes */
void ov91_0225D2EC(void *p);
void ov91_0225D380(void *p);
void ov91_0225D2D0(void *p);
void ov91_0225F3A0(void *p);
void ov91_022601C8(void *p);
void ov91_02260298(void *p, int n);
void ov91_0225F05C(void *a, void *b, int c);
void ov91_0225F010(void);
void ov91_0225FDC8(void *p);
BOOL ov91_0225CDD4_b(void);
void ov91_0225CD74(Ov91Cnt *a, const Ov91Ev *b);
BOOL ov91_0225CDAC(Ov91Work *w);
BOOL ov91_0225DB44(void *unused, void *b);
BOOL ov91_0225DC24(void *a);
void ov91_0225E3F4(void *unused, void *p, u16 b, u32 c);
BOOL ov91_0225E990(Ov91Cam *p);
BOOL ov91_0225E9AC(Ov91Cam *p);
void ov91_0225E9E4(void *a, void *b);
void ov91_0225EDB0(Ov91Ring *r);
u32 ov91_0225EE88(Ov91Ring *r);
void ov91_0225EEBC(void *p, void *a, void *b);
void ov91_0225F008(void);
void ov91_0225F388(Ov91F3D4 *p);
BOOL ov91_0225CCA8(Ov91Work *w, void *a, void *b, void *c);
extern BOOL ov90_02258CE0(void *p);
extern BOOL Comm_SendCommand(void *a, void *b, void *c);
void ov91_0225E6B0(void *p);
BOOL ov91_0225EE48(Ov91Ring *r, Ov91Ev *out);
BOOL ov91_0225EE64(Ov91Ring *r, Ov91Ev *out);
void ov91_0225EE9C(void *p);
void ov91_0225F23C(void *p);
void ov91_0225F414(void);
void ov91_0225FD0C(void *p);
void ov91_022601AC(void *p);
void ov91_0225DB5C(void *p);
void ov91_0225E62C(void *p);
void ov91_0225EA7C(void *a);
void ov91_0225EA98(void *a);
void ov91_0225EC7C(void *a);
void ov91_0225E67C(void *a, void *b);
void ov91_02260D00(void *p);
void ov91_02260378(void *p);
extern void GfGfx_SetBanks(const Ov91Banks *b);
extern const u8 ov91_02261D84[40];
extern const u8 ov91_02261C34[16];
void *ov91_0225CCC4(u32 heap, void *a, void *b, void *c);
void ov91_0225CD84(void *p, u32 v, int bit);
u32 ov91_0225D600(void *p);
BOOL ov91_0225E6D0(void *p);
BOOL ov91_0225E9C0(void *p);
void ov91_0225E7C4(void *a, void *b, int mode);
void ov91_0225E7E8(void *a, void *b, int c, u32 d, int e);
void ov91_0225DB18(void *p, void *b);
extern void *Heap_Alloc(u32 heap, u32 size);
extern void DrawFrameAndWindow1(void *w, int a, int b, int c);
extern void ov90_02258C8C(void *p);
extern const u8 ov91_02261F8C[4];
extern const u8 ov91_02261F90[540];
void ov91_0225CE80(void *p);
void ov91_0225DB7C(void *unused, void *b);
BOOL ov91_0225DBF4(void *p);
void ov91_0225DD24(void *p, s16 ang);
s32 ov91_0225E044(void *p, s32 x);
s32 ov91_0225E070(void *p, s32 x);
void ov91_0225D4D4(int i, int j, int k, u32 *outA, u32 *outB);
void ov91_0225F1E8(void *a, void *b);
void ov91_0225F66C(void *p);
void ov91_02260334(void *p, int n);
extern BOOL sub_02021238(void);
extern void GF_AssertFail(void);
extern void sub_02018198(void *p, u32 v);
extern BOOL sub_0201815C(void *p, u32 v);
extern s32 _s32_div_f(s32 a, s32 b);
void ov91_022601F4(void *p, void *b);
void ov91_02260218(void *p, void *b);
void ov91_0226023C(void *p);
void ov91_0226031C(void *p);
void ov91_02260364(void *p, u16 v);
void ov91_02260370(void *p, u16 v);
extern void sub_020182E0(void *p, u16 a, int b);
extern void sub_020182C4(void *p, u32 a, u32 b, u32 c);
extern void sub_020181EC(void *p);
void ov91_02260830(void *unused, void *p);
void *ov91_02260B48(void *p, void *b);
void ov91_02260CA8(void *p);
void ov91_02260CB4(void *p);
void ov91_02260A2C(void *p);
void ov91_02260884(void *a, void *p, u32 v);
void ov91_02260AF8(void *a, void *b);
extern void sub_020182A0(void *p, int n);
extern void Sprite_Delete2(void *s);
void ov91_02260D80(void *unused, void *p);
void ov91_02261200(void *p, u32 v);
void ov91_0226137C(void *p, u32 v);
void ov91_022613AC(void *p, u8 v);
void ov91_02261890(void *p);
const u8 *ov91_02261B20(void);
void ov91_02261B10(void);
void ov91_022619E8(void *p);
BOOL ov91_0225F3E8(void *p, u32 n);
void ov91_02260400(s32 a, s32 b, s32 *out);
extern const u8 ov91_02262754[96];
extern void GF_SndHandleSetTempoRatio(int h, int r);
extern void PlaySE(int id);
int ov91_02261B28(void);
int ov91_02261B48(void);
void ov91_02261B4C(void *a, void *b, void *c, Ov91Work *d);
void ov91_02261B58(void *a, void *b, void *c, Ov91Work *d);
extern void String_Delete(void *s);
extern void DestroyMsgData(void *s);
extern void MessageFormat_Delete(void *s);
extern void sub_02018124(void *a, u32 b);
extern void EraseFrameAndWindow1(void *w, int n);
extern void ClearWindowTilemapAndScheduleTransfer(void *w);
extern void GfGfx_EngineATogglePlanes(int planes, int on);
extern struct { u8 pad[0x60]; u16 touchX; u16 touchY; u16 touchNew; u16 touchHeld; } gSystem;
extern void Sprite_SetDrawFlag(void *sprite, BOOL flag);
extern void RemoveWindow(void *w);
extern void GF_3DVramMan_Delete(void *p);
extern void Camera_Delete(void *c);
extern void Camera_PushLookAtToNNSGlb(void *c);
void ov91_0225CA90(Ov91Work *work);
void ov91_0225CAE0(Ov91Work *work);
void ov91_0225CB58(Ov91Work *work);
void ov91_0225D368(void *p, const void *src);
void ov91_0225D3B4(void *p);
void ov91_0225D8E0(void **p, int idx);
void ov91_0225DB0C(void *p);
void ov91_0225DBC0(void *p);
void ov91_0225DBE4(void *p);
void ov91_0225E1E4(void *p, u32 v);
u32 ov91_0225E1EC_v(void);
void ov91_0225E400(void *p);
void ov91_0225F3D4(Ov91F3D4 *p);
u32 ov91_0225F3E4(Ov91F3D4 *p);
void ov91_0225F4F8(void *p);
void ov91_0225FDD8(void *c);
void ov91_0225FEFC(void);
extern void *memset(void *d, int c, u32 n);
void *ov91_0225CDF4(u32 heap, void *a, void *b, void *c, void *d);
void ov91_0225DC3C(void *p, void *q);
void ov91_0225F508(void *p, u32 heap);
void ov91_0225F0C0(void *p, void *q, u32 heap);
extern BOOL sub_020210D8(void *p, u32 count, u32 mode);
extern const u8 ov91_022627C0[];
u32 ov91_0225CDD4(Ov91Work *work);
void ov91_0225D67C(void *p);
void ov91_0225D6A0(void *p);
void ov91_0225D694(Ov91Rec40 *p, int idx);

/* referenced by the .rodata pointer table at ov91_02261C58 */
u32 ov91_0225E1EC(void);
u32 ov91_0225E1F4(void);
void ov91_0225E1FC(void);

extern void Heap_Free(void *ptr);
extern void Sprite_Delete(void *sprite);

/* ---- definitions ---- */
/* ---------- batch 2 ---------- */
/* ---------- batch 3 ---------- */
/* ---------- batch 4 ---------- */
/* ---------- batch 5 ---------- */
/* ---------- batch 6 ---------- */
/* ---------- batch 7 ---------- */
/* ---------- batch 8 ---------- */
/* ---------- batch 9 ---------- */
/* ---------- batch 10 ---------- */
/* ---------- batch 11 (trampolines) ---------- */
/* ======== round-2 batch A ======== */
int ov91_02261BD8(void);
int ov91_02261BDC(void);
int ov91_02261BE0(void);
int ov91_02261BE4(void);
void ov91_02261BBC(void *a, void *b, void *c, Ov91Work *d);
void ov91_02261B2C(void *ctx);
void ov91_02260CE0(Ov91Spr *p, u16 x, u16 y);
void ov91_02260CC8(Ov91Spr *p);
extern void Sprite_SetMatrix(void *sprite, const VecFx32 *v);
extern void DoScheduledBgGpuUpdates(void *p);
extern void GF_RunVramTransferTasks(void);
extern void OamManager_ApplyAndResetBuffers(void);
extern void Comm_RegisterCommandTable(const void *table, int count, void *context);

/* ======== round-2 batch B ======== */
BOOL ov91_0225EE64_d(Ov91Ring *r, Ov91Ev *out);

void ov91_0225E1E0(u16 *p, u16 v) {
    p[3] = v;
}


void ov91_0225E1E4(void *p, u32 v) { *(u32 *)((u8 *)p + 0xA4) = v; }


u32 ov91_0225E1EC(void) { return 0xE38; }


u32 ov91_0225E1F4(void) { return 0x3FFF; }
