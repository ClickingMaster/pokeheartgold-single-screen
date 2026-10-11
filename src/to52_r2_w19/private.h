#ifndef TO52_SOL_R2_W19_PRIVATE_H
#define TO52_SOL_R2_W19_PRIVATE_H
#include "global.h"
#pragma require_prototypes off
#define U32(p,o) (*(u32 *)((u8 *)(p)+(o)))
#define S32(p,o) (*(s32 *)((u8 *)(p)+(o)))
#define PTR(p,o) (*(void **)((u8 *)(p)+(o)))
extern u8 gSystem[];
void DrawFrameAndWindow1(void *, int, int, int);
void ov73_021EA0F0(void *, void *, int, int);
void ov73_021EA134(void *, int, int);
void CopyWindowToVram(void *);
void PlaySE(int);
void ov00_021ECB94(void *, void *);
int BattleRoomDownload_IsRoomMarked(void *, u8, u8, void *);
void BufferIntegerAsString(void *, int, int, int, int, int);
void ov73_021E9F34(void *, void *, int, int, int);
void ov73_021E83EC(void *, int, int);
int ov73_021E9224(void *);
int ov73_021E926C(void *);
int ov73_021E9378(void *);
#endif
