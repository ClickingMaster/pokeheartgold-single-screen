#include "global.h"

#include "easy_chat.h"
#include "math_util.h"
#include "wifi_plaza_internal.h"

// Receive callbacks (see wifi_plaza_receive.c): the sender id marks its lobby slot in WiFiPlazaWork.unk_1e8 or joins a
// group of WiFiPlazaWork.unk_20c.
void ov45_0222C83C(int id, void *data, u32 size, WiFiPlazaWork *work) {
    int slot = WiFiPlazaLobbyClient_IdToSlot(id);
    if (slot != -1) {
        work->unk_1e8[slot] = 1;
    }
}

void ov45_0222C858(int id, void *data, u32 size, WiFiPlazaWork *work) {
    int slot = WiFiPlazaLobbyClient_IdToSlot(id), value;
    if (slot != -1) {
        for (value = 0; value < 3; value++) {
            if (ov45_0222C408(&work->unk_20c, slot, value) != -1) {
                break;
            }
        }
    }
}

void ov45_0222C88C(int id, void *data, u32 size, WiFiPlazaWork *work) {
    int slot = WiFiPlazaLobbyClient_IdToSlot(id);
    if (slot != -1) {
        ov45_0222C514(&work->unk_20c, slot);
    }
}

void WiFiPlazaGroupWeights_Init(WiFiPlazaGroupWeights *weights) {
    int i;
    memset(weights, 0, sizeof(WiFiPlazaGroupWeights));
    for (i = 0; i < 9; i++) {
        weights->weights[i] = 100;
    }
}

// The first time the player of a lobby slot reports a place, the weight of that place's group is halved.
void WiFiPlazaGroupWeights_ReportPlace(WiFiPlazaGroupWeights *weights, u32 place, u32 slot) {
    if (slot >= 20) {
        GF_AssertFail();
    }
    if (place >= 27) {
        GF_AssertFail();
    }
    if (weights->seen[slot] == 0) {
        weights->weights[place / 3] >>= 1;
        weights->seen[slot] = 1;
    }
}

// A random group by weight; returns its first place (group * 3).
u32 WiFiPlazaGroupWeights_PickGroup(WiFiPlazaGroupWeights *weights) {
    int sum = 0;
    int i = 0;
    u32 target;
    u32 accum;
    while (i < 9) {
        sum += weights->weights[i];
        i++;
    }
    target = MTRandom() % (u32)sum;
    accum = 0;
    i = 0;
    while (i < 9) {
        if (accum <= target && accum + weights->weights[i] > target) {
            return i * 3;
        }
        accum += weights->weights[i];
        i++;
    }
    GF_AssertFail();
    return 0;
}

void WiFiPlaza_SetSlotFlag(u8 *flags, u32 slot, u8 value) {
    if (slot >= 20) {
        GF_AssertFail();
    }
    if (slot < 20) {
        flags[slot] = value;
    }
}

u32 WiFiPlaza_GetSlotFlag(u8 *flags, u32 slot) {
    if (slot >= 20) {
        GF_AssertFail();
    }
    if (slot < 20) {
        return flags[slot];
    }
    return 0;
}

void WiFiPlazaWordSet_Init(WiFiPlazaWordSet *wordSet, enum HeapID heapID) {
    MI_CpuFill8(wordSet, 0, sizeof(WiFiPlazaWordSet));
    wordSet->bank = WallpaperPasswordBank_Create(heapID);
}

void WiFiPlazaWordSet_Free(WiFiPlazaWordSet *wordSet) {
    WallpaperPasswordBank_Delete(wordSet->bank);
}

// Sets the flag of a lobby slot; flag 1 also makes its four words from code.
void WiFiPlazaWordSet_Set(WiFiPlazaWordSet *wordSet, u32 slot, u8 flag, u32 code) {
    if (slot >= 20) {
        GF_AssertFail();
    }
    if (slot < 20) {
        wordSet->flags[slot] = flag;
        if (flag == 1) {
            WiFiPlaza_MakeWordsFromCode(wordSet->bank, code, wordSet->words[slot]);
        }
    }
}

u32 WiFiPlazaWordSet_HasWords(WiFiPlazaWordSet *wordSet, u32 slot) {
    if (slot >= 20) {
        GF_AssertFail();
    }
    if (slot < 20) {
        return wordSet->flags[slot];
    }
    return 0;
}

u16 *WiFiPlazaWordSet_GetWords(WiFiPlazaWordSet *wordSet, u32 slot) {
    if (slot >= 20) {
        GF_AssertFail();
    }
    if (slot < 20 && wordSet->flags[slot]) {
        return wordSet->words[slot];
    }
    return NULL;
}

#pragma force_active on

// Four words of the bank, each picked by the sum of two neighbouring bytes of code.
void WiFiPlaza_MakeWordsFromCode(WallpaperPasswordBank *bank, u32 code, u16 *words) {
    union {
        u32 word;
        u8 bytes[4];
    } value;

    u32 count;
    u32 a;
    u32 b;
    u32 c;
    u32 d;
    count = WallpaperPasswordBank_GetCount(bank);
    value.word = code;
    a = value.bytes[0];
    d = value.bytes[3];
    words[0] = WallpaperPasswordBank_GetWordAtIndex(bank, (d + a) % count);
    b = value.bytes[1];
    words[1] = WallpaperPasswordBank_GetWordAtIndex(bank, (a + b) % count);
    c = value.bytes[2];
    words[2] = WallpaperPasswordBank_GetWordAtIndex(bank, (b + c) % count);
    words[3] = WallpaperPasswordBank_GetWordAtIndex(bank, (c + d) % count);
}

#pragma force_active reset

void WiFiPlazaTimer_Init(WiFiPlazaTimer *timer, int start, int end) {
    timer->active = 0;
    timer->elapsed = 0;
    timer->duration = (end - start) * 30;
}

void WiFiPlazaTimer_Start(WiFiPlazaTimer *timer) {
    timer->active = 1;
    timer->elapsed = 0;
    WiFiPlazaLobbyClient_GetTime(&timer->start);
}

// Runs the timer (30 per second of WiFiPlazaLobbyClient_GetTime time) and steps its phase through _02254A28; phase 5 for 120
// updates at the end.
void WiFiPlazaTimer_Update(WiFiPlazaTimer *timer) {
    s64 now;
    s64 delta;
    int accum;
    u32 i;
    if (timer->active != 0) {
        WiFiPlazaLobbyClient_GetTime(&now);
        delta = (now - timer->start) * 30;
        if ((s64)timer->elapsed < delta) {
            timer->elapsed = (s32)delta;
        }
        if (timer->elapsed < timer->duration) {
            u32 value;
            timer->elapsed++;
            value = (timer->elapsed << 8) / timer->duration;
            accum = 0;
            for (i = 0; i < 5; i++) {
                accum += _02254A28[i].length;
                if ((u32)accum >= value) {
                    if (timer->phase != _02254A28[i].phase) {
                        timer->phase = _02254A28[i].phase;
                        timer->counter = 0;
                    }
                    break;
                }
            }
        } else {
            if (timer->phase != 5) {
                timer->phase = 5;
                timer->counter = 0;
            }
            if (timer->counter >= 120) {
                timer->phase = 0;
                timer->active = 0;
            }
        }
        timer->counter++;
    }
}

u32 WiFiPlazaTimer_GetCounter(WiFiPlazaTimer *timer) {
    return timer->counter;
}

u32 WiFiPlazaTimer_GetPhase(WiFiPlazaTimer *timer) {
    return timer->phase;
}
