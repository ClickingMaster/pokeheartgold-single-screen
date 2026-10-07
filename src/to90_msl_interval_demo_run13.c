#pragma force_active on
#include "global.h"
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef struct{u32 words[21];} Tail;
typedef struct{void *type;u32 b,c,d,e,f,g;Tail tail;} ThrowContext;
typedef struct{u32 a;void *record;u8 *cursor;u32 d,e,f;} ExceptionInfo;
typedef struct{ExceptionInfo info;ThrowContext context;} Iterator;
typedef struct{u32 count,flags;s32 offset;u8 *types;} Specification;
typedef struct{u32 type,tag;s32 offset;} Catch;
extern u32 _Z13CurrentActionPK14ActionIterator(const Iterator *);
extern u32 _Z10NextActionP14ActionIterator(Iterator *);
extern u8 *_Z22__DecodeUnsignedNumberPcPm(u8 *,u32 *);
extern u8 *_Z20__DecodeSignedNumberPcPl(u8 *,s32 *);
extern int __throw_catch_compare(void *,u32,s32 *);
extern int _Z17IsInSpecificationPcP16ex_specification(void *,Specification *);
extern void _Z16HandleUnexpectedP12ThrowContextP13ExceptionInfoP16ex_specificationPc(ThrowContext *,ExceptionInfo *,Specification *,u8 *);
extern void _ZSt9terminatev(void);
u8 *_Z20FindExceptionHandlerP12ThrowContextP13ExceptionInfoPl(ThrowContext *,ExceptionInfo *,s32 *);
#pragma force_active on
u8 *_Z20FindExceptionHandlerP12ThrowContextP13ExceptionInfoPl(ThrowContext *context,ExceptionInfo *info,s32 *offset){
 struct{Specification specification;Catch handler;Iterator iterator;}f;u8 *p;u32 action;
 f.iterator.info.a=info->a;f.iterator.info.record=info->record;f.iterator.info.cursor=info->cursor;f.iterator.info.d=info->d;f.iterator.info.e=info->e;f.iterator.info.f=info->f;
 f.iterator.context.type=context->type;f.iterator.context.b=context->b;f.iterator.context.c=context->c;f.iterator.context.d=context->d;f.iterator.context.e=context->e;f.iterator.context.f=context->f;f.iterator.context.g=context->g;f.iterator.context.tail=context->tail;
 action=_Z13CurrentActionPK14ActionIterator(&f.iterator);
 for(;;){
  switch(action){
  case 12:
   p=f.iterator.info.cursor;f.handler.type=p[1]|((u32)p[2]<<8)|((u32)p[3]<<16)|((u32)p[4]<<24);
   p=_Z22__DecodeUnsignedNumberPcPm(p+5,&f.handler.tag);_Z20__DecodeSignedNumberPcPl(p,&f.handler.offset);
   if(!__throw_catch_compare(context->type,f.handler.type,offset))goto next;
   goto found;
  case 15:
   p=_Z22__DecodeUnsignedNumberPcPm(f.iterator.info.cursor+1,&f.specification.count);p=_Z22__DecodeUnsignedNumberPcPm(p,&f.specification.flags);f.specification.types=_Z20__DecodeSignedNumberPcPl(p,&f.specification.offset);
   if(!_Z17IsInSpecificationPcP16ex_specification(context->type,&f.specification))_Z16HandleUnexpectedP12ThrowContextP13ExceptionInfoP16ex_specificationPc(context,info,&f.specification,f.iterator.info.cursor);
   break;
  case 0:case 2:case 3:case 4:case 5:case 6:case 7:case 8:case 9:case 10:case 11:case 13:case 16:case 17:case 18:case 19:break;
  default:_ZSt9terminatev();goto found;
  }
next:
  action=_Z10NextActionP14ActionIterator(&f.iterator);
 }
found:return f.iterator.info.cursor;
}
