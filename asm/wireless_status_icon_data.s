	.include "asm/macros.inc"
	.include "unk_0203A3B0.inc"
	.include "global.inc"

	.bss

	.public gWirelessStatusIcon
gWirelessStatusIcon:
	.space 0x4

	.rodata

	.public _020F6A9C
_020F6A9C:
	.byte 0xFC, 0x01, 0x14, 0x00
	.byte 0xFF, 0xFF, 0x00, 0x00
	.public _020F6AA4
_020F6AA4:
	.byte 0xF8, 0x01, 0x14, 0x00
	.byte 0xFF, 0xFF, 0x00, 0x00
	.public _020F6AAC
_020F6AAC:
	.byte 0xF4, 0x01, 0x14, 0x00
	.byte 0xFF, 0xFF, 0x00, 0x00
	.public _020F6AB4
_020F6AB4:
	.byte 0xF0, 0x01, 0x14, 0x00
	.byte 0xFF, 0xFF, 0x00, 0x00
	.public _020F6ABC
_020F6ABC:
	.byte 0xF0, 0x01, 0x14, 0x00
	.byte 0xF4, 0x01, 0x14, 0x00
	.byte 0xF8, 0x01, 0x14, 0x00
	.byte 0xFC, 0x01, 0x14, 0x00
	.byte 0xFF, 0xFF, 0x00, 0x00

	.data

	.balign 4, 0
	.public gWirelessStatusIconAnims
gWirelessStatusIconAnims:
	.word _020F6AB4
	.word _020F6AAC
	.word _020F6AA4
	.word _020F6A9C
	.word _020F6ABC

	.balign 4, 0
	.public gWirelessStatusIconNclr
gWirelessStatusIconNclr:
	.asciz "data/pl_wm.NCLR"

	.balign 4, 0
	.public gWirelessStatusIconWifiNcgr
gWirelessStatusIconWifiNcgr:
	.asciz "data/pl_wifi.NCGR"

	.balign 4, 0
	.public gWirelessStatusIconWmNcgr
gWirelessStatusIconWmNcgr:
	.asciz "data/pl_wm.NCGR"

	.balign 4, 0
	.public gWirelessStatusIconWm256kNcgr
gWirelessStatusIconWm256kNcgr:
	.asciz "data/wm256k.NCGR"

.text
