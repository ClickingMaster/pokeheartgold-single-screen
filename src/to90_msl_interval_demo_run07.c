#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct {u8 *address;u32 flags;u8 *data;} Index;
typedef struct {u8 *address,*unwind,*record;Index *first,*last;} Info;
extern int _Z20__FindExceptionTableP13ExceptionInfoPc(Info *,u8 *);
extern Index *_Z12BinarySearchP19ExceptionTableIndexmPc(Index *,u32,u8 *);
extern u8 *_Z16__SkipUnwindInfoPc(u8 *),*_Z22__DecodeUnsignedNumberPcPm(u8 *,u32 *);
void _Z19FindExceptionRecordPcP13ExceptionInfo(u8 *,Info *);
void _Z19FindExceptionRecordPcP13ExceptionInfo(u8 *address,Info *info){
 Index *index;u8 *p;u32 delta,length,offset,relative,base;
 info->unwind=0;info->record=0;
 if(!_Z20__FindExceptionTableP13ExceptionInfoPc(info,address))return;
 index=_Z12BinarySearchP19ExceptionTableIndexmPc(info->first,info->last-info->first,address);if(!index)return;
 if(index->flags&1)info->unwind=(u8 *)&index->data;else info->unwind=index->data;
 info->address=index->address;relative=address-index->address;p=_Z16__SkipUnwindInfoPc(info->unwind);base=0;
 for(;;){
  p=_Z22__DecodeUnsignedNumberPcPm(p,&delta);if(delta==0)return;
  p=_Z22__DecodeUnsignedNumberPcPm(p,&length);p=_Z22__DecodeUnsignedNumberPcPm(p,&offset);
  {u32 start=base+delta;if(relative<start)return;base=start+length;}if(relative>base)continue;
  info->record=info->unwind+offset;return;
 }
}
