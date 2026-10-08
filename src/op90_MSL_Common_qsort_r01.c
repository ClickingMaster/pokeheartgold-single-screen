#pragma force_active on
typedef unsigned long size_t;
void qsort(void *base, size_t num, size_t size, int (*compare)(const void *, const void *));
#define SWAP(a, b, s)                  \
    {                                  \
        size_t k = (s);                \
        char *p2 = (b);                \
        char *p1 = (a);                \
        char t;                        \
        for (; k != 0; k--) {          \
            t = *p1;                   \
            *p1++ = *p2;               \
            *p2++ = t;                 \
        }                              \
    }
// @fn qsort
void qsort(void *base, size_t num, size_t size, int (*compare)(const void *, const void *)) {
    size_t l, r, j;
    char *lp, *rp, *ip, *jp;
    if (num < 2) {
        return;
    }
    r = num;
    l = (num >> 1) + 1;
    lp = (char *)base + size * (l - 1);
    rp = (char *)base + size * (r - 1);
    for (;;) {
        if (l > 1) {
            l--;
            lp -= size;
        } else {
            SWAP(lp, rp, size);
            if (--r == 1) {
                return;
            }
            rp -= size;
        }
        j = l;
        jp = (char *)base + size * (j - 1);
        while (j * 2 <= r) {
            ip = jp;
            j *= 2;
            jp = (char *)base + size * (j - 1);
            if (j < r) {
                if (compare(jp, jp + size) < 0) {
                    j++;
                    jp += size;
                }
            }
            if (compare(ip, jp) >= 0) {
                break;
            }
            SWAP(ip, jp, size);
        }
    }
}
