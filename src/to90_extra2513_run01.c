#pragma force_active on
#include "global.h"
#pragma force_active on
extern u8 _022459B0[];typedef struct{u8 pad[48];u32 flags;}Global;extern Global _0224CF98;
int ov13_02222A84(u16),ov13_02221D58(u8*,u8*),ov13_02221EAC(u8*,u8*),ov13_02221F64(u8*,u8*);
int ov13_02221F94(int,u8*,int,u8*,u8*);
typedef struct{u8 type,reserved;u16 length;}Header;
int ov13_02221F94(int id,u8*p,int remaining,u8*out,u8*other){int flags=0;if(remaining<=0)return-2;Header*record;for(;;){record=(Header*)p;if(record->type==_022459B0[id])break;int skip=ov13_02222A84(*(u16*)(p+2))+4;remaining-=skip;p+=skip;if(remaining<=0)return-4;}u16 encoded=record->length;p+=4;int size=ov13_02222A84(encoded);u8*base=out+id*0x350;u8*first;u32 offset;u8*otherCopy;otherCopy=other;offset=(id+3)*128;first=base;first+=8;do{int r;switch(p[0]){case 3:r=ov13_02221D58(p,first);flags|=1;break;case 4:r=ov13_02221D58(p,base+0x138);flags|=2;break;case 5:r=ov13_02221EAC(p,base+0x268);flags|=4;break;case 6:r=ov13_02221EAC(p,base+0x2d8);flags|=8;break;case 10:r=ov13_02221F64(p,otherCopy+offset);break;default:r=-3;break;}if(r)return r;int skip=ov13_02222A84(*(u16*)(p+2))+4;size-=skip;p+=skip;}while(size>0);_0224CF98.flags|=flags;return 0;}
