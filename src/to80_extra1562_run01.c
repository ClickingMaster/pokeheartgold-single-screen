#pragma force_active on
#include "global.h"
#pragma force_active on
extern int Options_GetFrame(void*);extern void ov83_02247944(void*,int),ov83_02242BF0(void*),ov83_02242DFC(void*);
void ov83_02240348(u8*s);
void ov83_02240348(u8*s){ov83_02247944(s+0xb0,Options_GetFrame(*(void**)(s+0x508)));*(int*)(s+0x848)=-2;ov83_02242BF0(s);ov83_02242DFC(s);}
