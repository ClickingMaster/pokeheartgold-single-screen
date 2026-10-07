#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
#include "overlay45_format_buffer_private.h"
#include <string.h>
// clang-format on
#pragma force_active on
#define I(p,o) (*(s32*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
extern void *_022577C4;
extern char *_02254D04[];
extern void ov45_022320B4(Str*,void*),ov45_02237BFC(Str*,u32);
extern int ov45_02237F7C(Str*,Str*),ov45_02240A80(const char*),ov45_0223E4A8(void*),ov45_02233434(u32,u32,const void*,const void*,u32);
extern void *ov45_02231C78(void*);
extern u32 ov00_021FF368(void);
extern void ov45_02233BC8(Str*,const char*,...);
extern void ov45_0223325C(u32,u32,u32,u32,u32,const void*,u32),ov45_02241E24(const char*,void (*)(void),s32),ov45_022417B4(const char*,s32,const char*,void(*)(void),s32);
extern void ov45_022367E0(void),ov45_0223683C(void),ov45_0223694C(void),ov45_0223AF04(void*,void*);
extern Str *ov45_02233ABC(Str*,Str*,char*);
extern Str *ov45_022339F4(Str*,Str*,Str*);
extern char ov45_022550B8[];
extern char ov45_022550BC[];
extern char ov45_022550C4[];
extern char ov45_022550D4[];
extern char ov45_022550E4[];
extern char ov45_022550F4[];
void ov45_02233994(Str *out,u32 id,Str *s);
void ov45_02233B74(Str *out);
int ov45_022340F8(Str *a,Str *b);
int ov45_02234688(void);
int ov45_02234764(void);
int ov45_022347C4(void);
int ov45_02234814(void);
int ov45_02234870(void);
int ov45_022348C8(void);
int ov45_02234944(void);
int ov45_022349CC(u32 type);
int ov45_02234A24(int valid,u8 *data,u32 size,void **output,u32 *length);
void ov45_022353F0(int valid,u8 *data,u32 size);
void ov45_022354AC(int status);

#pragma warn_uninitializedvar off
extern Str *ov45_02238104(Str*,u32,u32,const char*,const char*,u8);
static inline u32 short_length(Str *p){if(p->l.flag)return p->l.size;return p->s.size;}
void ov45_02233B74(Str *out){u32 id=I(_022577C4,0x4c);u32 tick=ov00_021FF368();ov45_02233BC8(out,ov45_022550BC,id,tick%1000);}
