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

int ov00_02215BF4(void *p) { int used,r,err; if(!ov00_021FF694(W(p,0x4b0)))return 0; used=W(p,0x80);r=ov00_021FF92C(W(p,0x4b0),(u8 *)W(p,0x7c)+used,0x1000-used,0);if((u32)(r+1)<=1){ov00_02214130(p);return 3;}W(p,0x80)+=r;err=0;if(W(p,0)==2 || (int)W(p,0x5c8)>0)ov00_022122B8((u8 *)p+0x4bc,(u8 *)W(p,0x7c)+used,W(p,0x80)-used);if(W(p,0)==3)err=ov00_02215064(p);if(err)return err;if(W(p,0)==2 && (int)W(p,0x80)>0)return ov00_02215A30(p);return 0; }
