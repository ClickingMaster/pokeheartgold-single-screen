	.include "asm/macros.inc"
	.public _02258918
	.public _0225891C
	.public _02258920
	.public _02258924
	.public _02258928
	.public _0225894A
	.public _02258956
	.public _02258976
	.public _02258994
	.public _022589A4
	.public _022589CA
	.public _022589D0
	.public _022589EC
	.public _022589F0
	.public _022589F2
	.public _02258A0A
	.public _02258A32
	.public _02258A42
	.public _02258A4E
	.public _02258A64
	.public _02258A74
	.public _02258A80
	.public _02258AB2
	.public _02258ACE
	.public _02258ADC
	.public _02258B0E
	.public _02258B20
	.public _02258B22
	.public _02258B40
	.public _02258B46
	.public _02258B4C
	.public _02258B74
	.public _02258B94
	.public _02258BA6
	.public _02258BB4
	.public _02258BD8
	.public _02258BF6
	.public _02258C16
	.public _02258C24
	.public _02258C28
	.public _02258C30
	.public _02258C34
	.public _02258C68
	.public _02258CB0
	.public _02258D98
	.public _02258D9C
	.public _02258DA0
	.public _02258DA4
	.public _02258DC6
	.public _02258DD2
	.public _02258DE8
	.public _02258E06
	.public _02258E14
	.public _02258E2E
	.public _02258E3C
	.public _02258E4A
	.public _02258E5A
	.public _02258E6E
	.public _02258E74
	.public _02258E7E
	.public _02258E9A
	.public _02258EAE
	.public _02258EBC
	.public _02258EDC
	.public _02258EF2
	.public _02258EF8
	.public _02258F2C
	.public _02258F6C
	.public _02258FAE
	.public _02259126
	.public _02259138
	.public _02259142
	.public _0225915A
	.public _022591A6
	.public _022591F4
	.public _022591F8
	.public _022591FC
	.public _02259200
	.public _02259204
	.public _02259208
	.public _0225920C
	.public _02259246
	.public _02259254
	.public _0225926A
	.public _02259292
	.public _022592AE
	.public _022592B0
	.public _022592B4
	.public sWiFiPlazaConnectBgLayers
	.public WiFiPlazaConnect_Init
	.public WiFiPlazaConnect_Main
	.public WiFiPlazaConnect_Exit
	.public WiFiPlazaConnect_DisconnectInit
	.public WiFiPlazaConnect_DisconnectMain
	.public WiFiPlazaConnect_DisconnectExit
	.public WiFiPlazaConnect_InitGraphics
	.public WiFiPlazaConnect_PaletteCycleTask
	.public sWiFiPlazaConnectGraphicsModes
	.public sWiFiPlazaConnectGraphicsBanks
	.public sWiFiPlazaConnectBgTemplates
	.public sWiFiPlazaConnectBgLayers
	.public WiFiPlazaConnect_Init
	.public WiFiPlazaConnect_Main
	.public WiFiPlazaConnect_Exit
	.public WiFiPlazaConnect_DisconnectInit
	.public WiFiPlazaConnect_DisconnectMain
	.public WiFiPlazaConnect_DisconnectExit
	.public WiFiPlazaConnect_VBlankCB
	.public WiFiPlazaConnect_InitGraphics
	.public WiFiPlazaConnect_PaletteCycleTask
	.public WiFiPlazaConnect_FreeBgs
	.public WiFiPlazaConnect_DoBgUpdates
	.public WiFiPlazaConnect_InitMessageBox
	.public WiFiPlazaConnect_PrintMessage
	.public WiFiPlazaConnect_ClearMessageBox
	.public WiFiPlazaConnect_StartWaitingIcon
	.public WiFiPlazaConnect_StopWaitingIcon
	.public WiFiPlazaConnect_FreeMessageBox
	.public WiFiPlazaConnect_PrintCenteredTitle
	.public WiFiPlazaConnect_BufferErrorCode
	.public WiFiPlazaConnect_CreateYesNoPrompt
	.include "overlay_46.inc"
	.include "global.inc"

	.text
	.public WiFiPlazaConnect_VBlankCB
	.public WiFiPlazaConnect_FreeBgs
	.public WiFiPlazaConnect_DoBgUpdates
	.public WiFiPlazaConnect_InitMessageBox
	.public WiFiPlazaConnect_PrintMessage
	.public WiFiPlazaConnect_ClearMessageBox
	.public WiFiPlazaConnect_StartWaitingIcon
	.public WiFiPlazaConnect_StopWaitingIcon
	.public WiFiPlazaConnect_FreeMessageBox
	.public WiFiPlazaConnect_PrintCenteredTitle
	.public WiFiPlazaConnect_BufferErrorCode
	.public WiFiPlazaConnect_CreateYesNoPrompt
	.rodata

sWiFiPlazaConnectBgLayers:
	.byte 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00
	.byte 0x04, 0x00, 0x00, 0x00

sWiFiPlazaConnectGraphicsModes: ; 0x022595A4
	.byte 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00

sWiFiPlazaConnectGraphicsBanks: ; 0x022595B4
	.byte 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

sWiFiPlazaConnectBgTemplates: ; 0x022595DC
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x1D, 0x00
	.byte 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x1C, 0x04, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x1D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
