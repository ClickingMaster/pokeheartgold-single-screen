	.include "asm/macros.inc"
	.include "unk_02034B0C.inc"
	.include "global.inc"

	.bss

	.public gCommWireless
gCommWireless:
	.space 0xC

	.rodata

	.public gCommWirelessAnyBssid
gCommWirelessAnyBssid:
	.byte 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
