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

int BN_is_bit_set(const BIGNUM *p, int n) {
    int w, b;
    if (n < 0) {
        return 0;
    }
    w = n / 32;
    b = n % 32;
    if (p->top <= w) {
        return 0;
    }
    return (p->d[w] & (1U << b)) != 0;
}
