#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct GPIConnection GPIConnection;
typedef GPIConnection *GPConnection;
typedef void (*GPCallback)(GPConnection *,void *,void *);
typedef struct GPConnectResponseArg {int result,profile,unk8,unkC,unk10,unk14,unk18,unk1C;} GPConnectResponseArg;
typedef union W41287Account {u8 bytes[64];struct {u8 prefix[0x1c];u32 profile;u8 tail[0x40-0x20];} fields;} W41287Account;
typedef struct LoginState {GPConnection *connection;int state;u32 product,gameCode;void *text;void (*callback)(int,int,void *);void *arg;W41287Account *account;u8 reserved20[0x30-0x20];int pending;u8 reserved34[0x260-0x34];GPConnectResponseArg message;} LoginState;
extern struct {u32 zero;LoginState *state;u32 reserved8;} ov00_0221A90C;
extern char ov00_02217A04[];
extern int ov00_021F04A0(int,char *,char *),ov00_021EF494(u32),ov00_021EE584(void),ov00_021F179C(int);
extern void ov00_021EF3C0(int,int);
extern void ov00_021EFAC8(GPConnection *,void *,void *);
extern int ov00_02200020(GPConnection *,int,int,int,GPCallback,void *);
void ov00_021EF508(GPConnection *,void *,void *);
void ov00_021EF508(GPConnection *connection,void *reply,void *parameter){
 GPConnectResponseArg *message=reply;
 ov00_0221A90C.state->pending=0;
 if(message->result==0){
  {GPConnectResponseArg *source=message;GPConnectResponseArg *dest=&ov00_0221A90C.state->message;*dest=*source;}
  if(ov00_0221A90C.state->state==2){
   if(ov00_0221A90C.state->account->fields.profile==message->profile){
    int error;
    ov00_0221A90C.state->state=5;
    error=ov00_021F04A0(1,ov00_02217A04,0);
    if(ov00_021EF494(error))return;
    ov00_0221A90C.state->callback(0,message->profile,ov00_0221A90C.state->arg);
    if(ov00_021EE584())return;
    if(ov00_021F179C(message->profile))return;
   }else ov00_021EF3C0(6,-60000);
  }else if(ov00_0221A90C.state->state==3){
   int error=ov00_02200020(connection,message->profile,0,0,ov00_021EFAC8,0);
   if(ov00_021EF494(error))return;
  }
 }else ov00_021EF494(message->result);
}
