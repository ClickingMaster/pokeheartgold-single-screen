#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#define W(p,n) (*(int*)((u8*)(p)+(n)))
#define P(p,n) (*(void**)((u8*)(p)+(n)))
void __msl_assertion_failed(char*,char*,char*,int);
int ArrayLength(void*);void *ov00_021FE6EC(void*,int);void ov00_021FE968(void*,int);void ov00_021EC3D8(void*);void *ov00_021EC3A8(int);void ov00_021FEE8C(void*);void ov00_021FE678(void*);
void *ov00_021FF0C0(void*,void*);void *ov00_021FE5B4(int,int,int);void *ov00_021FED4C(int,int,int,int(*)(char*,int),int(*)(char*,char*),void(*)(void*));void ov00_021FE754(void*,void*);void ov00_021FE8D8(void*,int);void ov00_021FF01C(void*,void*);int ov00_021FEA40(void*,void*,int(*)(char*,char*),int);
int ov00_021FEF00(void*);int strnicmp(const char*,const char*);char *strncpy(char*,const char*,size_t);extern u8 __lower_mapC[];
int ov45_0224739C(void*,int);int ov45_02247650(char*,char*);
extern char ov45_0225565C[];
extern char ov45_02255668[];
extern char ov45_02255678[];
extern char ov45_02255688[];
extern char ov45_022556B0[];
extern char ov45_02255700[];
extern char ov45_02255714[];
extern char ov45_02255728[];
extern char ov45_0225573C[];
extern char ov45_022557A0[];
extern char ov45_022557CC[];
extern char ov45_022557E4[];
extern char ov45_022557FC[];
extern char ov45_02255814[];
extern char ov45_02255844[];
extern char ov45_022558A4[];
extern char ov45_022558D4[];
extern char ov45_022558F0[];
extern char ov45_0225590C[];
extern char ov45_02255918[];
extern char ov45_02255928[];
extern char ov45_02255938[];
extern char ov45_02255948[];
extern char ov45_022559B8[];
extern char ov45_022559C8[];
extern char ov45_022559D4[];
extern char ov45_022559E4[];
extern char ov45_022559F8[];
extern char ov45_02255A2C[];
void ov00_021FF160(void*,void(*)(void*,void*),void*);
extern char ov45_02255420[];
extern char ov45_02255458[];
extern char ov45_02255468[];
extern char ov45_02255478[];
extern char ov45_02255488[];
extern char ov45_022554A4[];
extern char ov45_022554BC[];
extern char ov45_022554DC[];
extern char ov45_022554F0[];
extern char ov45_0225565C[];
extern char ov45_02255668[];
extern char ov45_02255678[];
extern char ov45_02255688[];
extern char ov45_022556B0[];
extern char ov45_022556EC[];
extern char ov45_02255700[];
extern char ov45_02255714[];
extern char ov45_02255728[];
extern char ov45_0225573C[];
extern char ov45_02255750[];
extern char ov45_022557A0[];
extern char ov45_022557B4[];
extern char ov45_022557CC[];
extern char ov45_022557E4[];
extern char ov45_022557FC[];
extern char ov45_02255814[];
extern char ov45_02255844[];
extern char ov45_0225588C[];
extern char ov45_022558A4[];
extern char ov45_022558BC[];
extern char ov45_022558D4[];
extern char ov45_022558F0[];
extern char ov45_0225590C[];
extern char ov45_02255918[];
extern char ov45_02255928[];
extern char ov45_02255938[];
extern char ov45_02255948[];
extern char ov45_02255958[];
extern char ov45_02255968[];
extern char ov45_02255978[];
extern char ov45_02255988[];
extern char ov45_02255998[];
extern char ov45_022559A8[];
extern char ov45_022559B8[];
extern char ov45_022559C8[];
extern char ov45_022559D4[];
extern char ov45_022559E4[];
extern char ov45_022559F8[];
extern char ov45_02255A2C[];
extern char ov45_02255A30[];
extern char ov45_02255AA8[];
extern char ov45_02255B44[];
extern char ov45_02255B54[];
extern char ov45_02255B64[];
extern char ov45_02255B7C[];
extern char ov45_02255B94[];
extern char ov45_02255BA8[];
extern char ov45_02255BC4[];
int ov45_022474FC(void*c,int n);
int ov45_02247518(char*s,int n);
void *ov45_022475B8(void*c,char*s);
int ov45_022475F0(char*s,int n);
int ov45_022477B8(char*s,int n);
void ov45_02247770(void*c);
void ov45_02247934(void*c);
int ov45_02247968(void*c);
void ov45_022479F0(void*c);
void ov45_02247A44(void*c,char*s);
int ov45_02247AD4(char*a,char*b);
int ov45_02247AE0(void*c,char*s);
void ov45_02247D84(void*c,char*s);
void *ov45_02247E98(void*c,char*s);
int ov45_02248238(void*c,char*s);
int ov45_02248288(void*c,char*s,void*out);
void ov45_02248374(void*c,char*s,void*data);
void ov45_02248454(void*c,char*s,char*data);
void ov45_02248558(void*c,char*s);
int ov45_0224861C(void*c,char*s);
void ov45_022486E0(void*c,char*s,char*data);
int ov45_022487BC(void*c,char*s);
int ov45_02247650(char*a,char*b);
int ov45_02247818(char*a,char*b);
void ov45_02248A98(void*c,char*s,char*name);
void ov45_02248BCC(void*c,void*t);
void ov45_02248CE8(void*c,char*s,void*cb,void*arg);
int ov45_0224739C(void*c,int id);
int ov45_02248238(void*c,char*s){if(!c)__msl_assertion_failed(ov45_022559B8,ov45_02255918,ov45_0225565C,0x20f);if(ov45_022475B8(c,s)==0)return 0;return 1;}

int ov45_02248288(void*c,char*s,void*out){void*p;if(!c)__msl_assertion_failed(ov45_022559B8,ov45_02255918,ov45_022556B0,0x21a);if(!s)__msl_assertion_failed(ov45_022559D4,ov45_02255918,ov45_022556B0,0x21c);if(!*s)__msl_assertion_failed(ov45_022559E4,ov45_02255918,ov45_022556B0,0x21c);if(strlen(s)>=257)__msl_assertion_failed(ov45_022559F8,ov45_02255918,ov45_022556B0,0x21c);p=ov45_022475B8(c,s);if(!p)return 0;if(!W(p,0x158))return 0;memcpy(out,(u8*)p+0x138,32);return 1;}

void ov45_02248374(void*c,char*s,void*data){void*p;if(!c)__msl_assertion_failed(ov45_022559B8,ov45_02255918,ov45_02255688,0x233);if(!s)__msl_assertion_failed(ov45_022559D4,ov45_02255918,ov45_02255688,0x235);if(!*s)__msl_assertion_failed(ov45_022559E4,ov45_02255918,ov45_02255688,0x235);if(strlen(s)>=257)__msl_assertion_failed(ov45_022559F8,ov45_02255918,ov45_02255688,0x235);p=ov45_022475B8(c,s);if(!p)return;W(p,0x158)=1;memcpy((u8*)p+0x138,data,32);}

void ov45_02248454(void*c,char*s,char*data){void*p;int n;if(!c)__msl_assertion_failed(ov45_022559B8,ov45_02255918,ov45_022557CC,0x24a);if(!s)__msl_assertion_failed(ov45_022559D4,ov45_02255918,ov45_022557CC,0x24c);if(!*s)__msl_assertion_failed(ov45_022559E4,ov45_02255918,ov45_022557CC,0x24c);if(strlen(s)>=257)__msl_assertion_failed(ov45_022559F8,ov45_02255918,ov45_022557CC,0x24c);p=ov45_022475B8(c,s);if(!p)return;ov00_021EC3D8(P(p,0x15c));if(!data)data=ov45_02255A2C;n=strlen(data);P(p,0x15c)=ov00_021EC3A8(n+1);if(!P(p,0x15c))return;memcpy(P(p,0x15c),data,n+1);}

void ov45_02248558(void*c,char*s){void*p;if(!c)__msl_assertion_failed(ov45_022559B8,ov45_02255918,ov45_022557E4,0x278);if(!s)__msl_assertion_failed(ov45_022559D4,ov45_02255918,ov45_022557E4,0x27a);if(!*s)__msl_assertion_failed(ov45_022559E4,ov45_02255918,ov45_022557E4,0x27a);if(strlen(s)>=257)__msl_assertion_failed(ov45_022559F8,ov45_02255918,ov45_022557E4,0x27a);p=ov45_022475B8(c,s);if(p)W(p,0x160)=1;}

int ov45_0224861C(void*c,char*s){void*p;if(!c)__msl_assertion_failed(ov45_022559B8,ov45_02255918,ov45_022558A4,0x28a);if(!s)__msl_assertion_failed(ov45_022559D4,ov45_02255918,ov45_022558A4,0x28c);if(!*s)__msl_assertion_failed(ov45_022559E4,ov45_02255918,ov45_022558A4,0x28c);if(strlen(s)>=257)__msl_assertion_failed(ov45_022559F8,ov45_02255918,ov45_022558A4,0x28c);p=ov45_022475B8(c,s);if(p==0)return 0;return W(p,0x160);}

void ov45_022486E0(void*c,char*s,char*data){void*p;if(!c)__msl_assertion_failed(ov45_022559B8,ov45_02255918,ov45_02255728,0x29a);if(!s)__msl_assertion_failed(ov45_022559D4,ov45_02255918,ov45_02255728,0x29c);if(!*s)__msl_assertion_failed(ov45_022559E4,ov45_02255918,ov45_02255728,0x29c);if(strlen(s)>=257)__msl_assertion_failed(ov45_022559F8,ov45_02255918,ov45_02255728,0x29c);p=ov45_022475B8(c,s);if(!p)return;strncpy((u8*)p+0x164,data,128);((char*)p)[0x1e3]=0;}

int ov45_022487BC(void*c,char*s){void*p;if(!c)__msl_assertion_failed(ov45_022559B8,ov45_02255918,ov45_02255814,0x2bc);if(!s)__msl_assertion_failed(ov45_022559D4,ov45_02255918,ov45_02255814,0x2be);if(!*s)__msl_assertion_failed(ov45_022559E4,ov45_02255918,ov45_02255814,0x2be);if(strlen(s)>=257)__msl_assertion_failed(ov45_022559F8,ov45_02255918,ov45_02255814,0x2be);p=ov45_022475B8(c,s);if(!p)return -1;return ov00_021FEF00(P(p,0x134));}
