#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct {u32 p[4];u8 *records;int count;} S;
extern int ov38_0222308C(void*,void**);
int ov38_02221468(void*,S*);
extern const char ov38_02225EC0[];
extern void ov38_02221888(void*,int);
extern int ov38_02222F88(void*,const char*,char**);
extern int ov38_022218B4(const char*,int),ov38_022201A0(int);
extern void ov38_0222189C(void*,const char*,u32),ov38_022202C0(void*,const char*,int);
int ov38_02221494(void*,S*);
int ov38_02221468(void *a,S*s){void *p;int count=ov38_0222308C(a,&p);return count==s->count;}
