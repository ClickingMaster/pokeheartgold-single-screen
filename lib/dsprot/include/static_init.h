#ifndef DSPROT_STATIC_INIT_H
#define DSPROT_STATIC_INIT_H

// Registers this file's NitroStaticInit in .sinit, to run when the overlay loads.
static void NitroStaticInit(void);
#pragma define_section SINIT ".sinit" abs32 RWX
#pragma section SINIT begin
SDK_FORCE_EXPORT static void (*NitroStaticInit_[])(void) = { NitroStaticInit };
#pragma section SINIT end

#endif // DSPROT_STATIC_INIT_H
