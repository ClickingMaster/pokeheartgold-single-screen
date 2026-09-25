typedef unsigned char u8;
typedef unsigned short u16;
extern void MI_CpuCopy8(const void *, void *, unsigned);
extern u8 OSi_ThreadInfo[];
extern u8 _021D4430[];
extern unsigned OS_DisableInterrupts(void);
extern void OS_RestoreInterrupts(unsigned);
extern int _s32_div_f(int, int);
int SOCLi_GetOptimumSendBufLen(int);
int SOCLi_MemCpy(void *, int, u8 *);

int SOCLi_GetOptimumSendBufLen(int size) {
    u8 *thread = *(u8 **)(OSi_ThreadInfo + 4);
    u8 *pcb = *(u8 **)(thread + 0xa4);
    unsigned state = OS_DisableInterrupts();
    int limit;
    if (pcb != 0) {
        int left = *(u16 *)(pcb + 0x2e);
        if (left != 0 && *(u16 *)(pcb + 0x2c) != 0) {
            int right = *(u16 *)(pcb + 0x2c);
            int max = *(int *)(_021D4430 + 0x24);
            if (left > right) {
                left = right;
            }
            if (left > max) {
                left = max;
            }
            limit = left * 2;
        } else {
            limit = *(int *)(_021D4430 + 0x24) * 2;
        }
    } else {
        limit = *(int *)(_021D4430 + 0x24) * 2;
    }
    OS_RestoreInterrupts(state);
    if (limit > 0) {
        int n = _s32_div_f(size, limit);
        if (n > 0) {
            return limit * n;
        }
    }
    return size;
}

int SOCLi_MemCpy(void *dst, int size, u8 *packet) {
    int first = *(int *)(packet + 0x14);
    int second = *(int *)(packet + 0x1c);
    if (first > size) {
        first = size;
        second = 0;
    } else if (second > size - first) {
        second = size - first;
    }
    if (first > 0) {
        MI_CpuCopy8(*(void **)(packet + 0x10), dst, first);
        *(u8 **)(packet + 0x10) += first;
        *(int *)(packet + 0x14) -= first;
    }
    if (second > 0) {
        MI_CpuCopy8(*(void **)(packet + 0x18), (u8 *)dst + first, second);
        *(u8 **)(packet + 0x18) += second;
        *(int *)(packet + 0x1c) -= second;
    }
    return first + second;
}
