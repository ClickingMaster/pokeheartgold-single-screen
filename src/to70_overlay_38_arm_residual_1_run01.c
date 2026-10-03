#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct{u32 p[2];s8 *data;int size,encode;} StringData;
typedef struct{StringData *data;int position;} State;
typedef struct{u8 p[12];int flag;} Post;
typedef struct{u8 p[0x50];u8 buffer[0xec];Post *post;} Session;
typedef struct{char a,b,c,d;} Enc;
extern Enc _02224E60;extern char ov38_022257B8[],ov38_02225644[],ov38_022255A4[],ov38_022257C8[],ov38_02225650[],ov38_022257F4[];extern s8 ov38_02225804[];
extern void __msl_assertion_failed(const char *,const char *,const char *,int);extern char *strchr(const char *,int);extern void ghiAppendCharToBuffer(void *,int);extern void ghiAppendDataToBuffer(void *,void *,int);extern int ov38_0221CD90(Session *,void *,int);
int ghiPostStringStateDoPosting(State *,Session *);
int ghiPostStringStateDoPosting(State *state,Session *s){if(state->position<0)__msl_assertion_failed(ov38_022257B8,ov38_02225644,ov38_022255A4,743);if(state->data->size==0)return 1;if(state->position>=state->data->size)__msl_assertion_failed(ov38_022257C8,ov38_02225644,ov38_022255A4,750);if(!s->post->flag&&state->data->encode){int i,ch;s8 *input;Enc enc;input=state->data->data;enc=_02224E60;i=0;ch=input[0];while(ch){if(strchr(ov38_02225650,ch)){ghiAppendCharToBuffer(s->buffer,ch);}else if(ch==32){ghiAppendCharToBuffer(s->buffer,43);}else{int high=ch/16;if(high>=16)__msl_assertion_failed(ov38_022257F4,ov38_02225644,ov38_022255A4,781);enc.b=ov38_02225804[high];enc.c=ov38_02225804[ch%16];ghiAppendDataToBuffer(s->buffer,&enc,3);}ch=input[++i];}return 1;}else{int left=state->data->size-state->position;int sent=ov38_0221CD90(s,state->data->data,left);if(sent==-1)return 0;state->position+=sent;return sent==left?1:2;}}
