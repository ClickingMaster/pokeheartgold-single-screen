#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
extern int rand(void);
// clang-format on
#pragma force_active on
#define I(p,o) (*(s32*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
extern void ov00_021EC3D8(void*),ov00_02202B28(void*,void*);
extern int ov00_0220A5A8(void*,void*,s32*),ov00_02207E40(void*,void*);
extern void ov00_0220A86C(void*,s32,const void*),ov00_02202990(void*,s32,s32),ov00_0220A890(void*,const void*),ov00_0220A348(void*,const void*,s32);
extern void __msl_assertion_failed(const void*,const void*,const void*,s32);
extern char ov00_0221869C[];
extern char ov00_022186EC[];
extern char ov00_02218860[];
extern char ov00_02218AC4[];
extern char ov00_02218AEC[];
extern char ov00_02218B1C[];
extern char ov00_02218B40[];
extern char ov00_02218B4C[];
extern char ov00_02218B5C[];
int ov00_02202D20(void *ctx,void *filter);
void ov00_02202E08(char *out,int count);
int ov00_0220411C(void *ctx);
int ov00_022041C0(void *ctx,void *node);
int ov00_0220444C(s32 day,s32 month,s32 year);
int ov00_022045AC(void *ctx,s32 value,s32 *day,s32 *month,s32 *year);
void ov00_02204654(void *input,void *output);

extern int ov00_022020EC(void*,void*,const void*),ov00_0220211C(void*,void*,s32),ov00_0220267C(void*,void*,void*,s32*,s32,const void*);
extern void ov00_021FF82C(void*,s32),ov00_021FF818(void*),ov00_022066D0(void*,void*),ov00_022071F4(void*,void*);
extern int ov00_02207F1C(void*,int (*)(void*,void*),s32);
extern void ov00_02204284(void*,s32);
extern char ov00_02218B04[],ov00_022189C0[],ov00_02218B18[];
int ov00_02202D20(void *ctx,void *filter){void *s=P(ctx,0),*head,*tail,*p;if(filter){void *prev;head=P(s,0x438);tail=P(s,0x43c);prev=0;P(s,0x438)=0;P(s,0x43c)=0;p=head;while(p){void *next=P(p,20);if(P(p,16)==filter||I(p,12)==1){if(prev)P(prev,20)=next;else head=next;if(tail==p)tail=prev;ov00_02202B28(ctx,p);}else prev=p;p=next;}if(P(s,0x438))P(P(s,0x43c),20)=head;else P(s,0x438)=head;P(s,0x43c)=tail;return 0;}while((p=P(s,0x438))!=0){P(s,0x438)=0;P(s,0x43c)=0;while(p){void *next=P(p,20);ov00_02202B28(ctx,p);p=next;}}return 0;}
void ov00_02202E08(char *out,int count){int i;for(i=0;i<count;i++)out[i]=ov00_022186EC[(u32)rand()%62];out[i]=0;}
