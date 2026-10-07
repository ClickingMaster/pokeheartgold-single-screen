#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct {u32 begin,size,extra;} Entry;Entry *_Z12BinarySearchP19ExceptionTableIndexmPc(Entry *,u32,const char *);
#pragma force_active on
Entry *_Z12BinarySearchP19ExceptionTableIndexmPc(Entry *table,u32 count,const char *pc){s32 low=0,high=count-1;while(low<=high){s32 mid=(low+high)>>1;Entry *entry=&table[mid];if((u32)pc<entry->begin)high=mid-1;else if((u32)pc>entry->begin+(entry->size&~1))low=mid+1;else return entry;}return NULL;}
