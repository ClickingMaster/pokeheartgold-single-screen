#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
#pragma opt_lifetimes off
typedef struct {u32 remaining;char *dest;} Sink;
void string_put_string(Sink*,const char*,int);
void string_put_string(Sink *s,const char *src,int count){int i;u32 n;if(count<=0)return;n=s->remaining;if(n>(u32)count)n=count;for(i=0;i<n;i++)s->dest[i]=src[i];s->remaining-=n;s->dest+=count;}
