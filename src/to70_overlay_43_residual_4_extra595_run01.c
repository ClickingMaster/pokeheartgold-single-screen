#pragma force_active on
#include "global.h"
extern void *sub_0202C6F4(void *);extern void *sub_0202C08C(void *);extern u64 DWC_CreateFriendKey(void *);extern int sub_0202C2DC(void *,int);extern u64 String_atoi(void *,int *);extern int DWC_CheckFriendKey(void *,u64);extern int sub_0203A128(void *,u64,int *);extern void *sub_0202C23C(void *,int);extern void DWC_CreateFriendKeyToken(void *,u64);extern void sub_0202C270(void *,int,void *);extern void sub_0202C190(void *,int,int,int);
typedef struct{u32 pad;void *save;} Work;
int ov43_0222A358(Work *,void *,void *);
int ov43_0222A358(Work *w,void *text,void *out){int i,ok,other;u64 self;void *book;book=sub_0202C6F4(w->save);self=DWC_CreateFriendKey(sub_0202C08C(book));for(i=0;i<32;i++){if(sub_0202C2DC(book,i)==0){u64 key=String_atoi(text,&ok);if(ok==1&&key!=self){if(!DWC_CheckFriendKey(sub_0202C08C(book),key))return 1;if(!sub_0203A128(w->save,key,&other))return 2;DWC_CreateFriendKeyToken(sub_0202C23C(book,i),key);sub_0202C270(book,i,out);sub_0202C190(book,i,8,2);return 0;}return 1;}}GF_AssertFail();return 0;}
