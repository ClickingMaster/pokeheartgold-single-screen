	.public gWirelessConnectTag
	.public gWirelessManager
	.public gWirelessConnectTag
	.public gWirelessManager
	.include "asm/macros.inc"
	.include "wireless_manager_data.inc"
	.include "global.inc"

	.data

gWirelessConnectTag:
	.byte 0x44, 0x50, 0x00, 0x00

	.bss

gWirelessManager:
	.space 0x8

.text
