// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
int __StringRead(void *, int, int);
#define CURSOR(context) (*(const char **)(context))
#define EOF_FLAG(context) (*(int *)((char *)(context) + sizeof(const char *)))
int __StringRead(void *context, int character, int action) {
switch(action) {
case 0: { const char *cursor=CURSOR(context); char value=*cursor; if(!value) {EOF_FLAG(context)=1;return -1;} CURSOR(context)=cursor+1;return (unsigned char)value; }
case 1: { int *eof=(int *)((char *)context+sizeof(const char*)); if(!*eof) --CURSOR(context); else *eof=0; return character; }
case 2: return EOF_FLAG(context);
default: return 0;
}
}
