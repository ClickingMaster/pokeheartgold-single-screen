	.include "asm/macros.inc"
	.include "battle_record_data.inc"
	.include "global.inc"

	.bss

	.public gBattleRecord
gBattleRecord:
	.space 0x4

	.rodata

	.public gBattleRecordMultiOrder
gBattleRecordMultiOrder:
	.byte 0x00, 0x02, 0x01, 0x03
	.public gBattleRecordBattlerOrder
gBattleRecordBattlerOrder:
	.byte 0x00, 0x02, 0x03, 0x01, 0x03, 0x01, 0x00, 0x02
