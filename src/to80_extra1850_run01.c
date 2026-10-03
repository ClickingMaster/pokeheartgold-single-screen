#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
#include <string.h>
// clang-format on
extern char ov45_02257600[],ov45_02257258[],ov45_02257184[],ov45_02257610[],ov45_02257620[],ov45_02257624[];
extern void __msl_assertion_failed(char*,char*,char*,int);
extern void*ov00_021EC3A8(u32);extern void ov00_021EC3D8(void*);extern int ov45_02253374(char*,void*);
int ov45_022533E4(char*,void*);
#pragma force_active on
int ov45_022533E4(char*input,void*out){char*tail;char*token;char*copy;
 if(!input)__msl_assertion_failed(ov45_02257600,ov45_02257258,ov45_02257184,0x2a1);
 if(!out)__msl_assertion_failed(ov45_02257610,ov45_02257258,ov45_02257184,0x2a2);
 copy=ov00_021EC3A8(strlen(input)+1);if(!copy)return 0;strcpy(copy,input);
 if(copy[0]==':'){copy[0]=0;tail=copy+1;}else{tail=strstr(copy,ov45_02257620);if(tail){*tail=0;tail+=2;}}
 token=strtok(copy,ov45_02257624);if(token){do{if(!ov45_02253374(token,out)){ov00_021EC3D8(copy);return 0;}token=strtok(NULL,ov45_02257624);}while(token);}
 if(tail&&!ov45_02253374(tail,out)){ov00_021EC3D8(copy);return 0;}ov00_021EC3D8(copy);return 1;}
