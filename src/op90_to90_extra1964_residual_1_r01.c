#pragma force_active on
#include "global.h"
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef struct {u32 spare0:2,io:3,buffering:2,kind:3,spare1:2,binary:1,spare2:19;} Mode;
typedef struct {u32 value:3,spare:29;} State;
typedef struct {u32 handle;Mode mode;State state;u8 eof,error;union {u8 bytes[14];u16 words[7];} push;u8 *buffer;u32 bufferSize;u8 *cursor;u32 available;u32 word2c,savedAvailable;u32 pos34,pad38;int (*read3c)(u32,u32,u32 *,u32);int (*write40)(u32,u32,u32 *,u32);u32 pad44,extra48;} Stream;
typedef char StreamExtent[sizeof(Stream)==0x4c?1:-1];
typedef char StreamReadOffset[offsetof(Stream,read3c)==0x3c?1:-1];
extern int fwide(void *,int);
extern int __flush_line_buffered_output_files(void);
extern int __load_buffer(Stream *,u32 *,int);
extern void __prep_buffer(Stream *);
u32 __fread(void *,u32,u32,Stream *);
// @fn __fread
u32 __fread(void *destination,u32 size,u32 count,Stream *stream){
 u32 remaining;u8 *dst;u32 done;int result;int buffered;u32 n;
 dst=destination;
 if(fwide(stream,0)==0)fwide(stream,-1);
 remaining=size*count;
 if(!remaining||stream->error||!stream->mode.kind)return 0;
 buffered=1;
 if(stream->mode.binary&&stream->mode.buffering!=2)buffered=0;
 if(stream->state.value==0&&(stream->mode.io&1)){stream->state.value=2;stream->available=0;}
 if(stream->state.value<2){stream->error=1;stream->available=0;return 0;}
 if((stream->mode.buffering&1)&&__flush_line_buffered_output_files()!=0){stream->error=1;stream->available=0;return 0;}
 done=0;
 if(remaining&&stream->state.value>=3){
  do{
   if(fwide(stream,0)==1){*(u16 *)dst=stream->push.words[stream->state.value];dst+=2;done+=2;remaining-=2;}
   else{*dst++=stream->push.bytes[1+stream->state.value];done++;remaining--;}
   stream->state.value--;
  }while(remaining&&stream->state.value>=3);
  if(stream->state.value==2)stream->available=stream->savedAvailable;
 }
 if(remaining&&(stream->available||buffered)){
  do{
   if(!stream->available){result=__load_buffer(stream,NULL,0);if(result){if(result==1){stream->error=1;stream->available=0;}else{stream->state.value=0;stream->eof=1;stream->available=0;}remaining=0;break;}}
   n=stream->available;if(n>remaining)n=remaining;
   memcpy(dst,stream->cursor,n);dst+=n;done=n+done;remaining-=n;stream->cursor+=n;stream->available=stream->available-n;
  }while(remaining&&buffered);
 }
 if(remaining&&!buffered){
  u8 *oldBuffer=stream->buffer;u32 oldSize=stream->bufferSize;
  stream->buffer=dst;stream->bufferSize=remaining;result=__load_buffer(stream,&n,1);
  if(result){if(result==1){stream->error=1;stream->available=0;}else{stream->state.value=0;stream->eof=1;stream->available=0;}}
  {u32 copied=n;stream->buffer=oldBuffer;stream->bufferSize=oldSize;done+=copied;}__prep_buffer(stream);stream->available=0;
 }
 return done/size;
}
