#pragma force_active on
#include "global.h"
typedef struct{u32 value;u8 p[0x78-4];u16 a,b;u32 c,d;u8 q[4];} Record;
typedef struct{u32 a,b;u16 p,field;u8 q[0x70-12];u32 words[7];Record records[24];} Packet;
typedef struct{u8 low:4;u8 lock0:1;u8 lock1:1;u8 lock2:1;u8 lock3:1;} Flags;
typedef struct{Flags flags;u8 p[3];u8 bytes[360];u8 block364[548];u8 q[1344-912];u8 block1344[392];} Shared;
typedef struct{u32 p,index;u8 q[4];void*buffer;u8 r[0x30-16];u8*base;Shared*shared;Packet*packet;u64 tick;} Global;
typedef struct{u8 p[28];u8 op,state;} Command;
extern Global ov112_021FFAA4;extern Command ov112_021FFB24;
extern float _f_ulltof(u64);
extern void ov112_021E5EC4(void*,u32,s32),ov112_021E6134(void),ov112_021E5A68(int,int,int,int),ov112_021E5EEC(void);
#define SW16(v) (((u32)(u8)((v)>>8))|(((v)<<8)&0xff00))
#define SW32(v) (((u32)(u8)((v)>>24))|(((v)>>8)&0xff00)|(((v)<<8)&0xff0000)|(((v)<<24)&0xff000000))
#define LOAD_NEXT() switch(ov112_021FFAA4.index){case 0:ov112_021FFB24.state=176;ov112_021E5EC4((void*)(ov112_021FFAA4.shared->bytes),(u32)((void*)0xb804),576);break;case 1:ov112_021FFB24.state=178;ov112_021E5EC4((void*)((void*)((s32)ov112_021FFAA4.shared+580)),(u32)((void*)0xba44),764);break;case 2:ov112_021FFB24.state=180;ov112_021E5EC4((void*)(ov112_021FFAA4.shared->block1344),(u32)((void*)0xbd40),392);break;case 3:ov112_021FFB24.state=182;ov112_021E5EC4((void*)(ov112_021FFAA4.buffer),(u32)((void*)0xbf00),3260);break;}
#define CHECK_LOCKS() switch(ov112_021FFAA4.index){case 0:if(ov112_021FFAA4.shared->flags.lock0){ov112_021E5A68(0,0,156,1);return;}break;case 1:if(ov112_021FFAA4.shared->flags.lock1){ov112_021E5A68(0,0,156,1);return;}break;case 2:if(ov112_021FFAA4.shared->flags.lock2){ov112_021E5A68(0,0,156,1);return;}break;case 3:if(ov112_021FFAA4.shared->flags.lock3){ov112_021E5A68(0,0,156,1);return;}break;}
#define HANDSHAKE(bit,mask) CHECK_LOCKS();{u8 flags=ov112_021FFAA4.shared->flags.low;if((flags & mask)==mask){flags|=bit;ov112_021FFAA4.shared->flags.low=flags;LOAD_NEXT();ov112_021E5EEC();return;}if(flags&bit){ov112_021E5A68(0,0,158,1);return;}ov112_021FFB24.state=ov112_021FFB24.op;ov112_021E5A68(0,0,36,1);return;}
void ov112_021E6164(void);
void ov112_021E6164(void){int i,offset;u32*words;Record*rec;float elapsed=_f_ulltof(((OS_GetTick()-ov112_021FFAA4.tick)<<6)/33514)/1000.0f;(void)elapsed;
switch(ov112_021FFB24.state){case 70:ov112_021FFB24.state=72;ov112_021E5EC4((void*)((void*)0xce80),(u32)(ov112_021FFAA4.packet),3404);ov112_021E6134();return;
case 72:ov112_021FFAA4.packet->field=SW16(ov112_021FFAA4.packet->field);for(i=0,offset=0;i<7;i++,offset+=4){words=(u32*)((u8*)ov112_021FFAA4.packet+offset+0x70);*words=SW32(*words);}ov112_021FFAA4.packet->a=SW32(ov112_021FFAA4.packet->a);ov112_021FFAA4.packet->b=SW32(ov112_021FFAA4.packet->b);for(i=0,rec=ov112_021FFAA4.packet->records;i<24;i++,rec++){rec->value=SW32(rec->value);rec->a=SW16(rec->a);rec->b=SW16(rec->b);rec->c=SW32(rec->c);rec->d=SW32(rec->d);}switch(ov112_021FFB24.op){case 176:case 178:case 180:case 182:case 184:case 186:case 188:case 190:ov112_021FFB24.state=74;ov112_021E5EC4((void*)((void*)0xb800),(u32)(ov112_021FFAA4.shared),4);ov112_021E6134();return;case 96:ov112_021FFB24.state=76;ov112_021E5EC4((void*)((void*)0xde24),(u32)(ov112_021FFAA4.base+548),5480);ov112_021E6134();return;default:ov112_021FFB24.state=74;ov112_021E5EC4((void*)((void*)0xb800),(u32)(ov112_021FFAA4.shared),1736);ov112_021E6134();return;}
case 74:switch(ov112_021FFB24.op){case 82:ov112_021E5A68(0,0,36,1);return;case 64:ov112_021FFB24.state=76;ov112_021E5EC4((void*)((void*)0xde24),(u32)(ov112_021FFAA4.base+548),5480);ov112_021E6134();return;
case 176:if(ov112_021FFAA4.shared->flags.lock0){ov112_021E5A68(0,0,156,1);return;}ov112_021FFB24.state=176;ov112_021E5EC4((void*)((u8*)ov112_021FFAA4.shared+4),(u32)((void*)0xb804),576);ov112_021E5EEC();return;
case 178:if(ov112_021FFAA4.shared->flags.lock1){ov112_021E5A68(0,0,156,1);return;}ov112_021FFB24.state=178;ov112_021E5EC4((void*)((void*)((s32)ov112_021FFAA4.shared+580)),(u32)((void*)0xba44),764);ov112_021E5EEC();return;
case 180:if(ov112_021FFAA4.shared->flags.lock2){ov112_021E5A68(0,0,156,1);return;}ov112_021FFB24.state=180;ov112_021E5EC4((void*)(ov112_021FFAA4.shared->block1344),(u32)((void*)0xbd40),392);ov112_021E5EEC();return;
case 182:if(ov112_021FFAA4.shared->flags.lock3){ov112_021E5A68(0,0,156,1);return;}ov112_021FFB24.state=182;ov112_021E5EC4((void*)(ov112_021FFAA4.buffer),(u32)((void*)0xbf00),3260);ov112_021E5EEC();return;
case 184:HANDSHAKE(1,14);case 186:HANDSHAKE(2,13);case 188:HANDSHAKE(4,11);case 190:HANDSHAKE(8,7);default:OS_Halt();return;}
case 76:ov112_021E5A68(0,0,36,1);return;default:return;}}
