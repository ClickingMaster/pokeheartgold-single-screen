#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
extern char ov00_02218D04[],ov00_02218D18[],ov00_02218D1C[],ov00_02218D20[],ov00_02218D24[],ov00_02218D28[],ov00_02218C58[],ov00_02218DB4[],ov00_02218C18[],ov00_02218C00[],ov00_02218D38[],ov00_02218D48[],ov00_02218D54[],ov00_02218D60[],ov00_02218D70[],ov00_02218D80[],ov00_02218D90[],ov00_02218DA4[],ov00_02218C70[],ov00_02218C78[],ov00_02218C80[],ov00_02218C88[],ov00_02218C90[],ov00_02218C98[],ov00_02218CA0[];
int sprintf(char*,const char*,...);
void ov00_0220A890(void*,char*);
int ov00_02205128(void*,char*,char*);
int ov00_02205168(void*,char*,char*);
int ov00_022051A8(void*,int,int);
int ov00_022051A8(void*c,int field,int value){char buffer[16];int result;switch(field){
case 1800:
if(value<0){ov00_0220A890(c,ov00_02218D04);return 2;}
sprintf(buffer,ov00_02218D18,value);result=ov00_02205128(c,ov00_02218C18,buffer);if(result)return result;break;
case 0x70b:switch(value){
case 1280:result=ov00_02205128(c,ov00_02218C58,ov00_02218D1C);if(result)return result;break;
case 1281:result=ov00_02205128(c,ov00_02218C58,ov00_02218D20);if(result)return result;break;
case 1282:result=ov00_02205128(c,ov00_02218C58,ov00_02218D24);if(result)return result;break;
default:ov00_0220A890(c,ov00_02218D28);return 2;}break;
case 1798:
sprintf(buffer,ov00_02218D18,value);result=ov00_02205128(c,ov00_02218C00,buffer);if(result)return result;break;
case 1804:
sprintf(buffer,ov00_02218D18,value);result=ov00_02205168(c,ov00_02218D38,buffer);if(result)return result;break;
case 1805:
sprintf(buffer,ov00_02218D18,value);result=ov00_02205168(c,ov00_02218D48,buffer);if(result)return result;break;
case 1806:
sprintf(buffer,ov00_02218D18,value/16);result=ov00_02205168(c,ov00_02218D54,buffer);if(result)return result;break;
case 1808:
sprintf(buffer,ov00_02218D18,value/4);result=ov00_02205168(c,ov00_02218D60,buffer);if(result)return result;break;
case 1810:
sprintf(buffer,ov00_02218D18,value/4);result=ov00_02205168(c,ov00_02218D70,buffer);if(result)return result;break;
case 1811:
sprintf(buffer,ov00_02218D18,value);result=ov00_02205168(c,ov00_02218D80,buffer);if(result)return result;break;
case 1812:
sprintf(buffer,ov00_02218D18,value);result=ov00_02205168(c,ov00_02218D90,buffer);if(result)return result;break;
case 1813:
if(value)value=1;sprintf(buffer,ov00_02218D18,value);result=ov00_02205168(c,ov00_02218DA4,buffer);if(result)return result;break;
case 1816:
sprintf(buffer,ov00_02218D18,value);result=ov00_02205128(c,ov00_02218C70,buffer);if(result)return result;break;
case 1817:
sprintf(buffer,ov00_02218D18,value);result=ov00_02205128(c,ov00_02218C78,buffer);if(result)return result;break;
case 1818:
sprintf(buffer,ov00_02218D18,value);result=ov00_02205128(c,ov00_02218C80,buffer);if(result)return result;break;
case 1819:
sprintf(buffer,ov00_02218D18,value);result=ov00_02205128(c,ov00_02218C88,buffer);if(result)return result;break;
case 1820:
sprintf(buffer,ov00_02218D18,value);result=ov00_02205128(c,ov00_02218C90,buffer);if(result)return result;break;
case 1821:
sprintf(buffer,ov00_02218D18,value);result=ov00_02205128(c,ov00_02218C98,buffer);if(result)return result;break;
case 1822:
sprintf(buffer,ov00_02218D18,value);result=ov00_02205128(c,ov00_02218CA0,buffer);if(result)return result;break;
default:ov00_0220A890(c,ov00_02218DB4);return 2;}return 0;}
