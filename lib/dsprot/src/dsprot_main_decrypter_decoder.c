#include "stub_macro.h"
#include "garbage.h"
#include "static_init.h"

// Decryption wrappers in dsprot_main_decrypter.c
extern void DSProt_DetectFlashcart(void);
extern void DSProt_DetectNotFlashcart(void);
extern void DSProt_DetectEmulator(void);
extern void DSProt_DetectNotEmulator(void);
extern void DSProt_DetectDummy(void);
extern void DSProt_DetectNotDummy(void);

static asm void NitroStaticInit(void) {
    DECODE_FUNC_TABLE
    FUNC_TABLE_ENTRY(DSProt_DetectFlashcart, 0x68)
    FUNC_TABLE_ENTRY(DSProt_DetectNotFlashcart, 0x68)
    FUNC_TABLE_ENTRY(DSProt_DetectEmulator, 0x68)
    FUNC_TABLE_ENTRY(DSProt_DetectNotEmulator, 0x68)
    FUNC_TABLE_ENTRY(DSProt_DetectDummy, 0x68)
    FUNC_TABLE_ENTRY(DSProt_DetectNotDummy, 0x68)
    FUNC_TABLE_END
    GARBAGE_REF(DSProt_Garbage)
}
