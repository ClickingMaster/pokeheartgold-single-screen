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
void NNSi_SndPlayerInit(void) { int i; struct Player *p; struct SeqList *s; NNS_FndInitList(_021DF978,0x14); NNS_FndInitList(_021DF96C,0x14); for(i=0,p=_021DF984;i<16;i++,p++) { U8(p,0x2c)=0; U8(p,0x3c)=i; NNS_FndAppendListObject(_021DF96C,p); } for(i=0,s=_021DFDC4;i<32;i++,s++) { NNS_FndInitList(s,12); NNS_FndInitList((u8 *)s+12,0); U8(s,0x20)=127; U32(s,0x18)=1; U32(s,0x1c)=0; } }
