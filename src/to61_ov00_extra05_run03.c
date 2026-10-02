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

int ov00_02215EBC(void *p) { struct {u8 len,fam;u16 port;u32 addr;} sa; int size=8;u8 buf[1500];if(ov00_021FF694(W(p,0x4b0)))do {int i;void *q; if(ov00_021FF940(W(p,0x4b0),buf,1499,0,&sa,&size)!=-1){i=ov00_02213CFC(p,sa.addr,sa.port);if(i==-1){q=ov00_022132C4(p,sa.addr,sa.port);if(ov00_0221339C(q))return 5;ov00_02213370(q,0x11);ov00_02213C6C(p,q);}}}while(ov00_021FF694(W(p,0x4b0)));if(ov00_021FF368()-W(p,0x4b4)>2000){ov00_021FF818(W(p,0x4b0));W(p,0x4b0)=-1;W(p,0)=1;((void (*)(void *,int,u32,u32))W(p,0x488))(p,3,ov00_0221B9D4,W(p,0x494));}return 0; }

int ov00_02215FE0(void *p) { ov00_02213E10(p);switch(W(p,0)){case 2:case 3:return ov00_02215BF4(p);case 0:return ov00_02215EBC(p);case 1:default:return 0;} }
