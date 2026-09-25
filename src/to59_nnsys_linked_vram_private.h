#ifndef TO59_NNSYS_LINKED_VRAM_H
#define TO59_NNSYS_LINKED_VRAM_H
typedef unsigned int u32;
typedef int BOOL;
extern u32 _021D84E4[];
extern u32 _021D84E8[];
extern u32 _021D84EC[];
extern u32 _021D8500[];
extern u32 _021D8504[];
extern u32 (*NNS_GfdDefaultFuncAllocTexVram)(u32, BOOL, u32);
extern int (*NNS_GfdDefaultFuncFreeTexVram)(u32);
extern u32 (*NNS_GfdDefaultFuncAllocPlttVram)(u32, BOOL, u32);
extern int (*NNS_GfdDefaultFuncFreePlttVram)(u32);
extern void NNS_GfdResetLnkTexVramState(void);
extern void NNS_GfdResetLnkPlttVramState(void);
extern u32 NNS_GfdAllocLnkTexVram(u32, BOOL, u32);
extern int NNS_GfdFreeLnkTexVram(u32);
extern u32 NNS_GfdAllocLnkPlttVram(u32, BOOL, u32);
extern int NNS_GfdFreeLnkPlttVram(u32);
u32 NNS_GfdGetLnkTexVramManagerWorkSize(u32);
void NNS_GfdInitLnkTexVramManager(u32, u32, void *, u32, BOOL);
u32 NNS_GfdGetLnkPlttVramManagerWorkSize(u32);
void NNS_GfdInitLnkPlttVramManager(u32, void *, u32, BOOL);
extern u32 NNSi_GfdAllocLnkVram(u32 *, u32 *, u32 *, u32);
extern u32 NNSi_GfdAllocLnkVramAligned(u32 *, u32 *, u32 *, u32, u32);
extern int NNSi_GfdFreeLnkVram(u32 *, u32 *, u32, u32);
extern void NNSi_GfdInitLnkVramMan(u32 *);
extern u32 NNSi_GfdInitLnkVramBlockPool(void *, u32);
extern void NNSi_GfdAddNewFreeBlock(u32 *, u32 *, u32, u32);
extern void NNSi_GfdMergeAllFreeBlocks(u32 *, u32 *);
#endif
