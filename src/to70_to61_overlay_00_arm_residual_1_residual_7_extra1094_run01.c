#pragma force_active on
#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#include <string.h>
extern char *strchr(const char *,int);extern int strncmp(const char *,const char *,u32);
typedef struct {u64 seed,multiplier,increment;} RandomState;extern RandomState ov00_0221A76C;int ov00_021ED4E8(char *,char *,char *,int);u32 ov00_021ED5E4(u32);
int ov00_021ED4E8(char *key,char *out,char *input,int separator){char *p,*q;int length;if(out==0)return -1;p=strchr(input,separator);if(p==0)return -1;for(;;){if(strncmp(p+1,key,strlen(key))==0&&*((char *)strlen(key)+(u32)p+1)==separator)break;q=strchr(p+1,separator);if(q==0)return -1;p=strchr(q+1,separator);if(p==0)return -1;}p=strchr(p+1,separator);if(p==0)return -1;q=strchr(p+1,separator);if(q!=0)length=q-(p+1);else length=strlen(p+1);strncpy(out,p+1,length);out[length]=0;return length;}
