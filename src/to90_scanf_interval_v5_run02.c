#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
#include <stdarg.h>
extern int vsscanf(const char*,const char*,va_list);
int sscanf(const char*,const char*,...);
int sscanf(const char*s,const char*format,...){int result;va_list ap;va_start(ap,format);result=vsscanf(s,format,ap);va_end(ap);return result;}
