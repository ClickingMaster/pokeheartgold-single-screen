#ifndef CANDIDATE_PRIVATE_H
#define CANDIDATE_PRIVATE_H
#include "global.h"
#pragma require_prototypes off
#define PTR(p, off)  (*(void **)((u8 *)(p) + (off)))
#define WORD(p, off) (*(u32 *)((u8 *)(p) + (off)))
void VsRecorder_RebuildMenuRowsParent(void *);
void BgClearTilemapBufferAndCommit(void *, int);
void ov40_0222FB90(void *, int);
void *ov40_0222FBB4(void *);
int VsRecorder_StepFade(void *, int);
void VsRecorder_ClearPaletteBlend(void *);
void VsRecorder_ResetFade(void *);
void PaletteData_BlendPalettes(void *, int, int, int, u16);
void VsRecorder_SetScreenIfDone(void *, int, int, void *);
void VsRecorder_SetSubState(void *, int);
void Heap_Free(void *);
void SetBgPriority(int, int);
void GfGfxLoader_LoadCharDataFromOpenNarc(void *, int, void *, int, int, int, int, int);
void ov40_02230964(void *, int);
void VsRecorder_FreeMenuRows(void *);
void BattleRecord_GetBattlerAndPartyCount(int, int *, int *);
void *String_New(int, int);
void *Heap_Alloc(int, int);
void ov40_02244A84(void *, int);
void CopyStringToU16Array(void *, u16 *, int);
void String_SetEmpty(void *);
void CopyU16ArrayToString(void *, u16 *);
int FontID_String_AllCharsValid(int, void *, void *);
int Party_GetCount(void *);
void *Party_GetMonByIndex(void *, int);
u32 GetMonData(void *, int, void *);
void SetMonData(void *, int, void *);
void String_Delete(void *);
#endif
