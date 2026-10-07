#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct {u8 len,family;u16 port;u32 ip;} SockAddress;
typedef struct {u8 pad0[0x10];int state;u8 pad14[8];u32 ip;u16 port;u8 pad22[0x16];int error;u8 pad3c[12];int socket,lastError;u8 pad50[0xac];int failed;u8 pad100[0x34];int option;u8 pad138[0x24];int ownPort;u16 customPort;} Request;
extern int ov38_02225320,ov38_02226028;
extern u16 _02226024;
int ov00_021FF804(int,int,int);
int ov00_021FF44C(int,int);
void ov00_021FF48C(int,int);
int ov00_021FF8A0(int,const SockAddress*,int);
int ov00_021FFA60(int);
int ov00_021FF59C(int,int,int*,int*);
void ghiCallProgressCallback(Request*,int,int);
void ov38_0221EC74(Request *r);
#pragma opt_propagation off
void ov38_0221EC74(Request *r) {
    SockAddress addr;
    int writable,readable;
    BOOL result;
    if(r->socket == -1) {
        r->socket=ov00_021FF804(2,1,0);
        if(r->socket == -1) {r->failed=1;r->error=5;r->lastError=ov00_021FFA60(r->socket);return;}
        if(!ov00_021FF44C(r->socket,0)) {r->failed=1;r->error=5;r->lastError=ov00_021FFA60(r->socket);return;}
        if(r->option) ov00_021FF48C(r->socket,ov38_02225320);
        {SockAddress *a=&addr;*(u32*)a=0;*(u32*)((u8*)a+4)=0;}addr.family=2;
        if(r->ownPort) {int p=r->customPort;addr.port=((p>>8)&0xff)|((p<<8)&0xff00);}
        else if(ov38_02226028) {int p=_02226024;addr.port=((p>>8)&0xff)|((p<<8)&0xff00);}
        else {int p=r->port;addr.port=((p>>8)&0xff)|((p<<8)&0xff00);}
        addr.ip=r->ip;
        if(ov00_021FF8A0(r->socket,&addr,8)==-1) {
            int e=ov00_021FFA60(r->socket);
            if(e!=-6 && e!=-26 && e!=-76){r->failed=1;r->error=6;r->lastError=e;return;}
        }
    }
    result=ov00_021FF59C(r->socket,0,&writable,&readable)>0;
    if(result == -1 || readable) {
        r->failed=1;r->error=6;
        if(result==0) r->lastError=ov00_021FFA60(r->socket);
        return;
    }
    if(writable){r->state=2;ghiCallProgressCallback(r,0,0);}
}
