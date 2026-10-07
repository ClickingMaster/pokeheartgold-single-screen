#pragma force_active on
// clang-format off
#include "global.h"
#include "to90_vct_ssp_private.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct AdpcmState { s16 value; u8 index; u8 pad; } AdpcmState;
typedef struct AudioContext {u16 seq; u16 pad; u32 mask; u32 flag; u32 packetSize; AdpcmState state; u32 reset; u32 echo; s32 codec; u32 sessions; u32 size; u32 vad; u8 gap[0xed4]; u32 sendRead,sendWrite,sendBuffer,sendMask,balanceIndex,balance;} AudioContext;
typedef struct Stream {u8 pad[0x94];} Stream;
extern AudioContext _021D6040;

extern u8 sCodecInfo[];
extern s32 sEnableVAD;
extern Session *sAudioSession[];
extern Stream sStream[];
extern u32 _021D6F58[], _021D6F60[];
extern u8 _021D6234[];
extern void vct_decode_adpcm_16(u8*,s16*,u32,AdpcmState*);
extern void vct_decode_adpcm_24(u8*,s16*,u32,AdpcmState*);
extern void vct_decode_adpcm_32(u8*,s16*,u32,AdpcmState*);
extern void vct_encode_adpcm_16(u8*,s16*,u32,AdpcmState*);
extern void vct_encode_adpcm_24(u8*,s16*,u32,AdpcmState*);
extern void vct_encode_adpcm_32(u8*,s16*,u32,AdpcmState*);
extern void VCT_ResetVAD(void);
extern void vct_init_vad(void);
extern void vct_init_audio_queue(u32);
extern void vct_init_stream(Stream*,u32,u32);
extern void vct_init_decoder(void);
extern void InitFIRFilter(void);
extern void vct_reset_stream(Stream*);
extern void vct_flush_audio_queue(u32);
extern void vct_flush_decoder(u32);
extern void *vct_top_decoder(void);
extern s32 vct_count_decoder_queue(u32);
extern u8 *vct_get_packet_from_decoder(u32);
extern void vct_decode_adpcm(u8*,s16*,u32,s32);
extern void vct_decode_g711_ulaw(u8*,s16*,u32);
extern void vct_decode_8bit_raw(u8*,s16*,u32);
extern void vct_remove_packet_from_decoder(void*);
extern s32 vct_insert_audio_queue(void*,u32);
extern void vct_free_packet_buffer(void*);
extern s32 VCTi_ReceiveAudioData(void*,u32,u32,u32*);
extern void vct_mix_audio(void*,void*,void*,u32);
extern void vct_set_speaker_samples(void*,u32);
extern void vct_encode_adpcm(u8*,s16*,u32,AdpcmState*,s32);
extern s32 vct_init_audio(void);
extern void vct_finish_audio(void);
extern s32 VCT_StartStreaming(Session*);
extern void VCT_StopStreaming(Session*);
extern s32 VCT_ReceiveAudio(void*,u32,u32*);
extern s32 VCT_SetCodec(s32);
extern void VCT_EnableVAD(s32);
extern void VCT_EnableEchoCancel(s32);
extern s32 vct_decode_audio_buffer(void);
extern s16 cAdpcmStepSizeTable[];
extern s8 cAdpcmIndexTable4[];
extern s8 cAdpcmIndexTable2[];
extern u64 _ll_udiv(u64,u64);
extern u8 sSendInfo[];
extern s32 VCT_SendAudio(void*,u32);
void vct_decode_adpcm(u8 *in,s16 *out,u32 size,s32 codec) {
 AdpcmState state; state.value=*(s16*)in; state.index=in[2];
 switch(codec) {case 2:vct_decode_adpcm_16(in+4,out,size-4,&state);break;case 3:vct_decode_adpcm_24(in+4,out,size-4,&state);break;case 4:vct_decode_adpcm_32(in+4,out,size-4,&state);break;}
}
void vct_encode_adpcm(u8 *out,s16 *in,u32 size,AdpcmState *state,s32 codec) {
 *(s16*)out=state->value; out[2]=state->index; out[3]=0;
 switch(codec) {case 2:vct_encode_adpcm_16(out+4,in,size,state);break;case 3:vct_encode_adpcm_24(out+4,in,size,state);break;case 4:vct_encode_adpcm_32(out+4,in,size,state);break;}
}
