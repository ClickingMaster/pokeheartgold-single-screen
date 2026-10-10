#ifndef DSPROT_STUB_MACRO_H
#define DSPROT_STUB_MACRO_H

// C counterparts of the macros in asm_macro.inc, for the stub sources
// src/*_decrypter.c (decryption wrappers) and src/*_decoder.c (decoding static
// initializers). The stubs are asm functions: see doc/ASSEMBLY.MD for why they
// cannot be C. Their parameters (encoded sizes and keys) are the values elfcoder
// reports for the compiled functions; the Makefile checks them against
// elfcoder's own output ($(BUILD_DIR)/*.ref.s) before compiling a stub.

#include "bss.h"
#include "encoding_constants.h"
#include "encryptor.h"

// Decrypt-run-re-encrypt wrapper around `func`, which elfcoder encrypts with
// `key` over its first `length` bytes. Same body as run_encrypted_func; the
// pc-relative offsets address the four-word pool at the foot (storage for lr,
// length, key, function), and the new key is written back into the pool.
#define RUN_ENCRYPTED_FUNC(name, func, length, key) \
    extern void func(void);                         \
    void name(void);                                \
    asm void name(void) {                           \
        stmfd sp!, {r4-r7};                         \
        stmfd sp!, {r0-r3};                         \
        ldmfd sp!, {r4-r7};                         \
        ldr r1, [pc, #0x60];                        \
        add r3, pc, #0x50;                          \
        str lr, [r3];                               \
        ldr r2, [pc, #0x4c];                        \
        ldr r0, [pc, #0x4c];                        \
        bl Encryptor_DecryptFunction;               \
        mov ip, r0;                                 \
        stmfd sp!, {r4-r7};                         \
        ldmfd sp!, {r0-r3};                         \
        ldmfd sp!, {r4-r7};                         \
        blx ip;                                     \
        stmfd sp!, {r4};                            \
        mov r4, r0;                                 \
        ldr r1, [pc, #0x2c];                        \
        ldr r2, [pc, #0x20];                        \
        ldr r0, [pc, #0x20];                        \
        bl Encryptor_EncryptFunction;               \
        str r0, [pc, #0x18];                        \
        mov r0, r4;                                 \
        ldmfd sp!, {r4};                            \
        ldr lr, [pc, #4];                           \
        str pc, [pc, #0];                           \
        bx lr;                                      \
        dcd DSProt_BSS + 1;                         \
        dcd DSProt_BSS + (length) + ENC_VAL_1;      \
        dcd DSProt_BSS + (key) + ENC_VAL_1;         \
        dcd func + ENC_VAL_1;                       \
    }

// Body of a decoding static initializer: passes the function table that
// follows it in the pool to Encryptor_DecodeFunctionTable.
#define DECODE_FUNC_TABLE                \
    stmdb sp!, {lr};                     \
    add r0, pc, #4;                      \
    bl Encryptor_DecodeFunctionTable;    \
    ldmia sp!, {pc}

// (function, encoded size) entry of a decoding table
#define FUNC_TABLE_ENTRY(func, size) \
    dcd func + ENC_VAL_1;            \
    dcd DSProt_BSS + (size) + ENC_VAL_1

#define FUNC_TABLE_END \
    dcd 0;             \
    dcd 0

#define GARBAGE_REF(ref) \
    dcd ref + ENC_VAL_1

#endif // DSPROT_STUB_MACRO_H
