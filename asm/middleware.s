	.public _version_NINTENDO_DWC
	.public _version_NINTENDO_WiFi
	.public _version_UBIQUITOUS_CPS
	.public _version_UBIQUITOUS_SSL
	.public _version_Abiosso_libVCT
	.public _version_NINTENDO_BACKUP
	.public _version_NINTENDO_DWC_LOBBY

	.section .version,4

	.balign 4, 0
_version_NINTENDO_DWC:
	.asciz "[SDK+NINTENDO:DWC2.2.30008.080630.1906_DWC_2_2_PLUS8]"

	.balign 4, 0
_version_NINTENDO_WiFi:
	.asciz "[SDK+NINTENDO:WiFi2.1.30003.0709200229]"

	.balign 4, 0
_version_UBIQUITOUS_CPS:
	.asciz "[SDK+UBIQUITOUS:CPS]"

	.balign 4, 0
_version_UBIQUITOUS_SSL:
	.asciz "[SDK+UBIQUITOUS:SSL]"

	.balign 4, 0
_version_Abiosso_libVCT:
	.asciz "[SDK+Abiosso:libVCT 1.3.1]"

	.balign 4, 0
_version_NINTENDO_BACKUP:
	.asciz "[SDK+NINTENDO:BACKUP]"

	.balign 4, 0
_version_NINTENDO_DWC_LOBBY:
	.asciz "[SDK+NINTENDO:DWC_LOBBY]"
