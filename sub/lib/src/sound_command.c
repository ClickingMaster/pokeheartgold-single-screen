#pragma force_active on
#include <nitro/types.h>
// The SDK headers below expect <nitro/types.h> to be included first.
#include <nitro/SND_command_shared.h>
#include <nitro/SND_work_shared.h>
#include <nitro/mi/memory.h>
#include <nitro/pxi/fifo.h>

#include "OS_message.h"
#include "OS_system.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

extern OSMessageQueue sCommandMesgQueue;
extern OSMessage sCommandMesgBuffer[8];

// Sound driver entry points called by the command processor (defined in the other sound files).
u32 SND_GetChannelControl(int c);
u32 SND_GetLockedChannel(u32 v);
void SND_InvalidateBank(const void *start, const void *end);
void SND_InvalidateSeq(const void *start, const void *end);
void SND_InvalidateWave(u32 start, u32 end);
void SND_LockChannel(u32 bits, u32 kind);
void SND_PauseSeq(int player, int pause);
void SND_SetChannelPan(int c, int pan);
void SND_SetChannelTimer(int c, u32 timer);
void SND_SetMasterPan(int pan);
void SND_SetPlayerGlobalVariable(int i, s16 v);
void SND_SetPlayerLocalVariable(int p, int i, s16 v);
void SND_SetTrackMute(int player, u32 mask, int mute);
void SND_SetupAlarm(int i, u64 delay, u64 period, int id);
void SND_SetupChannelNoise(int c, int volume, int shift, int timer, int pan);
void SND_SetupChannelPcm(int c, u32 src, int format, int repeat, int loop, u32 len, int volume, int shift, int timer, int pan);
void SND_SkipSeq(int player, u32 ticks);
void SND_StartAlarm(int i);
void SND_StartPreparedSeq(int player);
void SND_StartSeq(int player, const u8 *data, u32 offset, void *bank);
void SND_StopAlarm(int i);
void SND_StopSeq(int player);
void SND_StopUnlockedChannel(u32 bits, u32 unusedCommandWord);
void SND_UnlockChannel(u32 mask, u32 v);
void SND_UpdateSharedWork(void);
void SNDi_SetPlayerParam(int player, u32 offset, u32 value, int size);
void SNDi_SetTrackParam(int player, u32 mask, u32 offset, u32 value, int size);
void SND_PrepareSeq(int player, const u8 *data, u32 offset, void *bank);
void SND_SetTrackAllocatableChannel(int player, u32 mask, u32 channels);
void SND_StopChannel(int channel, int flags);
void SND_SetChannelVolume(int channel, int volume, int shift);
void SND_SetupChannelPsg(int channel, u32 duty, int volume, int shift, int timer, int pan);
void SND_SetupCapture(u32 channel, u32 format, u32 address, u32 length, u32 repeat, u32 input, u32 output);
void SNDi_SetSurroundDecay(int decay);
void SND_SetMasterVolume(s32 volume);
void SND_SetOutputSelector(u32 left, u32 right, u32 channel1, u32 channel3);
void SND_SendWakeupMessage(void);

void SND_CommandInit(void);
void SND_CommandProc(void);
void PxiFifoCallback(PXIFifoTag tag, u32 data, BOOL err);

// -ipa file would otherwise inline these small functions into their callers in this file.
#pragma dont_inline on

void SND_CommandInit(void) {
    OS_InitMessageQueue(&sCommandMesgQueue, sCommandMesgBuffer, 8);
    PXI_SetFifoRecvCallback(PXI_FIFO_TAG_SOUND, PxiFifoCallback);
    SNDi_SharedWork = NULL;
}

void SND_CommandProc(void) {
    OSMessage message;
    while (OS_ReceiveMessage(&sCommandMesgQueue, &message, OS_MESSAGE_NOBLOCK)) {
        volatile u8 *cap1 = (volatile u8 *)0x04000509, *cap0 = cap1 - 1;
        struct SNDCommand *list = (struct SNDCommand *)message;
        while (list) {
            struct SNDCommand cmd = *list;
            switch (cmd.id) {
            case SND_CMD_START_SEQ:
                SND_StartSeq((int)cmd.arg[0], (const u8 *)cmd.arg[1], cmd.arg[2], (void *)cmd.arg[3]);
                break;
            case SND_CMD_STOP_SEQ:
                SND_StopSeq(cmd.arg[0]);
                break;
            case SND_CMD_PREPARE_SEQ:
                SND_PrepareSeq((int)cmd.arg[0], (const u8 *)cmd.arg[1], cmd.arg[2], (void *)cmd.arg[3]);
                break;
            case SND_CMD_START_PREPARED_SEQ:
                SND_StartPreparedSeq(cmd.arg[0]);
                break;
            case SND_CMD_PAUSE_SEQ:
                SND_PauseSeq(cmd.arg[0], cmd.arg[1]);
                break;
            case SND_CMD_SKIP_SEQ:
                SND_SkipSeq(cmd.arg[0], cmd.arg[1]);
                break;
            case SND_CMD_PLAYER_PARAM:
                SNDi_SetPlayerParam(cmd.arg[0], cmd.arg[1], cmd.arg[2], cmd.arg[3]);
                break;
            case SND_CMD_TRACK_PARAM:
                SNDi_SetTrackParam(cmd.arg[0] & 0xffffff, cmd.arg[1], cmd.arg[2], cmd.arg[3], (cmd.arg[0] >> 24) & 255);
                break;
            case SND_CMD_MUTE_TRACK:
                SND_SetTrackMute(cmd.arg[0], cmd.arg[1], cmd.arg[2]);
                break;
            case SND_CMD_ALLOCATABLE_CHANNEL:
                SND_SetTrackAllocatableChannel(cmd.arg[0], cmd.arg[1], cmd.arg[2]);
                break;
            case SND_CMD_PLAYER_LOCAL_VAR:
                SND_SetPlayerLocalVariable(cmd.arg[0], cmd.arg[1], (s16)cmd.arg[2]);
                break;
            case SND_CMD_PLAYER_GLOBAL_VAR:
                SND_SetPlayerGlobalVariable(cmd.arg[0], (s16)cmd.arg[1]);
                break;
            case SND_CMD_START_TIMER: {
                u32 channels, capture, alarms;
                alarms = cmd.arg[2];
                capture = cmd.arg[1];
                channels = cmd.arg[0];
                OSIntrMode irq = OS_DisableInterrupts();
                int i;
                for (i = 0; i < 16 && channels; i++, channels >>= 1) {
                    if (channels & 1) {
                        volatile u8 *ctrl = (volatile u8 *)(0x04000403 + (u32)i * 16);
                        *ctrl |= 128;
                    }
                }
                if (capture & 1) {
                    if (capture & 2) {
                        *(volatile u16 *)cap0 |= 0x8080;
                    } else {
                        *cap0 |= 128;
                    }
                } else if (capture & 2) {
                    *cap1 |= 128;
                }
                for (i = 0; i < 8 && alarms; i++, alarms >>= 1) {
                    if (alarms & 1) {
                        SND_StartAlarm(i);
                    }
                }
                OS_RestoreInterrupts(irq);
                SND_UpdateSharedWork();
                break;
            }
            case SND_CMD_STOP_TIMER: {
                u32 channels, capture, alarms;
                int i;
                u32 flag;
                flag = cmd.arg[3];
                alarms = cmd.arg[2];
                capture = cmd.arg[1];
                channels = cmd.arg[0];
                OSIntrMode irq = OS_DisableInterrupts();
                for (i = 0; i < 8 && alarms; i++, alarms >>= 1) {
                    if (alarms & 1) {
                        SND_StopAlarm(i);
                    }
                }
                for (i = 0; i < 16 && channels; i++, channels >>= 1) {
                    if (channels & 1) {
                        SND_StopChannel(i, flag);
                    }
                }
                if (capture & 1) {
                    *cap0 = 0;
                }
                if (capture & 2) {
                    *cap1 = 0;
                }
                OS_RestoreInterrupts(irq);
                SND_UpdateSharedWork();
                break;
            }
            case SND_CMD_SETUP_CAPTURE:
                SND_SetupCapture((cmd.arg[2] >> 31) & 1, (cmd.arg[2] >> 30) & 1, cmd.arg[0], cmd.arg[1], (cmd.arg[2] >> 29) & 1, (cmd.arg[2] >> 28) & 1, (cmd.arg[2] >> 27) & 1);
                break;
            case SND_CMD_SETUP_ALARM:
                SND_SetupAlarm(cmd.arg[0], (u64)cmd.arg[1], (u64)cmd.arg[2], cmd.arg[3]);
                break;
            case SND_CMD_CHANNEL_TIMER: {
                u32 channels, timer;
                timer = cmd.arg[1];
                channels = cmd.arg[0];
                for (int i = 0; i < 16 && channels; i++, channels >>= 1) {
                    if (channels & 1) {
                        SND_SetChannelTimer(i, timer);
                    }
                }
                break;
            }
            case SND_CMD_CHANNEL_VOLUME: {
                u32 channels;
                int volume, shift;
                shift = cmd.arg[2];
                volume = cmd.arg[1];
                channels = cmd.arg[0];
                for (int i = 0; i < 16 && channels; i++, channels >>= 1) {
                    if (channels & 1) {
                        SND_SetChannelVolume(i, volume, shift);
                    }
                }
                break;
            }
            case SND_CMD_CHANNEL_PAN: {
                u32 channels;
                int pan;
                pan = cmd.arg[1];
                channels = cmd.arg[0];
                for (int i = 0; i < 16 && channels; i++, channels >>= 1) {
                    if (channels & 1) {
                        SND_SetChannelPan(i, pan);
                    }
                }
                break;
            }
            case SND_CMD_SETUP_CHANNEL_PCM:
                SND_SetupChannelPcm((u16)cmd.arg[0], cmd.arg[1] & 0x7ffffff, (cmd.arg[3] >> 24) & 3, (cmd.arg[3] >> 26) & 3, (u16)cmd.arg[3], cmd.arg[2] & 0x3fffff, (cmd.arg[2] >> 24) & 127, (cmd.arg[2] >> 22) & 3, (u16)(cmd.arg[0] >> 16), (cmd.arg[3] >> 16) & 127);
                break;
            case SND_CMD_SETUP_CHANNEL_PSG:
                SND_SetupChannelPsg(cmd.arg[0], cmd.arg[3], cmd.arg[1] & 127, (cmd.arg[1] >> 8) & 3, (cmd.arg[2] >> 8) & 65535, cmd.arg[2] & 127);
                break;
            case SND_CMD_SETUP_CHANNEL_NOISE:
                SND_SetupChannelNoise(cmd.arg[0], cmd.arg[1] & 127, (cmd.arg[1] >> 8) & 3, (cmd.arg[2] >> 8) & 65535, cmd.arg[2] & 127);
                break;
            case SND_CMD_SURROUND_DECAY:
                SNDi_SetSurroundDecay(cmd.arg[0]);
                break;
            case SND_CMD_MASTER_VOLUME:
                SND_SetMasterVolume(cmd.arg[0]);
                break;
            case SND_CMD_MASTER_PAN:
                SND_SetMasterPan(cmd.arg[0]);
                break;
            case SND_CMD_OUTPUT_SELECTOR:
                SND_SetOutputSelector(cmd.arg[0], cmd.arg[1], cmd.arg[2], cmd.arg[3]);
                break;
            case SND_CMD_LOCK_CHANNEL:
                SND_LockChannel(cmd.arg[0], cmd.arg[1]);
                break;
            case SND_CMD_UNLOCK_CHANNEL:
                SND_UnlockChannel(cmd.arg[0], cmd.arg[1]);
                break;
            case SND_CMD_STOP_UNLOCKED_CHANNEL:
                SND_StopUnlockedChannel(cmd.arg[0], cmd.arg[1]);
                break;
            case SND_CMD_INVALIDATE_SEQ:
                SND_InvalidateSeq((const void *)cmd.arg[0], (const void *)cmd.arg[1]);
                break;
            case SND_CMD_INVALIDATE_BANK:
                SND_InvalidateBank((const void *)cmd.arg[0], (const void *)cmd.arg[1]);
                break;
            case SND_CMD_INVALIDATE_WAVE:
                SND_InvalidateWave(cmd.arg[0], cmd.arg[1]);
                break;
            case SND_CMD_SET_SHARED_WORK:
                SNDi_SharedWork = (struct SNDSharedWork *)cmd.arg[0];
                break;
            case SND_CMD_READ_DRIVER_INFO: {
                struct SNDDriverInfo *dest = (struct SNDDriverInfo *)cmd.arg[0];
                MIi_CpuCopy32((const u32 *)&SNDi_Work, (u32 *)&dest->work, sizeof(dest->work));
                dest->workPtr = &SNDi_Work;
                for (int i = 0; i < 16; i++) {
                    dest->channelControls[i] = SND_GetChannelControl(i);
                }
                dest->lockedChannels = SND_GetLockedChannel(0);
                break;
            }
            }
            list = cmd.llNext;
        }
        SNDi_SharedWork->finishedCommandTag++;
    }
}

// Words >= 0x02000000 are pointers to a command list in main memory; 0 is a wake-up request.
void PxiFifoCallback(PXIFifoTag tag, u32 data, BOOL err) {
    OSIntrMode irq = OS_DisableInterrupts();
    if (data >= 0x02000000) {
        OS_SendMessage(&sCommandMesgQueue, (void *)data, OS_MESSAGE_NOBLOCK);
    } else if (!data) {
        SND_SendWakeupMessage();
    }
    OS_RestoreInterrupts(irq);
}

#pragma dont_inline reset
