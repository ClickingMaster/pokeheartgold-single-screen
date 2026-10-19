	.include "asm/macros.inc"
	.public LockCapsuleCard_BufferReceiveDate
	.public LockCapsuleCard_PrintCardText
	.public LockCapsuleCard_PrintCardTitle
	.public LockCapsuleCard_PrintMessageOnly
	.public LockCapsuleCard_PrintNothing
	.public LockCapsuleCard_PrintStatus
	.public LockCapsuleCard_ShowCardMenu
	.public gLockCapsuleCardTextBoxes

	.data

	; page, x, y, width, height, font, colour, fill, message, print function, text x, text y
gLockCapsuleCardTextBoxes:
	.word 0, 2, 2, 0xC, 2, 1, 0x00010200, -1, 0x25, LockCapsuleCard_PrintMessageOnly, 0, 0
	.word 0, 6, 0x11, 0xB, 2, 1, 0x000F0200, -1, 0x29, LockCapsuleCard_PrintMessageOnly, 0, 0
	.word 0, 2, 9, 0x1C, 4, 1, 0x00010200, -1, 0, LockCapsuleCard_PrintStatus, 0, 0
	.word 0, 2, 6, 0x1C, 2, 1, 0x000F0200, -1, 0, LockCapsuleCard_PrintCardTitle, 0, 0
	.word 0, 0x12, 0x11, 0xB, 2, 1, 0x00010200, -1, 0x32, LockCapsuleCard_BufferReceiveDate, 0, 0
	.word 1, 2, 3, 0x1C, 0xA, 1, 0x00010200, -1, 0, LockCapsuleCard_PrintCardText, 0, 0
	.word 1, 2, 0xE, 0x1C, 4, 1, 0x00010200, -1, 0x2A, LockCapsuleCard_PrintNothing, 0, 0
	.word 2, 2, 0x13, 0x1B, 4, 1, 0x00010200, 0xF, 0x25, LockCapsuleCard_PrintMessageOnly, 0, 0
	.word 2, 0x12, 9, 0xC, 4, 1, 0x00010200, 5, 0, LockCapsuleCard_ShowCardMenu, 0, 0
	.word 2, 0x18, 0xD, 6, 4, 1, 0x00010200, 5, 0, 0, 0, 0
	.word 2, 0x18, 0xD, 6, 4, 1, 0x00010200, 5, 0, 0, 0, 0
	; 16 zero bytes nothing references
	.word 0, 0, 0, 0
