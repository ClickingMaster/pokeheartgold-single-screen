#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
typedef struct SeqTrack SeqTrack;
typedef struct SeqChannel SeqChannel;

typedef struct {
    u8 active : 1, noteWait : 1, mute : 1, tie : 1, waiting : 1, portamento : 1, compare : 1, alloc : 1;
} SeqFlags;

typedef struct {
    u8 a : 2, manualSweep : 1, b : 5;
} SeqChannelFlags;

struct SeqChannel {
    u8 pad0[3];
    SeqChannelFlags flags;
    u8 pad4[4];
    u8 key, velocity;
    u8 pada[10];
    s32 sweep, length;
    u8 pad1c[6];
    u8 status;
    u8 pad23[15];
    s16 pitch;
    s32 counter;
    u8 pad38[24];
    SeqChannel *next;
};

struct SeqTrack {
    SeqFlags flags;
    u8 pad1;
    u16 program;
    u8 volume, expression;
    s8 bend;
    u8 range;
    s8 pan;
    u8 pad9[5];
    u8 attack, decay, sustain, release, priority;
    s8 transpose;
    u8 key, portTime;
    s16 sweepPitch;
    u8 modType, modRange, modSpeed, modDepth;
    u16 modDelay, channelMask;
    s32 wait;
    const u8 *base, *pos, *stack[3];
    u8 loopCount[3], depth;
    SeqChannel *channels;
};

typedef struct {
    u8 pad0[4];
    u8 priority, volume;
    u8 pad6[18];
    u16 tempo;
    u8 pad1a[6];
    void *bank;
} SeqPlayer;

typedef struct {
    u8 type, pad;
    u16 wave, arc;
    u8 key, attack, decay, sustain, release, pan;
} SeqInst;

extern u32 sMmlPrintEnable;
extern SeqTrack *GetPlayerTrack(SeqPlayer *, int);
extern void InitCache(const void *);
extern u8 ReadByte(SeqTrack *);
extern u32 Read24(SeqTrack *);
extern s32 ReadArg(SeqTrack *, SeqPlayer *, int);
extern int SND_ReadInstData(void *, int, int, SeqInst *);
extern void ChannelCallback(SeqChannel *, int, SeqTrack *);
extern SeqChannel *SND_AllocExChannel(u32, int, int, void (*)(SeqChannel *, int, SeqTrack *), SeqTrack *);
extern int SND_NoteOn(SeqChannel *, int, int, int, void *, SeqInst *);
extern void SND_FreeExChannel(SeqChannel *);
extern void SND_SetExChannelAttack(SeqChannel *, int);
extern void SND_SetExChannelDecay(SeqChannel *, int);
extern void SND_SetExChannelSustain(SeqChannel *, int);
extern void SND_SetExChannelRelease(SeqChannel *, int);
extern void ReleaseTrackChannelAll(SeqTrack *, SeqPlayer *, int);
extern void FreeTrackChannelAll(SeqTrack *);
extern void StartTrack(SeqTrack *, const u8 *, u32);
extern void SetTrackMute(SeqTrack *, SeqPlayer *, int);
extern void ClosePlayerTrack(SeqPlayer *, int);
extern s16 *GetVariablePtr(SeqPlayer *, int);
extern u16 SND_CalcRandom(void);
int PlayerSeqMain(SeqPlayer *, int);

static inline int TrackSeqMain(SeqTrack *track, SeqPlayer *player, int trackIndex, int playNotes) {
    for (SeqChannel *ch = track->channels; ch; ch = ch->next) {
        if (ch->counter > 0) {
            ch->counter--;
        }
        if (!ch->flags.manualSweep && ch->sweep < ch->length) {
            ch->sweep++;
        }
    }
    if (track->flags.waiting) {
        if (track->channels) {
            return 0;
        }
        track->flags.waiting = 0;
    }
    if (track->wait > 0) {
        track->wait--;
        if (track->wait > 0) {
            return 0;
        }
    }
    InitCache(track->pos);
    while (track->wait == 0 && !track->flags.waiting) {
        int modified;
        int execute;
        int opcode;
        int argType;
        modified = 0;
        execute = 1;
        opcode = ReadByte(track);
        if (opcode == 0xa2) {
            opcode = ReadByte(track);
            execute = track->flags.compare;
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
            if (!track->flags.mute && playNotes) {
                SeqChannel *channel = 0;
                if (track->flags.tie) {
                    channel = track->channels;
                    if (channel) {
                        channel->key = key;
                        channel->velocity = velocity;
                    }
                }
                if (channel == 0) {
                    SeqInst inst;
                    if (!SND_ReadInstData(player->bank, track->program, key, &inst)) {
                        goto note_done;
                    }
                    u32 mask;
                    switch (inst.type) {
                    case 1:
                    case 4:
                        mask = 65535;
                        break;
                    case 2:
                        mask = 0x3f00;
                        break;
                    case 3:
                        mask = 0xc000;
                        break;
                    default:
                        goto note_done;
                    }
                    mask &= track->channelMask;
                    channel = SND_AllocExChannel(mask, player->priority + track->priority, track->flags.alloc, ChannelCallback, track);
                    if (!channel) {
                        goto note_done;
                    }
                    if (!SND_NoteOn(channel, key, velocity, track->flags.tie ? -1 : (length > 0 ? length : -1), player->bank, &inst)) {
                        channel->status = 0;
                        SND_FreeExChannel(channel);
                        goto note_done;
                    }
                    channel->next = track->channels;
                    track->channels = channel;
                }
                if (track->attack != 255) {
                    SND_SetExChannelAttack(channel, track->attack);
                }
                if (track->decay != 255) {
                    SND_SetExChannelDecay(channel, track->decay);
                }
                if (track->sustain != 255) {
                    SND_SetExChannelSustain(channel, track->sustain);
                }
                if (track->release != 255) {
                    SND_SetExChannelRelease(channel, track->release);
                }
                channel->pitch = track->sweepPitch;
                if (track->flags.portamento) {
                    channel->pitch = (s16)(channel->pitch + (s16)((u32)((int)track->key - key) << 6));
                }
                if (!track->portTime) {
                    channel->length = length > 0 ? length : -1;
                    channel->flags.manualSweep = 0;
                } else {
                    int t = track->portTime * track->portTime;
                    int distance = channel->pitch;
                    if (distance < 0) {
                        distance = -distance;
                    }
                    channel->length = (t * distance) >> 11;
                }
                channel->sweep = 0;
            }
note_done:
            track->key = key;
            if (track->flags.noteWait) {
                track->wait = length;
                if (length == 0) {
                    track->flags.waiting = 1;
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
                    SeqTrack *other = GetPlayerTrack(player, destination);
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
                    track->pos = track->base + offset;
                }
            } break;
            case 0x95: {
                u32 offset = Read24(track);
                if (execute && track->depth < 3) {
                    track->stack[track->depth] = track->pos;
                    track->depth++;
                    track->pos = track->base + offset;
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
                track->range = raw;
                break;
            case 0xc6:
                track->priority = raw;
                break;
            case 0xc7:
                track->flags.noteWait = raw;
                break;
            case 0xcf:
                track->portTime = raw;
                break;
            case 0xca:
                track->modSpeed = raw;
                break;
            case 0xcb:
                track->modRange = raw;
                break;
            case 0xcc:
                track->modType = raw;
                break;
            case 0xcd:
                track->modDepth = raw;
                break;
            case 0xd0:
                track->attack = raw;
                break;
            case 0xd1:
                track->decay = raw;
                break;
            case 0xd2:
                track->sustain = raw;
                break;
            case 0xd3:
                track->release = raw;
                break;
            case 0xd4:
                if (track->depth < 3) {
                    track->stack[track->depth] = track->pos;
                    track->loopCount[track->depth] = raw;
                    track->depth++;
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
                track->key = raw + track->transpose;
                track->flags.portamento = 1;
                break;
            case 0xce:
                track->flags.portamento = raw;
                break;
            case 0xc3:
                track->transpose = *(s8 *)&raw;
                break;
            case 0xc4:
                track->bend = *(s8 *)&raw;
                break;
            case 0xc0:
                track->pan = (s8)(raw - 64);
                break;
            case 0xd6:
                if (sMmlPrintEnable) {
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
                track->modDelay = value;
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
                track->flags.compare = (u8)(*value == arg);
                break;
            case 0xb9:
                track->flags.compare = (u8)(*value >= arg);
                break;
            case 0xba:
                track->flags.compare = (u8)(*value > arg);
                break;
            case 0xbb:
                track->flags.compare = (u8)(*value <= arg);
                break;
            case 0xbc:
                track->flags.compare = (u8)(*value < arg);
                break;
            case 0xbd:
                track->flags.compare = (u8)(*value != arg);
                break;
            }
            break;
        }
        case 0xf0:
            if (execute) {
                switch (opcode) {
                case 0xfd:
                    if (track->depth) {
                        track->depth--;
                        track->pos = track->stack[track->depth];
                    }
                    break;
                case 0xfc:
                    if (track->depth) {
                        u8 n = track->loopCount[track->depth - 1];
                        if (n) {
                            n--;
                            if (!n) {
                                track->depth--;
                                break;
                            }
                        }
                        track->loopCount[track->depth - 1] = n;
                        track->pos = track->stack[track->depth - 1];
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

int PlayerSeqMain(SeqPlayer *player, int playNotes) {
    int active = 0;
    int i;
    for (i = 0; i < 16; i++) {
        SeqTrack *track = GetPlayerTrack(player, i);
        if (track && track->pos) {
            if (TrackSeqMain(track, player, i, playNotes) == 0) {
                active = 1;
            } else {
                ClosePlayerTrack(player, i);
            }
        }
    }
    return active == 0;
}
