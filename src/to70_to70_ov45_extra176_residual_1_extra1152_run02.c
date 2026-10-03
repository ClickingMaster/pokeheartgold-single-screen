#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
#include <string.h>
void ov45_02249554(u8 *,int,u8 *);
void ov45_022495DC(u8 *,int,u8 *);
void ov45_0224966C(s8 *,int,s8 *);
#define W(p,o) (*(u32 *)((u8 *)(p)+(o)))
extern char ov45_02255938[],ov45_02255918[],ov45_02255874[],ov45_02255A30[],ov45_02255B7C[];
extern char ov45_022565C0[],ov45_022565D0[],ov45_0225609C[],ov45_02256630[],ov45_02256638[];
extern void __msl_assertion_failed(char*,char*,char*,int);
extern u8 *ov00_021FF0C0(u32,char*);
extern int strnicmp(char*,char*);
void ov45_02249390(u8*,char**);
u8 *ov45_0224983C(u8*,char*);
void ov45_02249554(u8 *key,int n,u8 *s){int i,k,j;for(i=0;i<256;i++)s[255-i]=i;k=0;s[256]=k;j=k;i=k;s[257]=k;for(i=0;i<256;i++){int t=s[i];int x=key[k];x+=t;j=((j+x)%256)&255;s[i]=s[j];s[j]=t;k=((k+1)%n)&255;}}
void ov45_022495DC(u8 *data,int n,u8 *s){int i=s[256],j=s[257],k;for(k=0;k<n;k++){int t;i=((i+1)%256)&255;t=s[i];j=((j+t)%256)&255;s[i]=s[j];s[j]=t;{int idx=s[i];idx+=t;data[k]^=s[(idx%256)&255];}}s[256]=i;s[257]=j;}
void ov45_0224966C(s8 *data,int n,s8 *key){int i; s8 *p=key;for(i=0;i<n;i++){data[i]^=*p;if(!*++p)p=key;}}
