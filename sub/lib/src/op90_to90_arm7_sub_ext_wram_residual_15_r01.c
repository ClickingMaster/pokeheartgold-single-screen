#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef union W40887State {u32 words[512];u16 halves[1024];u8 bytes[2048];} W40887State;
typedef struct W40887Scan {u16 words,rssi,mac[3],ssidLength;u8 ssid[32];u16 capability,support,extended,beacon,dtim,channel,cf,reserved,vendorLength,count;} W40887Scan;
typedef union W40887Parent {u32 words[52];u16 halves[104];u8 bytes[208];struct {u8 prefix[16];W40887Scan scan;u32 gap,gid;u16 group;u8 pad,flags;u16 parentMax,childMax;u8 rest[112];} view;} W40887Parent;
typedef struct W40887Owner {u8 pad[0x54c];W40887Parent *parent;W40887State *status;} W40887Owner;
typedef struct W40887Request {u32 command;const void *parent;u8 user[24];u32 power;u16 other,auth;} W40887Request;
typedef struct W40887Descriptor {u16 data[32];u32 reserved;} W40887Descriptor;
extern u8 wmspW[];
extern u16 *__VENEER_WMSP_GetBuffer4Callback2Wm9(void);
extern void __VENEER_WMSP_ReturnResult2Wm9(void *),__VENEER_MI_CpuCopy8(const void *,void *,u32),__VENEER_MIi_CpuClear16(u32,void *,u32);
extern u32 __VENEER_OS_DisableInterrupts(void);extern void __VENEER_OS_RestoreInterrupts(u32);
extern u64 __VENEER_OS_GetTick(void);
extern int WMSP_SetAllParams(u16,void *),__VENEER__s32_div_f(int,int);
extern void WmspError_060045D0(u16,u16,u16);
extern u16 *WMSP_WL_ParamSetNullKeyResponseMode(void *,u16),*WMSP_WL_ParamSetBeaconLostThreshold(void *,u16),*WMSP_WL_DevClass1(void *),*WMSP_WL_MlmePowerManagement(void *,u16,u16,u16),*WMSP_WL_MlmeJoin(void *,u16,const void *),*WMSP_WL_MlmeAuthenticate(void *,const void *,u16,u16),*WMSP_WL_MlmeAssociate(void *,const void *,u16,u16);
extern u16 WMSP_GetLinkLevel(u16);
extern void WMSP_FillRssiIntoList(u16),WMSP_SetParentMaxSize(u16),WMSP_SetChildMaxSize(u16);
void WMSP_StartConnectEx(W40887Request *);
// @fn WMSP_StartConnectEx
void WMSP_StartConnectEx(W40887Request *input){
    u16 command[256];u16 *output=command;
    W40887Descriptor descriptor;
    u8 authMac[6],associateMac[6];
    /*D*/W40887Owner *owner=(W40887Owner *)(wmspW+0x1000);W40887Parent *parent=owner->parent;u32 irq;W40887State *status;W40887Request *req=input;u16 *result;u16 *callback;u16 power;u32 inner;u16 lost;u8 rssi;/*E*/
    status=owner->status;
    if(status->halves[0]!=2 || (status->words[0xc8/4]&1)){
        callback=__VENEER_WMSP_GetBuffer4Callback2Wm9();callback[0]=12;callback[1]=3;callback[4]=6;__VENEER_WMSP_ReturnResult2Wm9(callback);return;
    }
    __VENEER_MI_CpuCopy8(req->parent,parent->bytes+16,192);
    if(parent->view.scan.vendorLength>=16 && !(parent->view.flags&1)){
        callback=__VENEER_WMSP_GetBuffer4Callback2Wm9();callback[0]=12;callback[1]=11;callback[4]=6;__VENEER_WMSP_ReturnResult2Wm9(callback);return;
    }
    {int bit=1<<parent->view.scan.channel;
    if(!(bit&status->halves[0x1f4/2]) || !(0x1fff&(bit>>1))){
        callback=__VENEER_WMSP_GetBuffer4Callback2Wm9();callback[0]=12;callback[1]=6;callback[4]=6;__VENEER_WMSP_ReturnResult2Wm9(callback);return;
    }}
    callback=__VENEER_WMSP_GetBuffer4Callback2Wm9();callback[0]=12;callback[1]=0;callback[4]=6;__VENEER_WMSP_ReturnResult2Wm9(callback);
    if(status->halves[0x1ec/2]==1){if(parent->view.scan.support&1)status->halves[0x1ec/2]=1;else status->halves[0x1ec/2]=2;}
    else {if(parent->view.scan.support&2)status->halves[0x1ec/2]=2;else status->halves[0x1ec/2]=1;}
    if(parent->view.scan.capability&32)status->halves[0x1ee/2]=1;else status->halves[0x1ee/2]=0;
    if(parent->view.scan.vendorLength==0)status->halves[0xe6/2]=3;else status->halves[0xe6/2]=2;
    if(!WMSP_SetAllParams(12,output))return;
    result=WMSP_WL_ParamSetNullKeyResponseMode(output,0);
    if(result[2]){WmspError_060045D0(0x216,result[2],0);return;}
    if(parent->view.scan.vendorLength<16){
        lost=(parent->view.scan.beacon==0)?1:__VENEER__s32_div_f(10000,parent->view.scan.beacon)+1;
        if(lost>255)lost=255;
        result=WMSP_WL_ParamSetBeaconLostThreshold(output,lost);
        if(result[2]){WmspError_060045D0(0x20b,result[2],0);return;}
    }
    result=WMSP_WL_DevClass1(output);
    if(result[2]){WmspError_060045D0(0x303,result[2],0);return;}
    status->halves[0]=3;
    power=req->power?1:0;
    result=WMSP_WL_MlmePowerManagement(output,power,0,1);
    if(result[2]){WmspError_060045D0(1,result[2],0);return;}
    status->halves[0xc6/2]=power;
    __VENEER_MI_CpuCopy8(parent->bytes+16,&descriptor,64);
    if(status->halves[0xe6/2]==2){
        descriptor.data[5]=32;descriptor.data[6]=parent->view.gid;descriptor.data[7]=parent->view.gid>>16;descriptor.data[8]=parent->view.group;descriptor.data[9]=0;
        __VENEER_MI_CpuCopy8(req->user,descriptor.data+10,24);
    }
    result=WMSP_WL_MlmeJoin(output,2000,&descriptor);
    if(result[2]||result[3]){WmspError_060045D0(3,result[2],result[3]);return;}
    __VENEER_MI_CpuCopy8(result+4,status->bytes+0x18a,6);
    __VENEER_MI_CpuCopy8(status->bytes+0x18a,authMac,6);
    result=WMSP_WL_MlmeAuthenticate(output,authMac,req->auth,2000);
    if(result[2]==12 && result[3]==19){callback=__VENEER_WMSP_GetBuffer4Callback2Wm9();callback[0]=12;callback[1]=12;callback[4]=6;__VENEER_WMSP_ReturnResult2Wm9(callback);return;}
    if(result[2]||result[3]){WmspError_060045D0(4,result[2],result[3]);return;}
    __VENEER_MI_CpuCopy8(status->bytes+0x18a,associateMac,6);
    result=WMSP_WL_MlmeAssociate(output,associateMac,1,2000);
    irq=__VENEER_OS_DisableInterrupts();
    if(result[2]==12 && result[3]==19){__VENEER_OS_RestoreInterrupts(irq);callback=__VENEER_WMSP_GetBuffer4Callback2Wm9();callback[0]=12;callback[1]=12;callback[4]=6;__VENEER_WMSP_ReturnResult2Wm9(callback);return;}
    if(result[2]||result[3]){__VENEER_OS_RestoreInterrupts(irq);WmspError_060045D0(6,result[2],result[3]);return;}
    status->halves[0x188/2]=result[4];status->halves[0xba/2]=parent->view.group;
    __VENEER_MIi_CpuClear16(1,status->bytes+0x1f8,16);
    rssi=parent->view.scan.rssi&255;if(!(rssi&2))rssi=(rssi>>2)+25;else rssi=rssi>>2;
    status->halves[0xbc/2]=WMSP_GetLinkLevel(rssi);WMSP_FillRssiIntoList(rssi);
    inner=__VENEER_OS_DisableInterrupts();status->halves[0x182/2]=1;status->halves[0x86/2]=1;
    if(*(u64*)(status->bytes+0x7b8)!=0){u64 tick=__VENEER_OS_GetTick()|1;status->words[0x738/4]=(u32)tick;status->words[0x73c/4]=(u32)(tick>>32);}
    status->halves[0]=8;
    WMSP_SetParentMaxSize(parent->view.parentMax+((parent->view.flags&4)?42:0));
    WMSP_SetChildMaxSize(parent->view.childMax+((parent->view.flags&4)?6:0));
    __VENEER_OS_RestoreInterrupts(inner);status->halves[0xc2/2]=1;
    callback=__VENEER_WMSP_GetBuffer4Callback2Wm9();callback[0]=12;callback[1]=0;callback[4]=7;callback[5]=status->halves[0x188/2];
    __VENEER_MI_CpuCopy8(status->bytes+0x18a,callback+8,6);callback[11]=status->halves[0x30/2];callback[12]=status->halves[0x32/2];__VENEER_WMSP_ReturnResult2Wm9(callback);
    __VENEER_OS_RestoreInterrupts(irq);
}
