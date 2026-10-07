#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct{u32 zero:2,buffer:3,pad:2,kind:3,rest:22;}Mode;typedef struct{u32 io:3,rest:29;}State;
typedef struct Stream{u32 handle;Mode mode;State state;u8 eof,error;u16 pad;u32 unk[2],position;u8*base;u32 size;u8*ptr;u32 remaining,pad2[2],start;int(*seek)(u32,long*,int,void*);u32 pad3[3];void*context;}Stream;
extern int errno;extern int __flush_buffer(Stream*,void*);extern long _ftell(Stream*);int _fseek(Stream*,long,int);
int _fseek(Stream*f,long offset,int origin){u8 kind=f->mode.kind;if(kind!=1||f->error){errno=40;return -1;}if(f->state.io==1&&__flush_buffer(f,0)){f->error=1;f->remaining=0;errno=40;return -1;}if(origin==1){origin=0;offset+=_ftell(f);}if(origin!=2&&f->mode.buffer!=3&&(u32)(f->state.io-2)<=1){if((u32)offset>=f->position||(u32)offset<f->start)f->state.io=0;else{f->ptr=f->base+((u32)offset-f->start);f->remaining=f->position-offset;f->state.io=2;}}else f->state.io=0;if(f->state.io==0){if(f->seek&&f->seek(f->handle,&offset,origin,f->context)){f->error=1;f->remaining=0;errno=40;return -1;}f->eof=0;f->position=offset;f->remaining=0;}return 0;}
