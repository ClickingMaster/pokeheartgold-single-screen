#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct {u32 left;char *p;} Out;
u32 STD_GetStringLength(const char *s);
u32 STD_CopyLString(char *d,const char *s,u32 cap);
u32 STD_GetStringLength(const char *s);
int STD_CompareString(const char*a,const char*b);
int STD_CompareNString(const char*a,const char*b,int n);
void string_put_char(Out *o,int c);
void string_fill_char(Out*o,int c,int n);
void string_put_string(Out*o,const char*s,int n);
u32 STD_GetStringLength(const char *s){u32 n=0;while(s[n])n++;return n;}
int STD_CompareString(const char*a,const char*b){while(*a==*b&&*a){a++;b++;}return *a-*b;}
int STD_CompareNString(const char*a,const char*b,int n){int i;if(n){for(i=0;i<n;i++){if((u8)a[i]!=(u8)b[i])return (u8)a[i]-(u8)b[i];}}return 0;}
void string_put_char(Out *o,int c){if(o->left){*o->p=c;o->left--;}o->p++;}
