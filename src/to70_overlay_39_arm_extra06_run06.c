#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#include <string.h>
#pragma force_active on
#define W(n) (*(int*)(_0222AB84+(n)))
extern u8 _0222AB84[],ov39_0222AD84[],ov39_0222B6D0[],ov39_0222AA7C[],ov39_0222AA78[];
extern char ov39_0222ACC4[],ov39_0222AA5C[];
extern OSMutex ov39_0222AEB8;
extern OSThread ov39_0222ADF8;
typedef struct{u8 bytes[4];} B4;
typedef union{int n;u8 bytes[4];} U4;
typedef struct{u32 x[78];} Z312;
typedef struct{B4 size;u16 cmd,pad;Z312 config;} Packet;
typedef struct{int x[12];} B48;
typedef struct{int type,len;void*p;u32**addr;} Host;
typedef struct{u8 len,family;u16 port;u32 addr;} Addr;
extern u8 _0222A9AC[];
extern Host*SOC_GetHostByName(char*);
typedef struct{u32 addr;} InAddr;

extern char*SOC_InetNtoA(InAddr);
extern int SOC_Connect(int,Addr*);
extern int SOC_Send(int,void*,int,int);
extern int SOC_Recv(int,void*,int,int);
extern int ov39_02229FCC(void);
extern int ov39_02229E40(void);
typedef struct{u16 a,b;int n,h;} I12;
typedef struct{u32 x[4];} B16;
typedef struct{u32 x[56];} B224;
typedef struct{u32 x[135];} B540;
typedef struct{int n;B540 b;} P544;
typedef struct{u32 x[9];} B36;
typedef struct{u32 x[1875];} B7500;
typedef struct{u16 a,c;B16 b;int tail[2];} P28;
typedef struct{u16 a,c;int data[6];} Q28;
extern void* ov00_021EC2A8(int,int);
extern void ov00_021EC2EC(int,void*,int);
extern int ov39_02229630(int,void*,void*,int);
extern int ov39_0222A6DC(int);
extern void ov39_02229474(void);
extern int ov39_02229A6C(char*,char*);
extern int ov00_021FFA44(char*);
extern int SOC_Socket(int,int,int);
extern int SOC_Fcntl(int,int,int);
extern int SOC_Shutdown(int,int);
extern int SOC_Close(int);
void ov39_02229310(void);
int ov39_02229374(char*p,int n,Z312*q);
void ov39_02229408(void);
int ov39_0222981C(void);
int ov39_0222982C(void);
int ov39_02229864(void);
int ov39_02229898(void);
void* ov39_022298CC(int n);
void ov39_022298E0(void*p);
void ov39_022298FC(void);
void ov39_02229910(void);
int ov39_02229924(void);
int ov39_0222993C(void);
int ov39_02229944(void);
void ov39_02229970(void);
void ov39_02229974(void);
int ov39_02229980(void);
int ov39_022299D4(signed char*p);
int ov39_02229AD4(void);
void ov39_02229BA4(void);
void ov39_02229FC0(u32 n);
int ov39_0222A110(Z312*p);
void ov39_0222A130(void);
int ov39_0222A13C(void);
int ov39_0222A158(void);
int ov39_0222A164(B224*p,void*out);
int ov39_0222A1C0(int n,void*out);
int ov39_0222A268(int n,void*out);
int ov39_0222A200(int n,B540*p,void*out);
int ov39_0222A2A8(void);
int ov39_0222A2B4(void);
int ov39_0222A2C0(int n);
int ov39_0222A2CC(void*p);
int ov39_0222A2EC(B36*p,void*out);
int ov39_0222A33C(B7500*p,void*out);
int ov39_0222A394(B16*p,void*out);
int ov39_0222A3DC(int*p,void*out);
int ov39_0222A434(int*p,void*out);
int ov39_0222A48C(int a,int b,int c,void*out);
int ov39_0222A4C0(int a,int b,void*out);
void ov39_0222A4EC(u8*p,int n,u8*out);
void ov39_0222A544(u8*p,int n,u8*out);
int ov39_0222A59C(int n);
void ov39_0222A8AC(void*p,void*q);
int ov39_02229A6C(char*p,char*out);
int ov39_02229BE4(void);
int ov39_02229D2C(void);
void ov39_02229474(void);
int ov39_0222A6DC(int n);
int ov39_02229E40(void);
int ov39_02229630(int cmd,void*in,void*out,int n);

int ov39_0222A59C(int n){int size;switch(n){case 0:size=0x400;break;case 0x4e20:size=224;break;case 0x4e21:size=12;break;case 0x5208:size=544;break;case 0x5209:size=12;break;case 0x55f0:size=0;break;case 0x55f1:size=36;break;case 0x59d8:size=7500;break;case 0x59d9:size=28;break;case 0x59da:size=12;break;case 0x59db:size=8;break;default:return -1;}return size+0x140;}

int ov39_0222A6DC(int n){B48 list=*(B48*)_0222A9AC;int size;switch(n){case -1:{int i=0;size=-1;while(1){int v=list.x[i];if(v==-1)break;v=ov39_0222A6DC(v);if(v>size)size=v;i++;}if(size==-1)return -1;break;}case 0:size=0x400;break;case 0x4e20:size=8;break;case 0x4e21:size=0x93c;break;case 0x5208:size=8;break;case 0x5209:size=0x2b74;break;case 0x55f0:size=4;break;case 0x55f1:size=0x630;break;case 0x59d8:size=8;break;case 0x59d9:size=0x1c24;break;case 0x59da:size=0x1d58;break;case 0x59db:size=0;break;default:return -1;}return size+8;}

void ov39_0222A8AC(void*p,void*q){}
