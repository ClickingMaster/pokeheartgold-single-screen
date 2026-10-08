#pragma force_active on
#include "global.h"
typedef struct {u32 key;u16 value,duration;u16 pad;u8 active,padB;} Record;
typedef struct {Record records[8];u8 state;} Context;
typedef struct {u32 sender:8,level:4,reserved:20; u32 target;} Control;
typedef struct {u16 pad,error;u32 pad4;u16 kind;u8 data[6];u16 duration,pad12;Control control;} Message;
void ov74_0223144C(u16);
void *ov74_0223115C(void);
void ov74_02231070(int);
BOOL ov74_0222FE78(void);
BOOL ov74_02231670(void);
void ov74_0222FE4C(void);
void ov74_0223110C(int);
void ov74_02231458(void*);
u8 ov74_022311AC(void);u32 ov74_022311BC(void),ov74_022311CC(void);
u32 ov74_0222FE5C(void*);
int ov74_0222FE68(void*);
void ov74_0222FEA0(u16);
void ov74_02231448(void);
void ov74_02230138(Message*);
typedef struct{u32 key;}PeerKey;
typedef struct{u16 value;}PeerValue;
static inline PeerKey DecodeKey(Message*m){PeerKey decoded;decoded.key=ov74_0222FE5C(m->data);return decoded;}
static inline PeerValue DecodeValue(Message*m){PeerValue decoded;decoded.value=ov74_0222FE68(m->data);return decoded;}
static inline BOOL CheckControl(Control *c){
  return c->sender==ov74_022311AC() && c->level<=ov74_022311BC() && (c->target>>16)==ov74_022311CC();
}
// @fn ov74_02230138
void ov74_02230138(Message *m){
 Context *ctx;
 ov74_0223144C(m->kind);
 if(!m->error){
  ctx=ov74_0223115C();ov74_02231070(8);
  if(ov74_0222FE78())return;
  if(ctx->state==1)ctx->state=2;
  switch(m->kind){
  case 0:if(!ov74_02231670())ov74_0222FE4C();else ov74_0223110C(0);break;
  case 2:break;
  case 7:
   ov74_02231458(ov74_0223115C());
   if(ctx->state==2){Control *ctl=&m->control;
    if(ctl->sender==ov74_022311AC() && ctl->level<=ov74_022311BC() && (ctl->target>>16)==ov74_022311CC()){
    ctx=ov74_0223115C();PeerKey key=DecodeKey(m);PeerValue value=DecodeValue(m);int i;
    for(i=0;i<8;i++){u32 k=ctx->records[i].key;if(k==key.key){ctx->records[i].duration=m->duration;ctx->records[i].active=1;return;}if(!k){ctx->records[i].key=key.key;ctx->records[i].value=value.value;ctx->records[i].duration=m->duration;ctx->records[i].active=1;return;}}
   }}break;
  case 9:ov74_0222FEA0(m->duration);break;
  }
 }else{ov74_02231448();ov74_0222FE4C();}
}
