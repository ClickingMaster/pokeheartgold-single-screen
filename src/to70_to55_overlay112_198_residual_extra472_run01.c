#pragma force_active on
#include "global.h"
typedef struct {u32 pad;u32 heap;u8 rest[0x13c];u32 flags;} W;
extern W *OverlayManager_GetData(void *);
extern void ov112_021F2DE4(void *),ov112_021F1838(W *),ov112_021F1A40(W *),ov112_021F1B94(W *),ov112_021F178C(W *),ov112_021F2B80(W *);
extern int ov112_021F1DC0(W *),IsPaletteFadeFinished(void);
extern void BeginNormalPaletteFade(int,int,int,int,int,int,int);
int ov112_021F2EF4(void *,int *);
int ov112_021F2EF4(void *mgr,int *state){W *w=OverlayManager_GetData(mgr);switch(*state){case 0:ov112_021F2DE4(mgr);++*state;break;case 1:ov112_021F1838(w);ov112_021F1A40(w);++*state;break;case 2:if(ov112_021F1DC0(w)){ov112_021F1B94(w);ov112_021F178C(w);w->flags=(w->flags&~255)|255;ov112_021F2B80(w);++*state;}break;case 3:BeginNormalPaletteFade(0,1,1,0x7fff,6,1,w->heap);++*state;break;case 4:if(IsPaletteFadeFinished())return 1;break;default:GF_AssertFail();break;}return 0;}
