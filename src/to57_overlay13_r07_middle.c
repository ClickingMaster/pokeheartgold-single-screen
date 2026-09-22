#include "global.h"

typedef struct Ov13CipherState {
    u32 unk00;
    u32 unk04;
    u8 *table;
    u32 tableSize;
} Ov13CipherState;

extern u8 ov13_0224DD00[];
extern u8 ov13_0224DD02[];

void *ov13_022208E8(int size);
void ov13_022208F8(void *ptr);
void ov13_022214AC(int value);
int ov13_02222AB0(void);
void ov13_02222968(void *dst, const void *src, int size);
int ov13_02222708(const void *data, int size);
void ov13_02222638(Ov13CipherState *state, const u8 *key, u32 keySize, u32 tableSize);
void ov13_0222269C(Ov13CipherState *state, u8 *dst, const u8 *src, u32 size);

int ov13_02222534(void *src, void *dst, int size, u8 *checksum, void *seedDst, const void *seedSrc, int seedSize, const void *key);
int ov13_022225B0(void *dst, const void *src, int size, int expected, void *seedDst, const void *seedSrc, int seedSize);

int ov13_02222534(void *src, void *dst, int size, u8 *checksum, void *seedDst, const void *seedSrc, int seedSize, const void *key) {
    u16 seed;
    Ov13CipherState state;

    *checksum = ov13_02222708(src, size);
    state.table = ov13_022208E8(size);
    if (state.table == NULL) {
        return -1;
    }
    seed = ov13_02222AB0();
    ov13_02222968(seedDst, &seed, 2);
    ov13_02222968(ov13_0224DD00, seedDst, 2);
    ov13_02222968(ov13_0224DD02, seedSrc, seedSize);
    ov13_02222638(&state, ov13_0224DD00, seedSize + 2, size);
    ov13_0222269C(&state, dst, src, size);
    ov13_022208F8(state.table);
    return 0;
}

int ov13_022225B0(void *dst, const void *src, int size, int expected, void *seedDst, const void *seedSrc, int seedSize) {
    Ov13CipherState state;

    state.table = ov13_022208E8(size);
    if (state.table == NULL) {
        ov13_022214AC(2);
        return -1;
    }
    ov13_02222968(ov13_0224DD00, seedDst, 2);
    ov13_02222968(ov13_0224DD02, seedSrc, seedSize);
    ov13_02222638(&state, ov13_0224DD00, seedSize + 2, size);
    ov13_0222269C(&state, src, dst, size);
    {
        int actual = ov13_02222708(src, size);
        if (actual != expected) {
            ov13_022214AC(0x12);
            ov13_022208F8(state.table);
            return -1;
        }
    }
    ov13_022208F8(state.table);
    return 0;
}
