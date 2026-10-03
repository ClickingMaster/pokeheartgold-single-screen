#pragma force_active on
#include "global.h"
// clang-format off
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
#include <string.h>
extern long atol(const char *);
typedef struct State {u8 pad[0x110];char first[31],second[21],third[51],fourth[31];} State;
typedef struct Context {State *state;} Context;
typedef struct Pair {char first,second;} Pair;
extern void ov00_0220A890(Context *,const char *);
extern void ov00_0220A348(char *,const char *,u32);
extern int ov00_02205128(Context *,const char *,const char *);
extern int ov00_02205168(Context *,const char *,const char *);
extern int ov00_022051A8(Context *,int,int);
extern void ov00_021FF408(char *);
extern const u8 __upper_mapC[];
extern const char ov00_02218DC4[];
extern const char ov00_02218BC8[];
extern const char ov00_02218BD0[];
extern const char ov00_02218BE0[];
extern const char ov00_02218DD4[];
extern const char ov00_02218BE8[];
extern const char ov00_02218BF4[];
extern const char ov00_02218C0C[];
extern const char ov00_02218C18[];
extern const char ov00_02218DE0[];
extern const char ov00_02218C24[];
extern const char ov00_02218D1C[];
extern const char ov00_02218D20[];
extern const char ov00_02218D24[];
extern const char ov00_02218C58[];
extern const char ov00_02218C00[];
extern const char ov00_02218DF8[];
extern const char ov00_02218E0C[];
extern const char ov00_02218E20[];
extern const char ov00_02218C68[];
extern const char ov00_02218C70[];
extern const char ov00_02218C78[];
extern const char ov00_02218C80[];
extern const char ov00_02218C88[];
extern const char ov00_02218C90[];
extern const char ov00_02218C98[];
extern const char ov00_02218CA0[];
extern const char ov00_02218DB4[];
static inline int Upper(int c){return (c<0||c>=128)?c:__upper_mapC[c];}
int ov00_02205678(Context *,int,const char *);
int ov00_02205678(Context *context,int key,const char *value){
 State *state=context->state;char buffer[256];int result;
 if(value==NULL){ov00_0220A890(context,ov00_02218DC4);return 2;}
 switch(key){
case 1792:
 if(value[0]==0){ov00_0220A890(context,ov00_02218DC4);return 2;}
 ov00_0220A348(buffer,value,31);
 ov00_0220A348(state->first,buffer,31);
 result=ov00_02205128(context,ov00_02218BC8,buffer);if(result!=0)return result;break;
case 1793:
 if(value[0]==0){ov00_0220A890(context,ov00_02218DC4);return 2;}
 ov00_0220A348(buffer,value,21);
 ov00_0220A348(state->second,buffer,21);
 result=ov00_02205128(context,ov00_02218BD0,buffer);if(result!=0)return result;break;
case 1794:
 if(value[0]==0){ov00_0220A890(context,ov00_02218DC4);return 2;}
 ov00_0220A348(buffer,value,51);
 ov00_021FF408(buffer);
 ov00_0220A348(state->third,buffer,51);
 result=ov00_02205168(context,ov00_02218BE0,buffer);if(result!=0)return result;break;
case 1795:
 if(value[0]==0){ov00_0220A890(context,ov00_02218DC4);return 2;}
 ov00_0220A348(buffer,value,31);
 ov00_0220A348(state->fourth,buffer,31);
 result=ov00_02205168(context,ov00_02218DD4,buffer);if(result!=0)return result;break;
case 1796:ov00_0220A348(buffer,value,31);result=ov00_02205128(context,ov00_02218BE8,buffer);if(result!=0)return result;break;
case 1797:ov00_0220A348(buffer,value,31);result=ov00_02205128(context,ov00_02218BF4,buffer);if(result!=0)return result;break;
case 1799:ov00_0220A348(buffer,value,76);result=ov00_02205128(context,ov00_02218C0C,buffer);if(result!=0)return result;break;
case 1800:ov00_0220A348(buffer,value,11);result=ov00_02205128(context,ov00_02218C18,buffer);if(result!=0)return result;break;
case 0x709:
 if(strlen(value)!=2){ov00_0220A890(context,ov00_02218DE0);return 2;}
 ov00_0220A348(buffer,value,3);result=ov00_02205128(context,ov00_02218C24,buffer);if(result!=0)return result;break;
 case 0x70b:{char gender=Upper(value[0]);
 if(gender=='M')memcpy(buffer,ov00_02218D1C,2);
 else if(gender=='F')memcpy(buffer,ov00_02218D20,2);
 else memcpy(buffer,ov00_02218D24,2);
 result=ov00_02205128(context,ov00_02218C58,buffer);if(result!=0)return result;break;}
case 1798:ov00_0220A348(buffer,value,256);result=ov00_02205128(context,ov00_02218C00,buffer);if(result!=0)return result;break;
case 1805:result=ov00_022051A8(context,1805,atol(value));if(result!=0)return result;break;
case 1806:result=ov00_022051A8(context,1806,atol(value));if(result!=0)return result;break;
case 1807:ov00_0220A348(buffer,value,256);result=ov00_02205128(context,ov00_02218DF8,buffer);if(result!=0)return result;break;
case 1808:result=ov00_022051A8(context,1808,atol(value));if(result!=0)return result;break;
case 1809:ov00_0220A348(buffer,value,256);result=ov00_02205128(context,ov00_02218E0C,buffer);if(result!=0)return result;break;
case 1810:result=ov00_022051A8(context,1810,atol(value));if(result!=0)return result;break;
case 1812:result=ov00_022051A8(context,1812,atol(value));if(result!=0)return result;break;
case 1813:result=ov00_022051A8(context,1813,atol(value));if(result!=0)return result;break;
case 1814:ov00_0220A348(buffer,value,256);result=ov00_02205128(context,ov00_02218E20,buffer);if(result!=0)return result;break;
case 1815:ov00_0220A348(buffer,value,51);result=ov00_02205128(context,ov00_02218C68,buffer);if(result!=0)return result;break;
case 1816:ov00_0220A348(buffer,value,256);result=ov00_02205128(context,ov00_02218C70,buffer);if(result!=0)return result;break;
case 1817:ov00_0220A348(buffer,value,256);result=ov00_02205128(context,ov00_02218C78,buffer);if(result!=0)return result;break;
case 1818:ov00_0220A348(buffer,value,256);result=ov00_02205128(context,ov00_02218C80,buffer);if(result!=0)return result;break;
case 1819:ov00_0220A348(buffer,value,256);result=ov00_02205128(context,ov00_02218C88,buffer);if(result!=0)return result;break;
case 1820:ov00_0220A348(buffer,value,256);result=ov00_02205128(context,ov00_02218C90,buffer);if(result!=0)return result;break;
case 1821:ov00_0220A348(buffer,value,256);result=ov00_02205128(context,ov00_02218C98,buffer);if(result!=0)return result;break;
case 1822:ov00_0220A348(buffer,value,256);result=ov00_02205128(context,ov00_02218CA0,buffer);if(result!=0)return result;break;
default:ov00_0220A890(context,ov00_02218DB4);return 2;
 }return 0;
}
