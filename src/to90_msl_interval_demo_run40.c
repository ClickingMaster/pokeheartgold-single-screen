#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct{u32 pad[3];const u8 *start,*end;} Info;
extern const u8 __exception_table_start__[],__exception_table_end__[];
BOOL _Z20__FindExceptionTableP13ExceptionInfoPc(Info*,char*);
BOOL _Z20__FindExceptionTableP13ExceptionInfoPc(Info *info,char *position){info->start=__exception_table_start__;info->end=__exception_table_end__;return TRUE;}
