#pragma force_active on
#include "global.h"
extern u32 ov13_0224E284[8],ov13_0224E380[18];
extern u8 ov13_0224E184[],ov13_0224E2E8[][40];
extern u32 ov13_0224E2B0[];
extern u8 *ov13_02224694(u8**,const u8*,u32*,int*);
extern int ov13_02226D0C(char*,int);
int ov13_02224B2C(const u8*);
static inline u16 swap(u16 v){return (u8)(v>>8)|((v<<8)&0xff00);}
int ov13_02224B2C(const u8*data){u8 *start=(u8*)data+8,*pos=start,*value;int result=0;u32 bytes=swap(*(const u16*)data),type;int length;
 value=ov13_02224694(&pos,start+bytes,&type,&length);
 while(value){
 switch(type){
 case 0x201:memset(ov13_0224E284,0,sizeof(ov13_0224E284));memcpy(ov13_0224E284,value,length);result=1;break;
 case 0x202:ov13_0224E2B0[0]=swap(*(u16*)value);break;
 case 0x203:{int i=0;u8*p=ov13_0224E184;u16 v=swap(*(u16*)value);do{*(u32*)(p+0x15c)=v;p+=40;i++;}while(i<4);break;}
 case 0x204:{u8*p=ov13_0224E184;int i=0;u16 v=swap(*(u16*)value);do{*(u32*)(p+0x160)=v;p+=40;i++;}while(i<4);break;}
 case 0x205:ov13_0224E2B0[1]=swap(*(u16*)value);break;
 case 0x206:case 0x207:case 0x208:case 0x209:{u8 *out;int i;memset(ov13_0224E2E8[type-0x206],0,32);if(ov13_0224E2B0[12]==1){out=ov13_0224E2E8[type-0x206];for(i=0;i<length;i++){out+=ov13_02226D0C((char*)out,*(s8*)value++);}}else memcpy(ov13_0224E2E8[type-0x206],value,length);break;}
 case 0x20a:memset(ov13_0224E380,0,sizeof(ov13_0224E380));memcpy(ov13_0224E380,value,length);break;
 }
 value=ov13_02224694(&pos,start+bytes,&type,&length);
 }
 return result;
}
