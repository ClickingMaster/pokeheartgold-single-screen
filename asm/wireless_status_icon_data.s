	.include "asm/macros.inc"
	.include "wireless_status_icon_data.inc"
	.include "global.inc"

	.bss

	.rodata

	.public sWirelessStatusIconLevel3Anim
sWirelessStatusIconLevel3Anim:
	.short 0x1FC, 20
	.short 0xFFFF, 0
	.public sWirelessStatusIconLevel2Anim
sWirelessStatusIconLevel2Anim:
	.short 0x1F8, 20
	.short 0xFFFF, 0
	.public sWirelessStatusIconLevel1Anim
sWirelessStatusIconLevel1Anim:
	.short 0x1F4, 20
	.short 0xFFFF, 0
	.public sWirelessStatusIconLevel0Anim
sWirelessStatusIconLevel0Anim:
	.short 0x1F0, 20
	.short 0xFFFF, 0
	.public sWirelessStatusIconCycleAnim
sWirelessStatusIconCycleAnim:
	.short 0x1F0, 20
	.short 0x1F4, 20
	.short 0x1F8, 20
	.short 0x1FC, 20
	.short 0xFFFF, 0

	.data

	.balign 4, 0
	.public gWirelessStatusIconAnims
gWirelessStatusIconAnims:
	.word sWirelessStatusIconLevel0Anim
	.word sWirelessStatusIconLevel1Anim
	.word sWirelessStatusIconLevel2Anim
	.word sWirelessStatusIconLevel3Anim
	.word sWirelessStatusIconCycleAnim

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
