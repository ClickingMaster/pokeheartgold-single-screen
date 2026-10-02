#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct SndPlayer SndPlayer;
typedef struct SndHeap {u32 node[2]; void *heap; SndPlayer *owner;u32 index;} SndHeap;
struct SndPlayer {void **handle;void *list;SndHeap *heap;u8 p0c[16];u8 fader[16];u8 status,x2d,x2e,x2f;u8 p30[4];u16 x34;u8 p36[6];u8 id,priority;u16 x3e;u8 volume,x41;};
extern u8 _021DFDC4[],_021DF978[],_021DF96C[],_021E0248[];
extern int _021E0244;
void *NNS_FndGetNextListObject(void *,void *);
void NNS_FndRemoveListObject(void *,void *);
void NNS_FndInsertListObject(void *,void *,void *);
void NNS_FndAppendListObject(void *,void *);
void NNS_FndInitList(void *,int);
void NNS_SndHeapClear(void *);
void NNS_SndHeapDestroy(void *);
void NNSi_SndFaderInit(void *);
void NNSi_SndFaderSet(void *,int,int);
int NNS_SndLockChannel(u32);
void SND_SetPlayerVolume(u32,int);
void SND_StopSeq(u32);
void BeginSleep__Strm(void *);
void EndSleep__Strm(void *);
void *NNSi_SndPlayerAllocHeap(int,SndPlayer *);
void InitPlayer(SndPlayer *);
void InsertPlayerList(void *,SndPlayer *);
void InsertPrioList(SndPlayer *);
void ForceStopSeq(SndPlayer *);
SndPlayer *AllocSeqPlayer(int);
void ShutdownPlayer(SndPlayer *);
void PlayerHeapDisposeCallback(SndHeap *);
void SetPlayerPriority(SndPlayer *,int);
void NNS_SndStrmInit(u32 *);
int NNS_SndStrmAllocChannel(u8 *,int,const volatile u8 *);
void *NNSi_SndPlayerAllocHeap(int index,SndPlayer *owner) {
 void *list=_021DFDC4+index*36+12;
 SndHeap *h=NNS_FndGetNextListObject(list,0);
 if(!h)return 0;
 NNS_FndRemoveListObject(list,h);
 h->owner=owner;owner->heap=h;
 NNS_SndHeapClear(h->heap);return h->heap;
}
void InitPlayer(SndPlayer *p){p->x2e=0;p->x2d=0;p->x2f=0;p->x34=0;p->x3e=0;p->volume=127;p->x41=127;NNSi_SndFaderInit(p->fader);NNSi_SndFaderSet(p->fader,0x7f00,1);}
void InsertPlayerList(void *list,SndPlayer *p){SndPlayer *it=NNS_FndGetNextListObject(list,0);while(it){if(p->priority<it->priority)break;it=NNS_FndGetNextListObject(list,it);}NNS_FndInsertListObject(list,it,p);p->list=list;}
void InsertPrioList(SndPlayer *p){SndPlayer *it=NNS_FndGetNextListObject(_021DF978,0);while(it){if(p->priority<it->priority)break;it=NNS_FndGetNextListObject(_021DF978,it);}NNS_FndInsertListObject(_021DF978,it,p);}
void ForceStopSeq(SndPlayer *p){if(p->status==2)SND_SetPlayerVolume(p->id,-723);SND_StopSeq(p->id);ShutdownPlayer(p);}
SndPlayer *AllocSeqPlayer(int prio){SndPlayer *p=NNS_FndGetNextListObject(_021DF96C,0);if(!p){p=NNS_FndGetNextListObject(_021DF978,0);if(prio<p->priority)return 0;ForceStopSeq(p);}NNS_FndRemoveListObject(_021DF96C,p);p->priority=prio;InsertPrioList(p);return p;}
void ShutdownPlayer(SndPlayer *p){void *list;if(p->handle){*p->handle=0;p->handle=0;}list=p->list;NNS_FndRemoveListObject(list,p);p->list=0;if(p->heap){NNS_FndAppendListObject((u8 *)list+12,p->heap);p->heap->owner=0;p->heap=0;}NNS_FndRemoveListObject(_021DF978,p);NNS_FndAppendListObject(_021DF96C,p);p->status=0;}
void PlayerHeapDisposeCallback(SndHeap *h){if(!h->heap)return;NNS_SndHeapDestroy(h->heap);if(h->owner)h->owner->heap=0;else NNS_FndRemoveListObject(_021DFDC4+h->index*36+12,h);}
void SetPlayerPriority(SndPlayer *p,int prio){void *list=p->list;if(list){NNS_FndRemoveListObject(list,p);p->list=0;}NNS_FndRemoveListObject(_021DF978,p);p->priority=prio;if(list)InsertPlayerList(list,p);InsertPrioList(p);}
void NNS_SndStrmInit(u32 *p){if(!_021E0244){NNS_FndInitList(_021E0248,0);_021E0244=1;}p[2]=(u32)BeginSleep__Strm;p[3]=(u32)p;p[5]=(u32)EndSleep__Strm;p[6]=(u32)p;p[17]=0;p[18]=0;p[9]&=~1;p[9]&=~2;}
int NNS_SndStrmAllocChannel(u8 *p,int n,const volatile u8 *channels){u32 bits=0;int i;for(i=0;i<n;i++){p[76+i]=channels[i];bits|=1<<channels[i];}if(!NNS_SndLockChannel(bits))return 0;*(int *)(p+72)=n;*(u32 *)(p+68)=bits;return 1;}
