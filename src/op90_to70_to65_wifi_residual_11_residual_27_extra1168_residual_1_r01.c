#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
extern int chars_till_end(s8*);

int compare_fqdn(s8*a,s8*b);

typedef struct {u32 tag;int length;void *modulus;int explen;void *exponent;} Key;
typedef struct W39100Ctx W39100Ctx;
struct W39100Ctx {u8 pad[0x455];u8 state;u8 pad2[2];int error,valid;u8 pad3[0x20];Key key;u8 keyStorage[0x114-sizeof(Key)];u32 items;u8 pad4[8];void*root;u8 pad5[8];u8 f5ac,f5ad,f5ae,f5af,f5b0;u8 pad6[0xff];u8 flag;u8 pad7[0xff];s8 name[0x50];s8*host;u8*certificate;u32 length,date;u32(*callback)(u32,W39100Ctx*,int);};
extern RTCResult RTC_GetDate(RTCDate*);extern int cert_item(W39100Ctx*,u8**,int,int,int);extern u32 auth_cert(W39100Ctx*);extern int validate_signature(u8*,Key*);
void rcv_certificate(W39100Ctx*,u8*);
// @fn rcv_certificate
void rcv_certificate(W39100Ctx*c,u8*data){RTCDate date;u32 mode; u32 result; u32 length; u32 left; int index;left=data[2]+(((data[0]<<8)+data[1])<<8);data+=3;c->valid=-1;RTC_GetDate(&date);c->date=(((u32)date.year+2000)<<16)+((u32)date.month<<8)+date.day;mode=0;c->flag=0;c->root=0;c->items=0;index=0;for(;;){length=data[2]+(((data[0]<<8)+data[1])<<8);data+=3;left-=length+3;c->error=-1;c->f5ad=0;c->f5ac=0;c->f5af=0;c->flag=0;c->f5b0=0;c->name[0]=0;c->certificate=data;c->length=length;if(cert_item(c,&data,0,0,mode)!=0||c->items<51||c->root==0){c->state=9;return;}result=auth_cert(c);if(index==0&&c->host!=0){if(compare_fqdn(c->host,c->name)!=0)result|=0x4000;}mode=result&255;if(mode==1&&left!=0){u8*next=data+3;c->f5ad=0;if(cert_item(c,&next,0,0,2)!=0){c->state=9;return;}result=(result&~255)|validate_signature((u8*)c,&c->key);}if(c->callback)result=c->callback(result,c,index);index++;if(mode==0||result!=0||left==0)break;mode=1;}if(result==0)c->state=3;else c->state=9;}
