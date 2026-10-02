#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
#include <string.h>
// clang-format on
#pragma force_active on
typedef struct Session {s32 mode;u8 id,peer,pad[2];u32 mask;s32 state;u32 other;struct Session *next;} Session;
typedef struct Header {u32 magic;u8 kind,size,type,pad,id,target,notify,peer;u32 mask;} Header;
typedef struct Event {s32 event;Session *session;} Event;
typedef struct Packet {struct Packet *next;u32 flags,other;u8 data[0x46c];} Packet;
typedef struct Sessions {u32 count,mask;u64 tick;u32 timer;Session *head;u8 gap[0x24];Session server;} Sessions;
typedef struct Control {u32 frame,received,active;u8 id,other,pad[2];void (*callback)(u32,s32,Session*,void*);void *arg;s32 status,mode;} Control;
typedef struct VCT {u8 id,other,pad[14];s32 mode;} VCT;
extern VCT __vct;
extern u8 sRequestEvent[],sResponseEvent[];
extern s8 sRecvState[][11],sTransRecvState[][11],sSendState[][11],sTransSendState[][11];
extern Sessions _021D6F90;
extern Session _021D6FCC,_021D6FB4;
extern Packet *_021D6FE4[];
extern u32 E2scales[];
typedef struct VADState {s32 level,index,count;} VADState;
extern VADState _021D6FEC;
extern s32 sVADInfo[],_021D7008[],sTap[],sH[];
extern s16 sImpulseResponse[],sDelayLine[];
extern u32 sSpkBuffer[];
extern Control _021D848C;
extern s32 vct_send_notify_impl(Session*,Header*);
extern s32 DWC_SendReliable(u8,const void*,u32);
extern void vct_response_impl(Session*,s32);
extern void vct_finish_audio(void),vct_finish_ssp(void),vct_check_timeout(void);
extern s32 vct_flush_send_buffer(void),vct_decode_audio_buffer(void);
extern void DoFIRFilter(void*,void*,u32,u32);
extern s32 vct_handle_ssp(u32,Header*,u32,Event*);
extern s32 vct_handle_audio(u32,Header*,u32,u64);
extern void vct_init_session(Session*,u8);
extern s32 vct_send_ssp(Session*,Header*);
extern s32 vct_dispatch_session(Header*,Session*);
extern void vct_build_ssp_header(Session*,Header*,u8);
extern s32 vct_check_recv_state(Session*,Header*);
extern s32 vct_check_multisession(Session*);
extern s32 vct_check_send_state(Session*,Header*);
extern Session *vct_find_session(u8);
extern void vct_cleanup_packet_buffer(void);
extern Packet *vct_alloc_packet_buffer(void);
extern void vct_free_packet_buffer(Packet*);
extern void VCT_ResetVAD(void),VCT_GetVADInfo(void*),vct_init_vad(void),InitFIRFilter(void);
extern void vct_set_speaker_samples(void*,u32),vct_process_fir(void*,u32,u32);
extern void vct_mix_audio(s16*,s16*,s16*,u32);
extern void VCT_Cleanup(void),VCT_Main(void);
extern s32 VCT_HandleData(u32,Header*,u32),VCTi_HandleData(u32,Header*,u32,Event*);
extern s32 vct_handle_notify(Header*,Event*),vct_response_reject(Header*,Event*),vct_handle_ssp_client(Header*,Event*);
extern Session *vct_create_session_impl(u8);
extern void VCT_DeleteSession(Session*);
extern s32 vct_init_packet_buffer(void*,u32);
typedef struct InitArgs {u32 a,b;s32 mode;u8 id,pad[3];s32 buffer;u32 size;void (*callback)(u32,s32,Session*,void*);void *arg;} InitArgs;
extern char _version_Abiosso_libVCT[];
extern void OSi_ReferSymbol(const void*);
extern s32 vct_init_ssp(InitArgs*),vct_init_audio(void),VCT_Init(InitArgs*);
extern void vct_send_notify(s32);
extern s32 vct_handle_ssp_server(Header*,Event*);
void vct_set_speaker_samples(void *samples,u32 size){MIi_CpuCopyFast(samples,sSpkBuffer,size);}
