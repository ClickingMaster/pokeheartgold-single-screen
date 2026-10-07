#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 enabled:1;u8 owner:1;u8 rest:6;} Flags;
typedef struct {u8 pad[0x34];int heap;u8 rest[0x2c];u8 source[0x58];} Context;
typedef struct {u32 a,b;} Pair;
typedef struct {Context *ctx;u8 pad4[0x14];void *source,*strings[28],*message,*callback;u8 initial,pad95,mode;Flags flags;u8 x,y,pad9a,z;u8 pad9c[4];u16 *result;u8 padA4[16];Pair first[28];u8 pad194[40];Pair second[28];u16 status[28],modeCopy,pad2d6;} State;
extern void *NewMsgDataFromNarc(int,int,int,int),*String_New(int,int);
void ov80_0222E450(Context *,State *,u8,u8,u8,u8,u16 *,void *,void *);
void ov80_0222E450(Context *ctx,State *s,u8 x,u8 y,u8 mode,u8 enabled,u16 *result,void *callback,void *message){
 int i;
 if(message==0){s->message=NewMsgDataFromNarc(1,27,191,ctx->heap);s->flags.owner=1;}
 else {s->message=message;s->flags.owner=0;}
 s->callback=callback;s->ctx=ctx;s->result=result;*s->result=0;
 s->flags.enabled=enabled;s->mode=mode;s->x=x;s->y=y;s->z=0;
 s->source=ctx->source;s->initial=3;s->modeCopy=mode;
 for(i=0;i<28;i++){s->first[i].a=0;s->first[i].b=0;}
 for(i=0;i<28;i++){s->second[i].a=0;s->second[i].b=0;s->status[i]=255;}
 for(i=0;i<28;i++)s->strings[i]=String_New(80,ctx->heap);
 *s->result=0xeeee;
}
