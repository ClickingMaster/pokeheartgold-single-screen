#include "global.h"
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef struct Packet { char gap[16]; u8 owner; u8 sender; u16 pad; u32 size; u32 sequence; char samples[0x440]; int flagged; u32 timestamp; u64 receive; u64 ready; void *data; } Packet;
typedef struct Stream { u32 jitter; u32 p4; u32 left; u32 pC; u32 p10; u32 lost; u32 p18; u32 p1c; u32 retrans; u32 codec; u32 oldDelay; u32 delay; u32 seq; u32 owner; u32 session; Packet *previous; char gap40[24]; u32 last; char gap5c[20]; int skip; char rest[32]; } Stream;
extern Stream sStream[];
extern int vct_count_audio_queue(u32);
extern int vct_save_previous_buffer(Stream *, Packet *);
extern Packet *vct_top_audio_queue(u32);
extern void vct_free_packet_buffer(Packet *);

#pragma force_active on

int VCTi_ReceiveAudioData(void *dst, u32 size, u32 ch, u32 *mask);
// @fn VCTi_ReceiveAudioData
int VCTi_ReceiveAudioData(void *dst, u32 size, u32 ch, u32 *mask){
 int count;
 Packet *p;
 Stream *stream;
 int first;
 u64 now;
 stream=&sStream[ch];
 first=1;
 count=vct_count_audio_queue(ch);
 while(count){
  now=OS_GetTick();
  p=vct_top_audio_queue(ch);
  if(p==NULL)goto empty;
  if(p->ready>now+52){
   if(p->flagged==0&&stream->previous!=NULL){
    stream->retrans++;
    MIi_CpuCopyFast(stream->previous->data,dst,size);
    vct_free_packet_buffer(stream->previous);
    stream->previous=NULL;
    if(mask!=NULL)*mask|=1<<p->owner;
    return 1;
   }
   return 0;
  }
  if(now>p->ready+0x8B4C){
   stream->last=p->sequence;
   count=vct_save_previous_buffer(stream,p);
   vct_top_audio_queue(ch);
   first=0;
   continue;
  }
  {
   u32 seq=p->sequence;
   u32 last=stream->last;
   if(last+1!=seq&&last!=0&&last<=seq&&p->flagged==0)stream->lost++;
  }
  stream->last=p->sequence;
  if(first&&stream->skip){
   count=vct_save_previous_buffer(stream,p);
   vct_top_audio_queue(ch);
   stream->skip--;
   continue;
  }
  {
   u32 delta=now-p->receive;
   u32 diff=delta-stream->oldDelay;
   u32 d;
   stream->oldDelay=delta;
   d=delta+((diff-delta)>>4);
   stream->delay=d;
   stream->jitter=d;
  }
  MIi_CpuCopyFast(p->data,dst,size);
  vct_save_previous_buffer(stream,p);
  if(mask!=NULL)*mask|=1<<p->owner;
  return 1;
 empty:
  stream->left=count;
  return 0;
 }
 if(stream->previous!=NULL){
  vct_free_packet_buffer(stream->previous);
  stream->previous=NULL;
 }
 return 0;
}
