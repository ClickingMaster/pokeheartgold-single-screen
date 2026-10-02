#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
#define U32(p,o) (*(u32 *)((u8 *)(p)+(o)))
#define U16(p,o) (*(u16 *)((u8 *)(p)+(o)))
#define U8(p,o) (*(u8 *)((u8 *)(p)+(o)))
struct Player { u8 data[0x44]; };
struct SeqList { u8 data[0x24]; };
extern u8 _021DF978[],_021DF96C[];
extern struct Player _021DF984[];
extern struct SeqList _021DFDC4[];
extern void NNS_FndInitList(void *,int);
extern void NNS_FndAppendListObject(void *,void *);
extern void *NNS_FndGetNextListObject(void *,void *);

extern void ForceStopSeq(void *);
extern void *AllocSeqPlayer(int);
extern void InsertPlayerList(void *,void *);
extern void NNSi_SndPlayerInit(void);
extern void *NNSi_SndPlayerAllocSeqPlayer(void **,int,int);
extern const s16 _0210D750[];
extern u32 SND_GetPlayerStatus(void);
extern BOOL SND_IsFinishedCommandTag(u32);
extern void SND_SetPlayerVolume(int,int);
extern void SND_StartPreparedSeq(int);
extern void ShutdownPlayer(void *);
extern void NNSi_SndFaderUpdate(void *);
extern int NNSi_SndFaderGet(void *);
extern BOOL NNSi_SndFaderIsFinished(void *);
extern void NNSi_SndPlayerMain(void);
#define S16(p,o) (*(s16 *)((u8 *)(p)+(o)))
void *NNSi_SndPlayerAllocSeqPlayer(void **handle,int index,int priority) { u8 *list=(u8 *)&_021DFDC4[index]; u8 *p=*handle; if(p) NNS_SndHandleReleaseSeq((NNSSndHandle *)handle); if(U16(list,8)>=U32(list,0x18)) { p=NNS_FndGetNextListObject(list,NULL); if(!p) return NULL; if(priority<U8(p,0x3d)) return NULL; ForceStopSeq(p); } p=AllocSeqPlayer(priority); if(!p) return NULL; InsertPlayerList(list,p); U32(p,0)=(u32)handle; *handle=p; return p; }
