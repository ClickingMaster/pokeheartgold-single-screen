typedef unsigned char u8;
typedef unsigned long u32;
void MI_CpuFill8(void *, unsigned char, u32);

typedef struct BIGNUM {
    u32 *d;
    int top;
    int dmax;
    int neg;
    int flags;
} BIGNUM;
typedef struct BN_RECP_CTX {
    BIGNUM N;
    BIGNUM Nr;
    int num_bits;
    int shift;
    int flags;
} BN_RECP_CTX;
typedef struct BN_MONT_CTX {
    int ri;
    u32 n0[2];
    BIGNUM RR;
    BIGNUM N;
    BIGNUM Ni;
    int pad;
    int flags;
} BN_MONT_CTX;
typedef struct BN_CTX {
    int tos;
    BIGNUM bn[12];
    u32 pad[5];
    int flags;
    int depth;
} BN_CTX;

typedef void *(*CryptoAlloc)(u32);
typedef void (*CryptoFree)(void *);
extern CryptoAlloc CRYPTOi_MyAllocFunc;
extern CryptoFree CRYPTOi_MyFreeFunc;
void *CRYPTOi_MyAlloc(u32);
void CRYPTOi_MyFree(void *);
void BN_init(BIGNUM *);
void BN_free(BIGNUM *);
void BN_clear(BIGNUM *);
void BN_clear_free(BIGNUM *);
void BN_MONT_CTX_init(BN_MONT_CTX *);
void BN_CTX_init(BN_CTX *);
void BN_RECP_CTX_init(BN_RECP_CTX *);
void *BN_copy(BIGNUM *, const BIGNUM *);
int BN_set_word(BIGNUM *, u32);
int BN_num_bits(const BIGNUM *);
int BN_num_bits_word(u32);
void MATH_CalcSHA1(void *, const void *, u32);
int CRYPTO_VerifySignatureWithHash(const void *, const void *, u32);

void OS_FreeToHeap(int, int, void *);
void *OS_AllocFromHeap(int, int, u32);
int BN_div(BIGNUM *, BIGNUM *, const BIGNUM *, const BIGNUM *, void *);
int BN_mod(BIGNUM *, const BIGNUM *, const BIGNUM *, void *);
void bn_fix_top(BIGNUM *);
int BN_ucmp(const BIGNUM *, const BIGNUM *);
int BN_mod_exp(BIGNUM *, const BIGNUM *, const BIGNUM *, const BIGNUM *, void *);
int BN_mod_exp_mont(BIGNUM *, const BIGNUM *, const BIGNUM *, const BIGNUM *, void *, void *);
int BN_mod_exp_recp(BIGNUM *, const BIGNUM *, const BIGNUM *, const BIGNUM *, void *);
BIGNUM *bn_expand2(BIGNUM *, int);
int BN_set_word(BIGNUM *, u32);
int BN_is_bit_set(const BIGNUM *, int);
int CRYPTO_VerifySignature(const void *, u32, const void *, u32);
int BN_RECP_CTX_set(BN_RECP_CTX *, const BIGNUM *);
void BN_RECP_CTX_free(BN_RECP_CTX *);
void BN_RECP_CTX_init(BN_RECP_CTX *);
void BN_MONT_CTX_free(BN_MONT_CTX *);
void BN_MONT_CTX_init(BN_MONT_CTX *);
BN_MONT_CTX *BN_MONT_CTX_new(void);
void BN_clear(BIGNUM *);
void BN_CTX_free(BN_CTX *);
void BN_CTX_init(BN_CTX *);
BN_CTX *BN_CTX_new(void);
BIGNUM *BN_new(void);
void BN_init(BIGNUM *);
void BN_free(BIGNUM *);
void BN_clear_free(BIGNUM *);
int BN_num_bits(const BIGNUM *);
void CRYPTO_SetAllocator(CryptoAlloc, CryptoFree);
void CRYPTOi_MyFree(void *);
void *CRYPTOi_MyAlloc(u32);

BN_MONT_CTX *BN_MONT_CTX_new(void) {
    BN_MONT_CTX *p = CRYPTOi_MyAlloc(0x50);
    if (!p) {
        return 0;
    }
    BN_MONT_CTX_init(p);
    p->flags = 1;
    return p;
}
void BN_MONT_CTX_init(BN_MONT_CTX *p) {
    p->ri = 0;
    p->n0[1] = 0;
    BN_init(&p->RR);
    BN_init(&p->N);
    BN_init(&p->Ni);
    p->flags = 0;
}
void BN_MONT_CTX_free(BN_MONT_CTX *p) {
    BN_free(&p->RR);
    BN_free(&p->N);
    BN_free(&p->Ni);
    if (p->flags & 1) {
        CRYPTOi_MyFree(p);
    }
}
void BN_RECP_CTX_init(BN_RECP_CTX *p) {
    BN_init(&p->N);
    BN_init(&p->Nr);
    p->num_bits = 0;
    p->flags = 0;
}
void BN_RECP_CTX_free(BN_RECP_CTX *p) {
    BN_free(&p->N);
    BN_free(&p->Nr);
    if (p->flags & 1) {
        CRYPTOi_MyFree(p);
    }
}
int BN_RECP_CTX_set(BN_RECP_CTX *r, const BIGNUM *n) {
    (void)BN_copy(&r->N, n);
    (void)BN_set_word(&r->Nr, 0);
    r->num_bits = BN_num_bits(n);
    r->shift = 0;
    return 1;
}
