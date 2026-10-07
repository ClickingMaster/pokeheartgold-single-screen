#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct {u32 low:2,io:3,buffer:2,kind:3,pad:2,binary:1,upper:19;} Modes;
typedef struct {u32 io:3,pad:29;} State;
typedef struct {u32 fd;Modes modes;State state;u8 padc,error;u16 pade;u32 pad10,pad14,pos;u8*buffer;u32 size;u8*ptr;u32 avail;} File;
extern int fwide(File*,int),__flush_buffer(File*,u32*);extern void __prep_buffer(File*);extern void*__memrchr(const void*,int,u32);
u32 __fwrite(const void*,u32,u32,File*);
u32 __fwrite(const void*input,u32 size,u32 count,File*f){const u8*src=input;u32 left,written,n;void*newline;int buffered;
if(!fwide(f,0))fwide(f,-1);
left=size*count;
if(!left||f->error||!f->modes.kind)return 0;
buffered=1;
if(f->modes.binary&&f->modes.buffer!=2&&f->modes.buffer!=1)buffered=0;
if(f->state.io==0&&(f->modes.io&2)){f->state.io=1;__prep_buffer(f);}
if(f->state.io!=1){f->error=1;f->avail=0;return 0;}
written=0;
if(left&&(f->ptr!=f->buffer||buffered)){
f->avail=f->size-(f->ptr-f->buffer);
do{n=f->avail;newline=0;if(n>left)n=left;
if(f->modes.buffer==1&&n){newline=__memrchr(src,10,n);if(newline)n=(u8*)newline+1-src;}
if(n){memcpy(f->ptr,src,n);{u32 copied=n;src+=copied;f->ptr+=copied;f->avail-=n;left-=copied;}}
if(!f->avail||newline||!f->modes.buffer){if(__flush_buffer(f,0)){f->error=1;left=0;f->avail=0;break;}}
written+=n;
}while(left&&buffered);
}
if(left&&!buffered){u8*oldbuf=f->buffer;u32 oldsize=f->size;f->buffer=(u8*)src;f->size=left;f->ptr=(u8*)src+left;if(!__flush_buffer(f,&n))written+=n;else{f->error=1;f->avail=0;}f->buffer=oldbuf;f->size=oldsize;__prep_buffer(f);f->avail=0;}
if(f->modes.buffer!=2)f->avail=0;
return written/size;
}
