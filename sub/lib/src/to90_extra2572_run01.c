#pragma force_active on
#include <nitro/SND_command_shared.h>
#include <nitro/SND_work_shared.h>
#include <nitro/types.h>

#include "OS_message.h"
#include "OS_system.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern OSMessageQueue sCommandMesgQueue;
extern void MIi_CpuCopy32(const void *, void *, u32);
extern u32 SND_GetChannelControl(int c);
extern u32 SND_GetLockedChannel(u32 v);
extern void SND_InvalidateBank(const void *start, const void *end);
extern void SND_InvalidateSeq(const void *start, const void *end);
extern void SND_InvalidateWave(u32 start, u32 end);
extern void SND_LockChannel(u32 bits, u32 kind);
extern void SND_PauseSeq(int player, int pause);
extern void SND_SetChannelPan(int c, int pan);
extern void SND_SetChannelTimer(int c, u32 timer);
extern void SND_SetMasterPan(int pan);
extern void SND_SetPlayerGlobalVariable(int i, s16 v);
extern void SND_SetPlayerLocalVariable(int p, int i, s16 v);
extern void SND_SetTrackMute(int player, u32 mask, int mute);
extern void SND_SetupAlarm(int i, u64 delay, u64 period, int id);
extern void SND_SetupChannelNoise(int c, int volume, int shift, int timer, int pan);
extern void SND_SetupChannelPcm(int c, u32 src, int format, int repeat, int loop, u32 len, int volume, int shift, int timer, int pan);
extern void SND_SkipSeq(int player, u32 ticks);
extern void SND_StartAlarm(int i);
extern void SND_StartPreparedSeq(int player);
extern void SND_StartSeq(int player, const u8 *data, u32 offset, void *bank);
extern void SND_StopAlarm(int i);
extern void SND_StopSeq(int player);
extern void SND_StopUnlockedChannel(u32 bits, u32 unusedCommandWord);
extern void SND_UnlockChannel(u32 mask, u32 v);
extern void SND_UpdateSharedWork(void);
extern void SNDi_SetPlayerParam(int player, u32 offset, u32 value, int size);
extern void SNDi_SetTrackParam(int player, u32 mask, u32 offset, u32 value, int size);
extern void SND_PrepareSeq(int player, const u8 *data, u32 offset, void *bank);
extern void SND_SetTrackAllocatableChannel(int player, u32 mask, u32 channels);
extern void SND_StopChannel(int channel, int flags);
extern void SND_SetChannelVolume(int channel, int volume, int shift);
extern void SND_SetupChannelPsg(int channel, u32 duty, int volume, int shift, int timer, int pan);
extern void SND_SetupCapture(u32 channel, u32 format, u32 address, u32 length, u32 repeat, u32 input, u32 output);
extern void SNDi_SetSurroundDecay(int decay);
extern void SND_SetMasterVolume(s32 volume);
extern void SND_SetOutputSelector(u32 left, u32 right, u32 channel1, u32 channel3);
void SND_CommandProc(void);

void SND_CommandProc(void) {
    OSMessage message;
    while (OS_ReceiveMessage(&sCommandMesgQueue, &message, 0)) {
        volatile u8 *cap1 = (volatile u8 *)0x04000509, *cap0 = cap1 - 1;
        struct SNDCommand *list = (struct SNDCommand *)message;
        while (list) {
            struct SNDCommand cmd = *list;
            switch (cmd.id) {
            case 0:
                SND_StartSeq((int)cmd.arg[0], (const u8 *)cmd.arg[1], cmd.arg[2], (void *)cmd.arg[3]);
                break;
            case 1:
                SND_StopSeq(cmd.arg[0]);
                break;
            case 2:
                SND_PrepareSeq((int)cmd.arg[0], (const u8 *)cmd.arg[1], cmd.arg[2], (void *)cmd.arg[3]);
                break;
            case 3:
                SND_StartPreparedSeq(cmd.arg[0]);
                break;
            case 4:
                SND_PauseSeq(cmd.arg[0], cmd.arg[1]);
                break;
            case 5:
                SND_SkipSeq(cmd.arg[0], cmd.arg[1]);
                break;
            case 6:
                SNDi_SetPlayerParam(cmd.arg[0], cmd.arg[1], cmd.arg[2], cmd.arg[3]);
                break;
            case 7:
                SNDi_SetTrackParam(cmd.arg[0] & 0xffffff, cmd.arg[1], cmd.arg[2], cmd.arg[3], (cmd.arg[0] >> 24) & 255);
                break;
            case 8:
                SND_SetTrackMute(cmd.arg[0], cmd.arg[1], cmd.arg[2]);
                break;
            case 9:
                SND_SetTrackAllocatableChannel(cmd.arg[0], cmd.arg[1], cmd.arg[2]);
                break;
            case 10:
                SND_SetPlayerLocalVariable(cmd.arg[0], cmd.arg[1], (s16)cmd.arg[2]);
                break;
            case 11:
                SND_SetPlayerGlobalVariable(cmd.arg[0], (s16)cmd.arg[1]);
                break;
            case 12: {
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
            case 13: {
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
            case 17:
                SND_SetupCapture((cmd.arg[2] >> 31) & 1, (cmd.arg[2] >> 30) & 1, cmd.arg[0], cmd.arg[1], (cmd.arg[2] >> 29) & 1, (cmd.arg[2] >> 28) & 1, (cmd.arg[2] >> 27) & 1);
                break;
            case 18:
                SND_SetupAlarm(cmd.arg[0], (u64)cmd.arg[1], (u64)cmd.arg[2], cmd.arg[3]);
                break;
            case 19: {
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
            case 20: {
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
            case 21: {
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
            case 14:
                SND_SetupChannelPcm((u16)cmd.arg[0], cmd.arg[1] & 0x7ffffff, (cmd.arg[3] >> 24) & 3, (cmd.arg[3] >> 26) & 3, (u16)cmd.arg[3], cmd.arg[2] & 0x3fffff, (cmd.arg[2] >> 24) & 127, (cmd.arg[2] >> 22) & 3, (u16)(cmd.arg[0] >> 16), (cmd.arg[3] >> 16) & 127);
                break;
            case 15:
                SND_SetupChannelPsg(cmd.arg[0], cmd.arg[3], cmd.arg[1] & 127, (cmd.arg[1] >> 8) & 3, (cmd.arg[2] >> 8) & 65535, cmd.arg[2] & 127);
                break;
            case 16:
                SND_SetupChannelNoise(cmd.arg[0], cmd.arg[1] & 127, (cmd.arg[1] >> 8) & 3, (cmd.arg[2] >> 8) & 65535, cmd.arg[2] & 127);
                break;
            case 22:
                SNDi_SetSurroundDecay(cmd.arg[0]);
                break;
            case 23:
                SND_SetMasterVolume(cmd.arg[0]);
                break;
            case 24:
                SND_SetMasterPan(cmd.arg[0]);
                break;
            case 25:
                SND_SetOutputSelector(cmd.arg[0], cmd.arg[1], cmd.arg[2], cmd.arg[3]);
                break;
            case 26:
                SND_LockChannel(cmd.arg[0], cmd.arg[1]);
                break;
            case 27:
                SND_UnlockChannel(cmd.arg[0], cmd.arg[1]);
                break;
            case 28:
                SND_StopUnlockedChannel(cmd.arg[0], cmd.arg[1]);
                break;
            case 30:
                SND_InvalidateSeq((const void *)cmd.arg[0], (const void *)cmd.arg[1]);
                break;
            case 31:
                SND_InvalidateBank((const void *)cmd.arg[0], (const void *)cmd.arg[1]);
                break;
            case 32:
                SND_InvalidateWave(cmd.arg[0], cmd.arg[1]);
                break;
            case 29:
                SNDi_SharedWork = (struct SNDSharedWork *)cmd.arg[0];
                break;
            case 33: {
                struct SNDDriverInfo *dest = (struct SNDDriverInfo *)cmd.arg[0];
                MIi_CpuCopy32(&SNDi_Work, &dest->work, sizeof(dest->work));
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
