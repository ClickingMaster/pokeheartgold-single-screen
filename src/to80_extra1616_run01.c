#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct {u8 justify,sign,flag,alt,length,pad;u16 conversion;int width,precision;} Format;
extern const u16 __wctype_mapC[];
#define DIGIT(c) ((u32)(c)>=128?0:__wctype_mapC[(c)]&8)
u16 *parse_format__wide(u16 *p,char **args,Format *out);
u16 *parse_format__wide(u16 *start,char **args,Format *out){
 Format f;int c,more;u16 *p;
 c=start[1];p=start+1;f.justify=1;f.sign=0;f.flag=0;f.alt=0;f.length=0;f.width=0;f.precision=0;
 if(c=='%'){f.conversion=c;*out=f;return p+1;}
 do{more=1;switch(c){case '-':f.justify=0;break;case '+':f.sign=1;break;case ' ':if(f.sign!=1)f.sign=2;break;case '#':f.alt=1;break;case '0':if(f.justify!=0)f.justify=2;break;default:more=0;break;}if(!more)break;c=*++p;}while(1);
 if(c=='*'){*args+=4;f.width=*(int*)(*args-4);if(f.width<0){f.justify=0;f.width=-f.width;}c=*++p;}else{while(DIGIT(c)){f.width=f.width*10+(c-'0');c=*++p;}}
 if(f.width>509){f.conversion=65535;*out=f;return p+1;}
 if(c=='.'){c=*++p;f.flag=1;if(c=='*'){*args+=4;f.precision=*(int*)(*args-4);c=*++p;if(f.precision<0)f.flag=0;}else{while(DIGIT(c)){f.precision=f.precision*10+(c-'0');c=*++p;}}}
 more=1;
 switch(c){case 'h':f.length=2;if(p[1]=='h'){f.length=1;c=*++p;}break;case 'l':f.length=3;if(p[1]=='l'){f.length=4;c=*++p;}break;case 'L':f.length=9;break;case 'j':f.length=6;break;case 't':f.length=8;break;case 'z':f.length=7;break;default:more=0;break;}
 if(more)c=*++p;f.conversion=c;
 switch(c){
 case 'd':case 'i':case 'o':case 'u':case 'x':case 'X':if(f.length==9)f.length=4;if(!f.flag)f.precision=1;else if(f.justify==2)f.justify=1;break;
 case 'f':case 'F':if(f.length==2||f.length==6||f.length==7||f.length==8||f.length==4)f.conversion=65535;else if(!f.flag)f.precision=6;break;
 case 'a':case 'A':if(!f.flag)f.precision=13;if(f.length==2||f.length==6||f.length==7||f.length==8||f.length==4||f.length==1)f.conversion=65535;break;
 case 'g':case 'G':if(f.precision==0)f.precision=1;
 case 'e':case 'E':if(f.length==2||f.length==6||f.length==7||f.length==8||f.length==4||f.length==1)f.conversion=65535;else if(!f.flag)f.precision=6;break;
 case 'p':f.length=3;f.alt=1;f.conversion='x';f.precision=8;break;
 case 'c':if(f.length==3)f.length=5;else if(f.flag||f.length!=0)f.conversion=65535;break;
 case 's':if(f.length==3)f.length=5;else if(f.length!=0)f.conversion=65535;break;
 case 'n':if(f.length==9)f.length=4;break;
 default:f.conversion=65535;break;
 }
 *out=f;return p+1;
}
