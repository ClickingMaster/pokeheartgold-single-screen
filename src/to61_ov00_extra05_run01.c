#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#include <string.h>
#define W(p,o) (*(u32 *)((u8 *)(p)+(o)))
#define H(p,o) (*(u16 *)((u8 *)(p)+(o)))
extern void ov00_022148A4(void *);
extern void *ov00_021FE5B4(int,int,void *);
extern void ov00_021FE754(void *,void *);
extern int ov00_02213FD4(const char *,int);
extern char *ov00_02213EE8(void *,const char *);
extern int ov00_02213CFC(void *,u32,u16);
extern void *ov00_02213DFC(void *,int);
extern void ov00_02213DA0(void *,int);
extern void ov00_02214BF0(void *,const void *,int,void *,void *);
extern void *ov00_022132C4(void *,u32,u16);
extern int ov00_0221339C(void *);
extern int ov00_02214C60(void *,void *,void *,int,int);
extern void ov00_02213C6C(void *,void *,...);
extern int ov00_021FF694(int);
extern int ov00_021FF92C(int,void *,int,int);
extern int ov00_021FF940(int,void *,int,int,void *,void *);
extern int ov00_021FF970(int,void *,int,int);
extern int ov00_021FF818(int);
extern u32 ov00_021FF368(void);
extern void ov00_02214130(void *);
extern void ov00_022122B8(void *,void *,int);
extern int ov00_02215064(void *);
extern void ov00_02213E10(void *);
extern void ov00_02213370(void *,u8);
extern void ov00_02213378(void *,u32,u16);
extern void __msl_assertion_failed(const char *,const char *,const char *,int);
extern char ov00_0221A650[],ov00_0221A5C8[],ov00_0221A574[];
extern u32 ov00_0221B9D4;
extern int ov00_02215728(void *,u8 *,int);
extern int ov00_0221556C(void *,u8 *,int);
int ov00_02215490(void *p,u8 *s,int n) ;
int ov00_022158AC(void *p,u8 *s,int n) ;
int ov00_02215938(void *p,void *s,int n) ;
int ov00_02215BF4(void *p) ;
int ov00_02215EBC(void *p) ;
int ov00_02215FE0(void *p) ;
int ov00_02215A30(void *p) ;

int ov00_02215728(void *p,u8 *s,int n);
int ov00_0221556C(void *p,u8 *s,int n);

int ov00_0221556C(void *p,u8 *s,int n) { u32 count,last;int i;u32 addr,value;u16 port;char *name;if(n<2)return 4;count=s[1];last=s[0];s+=2;n-=2;for(i=0;i<(int)count;i++){int len;name=(char *)s;len=ov00_02213FD4((char *)s,n);if(len==-1)return 4;s+=len;n-=len;if(n<11)return 4;memcpy(&addr,s,4);memcpy(&port,s+4,2);memcpy(&value,s+6,4);s+=10;n-=10;value=((value>>24)&255)|((value>>8)&0xff00)|((value<<8)&0xff0000)|((value<<24)&0xff000000);len=ov00_02213FD4((char *)s,n);if(len==-1)return 4;((void (*)(void *,char *,u32,u16,u32,char *,u32))W(p,0x490))(p,name,addr,port,value,(char *)s,W(p,0x494));s+=len;n-=len;}if(last)((void (*)(void *,char *,u32,u16,u32,char *,u32))W(p,0x490))(p,0,0,0,0,0,W(p,0x494));return 0; }

int ov00_02215728(void *p,u8 *s,int n) { u32 value;u16 port;u32 addr;char *strings[16];int i,count;void *q;if(n<11)return 4;memcpy(&addr,s,4);memcpy(&port,s+4,2);i=ov00_02213CFC(p,addr,port);if(i==-1)return 0;q=ov00_02213DFC(p,i);memcpy(&value,s+6,4);value=((value>>24)&255)|((value>>8)&0xff00)|((value<<8)&0xff0000)|((value<<24)&0xff000000);count=s[10];s+=11;n-=11;for(i=0;i<count && i<16;i++){int len;if(n<1)break;len=ov00_02213FD4((char *)s,n);if(len==-1)return 4;strings[i]=(char *)s;s+=len;n-=len;}if(!W(p,0x48c))return 0;((void (*)(void *,void *,u32,int,char **,u32))W(p,0x48c))(p,q,value,i,strings,W(p,0x494));return 0; }

int ov00_022158AC(void *p,u8 *s,int n) { u32 addr; u16 port; int i; if(n<6)return 4; memcpy(&addr,s,4); memcpy(&port,s+4,2); i=ov00_02213CFC(p,addr,port); if(i==-1)return 0;ov00_02213DA0(p,i);return 0; }

int ov00_02215938(void *p,void *s,int n) { u32 addr; u16 port; void *q; int i; if(n<5)return 4; ov00_02214BF0(p,s,n,&addr,&port);i=ov00_02213CFC(p,addr,port); if(i==-1){q=ov00_022132C4(p,addr,port);if(ov00_0221339C(q))return 5;}else q=ov00_02213DFC(p,i); if(ov00_02214C60(p,q,s,n,0)<0)return 4; if(i==-1)ov00_02213C6C(p,q);((void (*)(void *,int,void *,u32))W(p,0x488))(p,1,q,W(p,0x494));return 0; }
