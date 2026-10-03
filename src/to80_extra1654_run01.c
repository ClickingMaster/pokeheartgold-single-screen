#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
extern void __msl_assertion_failed(const char*,const char*,const char*,int);
typedef struct Item{u32 kind;char pad[24];int result;}Item;
extern int ov00_02203A58(void*,Item*,int),ov00_02207B8C(void*,Item*,int),ov00_0220484C(void*,Item*,int),ov00_0220A244(void*,Item*,int);extern void ov00_0220A3BC(void*,void*,u32);extern char ov00_02218F08[],ov00_02218EE4[],ov00_02218E9C[],ov00_02218E74[];int ov00_022067A8(void*,Item*,int);
int ov00_022067A8(void*p,Item*i,int x){int result=0;switch(i->kind){case 0:result=ov00_02203A58(p,i,x);break;case 1:result=ov00_02207B8C(p,i,x);break;case 2:result=ov00_0220484C(p,i,x);break;case 4:result=ov00_0220A244(p,i,x);break;default:ov00_0220A3BC(p,ov00_02218F08,i->kind);__msl_assertion_failed(ov00_02218EE4,ov00_02218E9C,ov00_02218E74,326);}if(result)i->result=result;return result;}
