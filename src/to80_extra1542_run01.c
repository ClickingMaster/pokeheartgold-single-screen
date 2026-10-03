#pragma force_active on
#include "global.h"
#pragma force_active on
extern void String_Delete(void*);
void ov70_0223A224(u8*s);
void ov70_0223A224(u8*s){int i=0;u8*cursor=s;for(;i<10;i++){String_Delete(*(void**)(cursor+0xbc4));cursor+=4;}String_Delete(*(void**)(s+0xbbc));String_Delete(*(void**)(s+0xbc0));}
