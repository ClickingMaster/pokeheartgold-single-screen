#include "global.h"
#include "heap.h"
#include <string.h>
#pragma force_active on
typedef struct {u8 enabled:1;u8 owner:1;u8 rest:6;} Flags;
typedef struct {u8 pad[0x34];int heap;u8 rest[0x2c];u8 source[0x58];} Context;
typedef struct {u32 a,b;} Pair;
typedef struct {Context *ctx;u8 pad4[0x14];void *source,*strings[28],*message,*callback;u8 initial,pad95,mode;Flags flags;u8 x,y,pad9a,z;u8 pad9c[4];u16 *result;u8 padA4[16];Pair first[28];u8 pad194[40];Pair second[28];u16 status[28],modeCopy,pad2d6;} State;
void ov80_0222E450(Context*,State*,u8,u8,u8,u8,u16*,void*,void*);
void*ov80_0222E558(void*,u8,u8,u8,u8,u16*,void*,void*);
typedef char StateSize[sizeof(State)==728?1:-1];
typedef char ContextSize[sizeof(Context)==0xbc?1:-1];
typedef char SourceOffset[offsetof(Context,source)==0x64?1:-1];
void*ov80_0222E558(void*ctx,u8 x,u8 y,u8 mode,u8 enabled,u16*result,void*callback,void*message){Context*c=ctx;State*s=Heap_Alloc((enum HeapID)c->heap,sizeof(State));if(s==NULL)return NULL;memset(s,0,sizeof(State));ov80_0222E450(c,s,x,y,mode,enabled,result,callback,message);return s;}
