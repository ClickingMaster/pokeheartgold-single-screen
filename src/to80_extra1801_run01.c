#pragma force_active on
#include "global.h"
#pragma force_active on
#define P(p,o) (*(void **)((u8*)(p)+(o)))
#define U(p,o) (*(u32 *)((u8*)(p)+(o)))
#define H(p,o) (*(u16 *)((u8*)(p)+(o)))
#define B(p,o) (*((u8*)(p)+(o)))
extern int IsPaletteFadeFinished(void),Comm_GetLocalNetId(void);int ov85_021E5B0C(void*);
int ov85_021E5B0C(void*w){if(IsPaletteFadeFinished()){if(!Comm_GetLocalNetId())U(w,0)=2;else U(w,0)=8;return 1;}return 0;}
