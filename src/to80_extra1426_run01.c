#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct {u32 first,second;u8 context[0x1bac-8];int active;u8 pad[0x1c1c-0x1bb0];void *output;} Work;
typedef struct {char *host;int zero,limit;u32 first,second;int different; int timeout;} Param;
extern Work *ov00_0221A748;
extern char *ov00_02216420;
extern const char ov00_02216B30[],ov00_02216B50[],ov00_02216B58[],ov00_02216B60[];
extern int strcmp(const char*,const char*);
extern int ov00_021E8FA0(void*,Param*);
extern void ov00_021EC24C(int,int);
extern int ov00_021E8984(void*);
extern int ov00_021E8C5C(void*,void*,int);
extern void ov00_021E9754(void*);
extern int ov00_021E99A4(void*,const char*,const void*,int);
extern int ov00_021E90BC(void*);
extern void ov00_021E9154(void*,int);
int ov00_021EB8B0(void*,void*);
int ov00_021EB8B0(void *input,void *output) {
 u8 auth[0x94]; Param param; register Work *work;
 ov00_0221A748->output=output;MI_CpuFill8(output,0,0x174);
 if(ov00_0221A748->active && !OS_IsThreadTerminated((OSThread*)((u8*)ov00_0221A748+0x1b40)))OS_Terminate();
 param.host=ov00_02216420;param.zero=0;param.limit=0x1000;
 work=ov00_0221A748;param.first=work->first;param.second=work->second;
 if(strcmp(param.host,ov00_02216B30)!=0)param.different=1;else param.different=0;param.timeout=20000;
 if(ov00_021E8FA0(work->context,&param)) {ov00_021EC24C(9,-24100);return 0;}
 if(!ov00_021E8984(auth) || !ov00_021E8C5C(ov00_0221A748->context,auth,0)) {
  ov00_021E9754(ov00_0221A748->context);ov00_021EC24C(9,-24100);return 0;
 }
 if(ov00_021E99A4(ov00_0221A748->context,ov00_02216B50,ov00_02216B58,6) || ov00_021E99A4(ov00_0221A748->context,ov00_02216B60,input,4)) {
  ov00_021E9754(ov00_0221A748->context);ov00_021EC24C(9,-24100);return 0;
 }
 if(ov00_021E90BC(ov00_0221A748->context)) {
  ov00_021E9754(ov00_0221A748->context);ov00_021EC24C(9,-24100);return 0;
 }
 ov00_021E9154(ov00_0221A748->context,OS_GetThreadPriority(OS_GetCurrentThread())-1);
 if(ov00_0221A748->active)return 1;
 ov00_021E9754(ov00_0221A748->context);ov00_021EC24C(9,-24100);return 0;
}
