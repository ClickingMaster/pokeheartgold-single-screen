#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma opt_common_subs off
#pragma force_active on
#define W(p,o) (*(u32 *)((u8 *)(p)+(o)))
typedef union Str{struct{u32 large:1;u32 other:31;u32 length;char *ptr;} f;struct{u8 bit:1;u8 size:7;u8 rest[11];}small;struct{u8 header;char data[11];}shortData;u8 bytes[12];}Str;
extern u8 *_022577C4;
extern char ov45_022550C4[];
extern int ov45_02237F7C(Str*,Str*);
extern void *ov45_02231C78(void *);
extern void ov45_02233434(u32,u32,char *,void *,u32);
int ov45_022340F8(Str*,Str*);
int ov45_02234764(void);
static inline u32 strsize(Str*s){if(s->f.large)return s->f.length;return s->small.size;}
static inline int flush(u8*c){u8 *peer;void *buf;if(W(c,0x11c)==0)return 1;W(c,0x11c)=0;peer=(u8*)W(_022577C4,0x4c);buf=ov45_02231C78(c+0x110);ov45_02233434((u32)peer,0,ov45_022550C4,buf,W(c,0x114));return 1;}
extern void ov45_022320B4(Str*,void*);
extern int ov45_02240A80(char*);
extern void ov45_02237BC8(Str*);
extern int ov45_0223E4A8(void*);
int ov45_02234688(void);
static inline BOOL ready(int n){return n>=4;}
int ov45_02234764(void){return flush(_022577C4);}
