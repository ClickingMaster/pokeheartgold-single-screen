#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
#define I(p,o) (*(s32*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
typedef struct Copy {u32 data[60];} Copy;
extern int ov00_022020EC(void*,void*,const void*),ov00_0220211C(void*,void*,s32);
extern void *ov00_021EC3A8(u32),*ov00_021FF3D4(void*),*ov00_021FE6EC(void*,s32);
extern void ov00_021EC3D8(void*),ov00_021FF82C(void*,s32),ov00_021FF818(void*),ov00_021FE678(void*),ov00_021FE968(void*,s32);
extern s32 ArrayLength(void*);
extern void __msl_assertion_failed(const void*,const void*,const void*,s32);
extern void ov00_0220A890(void*,const void*),ov00_0220A3BC(void*,const void*,...);
extern s32 ov00_02203A58(void*,void*),ov00_02207B8C(void*,void*),ov00_0220484C(void*,void*),ov00_0220A244(void*,void*),ov00_0220684C(void*,void*),ov00_02206BA8(void*,void*),ov00_02206ED8(void*,void*);
extern int ov00_0220267C(void*,void*,void*,s32*,s32,const void*);
extern void ov00_02201AE8(void*,s32,s32,void*);
extern void ov00_021FF48C(void*,u32),ov00_021FF4D0(void*,u32),ov00_021FF510(void*),ov00_021FF558(void*);
extern char ov00_02218BAC[];
extern char ov00_02218CD0[];
extern char ov00_02218CE8[];
extern char ov00_02218CF0[];
extern char ov00_02218E2C[];
extern char ov00_02218E44[];
extern char ov00_02218E4C[];
extern char ov00_02218E74[];
extern char ov00_02218E9C[];
extern char ov00_02218ED4[];
extern char ov00_02218EE4[];
extern char ov00_02218EE8[];
extern char ov00_02218F08[];
extern char ov00_02218F60[];
extern char ov00_0221904C[];
extern char ov00_022190A8[];
extern char ov00_022190F0[];
int ov00_02205064(void *ctx,void *buf);
int ov00_02205128(void *ctx,void *key,void *value);
int ov00_02205168(void *ctx,void *key,void *value);
int ov00_02205E50(void *ctx,s32 a,s32 b);
int ov00_0220608C(void *ctx,void *node,Copy *input);
void ov00_02206164(void *node);
int ov00_0220657C(void *ctx,s32 type,void *data,void **result,s32 flag,void *a,void *b);
void ov00_02206634(void *ctx,void *node);
void ov00_022066D0(void *ctx,void *node);
int ov00_02206720(void *ctx,void **result,s32 id);
int ov00_0220676C(void *ctx);
int ov00_022067A8(void *ctx,void *node);
int ov00_02206E04(void *ctx,void *node);
int ov00_02207158(void *ctx,void *node);
void ov00_022071F4(void *ctx,void *node);
void ov00_02207370(void *ctx);

int ov00_02206E04(void *ctx,void *node){s32 done;void *buf;int e;if(I(node,48))return 0;if(ArrayLength(P(node,56))){do{buf=ov00_021FE6EC(P(node,56),0);e=ov00_0220267C(ctx,P(node,8),buf,&done,0,ov00_0221904C);if(done||e){I(node,0)=0x6a;return 0;}if(I(buf,12)!=I(buf,8))break;ov00_021FE968(P(node,56),0);}while(ArrayLength(P(node,56)));}return 0;}
