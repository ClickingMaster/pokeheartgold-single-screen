#include "pokepic_anim.h"

#include "global.h"

#include "filesystem.h"
#include "heap.h"
#include "pokepic.h"
#include "pokepic_anim_internal.h"
#include "sys_task_api.h"

// Animation scripts of Pokemon pictures (NARC a/0/9/0, one per animation): a word stream of commands
// (gPokepicAnimCommands, by opcode) that read their arguments from the stream. Commands work on 8 variables and on
// a transform (position offsets, scale, rotation) that is applied to the Pokepic; up to 4 motions animate one
// transform field per frame until they end.

typedef struct PokepicScriptAnim PokepicScriptAnim;

typedef struct PokepicMotion {
    u32 active;

    union { // 0x04, read from the script; the argument count is per motion type (gPokepicMotionTypes)
        int args[7];

        struct {
            int waveform; // 0x1E sin, 0x1F cos, 0x20 -sin, 0x21 -cos
            int target;
            int amplitude;
            int angleStep; // angle range for PokepicMotion_UpdateWaveOverDuration
            int startAngle;
            int duration;
            int frame;
        } wave;

        struct {
            int target;
            int velocity;
            int acceleration;
            int durationOrLimit;
            int frame;
        } accel;

        struct {
            int target;
            int total;
            int duration;
            int frame;
        } linear;
    };

    u8 unk_20[4];
    int *value; // 0x24 accumulator in values
    int *dest;  // 0x28 the PokepicScriptAnim field the motion drives
    u8 op;      // 0x2C 0x18 set, 0x19 base + value, 0x1A add
    u8 delay;   // 0x2D frames before the first update
    u8 unk_2E[2];
    int base; // 0x30 *dest when the motion started
    u8 unk_34[8];
    int values[5];                                                         // 0x3C one per target: dx, dy, scaleX, scaleY, rotation
    void (*update)(struct PokepicMotion *motion, PokepicScriptAnim *anim); // 0x50
} PokepicMotion;

struct PokepicScriptAnim {
    Pokepic *pic;
    SysTask *task;
    void *data;    // 0x08 the script
    void **cursor; // 0x0C current script word
    u32 active;
    u32 scriptId;     // 0x14 NARC member
    u32 yield;        // 0x18 set by a command that ends the frame
    u32 done;         // 0x1C the script ended
    u32 state;        // 0x20 1 = idle
    s32 vars[8];      // 0x24
    s32 commandCount; // 0x44 commands run this frame
    s32 loopCount;    // 0x48
    s32 loopIndex;    // 0x4C
    void **loopStart; // 0x50
    u32 delay;        // 0x54 frames to wait
    s32 baseX;        // 0x58 position of the picture when the animation started
    s32 baseY;
    s32 x; // 0x60
    s32 y;
    s32 dx; // 0x68 offsets, usually driven by motions
    s32 dy;
    s32 scaleX; // 0x70 added to 0x100
    s32 scaleY;
    s32 rotation;             // 0x78
    PokepicMotion motions[4]; // 0x7C
    u8 reverseX;              // 0x1CC
    u8 runMotions;            // 0x1CD
    u8 scaleMode;             // 0x1CE 0x1B, 0x1C or 0x1D (see PokepicScriptAnimCmd_ApplyTransform)
    u8 waitPaletteFade;       // 0x1CF
};

struct PokepicAnimManager {
    PokepicScriptAnim *anims;
    enum HeapID heapId;
    u8 allowReverse;
    u8 count;
    u8 unk_0A[2];
};

typedef struct PokepicMotionType {
    void (*update)(PokepicMotion *motion);
    int argCount;
    int targetArg;
} PokepicMotionType;

// asm/pokepic_anim_data.s
extern const PokepicMotionType gPokepicMotionTypes[];
extern const int gPokepicMotionTypeArgCounts[];
extern const int gPokepicMotionTypeTargetArgs[];
extern void (*gPokepicAnimCommands[])(PokepicScriptAnim *anim);

void PokepicScriptAnim_Task(SysTask *task, void *data);
void PokepicScriptAnim_RunFrame(PokepicScriptAnim *anim);
void *PokepicScriptAnim_GetScriptWordAt(void **array, int index, int expected);
void *PokepicScriptAnim_GetScriptWordChecked(void **array, int expected);
void *PokepicScriptAnim_GetScriptWord(void **array);
PokepicMotion *PokepicScriptAnim_AllocMotion(PokepicScriptAnim *anim, int type);
void PokepicScriptAnim_ReadArg(PokepicScriptAnim *anim, s32 *out);
void PokepicScriptAnim_ReadArgByte(PokepicScriptAnim *anim, u8 *out);
void PokepicScriptAnim_ReadVarIndex(PokepicScriptAnim *anim, u8 *out);
void PokepicScriptAnim_ReadVarIndexPair(PokepicScriptAnim *anim, u8 *a, u8 *b);
void PokepicScriptAnim_ReadDestVarAndOperands(PokepicScriptAnim *anim, u8 *dest, s32 *a, s32 *b);
void PokepicScriptAnim_ReadDestAndOperands(PokepicScriptAnim *anim, u8 *dest, s32 *a, s32 *b);
void PokepicScriptAnim_ReadDestAngleAndAmplitude(PokepicScriptAnim *anim, u8 *dest, s32 *angle, s32 *amplitude);
u32 PokepicScriptAnim_Compare(const s32 *a, const s32 *b);
void PokepicScriptAnim_AdjustYForScale(PokepicScriptAnim *anim);
void PokepicScriptAnimCmd_End(PokepicScriptAnim *anim);
void PokepicScriptAnimCmd_Yield(PokepicScriptAnim *anim);
void PokepicScriptAnimCmd_ResetTransform(PokepicScriptAnim *anim);
void PokepicScriptAnimCmd_CopyVar(PokepicScriptAnim *anim);
void PokepicScriptAnimCmd_AddVar(PokepicScriptAnim *anim);
void PokepicScriptAnimCmd_MulVar(PokepicScriptAnim *anim);
void PokepicScriptAnimCmd_SubVar(PokepicScriptAnim *anim);
void PokepicScriptAnimCmd_DivVar(PokepicScriptAnim *anim);
void PokepicScriptAnimCmd_ModVar(PokepicScriptAnim *anim);
void PokepicScriptAnimCmd_SetVarIfCompare(PokepicScriptAnim *anim);
void PokepicScriptAnimCmd_SetVar(PokepicScriptAnim *anim);
void PokepicScriptAnimCmd_LoopStart(PokepicScriptAnim *anim);
void PokepicScriptAnimCmd_LoopEnd(PokepicScriptAnim *anim);
void PokepicScriptAnimCmd_SetAttrFromVar(PokepicScriptAnim *anim);
void PokepicScriptAnimCmd_AddAttrFromVar(PokepicScriptAnim *anim);
void PokepicScriptAnimCmd_SetOrAddAttr(PokepicScriptAnim *anim);
void PokepicScriptAnimCmd_Sin(PokepicScriptAnim *anim);
void PokepicScriptAnimCmd_Cos(PokepicScriptAnim *anim);
void PokepicScriptAnimCmd_SetPosFromVar(PokepicScriptAnim *anim);
void PokepicScriptAnimCmd_AddPosFromVar(PokepicScriptAnim *anim);
void PokepicScriptAnimCmd_SetOrAddTransform(PokepicScriptAnim *anim);
void PokepicScriptAnimCmd_ApplyPosition(PokepicScriptAnim *anim);
void PokepicScriptAnimCmd_ApplyTransform(PokepicScriptAnim *anim);
void PokepicScriptAnimCmd_SetOffsetFromVar(PokepicScriptAnim *anim);
void PokepicScriptAnimCmd_Wait(PokepicScriptAnim *anim);
void PokepicScriptAnimCmd_StartPaletteFade(PokepicScriptAnim *anim);
void PokepicScriptAnimCmd_WaitPaletteFade(PokepicScriptAnim *anim);
void PokepicScriptAnimCmd_RunMotions(PokepicScriptAnim *anim);
void PokepicScriptAnimCmd_SetScaleMode(PokepicScriptAnim *anim);
void PokepicScriptAnimCmd_StartWaveMotion(PokepicScriptAnim *anim);
void PokepicScriptAnimCmd_StartWaveMotionOverDuration(PokepicScriptAnim *anim);
void PokepicScriptAnimCmd_StartAccelMotion(PokepicScriptAnim *anim);
void PokepicScriptAnimCmd_StartLinearMotion(PokepicScriptAnim *anim);
void PokepicScriptAnimCmd_StartAccelMotionToLimit(PokepicScriptAnim *anim);
void PokepicMotion_ApplyValue(int op, int *base, int *value, int *dest);
void PokepicMotion_SetTarget(u8 target, PokepicMotion *motion, PokepicScriptAnim *anim);
void PokepicScriptAnim_StartMotion(PokepicScriptAnim *anim, int type);
void PokepicMotion_UpdateWave(PokepicMotion *motion);
void PokepicMotion_UpdateWaveOverDuration(PokepicMotion *motion);
void PokepicMotion_UpdateAccel(PokepicMotion *motion);
void PokepicMotion_UpdateLinear(PokepicMotion *motion);
void PokepicMotion_UpdateAccelToLimit(PokepicMotion *motion);

PokepicAnimManager *PokepicAnimManager_New(enum HeapID heapId, int count, int flag) {
    PokepicAnimManager *mgr = Heap_Alloc(heapId, sizeof(PokepicAnimManager));
    int i;
    mgr->allowReverse = flag;
    mgr->count = count;
    mgr->heapId = heapId;
    mgr->anims = Heap_Alloc(heapId, count * sizeof(PokepicScriptAnim));
    MI_CpuFill8(mgr->anims, 0, count * sizeof(PokepicScriptAnim));
    for (i = 0; i < count; i++) {
        mgr->anims[i].state = 1;
    }
    return mgr;
}

void PokepicAnimManager_Delete(PokepicAnimManager *mgr) {
    Heap_Free(mgr->anims);
    Heap_Free(mgr);
}

#pragma force_active on

void PokepicAnimManager_StartAnim(void *managerArg, void *picArg, struct UnkStruct_0207294C *params, u8 index) {
    PokepicAnimManager *mgr = managerArg;
    Pokepic *pic = picArg;
    int scriptId = params->unk_0;
    int delay = params->unk_2;
    if (index >= mgr->count) {
        GF_AssertFail();
    }
    if (mgr->anims[index].active) {
        GF_AssertFail();
    }
    MI_CpuFill8(&mgr->anims[index], 0, sizeof(PokepicScriptAnim));
    mgr->anims[index].active = 1;
    mgr->anims[index].pic = pic;
    if (scriptId >= 143) {
        scriptId = 0;
        delay = 0;
    }
    mgr->anims[index].scriptId = scriptId;
    mgr->anims[index].reverseX = mgr->allowReverse ? params->unk_4 : 0;
    mgr->anims[index].data = AllocAtEndAndReadWholeNarcMemberByIdPair(NARC_a_0_9_0, mgr->anims[index].scriptId, mgr->heapId);
    mgr->anims[index].cursor = mgr->anims[index].data;
    mgr->anims[index].done = 0;
    mgr->anims[index].state = 0;
    mgr->anims[index].runMotions = 0;
    mgr->anims[index].scaleMode = 28;
    mgr->anims[index].waitPaletteFade = 0;
    mgr->anims[index].task = SysTask_CreateOnMainQueue(PokepicScriptAnim_Task, &mgr->anims[index], 0);
    mgr->anims[index].delay = delay;
    mgr->anims[index].baseX = Pokepic_GetAttr(pic, 0);
    mgr->anims[index].baseY = Pokepic_GetAttr(pic, 1);
    mgr->anims[index].x = 0;
    mgr->anims[index].y = 0;
    mgr->anims[index].dx = 0;
    mgr->anims[index].dy = 0;
    mgr->anims[index].scaleX = 0;
    mgr->anims[index].scaleY = 0;
    mgr->anims[index].rotation = 0;
}

#pragma force_active off

u32 PokepicAnimManager_GetAnimState(PokepicAnimManager *mgr, u32 index) {
    GF_ASSERT(index < mgr->count);
    return mgr->anims[index].state;
}

void PokepicAnimManager_StopAnim(PokepicAnimManager *mgr, u32 index) {
    if (mgr->anims[index].task != NULL) {
        SysTask_Destroy(mgr->anims[index].task);
        mgr->anims[index].task = NULL;
        mgr->anims[index].state = 1;
        mgr->anims[index].active = 0;
        Heap_Free(mgr->anims[index].data);
    }
}

void PokepicScriptAnim_Task(SysTask *task, void *data) {
    PokepicScriptAnim *anim = data;
    if (anim->delay == 0) {
        PokepicScriptAnim_RunFrame(anim);
    } else {
        anim->delay--;
    }
    if (anim->done != 0) {
        anim->state = 1;
        anim->active = 0;
        SysTask_Destroy(task);
        anim->task = NULL;
        Heap_Free(anim->data);
    }
}

void PokepicScriptAnim_RunFrame(PokepicScriptAnim *anim) {
    u8 ended;
    u8 i;

    anim->yield = 0;
    anim->commandCount = 0;
    ended = 0;
    for (i = 0; i < 4; i++) {
        PokepicMotion *motion = &anim->motions[i];
        if (motion->active != 0) {
            if (motion->delay == 0) {
                motion->update(motion, anim);
            } else {
                motion->delay--;
            }
        } else {
            ended++;
        }
    }
    if (ended == 4) {
        anim->runMotions = 0;
    }
    if (anim->runMotions != 0) {
        PokepicScriptAnimCmd_ApplyPosition(anim);
        PokepicScriptAnimCmd_ApplyTransform(anim);
        return;
    }
    if (anim->waitPaletteFade != 0) {
        if (Pokepic_ResumePaletteFade(anim->pic) == 0) {
            anim->waitPaletteFade = 0;
        } else {
            return;
        }
    }
    do {
        anim->commandCount++;
        if ((u32)*anim->cursor >= 0x22) {
            GF_AssertFail();
        }
        gPokepicAnimCommands[(u32)*anim->cursor](anim);
        if (anim->done != 0) {
            return;
        }
        anim->cursor++;
        if (anim->yield != 0) {
            return;
        }
        if (anim->runMotions != 0) {
            PokepicScriptAnimCmd_ApplyPosition(anim);
            PokepicScriptAnimCmd_ApplyTransform(anim);
            return;
        }
    } while (anim->commandCount < 0x100);
    GF_AssertFail();
    anim->done = 1;
}

void *PokepicScriptAnim_GetScriptWordAt(void **array, int index, int expected) {
    void *value = array[index];
    GF_ASSERT(expected == 1);
    return value;
}

void *PokepicScriptAnim_GetScriptWordChecked(void **array, int expected) {
    return PokepicScriptAnim_GetScriptWordAt(array, 0, expected);
}

void *PokepicScriptAnim_GetScriptWord(void **array) {
    return PokepicScriptAnim_GetScriptWordChecked(array, 1);
}

PokepicMotion *PokepicScriptAnim_AllocMotion(PokepicScriptAnim *anim, int type) {
    u8 i;

    for (i = 0; i < 4; i++) {
        PokepicMotion *motion = &anim->motions[i];
        if (motion->active == 0) {
            MI_CpuFill8(motion, 0, sizeof(PokepicMotion));
            motion->active = 1;
            motion->update = (void (*)(PokepicMotion *, PokepicScriptAnim *))gPokepicMotionTypes[type].update;
            return motion;
        }
    }
    GF_AssertFail();
    return NULL;
}

void PokepicScriptAnim_ReadArg(PokepicScriptAnim *anim, s32 *out) {
    anim->cursor++;
    *out = (u32)PokepicScriptAnim_GetScriptWord(anim->cursor);
}

void PokepicScriptAnim_ReadArgByte(PokepicScriptAnim *anim, u8 *out) {
    anim->cursor++;
    *out = (u8)(u32)PokepicScriptAnim_GetScriptWord(anim->cursor);
}

void PokepicScriptAnim_ReadVarIndex(PokepicScriptAnim *anim, u8 *out) {
    anim->cursor++;
    *out = (u8)(u32)PokepicScriptAnim_GetScriptWord(anim->cursor);
    GF_ASSERT(*out < 8);
}

void PokepicScriptAnim_ReadVarIndexPair(PokepicScriptAnim *anim, u8 *a, u8 *b) {
    PokepicScriptAnim_ReadVarIndex(anim, a);
    PokepicScriptAnim_ReadVarIndex(anim, b);
}

void PokepicScriptAnim_ReadDestVarAndOperands(PokepicScriptAnim *anim, u8 *dest, s32 *a, s32 *b) {
    u8 args[4];
    PokepicScriptAnim_ReadVarIndex(anim, dest);
    PokepicScriptAnim_ReadArgByte(anim, &args[0]);
    if (args[0] == 0x12) {
        PokepicScriptAnim_ReadVarIndex(anim, &args[2]);
        *a = anim->vars[args[2]];
        PokepicScriptAnim_ReadArg(anim, b);
    } else if (args[0] == 0x13) {
        PokepicScriptAnim_ReadVarIndexPair(anim, &args[2], &args[1]);
        *a = anim->vars[args[2]];
        *b = anim->vars[args[1]];
    } else {
        GF_AssertFail();
    }
}

void PokepicScriptAnim_ReadDestAndOperands(PokepicScriptAnim *anim, u8 *dest, s32 *a, s32 *b) {
    u8 args[4];
    PokepicScriptAnim_ReadVarIndex(anim, dest);
    PokepicScriptAnim_ReadArgByte(anim, &args[1]);
    PokepicScriptAnim_ReadArgByte(anim, &args[0]);
    if (args[1] == 0x12) {
        PokepicScriptAnim_ReadArg(anim, a);
    } else if (args[1] == 0x13) {
        PokepicScriptAnim_ReadVarIndex(anim, &args[3]);
        *a = anim->vars[args[3]];
    } else {
        GF_AssertFail();
    }
    if (args[0] == 0x12) {
        PokepicScriptAnim_ReadArg(anim, b);
    } else if (args[0] == 0x13) {
        PokepicScriptAnim_ReadVarIndex(anim, &args[2]);
        *b = anim->vars[args[2]];
    } else {
        GF_AssertFail();
    }
}

void PokepicScriptAnim_ReadDestAngleAndAmplitude(PokepicScriptAnim *anim, u8 *dest, s32 *angle, s32 *amplitude) {
    u8 angleVar, amplitudeVar, offsetVar, argType;
    s32 baseAngle, offset;

    PokepicScriptAnim_ReadVarIndexPair(anim, dest, &angleVar);
    baseAngle = anim->vars[angleVar];
    PokepicScriptAnim_ReadArgByte(anim, &argType);

    if (argType == 0x14) {
        PokepicScriptAnim_ReadArg(anim, amplitude);
    } else if (argType == 0x15) {
        PokepicScriptAnim_ReadVarIndex(anim, &amplitudeVar);
        *amplitude = anim->vars[amplitudeVar];
    } else {
        GF_AssertFail();
    }

    PokepicScriptAnim_ReadArgByte(anim, &argType);

    if (argType == 0x14) {
        PokepicScriptAnim_ReadArg(anim, &offset);
    } else if (argType == 0x15) {
        PokepicScriptAnim_ReadVarIndex(anim, &offsetVar);
        offset = anim->vars[offsetVar];
    } else {
        GF_AssertFail();
    }

    *angle = baseAngle + offset;
    *angle %= 0x10000;
}

u32 PokepicScriptAnim_Compare(const s32 *a, const s32 *b) {
    s32 difference = *a - *b;
    if (difference < 0) {
        return 0xf;
    }
    if (difference > 0) {
        return 0x10;
    }
    return 0x11;
}

void PokepicScriptAnim_AdjustYForScale(PokepicScriptAnim *anim) {
    Pokepic_AddAttr(anim->pic, 1, -anim->scaleY / 8);
}

void PokepicScriptAnimCmd_End(PokepicScriptAnim *anim) {
    PokepicScriptAnimCmd_ResetTransform(anim);
    anim->yield = 1;
    anim->done = 1;
}

void PokepicScriptAnimCmd_Yield(PokepicScriptAnim *anim) {
    anim->yield = 1;
}

void PokepicScriptAnimCmd_ResetTransform(PokepicScriptAnim *anim) {
    Pokepic_SetAttr(anim->pic, 0, anim->baseX);
    Pokepic_SetAttr(anim->pic, 1, anim->baseY);
    Pokepic_SetAttr(anim->pic, 9, 0);
    Pokepic_SetAttr(anim->pic, 10, 0);
    Pokepic_SetAttr(anim->pic, 12, 0x100);
    Pokepic_SetAttr(anim->pic, 13, 0x100);
}

void PokepicScriptAnimCmd_CopyVar(PokepicScriptAnim *anim) {
    u8 dest, src;
    PokepicScriptAnim_ReadVarIndexPair(anim, &dest, &src);
    anim->vars[dest] = anim->vars[src];
}

void PokepicScriptAnimCmd_AddVar(PokepicScriptAnim *anim) {
    u8 dest;
    s32 a, b;
    PokepicScriptAnim_ReadDestVarAndOperands(anim, &dest, &a, &b);
    anim->vars[dest] = a + b;
}

void PokepicScriptAnimCmd_MulVar(PokepicScriptAnim *anim) {
    u8 dest;
    s32 a, b;
    PokepicScriptAnim_ReadDestVarAndOperands(anim, &dest, &a, &b);
    anim->vars[dest] = a * b;
}

void PokepicScriptAnimCmd_SubVar(PokepicScriptAnim *anim) {
    u8 dest;
    s32 a, b;
    PokepicScriptAnim_ReadDestAndOperands(anim, &dest, &a, &b);
    anim->vars[dest] = a - b;
}

void PokepicScriptAnimCmd_DivVar(PokepicScriptAnim *anim) {
    u8 dest;
    s32 a, b;
    PokepicScriptAnim_ReadDestAndOperands(anim, &dest, &a, &b);
    anim->vars[dest] = a / b;
}

void PokepicScriptAnimCmd_ModVar(PokepicScriptAnim *anim) {
    u8 dest;
    s32 a, b;
    PokepicScriptAnim_ReadDestAndOperands(anim, &dest, &a, &b);
    anim->vars[dest] = a % b;
}

void PokepicScriptAnimCmd_SetVarIfCompare(PokepicScriptAnim *anim) {
    s32 b;
    s32 a;
    s32 newValue;
    u8 args[4];
    int result;

    PokepicScriptAnim_ReadArgByte(anim, &args[0]);
    if (args[0] == 0x14) {
        PokepicScriptAnim_ReadVarIndex(anim, &args[3]);
        b = anim->vars[args[3]];
        PokepicScriptAnim_ReadArg(anim, &a);
    } else if (args[0] == 0x15) {
        PokepicScriptAnim_ReadVarIndexPair(anim, &args[3], &args[2]);
        b = anim->vars[args[3]];
        a = anim->vars[args[2]];
    } else {
        GF_AssertFail();
    }
    PokepicScriptAnim_ReadArgByte(anim, &args[1]);
    if (args[1] > 0x11) {
        GF_AssertFail();
    }
    result = PokepicScriptAnim_Compare(&b, &a);
    PokepicScriptAnim_ReadArgByte(anim, &args[0]);
    if (args[0] == 0x14) {
        PokepicScriptAnim_ReadVarIndex(anim, &args[3]);
        PokepicScriptAnim_ReadArg(anim, &newValue);
    } else if (args[0] == 0x15) {
        PokepicScriptAnim_ReadVarIndexPair(anim, &args[3], &args[2]);
        newValue = anim->vars[args[2]];
    } else {
        GF_AssertFail();
    }
    if (args[1] == result) {
        anim->vars[args[3]] = newValue;
    }
}

void PokepicScriptAnimCmd_SetVar(PokepicScriptAnim *anim) {
    u8 dest;
    PokepicScriptAnim_ReadVarIndex(anim, &dest);
    anim->cursor++;
    anim->vars[dest] = (u32)PokepicScriptAnim_GetScriptWord(anim->cursor);
}

void PokepicScriptAnimCmd_LoopStart(PokepicScriptAnim *anim) {
    GF_ASSERT(anim->loopStart == NULL);
    anim->cursor++;
    anim->loopStart = anim->cursor;
    anim->loopCount = (u32)PokepicScriptAnim_GetScriptWord(anim->cursor);
    anim->loopIndex = 0;
}

void PokepicScriptAnimCmd_LoopEnd(PokepicScriptAnim *anim) {
    anim->loopIndex++;
    if (anim->loopIndex >= anim->loopCount) {
        anim->loopStart = NULL;
        anim->loopIndex = 0;
        anim->loopCount = 0;
    } else {
        anim->cursor = anim->loopStart;
    }
}

void PokepicScriptAnimCmd_SetAttrFromVar(PokepicScriptAnim *anim) {
    s32 attr;
    u8 var;
    PokepicScriptAnim_ReadArg(anim, &attr);
    PokepicScriptAnim_ReadVarIndex(anim, &var);
    Pokepic_SetAttr(anim->pic, attr, anim->vars[var]);
}

void PokepicScriptAnimCmd_AddAttrFromVar(PokepicScriptAnim *anim) {
    s32 attr;
    u8 var;
    PokepicScriptAnim_ReadArg(anim, &attr);
    PokepicScriptAnim_ReadVarIndex(anim, &var);
    Pokepic_AddAttr(anim->pic, attr, anim->vars[var]);
}

void PokepicScriptAnimCmd_SetOrAddAttr(PokepicScriptAnim *anim) {
    struct {
        u8 command;
        u8 valueType;
        u8 var;
        u8 unk_3;
        s32 value;
        s32 attr;
    } args;

    PokepicScriptAnim_ReadArg(anim, &args.attr);
    PokepicScriptAnim_ReadArgByte(anim, &args.valueType);
    if (args.valueType == 0x14) {
        PokepicScriptAnim_ReadArg(anim, &args.value);
    } else if (args.valueType == 0x15) {
        PokepicScriptAnim_ReadVarIndex(anim, &args.var);
        args.value = anim->vars[args.var];
    } else {
        GF_ASSERT(FALSE);
    }
    PokepicScriptAnim_ReadArgByte(anim, &args.command);
    if (args.command == 0x16) {
        Pokepic_SetAttr(anim->pic, args.attr, args.value);
    } else if (args.command == 0x17) {
        Pokepic_AddAttr(anim->pic, args.attr, args.value);
    } else {
        GF_ASSERT(FALSE);
    }
}

void PokepicScriptAnimCmd_Sin(PokepicScriptAnim *anim) {
    u8 dest;
    s32 angle;
    s32 amplitude;
    PokepicScriptAnim_ReadDestAngleAndAmplitude(anim, &dest, &angle, &amplitude);
    anim->vars[dest] = (FX_SinCosTable_[(angle >> 4) * 2] * amplitude) >> 12;
}

void PokepicScriptAnimCmd_Cos(PokepicScriptAnim *anim) {
    u8 dest;
    s32 angle;
    s32 amplitude;
    PokepicScriptAnim_ReadDestAngleAndAmplitude(anim, &dest, &angle, &amplitude);
    anim->vars[dest] = (FX_CosIdx(angle) * amplitude) >> 12;
}

void PokepicScriptAnimCmd_SetPosFromVar(PokepicScriptAnim *anim) {
    u8 args[2];
    PokepicScriptAnim_ReadVarIndex(anim, &args[1]);
    PokepicScriptAnim_ReadArgByte(anim, &args[0]);
    if (args[0] == 8) {
        anim->x = anim->vars[args[1]];
    } else if (args[0] == 9) {
        anim->y = anim->vars[args[1]];
    } else {
        GF_AssertFail();
    }
}

void PokepicScriptAnimCmd_AddPosFromVar(PokepicScriptAnim *anim) {
    u8 args[2];
    PokepicScriptAnim_ReadVarIndex(anim, &args[1]);
    PokepicScriptAnim_ReadArgByte(anim, &args[0]);
    if (args[0] == 8) {
        anim->x += anim->vars[args[1]];
    } else if (args[0] == 9) {
        anim->y += anim->vars[args[1]];
    } else {
        GF_AssertFail();
    }
}

void PokepicScriptAnimCmd_SetOrAddTransform(PokepicScriptAnim *anim) {
    s32 value;
    u8 args[4];
    s32 *target;
    PokepicScriptAnim_ReadArgByte(anim, &args[3]);
    if (args[3] == 8) {
        target = &anim->x;
    } else if (args[3] == 9) {
        target = &anim->y;
    } else if (args[3] == 10) {
        target = &anim->dx;
    } else if (args[3] == 11) {
        target = &anim->dy;
    } else if (args[3] == 12) {
        target = &anim->scaleX;
    } else if (args[3] == 13) {
        target = &anim->scaleY;
    } else if (args[3] == 14) {
        target = &anim->rotation;
    } else {
        GF_AssertFail();
    }
    PokepicScriptAnim_ReadArgByte(anim, &args[1]);
    if (args[1] == 20) {
        PokepicScriptAnim_ReadArg(anim, &value);
    } else if (args[1] == 21) {
        PokepicScriptAnim_ReadVarIndex(anim, &args[2]);
        value = anim->vars[args[2]];
    } else {
        GF_AssertFail();
    }
    PokepicScriptAnim_ReadArgByte(anim, &args[0]);
    if (args[0] == 22) {
        *target = value;
    } else if (args[0] == 23) {
        *target += value;
    } else {
        GF_AssertFail();
    }
}

void PokepicScriptAnimCmd_ApplyPosition(PokepicScriptAnim *anim) {
    if (anim->reverseX != 0) {
        Pokepic_SetAttr(anim->pic, 0, anim->baseX - (anim->x + anim->dx));
    } else {
        Pokepic_SetAttr(anim->pic, 0, anim->baseX + anim->x + anim->dx);
    }
    Pokepic_SetAttr(anim->pic, 1, anim->baseY + anim->y + anim->dy);
}

void PokepicScriptAnimCmd_ApplyTransform(PokepicScriptAnim *anim) {
    Pokepic_SetAttr(anim->pic, 12, anim->scaleX + 0x100);
    Pokepic_SetAttr(anim->pic, 13, anim->scaleY + 0x100);
    Pokepic_SetAttr(anim->pic, 9, (u16)anim->rotation);
    if (anim->scaleMode == 0x1b) {
        if (anim->scaleY < 0) {
            PokepicScriptAnim_AdjustYForScale(anim);
        }
    } else if (anim->scaleMode == 0x1d) {
        if (anim->scaleY != 0) {
            PokepicScriptAnim_AdjustYForScale(anim);
        }
    } else if (anim->scaleMode != 0x1c) {
        GF_AssertFail();
    }
}

void PokepicScriptAnimCmd_SetOffsetFromVar(PokepicScriptAnim *anim) {
    u8 var;
    u8 target;
    PokepicScriptAnim_ReadVarIndex(anim, &var);
    anim->cursor++;
    target = (u8)((u32)PokepicScriptAnim_GetScriptWord(anim->cursor) & 0xff);
    if (target == 8 || target == 10) {
        anim->dx = anim->vars[var];
    } else if (target == 9 || target == 11) {
        anim->dy = anim->vars[var];
    } else {
        GF_AssertFail();
    }
}

void PokepicScriptAnimCmd_Wait(PokepicScriptAnim *anim) {
    PokepicScriptAnim_ReadArg(anim, (s32 *)&anim->delay);
    anim->yield = 1;
}

void PokepicScriptAnimCmd_StartPaletteFade(PokepicScriptAnim *anim) {
    s32 duration;
    u8 args[4];
    PokepicScriptAnim_ReadArgByte(anim, &args[2]);
    PokepicScriptAnim_ReadArgByte(anim, &args[1]);
    PokepicScriptAnim_ReadArgByte(anim, &args[0]);
    PokepicScriptAnim_ReadArg(anim, &duration);
    Pokepic_StartPaletteFade(anim->pic, args[2], args[1], args[0], duration);
}

void PokepicScriptAnimCmd_WaitPaletteFade(PokepicScriptAnim *anim) {
    if (Pokepic_ResumePaletteFade(anim->pic)) {
        anim->waitPaletteFade = 1;
        anim->yield = 1;
    }
}

void PokepicScriptAnimCmd_RunMotions(PokepicScriptAnim *anim) {
    anim->runMotions = 1;
}

void PokepicScriptAnimCmd_SetScaleMode(PokepicScriptAnim *anim) {
    u8 mode;
    BOOL valid;
    BOOL keepsFeet;
    PokepicScriptAnim_ReadArgByte(anim, &anim->scaleMode);
    mode = anim->scaleMode;
    valid = TRUE;
    keepsFeet = TRUE;
    if (mode != 0x1B && mode != 0x1D) {
        keepsFeet = FALSE;
    }
    if (!keepsFeet && mode != 0x1C) {
        valid = FALSE;
    }
    GF_ASSERT(valid);
}

void PokepicScriptAnimCmd_StartWaveMotion(PokepicScriptAnim *anim) {
    PokepicScriptAnim_StartMotion(anim, 0);
}

void PokepicScriptAnimCmd_StartWaveMotionOverDuration(PokepicScriptAnim *anim) {
    PokepicScriptAnim_StartMotion(anim, 1);
}

void PokepicScriptAnimCmd_StartAccelMotion(PokepicScriptAnim *anim) {
    PokepicScriptAnim_StartMotion(anim, 2);
}

void PokepicScriptAnimCmd_StartLinearMotion(PokepicScriptAnim *anim) {
    PokepicScriptAnim_StartMotion(anim, 3);
}

void PokepicScriptAnimCmd_StartAccelMotionToLimit(PokepicScriptAnim *anim) {
    PokepicScriptAnim_StartMotion(anim, 4);
}

void PokepicMotion_ApplyValue(int op, int *base, int *value, int *dest) {
    if (op == 0x18) {
        *dest = *value;
    } else if (op == 0x19) {
        *dest = *base + *value;
    } else if (op == 0x1A) {
        *dest += *value;
    } else {
        GF_ASSERT(FALSE);
    }
}

void PokepicMotion_SetTarget(u8 target, PokepicMotion *motion, PokepicScriptAnim *anim) {
    switch (target) {
    case 0x23:
        motion->value = &motion->values[0];
        motion->dest = &anim->dx;
        motion->base = anim->dx;
        break;
    case 0x24:
        motion->value = &motion->values[1];
        motion->dest = &anim->dy;
        motion->base = anim->dy;
        break;
    case 0x25:
        motion->value = &motion->values[2];
        motion->dest = &anim->scaleX;
        motion->base = anim->scaleX;
        break;
    case 0x26:
        motion->value = &motion->values[3];
        motion->dest = &anim->scaleY;
        motion->base = anim->scaleY;
        break;
    case 0x27:
        motion->value = &motion->values[4];
        motion->dest = &anim->rotation;
        motion->base = anim->rotation;
        break;
    default:
        GF_AssertFail();
        break;
    }
}

void PokepicScriptAnim_StartMotion(PokepicScriptAnim *anim, int type) {
    PokepicMotion *motion = PokepicScriptAnim_AllocMotion(anim, (u8)type);
    u8 i;
    int offset;
    PokepicScriptAnim_ReadArgByte(anim, &motion->op);
    PokepicScriptAnim_ReadArgByte(anim, &motion->delay);
    // the type's fields are addressed through labels inside gPokepicMotionTypes
    offset = type * sizeof(PokepicMotionType);
    i = 0;
    if (*(const int *)((const u8 *)gPokepicMotionTypeArgCounts + offset) > 0) {
        const PokepicMotionType *motionType = (const PokepicMotionType *)((const u8 *)gPokepicMotionTypes + offset);
        do {
            PokepicScriptAnim_ReadArg(anim, &motion->args[i]);
            i = (u8)(i + 1);
        } while (i < motionType->argCount);
    }
    PokepicMotion_SetTarget(motion->args[*(const int *)((const u8 *)gPokepicMotionTypeTargetArgs + offset)], motion, anim);
    if (motion->delay == 0) {
        motion->update(motion, anim);
    } else {
        motion->delay--;
    }
}

void PokepicMotion_UpdateWave(PokepicMotion *motion) {
    u16 angle = motion->wave.startAngle + motion->wave.angleStep * (motion->wave.frame + 1);
    switch (motion->wave.waveform) {
    case 0x1e:
        *motion->value = (FX_SinCosTable_[(angle >> 4) * 2] * motion->wave.amplitude) >> 12;
        break;
    case 0x1f:
        *motion->value = (FX_SinCosTable_[(angle >> 4) * 2 + 1] * motion->wave.amplitude) >> 12;
        break;
    case 0x20:
        *motion->value = -((FX_SinCosTable_[(angle >> 4) * 2] * motion->wave.amplitude) >> 12);
        break;
    case 0x21:
        *motion->value = -((FX_SinCosTable_[(angle >> 4) * 2 + 1] * motion->wave.amplitude) >> 12);
        break;
    default:
        GF_AssertFail();
        break;
    }
    PokepicMotion_ApplyValue(motion->op, &motion->base, motion->value, motion->dest);
    motion->wave.frame++;
    if (motion->wave.frame >= motion->wave.duration) {
        motion->active = 0;
    }
}

void PokepicMotion_UpdateWaveOverDuration(PokepicMotion *motion) {
    u16 angle = motion->wave.startAngle + motion->wave.angleStep * (motion->wave.frame + 1) / motion->wave.duration;
    switch (motion->wave.waveform) {
    case 0x1e:
        *motion->value = (FX_SinCosTable_[(angle >> 4) * 2] * motion->wave.amplitude) >> 12;
        break;
    case 0x1f:
        *motion->value = (FX_SinCosTable_[(angle >> 4) * 2 + 1] * motion->wave.amplitude) >> 12;
        break;
    case 0x20:
        *motion->value = -((FX_SinCosTable_[(angle >> 4) * 2] * motion->wave.amplitude) >> 12);
        break;
    case 0x21:
        *motion->value = -((FX_SinCosTable_[(angle >> 4) * 2 + 1] * motion->wave.amplitude) >> 12);
        break;
    default:
        GF_AssertFail();
        break;
    }
    PokepicMotion_ApplyValue(motion->op, &motion->base, motion->value, motion->dest);
    motion->wave.frame++;
    if (motion->wave.frame >= motion->wave.duration) {
        motion->active = 0;
    }
}

void PokepicMotion_UpdateAccel(PokepicMotion *motion) {
    int delta = motion->accel.velocity + motion->accel.acceleration * motion->accel.frame;
    *motion->value += delta;
    PokepicMotion_ApplyValue(motion->op, &motion->base, motion->value, motion->dest);
    motion->accel.frame++;
    if (motion->accel.frame >= motion->accel.durationOrLimit) {
        motion->active = 0;
    }
}

void PokepicMotion_UpdateLinear(PokepicMotion *motion) {
    *motion->value = (motion->linear.frame + 1) * motion->linear.total / motion->linear.duration;
    PokepicMotion_ApplyValue(motion->op, &motion->base, motion->value, motion->dest);
    motion->linear.frame++;
    if (motion->linear.frame >= motion->linear.duration) {
        motion->active = 0;
    }
}

void PokepicMotion_UpdateAccelToLimit(PokepicMotion *motion) {
    int delta = motion->accel.velocity + motion->accel.acceleration * motion->accel.frame;
    *motion->value += delta;
    if (motion->op == 0x18 || motion->op == 0x1a) {
        if (delta < 0) {
            int limit = motion->accel.durationOrLimit;
            int *value = motion->value;
            if (*value <= limit) {
                *value = limit;
                motion->active = 0;
            }
        } else {
            int limit = motion->accel.durationOrLimit;
            int *value = motion->value;
            if (*value >= limit) {
                *value = limit;
                motion->active = 0;
            }
        }
    } else if (motion->op == 0x19) {
        int total = motion->base + *motion->value;
        if (delta < 0) {
            if (total <= motion->accel.durationOrLimit) {
                *motion->value += motion->accel.durationOrLimit - total;
                motion->active = 0;
            }
        } else {
            if (total >= motion->accel.durationOrLimit) {
                *motion->value -= total - motion->accel.durationOrLimit;
                motion->active = 0;
            }
        }
    } else {
        GF_AssertFail();
    }
    PokepicMotion_ApplyValue(motion->op, &motion->base, motion->value, motion->dest);
    motion->accel.frame++;
}
