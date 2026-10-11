// Pokewalker infrared link: packet layer over the card SPI IR chip (checksum, XOR 0xAA framing, handshake,
// session id), the block transfer protocol (128-byte chunks, LZ compressed uploads) and the polling thread.

#include "application/pokewalker/pokewalker_ir.h"

#include <nitro/rtc.h>

#include "global.h"

#include "application/pokewalker/pokewalker_ir_internal.h"

extern void CARD_SpiWaitInit(void);
extern void CARD_SetSpiWriteWaitCycles(u32 cycles);
extern void CARD_SetSpiReadWaitCycles(u32 cycles);
extern u32 CARD_SpiWaitReadRange(void *dst);
extern void CARD_SpiWaitWriteRange(void *src, u32 len);
extern int MI_CompressLZImpl(const void *src, u32 size, void *dst, int unk);
extern float _f_ulltof(u64 value);
extern float _fdiv(float a, float b);
extern u64 _ll_udiv(u64 a, u64 b);
extern int RTC_GetTime(RTCTime *time);
extern int RTC_GetDate(RTCDate *date);

#define TICKS_TO_MS(ticks) (((ticks) << 6) / 0x82EA)

#define SWAP16(v) (((u32)(u8)((v) >> 8)) | (((v) << 8) & 0xFF00))
#define SWAP32(v) (((u32)(u8)((v) >> 24)) | (((v) >> 8) & 0xFF00) | (((v) << 8) & 0xFF0000) | (((v) << 24) & 0xFF000000))

#pragma force_active on

u16 PokewalkerIr_Checksum(u8 *bytes, u32 len) {
    u32 sum = 2, index;
    for (index = 0; index < len; index++) {
        u8 b = *bytes++;
        if (index & 1) {
            sum += b;
        } else {
            sum += b << 8;
        }
    }
    sum = (sum >> 16) + (u16)sum;
    sum += sum >> 16;
    return sum;
}

void ov112_021E5938(void) {
    _021FF9E0.closing = 1;
    _021FF9E0.numDisconnects++;
}

#pragma force_active reset

void PokewalkerIr_ResetLink(void) {
    _021FF9E0.unk_00 = 0;
    _021FF9E0.handshakeState = 0;
    _021FF9E0.active = 0;
    _021FF9E0.closing = 0;
    _021FF9E0.handshakeStarted = 0;
    _021FF9E0.connected = 0;
    _021FF9E0.numDisconnects = 0;
}

void PokewalkerIr_InitLink(void) {
    u64 tick;
    CARD_SpiWaitInit();
    CARD_SetSpiWriteWaitCycles(50);
    CARD_SetSpiReadWaitCycles(50);
    CARD_SpiWaitReadRange(&ov112_021FFA18);
    PokewalkerIr_ResetLink();
    _021FF500[0] = 0xFF;
    _021FF500[1] = 0xFF;
    tick = OS_GetTick();
    _021FF9E0.ownId = (u32)tick;
    _021FF9E0.sessionId = (u32)tick;
    _021FF9E0.onConnect = NULL;
    _021FF9E0.onReceive = NULL;
}

u32 PokewalkerIr_IsConnected(void) {
    return _021FF9E0.active;
}

#pragma force_active on
#pragma optimization_level 2

void PokewalkerIr_SendPacket(const u8 *src, int len, u8 command, u8 arg, u32 sessionId) {
    int i;
    int checksum;
    u8 *dst;
    PokewalkerIrPacket *packet;
    packet = &ov112_021FFA18;
    packet->command = command;
    packet->arg = arg;
    packet->sessionId = sessionId;
    packet->checksumLo = 0;
    packet->checksumHi = 0;
    dst = ov112_021FFA20;
    for (i = 0; i < len;) {
        u8 b = *src;
        src++;
        i++;
        *dst = b;
        dst++;
    }
    len = (u8)(len + 8);
    checksum = PokewalkerIr_Checksum((u8 *)&ov112_021FFA18, len);
    packet->checksumLo = checksum;
    packet->checksumHi = checksum >> 8;
    for (i = 0; i < len; i++) {
        ((u8 *)&ov112_021FFA18)[i] ^= 0xAA;
    }
    CARD_SpiWaitWriteRange(&ov112_021FFA18, len);
}

#pragma optimization_level reset

void PokewalkerIr_CloseLink(int result) {
    if (_021FF9E0.active && !_021FF9E0.connected && !result) {
        PokewalkerIr_SendPacket(NULL, 0, 0xF4, _021FF500[0], _021FF9E0.sessionId);
    }
    if (_021FF9E0.onClose) {
        _021FF9E0.onClose(result);
    }
    PokewalkerIr_ResetLink();
}

#pragma force_active reset

void PokewalkerIr_Disconnect(void) {
    PokewalkerIr_CloseLink(0);
}

void PokewalkerIr_Send(void *data, int len, u8 command, u8 arg) {
    if (_021FF9E0.active != 0) {
        PokewalkerIr_SendPacket(data, len, command, arg, _021FF9E0.sessionId);
    }
}

void ov112_021E5A80(void) {
}

#pragma force_active on
#pragma optimization_level 1

BOOL ov112_021E5A84(u32 size) {
    u32 i;
    if (size == 1) {
        if (_021FF9E0.active != 0) {
            return TRUE;
        }
        if (ov112_021FFA18.command != 0xFC) {
            return TRUE;
        }
        return FALSE;
    }
    switch (size) {
    case 2:
    case 3:
        for (i = 0; i < 4; i++) {
        }
        for (i = 0; i < size; i++) {
        }
        return TRUE;
    }
    return FALSE;
}

#pragma optimization_level reset
#pragma force_active reset

BOOL PokewalkerIr_CheckTimeout(void) {
    u64 elapsed;
    if (_021FF9E0.active == 0 && _021FF9E0.handshakeStarted == 0) {
        return FALSE;
    }
    elapsed = OS_GetTick() - _021FF9E0.lastReceiveTick;
    if (TICKS_TO_MS(elapsed) < 100) {
        return FALSE;
    }
    PokewalkerIr_CloseLink(1);
    _021FF9E0.handshakeStarted = 0;
    return TRUE;
}

#pragma force_active on
#pragma opt_rotateloops off

BOOL PokewalkerIr_VerifyPacket(u32 size) {
    u8 *p = (u8 *)&ov112_021FFA18;
    if (size == 1) {
        if (p[0] == 0xFC) {
            return TRUE;
        }
        return FALSE;
    }
    if (size < 4) {
        return FALSE;
    }
    u16 checksum = p[2] + (p[3] << 8);
    p[2] = 0;
    p[3] = 0;
    u32 actual = PokewalkerIr_Checksum(p, size);
    p[2] = checksum;
    p[3] = checksum >> 8;
    if (checksum == actual) {
        _021FF9E0.unk_00 = 0;
        return TRUE;
    }
    if (_021FF9E0.active) {
        // waits for the rest of the packet (the loop counter is passed but unused)
        u32 i = 0;
        goto test;
loop:
        i++;
test:
        if (i < size) {
            goto loop;
        }
        ((void (*)(u32))ov112_021E5938)(i);
    }
    return FALSE;
}

#pragma opt_rotateloops reset
#pragma opt_common_subs off

int PokewalkerIr_Poll(void) {
    PokewalkerIrPacket *packet;
    u32 length = CARD_SpiWaitReadRange(&ov112_021FFA18);
    if (ov112_021E5A84(length)) {
        length = 0;
    }
    if (length == 0) {
        if (PokewalkerIr_CheckTimeout()) {
            return 1;
        }
        return 0;
    }
    if (!PokewalkerIr_VerifyPacket(length)) {
        return 0;
    }
    _021FF9E0.lastReceiveTick = OS_GetTick();
    packet = &ov112_021FFA18;
    if (packet->command < 0xF0) {
        if (!_021FF9E0.active) {
            return 0;
        }
        if (packet->sessionId != _021FF9E0.sessionId) {
            return 0;
        }
    }
    switch (packet->command) {
    case 0xFC:
        switch (_021FF9E0.handshakeState) {
        case 1:
            _021FF9E0.handshakeState = 2;
            PokewalkerIr_SendPacket(NULL, 0, 0xFA, _021FF500[0], _021FF9E0.ownId);
            break;
        case 0:
        case 2:
        case 3:
        case 4:
        default:
            break;
        }
        if (_021FF9E0.handshakeStarted) {
            break;
        }
        _021FF9E0.handshakeStarted = 1;
        _021FF9E0.handshakeState = 2;
        _021FF9E0.connected = 0;
        if (length > 1 && !_021FF9E0.active) {
            break;
        }
        PokewalkerIr_SendPacket(NULL, 0, 0xFA, _021FF500[0], _021FF9E0.ownId);
        if (_021FF9E0.active) {
            ov112_021E5938();
        }
        ov112_021E5A80();
        break;
    case 0xFA:
        if (_021FF9E0.handshakeState != 1) {
            break;
        }
        _021FF9E0.handshakeState = 3;
        _021FF9E0.connected = 1;
        _021FF500[1] = packet->arg;
        if (_021FF500[1] != 1) {
            break;
        }
        _021FF9E0.sessionId = packet->sessionId ^ _021FF9E0.ownId;
        PokewalkerIr_SendPacket(NULL, 0, 0xF8, _021FF500[0], _021FF9E0.ownId);
        _021FF9E0.active = 1;
        _021FF9E0.closing = 0;
        _021FF9E0.handshakeStarted = 0;
        break;
    case 0xF8:
        if (_021FF9E0.handshakeState != 2) {
            break;
        }
        _021FF9E0.handshakeState = 4;
        _021FF500[1] = packet->arg;
        _021FF9E0.sessionId = packet->sessionId ^ _021FF9E0.ownId;
        _021FF9E0.active = 1;
        _021FF9E0.closing = 0;
        _021FF9E0.handshakeStarted = 0;
        if (_021FF9E0.onConnect) {
            _021FF9E0.onConnect(packet->arg, 0);
        }
        break;
    case 0xF6:
        ov112_021E5938();
        break;
    default:
        if (_021FF9E0.closing != 1) {
            if (_021FF9E0.active) {
                _021FF9E0.onReceive(ov112_021FFA20, (u8)(length - 8), ov112_021FFA18.command, ov112_021FFA18.arg);
            }
        }
        break;
    }
    return 0;
}

#pragma opt_common_subs reset
#pragma force_active reset

void PokewalkerIr_SetConnectCallback(void *callback) {
    _021FF9E0.onConnect = callback;
}

void PokewalkerIr_SetReceiveCallback(void *callback) {
    _021FF9E0.onReceive = callback;
}

void PokewalkerIr_SetCloseCallback(void *callback) {
    _021FF9E0.onClose = callback;
}

void ov112_021E5D5C(u8 value) {
    _021FF500[0] = value;
}

void PokewalkerIr_SetCompressBuffer(void *buffer) {
    if ((u32)buffer & 3) {
        OS_Halt();
    }
    ov112_021FFAA4.compressBuffer = buffer;
    ov112_021FFAA4.compressedSizes = (u8 *)buffer + 0xCD80;
}

#pragma force_active on

// Compresses size bytes in blocks of 128; sizes[i] receives the compressed size of block i (128 = stored).
void PokewalkerIr_CompressBlocks(u8 *src, u8 *dst, u16 size, u8 *sizes) {
    int i = 0, blocks = (u16)((size + 127) / 128);
    for (; i < blocks; i++) {
        u32 n = size;
        if (n > 128) {
            n = 128;
        }
        if (n != 128) {
            MI_CpuCopy8(src, dst, n);
            *sizes++ = 128;
        } else {
            u8 result = MI_CompressLZImpl(src, n, dst, 0);
            if (result == 0) {
                MI_CpuCopy8(src, dst, n);
                *sizes++ = 128;
            } else {
                *sizes++ = result;
            }
        }
        size -= n;
        src += 128;
        dst += 128;
    }
}

#pragma force_active reset

void PokewalkerIr_SetError(int error) {
    if (ov112_021FFAA4.error == 0) {
        ov112_021FFAA4.error = error;
    }
}

#pragma force_active on

void PokewalkerIr_StoreClock(void) {
    PokewalkerIrIdentity *identity = ov112_021FFAA4.identity;
    PokewalkerIrClock *clock = &identity->clock;
    s64 offset = OS_GetOwnerRtcOffset();
    identity->clock.rtcOffsetLow = (u32)offset;
    clock->rtcOffsetHigh = offset >> 32;
    OS_GetLowEntropyData(clock->entropy);
}

void PokewalkerIr_StoreCurrentTime(void) {
    RTCTime time;
    RTCDate date;
    if (RTC_GetTime(&time) == 0) {
        if (RTC_GetDate(&date) == 0) {
            s64 seconds = RTC_ConvertDateTimeToSecond(&date, &time);
            u32 s;
            if (seconds > 0xFFFFFFFFLL) {
                seconds = 0xFFFFFFFFLL;
            }
            if (seconds < 0) {
                seconds = 0;
            }
            s = (u32)seconds;
            ov112_021FFAA4.identity->currentTime = SWAP32(s);
        }
    }
}

#pragma force_active reset

void PokewalkerIr_SetIdentities(void *identity, void *peerIdentity) {
    ov112_021FFAA4.identity = identity;
    ov112_021FFAA4.peerIdentity = peerIdentity;
}

void PokewalkerIr_BeginTransfer(void *data, u32 address, int size) {
    u16 stored;
    ov112_021FFB24.data = data;
    ov112_021FFB24.dataStart = data;
    ov112_021FFB24.address = address;
    ov112_021FFB24.addressStart = address;
    ov112_021FFB24.remaining = size;
    stored = *(volatile u16 *)&ov112_021FFB24.remaining;
    ov112_021FFB24.size = stored;
    ov112_021FFB24.chunk = 0;
    ov112_021FFB24.numChunks = (size + 0x7F) / 128;
}

void PokewalkerIr_SendChunk(void) {
    u8 packet[0x80];
    u16 pos = ov112_021FFB24.address;
    u16 count = ov112_021FFB24.remaining;
    u8 **sizes;
    u8 length;
    if (count > 0x80) {
        count = 0x80;
    }
    sizes = NULL;
    switch (ov112_021FFB24.step) {
    case 0x3A:
        if (ov112_021FFAA4.upload0 != 0) {
            sizes = ov112_021FFAF0;
        }
        break;
    case 0x3C:
        if (ov112_021FFAA4.upload1 != 0) {
            sizes = ov112_021FFAF8;
        }
        break;
    }
    if (sizes == NULL) {
        if ((pos & 0x7F) != 0) {
            count = 0x80 - (pos & 0x7F);
            packet[0] = pos;
            MI_CpuCopy8(ov112_021FFB24.data, packet + 1, count);
            PokewalkerIr_Send(packet, (u8)(count + 1), 0xA, (u8)(pos >> 8));
        } else {
            PokewalkerIr_Send(ov112_021FFB24.data, (u8)count, (u8)(2 | (pos & 0x80)), (u8)(pos >> 8));
        }
    } else {
        length = (*sizes)[ov112_021FFB24.chunk];
        if (length > 0x80) {
            OS_Halt();
        }
        if (length == 0x80) {
            PokewalkerIr_Send(ov112_021FFB24.data, 0x80, (u8)(2 | (pos & 0x80)), (u8)(pos >> 8));
        } else {
            PokewalkerIr_Send(ov112_021FFB24.data, length, (u8)(pos & 0x80), (u8)(pos >> 8));
        }
    }
    ov112_021FFB24.data = ov112_021FFB24.data + count;
    ov112_021FFB24.address += count;
    ov112_021FFB24.remaining -= count;
    (ov112_021FFB24.chunk)++;
    (ov112_021FFB24.totalChunk)++;
}

void PokewalkerIr_OnSendComplete(void) {
    u64 elapsed = OS_GetTick() - ov112_021FFAA4.transferStartTick;
    _fdiv(_f_ulltof(_ll_udiv(elapsed << 6, 0x82EA)), 1000.0f);
    switch (ov112_021FFB24.step) {
    case 0x3A:
        ov112_021FFB24.step = 0x3C;
        if (ov112_021FFAA4.upload1 != NULL) {
            PokewalkerIr_BeginTransfer(ov112_021FFAA4.upload1, 0xD700, 0x28BE);
        } else {
            PokewalkerIr_BeginTransfer(ov112_021FFAA4.unk_2C, 0xD700, 0x28BE);
        }
        PokewalkerIr_SendChunk();
        break;
    case 0x3C:
        ov112_021FFB24.step = 0x3E;
        MI_CpuCopy8((u8 *)ov112_021FFAA4.peerIdentity + 0x10, ov112_021FFAA4.unk_30 + 8, 0x28);
        PokewalkerIr_BeginTransfer(ov112_021FFAA4.unk_30, 0xD480, 0x224);
        PokewalkerIr_SendChunk();
        break;
    case 0x3E:
        if (ov112_021FFB24.command == 0x52) {
            ov112_021FFB24.step = 0x48;
            PokewalkerIr_BeginTransfer((void *)0xCE80, (u32)ov112_021FFAA4.status, 0xD4C);
            PokewalkerIr_RequestChunk();
        } else {
            PokewalkerIr_Send(NULL, 0, 0x24, 1);
        }
        break;
    case 0xB0:
    case 0xB2:
    case 0xB4:
    case 0xB6:
    case 0xB8:
    case 0xBA:
    case 0xBC:
    case 0xBE:
        PokewalkerIr_Send(NULL, 0, 0x24, 1);
        break;
    default:
        PokewalkerIr_SetError(12);
        PokewalkerIr_Disconnect();
        break;
    }
}

void PokewalkerIr_RequestChunk(void) {
    u8 data[4];
    u16 addr;
    u16 n = ov112_021FFB24.remaining;
    if (n > 0x80) {
        n = 0x80;
    }
    addr = (u16)(u32)ov112_021FFB24.data;
    data[0] = addr >> 8;
    data[1] = addr;
    data[2] = n;
    PokewalkerIr_Send(data, 3, 0xC, 2);
}

#pragma force_active on

#define LOAD_NEXT_BLOCK()                                                                                             \
    switch (ov112_021FFAA4.blockIndex) {                                                                              \
    case 0:                                                                                                           \
        ov112_021FFB24.step = 176;                                                                                    \
        PokewalkerIr_BeginTransfer((void *)(ov112_021FFAA4.blocks->block0), (u32)((void *)0xB804), 576);              \
        break;                                                                                                        \
    case 1:                                                                                                           \
        ov112_021FFB24.step = 178;                                                                                    \
        PokewalkerIr_BeginTransfer((void *)((void *)((s32)ov112_021FFAA4.blocks + 580)), (u32)((void *)0xBA44), 764); \
        break;                                                                                                        \
    case 2:                                                                                                           \
        ov112_021FFB24.step = 180;                                                                                    \
        PokewalkerIr_BeginTransfer((void *)(ov112_021FFAA4.blocks->block2), (u32)((void *)0xBD40), 392);              \
        break;                                                                                                        \
    case 3:                                                                                                           \
        ov112_021FFB24.step = 182;                                                                                    \
        PokewalkerIr_BeginTransfer((void *)(ov112_021FFAA4.unk_0C), (u32)((void *)0xBF00), 3260);                     \
        break;                                                                                                        \
    }

#define CHECK_BLOCK_LOCKED()                        \
    switch (ov112_021FFAA4.blockIndex) {            \
    case 0:                                         \
        if (ov112_021FFAA4.blocks->flags.locked0) { \
            PokewalkerIr_Send(0, 0, 156, 1);        \
            return;                                 \
        }                                           \
        break;                                      \
    case 1:                                         \
        if (ov112_021FFAA4.blocks->flags.locked1) { \
            PokewalkerIr_Send(0, 0, 156, 1);        \
            return;                                 \
        }                                           \
        break;                                      \
    case 2:                                         \
        if (ov112_021FFAA4.blocks->flags.locked2) { \
            PokewalkerIr_Send(0, 0, 156, 1);        \
            return;                                 \
        }                                           \
        break;                                      \
    case 3:                                         \
        if (ov112_021FFAA4.blocks->flags.locked3) { \
            PokewalkerIr_Send(0, 0, 156, 1);        \
            return;                                 \
        }                                           \
        break;                                      \
    }

#define HANDSHAKE(bit, mask)                                \
    CHECK_BLOCK_LOCKED();                                   \
    {                                                       \
        u8 flags = ov112_021FFAA4.blocks->flags.handshake;  \
        if ((flags & mask) == mask) {                       \
            flags |= bit;                                   \
            ov112_021FFAA4.blocks->flags.handshake = flags; \
            LOAD_NEXT_BLOCK();                              \
            PokewalkerIr_SendChunk();                       \
            return;                                         \
        }                                                   \
        if (flags & bit) {                                  \
            PokewalkerIr_Send(0, 0, 158, 1);                \
            return;                                         \
        }                                                   \
        ov112_021FFB24.step = ov112_021FFB24.command;       \
        PokewalkerIr_Send(0, 0, 36, 1);                     \
        return;                                             \
    }

void PokewalkerIr_OnReceiveComplete(void) {
    int i, offset;
    u32 *words;
    PokewalkerIrRecord *record;
    float elapsed = _f_ulltof(((OS_GetTick() - ov112_021FFAA4.transferStartTick) << 6) / 33514) / 1000.0f;
    (void)elapsed;
    switch (ov112_021FFB24.step) {
    case 70:
        ov112_021FFB24.step = 72;
        PokewalkerIr_BeginTransfer((void *)((void *)0xCE80), (u32)(ov112_021FFAA4.status), 3404);
        PokewalkerIr_RequestChunk();
        return;
    case 72:
        ov112_021FFAA4.status->unk_0A = SWAP16(ov112_021FFAA4.status->unk_0A);
        for (i = 0, offset = 0; i < 7; i++, offset += 4) {
            words = (u32 *)((u8 *)ov112_021FFAA4.status + offset + 0x70);
            *words = SWAP32(*words);
        }
        ov112_021FFAA4.status->unk_00 = SWAP32(ov112_021FFAA4.status->unk_00);
        ov112_021FFAA4.status->unk_04 = SWAP32(ov112_021FFAA4.status->unk_04);
        for (i = 0, record = ov112_021FFAA4.status->records; i < 24; i++, record++) {
            record->value = SWAP32(record->value);
            record->unk_78 = SWAP16(record->unk_78);
            record->unk_7A = SWAP16(record->unk_7A);
            record->unk_7C = SWAP32(record->unk_7C);
            record->unk_80 = SWAP32(record->unk_80);
        }
        switch (ov112_021FFB24.command) {
        case 176:
        case 178:
        case 180:
        case 182:
        case 184:
        case 186:
        case 188:
        case 190:
            ov112_021FFB24.step = 74;
            PokewalkerIr_BeginTransfer((void *)((void *)0xB800), (u32)(ov112_021FFAA4.blocks), 4);
            PokewalkerIr_RequestChunk();
            return;
        case 96:
            ov112_021FFB24.step = 76;
            PokewalkerIr_BeginTransfer((void *)((void *)0xDE24), (u32)(ov112_021FFAA4.unk_30 + 548), 5480);
            PokewalkerIr_RequestChunk();
            return;
        default:
            ov112_021FFB24.step = 74;
            PokewalkerIr_BeginTransfer((void *)((void *)0xB800), (u32)(ov112_021FFAA4.blocks), 1736);
            PokewalkerIr_RequestChunk();
            return;
        }
    case 74:
        switch (ov112_021FFB24.command) {
        case 82:
            PokewalkerIr_Send(0, 0, 36, 1);
            return;
        case 64:
            ov112_021FFB24.step = 76;
            PokewalkerIr_BeginTransfer((void *)((void *)0xDE24), (u32)(ov112_021FFAA4.unk_30 + 548), 5480);
            PokewalkerIr_RequestChunk();
            return;
        case 176:
            if (ov112_021FFAA4.blocks->flags.locked0) {
                PokewalkerIr_Send(0, 0, 156, 1);
                return;
            }
            ov112_021FFB24.step = 176;
            PokewalkerIr_BeginTransfer((void *)((u8 *)ov112_021FFAA4.blocks + 4), (u32)((void *)0xB804), 576);
            PokewalkerIr_SendChunk();
            return;
        case 178:
            if (ov112_021FFAA4.blocks->flags.locked1) {
                PokewalkerIr_Send(0, 0, 156, 1);
                return;
            }
            ov112_021FFB24.step = 178;
            PokewalkerIr_BeginTransfer((void *)((void *)((s32)ov112_021FFAA4.blocks + 580)), (u32)((void *)0xBA44), 764);
            PokewalkerIr_SendChunk();
            return;
        case 180:
            if (ov112_021FFAA4.blocks->flags.locked2) {
                PokewalkerIr_Send(0, 0, 156, 1);
                return;
            }
            ov112_021FFB24.step = 180;
            PokewalkerIr_BeginTransfer((void *)(ov112_021FFAA4.blocks->block2), (u32)((void *)0xBD40), 392);
            PokewalkerIr_SendChunk();
            return;
        case 182:
            if (ov112_021FFAA4.blocks->flags.locked3) {
                PokewalkerIr_Send(0, 0, 156, 1);
                return;
            }
            ov112_021FFB24.step = 182;
            PokewalkerIr_BeginTransfer((void *)(ov112_021FFAA4.unk_0C), (u32)((void *)0xBF00), 3260);
            PokewalkerIr_SendChunk();
            return;
        case 184:
            HANDSHAKE(1, 14);
        case 186:
            HANDSHAKE(2, 13);
        case 188:
            HANDSHAKE(4, 11);
        case 190:
            HANDSHAKE(8, 7);
        default:
            OS_Halt();
            return;
        }
    case 76:
        PokewalkerIr_Send(0, 0, 36, 1);
        return;
    default:
        return;
    }
}

#pragma force_active reset

void PokewalkerIr_OnConnect(int result) {
    u8 data[4];
    u16 value;

    ov112_021FFAA4.error = 0;
    if (result == 0) {
        PokewalkerIr_SetError(12);
        PokewalkerIr_Disconnect();
        return;
    }
    if ((u32)result > 2) {
        PokewalkerIr_SetError(12);
        PokewalkerIr_Disconnect();
        return;
    }

    ov112_021FFAA4.transferStartTick = OS_GetTick();
    switch (ov112_021FFB24.command) {
    case 0xF0:
        PokewalkerIr_StoreClock();
        MI_CpuCopy8((u8 *)ov112_021FFAA4.identity + 0x10, ov112_021FFB64, 0x28);
        PokewalkerIr_Send(ov112_021FFB64, 0x74, ov112_021FFB24.command, 1);
        break;
    case 0xFE:
        PokewalkerIr_StoreClock();
        PokewalkerIr_Send((void *)ov112_021FFAA4.unk_28, 8, ov112_021FFB24.command, 1);
        break;
    case 0xE2:
        data[0] = 0x80;
        value = *(u16 *)&ov112_021FFAA4;
        data[1] = value >> 8;
        data[2] = value;
        data[3] = (data[1] + 1) + data[2];
        PokewalkerIr_Send(data, 4, 0xA, 0);
        break;
    case 0xE0:
        PokewalkerIr_StoreClock();
        PokewalkerIr_Send((u8 *)ov112_021FFAA4.identity + 0x10, 0x28, ov112_021FFB24.command, 1);
        break;
    case 0x2A:
        PokewalkerIr_StoreClock();
        PokewalkerIr_Send((u8 *)ov112_021FFAA4.identity + 0x10, 0x28, ov112_021FFB24.step, 1);
        PokewalkerIr_SetError(15);
        PokewalkerIr_Disconnect();
        break;
    case 0x32:
    case 0x40:
    case 0x52:
        PokewalkerIr_StoreCurrentTime();
        PokewalkerIr_Send(NULL, 0, 0x20, 1);
        break;
    case 0x60:
    case 0xB0:
    case 0xB2:
    case 0xB4:
    case 0xB6:
    case 0xB8:
    case 0xBA:
    case 0xBC:
    case 0xBE:
        ov112_021FFAA4.identity->currentTime = 0;
        PokewalkerIr_Send(NULL, 0, 0x20, 1);
        break;
    }
}

void PokewalkerIr_RestartTransfer(void) {
    ov112_021FFB24.data = ov112_021FFB24.dataStart;
    ov112_021FFB24.address = ov112_021FFB24.addressStart;
    ov112_021FFB24.remaining = ov112_021FFB24.size;
    ov112_021FFB24.chunk = 0;
    ov112_021FFB24.totalChunk = 0;
}

#define FAIL_RESPONSE(command, error)             \
    do {                                          \
        PokewalkerIr_Send(NULL, 0, (command), 1); \
        PokewalkerIr_SetError((error));           \
        PokewalkerIr_Disconnect();                \
        return;                                   \
    } while (0)

void PokewalkerIr_ValidatePeer(void) {
    u8 command = ov112_021FFB24.command;

    switch (command) {
    case 0x32:
        if (ov112_021FFAA4.unk_18 != 0 && ov112_021FFAA4.peerIdentity->unk_5B_0 != 0) {
            FAIL_RESPONSE(0x36, 4);
        }
        PokewalkerIr_Send(ov112_021FFAA4.identity, 0x68, 0x32, 1);
        return;

    case 0x40:
        if (ov112_021FFAA4.peerIdentity->unk_5B_0 == 0) {
            FAIL_RESPONSE(0x44, 5);
        }
        if (ov112_021FFAA4.peerIdentity->unk_5B_1 == 0) {
            FAIL_RESPONSE(0x44, 6);
        }
        if (ov112_021FFAA4.peerIdentity->unk_0C != ov112_021FFAA4.identity->unk_0C) {
            FAIL_RESPONSE(0x44, 11);
        }
        if (ov112_021FFAA4.peerIdentity->unk_04 != ov112_021FFAA4.identity->unk_04) {
            FAIL_RESPONSE(0x44, 8);
        }
        if (((u32)ov112_021FFAA4.peerIdentity->unk_0A >> 2) != ((u32)ov112_021FFAA4.identity->unk_0A >> 2)) {
            FAIL_RESPONSE(0x44, 9);
        }
        if (ov112_021FFAA4.peerIdentity->unk_5D > ov112_021FFAA4.identity->unk_5D) {
            FAIL_RESPONSE(0x44, 10);
        }
        PokewalkerIr_Send(ov112_021FFAA4.identity, 0x68, 0x40, 1);
        return;

    case 0x52:
        if (ov112_021FFAA4.peerIdentity->unk_5B_0 == 0) {
            FAIL_RESPONSE(0x56, 5);
        }
        if (ov112_021FFAA4.peerIdentity->unk_5B_1 == 1) {
            FAIL_RESPONSE(0x56, 7);
        }
        if (ov112_021FFAA4.peerIdentity->unk_00 != ov112_021FFAA4.identity->unk_00) {
            FAIL_RESPONSE(0x56, 8);
        }
        if (((u32)ov112_021FFAA4.peerIdentity->unk_08 >> 2) != ((u32)ov112_021FFAA4.identity->unk_08 >> 2)) {
            FAIL_RESPONSE(0x56, 9);
        }
        if (ov112_021FFAA4.peerIdentity->unk_0C != ov112_021FFAA4.identity->unk_0C) {
            FAIL_RESPONSE(0x56, 11);
        }
        if (ov112_021FFAA4.peerIdentity->unk_5D > ov112_021FFAA4.identity->unk_5D) {
            FAIL_RESPONSE(0x56, 10);
        }
        PokewalkerIr_Send(ov112_021FFAA4.identity, 0x68, 0x52, 1);
        return;

    case 0x60:
        if (ov112_021FFAA4.peerIdentity->unk_5B_0 == 0) {
            PokewalkerIr_Send(ov112_021FFAA4.identity, 0x68, 0x64, 1);
            PokewalkerIr_SetError(5);
            PokewalkerIr_Disconnect();
            return;
        }
        if (ov112_021FFAA4.peerIdentity->unk_5D > ov112_021FFAA4.identity->unk_5D) {
            PokewalkerIr_Send(ov112_021FFAA4.identity, 0x68, 0x64, 1);
            PokewalkerIr_SetError(10);
            PokewalkerIr_Disconnect();
            return;
        }
        PokewalkerIr_Send(ov112_021FFAA4.identity, 0x68, 0x60, 1);
        return;

    case 0xB0:
    case 0xB2:
    case 0xB4:
    case 0xB6:
    case 0xB8:
    case 0xBA:
    case 0xBC:
    case 0xBE:
        if (ov112_021FFAA4.peerIdentity->unk_5B_0 == 0) {
            FAIL_RESPONSE(0xD8, 5);
        }
        if (ov112_021FFAA4.peerIdentity->unk_00 != ov112_021FFAA4.identity->unk_00) {
            FAIL_RESPONSE(0xD8, 8);
        }
        if (((u32)ov112_021FFAA4.peerIdentity->unk_08 >> 2) != ((u32)ov112_021FFAA4.identity->unk_08 >> 2)) {
            FAIL_RESPONSE(0xD8, 9);
        }
        if (ov112_021FFAA4.peerIdentity->unk_5D > ov112_021FFAA4.identity->unk_5D) {
            FAIL_RESPONSE(0xD8, 10);
        }
        PokewalkerIr_Send(ov112_021FFAA4.identity, 0x68, (u8)(command - 0x10), 1);
        return;

    default:
        PokewalkerIr_SetError(12);
        PokewalkerIr_Disconnect();
        return;
    }
}

#pragma force_active on

void PokewalkerIr_OnReceive(void *data, u32 size, int command) {
    switch (command) {
    case 34: {
        u32 v;
        MI_CpuCopy8(data, ov112_021FFAA4.peerIdentity, 104);
        v = ov112_021FFAA4.peerIdentity->unk_64;
        ov112_021FFAA4.peerIdentity->unk_64 = (u8)(v >> 24) | ((v >> 8) & 0xFF00) | ((v << 8) & 0xFF0000) | ((v << 24) & 0xFF000000);
        PokewalkerIr_ValidatePeer();
    } break;
    case 38:
        switch (ov112_021FFB24.command) {
        case 50:
            PokewalkerIr_Send(0, 0, 56, 1);
            break;
        case 64:
            PokewalkerIr_Send(0, 0, 78, 1);
            PokewalkerIr_SetError(15);
            PokewalkerIr_Disconnect();
            break;
        case 82:
            PokewalkerIr_Send(0, 0, 90, 1);
            break;
        case 96:
            PokewalkerIr_Send(0, 0, 102, 1);
            PokewalkerIr_SetError(15);
            PokewalkerIr_Disconnect();
            break;
        case 176:
            PokewalkerIr_Send(0, 0, 192, 1);
            break;
        case 178:
            PokewalkerIr_Send(0, 0, 194, 1);
            break;
        case 180:
            PokewalkerIr_Send(0, 0, 196, 1);
            break;
        case 182:
            PokewalkerIr_Send(0, 0, 198, 1);
            break;
        case 184:
        case 186:
        case 188:
        case 190:
            if (ov112_021FFAA4.blocks->flags.handshake == 15) {
                switch (ov112_021FFAA4.blockIndex) {
                case 0:
                    PokewalkerIr_Send(0, 0, 208, 1);
                    break;
                case 1:
                    PokewalkerIr_Send(0, 0, 210, 1);
                    break;
                case 2:
                    PokewalkerIr_Send(0, 0, 212, 1);
                    break;
                case 3:
                    PokewalkerIr_Send(0, 0, 214, 1);
                    break;
                }
            } else {
                PokewalkerIr_Send(0, 0, ov112_021FFB24.command, 1);
            }
            break;
        }
        break;
    case 158:
        PokewalkerIr_SetError(12);
        PokewalkerIr_Disconnect();
        break;
    case 156:
        PokewalkerIr_SetError(12);
        PokewalkerIr_Disconnect();
        break;
    case 40:
        PokewalkerIr_SetError(3);
        PokewalkerIr_Disconnect();
        break;
    case 52:
        ov112_021FFB24.step = 58;
        if (ov112_021FFAA4.upload0) {
            PokewalkerIr_BeginTransfer(ov112_021FFAA4.upload0, 640, 0x8C50);
        } else {
            PokewalkerIr_BeginTransfer(ov112_021FFAA4.unk_10, 640, 0x8C50);
        }
        PokewalkerIr_SendChunk();
        break;
    case 56:
        PokewalkerIr_SetError(15);
        PokewalkerIr_Disconnect();
        break;
    case 66:
        ov112_021FFB24.step = 70;
        PokewalkerIr_BeginTransfer((void *)0x8F00, (u32)ov112_021FFAA4.unk_08, 42);
        PokewalkerIr_RequestChunk();
        break;
    case 68:
        PokewalkerIr_SetError(14);
        PokewalkerIr_Disconnect();
        break;
    case 84:
        ov112_021FFB24.step = 60;
        if (ov112_021FFAA4.upload1) {
            PokewalkerIr_BeginTransfer(ov112_021FFAA4.upload1, 0xD700, 0x28BE);
        } else {
            PokewalkerIr_BeginTransfer(ov112_021FFAA4.unk_2C, 0xD700, 0x28BE);
        }
        PokewalkerIr_SendChunk();
        break;
    case 90:
        PokewalkerIr_SetError(15);
        PokewalkerIr_Disconnect();
        break;
    case 94:
        PokewalkerIr_SetError(15);
        PokewalkerIr_Disconnect();
        break;
    case 98:
        ov112_021FFB24.step = 72;
        PokewalkerIr_BeginTransfer((void *)0xCE80, (u32)ov112_021FFAA4.status, 0xD4C);
        PokewalkerIr_RequestChunk();
        break;
    case 100:
        PokewalkerIr_SetError(14);
        PokewalkerIr_Disconnect();
        break;
    case 160:
    case 162:
    case 164:
    case 166:
    case 168:
    case 170:
    case 172:
    case 174:
        ov112_021FFB24.step = 72;
        PokewalkerIr_BeginTransfer((void *)0xCE80, (u32)ov112_021FFAA4.status, 0xD4C);
        PokewalkerIr_RequestChunk();
        break;
    case 192:
    case 194:
    case 196:
    case 198:
    case 200:
    case 202:
    case 204:
    case 206:
        PokewalkerIr_SetError(15);
        PokewalkerIr_Disconnect();
        break;
    case 240:
        PokewalkerIr_SetError(15);
        PokewalkerIr_Disconnect();
        break;
    case 254:
        PokewalkerIr_SetError(15);
        PokewalkerIr_Disconnect();
        break;
    case 226:
        PokewalkerIr_SetError(15);
        PokewalkerIr_Disconnect();
        break;
    case 224:
        PokewalkerIr_SetError(15);
        PokewalkerIr_Disconnect();
        break;
    case 4:
        if (ov112_021FFB24.remaining == 0) {
            PokewalkerIr_OnSendComplete();
        } else {
            PokewalkerIr_SendChunk();
        }
        break;
    case 14:
        MI_CpuCopy8(data, (void *)ov112_021FFB24.address, size);
        ov112_021FFB24.data += size;
        ov112_021FFB24.address = ov112_021FFB24.address + size;
        ov112_021FFB24.remaining -= size;
        ov112_021FFB24.chunk++;
        ov112_021FFB24.totalChunk++;
        if (ov112_021FFB24.remaining == 0) {
            PokewalkerIr_OnReceiveComplete();
        } else {
            PokewalkerIr_RequestChunk();
        }
        break;
    case 244:
        break;
    }
}

#pragma force_active reset

void PokewalkerIr_ThreadMain(void *arg) {
    for (;;) {
        OS_LockMutex(&ov112_021FFB08);
        if (PokewalkerIr_Poll() == 1) {
            PokewalkerIr_SetError(2);
        }
        OS_UnlockMutex(&ov112_021FFB08);
        OS_Sleep(4);
    }
}

void PokewalkerIr_Start(void *stack, u32 stackSize, u32 priority) {
    ov112_021FFAA4.unk_18 = 1;
    ov112_021FFAA4.error = 1;
    ov112_021FFAA4.compressedSizes = NULL;
    ov112_021FFAA4.compressBuffer = NULL;
    PokewalkerIr_InitLink();
    ov112_021E5D5C(1);
    PokewalkerIr_SetConnectCallback(PokewalkerIr_OnConnect);
    PokewalkerIr_SetReceiveCallback(PokewalkerIr_OnReceive);
    PokewalkerIr_SetCloseCallback(PokewalkerIr_RestartTransfer);
    CARD_SpiWaitGetStatus();
    if (CARD_SpiWaitGetStatus() == 0xAA) {
        CARD_SetSpiWriteWaitCycles(0);
        CARD_SetSpiReadWaitCycles(0);
    }
    OS_InitMutex(&ov112_021FFB08);
    OS_InitMessageQueue(&ov112_021FFB20, ov112_021FFAC0, 1);
    OS_CreateThread(&ov112_021FFBD8, PokewalkerIr_ThreadMain, NULL, (u8 *)stack + stackSize, stackSize, priority);
    OS_WakeupThreadDirect(&ov112_021FFBD8);
}

void PokewalkerIr_Stop(void) {
    OS_LockMutex(&ov112_021FFB08);
    OS_KillThread(&ov112_021FFBD8, NULL);
    OS_UnlockMutex(&ov112_021FFB08);
}

void ov112_021E7484(int active) {
    ov112_021FFB24.command = 0x2A;
    if (active != 0) {
        ov112_021FFB24.step = 0x2A;
    } else {
        ov112_021FFB24.step = 0x2C;
    }
    PokewalkerIr_BeginTransfer(NULL, 0, 0x28);
}
