#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 pad[0x18];u8 *script;} Ov07CallbackContext;
extern void PlaySE(u16);extern void StopSE(u16,int);extern int ov07_0221F8C8(Ov07CallbackContext *,s8);extern void SetSeqTrackPan(u16,u16,int);extern void SetAllSEPan(int);
void ov07_0221F098(Ov07CallbackContext *),ov07_0221F0B0(Ov07CallbackContext *),ov07_0221F0CC(Ov07CallbackContext *),ov07_0221F104(Ov07CallbackContext *);
static inline u32 readWord(Ov07CallbackContext *w){u32 a=*(u32 *)w->script;w->script+=4;return a;}
void ov07_0221F098(Ov07CallbackContext *w){u32 a;w->script+=4;a=readWord(w);PlaySE((u16)a);}
void ov07_0221F0B0(Ov07CallbackContext *w){u32 a;w->script+=4;a=readWord(w);StopSE((u16)a,0);}
void ov07_0221F0CC(Ov07CallbackContext *w){u16 a;int b;u32 raw;w->script+=4;raw=*(u32 *)w->script;a=(u16)raw;w->script+=4;b=ov07_0221F8C8(w,(s8)readWord(w));PlaySE(a);SetSeqTrackPan(a,65535,b);}
void ov07_0221F104(Ov07CallbackContext *w){int b;w->script+=4;b=ov07_0221F8C8(w,(s8)readWord(w));SetAllSEPan(b);}
