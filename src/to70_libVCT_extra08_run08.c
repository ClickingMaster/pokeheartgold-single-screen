#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct Packet { u32 a; struct Packet *prev,*next; u8 pad[5]; u8 channel; u8 pad2[6]; u32 seq; } Packet;
typedef struct Session { u32 state; u8 aid,other,pad[2]; u32 mask,mode,pad2; struct Session *next; } Session;
typedef struct Vct { u8 aid,pad[3]; void (*callback)(u32,u32,Session*,u32); u32 context; int state,mode,flag; } Vct;
typedef struct Ssp { u32 count,mask; u64 tick; Session *free,*used; u64 (*clock)(void); u32 seed,add; u8 pad[24]; u32 active; u8 aid; u8 pad2[7]; u32 extra; } Ssp;
extern Packet *_021D6F58[2]; extern u32 _021D6F60[3],_021D6F78[3]; extern Packet *_021D6F6C[3],*_021D6F84[3];
typedef struct Config {Session *data;u32 count;} Config;
extern Vct __vct; extern Ssp _021D6F90; extern Session _021D6FCC,_021D6FB4; extern u64 sTransceiverLimit; extern int ulaw_segment[8];
extern void vct_free_packet_buffer(void *); extern Packet *vct_get_packet_from_decoder(u32); extern u32 vct_remove_packet_from_decoder(Packet*);
extern u32 vct_pop_audio_queue(u32); extern void vct_init_sequence(u32*,u32); extern Session *vct_create_session_impl(u32); extern void vct_init_session(Session*,u32);
extern int vct_request_impl(Session*,int),vct_response_impl(Session*,int),vct_check_send_state(Session*,u8*),vct_check_multisession(Session*),vct_send_ssp(Session*,u8*);
extern void vct_build_ssp_header(Session*,u8*,u32); extern Session *vct_find_session(u32);
extern int vct_response_reject(u8*,u32),vct_handle_ssp_client(u8*,u32),vct_handle_ssp_server(u8*,u32),DWC_SendReliable(u32,void*,u32);
extern void vct_send_notify(u32); extern int vct_send_notify_impl(Session*,u8*);

void vct_flush_decoder(u32 ch);
void vct_flush_all_decoder(void);
u32 vct_insert_decoder(Packet *p);
Packet *vct_top_decoder(void);
u32 vct_count_decoder_queue(u32 ch);
void vct_encode_g711_ulaw(u8*out,s16*in,u32 count);
void vct_decode_g711_ulaw(u8*in,s16*out,u32 n);
void vct_encode_8bit_raw(u8*out,s16*in,u32 n);
void vct_decode_8bit_raw(s8*in,s16*out,u32 n);
void vct_init_audio_queue(u32 ch);
void vct_flush_audio_queue(u32 ch);
void vct_flush_all_audio_queue(void);
int vct_insert_audio_queue(Packet*p,u32 ch);
Packet *vct_top_audio_queue(u32 ch);
u32 vct_count_audio_queue(u32 ch);
void vct_init_stream(u32*s,u32 ch,u32 mode);
void vct_reset_stream(u32*s);
void vct_save_previous_buffer(u32*s,Packet*p);
int vct_update_sequence(u32*s,Packet*p,u32 seq);
Session *VCT_CreateSession(u32 aid);
int VCT_DeleteSession(Session*s);
int vct_build_request_data(Session*s,int kind,u8*b,u32 n);
int VCT_Request(Session*s,int kind);
int vct_build_response_data(Session*s,int kind,u8*b,u32 n);
int VCT_Response(Session*s,int kind);
int VCT_AddConferenceClient(u8 aid);
void vct_finish_ssp(void);
void vct_check_timeout(void);
int vct_handle_ssp(u32 aid,u8*b,u32 n,u32 arg);
void vct_add_notify_aid_list(Session*s,u8*b);


int vct_init_ssp(Config *cfg);

void vct_decode_g711_ulaw(u8*in,s16*out,u32 n){for(u32 i=0;i<n;i++){int u=~in[i];int t=((u&15)<<3)+0x84;t<<=(u&0x70)>>4;out[i]=(u&0x80)?0x84-t:t-0x84;}}
void vct_encode_8bit_raw(u8*out,s16*in,u32 n){for(u32 i=0;i<n;i++)out[i]=in[i]>>8;}
void vct_decode_8bit_raw(s8*in,s16*out,u32 n){for(u32 i=0;i<n;i++)out[i]=in[i]<<8;}
