#pragma force_active on
#include "global.h"
// clang-format off
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
 #include "overlay45_string_search.h"
u32 ov45_02237EE4(const StringSearch*s,const s8*needle,u32 start,u32 count){u32 len,left;const s8*data,*end,*at,*p,*q;if(s->raw.longForm.flag){data=s->raw.longForm.data;len=s->raw.longForm.size;}else{data=s->raw.shortForm.data;len=s->raw.shortForm.size;}if(start>len)goto fail;left=len-start;end=needle+count;at=data+start;if(left<count)goto fail;do{p=at;q=needle;while(q<end){if(*q!=*p)goto next;q++;p++;}return at-data;next:left--;at++;}while(left>=count);fail:return ~0U;}
