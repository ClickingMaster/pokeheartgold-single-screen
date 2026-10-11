	.include "asm/macros.inc"
	.include "overlay_91.inc"
	.include "global.inc"

	.public _02261BE8
	.public ov91_02261BEC
	.public ov91_02261BF0
	.public ov91_02261BF8
	.public ov91_02261BFE
	.public ov91_02261C04
	.public ov91_02261C0A
	.public ov91_02261C12
	.public ov91_02261C13
	.public ov91_02261C1C
	.public ov91_02261C28
	.public ov91_02261C34
	.public ov91_02261C44
	.public ov91_02261C4C
	.public ov91_02261C58
	.public ov91_02261C64
	.public ov91_02261C74
	.public ov91_02261C84
	.public ov91_02261C9C
	.public ov91_02261CB8
	.public ov91_02261CD4
	.public ov91_02261CF0
	.public ov91_02261D0C
	.public ov91_02261D28
	.public ov91_02261D3C
	.public ov91_02261D5C
	.public ov91_02261D64
	.public ov91_02261D84
	.public ov91_02261DAC
	.public ov91_02261F8C
	.public ov91_02261F90
	.public ov91_022621AC
	.public ov91_022621BC
	.public ov91_022621CC
	.public ov91_022621DC
	.public ov91_022621EC
	.public ov91_022621FC
	.public ov91_0226220C
	.public ov91_0226221C
	.public ov91_0226222C
	.public ov91_0226223C
	.public ov91_0226224C
	.public ov91_0226225C
	.public ov91_0226226C
	.public ov91_0226227C
	.public ov91_0226228C
	.public ov91_0226229C
	.public ov91_022622AC
	.public ov91_022622BC
	.public ov91_022622CC
	.public ov91_022622DC
	.public ov91_022622EC
	.public ov91_022622FC
	.public ov91_0226230C
	.public ov91_0226231C
	.public ov91_0226232C
	.public ov91_0226233C
	.public ov91_0226234C
	.public ov91_0226235C
	.public ov91_0226236C
	.public ov91_0226237C
	.public ov91_0226238C
	.public ov91_0226239C
	.public ov91_022623AC
	.public ov91_022623BC
	.public ov91_022623CC
	.public ov91_022623DC
	.public ov91_022623EC
	.public ov91_022623FC
	.public ov91_0226240C
	.public ov91_0226241C
	.public ov91_0226242C
	.public ov91_0226243C
	.public ov91_0226244C
	.public ov91_0226245C
	.public ov91_0226246C
	.public ov91_0226247C
	.public ov91_0226248C
	.public ov91_0226249C
	.public ov91_022624AC
	.public ov91_022624BC
	.public ov91_022624CC
	.public ov91_022624DC
	.public ov91_022624EC
	.public ov91_022624FC
	.public ov91_0226250C
	.public ov91_0226251C
	.public ov91_0226252C
	.public ov91_0226253C
	.public ov91_0226254C
	.public ov91_0226255C
	.public ov91_0226256C
	.public ov91_0226257C
	.public ov91_0226258C
	.public ov91_0226259C
	.public ov91_022625AC
	.public ov91_022625BC
	.public ov91_022625CC
	.public ov91_022625DC
	.public ov91_022625EC
	.public ov91_022625FC
	.public ov91_0226260C
	.public ov91_0226261C
	.public ov91_0226262C
	.public ov91_0226263C
	.public ov91_0226264C
	.public ov91_0226265C
	.public ov91_0226266C
	.public ov91_0226267C
	.public ov91_0226268C
	.public ov91_0226269C
	.public ov91_022626AC
	.public ov91_022626BC
	.public ov91_022626CC
	.public ov91_022626DC
	.public ov91_022626EC
	.public ov91_022626FC
	.public ov91_0226270C
	.public ov91_0226271C
	.public ov91_0226272C
	.public ov91_0226273C
	.public ov91_0226274C
	.public ov91_02262754
	.public ov91_022627C0
	.extern _0225C5BE
	.extern _0225C5CA
	.extern _0225C5D8
	.extern _0225C5EC
	.extern _0225C5FA
	.extern _0225C604
	.extern _0225C60A
	.extern _0225C61C
	.extern _0225C63C
	.extern _0225C650
	.extern _0225C674
	.extern _0225C67E
	.extern _0225C680
	.extern _0225C6C8
	.extern _0225C706
	.extern _0225C732
	.extern _0225C742
	.extern _0225C74C
	.extern _0225C758
	.extern _0225C764
	.extern _0225C768
	.extern _0225C77E
	.extern _0225C7A8
	.extern _0225C7CE
	.extern _0225C7D0
	.extern _0225C7D8
	.extern _0225C7E0
	.extern _0225C7F6
	.extern _0225C80A
	.extern _0225C82E
	.extern _0225C868
	.extern _0225C878
	.extern _0225C88C
	.extern _0225C8C6
	.extern _0225C8CE
	.extern _0225C8DC
	.extern _0225C8DE
	.extern _0225C8E6
	.extern _0225C8FA
	.extern _0225C922
	.extern _0225C940
	.extern _0225C944
	.extern _0225C948
	.extern _0225C94C
	.extern _0225C95C
	.extern _0225C968
	.extern _0225C97C
	.extern _0225C99C
	.extern _0225C9BE
	.extern _0225C9E4
	.extern _0225C9E6
	.extern _0225CA0A
	.extern _0225CA36
	.extern _0225CA44
	.extern _0225CA5C
	.extern _0225CA60
	.extern _0225CA98
	.extern _0225CAB8
	.extern _0225CACE
	.extern _0225CAE8
	.extern _0225CB28
	.extern _0225CB34
	.extern _0225CB3A
	.extern _0225CB4E
	.extern _0225CB60
	.extern _0225CB7C
	.extern _0225CB94
	.extern _0225CBD8
	.extern _0225CBDE
	.extern _0225CC16
	.extern _0225CC34
	.extern _0225CC6C
	.extern _0225CC82
	.extern _0225CC8C
	.extern _0225CC98
	.extern _0225CCA6
	.extern _0225CCB6
	.extern _0225CCF0
	.extern _0225CD06
	.extern _0225CD12
	.extern _0225CD2E
	.extern _0225CD3E
	.extern _0225CD44
	.extern _0225CD54
	.extern _0225CD80
	.extern _0225CD8E
	.extern _0225CDA4
	.extern _0225CDA8
	.extern _0225CDBE
	.extern _0225CDDE
	.extern _0225CDE8
	.extern _0225CE5E
	.extern _0225CE64
	.extern _0225CE68
	.extern _0225CE6C
	.extern _0225CE70
	.extern _0225CE74
	.extern _0225CE78
	.extern _0225CE7C
	.extern _0225CE90
	.extern _0225CEAC
	.extern _0225CEB0
	.extern _0225CED2
	.extern _0225CED4
	.extern _0225CEE6
	.extern _0225CF14
	.extern _0225CF20
	.extern _0225CF32
	.extern _0225CF44
	.extern _0225CF62
	.extern _0225CF7C
	.extern _0225CF98
	.extern _0225CFA8
	.extern _0225CFD2
	.extern _0225CFDC
	.extern _0225CFF4
	.extern _0225D00A
	.extern _0225D020
	.extern _0225D036
	.extern _0225D054
	.extern _0225D058
	.extern _0225D05C
	.extern _0225D060
	.extern _0225D064
	.extern _0225D068
	.extern _0225D06C
	.extern _0225D070
	.extern _0225D074
	.extern _0225D094
	.extern _0225D09C
	.extern _0225D0E2
	.extern _0225D0F2
	.extern _0225D10A
	.extern _0225D118
	.extern _0225D124
	.extern _0225D13E
	.extern _0225D156
	.extern _0225D158
	.extern _0225D166
	.extern _0225D168
	.extern _0225D1B8
	.extern _0225D1BC
	.extern _0225D1C0
	.extern _0225D1C4
	.extern _0225D1C8
	.extern _0225D1CC
	.extern _0225D1D0
	.extern _0225D1D4
	.extern _0225D1D8
	.extern _0225D202
	.extern _0225D22A
	.extern _0225D250
	.extern _0225D258
	.extern _0225D294
	.extern _0225D2A2
	.extern _0225D2A8
	.extern _0225D2AC
	.extern _0225D2B0
	.extern _0225D2B4
	.extern _0225D2B8
	.extern _0225D2BC
	.extern _0225D2C0
	.extern _0225D2C4
	.extern _0225D2C8
	.extern _0225D2CC
	.extern _0225D2E4
	.extern _0225D324
	.extern _0225D328
	.extern _0225D334
	.extern _0225D35A
	.extern _0225D378
	.extern _0225D3B0
	.extern _0225D3BC
	.extern _0225D3C0
	.extern _0225D404
	.extern _0225D408
	.extern _0225D4CC
	.extern _0225D4F8
	.extern _0225D4FC
	.extern _0225D582
	.extern _0225D5F8
	.extern _0225D5FC
	.extern _0225D60A
	.extern _0225D60E
	.extern _0225D61E
	.extern _0225D624
	.extern _0225D65A
	.extern _0225D682
	.extern _0225D6A6
	.extern _0225D6D0
	.extern _0225D744
	.extern _0225D756
	.extern _0225D782
	.extern _0225D794
	.extern _0225D7B4
	.extern _0225D7C6
	.extern _0225D7D0
	.extern _0225D7E6
	.extern _0225D7EC
	.extern _0225D7F2
	.extern _0225D802
	.extern _0225D808
	.extern _0225D80A
	.extern _0225D854
	.extern _0225D872
	.extern _0225D87A
	.extern _0225D8DC
	.extern _0225D8EC
	.extern _0225D908
	.extern _0225DA0E
	.extern _0225DA5C
	.extern _0225DA60
	.extern _0225DAC6
	.extern _0225DAD2
	.extern _0225DAF8
	.extern _0225DAFC
	.extern _0225DB00
	.extern _0225DB04
	.extern _0225DB08
	.extern _0225DB14
	.extern _0225DB58
	.extern _0225DBA8
	.extern _0225DBBA
	.extern _0225DBCC
	.extern _0225DBF0
	.extern _0225DC18
	.extern _0225DC1C
	.extern _0225DC20
	.extern _0225DC38
	.extern _0225DCB4
	.extern _0225DCB8
	.extern _0225DCBC
	.extern _0225DCC0
	.extern _0225DCD4
	.extern _0225DCD6
	.extern _0225DD1C
	.extern _0225DD4C
	.extern _0225DE84
	.extern _0225DE88
	.extern _0225DEA4
	.extern _0225DEF0
	.extern _0225DEF4
	.extern _0225DEFA
	.extern _0225DFA4
	.extern _0225DFA8
	.extern _0225DFAA
	.extern _0225DFC4
	.extern _0225E03C
	.extern _0225E06C
	.extern _0225E098
	.extern _0225E1F0
	.extern _0225E1F8
	.extern _0225E220
	.extern _0225E22C
	.extern _0225E230
	.extern _0225E234
	.extern _0225E238
	.extern _0225E290
	.extern _0225E2E0
	.extern _0225E2F2
	.extern _0225E30A
	.extern _0225E34C
	.extern _0225E35E
	.extern _0225E36C
	.extern _0225E39C
	.extern _0225E3DE
	.extern _0225E3EA
	.extern _0225E3F0
	.extern _0225E408
	.extern _0225E418
	.extern _0225E434
	.extern _0225E442
	.extern _0225E44E
	.extern _0225E470
	.extern _0225E496
	.extern _0225E498
	.extern _0225E4A8
	.extern _0225E4B0
	.extern _0225E4CE
	.extern _0225E4DA
	.extern _0225E4E8
	.extern _0225E4F6
	.extern _0225E502
	.extern _0225E522
	.extern _0225E538
	.extern _0225E544
	.extern _0225E562
	.extern _0225E56E
	.extern _0225E598
	.extern _0225E5A4
	.extern _0225E5BE
	.extern _0225E5E6
	.extern _0225E5FA
	.extern _0225E600
	.extern _0225E604
	.extern _0225E608
	.extern _0225E60C
	.extern _0225E610
	.extern _0225E614
	.extern _0225E618
	.extern _0225E61C
	.extern _0225E620
	.extern _0225E624
	.extern _0225E628
	.extern _0225E636
	.extern _0225E654
	.extern _0225E666
	.extern _0225E69A
	.extern _0225E6A0
	.extern _0225E6AA
	.extern _0225E6AC
	.extern _0225E6B4
	.extern _0225E6C4
	.extern _0225E6D2
	.extern _0225E6E8
	.extern _0225E6EC
	.extern _0225E724
	.extern _0225E7BC
	.extern _0225E7C0
	.extern _0225E7D0
	.extern _0225E7D2
	.extern _0225E7E0
	.extern _0225E7E4
	.extern _0225E816
	.extern _0225E818
	.extern _0225E822
	.extern _0225E824
	.extern _0225E850
	.extern _0225E854
	.extern _0225E87C
	.extern _0225E87E
	.extern _0225E8FE
	.extern _0225E91A
	.extern _0225E926
	.extern _0225E930
	.extern _0225E93E
	.extern _0225E964
	.extern _0225E970
	.extern _0225E980
	.extern _0225E984
	.extern _0225E988
	.extern _0225E98C
	.extern _0225E9A6
	.extern _0225E9BA
	.extern _0225E9D4
	.extern _0225E9DE
	.extern _0225EA24
	.extern _0225EA2C
	.extern _0225EA4C
	.extern _0225EA50
	.extern _0225EA8C
	.extern _0225EA94
	.extern _0225EAB0
	.extern _0225EAC2
	.extern _0225EAD4
	.extern _0225EAE4
	.extern _0225EB70
	.extern _0225EB74
	.extern _0225EBBC
	.extern _0225EC2E
	.extern _0225EC32
	.extern _0225EC44
	.extern _0225EC46
	.extern _0225EC5C
	.extern _0225EC6C
	.extern _0225EC70
	.extern _0225EC74
	.extern _0225EC78
	.extern _0225ECA4
	.extern _0225ECDA
	.extern _0225ED34
	.extern _0225ED56
	.extern _0225ED5C
	.extern _0225ED60
	.extern _0225ED64
	.extern _0225ED68
	.extern _0225ED78
	.extern _0225EDA2
	.extern _0225EDA8
	.extern _0225EDAC
	.extern _0225EDEC
	.extern _0225EE24
	.extern _0225EE54
	.extern _0225EE70
	.extern _0225EE76
	.extern _0225EE98
	.extern _0225EEB8
	.extern _0225EF4A
	.extern _0225EF4C
	.extern _0225EF60
	.extern _0225EF62
	.extern _0225EF6C
	.extern _0225EF7E
	.extern _0225F00C
	.extern _0225F054
	.extern _0225F058
	.extern _0225F070
	.extern _0225F0A6
	.extern _0225F0B2
	.extern _0225F0BE
	.extern _0225F154
	.extern _0225F162
	.extern _0225F258
	.extern _0225F278
	.extern _0225F280
	.extern _0225F2B2
	.extern _0225F2CE
	.extern _0225F2FA
	.extern _0225F31A
	.extern _0225F326
	.extern _0225F32E
	.extern _0225F340
	.extern _0225F352
	.extern _0225F36A
	.extern _0225F37C
	.extern _0225F380
	.extern _0225F384
	.extern _0225F38E
	.extern _0225F39C
	.extern _0225F3CA
	.extern _0225F3CC
	.extern _0225F3D0
	.extern _0225F3E0
	.extern _0225F3F2
	.extern _0225F3FC
	.extern _0225F40C
	.extern _0225F41E
	.extern _0225F430
	.extern _0225F4A4
	.extern _0225F4A8
	.extern _0225F4AC
	.extern _0225F4B0
	.extern _0225F4B4
	.extern _0225F4B8
	.extern _0225F4BC
	.extern _0225F4F4
	.extern _0225F638
	.extern _0225F63C
	.extern _0225F640
	.extern _0225F644
	.extern _0225F648
	.extern _0225F64C
	.extern _0225F650
	.extern _0225F654
	.extern _0225F658
	.extern _0225F65C
	.extern _0225F660
	.extern _0225F664
	.extern _0225F668
	.extern _0225F71C
	.extern _0225F720
	.extern _0225F724
	.extern _0225F728
	.extern _0225F72C
	.extern _0225F730
	.extern _0225F734
	.extern _0225F738
	.extern _0225F73C
	.extern _0225F740
	.extern _0225F744
	.extern _0225F748
	.extern _0225F798
	.extern _0225F79C
	.extern _0225F7A0
	.extern _0225F7A4
	.extern _0225F868
	.extern _0225F86C
	.extern _0225F870
	.extern _0225F874
	.extern _0225FA44
	.extern _0225FA48
	.extern _0225FA4C
	.extern _0225FA50
	.extern _0225FA54
	.extern _0225FA58
	.extern _0225FA5C
	.extern _0225FB16
	.extern _0225FB40
	.extern _0225FB44
	.extern _0225FB62
	.extern _0225FC14
	.extern _0225FC26
	.extern _0225FDB8
	.extern _0225FDBC
	.extern _0225FDC0
	.extern _0225FDC4
	.extern _0225FDD4
	.extern _0225FDDC
	.extern _0225FEF8
	.extern _0225FF24
	.extern _0225FF82
	.extern _0225FFC8
	.extern _0225FFCA
	.extern _0225FFD2
	.extern _0225FFD6
	.extern _0225FFE4
	.extern _02260000
	.extern _02260004
	.extern _02260006
	.extern _0226000E
	.extern _0226001E
	.extern _02260022
	.extern _02260034
	.extern _022600E4
	.extern _022600E8
	.extern _022600EC
	.extern _022600F0
	.extern _022600F4
	.extern _022600F8
	.extern _0226010A
	.extern _02260120
	.extern _02260174
	.extern _0226018E
	.extern _022601A8
	.extern _022601D0
	.extern _02260202
	.extern _02260220
	.extern _02260242
	.extern _02260290
	.extern _02260294
	.extern _022602A6
	.extern _02260314
	.extern _02260318
	.extern _02260330
	.extern _02260350
	.extern _02260358
	.extern _0226035C
	.extern _02260360
	.extern _0226036C
	.extern _022603A2
	.extern _022603AA
	.extern _022603B6
	.extern _022603B8
	.extern _022603CE
	.extern _022603E2
	.extern _022603E4
	.extern _022603F4
	.extern _022603F8
	.extern _022603FC
	.extern _02260428
	.extern _02260458
	.extern _0226049C
	.extern _022604A0
	.extern _022604EC
	.extern _022604F0
	.extern _02260508
	.extern _022605C2
	.extern _022605D4
	.extern _02260630
	.extern _0226064C
	.extern _02260650
	.extern _02260654
	.extern _02260658
	.extern _0226065C
	.extern _02260660
	.extern _02260664
	.extern _02260668
	.extern _0226067C
	.extern _022606DE
	.extern _02260714
	.extern _02260718
	.extern _0226071C
	.extern _02260720
	.extern _02260724
	.extern _0226074A
	.extern _02260760
	.extern _0226077A
	.extern _02260788
	.extern _0226079C
	.extern _022607AE
	.extern _022607C0
	.extern _02260828
	.extern _0226082C
	.extern _022608CE
	.extern _022608FC
	.extern _02260902
	.extern _02260940
	.extern _02260944
	.extern _02260948
	.extern _0226094C
	.extern _02260966
	.extern _02260978
	.extern _0226097A
	.extern _02260A08
	.extern _02260A0C
	.extern _02260A5C
	.extern _02260A72
	.extern _02260A84
	.extern _02260AEC
	.extern _02260AF2
	.extern _02260B42
	.extern _02260B44
	.extern _02260B52
	.extern _02260B54
	.extern _02260BFA
	.extern _02260C3C
	.extern _02260C40
	.extern _02260C44
	.extern _02260CA4
	.extern _02260CB0
	.extern _02260CDA
	.extern _02260CDC
	.extern _02260D50
	.extern _02260D7C
	.extern _02260D86
	.extern _02260DA4
	.extern _02260DC4
	.extern _02260DDE
	.extern _02260DF2
	.extern _02260E2E
	.extern _02260E34
	.extern _02260E6C
	.extern _02260E78
	.extern _02260E84
	.extern _02260E90
	.extern _02260E9E
	.extern _02260EA6
	.extern _02260EB2
	.extern _02260EB4
	.extern _02260EDC
	.extern _02260F7E
	.extern _02260F86
	.extern _02260F8E
	.extern _02260F94
	.extern _02260FCC
	.extern _02260FE0
	.extern _02260FF0
	.extern _02261010
	.extern _0226105C
	.extern _02261060
	.extern _02261064
	.extern _02261068
	.extern _0226106C
	.extern _0226107E
	.extern _02261096
	.extern _022610D6
	.extern _022610F2
	.extern _022610F4
	.extern _02261114
	.extern _02261128
	.extern _02261144
	.extern _02261146
	.extern _0226116C
	.extern _0226118A
	.extern _02261194
	.extern _02261198
	.extern _022611B4
	.extern _022611B6
	.extern _022611C8
	.extern _022611CC
	.extern _02261378
	.extern _022613E2
	.extern _022613EA
	.extern _02261406
	.extern _0226143E
	.extern _02261450
	.extern _02261456
	.extern _0226145C
	.extern _0226148E
	.extern _022614A2
	.extern _022614CA
	.extern _022614D0
	.extern _022614FA
	.extern _02261516
	.extern _0226151E
	.extern _0226154E
	.extern _02261568
	.extern _02261608
	.extern _02261616
	.extern _022616D2
	.extern _022616FA
	.extern _0226178C
	.extern _0226188C
	.extern _022618A6
	.extern _022618AC
	.extern _0226191E
	.extern _02261924
	.extern _02261942
	.extern _0226194A
	.extern _0226195C
	.extern _02261966
	.extern _02261978
	.extern _0226198A
	.extern _022619A0
	.extern _022619C0
	.extern _022619F8
	.extern _022619FC
	.extern _02261A22
	.extern _02261A2E
	.extern _02261A7C
	.extern _02261A8E
	.extern _02261AA8
	.extern _02261AD4
	.extern _02261AEE
	.extern _02261B02
	.extern _02261B08
	.extern _02261B0C
	.extern _02261B1C
	.extern _02261B24
	.extern _02261B54
	.extern _02261B60
	.extern _02261B88
	.extern _02261BC4
	.extern _02261BD6
	.extern SwalotPlop_Init
	.extern SwalotPlop_Main
	.extern SwalotPlop_Exit
	.extern SwalotPlop_ReceiveStart
	.extern SwalotPlop_ReceiveFinish
	.extern SwalotPlop_TallyShot
	.extern SwalotPlop_ReceiveShot
	.extern SwalotPlop_ReceiveFinalScore
	.extern SwalotPlop_ReceiveScores
	.extern SwalotPlop_ReceiveLevel
	.extern SwalotPlop_ReceiveScoreReport
	.extern SwalotPlop_ReceiveSync
	.extern SwalotPlop_VBlankCallback
	.extern SwalotPlop_SendShots
	.extern SwalotPlop_SendScoreReport
	.extern SwalotPlop_LoadSharedData
	.extern SwalotPlop_FreeSharedData
	.extern SwalotPlop_Abort
	.extern SwalotPlop_SendCommand
	.extern SwalotPlopTally_New
	.extern SwalotPlopTally_Free
	.extern SwalotPlopTally_Update
	.extern SwalotPlopTally_LevelChanged
	.extern SwalotPlopTally_ClearLevelChanged
	.extern SwalotPlopTally_GetLevel
	.extern SwalotPlopTally_AddShot
	.extern SwalotPlopTally_SetScore
	.extern SwalotPlopTally_HasAllScores
	.extern SwalotPlopTally_GetScores
	.extern SwalotPlopTally_CountScores
	.extern SwalotPlopGame_New
	.extern SwalotPlopGame_Free
	.extern SwalotPlopGame_RunIntro
	.extern SwalotPlopGame_RunFinish
	.extern SwalotPlopGame_Update
	.extern SwalotPlopGame_VBlank
	.extern SwalotPlopGame_GetFrame
	.extern SwalotPlopGame_AddRemoteShot
	.extern SwalotPlopGame_TakeShotReport
	.extern SwalotPlopGame_SetFinalScores
	.extern SwalotPlopGame_GetScore
	.extern SwalotPlopGame_SetLevel
	.extern SwalotPlopGame_SetScoreReport
	.extern SwalotPlopGame_SetOptions
	.extern SwalotPlopCurve_Init
	.extern SwalotPlopCurve_Update
	.extern SwalotPlop_GetPopupPos
	.extern SwalotPlopGame_ThrowDemoShot
	.extern SwalotPlopGame_GetHitPoints
	.extern SwalotPlopPopups_Init
	.extern SwalotPlopPopups_Free
	.extern SwalotPlopPopups_Show
	.extern SwalotPlopPopups_Update
	.extern SwalotPlopPopups_InitSeat
	.extern SwalotPlopPopups_FreeSeat
	.extern SwalotPlopPopups_ShowSeat
	.extern SwalotPlopPopups_UpdateSeat
	.extern SwalotPlopPopups_SetPos
	.extern SwalotPlopPopups_HideSprite
	.extern SwalotPlopIntro_Init
	.extern SwalotPlopIntro_Free
	.extern SwalotPlopIntro_ShowWindow
	.extern SwalotPlopIntro_IsCountingDown
	.extern SwalotPlopIntro_HideWindow
	.extern SwalotPlopIntro_StartScene
	.extern SwalotPlopIntro_Wait
	.extern SwalotPlopGfx_SetScenePos
	.extern SwalotPlopFinish_Init
	.extern ov91_0225DBE0
	.extern SwalotPlopFinish_Start
	.extern SwalotPlopFinish_Update
	.extern SwalotPlopFinish_IsSignDone
	.extern SwalotPlopBody_Init
	.extern SwalotPlopBody_Update
	.extern SwalotPlopBody_SetActive
	.extern SwalotPlopBody_SetTilt
	.extern SwalotPlopBody_SetSpin
	.extern SwalotPlopBody_CheckMouth
	.extern SwalotPlopBody_IsInside
	.extern SwalotPlopBody_CalcMouthDepth
	.extern SwalotPlopBody_CalcHeight
	.extern SwalotPlopBody_SetScale
	.extern SwalotPlopBody_SetSpinSpeed
	.extern SwalotPlopBody_SetMouthOffset
	.extern SwalotPlopBody_GetTiltNormal
	.extern SwalotPlopBody_GetTiltFast
	.extern SwalotPlopBody_GetTiltFastest
	.extern SwalotPlopShot_ToRecord
	.extern SwalotPlopShot_FromRecord
	.extern SwalotPlopGame_AddShot
	.extern SwalotPlopShot_End
	.extern SwalotPlopShot_Clear
	.extern SwalotPlopGame_UpdateShots
	.extern SwalotPlopGame_UpdateFlyingShot
	.extern SwalotPlopGame_RemoveEndedShots
	.extern SwalotPlopGame_DropEndedShots
	.extern SwalotPlopShot_CheckRemove
	.extern SwalotPlopGame_DropFlyingShots
	.extern SwalotPlopGame_HasActiveShots
	.extern SwalotPlopShot_GetSpeed
	.extern SwalotPlopShot_GetMove
	.extern SwalotPlopShot_Bounce
	.extern SwalotPlopShot_BounceEx
	.extern SwalotPlopGame_UpdateBouncedShot
	.extern SwalotPlopShot_IsOnRim
	.extern SwalotPlopShot_IsInPool
	.extern SwalotPlopShot_CheckOutside
	.extern SwalotPlopShot_Catch
	.extern SwalotPlopGame_UpdateCaughtShot
	.extern SwalotPlopShot_Move
	.extern SwalotPlopGame_HandleTouch
	.extern SwalotPlopGame_Throw
	.extern SwalotPlopGame_TrackFlick
	.extern SwalotPlopGame_CanStartFlick
	.extern SwalotPlopFlickQueue_Clear
	.extern SwalotPlopFlickQueue_Push
	.extern SwalotPlopFlickQueue_Pop
	.extern SwalotPlopFlickQueue_PeekFirst
	.extern SwalotPlopFlickQueue_PeekLast
	.extern SwalotPlopFlickQueue_Count
	.extern SwalotPlopGame_EndFlick
	.extern SwalotPlopLevelBlend_Init
	.extern SwalotPlopLevelBlend_Start
	.extern SwalotPlopLevelBlend_Update
	.extern SwalotPlopGame_ApplyLevel
	.extern SwalotPlopGame_ApplyLevelParams
	.extern SwalotPlop_GetLevel
	.extern SwalotPlopDemo_Init
	.extern SwalotPlopDemo_Free
	.extern SwalotPlopDemo_Start
	.extern SwalotPlopDemo_Update
	.extern SwalotPlopScoreReports_Init
	.extern SwalotPlopScoreReports_Set
	.extern SwalotPlopScoreReports_Reset
	.extern SwalotPlopScoreReports_GetLeader
	.extern SwalotPlopScoreReports_HasAll
	.extern SwalotPlopGfx_SetBanks
	.extern SwalotPlopGfx_Setup3D
	.extern SwalotPlopGfx_Init3D
	.extern SwalotPlopGfx_Free3D
	.extern SwalotPlopGame_InitGfx
	.extern SwalotPlopGame_FreeGfx
	.extern SwalotPlopGame_UpdateGfx
	.extern SwalotPlopGame_Draw
	.extern SwalotPlopGfx_InitBgs
	.extern SwalotPlopGfx_FreeBgs
	.extern SwalotPlopGfx_InitSprites
	.extern SwalotPlopGfx_FreeSprites
	.extern SwalotPlopGfx_LoadSpriteRes
	.extern SwalotPlopGfx_FreeSpriteRes
	.extern SwalotPlopGfx_InitText
	.extern SwalotPlopGfx_FreeText
	.extern SwalotPlopGfx_InitCamera
	.extern SwalotPlopGfx_FreeCamera
	.extern SwalotPlopGfx_ApplyCamera
	.extern SwalotPlopGfx_LoadSubBgs
	.extern ov91_0225FEFC
	.extern SwalotPlopScene_Init
	.extern SwalotPlopScene_Free
	.extern SwalotPlopScene_Update
	.extern SwalotPlopScene_Animate
	.extern SwalotPlopScene_SetPos
	.extern SwalotPlopScene_SetSpin
	.extern SwalotPlopScene_SetScale
	.extern SwalotPlopScene_Draw
	.extern SwalotPlopScene_SetTiltFrame
	.extern SwalotPlopScene_SetAnimation
	.extern SwalotPlopScene_AdvanceAnimation
	.extern SwalotPlopScene_SetBaseAnimation
	.extern SwalotPlopScene_SetBonus
	.extern SwalotPlopScene_SetLeader
	.extern SwalotPlopScene_UpdateSpecial
	.extern SwalotPlop_TouchToWorld
	.extern SwalotPlop_WorldToTouch
	.extern SwalotPlop_RotateToSeat
	.extern SwalotPlop_RotateFromSeat
	.extern SwalotPlopGfx_InitShotViews
	.extern SwalotPlopGfx_FreeShotViews
	.extern SwalotPlopGfx_AddShotView
	.extern SwalotPlopGfx_RemoveShotView
	.extern SwalotPlopGfx_DrawShotViews
	.extern SwalotPlopShotView_Init
	.extern SwalotPlopShotView_Free
	.extern SwalotPlopShotView_StartSprite
	.extern SwalotPlopShotView_StartModel
	.extern SwalotPlopShotView_Draw
	.extern SwalotPlopShotView_PlaceShadow
	.extern SwalotPlopShotView_ResetPolygonAttr
	.extern SwalotPlopShotView_Clear
	.extern SwalotPlopGfx_GetFreeShotView
	.extern SwalotPlopShotView_UpdateSprite
	.extern SwalotPlopShotView_SwitchToModel
	.extern SwalotPlop_GetShotModel
	.extern SwalotPlopShotView_Spin
	.extern SwalotPlopCursor_Init
	.extern SwalotPlopCursor_Free
	.extern SwalotPlopCursor_Show
	.extern SwalotPlopCursor_Update
	.extern SwalotPlopCursor_SetPos
	.extern SwalotPlopCursor_Hide
	.extern SwalotPlopParticles_Init
	.extern SwalotPlopParticles_Free
	.extern SwalotPlopParticles_Update
	.extern SwalotPlopParticles_Spawn
	.extern SwalotPlop_CalcArcStep
	.extern SwalotPlopStage_Init
	.extern SwalotPlopStage_Free
	.extern SwalotPlopStage_Draw
	.extern SwalotPlopStage_StartMouth
	.extern SwalotPlopStage_SetAnimSpeed
	.extern SwalotPlop_IntersectPlane
	.extern SwalotPlopGfx_SetCameraHeight
	.extern SwalotPlopGfx_SetCameraPitch
	.extern SwalotPlopBgFade_Init
	.extern SwalotPlopBgFade_Update
	.extern SwalotPlopBgFade_Step
	.extern SwalotPlopBgFade_SetLayers
	.extern SwalotPlopCombo_Init
	.extern SwalotPlopCombo_Free
	.extern SwalotPlopCombo_Show
	.extern SwalotPlopCombo_Hide
	.extern SwalotPlopCombo_SetCount
	.extern SwalotPlopCombo_Update
	.extern SwalotPlopSpeedUp_Start
	.extern SwalotPlopSpeedUp_Update
	.extern SwalotPlopSpeedUp_ResetTempo
	.extern SwalotPlop_GetCommandTable
	.extern SwalotPlop_GetCommandCount
	.extern SwalotPlop_RegisterCommands
	.extern SwalotPlop_GetNoDataSize
	.extern SwalotPlop_HandleStart
	.extern SwalotPlop_HandleFinish
	.extern SwalotPlop_HandleShot
	.extern SwalotPlop_HandleFinalScore
	.extern SwalotPlop_HandleScores
	.extern SwalotPlop_HandleScoreReport
	.extern SwalotPlop_HandleSync
	.extern SwalotPlop_HandleLevel
	.extern SwalotPlop_GetShotSize
	.extern SwalotPlop_GetScoresSize
	.extern SwalotPlop_GetScoreSize
	.extern SwalotPlop_GetLevelSize

	.text
	.rodata

_02261BE8:
	.byte 0x00, 0x00, 0x00, 0x00

ov91_02261BEC: ; 0x02261BEC
	.byte 0x00, 0x09, 0x0A, 0x0B

ov91_02261BF0: ; 0x02261BF0
	.byte 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x00, 0x00, 0x00

ov91_02261BF8: ; 0x02261BF8
	.byte 0x26, 0x00, 0x27, 0x00, 0x28, 0x00

ov91_02261BFE: ; 0x02261BFE
	.byte 0x00, 0x00
	.byte 0x01, 0x00, 0x01, 0x00

ov91_02261C04: ; 0x02261C04
	.byte 0x17, 0x00, 0x18, 0x00, 0x19, 0x00

ov91_02261C0A: ; 0x02261C0A
	.byte 0x23, 0x00, 0x23, 0x00, 0x22, 0x00
	.byte 0x21, 0x00

ov91_02261C12: ; 0x02261C12
	.byte 0x00

ov91_02261C13: ; 0x02261C13
	.byte 0x00, 0x01, 0x0A, 0x0B, 0x14, 0x15, 0x1E, 0x1F, 0x28

ov91_02261C1C: ; 0x02261C1C
	.byte 0x1A, 0x00, 0x1B, 0x00
	.byte 0x1C, 0x00, 0x1D, 0x00, 0x1E, 0x00, 0x1F, 0x00

ov91_02261C28: ; 0x02261C28
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0xFA, 0xFF
	.byte 0x00, 0x00, 0x00, 0x00

ov91_02261C34: ; 0x02261C34
	.byte 0x64, 0x00, 0x00, 0x00, 0xC8, 0x00, 0x00, 0x00, 0x2C, 0x01, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00

ov91_02261C44: ; 0x02261C44
	.byte 0x94, 0x02, 0x00, 0x00, 0xEE, 0x02, 0x00, 0x00

ov91_02261C4C: ; 0x02261C4C
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x40, 0x9C, 0x00, 0x00, 0x00, 0x00, 0x00

ov91_02261C58: ; 0x02261C58
	.word SwalotPlopBody_GetTiltNormal
	.word SwalotPlopBody_GetTiltFast
	.word SwalotPlopBody_GetTiltFastest

ov91_02261C64: ; 0x02261C64
	.byte 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x40, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00

ov91_02261C74: ; 0x02261C74
	.byte 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x01, 0x00, 0x00, 0x00

ov91_02261C84: ; 0x02261C84
	.byte 0xB1, 0x00, 0xA7, 0x00, 0x93, 0x00, 0x89, 0x00, 0x7F, 0x00, 0x75, 0x00
	.byte 0x6B, 0x00, 0x61, 0x00, 0x57, 0x00, 0x4D, 0x00, 0x4D, 0x00, 0x4D, 0x00

ov91_02261C9C: ; 0x02261C9C
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x0F, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

ov91_02261CB8: ; 0x02261CB8
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x0E, 0x01, 0x00, 0x01, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00

ov91_02261CD4: ; 0x02261CD4
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x1A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

ov91_02261CF0: ; 0x02261CF0
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x01, 0x00, 0x1B, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

ov91_02261D0C: ; 0x02261D0C
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x1C, 0x00
	.byte 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

ov91_02261D28: ; 0x02261D28
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x1D, 0x00

ov91_02261D3C: ; 0x02261D3C
	.byte 0x00, 0x03, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x7F
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x55, 0x55, 0xAA, 0xAA, 0x00, 0x00

ov91_02261D5C: ; 0x02261D5C
	.byte 0x00, 0x00, 0xFF, 0x7F
	.byte 0xFF, 0x3F, 0xFF, 0xBF

ov91_02261D64: ; 0x02261D64
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x7F, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0xFF, 0x7F, 0x54, 0xD5, 0xAA, 0x2A, 0x00, 0x00, 0xFF, 0x7F, 0x00, 0x00
	.byte 0xFF, 0xBF, 0xFF, 0x3F

ov91_02261D84: ; 0x02261D84
	.byte 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00

ov91_02261DAC: ; 0x02261DAC
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x17, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00
	.byte 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x17, 0x00, 0x00, 0x00
	.byte 0x06, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00
	.byte 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00
	.byte 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x17, 0x00, 0x00, 0x00
	.byte 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00
	.byte 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00
	.byte 0x06, 0x00, 0x00, 0x00, 0x17, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x17, 0x00, 0x00, 0x00
	.byte 0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00
	.byte 0x06, 0x00, 0x00, 0x00, 0x17, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

ov91_02261F8C: ; 0x02261F8C
	.byte 0x17, 0x00, 0x00, 0x00

ov91_02261F90: ; 0x02261F90
	.byte 0x06, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00
	.byte 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00
	.byte 0x90, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00
	.byte 0x90, 0x00, 0x00, 0x00, 0xD0, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00
	.byte 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00
	.byte 0x90, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0xD0, 0x00, 0x00, 0x00
	.byte 0x60, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00
	.byte 0x30, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00
	.byte 0x40, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x90, 0x00, 0x00, 0x00, 0xD0, 0x00, 0x00, 0x00
	.byte 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00
	.byte 0x30, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x90, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00
	.byte 0x60, 0x00, 0x00, 0x00, 0xD0, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xD0, 0x00, 0x00, 0x00
	.byte 0x40, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00
	.byte 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00
	.byte 0x60, 0x00, 0x00, 0x00, 0xD0, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00
	.byte 0x90, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xD0, 0x00, 0x00, 0x00
	.byte 0x60, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00
	.byte 0x30, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x90, 0x00, 0x00, 0x00

ov91_022621AC: ; 0x022621AC
	.ascii "polySurface2"
	.space 16-(.-ov91_022621AC)
ov91_022621BC:
	.ascii "polySurface3"
	.space 16-(.-ov91_022621BC)
ov91_022621CC:
	.ascii "pTorus2"
	.space 16-(.-ov91_022621CC)
ov91_022621DC:
	.ascii "null1"
	.space 16-(.-ov91_022621DC)
ov91_022621EC:
	.ascii ""
	.space 16-(ov91_022621EC-.)
ov91_022621FC:
	.ascii ""
	.space 16-(.-ov91_022621FC)
ov91_0226220C:
	.ascii ""
	.space 16-(.-ov91_0226220C)
ov91_0226221C:
	.ascii ""
	.space 16-(.-ov91_0226221C)
ov91_0226222C:
	.ascii ""
	.space 16-(.-ov91_0226222C)
ov91_0226223C:
	.ascii ""
	.space 16-(.-ov91_0226223C)
ov91_0226224C:
	.ascii ""
	.space 16-(.-ov91_0226224C)
ov91_0226225C:
	.ascii ""
	.space 16-(.-ov91_0226225C)
ov91_0226226C:
	.ascii ""
	.space 16-(.-ov91_0226226C)
ov91_0226227C:
	.ascii ""
	.space 16-(.-ov91_0226227C)
ov91_0226228C:
	.ascii ""
	.space 16-(.-ov91_0226228C)
ov91_0226229C:
	.ascii "polySurface7"
	.space 16-(.-ov91_0226229C)
ov91_022622AC:
	.ascii "polySurface13"
	.space 16-(.-ov91_022622AC)
ov91_022622BC:
	.ascii "pSphere2"
	.space 16-(.-ov91_022622BC)
ov91_022622CC:
	.ascii "pSphere3"
	.space 16-(.-ov91_022622CC)
ov91_022622DC:
	.ascii "pSphere6"
	.space 16-(.-ov91_022622DC)
ov91_022622EC:
	.ascii "pSphere7"
	.space 16-(.-ov91_022622EC)
ov91_022622FC:
	.ascii ""
	.space 16-(.-ov91_022622FC)
ov91_0226230C:
	.ascii ""
	.space 16-(.-ov91_0226230C)
ov91_0226231C:
	.ascii ""
	.space 16-(.-ov91_0226231C)
ov91_0226232C:
	.ascii ""
	.space 16-(.-ov91_0226232C)
ov91_0226233C:
	.ascii ""
	.space 16-(.-ov91_0226233C)
ov91_0226234C:
	.ascii ""
	.space 16-(.-ov91_0226234C)
ov91_0226235C:
	.ascii ""
	.space 16-(.-ov91_0226235C)
ov91_0226236C:
	.ascii ""
	.space 16-(.-ov91_0226236C)
ov91_0226237C:
	.ascii ""
	.space 16-(.-ov91_0226237C)
ov91_0226238C:
	.ascii "polySurface13"
	.space 16-(.-ov91_0226238C)
ov91_0226239C:
	.ascii "pSphere7"
	.space 16-(.-ov91_0226239C)
ov91_022623AC:
	.ascii ""
	.space 16-(.-ov91_022623AC)
ov91_022623BC:
	.ascii ""
	.space 16-(.-ov91_022623BC)
ov91_022623CC:
	.ascii ""
	.space 16-(.-ov91_022623CC)
ov91_022623DC:
	.ascii ""
	.space 16-(.-ov91_022623DC)
ov91_022623EC:
	.ascii ""
	.space 16-(.-ov91_022623EC)
ov91_022623FC:
	.ascii ""
	.space 16-(.-ov91_022623FC)
ov91_0226240C:
	.ascii ""
	.space 16-(.-ov91_0226240C)
ov91_0226241C:
	.ascii ""
	.space 16-(.-ov91_0226241C)
ov91_0226242C:
	.ascii ""
	.space 16-(.-ov91_0226242C)
ov91_0226243C:
	.ascii ""
	.space 16-(.-ov91_0226243C)
ov91_0226244C:
	.ascii ""
	.space 16-(.-ov91_0226244C)
ov91_0226245C:
	.ascii ""
	.space 16-(.-ov91_0226245C)
ov91_0226246C:
	.ascii ""
	.space 16-(.-ov91_0226246C)
ov91_0226247C:
	.ascii "null1"
	.space 16-(.-ov91_0226247C)
ov91_0226248C:
	.ascii "pCylinder4"
	.space 16-(.-ov91_0226248C)
ov91_0226249C:
	.ascii "pCylinder5"
	.space 16-(.-ov91_0226249C)
ov91_022624AC:
	.ascii "pSphere2"
	.space 16-(.-ov91_022624AC)
ov91_022624BC:
	.ascii "pSphere3"
	.space 16-(.-ov91_022624BC)
ov91_022624CC:
	.ascii "pSphere6"
	.space 16-(.-ov91_022624CC)
ov91_022624DC:
	.ascii "pSphere7"
	.space 16-(.-ov91_022624DC)
ov91_022624EC:
	.ascii "pTorus1"
	.space 16-(.-ov91_022624EC)
ov91_022624FC:
	.ascii "pTorus2"
	.space 16-(.-ov91_022624FC)
ov91_0226250C:
	.ascii "polySurface13"
	.space 16-(.-ov91_0226250C)
ov91_0226251C:
	.ascii "polySurface8"
	.space 16-(.-ov91_0226251C)
ov91_0226252C:
	.ascii "polySurface2"
	.space 16-(.-ov91_0226252C)
ov91_0226253C:
	.ascii "polySurface3"
	.space 16-(.-ov91_0226253C)
ov91_0226254C:
	.ascii "polySurface7"
	.space 16-(.-ov91_0226254C)
ov91_0226255C:
	.ascii "polySurface12"
	.space 16-(.-ov91_0226255C)
ov91_0226256C:
	.ascii "polySurface7"
	.space 16-(.-ov91_0226256C)
ov91_0226257C:
	.ascii "polySurface13"
	.space 16-(.-ov91_0226257C)
ov91_0226258C:
	.ascii "pSphere2"
	.space 16-(.-ov91_0226258C)
ov91_0226259C:
	.ascii "pSphere3"
	.space 16-(.-ov91_0226259C)
ov91_022625AC:
	.ascii "pSphere6"
	.space 16-(.-ov91_022625AC)
ov91_022625BC:
	.ascii "pSphere7"
	.space 16-(.-ov91_022625BC)
ov91_022625CC:
	.ascii "null1"
	.space 16-(.-ov91_022625CC)
ov91_022625DC:
	.ascii ""
	.space 16-(.-ov91_022625DC)
ov91_022625EC:
	.ascii ""
	.space 16-(.-ov91_022625EC)
ov91_022625FC:
	.ascii ""
	.space 16-(.-ov91_022625FC)
ov91_0226260C:
	.ascii ""
	.space 16-(.-ov91_0226260C)
ov91_0226261C:
	.ascii ""
	.space 16-(.-ov91_0226261C)
ov91_0226262C:
	.ascii ""
	.space 16-(.-ov91_0226262C)
ov91_0226263C:
	.ascii ""
	.space 16-(.-ov91_0226263C)
ov91_0226264C:
	.ascii ""
	.space 16-(.-ov91_0226264C)
ov91_0226265C:
	.ascii "polySurface7"
	.space 16-(.-ov91_0226265C)
ov91_0226266C:
	.ascii "polySurface13"
	.space 16-(.-ov91_0226266C)
ov91_0226267C:
	.ascii "pSphere2"
	.space 16-(.-ov91_0226267C)
ov91_0226268C:
	.ascii "pSphere3"
	.space 16-(.-ov91_0226268C)
ov91_0226269C:
	.ascii "pSphere6"
	.space 16-(.-ov91_0226269C)
ov91_022626AC:
	.ascii "pSphere7"
	.space 16-(.-ov91_022626AC)
ov91_022626BC:
	.ascii ""
	.space 16-(.-ov91_022626BC)
ov91_022626CC:
	.ascii ""
	.space 16-(.-ov91_022626CC)
ov91_022626DC:
	.ascii ""
	.space 16-(.-ov91_022626DC)
ov91_022626EC:
	.ascii ""
	.space 16-(.-ov91_022626EC)
ov91_022626FC:
	.ascii ""
	.space 16-(.-ov91_022626FC)
ov91_0226270C:
	.ascii ""
	.space 16-(.-ov91_0226270C)
ov91_0226271C:
	.ascii ""
	.space 16-(.-ov91_0226271C)
ov91_0226272C:
	.ascii ""
	.space 16-(.-ov91_0226272C)
ov91_0226273C:
	.ascii ""
	.space 16-(.-ov91_0226273C)

ov91_0226274C: ; 0x0226274C
	.byte 0x00, 0x00, 0x00, 0x02
	.byte 0x01, 0x00, 0x00, 0x00

ov91_02262754: ; 0x02262754
	.word SwalotPlop_HandleStart
	.word SwalotPlop_GetNoDataSize
	.word 0
	.word SwalotPlop_HandleFinish
	.word SwalotPlop_GetNoDataSize
	.word 0
	.word SwalotPlop_HandleShot
	.word SwalotPlop_GetShotSize
	.word 0
	.word SwalotPlop_HandleFinalScore
	.word SwalotPlop_GetScoreSize
	.word 0
	.word SwalotPlop_HandleScores
	.word SwalotPlop_GetScoresSize
	.word 0
	.word SwalotPlop_HandleLevel
	.word SwalotPlop_GetLevelSize
	.word 0
	.word SwalotPlop_HandleScoreReport
	.word SwalotPlop_GetScoreSize
	.word 0
	.word SwalotPlop_HandleSync
	.word SwalotPlop_GetNoDataSize
	.word 0
	; 0x022627B4

	.bss

ov91_022627C0: ; 0x022627C0
	.space 0x40
