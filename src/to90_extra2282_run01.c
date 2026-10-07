#pragma force_active on
#include "global.h"
#pragma force_active on
#pragma opt_propagation off
#define P(p,o) (*(void **)((u8 *)(p)+(o)))
#define U(p,o) (*(u32 *)((u8 *)(p)+(o)))
#define B(p,o) (*((u8 *)(p)+(o)))
extern u8 sPmAgbRomHeader[];typedef struct {u8 pad[32];u32 a,b;} Head;
u16 ov74_02233ACC(int a);
u16 ov74_02233ACC(int a){if(a==0){Head *p=(Head *)(sPmAgbRomHeader+0x68);return p->a;}if(a==4){Head *p=(Head *)(sPmAgbRomHeader+0x68);return p->b-11904;}if(a==13)return 2000;return 3968;}
