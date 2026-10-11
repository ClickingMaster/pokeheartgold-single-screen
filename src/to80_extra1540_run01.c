#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u8 pad[0x36ac];int a,b;}S;extern int Comm_GetLocalNetId(void);extern void ov65_0221DE10(int,int,int);
void ov65_0221DE24(S*s,int b,int a);
void ov65_0221DE24(S*s,int b,int a){if(a!=s->a||b!=s->b){ov65_0221DE10(Comm_GetLocalNetId(),b,a);s->a=a;s->b=b;}}
