	.include "asm/macros.inc"
	.include "comm_wireless_data.inc"
	.include "global.inc"

	.bss

	.public gCommWireless
gCommWireless:
	.space 0xC

	.rodata

	.public gCommWirelessAnyBssid
gCommWirelessAnyBssid:
	.byte 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
