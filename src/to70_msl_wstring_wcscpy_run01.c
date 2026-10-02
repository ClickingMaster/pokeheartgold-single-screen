#pragma force_active on
#include <nitro/types.h>
#pragma force_active on
u16 *wcscpy(u16 *,const u16 *);
u16 *wcscpy(u16 *out,const u16 *in) {u16 *p=out;while((*p++=*in++)!=0) {} return out;}
