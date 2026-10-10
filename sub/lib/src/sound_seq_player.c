#pragma force_active on
#include <nitro/types.h>
// The SDK headers below expect <nitro/types.h> to be included first.
#include <nitro/SND_bank_shared.h>
#include <nitro/SND_work_shared.h>
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#define SND_PLAYER_COUNT_ 16
#define SND_TRACK_COUNT_  32

// Sequence data is read through a 16-byte cache. The original code addresses the cache bounds
// through sMmlPrintEnable, the word that precedes the cache (seqCache), so this view of
// sMmlPrintEnable spans both.
typedef struct SeqCacheView {
    u32 mmlPrintEnable;
    const u8 *start;
    const u8 *end;
    u32 words[4];
} SeqCacheView;

extern SeqCacheView sMmlPrintEnable;
extern u8 seqCache[];
extern const s16 SNDi_DecibelTable[];

u16 SND_CalcRandom(void);
void SND_StopIntervalTimer(void);
void SND_StartIntervalTimer(void);
int SND_ReadInstData(struct SNDBankData *bank, int index, int key, struct SNDInstData *out);
struct SNDExChannel *SND_AllocExChannel(u32 mask, int priority, int weak, SNDExChannelCallback callback, void *userData);
int SND_NoteOn(struct SNDExChannel *chn, int key, int velocity, int length, const struct SNDBankData *bank, const struct SNDInstData *inst);
void SND_FreeExChannel(struct SNDExChannel *chn);
void SND_ReleaseExChannel(struct SNDExChannel *chn);
int SND_IsExChannelActive(struct SNDExChannel *chn);
void SND_SetExChannelAttack(struct SNDExChannel *chn, int attack);
void SND_SetExChannelDecay(struct SNDExChannel *chn, int decay);
void SND_SetExChannelSustain(struct SNDExChannel *chn, int sustain);
void SND_SetExChannelRelease(struct SNDExChannel *chn, int release);
void SND_InitLfoParam(struct SNDLfoParam *param);

void SND_SeqInit(void);
void SND_SeqMain(int update);
void SND_PrepareSeq(int player, const u8 *data, u32 offset, struct SNDBankData *bank);
u8 ReadByte(struct SNDTrack *track);
void SND_StartPreparedSeq(int player);
void SND_StartSeq(int player, const u8 *data, u32 offset, struct SNDBankData *bank);
void SND_StopSeq(int player);
void SND_PauseSeq(int player, int pause);
void SND_SkipSeq(int player, u32 ticks);
void SND_SetTrackMute(int player, u32 trackMask, int mute);
void SND_SetTrackAllocatableChannel(int player, u32 trackMask, u32 channelMask);
void SND_InvalidateSeq(const void *start, const void *end);
void SND_InvalidateBank(const void *start, const void *end);
void SNDi_SetPlayerParam(int player, u32 offset, u32 value, int size);
void SNDi_SetTrackParam(int player, u32 trackMask, u32 offset, u32 value, int size);
void InitCache(const u8 *pos);
u32 Read24(struct SNDTrack *track);
int ReadArg(struct SNDTrack *track, struct SNDPlayer *player, int type);
void InitTrack(struct SNDTrack *track);
void StartTrack(struct SNDTrack *track, const u8 *base, u32 offset);
void ReleaseTrackChannelAll(struct SNDTrack *track, struct SNDPlayer *player, int release);
void FreeTrackChannelAll(struct SNDTrack *track);
struct SNDTrack *GetPlayerTrack(struct SNDPlayer *player, int index);
void ClosePlayerTrack(struct SNDPlayer *player, int index);
void FinishPlayer(struct SNDPlayer *player);
void ChannelCallback(struct SNDExChannel *chn, int status, struct SNDTrack *track);
void UpdateTrackChannel(struct SNDTrack *track, struct SNDPlayer *player, BOOL release);
int PlayerSeqMain(struct SNDPlayer *player, int playNotes);
s16 *GetVariablePtr(struct SNDPlayer *player, int index);
int AllocTrack(void);
void SetTrackMute(struct SNDTrack *track, struct SNDPlayer *player, int mute);

// -ipa file would otherwise inline these small functions into their callers in this file.
#pragma dont_inline on

void SND_SeqInit(void) {
    int i;
    for (i = 0; i < SND_PLAYER_COUNT_; i++) {
        struct SNDPlayer *player = SNDi_Work.players + i;
        player->flags.active = 0;
        player->playerId = i;
    }
    struct SNDTrack *tracks = SNDi_Work.tracks;
    for (i = 0; i < SND_TRACK_COUNT_; i++) {
        tracks[i].flags.active = 0;
    }
}

// The tempo counter advances by tempo * tempoRatio / 256 per tick; 240 counts make one sequence tick.
void SND_SeqMain(int update) {
    struct SNDPlayer *player;
    int i;
    u32 active = 0;
    int count, ticks;
    for (i = 0; i < SND_PLAYER_COUNT_; i++) {
        player = SNDi_Work.players + i;
        if (!player->flags.active) {
            continue;
        }
        if (player->flags.prepared) {
            if (update && !player->flags.paused) {
                ticks = 0;
                while (player->tempoCounter >= 240) {
                    player->tempoCounter -= 240;
                    ticks++;
                }
                for (count = 0; count < ticks; count++) {
                    if (PlayerSeqMain(player, 1)) {
                        FinishPlayer(player);
                        break;
                    }
                }
                if (SNDi_SharedWork) {
                    ((u32 *)((u8 *)SNDi_SharedWork + player->playerId * 36))[16] += count;
                }
                {
                    int tempo = player->tempo;
                    tempo *= player->tempoRatio;
                    tempo >>= 8;
                    tempo += player->tempoCounter;
                    player->tempoCounter = tempo;
                }
            }
            for (ticks = 0; ticks < SND_TRACK_COUNT_PER_PLAYER; ticks++) {
                struct SNDTrack *track = GetPlayerTrack(player, ticks);
                if (track) {
                    UpdateTrackChannel(track, player, 1);
                }
            }
        }
        if (player->flags.active) {
            active |= 1u << i;
        }
    }
    if (SNDi_SharedWork) {
        SNDi_SharedWork->playerStatus = active;
    }
}

#pragma dont_inline reset

static inline void InitPlayer(struct SNDPlayer *player, struct SNDBankData *bank) {
    int i;
    if (player->flags.active) {
        FinishPlayer(player);
    }
    player->flags.paused = 0;
    player->bank = bank;
    player->tempo = 120;
    player->tempoRatio = 256;
    player->tempoCounter = 240;
    player->volume = 127;
    player->extFader = 0;
    player->prio = 64;
    for (i = 0; i < SND_TRACK_COUNT_PER_PLAYER; i++) {
        player->tracks[i] = SND_INVALID_TRACK_INDEX;
    }
    if (SNDi_SharedWork) {
        SNDi_SharedWork->players[player->playerId].tickCounter = 0;
        for (i = 0; i < 16; i++) {
            SNDi_SharedWork->players[player->playerId].localVars[i] = -1;
        }
    }
}

// A sequence may start with an 0xFE "allocate tracks" command (bit mask of extra tracks).
void SND_PrepareSeq(int player, const u8 *data, u32 offset, struct SNDBankData *bank) {
    int i, num;

    struct {
        struct SNDPlayer *player;
        struct SNDTrack *track;
    } state;

    state.player = &SNDi_Work.players[player];
    InitPlayer(state.player, bank);
    num = AllocTrack();
    if (num < 0) {
        return;
    }
    state.track = &SNDi_Work.tracks[num];
    InitTrack(state.track);
    state.track->base = data;
    state.track->cur = data + offset;
    state.player->tracks[0] = num;
    InitCache(state.track->cur);
    if (ReadByte(state.track) != 0xfe) {
        state.track->cur--;
    } else {
        u16 mask;
        int low = ReadByte(state.track);
        mask = (u16)(low | (ReadByte(state.track) << 8));
        mask >>= 1;
        for (i = 1; mask; i++, mask = (u16)(mask >> 1)) {
            if (mask & 1) {
                num = AllocTrack();
                if (num < 0) {
                    break;
                }
                InitTrack(&SNDi_Work.tracks[num]);
                state.player->tracks[i] = num;
            }
        }
    }
    state.player->flags.active = 1;
    state.player->flags.prepared = 0;
    if (SNDi_SharedWork) {
        SNDi_SharedWork->playerStatus |= 1u << player;
    }
}

// -ipa file would otherwise inline these small functions into their callers in this file.
#pragma dont_inline on

u8 ReadByte(struct SNDTrack *track) {
    const u8 *pos = track->cur;
    int value;
    if ((u32)pos < (u32)sMmlPrintEnable.start || (u32)pos >= (u32)sMmlPrintEnable.end) {
        InitCache(pos);
    }
    value = (seqCache + 8)[(u32)pos - (u32)sMmlPrintEnable.start];
    track->cur++;
    return value;
}

void SND_StartPreparedSeq(int player) {
    SNDi_Work.players[player].flags.prepared = 1;
}

void SND_StartSeq(int player, const u8 *data, u32 offset, struct SNDBankData *bank) {
    SND_PrepareSeq(player, data, offset, bank);
    SNDi_Work.players[player].flags.prepared = 1;
}

void SND_StopSeq(int player) {
    struct SNDPlayer *ply = SNDi_Work.players + player;
    if (ply->flags.active) {
        FinishPlayer(ply);
        if (SNDi_SharedWork) {
            SNDi_SharedWork->playerStatus &= ~(1u << player);
        }
    }
}

void SND_PauseSeq(int player, int pause) {
    struct SNDPlayer *ply = SNDi_Work.players + player;
    int i;
    ply->flags.paused = (u8)pause;
    if (pause) {
        for (i = 0; i < SND_TRACK_COUNT_PER_PLAYER; i++) {
            struct SNDTrack *track = GetPlayerTrack(ply, i);
            if (track) {
                ReleaseTrackChannelAll(track, ply, 127);
                FreeTrackChannelAll(track);
            }
        }
    }
}

void SND_SkipSeq(int player, u32 ticks) {
    struct SNDPlayer *ply = SNDi_Work.players + player;
    int i;
    u32 count;
    for (i = 0; i < SND_TRACK_COUNT_PER_PLAYER; i++) {
        struct SNDTrack *track = GetPlayerTrack(ply, i);
        if (track) {
            ReleaseTrackChannelAll(track, ply, 127);
            FreeTrackChannelAll(track);
        }
    }
    SND_StopIntervalTimer();
    for (count = 0; count < ticks; count++) {
        if (PlayerSeqMain(ply, 0)) {
            FinishPlayer(ply);
            break;
        }
    }
    SND_StartIntervalTimer();
    if (SNDi_SharedWork) {
        *(u32 *)((u8 *)SNDi_SharedWork + 64 + ply->playerId * 36) += count;
    }
}

void SND_SetTrackMute(int player, u32 trackMask, int mute) {
    int i;
    for (i = 0; i < SND_TRACK_COUNT_PER_PLAYER && trackMask; i++, trackMask >>= 1) {
        if (trackMask & 1) {
            struct SNDPlayer *ply = SNDi_Work.players + player;
            struct SNDTrack *track = GetPlayerTrack(ply, i);
            if (track) {
                SetTrackMute(track, ply, mute);
            }
        }
    }
}

void SND_SetTrackAllocatableChannel(int player, u32 trackMask, u32 channelMask) {
    int i;
    for (i = 0; i < SND_TRACK_COUNT_PER_PLAYER && trackMask; i++, trackMask >>= 1) {
        if (trackMask & 1) {
            struct SNDTrack *track = GetPlayerTrack(SNDi_Work.players + player, i);
            if (track) {
                track->channelMask = (u16)channelMask;
                track->flags.channelMask = 1;
            }
        }
    }
}

void SND_InvalidateSeq(const void *start, const void *end) {
    int i, j;
    for (i = 0; i < SND_PLAYER_COUNT_; i++) {
        struct SNDPlayer *ply = SNDi_Work.players + i;
        if (ply->flags.active) {
            for (j = 0; j < SND_TRACK_COUNT_PER_PLAYER; j++) {
                struct SNDTrack *track = GetPlayerTrack(ply, j);
                if (track && (u32)start <= (u32)track->cur && (u32)track->cur <= (u32)end) {
                    FinishPlayer(ply);
                    break;
                }
            }
        }
    }
}

void SND_InvalidateBank(const void *start, const void *end) {
    int i;
    for (i = 0; i < SND_PLAYER_COUNT_; i++) {
        struct SNDPlayer *ply = SNDi_Work.players + i;
        if (ply->flags.active && (u32)start <= (u32)ply->bank && (u32)ply->bank <= (u32)end) {
            FinishPlayer(ply);
        }
    }
}

// Writes a 1/2/4-byte value at a byte offset inside a player (ARM9 SNDi_SetPlayerParam).
void SNDi_SetPlayerParam(int player, u32 offset, u32 value, int size) {
    u8 *ply = (u8 *)SNDi_Work.players + player * 36;
    switch (size) {
    case 1:
        *(u8 *)(ply + offset) = value;
        break;
    case 2:
        *(u16 *)(ply + offset) = value;
        break;
    case 4:
        *(u32 *)(ply + offset) = value;
        break;
    }
}

void SNDi_SetTrackParam(int player, u32 trackMask, u32 offset, u32 value, int size) {
    int i;
    for (i = 0; i < SND_TRACK_COUNT_PER_PLAYER && trackMask; i++, trackMask >>= 1) {
        if (trackMask & 1) {
            struct SNDTrack *track = GetPlayerTrack(SNDi_Work.players + player, i);
            if (track) {
                switch (size) {
                case 1:
                    *(u8 *)((u8 *)track + offset) = value;
                    break;
                case 2:
                    *(u16 *)((u8 *)track + offset) = value;
                    break;
                case 4:
                    *(u32 *)((u8 *)track + offset) = value;
                    break;
                }
            }
        }
    }
}

void InitCache(const u8 *pos) {
    const u32 *src = (const u32 *)((u32)pos & ~3u);
    sMmlPrintEnable.start = (const u8 *)src;
    sMmlPrintEnable.end = (const u8 *)src + 16;
    sMmlPrintEnable.words[0] = src[0];
    sMmlPrintEnable.words[1] = src[1];
    sMmlPrintEnable.words[2] = src[2];
    sMmlPrintEnable.words[3] = src[3];
}

u32 Read24(struct SNDTrack *track) {
    u32 value = ReadByte(track);
    value |= (u32)ReadByte(track) << 8;
    value |= (u32)ReadByte(track) << 16;
    return value;
}

// Argument types: 0 u8, 1 u16, 2 variable-length, 3 random in [min, max], 4 variable.
int ReadArg(struct SNDTrack *track, struct SNDPlayer *player, int type) {
    int result;
    switch (type) {
    case 0:
        result = ReadByte(track);
        break;
    case 1: {
        int low = ReadByte(track);
        result = (u16)(low | (ReadByte(track) << 8));
        break;
    }
    case 2: {
        u32 value = 0;
        int byte;
        do {
            byte = ReadByte(track);
            value = ((u32)byte & 0x7f) | (value << 7);
        } while (byte & 0x80);
        result = (s32)value;
        break;
    }
    case 4: {
        int index = ReadByte(track);
        s16 *variable = GetVariablePtr(player, index);
        if (variable) {
            result = *variable;
        }
        break;
    }
    case 3: {
        int low = ReadByte(track);
        s16 min = (u16)(low | (ReadByte(track) << 8));
        int high = ReadByte(track);
        u16 max = (u16)(high | (ReadByte(track) << 8));
        u32 random = SND_CalcRandom();
        result = ((s32)(random * (u32)((s32)(s16)max - min + 1)) >> 16) + min;
        break;
    }
    }
    return result;
}

void InitTrack(struct SNDTrack *track) {
    track->base = NULL;
    track->cur = NULL;
    track->flags.noteWait = 1;
    track->flags.muted = 0;
    track->flags.tie = 0;
    track->flags.noteFinishWait = 0;
    track->flags.portamento = 0;
    track->flags.cmp = 1;
    track->flags.channelMask = 0;
    track->callStackDepth = 0;
    track->program = 0;
    track->priority = 64;
    track->volume = 127;
    track->expression = 127;
    track->extFader = 0;
    track->pan = 0;
    track->extPan = 0;
    track->pitchBend = 0;
    track->extPitch = 0;
    track->envAttack = 255;
    track->envDecay = 255;
    track->envSustain = 255;
    track->envRelease = 255;
    track->panRange = 127;
    track->bendRange = 2;
    track->portamentoKey = 60;
    track->portamentoTime = 0;
    track->sweepPitch = 0;
    track->transpose = 0;
    track->channelMask = 0xffff;
    SND_InitLfoParam(&track->mod);
    track->wait = 0;
    track->channelLLHead = NULL;
}

void StartTrack(struct SNDTrack *track, const u8 *base, u32 offset) {
    track->base = base;
    track->cur = base + offset;
}

void ReleaseTrackChannelAll(struct SNDTrack *track, struct SNDPlayer *player, int release) {
    struct SNDExChannel *chn;
    u8 level;
    UpdateTrackChannel(track, player, 0);
    chn = track->channelLLHead;
    level = (u8)release;
    for (; chn; chn = chn->channelLLNext) {
        if (SND_IsExChannelActive(chn)) {
            if (release >= 0) {
                SND_SetExChannelRelease(chn, level);
            }
            chn->priority = 1;
            SND_ReleaseExChannel(chn);
        }
    }
}

void FreeTrackChannelAll(struct SNDTrack *track) {
    struct SNDExChannel *chn;
    for (chn = track->channelLLHead; chn; chn = chn->channelLLNext) {
        SND_FreeExChannel(chn);
    }
    track->channelLLHead = NULL;
}

struct SNDTrack *GetPlayerTrack(struct SNDPlayer *player, int index) {
    int track;
    if (index > SND_TRACK_COUNT_PER_PLAYER - 1) {
        return NULL;
    }
    track = player->tracks[index];
    if (track == SND_INVALID_TRACK_INDEX) {
        return NULL;
    }
    return SNDi_Work.tracks + track;
}

void ClosePlayerTrack(struct SNDPlayer *player, int index) {
    struct SNDTrack *track = GetPlayerTrack(player, index);
    if (track) {
        ReleaseTrackChannelAll(track, player, -1);
        FreeTrackChannelAll(track);
        SNDi_Work.tracks[player->tracks[index]].flags.active = 0;
        player->tracks[index] = SND_INVALID_TRACK_INDEX;
    }
}

void FinishPlayer(struct SNDPlayer *player) {
    int i;
    for (i = 0; i < SND_TRACK_COUNT_PER_PLAYER; i++) {
        ClosePlayerTrack(player, i);
    }
    player->flags.active = 0;
}

// Unlinks a finished channel from its track's channel list.
void ChannelCallback(struct SNDExChannel *chn, int status, struct SNDTrack *track) {
    struct SNDExChannel *cur;
    if (status == 1) {
        chn->priority = 0;
        SND_FreeExChannel(chn);
    }
    cur = track->channelLLHead;
    if (cur == chn) {
        track->channelLLHead = chn->channelLLNext;
        return;
    }
    while (cur->channelLLNext) {
        if (cur->channelLLNext == chn) {
            cur->channelLLNext = chn->channelLLNext;
            break;
        }
        cur = cur->channelLLNext;
    }
}

// Pushes the track and player volume, pitch and pan to every channel the track plays.
void UpdateTrackChannel(struct SNDTrack *track, struct SNDPlayer *player, BOOL release) {
    struct SNDExChannel *chn;
    int volume;
    int fader;
    int pan;
    int pitch;
    volume = SNDi_DecibelTable[track->volume] + SNDi_DecibelTable[track->expression] + SNDi_DecibelTable[player->volume];
    fader = track->extFader + player->extFader;
    pitch = track->pitchBend;
    pitch *= track->bendRange << 6;
    pitch >>= 7;
    pitch += track->extPitch;
    pan = track->pan;
    if (track->panRange != 127) {
        pan = (pan * track->panRange + 64) >> 7;
    }
    pan += track->extPan;
    if (volume < -0x8000) {
        volume = -0x8000;
    }
    if (fader < -0x8000) {
        fader = -0x8000;
    }
    if (pan < -0x80) {
        pan = -0x80;
    } else if (pan > 0x7f) {
        pan = 0x7f;
    }
    for (chn = track->channelLLHead; chn != NULL; chn = chn->channelLLNext) {
        chn->userDecay2 = fader;
        if (chn->envStatus == SND_ENV_RELEASE) {
            continue;
        }
        chn->userDecay = volume;
        chn->userPitch = pitch;
        chn->userPan = pan;
        chn->panRange = track->panRange;
        // the modulation parameters are copied as three halfwords
        ((u16 *)&chn->lfo.param)[0] = ((u16 *)&track->mod)[0];
        ((u16 *)&chn->lfo.param)[1] = ((u16 *)&track->mod)[1];
        ((u16 *)&chn->lfo.param)[2] = ((u16 *)&track->mod)[2];
        if (chn->length == 0 && release) {
            chn->priority = 1;
            SND_ReleaseExChannel(chn);
        }
    }
}

#pragma dont_inline reset

// Runs one tick of a track's command stream; returns -1 when the track ends.
static inline int TrackSeqMain(struct SNDTrack *track, struct SNDPlayer *player, int trackIndex, int playNotes) {
    for (struct SNDExChannel *chn = track->channelLLHead; chn; chn = chn->channelLLNext) {
        if (chn->length > 0) {
            chn->length--;
        }
        if (!chn->flags.autoSweep && chn->sweepCounter < chn->sweepLength) {
            chn->sweepCounter++;
        }
    }
    if (track->flags.noteFinishWait) {
        if (track->channelLLHead) {
            return 0;
        }
        track->flags.noteFinishWait = 0;
    }
    if (track->wait > 0) {
        track->wait--;
        if (track->wait > 0) {
            return 0;
        }
    }
    InitCache(track->cur);
    while (track->wait == 0 && !track->flags.noteFinishWait) {
        int modified;
        int execute;
        int opcode;
        int argType;
        modified = 0;
        execute = 1;
        opcode = ReadByte(track);
        if (opcode == 0xa2) {
            opcode = ReadByte(track);
            execute = track->flags.cmp;
        }
        if (opcode == 0xa0) {
            opcode = ReadByte(track);
            argType = 3;
            modified = 1;
        }
        if (opcode == 0xa1) {
            opcode = ReadByte(track);
            argType = 4;
            modified = 1;
        }
        if (!(opcode & 0x80)) {
            u8 velocity = ReadByte(track);
            s32 length = ReadArg(track, player, modified ? argType : 2);
            int key = (int)opcode + track->transpose;
            if (!execute) {
                continue;
            }
            if (key < 0) {
                key = 0;
            } else if (key > 127) {
                key = 127;
            }
            if (!track->flags.muted && playNotes) {
                struct SNDExChannel *chn = 0;
                if (track->flags.tie) {
                    chn = track->channelLLHead;
                    if (chn) {
                        chn->midiKey = key;
                        chn->velocity = velocity;
                    }
                }
                if (chn == 0) {
                    struct SNDInstData inst;
                    if (!SND_ReadInstData(player->bank, track->program, key, &inst)) {
                        goto note_done;
                    }
                    u32 mask;
                    switch (inst.type) {
                    case SND_INST_PCM:
                    case SND_INST_DIRECTPCM:
                        mask = 0xffff;
                        break;
                    case SND_INST_PSG:
                        mask = 0x3f00;
                        break;
                    case SND_INST_NOISE:
                        mask = 0xc000;
                        break;
                    default:
                        goto note_done;
                    }
                    mask &= track->channelMask;
                    chn = SND_AllocExChannel(mask, player->prio + track->priority, track->flags.channelMask, (SNDExChannelCallback)ChannelCallback, track);
                    if (!chn) {
                        goto note_done;
                    }
                    if (!SND_NoteOn(chn, key, velocity, track->flags.tie ? -1 : (length > 0 ? length : -1), player->bank, &inst)) {
                        chn->priority = 0;
                        SND_FreeExChannel(chn);
                        goto note_done;
                    }
                    chn->channelLLNext = track->channelLLHead;
                    track->channelLLHead = chn;
                }
                if (track->envAttack != 255) {
                    SND_SetExChannelAttack(chn, track->envAttack);
                }
                if (track->envDecay != 255) {
                    SND_SetExChannelDecay(chn, track->envDecay);
                }
                if (track->envSustain != 255) {
                    SND_SetExChannelSustain(chn, track->envSustain);
                }
                if (track->envRelease != 255) {
                    SND_SetExChannelRelease(chn, track->envRelease);
                }
                chn->sweepPitch = track->sweepPitch;
                if (track->flags.portamento) {
                    chn->sweepPitch = (s16)(chn->sweepPitch + (s16)((u32)((int)track->portamentoKey - key) << 6));
                }
                if (!track->portamentoTime) {
                    chn->sweepLength = length > 0 ? length : -1;
                    chn->flags.autoSweep = 0;
                } else {
                    int time = track->portamentoTime * track->portamentoTime;
                    int distance = chn->sweepPitch;
                    if (distance < 0) {
                        distance = -distance;
                    }
                    chn->sweepLength = (time * distance) >> 11;
                }
                chn->sweepCounter = 0;
            }
note_done:
            track->portamentoKey = key;
            if (track->flags.noteWait) {
                track->wait = length;
                if (length == 0) {
                    track->flags.noteFinishWait = 1;
                }
            }
            continue;
        }
        switch (opcode & 0xf0) {
        case 0x80: {
            s32 value = ReadArg(track, player, modified ? argType : 2);
            if (!execute) {
                break;
            }
            switch (opcode) {
            case 0x80:
                track->wait = value;
                break;
            case 0x81:
                if (value < 65536) {
                    track->program = (u16)value;
                }
                break;
            }
            break;
        }
        case 0x90:
            switch (opcode) {
            case 0x93: {
                u32 destination = ReadByte(track), offset = Read24(track);
                if (execute) {
                    struct SNDTrack *other = GetPlayerTrack(player, destination);
                    if (other && other != track) {
                        ReleaseTrackChannelAll(other, player, -1);
                        FreeTrackChannelAll(other);
                        StartTrack(other, track->base, offset);
                    }
                }
            } break;
            case 0x94: {
                u32 offset = Read24(track);
                if (execute) {
                    track->cur = track->base + offset;
                }
            } break;
            case 0x95: {
                u32 offset = Read24(track);
                if (execute && track->callStackDepth < SND_TRACK_MAX_CALL) {
                    track->posCallStack[track->callStackDepth] = track->cur;
                    track->callStackDepth++;
                    track->cur = track->base + offset;
                }
            } break;
            }
            break;
        case 0xc0:
        case 0xd0: {
            u8 raw = (u8)ReadArg(track, player, modified ? argType : 0);
            if (!execute) {
                break;
            }
            switch (opcode) {
            case 0xc1:
                track->volume = raw;
                break;
            case 0xd5:
                track->expression = raw;
                break;
            case 0xc2:
                player->volume = raw;
                break;
            case 0xc5:
                track->bendRange = raw;
                break;
            case 0xc6:
                track->priority = raw;
                break;
            case 0xc7:
                track->flags.noteWait = raw;
                break;
            case 0xcf:
                track->portamentoTime = raw;
                break;
            case 0xca:
                track->mod.depth = raw;
                break;
            case 0xcb:
                track->mod.speed = raw;
                break;
            case 0xcc:
                track->mod.target = raw;
                break;
            case 0xcd:
                track->mod.range = raw;
                break;
            case 0xd0:
                track->envAttack = raw;
                break;
            case 0xd1:
                track->envDecay = raw;
                break;
            case 0xd2:
                track->envSustain = raw;
                break;
            case 0xd3:
                track->envRelease = raw;
                break;
            case 0xd4:
                if (track->callStackDepth < SND_TRACK_MAX_CALL) {
                    track->posCallStack[track->callStackDepth] = track->cur;
                    track->loopCount[track->callStackDepth] = raw;
                    track->callStackDepth++;
                }
                break;
            case 0xc8:
                track->flags.tie = raw;
                ReleaseTrackChannelAll(track, player, -1);
                FreeTrackChannelAll(track);
                break;
            case 0xd7:
                SetTrackMute(track, player, raw);
                break;
            case 0xc9:
                track->portamentoKey = raw + track->transpose;
                track->flags.portamento = 1;
                break;
            case 0xce:
                track->flags.portamento = raw;
                break;
            case 0xc3:
                track->transpose = *(s8 *)&raw;
                break;
            case 0xc4:
                track->pitchBend = *(s8 *)&raw;
                break;
            case 0xc0:
                track->pan = (s8)(raw - 64);
                break;
            case 0xd6:
                if (sMmlPrintEnable.mmlPrintEnable) {
                    GetVariablePtr(player, raw);
                }
                break;
            }
            break;
        }
        case 0xe0: {
            s16 value = (s16)ReadArg(track, player, modified ? argType : 1);
            if (!execute) {
                break;
            }
            switch (opcode) {
            case 0xe3:
                track->sweepPitch = value;
                break;
            case 0xe1:
                player->tempo = value;
                break;
            case 0xe0:
                track->mod.delay = value;
                break;
            }
            break;
        }
        case 0xb0: {
            u32 variable = ReadByte(track);
            s16 arg = (s16)ReadArg(track, player, modified ? argType : 1);
            s16 *value = GetVariablePtr(player, variable);
            if (!execute || !value) {
                break;
            }
            switch (opcode) {
            case 0xb0:
                *value = arg;
                break;
            case 0xb1:
                *value = (s16)(*value + arg);
                break;
            case 0xb2:
                *value = (s16)(*value - arg);
                break;
            case 0xb3:
                *value = (s16)((s32)*value * arg);
                break;
            case 0xb4:
                if (arg) {
                    *value = (s16)((s32)*value / arg);
                }
                break;
            case 0xb5:
                if (arg >= 0) {
                    *value = (s16)(*value << arg);
                } else {
                    *value = (s16)(*value >> -arg);
                }
                break;
            case 0xb6: {
                int negative = 0;
                if (arg < 0) {
                    arg = (s16) - (s32)arg;
                    negative = 1;
                }
                s32 result = SND_CalcRandom();
                result *= arg + 1;
                result >>= 16;
                if (negative) {
                    result = -result;
                }
                *value = (s16)result;
                break;
            }
            case 0xb8:
                track->flags.cmp = (u8)(*value == arg);
                break;
            case 0xb9:
                track->flags.cmp = (u8)(*value >= arg);
                break;
            case 0xba:
                track->flags.cmp = (u8)(*value > arg);
                break;
            case 0xbb:
                track->flags.cmp = (u8)(*value <= arg);
                break;
            case 0xbc:
                track->flags.cmp = (u8)(*value < arg);
                break;
            case 0xbd:
                track->flags.cmp = (u8)(*value != arg);
                break;
            }
            break;
        }
        case 0xf0:
            if (execute) {
                switch (opcode) {
                case 0xfd:
                    if (track->callStackDepth) {
                        track->callStackDepth--;
                        track->cur = track->posCallStack[track->callStackDepth];
                    }
                    break;
                case 0xfc:
                    if (track->callStackDepth) {
                        u8 count = track->loopCount[track->callStackDepth - 1];
                        if (count) {
                            count--;
                            if (!count) {
                                track->callStackDepth--;
                                break;
                            }
                        }
                        track->loopCount[track->callStackDepth - 1] = count;
                        track->cur = track->posCallStack[track->callStackDepth - 1];
                    }
                    break;
                case 0xfe:
                    break;
                case 0xff:
                    return -1;
                }
            }
            break;
        }
    }
    return 0;
}

int PlayerSeqMain(struct SNDPlayer *player, int playNotes) {
    int active = 0;
    int i;
    for (i = 0; i < SND_TRACK_COUNT_PER_PLAYER; i++) {
        struct SNDTrack *track = GetPlayerTrack(player, i);
        if (track && track->cur) {
            if (TrackSeqMain(track, player, i, playNotes) == 0) {
                active = 1;
            } else {
                ClosePlayerTrack(player, i);
            }
        }
    }
    return active == 0;
}

// -ipa file would otherwise inline these small functions into their callers in this file.
#pragma dont_inline on

// Variables 0-15 are the player's local variables, 16-31 the global ones.
s16 *GetVariablePtr(struct SNDPlayer *player, int index) {
    struct SNDSharedWork *shared = SNDi_SharedWork;
    if (!shared) {
        return NULL;
    }
    if (index < 16) {
        return shared->players[((const u8 *)player)[1]].localVars + index;
    }
    index -= 16;
    return shared->globalVars + index;
}

int AllocTrack(void) {
    struct SNDTrack *tracks = SNDi_Work.tracks;
    int i;
    for (i = 0; i < SND_TRACK_COUNT_; i++) {
        if (!tracks[i].flags.active) {
            tracks[i].flags.active = 1;
            return i;
        }
    }
    return -1;
}

void SetTrackMute(struct SNDTrack *track, struct SNDPlayer *player, int mute) {
    switch (mute) {
    case 0:
        track->flags.muted = 0;
        break;
    case 1:
        track->flags.muted = 1;
        break;
    case 2:
        track->flags.muted = 1;
        ReleaseTrackChannelAll(track, player, -1);
        break;
    case 3:
        track->flags.muted = 1;
        ReleaseTrackChannelAll(track, player, 127);
        FreeTrackChannelAll(track);
        break;
    }
}

#pragma dont_inline reset
