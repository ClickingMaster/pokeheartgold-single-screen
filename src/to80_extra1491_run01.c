#pragma force_active on
#include "global.h"
typedef struct{u8 low:4,high:4;} Flag;
typedef struct{u8 p[100];u32 word;} Mon;
typedef struct{u32 unk,state;u32 f8;u32 pad;u32 f10;u8 p[16];Mon*mon;u32 pad28,f2c,pad30;Flag*flag;u32 f38;u8 p2[20];u32 f50;u32 pad54,f58;} Root;
typedef struct{u8 p[28];u8 state,next;u8 p1[2];u32 total;u32 pad;void*dest;u32 pad2;u16 remaining;u8 p3[4];u16 n36;u16 pad38,n3a;} State;
extern Root ov112_021FFAA4;extern State ov112_021FFB24;
extern void ov112_021E6BF8(void),ov112_021E5A68(int,int,int,int),ov112_021E5E18(int),ov112_021E5A5C(void),ov112_021E5EC4(u32,u32,u32),ov112_021E5EEC(void),ov112_021E6134(void),ov112_021E6004(void),ov112_021E6164(void);void ov112_021E6F60(void*,u32,int);
void ov112_021E6F60(void*data,u32 size,int op){switch(op){
case 34:{u32 v;MI_CpuCopy8(data,ov112_021FFAA4.mon,104);v=ov112_021FFAA4.mon->word;ov112_021FFAA4.mon->word=(u8)(v>>24)|((v>>8)&0xff00)|((v<<8)&0xff0000)|((v<<24)&0xff000000);ov112_021E6BF8();}break;
case 38:switch(ov112_021FFB24.state){
case 50:ov112_021E5A68(0,0,56,1);break;
case 64:ov112_021E5A68(0,0,78,1);ov112_021E5E18(15);ov112_021E5A5C();break;
case 82:ov112_021E5A68(0,0,90,1);break;
case 96:ov112_021E5A68(0,0,102,1);ov112_021E5E18(15);ov112_021E5A5C();break;
case 176:ov112_021E5A68(0,0,192,1);break;
case 178:ov112_021E5A68(0,0,194,1);break;
case 180:ov112_021E5A68(0,0,196,1);break;
case 182:ov112_021E5A68(0,0,198,1);break;
case 184:case 186:case 188:case 190:if(ov112_021FFAA4.flag->low==15){switch(ov112_021FFAA4.state){case 0:ov112_021E5A68(0,0,208,1);break;case 1:ov112_021E5A68(0,0,210,1);break;case 2:ov112_021E5A68(0,0,212,1);break;case 3:ov112_021E5A68(0,0,214,1);break;}}else ov112_021E5A68(0,0,ov112_021FFB24.state,1);break;}break;
case 158:ov112_021E5E18(12);ov112_021E5A5C();break;
case 156:ov112_021E5E18(12);ov112_021E5A5C();break;
case 40:ov112_021E5E18(3);ov112_021E5A5C();break;
case 52:ov112_021FFB24.next=58;if(ov112_021FFAA4.f50)ov112_021E5EC4(ov112_021FFAA4.f50,640,0x8c50);else ov112_021E5EC4(ov112_021FFAA4.f10,640,0x8c50);ov112_021E5EEC();break;
case 56:ov112_021E5E18(15);ov112_021E5A5C();break;
case 66:ov112_021FFB24.next=70;ov112_021E5EC4(0x8f00,ov112_021FFAA4.f8,42);ov112_021E6134();break;
case 68:ov112_021E5E18(14);ov112_021E5A5C();break;
case 84:ov112_021FFB24.next=60;if(ov112_021FFAA4.f58)ov112_021E5EC4(ov112_021FFAA4.f58,0xd700,0x28be);else ov112_021E5EC4(ov112_021FFAA4.f2c,0xd700,0x28be);ov112_021E5EEC();break;
case 90:ov112_021E5E18(15);ov112_021E5A5C();break;
case 94:ov112_021E5E18(15);ov112_021E5A5C();break;
case 98:ov112_021FFB24.next=72;ov112_021E5EC4(0xce80,ov112_021FFAA4.f38,0xd4c);ov112_021E6134();break;
case 100:ov112_021E5E18(14);ov112_021E5A5C();break;
case 160:case 162:case 164:case 166:case 168:case 170:case 172:case 174:ov112_021FFB24.next=72;ov112_021E5EC4(0xce80,ov112_021FFAA4.f38,0xd4c);ov112_021E6134();break;
case 192:case 194:case 196:case 198:case 200:case 202:case 204:case 206:ov112_021E5E18(15);ov112_021E5A5C();break;
case 240:ov112_021E5E18(15);ov112_021E5A5C();break;
case 254:ov112_021E5E18(15);ov112_021E5A5C();break;
case 226:ov112_021E5E18(15);ov112_021E5A5C();break;
case 224:ov112_021E5E18(15);ov112_021E5A5C();break;
case 4:if(ov112_021FFB24.remaining==0)ov112_021E6004();else ov112_021E5EEC();break;
case 14:MI_CpuCopy8(data,ov112_021FFB24.dest,size);ov112_021FFB24.total+=size;ov112_021FFB24.dest=(u8*)ov112_021FFB24.dest+size;ov112_021FFB24.remaining-=size;ov112_021FFB24.n36++;ov112_021FFB24.n3a++;if(ov112_021FFB24.remaining==0)ov112_021E6164();else ov112_021E6134();break;
case 244:break;}}
