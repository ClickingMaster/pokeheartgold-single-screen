#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct { int signedFlag0:1,signedFlag1:1,signedFlag2:1,signedFlag3:1; unsigned int other:28; } Flags;
typedef struct { u8 data[0xe8]; u8 fader[0x28]; Flags flags; int valid,ready; u8 pad11c[0x38]; int volume,otherVolume,lastVolume; u8 pad160[0x14]; } Stream;
extern Stream _021E0FEC[];
extern s16 _0210D750[];
extern void ForceStopStrm__SndArc(Stream*);
extern void NNS_SndStrmStart(void*);
extern void NNSi_SndFaderUpdate(void*);
extern int NNSi_SndFaderGet(void*);
extern int NNSi_SndFaderIsFinished(void*);
extern void NNS_SndStrmSetVolume(void*,int);
static inline int addVolume(int a,int b) { return a+b; }
void NNSi_SndArcStrmMain(void);
void NNSi_SndArcStrmMain(void) {
 int i; for(i=0;i<4;i++) {
  Stream *p=&_021E0FEC[i];
  if(p->flags.signedFlag0) {
   if(!p->valid) ForceStopStrm__SndArc(p);
   else {
    if(p->flags.signedFlag2 && p->ready) {
     NNS_SndStrmStart(p); p->flags.signedFlag1=1;p->flags.signedFlag2=0;
    }
    if(p->flags.signedFlag1) {
     int volume;
     NNSi_SndFaderUpdate(p->fader);
     volume=_0210D750[p->volume];
     { int level=NNSi_SndFaderGet(p->fader)>>8;
     int other=p->otherVolume;
     volume=_0210D750[other]+addVolume(_0210D750[level],volume); }
     if(volume!=p->lastVolume) {NNS_SndStrmSetVolume(p,volume);p->lastVolume=volume;}
     if(p->flags.signedFlag3 && NNSi_SndFaderIsFinished(p->fader)) ForceStopStrm__SndArc(p);
    }
   }
  }
 }
}
