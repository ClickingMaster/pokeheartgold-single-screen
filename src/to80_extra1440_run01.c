#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct {u8 p[0x1a08];char *base,*end,*limit;int size;OSMutex mutex;u8 pad[0x1a30-0x1a18-sizeof(OSMutex)];long length;int got;} Work;
extern char ov00_022166C4[],ov00_022166E0[],ov00_022166F4[];extern long atol(const char*);
int ov00_021E9350(Work*);
int ov00_021E9350(Work*w){char *s=w->base,*header,*tail;char saved;int n;char **buffer=&w->base;if(!strstr(s,ov00_022166C4))return 0;s=strstr(s,ov00_022166C4)+4;OS_LockMutex(&w->mutex);w->got=buffer[1]-s;OS_UnlockMutex(&w->mutex);header=strstr(buffer[0],ov00_022166E0);if(!header)return 1;n=strlen(ov00_022166E0);tail=strstr(header+n,ov00_022166F4);saved=*tail;*tail=0;OS_LockMutex(&w->mutex);w->length=atol(header+n);OS_UnlockMutex(&w->mutex);*tail=saved;return 1;}
