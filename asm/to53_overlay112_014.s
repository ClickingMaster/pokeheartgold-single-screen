#include "config.h"
	.include "asm/macros.inc"
	.include "overlay_112.inc"
	.include "global.inc"
	.public gApplication_TitleScreen

	.text
	.extern ov112_021E5900
	.extern ov112_021E5938
	.extern ov112_021E594C
	.extern ov112_021E5964
	.extern ov112_021E59A8
	.extern ov112_021E59B4
	.extern ov112_021E5A14
	.extern ov112_021E5A5C
	.extern ov112_021E5A68
	.extern ov112_021E5A80
	.extern ov112_021E5A84
	.extern ov112_021E5AD0
	.extern ov112_021E5B28
	.extern ov112_021E5B98
	.extern ov112_021E5D38
	.extern ov112_021E5D44
	.extern ov112_021E5D50
	.extern ov112_021E5D5C
	.extern ov112_021E5D68
	.extern ov112_021E5D8C
	.extern ov112_021E5E18
	.extern ov112_021E5E28
	.extern ov112_021E5E48
	.extern ov112_021E5EB8
	.extern ov112_021E5EC4
	.extern ov112_021E5EEC
	.extern ov112_021E6004
	.extern ov112_021E6134
	.extern ov112_021E7464
	.extern ov112_021E7484
	.extern ov112_021E74A8
	.extern ov112_021E7548
	.extern ov112_021E7594
	.extern ov112_021E7620
	.extern ov112_021E7668
	.extern ov112_021E7670
	.extern ov112_021E768C
	.extern ov112_021E76A8
	.extern ov112_021E76CC
	.extern ov112_021E7768
	.extern ov112_021E77E4
	.extern ov112_021E7830
	.extern ov112_021E7910
	.extern ov112_021E795C
	.extern ov112_021E79A8
	.extern ov112_021E79D0
	.extern ov112_021E79F0
	.extern ov112_021E7AC8
	.extern ov112_021E7B18
	.extern ov112_021E7C18
	.extern ov112_021E7C6C
	.extern ov112_021E7CA4
	.extern ov112_021E7CC8
	.extern ov112_021E7D7C
	.extern ov112_021E7DA4
	.extern ov112_021E7DC0
	.extern ov112_021E7DFC
	.extern ov112_021E8B74
	.extern ov112_021E8C5C
	.extern ov112_021E8CB8
	.extern ov112_021E90FC
	.extern ov112_021E9148
	.extern ov112_021E9290
	.extern ov112_021E9338
	.extern ov112_021E935C
	.extern ov112_021E93BC
	.extern ov112_021E9464
	.extern ov112_021E9470
	.extern ov112_021E9480
	.extern ov112_021E95A0
	.extern ov112_021E9610
	.extern ov112_021E966C
	.extern ov112_021E96D0
	.extern ov112_021E9750
	.extern ov112_021E9888
	.extern ov112_021E98A4
	.extern ov112_021E98E8
	.extern ov112_021E98F8
	.extern ov112_021E9934
	.extern ov112_021E9998
	.extern ov112_021E9A30
	.extern ov112_021E9A78
	.extern ov112_021E9C10
	.extern ov112_021E9C94
	.extern ov112_021E9C98
	.extern ov112_021E9E30
	.extern ov112_021E9F40
	.extern ov112_021E9F5C
	.extern ov112_021E9FA4
	.extern ov112_021E9FD8
	.extern ov112_021EA010
	.extern ov112_021EA044
	.extern ov112_021EA08C
	.extern ov112_021EA10C
	.extern ov112_021EA17C
	.extern ov112_021EA19C
	.extern ov112_021EA1AC
	.extern ov112_021EA1F8
	.extern ov112_021EA230
	.extern ov112_021EA2C4
	.extern ov112_021EA3C0
	.extern ov112_021EA4F4
	.extern ov112_021EA51C
	.extern ov112_021EA570
	.extern ov112_021EA584
	.extern ov112_021EA5A4
	.extern ov112_021EA60C
	.extern ov112_021EA64C
	.extern ov112_021EA670
	.extern ov112_021EA688
	.extern ov112_021EA6A0
	.extern ov112_021EA6B8
	.extern ov112_021EA6D8
	.extern ov112_021EA76C
	.extern ov112_021EA7D0
	.extern ov112_021EA838
	.extern ov112_021EA864
	.extern ov112_021EA984
	.extern ov112_021EA9A0
	.extern ov112_021EA9BC
	.extern ov112_021EAA10
	.extern ov112_021EAA34
	.extern ov112_021EAA98
	.extern ov112_021EAAE4
	.extern ov112_021EAB58
	.extern ov112_021EAB78
	.extern ov112_021EABE8
	.extern ov112_021EAC18
	.extern ov112_021EAC34
	.extern ov112_021EAC58
	.extern ov112_021EAC5C
	.extern ov112_021EADD0
	.extern ov112_021EAF48
	.extern ov112_021EAF5C
	.extern ov112_021EAFFC
	.extern ov112_021EB1D8
	.extern ov112_021EB1E8
	.extern ov112_021EB208
	.extern ov112_021EB244
	.extern ov112_021EB28C
	.extern ov112_021EB388
	.extern ov112_021EB3A8
	.extern ov112_021EB3B8
	.extern ov112_021EB3D8
	.extern ov112_021EB44C
	.extern ov112_021EB568
	.extern ov112_021EB5AC
	.extern ov112_021EB5BC
	.extern ov112_021EB5DC
	.extern ov112_021EB664
	.extern ov112_021EB780
	.extern ov112_021EB7D8
	.extern ov112_021EB838
	.extern ov112_021EB85C
	.extern ov112_021EB864
	.extern ov112_021EB8C4
	.extern ov112_021EB8E8
	.extern ov112_021EB908
	.extern ov112_021EB90C
	.extern ov112_021EB950
	.extern ov112_021EB9C0
	.extern ov112_021EB9E4
	.extern ov112_021EBA08
	.extern ov112_021EBA28
	.extern ov112_021EBA50
	.extern ov112_021EBA54
	.extern ov112_021EBA6C
	.extern ov112_021EBA8C
	.extern ov112_021EBAB0
	.extern ov112_021EBAB4
	.extern ov112_021EBB58
	.extern ov112_021EBB84
	.extern ov112_021EBB98
	.extern ov112_021EBBB0
	.extern ov112_021EBBD8
	.extern ov112_021EBC3C
	.extern ov112_021EBC7C
	.extern ov112_021EBCD8
	.extern ov112_021EBD9C
	.extern ov112_021EBDE8
	.extern ov112_021EBE4C
	.extern ov112_021EBE6C
	.extern ov112_021EBE78
	.extern ov112_021EBF18
	.extern ov112_021EBF4C
	.extern ov112_021EBF70
	.extern ov112_021EBFC8
	.extern ov112_021EC000
	.extern ov112_021EC038
	.extern ov112_021EC044
	.extern ov112_021EC134
	.extern ov112_021EC1A8
	.extern ov112_021EC1E4
	.extern ov112_021EC208
	.extern ov112_021EC284
	.extern ov112_021EC2A8
	.extern ov112_021EC2E4
	.extern ov112_021EC304
	.extern ov112_021EC338
	.extern ov112_021EC374
	.extern ov112_021EC378
	.extern ov112_021EC390
	.extern ov112_021EC3B0
	.extern ov112_021EC3C8
	.extern ov112_021EC3F0
	.extern ov112_021EC404
	.extern ov112_021EC414
	.extern ov112_021EC434
	.extern ov112_021EC440
	.extern ov112_021EC460
	.extern ov112_021EC478
	.extern ov112_021EC520
	.extern ov112_021EC54C
	.extern ov112_021EC55C
	.extern ov112_021EC640
	.extern ov112_021EC67C
	.extern ov112_021EC69C
	.extern ov112_021EC6C0
	.extern ov112_021EC6D8
	.extern ov112_021EC6F8
	.extern ov112_021EC70C
	.extern ov112_021EC778
	.extern ov112_021EC7B8
	.extern ov112_021EC7D0
	.extern ov112_021EC8A4
	.extern ov112_021EC950
	.extern ov112_021EC99C
	.extern ov112_021ECA18
	.extern ov112_021ECA88
	.extern ov112_021ECAA8
	.extern ov112_021ECCC4
	.extern ov112_021ECDA0
	.extern ov112_021ECE28
	.extern ov112_021ECECC
	.extern ov112_021ECEEC
	.extern ov112_021ECEF8
	.extern ov112_021ECF38
	.extern ov112_021ECF88
	.extern ov112_021ECFD4
	.extern ov112_021ED020
	.extern ov112_021ED054
	.extern ov112_021ED090
	.extern ov112_021ED0C8
	.extern ov112_021ED0D0
	.extern ov112_021ED13C
	.extern ov112_021ED180
	.extern ov112_021ED1A0
	.extern ov112_021ED224
	.extern ov112_021ED25C
	.extern ov112_021ED264
	.extern ov112_021ED2F4
	.extern ov112_021ED314
	.extern ov112_021ED330
	.extern ov112_021ED35C
	.extern ov112_021ED4D4
	.extern ov112_021ED530
	.extern ov112_021ED590
	.extern ov112_021ED5DC
	.extern ov112_021ED610
	.extern ov112_021ED640
	.extern ov112_021ED66C
	.extern ov112_021ED7DC
	.extern ov112_021ED8DC
	.extern ov112_021ED918
	.extern ov112_021EDA4C
	.extern ov112_021EDAF4
	.extern ov112_021EDB24
	.extern ov112_021EDCB0
	.extern ov112_021EDD08
	.extern ov112_021EDD30
	.extern ov112_021EDD5C
	.extern ov112_021EDD80
	.extern ov112_021EDDB4
	.extern ov112_021EDDE8
	.extern ov112_021EDE1C
	.extern ov112_021EDE68
	.extern ov112_021EDF08
	.extern ov112_021EDF4C
	.extern ov112_021EDFE8
	.extern ov112_021EE044
	.extern ov112_021EE0F4
	.extern ov112_021EE10C
	.extern ov112_021EE12C
	.extern ov112_021EE178
	.extern ov112_021EE1A4
	.extern ov112_021EE1BC
	.extern ov112_021EE1DC
	.extern ov112_021EE1E8
	.extern ov112_021EE29C
	.extern ov112_021EE2B4
	.extern ov112_021EE2D4
	.extern ov112_021EE31C
	.extern ov112_021EE3E0
	.extern ov112_021EE464
	.extern ov112_021EE4B8
	.extern ov112_021EE4D0
	.extern ov112_021EE510
	.extern ov112_021EE514
	.extern ov112_021EE524
	.extern ov112_021EE52C
	.extern ov112_021EE550
	.extern ov112_021EE570
	.extern ov112_021EE594
	.extern ov112_021EE5B8
	.extern ov112_021EE604
	.extern ov112_021EE608
	.extern ov112_021EE624
	.extern ov112_021EE628
	.extern ov112_021EE688
	.extern ov112_021EE6AC
	.extern ov112_021EE6B0
	.extern ov112_021EE6D0
	.extern ov112_021EE6F8
	.extern ov112_021EE70C
	.extern ov112_021EE718
	.extern ov112_021EE73C
	.extern ov112_021EE754
	.extern ov112_021EE778
	.extern ov112_021EE7A8
	.extern ov112_021EE8BC
	.extern ov112_021EE920
	.extern ov112_021EE970
	.extern ov112_021EE9A4
	.extern ov112_021EE9E4
	.extern ov112_021EEA7C
	.extern ov112_021EEAF0
	.extern ov112_021EECF0
	.extern ov112_021EED08
	.extern ov112_021EEDFC
	.extern ov112_021EEE20
	.extern ov112_021EEE2C
	.extern ov112_021EEE4C
	.extern ov112_021EEE8C
	.extern ov112_021EEEA0
	.extern ov112_021EEEAC
	.extern ov112_021EEEC0
	.extern ov112_021EEEE0
	.extern ov112_021EEF08
	.extern ov112_021EEF1C
	.extern ov112_021EEF28
	.extern ov112_021EEF30
	.extern ov112_021EEF68
	.extern ov112_021EF03C
	.extern ov112_021EF100
	.extern ov112_021EF15C
	.extern ov112_021EF17C
	.extern ov112_021EF19C
	.extern ov112_021EF1CC
	.extern ov112_021EF1F0
	.extern ov112_021EF298
	.extern ov112_021EF300
	.extern ov112_021EF310
	.extern ov112_021EF31C
	.extern ov112_021EF3F8
	.extern ov112_021EF568
	.extern ov112_021EF5AC
	.extern ov112_021EF62C
	.extern ov112_021EF640
	.extern ov112_021EF644
	.extern ov112_021EF660
	.extern ov112_021EF664
	.extern ov112_021EF67C
	.extern ov112_021EF6A0
	.extern ov112_021EF700
	.extern ov112_021EF758
	.extern ov112_021EF8C0
	.extern ov112_021EF8D4
	.extern ov112_021EF958
	.extern ov112_021EF994
	.extern ov112_021EF9D4
	.extern ov112_021EFA1C
	.extern ov112_021EFA44
	.extern ov112_021EFA74
	.extern ov112_021EFA9C
	.extern ov112_021EFAC0
	.extern ov112_021EFAD4
	.extern ov112_021EFB14
	.extern ov112_021EFC9C
	.extern ov112_021EFCB0
	.extern ov112_021EFCD8
	.extern ov112_021EFCFC
	.extern ov112_021EFD24
	.extern ov112_021EFD4C
	.extern ov112_021EFDA8
	.extern ov112_021EFED8
	.extern ov112_021EFEEC
	.extern ov112_021EFF14
	.extern ov112_021EFF38
	.extern ov112_021EFF4C
	.extern ov112_021EFF70
	.extern ov112_021EFF84
	.extern ov112_021F00CC
	.extern ov112_021F00E0
	.extern ov112_021F0114
	.extern ov112_021F015C
	.extern ov112_021F0180
	.extern ov112_021F0194
	.extern ov112_021F01B8
	.extern ov112_021F01CC
	.extern ov112_021F0314
	.extern ov112_021F0328
	.extern ov112_021F0368
	.extern ov112_021F0394
	.extern ov112_021F039C
	.extern ov112_021F03BC
	.extern ov112_021F04DC
	.extern ov112_021F050C
	.extern ov112_021F051C
	.extern ov112_021F05CC
	.extern ov112_021F05FC
	.extern ov112_021F0668
	.extern ov112_021F06CC
	.extern ov112_021F0908
	.extern ov112_021F0980
	.extern ov112_021F09B4
	.extern ov112_021F0B28
	.extern ov112_021F0B9C
	.extern ov112_021F0C30
	.extern ov112_021F0C50
	.extern ov112_021F0C8C
	.extern ov112_021F0D04
	.extern ov112_021F0D48
	.extern ov112_021F0DC0
	.extern ov112_021F0DF4
	.extern ov112_021F0E14
	.extern ov112_021F0E60
	.extern ov112_021F0EB4
	.extern ov112_021F0EFC
	.extern ov112_021F0F48
	.extern ov112_021F0F60
	.extern ov112_021F0F70
	.extern ov112_021F0F90
	.extern ov112_021F1288
	.extern ov112_021F1324
	.extern ov112_021F135C
	.extern ov112_021F13BC
	.extern ov112_021F13CC
	.extern ov112_021F1488
	.extern ov112_021F1504
	.extern ov112_021F1548
	.extern ov112_021F1584
	.extern ov112_021F15CC
	.extern ov112_021F15E4
	.extern ov112_021F1624
	.extern ov112_021F178C
	.extern ov112_021F179C
	.extern ov112_021F1814
	.extern ov112_021F1838
	.extern ov112_021F1904
	.extern ov112_021F194C
	.extern ov112_021F196C
	.extern ov112_021F1A08
	.extern ov112_021F1A40
	.extern ov112_021F1AF4
	.extern ov112_021F1B44
	.extern ov112_021F1B94
	.extern ov112_021F1CC8
	.extern ov112_021F1D28
	.extern ov112_021F1D58
	.extern ov112_021F1D70
	.extern ov112_021F1DC0
	.extern ov112_021F1EFC
	.extern ov112_021F1F3C
	.extern ov112_021F1F54
	.extern ov112_021F1F80
	.extern ov112_021F1FD0
	.extern ov112_021F2000
	.extern ov112_021F2098
	.extern ov112_021F2204
	.extern ov112_021F22B0
	.extern ov112_021F22D0
	.extern ov112_021F2328
	.extern ov112_021F2338
	.extern ov112_021F2348
	.extern ov112_021F235C
	.extern ov112_021F238C
	.extern ov112_021F24C4
	.extern ov112_021F24D8
	.extern ov112_021F24F8
	.extern ov112_021F257C
	.extern ov112_021F25FC
	.extern ov112_021F2688
	.extern ov112_021F26E8
	.extern ov112_021F2768
	.extern ov112_021F27B8
	.extern ov112_021F27E0
	.extern ov112_021F2810
	.extern ov112_021F2874
	.extern ov112_021F28DC
	.extern ov112_021F2920
	.extern ov112_021F2944
	.extern ov112_021F298C
	.extern ov112_021F2A78
	.extern ov112_021F2B2C
	.extern ov112_021F2B80
	.extern ov112_021F2CC4
	.extern ov112_021F2CD4
	.extern ov112_021F2DE4
	.extern ov112_021F2E70
	.extern ov112_021F2EB0
	.extern ov112_021F2EF4
	.extern ov112_021F2FAC
	.extern ov112_021F3018
	.extern ov112_021F30BC
	.extern ov112_021F3140
	.extern ov112_021F31BC
	.extern ov112_021F31D8
	.extern ov112_021F3244
	.extern ov112_021F328C
	.extern ov112_021F336C
	.extern ov112_021F33D8
	.extern ov112_021F35A4
	.extern ov112_021F35C8
	.extern ov112_021F3608
	.extern ov112_021F3630
	.extern ov112_021F3664
	.extern ov112_021F366C
	.extern ov112_021F3678
	.extern ov112_021F3684
	.extern ov112_021F3694
	.extern ov112_021F36A4
	.extern ov112_021F36B8
	.extern ov112_021F36DC
	.extern ov112_021F3704
	.extern ov112_021F3720
	.extern ov112_021F373C
	.extern ov112_021F3758
	.extern ov112_021F3774
	.extern ov112_021F3790
	.extern ov112_021F37AC
	.extern ov112_021F3814
	.extern ov112_021F387E
	.extern ov112_021F38E8
	.extern ov112_021F397E
	.extern ov112_021F4138
	.extern ov112_021F413C
	.extern ov112_021F5578
	.extern ov112_021F5588
	.extern ov112_021F5598
	.extern ov112_021F55A8
	.extern ov112_021F55B8
	.extern ov112_021F55C8
	.extern ov112_021F55D8
	.extern ov112_021F55E8
	.extern ov112_021F55F8
	.extern ov112_021F5608
	.extern ov112_021F5618
	.extern ov112_021F5628
	.extern ov112_021F5638
	.extern ov112_021F5648
	.extern ov112_021F5658
	.extern ov112_021F5668
	.extern ov112_021F5678
	.extern ov112_021F5688
	.extern ov112_021F5698
	.extern ov112_021F56A8
	.extern ov112_021F56B8
	.extern ov112_021F56C8
	.extern ov112_021F56D8
	.extern ov112_021F56E8
	.extern ov112_021F56F8
	.extern ov112_021F5708
	.extern ov112_021F5718
	.extern ov112_App_MainMenu_SelectOption_ConnectToPokewalker
	.extern ov112_021F5738
	.extern ov112_021F5748
	.extern ov112_021F5758
	.extern ov112_021F5778
	.extern ov112_021F5798
	.extern ov112_021F57B8
	.extern ov112_021F57D8
	.extern ov112_021F57F8
	.extern ov112_021F5818
	.extern ov112_021F5838
	.extern ov112_021F5858
	.extern ov112_021F5878
	.extern ov112_021F5898
	.extern ov112_021F58B8
	.extern ov112_021F58D8
	.extern ov112_021F58F8
	.extern ov112_021F5918
	.extern ov112_021F5938
	.extern ov112_021F5958
	.extern ov112_021F5978
	.extern ov112_021F5998
	.extern ov112_021F59D8
	.extern ov112_021F5A18
	.extern ov112_021F5A58
	.extern ov112_021F5A98
	.extern ov112_021F5AD8
	.extern ov112_021F5B18
	.extern ov112_021F5B58
	.extern ov112_021F5B98
	.extern ov112_021F5BD8
	.extern ov112_021F5C18
	.extern ov112_021F5C58
	.extern ov112_021F5C98
	.extern ov112_021F5CD8
	.extern ov112_021F5D18
	.extern ov112_021F5D58
	.extern ov112_021F5DB8
	.extern ov112_021F5E18
	.extern ov112_021F5E78
	.extern ov112_021F5ED8
	.extern ov112_021F5F38
	.extern ov112_021F5F98
	.extern ov112_021F5FF8
	.extern ov112_021F6058
	.extern ov112_021F60B8
	.extern ov112_021F6118
	.extern ov112_021F6178
	.extern ov112_021F61F8
	.extern ov112_021F6278
	.extern ov112_021F62F8
	.extern ov112_021F6378
	.extern ov112_021F63F8
	.extern ov112_021F6498
	.extern ov112_021F6538
	.extern ov112_021F65D8
	.extern ov112_021F6678
	.extern ov112_021F6718
	.extern ov112_021F67D8
	.extern ov112_021F6898
	.extern ov112_021F6958
	.extern ov112_021F6A18
	.extern ov112_021F6AD8
	.extern ov112_021F6B98
	.extern ov112_021F6C58
	.extern ov112_021F6D18
	.extern ov112_021F6DD8
	.extern ov112_021F6E98
	.extern ov112_021F6F58
	.extern ov112_021F7018
	.extern ov112_021F70D8
	.extern ov112_021F71D8
	.extern ov112_021F72D8
	.extern ov112_021F7418
	.extern ov112_021F7558
	.extern ov112_021F7698
	.extern ov112_021F77D8
	.extern ov112_021F7918
	.extern ov112_021F7A58
	.extern ov112_021F7B98
	.extern ov112_021F7D18
	.extern ov112_021F7E98
	.extern ov112_021F8018
	.extern ov112_021F8198
	.extern ov112_021F8318
	.extern ov112_021F8498
	.extern ov112_021F8618
	.extern ov112_021F8798
	.extern ov112_021F8918
	.extern ov112_021F8A98
	.extern ov112_021F8C18
	.extern ov112_021F8D98
	.extern ov112_021F8F18
	.extern ov112_021F9098
	.extern ov112_021F9218
	.extern ov112_021F9398
	.extern ov112_021F9518
	.extern ov112_021F9698
	.extern ov112_021F9818
	.extern ov112_021F9998
	.extern ov112_021F9B18
	.extern ov112_021F9C98
	.extern ov112_021F9E18
	.extern ov112_021F9F98
	.extern ov112_021FA118
	.extern ov112_021FA298
	.extern ov112_021FA418
	.extern ov112_021FA598
	.extern ov112_021FA718
	.extern ov112_021FA898
	.extern ov112_021FAA18
	.extern ov112_021FAB98
	.extern ov112_021FAD18
	.extern ov112_021FAE98
	.extern ov112_021FB018
	.extern ov112_021FB198
	.extern ov112_021FB318
	.extern ov112_021FB498
	.extern ov112_021FB618
	.extern ov112_021FB798
	.extern ov112_021FB918
	.extern ov112_021FBA98
	.extern ov112_021FBC18
	.extern ov112_021FBD98
	.extern ov112_021FBF18
	.extern ov112_021FC098
	.extern ov112_021FC218
	.extern ov112_021FC398
	.extern ov112_021FC518
	.extern ov112_021FC698
	.extern ov112_021FC818
	.extern ov112_021FC998
	.extern ov112_021FCB18
	.extern ov112_021FCC98
	.extern ov112_021FCE18
	.extern ov112_021FCF98
	.extern ov112_021FD118
	.extern ov112_021FD298
	.extern ov112_021FD418
	.extern ov112_021FD598
	.extern ov112_021FD718
	.extern ov112_021FD898
	.extern ov112_021FDA18
	.extern ov112_021FDB98
	.extern ov112_021FDD18
	.extern ov112_021FDE98
	.extern ov112_021FE018
	.extern ov112_021FE198
	.extern ov112_021FE498
	.extern ov112_021FEA98
	.extern ov112_021FEAA0
	.extern ov112_021FEAA8
	.extern ov112_021FEAB0
	.extern ov112_021FEAB8
	.extern ov112_021FEAC0
	.extern ov112_021FEAC8
	.extern ov112_021FEAD0
	.extern ov112_021FEAD8
	.extern ov112_021FEAE0
	.extern ov112_021FEAE8
	.extern ov112_021FEAF0
	.extern ov112_021FEAF8
	.extern ov112_021FEB00
	.extern ov112_021FEB08
	.extern ov112_021FEB10
	.extern ov112_021FEB18
	.extern ov112_021FEB20
	.extern ov112_021FEB28
	.extern ov112_021FEB30
	.extern ov112_021FEB38
	.extern ov112_021FEB40
	.extern ov112_021FEB48
	.extern ov112_021FEB50
	.extern ov112_021FEB58
	.extern ov112_021FEB60
	.extern ov112_021FEB68
	.extern ov112_021FEB70
	.extern ov112_021FEB78
	.extern ov112_021FEB80
	.extern ov112_021FEB88
	.extern ov112_021FEB90
	.extern ov112_021FEB98
	.extern ov112_021FEBA0
	.extern ov112_021FEBA8
	.extern ov112_021FEBB0
	.extern ov112_021FEBB8
	.extern ov112_021FEBC0
	.extern ov112_021FEBC8
	.extern ov112_021FEBD0
	.extern ov112_021FEBD8
	.extern ov112_021FEBE0
	.extern ov112_021FEBE8
	.extern ov112_021FEBF0
	.extern ov112_021FEBF8
	.extern ov112_021FEC00
	.extern ov112_021FEC08
	.extern ov112_021FEC10
	.extern ov112_021FEC18
	.extern ov112_021FEC20
	.extern ov112_021FEC28
	.extern ov112_021FEC30
	.extern ov112_021FEC38
	.extern ov112_021FEC40
	.extern ov112_021FEC48
	.extern ov112_021FEC50
	.extern ov112_021FEC58
	.extern ov112_021FEC60
	.extern ov112_021FEC68
	.extern ov112_021FEC70
	.extern ov112_021FEC78
	.extern ov112_021FEC80
	.extern ov112_021FEC88
	.extern ov112_021FEC98
	.extern ov112_021FECAC
	.extern ov112_021FECC4
	.extern ov112_021FECE4
	.extern ov112_021FECE5
	.extern ov112_021FECE6
	.extern ov112_021FECE7
	.extern ov112_021FED1C
	.extern ov112_021FF028
	.extern ov112_021FF05C
	.extern ov112_021FF090
	.extern ov112_021FF09C
	.extern ov112_021FF0AC
	.extern ov112_021FF0C4
	.extern ov112_021FF0DC
	.extern ov112_021FF0EC
	.extern ov112_021FF0FC
	.extern ov112_021FF124
	.extern ov112_021FF134
	.extern ov112_021FF150
	.extern ov112_021FF16C
	.extern ov112_021FF172
	.extern ov112_021FF17A
	.extern ov112_021FF182
	.extern ov112_021FF18A
	.extern ov112_021FF192
	.extern ov112_021FF19A
	.extern ov112_021FF1A4
	.extern ov112_021FF1AE
	.extern ov112_021FF1B8
	.extern ov112_021FF1C6
	.extern ov112_021FF1D8
	.extern ov112_021FF1EC
	.extern ov112_021FF200
	.extern ov112_021FF21C
	.extern ov112_021FF244
	.extern ov112_021FF284
	.extern ov112_021FF2CC
	.extern ov112_021FF2DC
	.extern ov112_021FF2EC
	.extern ov112_021FF2F4
	.extern ov112_021FF304
	.extern ov112_021FF318
	.extern ov112_021FF330
	.extern ov112_021FF350
	.extern ov112_021FF370
	.extern ov112_021FF398
	.extern ov112_021FF3CC
	.extern ov112_021FF400
	.extern ov112_021FF434
	.extern ov112_021FF4C0
	.extern ov112_021FF4C8
	.extern ov112_021FF4D4
	.extern _021FF500
	.extern ov112_021FF504
	.extern ov112_021FF514
	.extern ov112_021FF528
	.extern ov112_021FF54C
	.extern ov112_021FF550
	.extern ov112_021FF554
	.extern ov112_021FF5D0
	.extern ov112_021FF5D8
	.extern ov112_021FF5F0
	.extern ov112_021FF608
	.extern ov112_021FF624
	.extern ov112_021FF640
	.extern ov112_021FF660
	.extern ov112_021FF684
	.extern ov112_021FF6C4
	.extern ov112_021FF730
	.extern ov112_021FF740
	.extern ov112_021FF7CC
	.extern ov112_021FF8D8
	.extern ov112_021FF8F8
	.extern ov112_021FF924
	.extern ov112_021FF9B8
	.extern _021FF9E0
	.extern ov112_021FFA18
	.extern ov112_021FFA20
	.extern ov112_021FFAA4
	.extern ov112_021FFAC0
	.extern ov112_021FFAF0
	.extern ov112_021FFAF8
	.extern ov112_021FFB08
	.extern ov112_021FFB20
	.extern ov112_021FFB24
	.extern ov112_021FFB64
	.extern ov112_021FFBD8


	thumb_func_start ov112_021E6164
ov112_021E6164: ; 0x021E6164
	push {r3, r4, r5, r6, r7, lr}
	bl OS_GetTick
	ldr r2, _021E64C8 ; =ov112_021FFAA4
	ldr r3, [r2, #0x3c]
	ldr r2, [r2, #0x40]
	sub r3, r0, r3
	sbc r1, r2
	lsr r0, r3, #0x1a
	lsl r1, r1, #6
	orr r1, r0
	lsl r0, r3, #6
	ldr r2, _021E64CC ; =0x000082EA
	mov r3, #0
	bl _ll_udiv
	bl _f_ulltof
	ldr r1, _021E64D0 ; =0x447A0000
	bl _fdiv
	ldr r1, _021E64D4 ; =ov112_021FFB24
	ldrb r0, [r1, #0x1d]
	sub r0, #0x46
	cmp r0, #6
	bls _021E619C
	bl _021E6A50
_021E619C:
	add r0, r0, r0
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_021E61A8: ; jump table
	.short _021E61B6 - _021E61A8 - 2 ; case 0
	.short _021E6A50 - _021E61A8 - 2 ; case 1
	.short _021E61CC - _021E61A8 - 2 ; case 2
	.short _021E6A50 - _021E61A8 - 2 ; case 3
	.short _021E63C2 - _021E61A8 - 2 ; case 4
	.short _021E6A50 - _021E61A8 - 2 ; case 5
	.short _021E6A44 - _021E61A8 - 2 ; case 6
_021E61B6:
	mov r0, #0x48
	strb r0, [r1, #0x1d]
	ldr r1, _021E64C8 ; =ov112_021FFAA4
	ldr r0, _021E64D8 ; =0x0000CE80
	ldr r1, [r1, #0x38]
	ldr r2, _021E64DC ; =0x00000D4C
	bl ov112_021E5EC4
	bl ov112_021E6134
	pop {r3, r4, r5, r6, r7, pc}
_021E61CC:
	ldr r0, _021E64C8 ; =ov112_021FFAA4
	mov r4, #0
	ldr r0, [r0, #0x38]
	add r5, r4, #0
	ldrh r2, [r0, #0xa]
	asr r1, r2, #8
	lsl r1, r1, #0x18
	lsr r3, r1, #0x18
	mov r1, #0xff
	lsl r2, r2, #8
	lsl r1, r1, #8
	and r1, r2
	orr r1, r3
	strh r1, [r0, #0xa]
_021E61E8:
	ldr r0, _021E64C8 ; =ov112_021FFAA4
	mov r1, #0xff
	ldr r0, [r0, #0x38]
	lsl r1, r1, #0x18
	add r3, r0, r5
	ldr r0, [r3, #0x70]
	add r4, r4, #1
	lsl r2, r0, #0x18
	add r7, r2, #0
	and r7, r1
	lsl r2, r0, #8
	lsr r1, r1, #8
	and r2, r1
	lsr r1, r0, #0x18
	lsl r1, r1, #0x18
	lsr r6, r1, #0x18
	lsr r1, r0, #8
	mov r0, #0xff
	lsl r0, r0, #8
	and r0, r1
	orr r0, r6
	orr r0, r2
	orr r0, r7
	add r5, r5, #4
	str r0, [r3, #0x70]
	cmp r4, #7
	blt _021E61E8
	ldr r7, _021E64C8 ; =ov112_021FFAA4
	mov r1, #0xff
	ldr r5, [r7, #0x38]
	lsl r1, r1, #0x18
	ldr r0, [r5]
	lsl r2, r0, #0x18
	add r4, r2, #0
	lsl r3, r0, #8
	lsr r2, r1, #8
	and r3, r2
	lsr r2, r0, #0x18
	lsl r2, r2, #0x18
	lsr r6, r2, #0x18
	lsr r2, r0, #8
	lsr r0, r1, #0x10
	and r0, r2
	orr r0, r6
	and r4, r1
	orr r0, r3
	orr r0, r4
	str r0, [r5]
	ldr r0, [r7, #0x38]
	mov r6, #0
	ldr r2, [r0, #4]
	lsl r3, r2, #0x18
	add r5, r3, #0
	lsl r4, r2, #8
	lsr r3, r1, #8
	and r4, r3
	lsr r3, r2, #0x18
	and r5, r1
	lsl r3, r3, #0x18
	lsr r2, r2, #8
	lsr r1, r1, #0x10
	lsr r3, r3, #0x18
	and r1, r2
	orr r1, r3
	orr r1, r4
	orr r1, r5
	str r1, [r0, #4]
	ldr r0, [r7, #0x38]
	mov r7, #0xff
	add r0, #0x8c
	lsl r7, r7, #8
_021E6276:
	ldr r1, [r0]
	mov r2, #0xff
	lsl r3, r1, #0x18
	add r5, r3, #0
	lsl r2, r2, #0x18
	lsl r3, r1, #8
	and r5, r2
	lsr r2, r2, #8
	add r4, r3, #0
	and r4, r2
	lsr r2, r1, #0x18
	lsl r2, r2, #0x18
	lsr r3, r2, #0x18
	lsr r2, r1, #8
	mov r1, #0xff
	lsl r1, r1, #8
	and r1, r2
	orr r1, r3
	orr r1, r4
	orr r1, r5
	str r1, [r0]
	add r1, r0, #0
	add r1, #0x78
	ldrh r2, [r1]
	add r6, r6, #1
	asr r1, r2, #8
	lsl r3, r2, #8
	mov r2, #0xff
	lsl r1, r1, #0x18
	lsl r2, r2, #8
	lsr r1, r1, #0x18
	and r2, r3
	orr r2, r1
	add r1, r0, #0
	add r1, #0x78
	strh r2, [r1]
	add r1, r0, #0
	add r1, #0x7a
	ldrh r2, [r1]
	asr r1, r2, #8
	lsl r3, r2, #8
	mov r2, #0xff
	lsl r1, r1, #0x18
	lsl r2, r2, #8
	lsr r1, r1, #0x18
	and r2, r3
	orr r2, r1
	add r1, r0, #0
	add r1, #0x7a
	strh r2, [r1]
	ldr r1, [r0, #0x7c]
	mov r2, #0xff
	lsl r3, r1, #0x18
	add r5, r3, #0
	lsl r2, r2, #0x18
	lsl r3, r1, #8
	and r5, r2
	lsr r2, r2, #8
	add r4, r3, #0
	and r4, r2
	lsr r2, r1, #0x18
	lsl r2, r2, #0x18
	lsr r3, r2, #0x18
	lsr r2, r1, #8
	mov r1, #0xff
	lsl r1, r1, #8
	and r1, r2
	orr r1, r3
	orr r1, r4
	orr r1, r5
	str r1, [r0, #0x7c]
	add r1, r0, #0
	add r1, #0x80
	ldr r4, [r1]
	mov r1, #0xff
	lsl r2, r4, #0x18
	add r3, r2, #0
	lsl r1, r1, #0x18
	and r3, r1
	lsl r2, r4, #8
	lsr r1, r1, #8
	and r2, r1
	lsr r1, r4, #0x18
	lsl r1, r1, #0x18
	lsr r4, r4, #8
	lsr r1, r1, #0x18
	and r4, r7
	orr r1, r4
	orr r1, r2
	add r2, r3, #0
	orr r2, r1
	add r1, r0, #0
	add r1, #0x80
	add r0, #0x88
	str r2, [r1]
	cmp r6, #0x18
	blt _021E6276
	ldr r1, _021E64D4 ; =ov112_021FFB24
	ldrb r0, [r1, #0x1c]
	cmp r0, #0x60
	bgt _021E6344
	beq _021E638C
	b _021E63A8
_021E6344:
	sub r0, #0xb0
	cmp r0, #0xe
	bhi _021E63A8
	add r0, r0, r0
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_021E6356: ; jump table
	.short _021E6374 - _021E6356 - 2 ; case 0
	.short _021E63A8 - _021E6356 - 2 ; case 1
	.short _021E6374 - _021E6356 - 2 ; case 2
	.short _021E63A8 - _021E6356 - 2 ; case 3
	.short _021E6374 - _021E6356 - 2 ; case 4
	.short _021E63A8 - _021E6356 - 2 ; case 5
	.short _021E6374 - _021E6356 - 2 ; case 6
	.short _021E63A8 - _021E6356 - 2 ; case 7
	.short _021E6374 - _021E6356 - 2 ; case 8
	.short _021E63A8 - _021E6356 - 2 ; case 9
	.short _021E6374 - _021E6356 - 2 ; case 10
	.short _021E63A8 - _021E6356 - 2 ; case 11
	.short _021E6374 - _021E6356 - 2 ; case 12
	.short _021E63A8 - _021E6356 - 2 ; case 13
	.short _021E6374 - _021E6356 - 2 ; case 14
_021E6374:
	mov r0, #0x4a
	strb r0, [r1, #0x1d]
	ldr r1, _021E64C8 ; =ov112_021FFAA4
	mov r0, #0x2e
	ldr r1, [r1, #0x34]
	lsl r0, r0, #0xa
	mov r2, #4
	bl ov112_021E5EC4
	bl ov112_021E6134
	pop {r3, r4, r5, r6, r7, pc}
_021E638C:
	mov r0, #0x4c
	strb r0, [r1, #0x1d]
	ldr r1, _021E64C8 ; =ov112_021FFAA4
	ldr r0, _021E64E0 ; =0x0000DE24
	ldr r2, [r1, #0x30]
	mov r1, #0x89
	lsl r1, r1, #2
	add r1, r2, r1
	ldr r2, _021E64E4 ; =0x00001568
	bl ov112_021E5EC4
	bl ov112_021E6134
	pop {r3, r4, r5, r6, r7, pc}
_021E63A8:
	ldr r0, _021E64D4 ; =ov112_021FFB24
	mov r1, #0x4a
	strb r1, [r0, #0x1d]
	ldr r1, _021E64C8 ; =ov112_021FFAA4
	mov r0, #0x2e
	ldr r1, [r1, #0x34]
	ldr r2, _021E64E8 ; =0x000006C8
	lsl r0, r0, #0xa
	bl ov112_021E5EC4
	bl ov112_021E6134
	pop {r3, r4, r5, r6, r7, pc}
_021E63C2:
	ldrb r0, [r1, #0x1c]
	cmp r0, #0x52
	bgt _021E63D0
	bge _021E6404
	cmp r0, #0x40
	beq _021E6412
	b _021E6A3E
_021E63D0:
	add r2, r0, #0
	sub r2, #0xb0
	cmp r2, #0xe
	bls _021E63DA
	b _021E6A3E
_021E63DA:
	add r2, r2, r2
	add r2, pc
	ldrh r2, [r2, #6]
	lsl r2, r2, #0x10
	asr r2, r2, #0x10
	add pc, r2
_021E63E6: ; jump table
	.short _021E642E - _021E63E6 - 2 ; case 0
	.short _021E6A3E - _021E63E6 - 2 ; case 1
	.short _021E645E - _021E63E6 - 2 ; case 2
	.short _021E6A3E - _021E63E6 - 2 ; case 3
	.short _021E6490 - _021E63E6 - 2 ; case 4
	.short _021E6A3E - _021E63E6 - 2 ; case 5
	.short _021E64C2 - _021E63E6 - 2 ; case 6
	.short _021E6A3E - _021E63E6 - 2 ; case 7
	.short _021E6526 - _021E63E6 - 2 ; case 8
	.short _021E6A3E - _021E63E6 - 2 ; case 9
	.short _021E6664 - _021E63E6 - 2 ; case 10
	.short _021E6A3E - _021E63E6 - 2 ; case 11
	.short _021E67A4 - _021E63E6 - 2 ; case 12
	.short _021E6A3E - _021E63E6 - 2 ; case 13
	.short _021E68FE - _021E63E6 - 2 ; case 14
_021E6404:
	mov r0, #0
	add r1, r0, #0
	mov r2, #0x24
	mov r3, #1
	bl ov112_021E5A68
	pop {r3, r4, r5, r6, r7, pc}
_021E6412:
	mov r0, #0x4c
	strb r0, [r1, #0x1d]
	ldr r1, _021E64C8 ; =ov112_021FFAA4
	ldr r0, _021E64E0 ; =0x0000DE24
	ldr r2, [r1, #0x30]
	mov r1, #0x89
	lsl r1, r1, #2
	add r1, r2, r1
	ldr r2, _021E64E4 ; =0x00001568
	bl ov112_021E5EC4
	bl ov112_021E6134
	pop {r3, r4, r5, r6, r7, pc}
_021E642E:
	ldr r0, _021E64C8 ; =ov112_021FFAA4
	ldr r2, [r0, #0x34]
	ldrb r0, [r2]
	lsl r0, r0, #0x1b
	lsr r0, r0, #0x1f
	beq _021E6448
	mov r0, #0
	add r1, r0, #0
	mov r2, #0x9c
	mov r3, #1
	bl ov112_021E5A68
	pop {r3, r4, r5, r6, r7, pc}
_021E6448:
	mov r0, #0xb0
	strb r0, [r1, #0x1d]
	add r0, r2, #4
	mov r2, #9
	ldr r1, _021E64EC ; =0x0000B804
	lsl r2, r2, #6
	bl ov112_021E5EC4
	bl ov112_021E5EEC
	pop {r3, r4, r5, r6, r7, pc}
_021E645E:
	ldr r0, _021E64C8 ; =ov112_021FFAA4
	ldr r3, [r0, #0x34]
	ldrb r0, [r3]
	lsl r0, r0, #0x1a
	lsr r0, r0, #0x1f
	beq _021E6478
	mov r0, #0
	add r1, r0, #0
	mov r2, #0x9c
	mov r3, #1
	bl ov112_021E5A68
	pop {r3, r4, r5, r6, r7, pc}
_021E6478:
	mov r0, #0xb2
	mov r2, #0x91
	strb r0, [r1, #0x1d]
	lsl r2, r2, #2
	add r0, r3, r2
	ldr r1, _021E64F0 ; =0x0000BA44
	add r2, #0xb8
	bl ov112_021E5EC4
	bl ov112_021E5EEC
	pop {r3, r4, r5, r6, r7, pc}
_021E6490:
	ldr r0, _021E64C8 ; =ov112_021FFAA4
	ldr r3, [r0, #0x34]
	ldrb r0, [r3]
	lsl r0, r0, #0x19
	lsr r0, r0, #0x1f
	beq _021E64AA
	mov r0, #0
	add r1, r0, #0
	mov r2, #0x9c
	mov r3, #1
	bl ov112_021E5A68
	pop {r3, r4, r5, r6, r7, pc}
_021E64AA:
	mov r2, #0xb4
	strb r2, [r1, #0x1d]
	mov r0, #0x15
	lsl r0, r0, #6
	ldr r1, _021E64F4 ; =0x0000BD40
	add r0, r3, r0
	add r2, #0xd4
	bl ov112_021E5EC4
	bl ov112_021E5EEC
	pop {r3, r4, r5, r6, r7, pc}
_021E64C2:
	ldr r0, _021E64C8 ; =ov112_021FFAA4
	b _021E64F8
	nop
_021E64C8: .word ov112_021FFAA4
_021E64CC: .word 0x000082EA
_021E64D0: .word 0x447A0000
_021E64D4: .word ov112_021FFB24
_021E64D8: .word 0x0000CE80
_021E64DC: .word 0x00000D4C
_021E64E0: .word 0x0000DE24
_021E64E4: .word 0x00001568
_021E64E8: .word 0x000006C8
_021E64EC: .word 0x0000B804
_021E64F0: .word 0x0000BA44
_021E64F4: .word 0x0000BD40
_021E64F8:
	ldr r2, [r0, #0x34]
	ldrb r2, [r2]
	lsl r2, r2, #0x18
	lsr r2, r2, #0x1f
	beq _021E6510
	mov r0, #0
	add r1, r0, #0
	mov r2, #0x9c
	mov r3, #1
	bl ov112_021E5A68
	pop {r3, r4, r5, r6, r7, pc}
_021E6510:
	mov r2, #0xb6
	strb r2, [r1, #0x1d]
	mov r1, #0xbf
	ldr r0, [r0, #0xc]
	ldr r2, _021E6824 ; =0x00000CBC
	lsl r1, r1, #8
	bl ov112_021E5EC4
	bl ov112_021E5EEC
	pop {r3, r4, r5, r6, r7, pc}
_021E6526:
	ldr r2, _021E6828 ; =ov112_021FFAA4
	ldr r1, [r2, #4]
	cmp r1, #3
	bhi _021E65A2
	add r1, r1, r1
	add r1, pc
	ldrh r1, [r1, #6]
	lsl r1, r1, #0x10
	asr r1, r1, #0x10
	add pc, r1
_021E653A: ; jump table
	.short _021E6542 - _021E653A - 2 ; case 0
	.short _021E655A - _021E653A - 2 ; case 1
	.short _021E6572 - _021E653A - 2 ; case 2
	.short _021E658A - _021E653A - 2 ; case 3
_021E6542:
	ldr r1, [r2, #0x34]
	ldrb r1, [r1]
	lsl r1, r1, #0x1b
	lsr r1, r1, #0x1f
	beq _021E65A2
	mov r0, #0
	add r1, r0, #0
	mov r2, #0x9c
	mov r3, #1
	bl ov112_021E5A68
	pop {r3, r4, r5, r6, r7, pc}
_021E655A:
	ldr r1, [r2, #0x34]
	ldrb r1, [r1]
	lsl r1, r1, #0x1a
	lsr r1, r1, #0x1f
	beq _021E65A2
	mov r0, #0
	add r1, r0, #0
	mov r2, #0x9c
	mov r3, #1
	bl ov112_021E5A68
	pop {r3, r4, r5, r6, r7, pc}
_021E6572:
	ldr r1, [r2, #0x34]
	ldrb r1, [r1]
	lsl r1, r1, #0x19
	lsr r1, r1, #0x1f
	beq _021E65A2
	mov r0, #0
	add r1, r0, #0
	mov r2, #0x9c
	mov r3, #1
	bl ov112_021E5A68
	pop {r3, r4, r5, r6, r7, pc}
_021E658A:
	ldr r1, [r2, #0x34]
	ldrb r1, [r1]
	lsl r1, r1, #0x18
	lsr r1, r1, #0x1f
	beq _021E65A2
	mov r0, #0
	add r1, r0, #0
	mov r2, #0x9c
	mov r3, #1
	bl ov112_021E5A68
	pop {r3, r4, r5, r6, r7, pc}
_021E65A2:
	ldr r1, _021E6828 ; =ov112_021FFAA4
	mov r5, #0xe
	ldr r3, [r1, #0x34]
	ldrb r2, [r3]
	lsl r4, r2, #0x1c
	lsr r4, r4, #0x1c
	and r5, r4
	cmp r5, #0xe
	bne _021E6640
	mov r0, #0xf
	bic r2, r0
	mov r0, #1
	orr r0, r4
	lsl r0, r0, #0x18
	lsr r4, r0, #0x18
	mov r0, #0xf
	and r0, r4
	orr r0, r2
	strb r0, [r3]
	ldr r0, [r1, #4]
	cmp r0, #3
	bhi _021E663A
	add r0, r0, r0
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_021E65DA: ; jump table
	.short _021E65E2 - _021E65DA - 2 ; case 0
	.short _021E65F8 - _021E65DA - 2 ; case 1
	.short _021E6610 - _021E65DA - 2 ; case 2
	.short _021E6628 - _021E65DA - 2 ; case 3
_021E65E2:
	ldr r0, _021E682C ; =ov112_021FFB24
	mov r2, #0xb0
	strb r2, [r0, #0x1d]
	ldr r0, [r1, #0x34]
	mov r2, #9
	ldr r1, _021E6830 ; =0x0000B804
	add r0, r0, #4
	lsl r2, r2, #6
	bl ov112_021E5EC4
	b _021E663A
_021E65F8:
	ldr r0, _021E682C ; =ov112_021FFB24
	mov r2, #0xb2
	strb r2, [r0, #0x1d]
	ldr r0, [r1, #0x34]
	mov r2, #0x91
	lsl r2, r2, #2
	add r0, r0, r2
	ldr r1, _021E6834 ; =0x0000BA44
	add r2, #0xb8
	bl ov112_021E5EC4
	b _021E663A
_021E6610:
	ldr r0, _021E682C ; =ov112_021FFB24
	mov r2, #0xb4
	strb r2, [r0, #0x1d]
	mov r0, #0x15
	ldr r1, [r1, #0x34]
	lsl r0, r0, #6
	add r0, r1, r0
	ldr r1, _021E6838 ; =0x0000BD40
	add r2, #0xd4
	bl ov112_021E5EC4
	b _021E663A
_021E6628:
	ldr r0, _021E682C ; =ov112_021FFB24
	mov r2, #0xb6
	strb r2, [r0, #0x1d]
	ldr r0, [r1, #0xc]
	mov r1, #0xbf
	ldr r2, _021E6824 ; =0x00000CBC
	lsl r1, r1, #8
	bl ov112_021E5EC4
_021E663A:
	bl ov112_021E5EEC
	pop {r3, r4, r5, r6, r7, pc}
_021E6640:
	mov r3, #1
	add r1, r4, #0
	tst r1, r3
	beq _021E6654
	mov r0, #0
	add r1, r0, #0
	mov r2, #0x9e
	bl ov112_021E5A68
	pop {r3, r4, r5, r6, r7, pc}
_021E6654:
	ldr r1, _021E682C ; =ov112_021FFB24
	mov r2, #0x24
	strb r0, [r1, #0x1d]
	mov r0, #0
	add r1, r0, #0
	bl ov112_021E5A68
	pop {r3, r4, r5, r6, r7, pc}
_021E6664:
	ldr r2, _021E6828 ; =ov112_021FFAA4
	ldr r1, [r2, #4]
	cmp r1, #3
	bhi _021E66E0
	add r1, r1, r1
	add r1, pc
	ldrh r1, [r1, #6]
	lsl r1, r1, #0x10
	asr r1, r1, #0x10
	add pc, r1
_021E6678: ; jump table
	.short _021E6680 - _021E6678 - 2 ; case 0
	.short _021E6698 - _021E6678 - 2 ; case 1
	.short _021E66B0 - _021E6678 - 2 ; case 2
	.short _021E66C8 - _021E6678 - 2 ; case 3
_021E6680:
	ldr r1, [r2, #0x34]
	ldrb r1, [r1]
	lsl r1, r1, #0x1b
	lsr r1, r1, #0x1f
	beq _021E66E0
	mov r0, #0
	add r1, r0, #0
	mov r2, #0x9c
	mov r3, #1
	bl ov112_021E5A68
	pop {r3, r4, r5, r6, r7, pc}
_021E6698:
	ldr r1, [r2, #0x34]
	ldrb r1, [r1]
	lsl r1, r1, #0x1a
	lsr r1, r1, #0x1f
	beq _021E66E0
	mov r0, #0
	add r1, r0, #0
	mov r2, #0x9c
	mov r3, #1
	bl ov112_021E5A68
	pop {r3, r4, r5, r6, r7, pc}
_021E66B0:
	ldr r1, [r2, #0x34]
	ldrb r1, [r1]
	lsl r1, r1, #0x19
	lsr r1, r1, #0x1f
	beq _021E66E0
	mov r0, #0
	add r1, r0, #0
	mov r2, #0x9c
	mov r3, #1
	bl ov112_021E5A68
	pop {r3, r4, r5, r6, r7, pc}
_021E66C8:
	ldr r1, [r2, #0x34]
	ldrb r1, [r1]
	lsl r1, r1, #0x18
	lsr r1, r1, #0x1f
	beq _021E66E0
	mov r0, #0
	add r1, r0, #0
	mov r2, #0x9c
	mov r3, #1
	bl ov112_021E5A68
	pop {r3, r4, r5, r6, r7, pc}
_021E66E0:
	ldr r1, _021E6828 ; =ov112_021FFAA4
	mov r5, #0xd
	ldr r4, [r1, #0x34]
	ldrb r2, [r4]
	lsl r3, r2, #0x1c
	lsr r3, r3, #0x1c
	and r5, r3
	cmp r5, #0xd
	bne _021E677E
	mov r0, #0xf
	bic r2, r0
	mov r0, #2
	orr r0, r3
	lsl r0, r0, #0x18
	lsr r3, r0, #0x18
	mov r0, #0xf
	and r0, r3
	orr r0, r2
	strb r0, [r4]
	ldr r0, [r1, #4]
	cmp r0, #3
	bhi _021E6778
	add r0, r0, r0
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_021E6718: ; jump table
	.short _021E6720 - _021E6718 - 2 ; case 0
	.short _021E6736 - _021E6718 - 2 ; case 1
	.short _021E674E - _021E6718 - 2 ; case 2
	.short _021E6766 - _021E6718 - 2 ; case 3
_021E6720:
	ldr r0, _021E682C ; =ov112_021FFB24
	mov r2, #0xb0
	strb r2, [r0, #0x1d]
	ldr r0, [r1, #0x34]
	mov r2, #9
	ldr r1, _021E6830 ; =0x0000B804
	add r0, r0, #4
	lsl r2, r2, #6
	bl ov112_021E5EC4
	b _021E6778
_021E6736:
	ldr r0, _021E682C ; =ov112_021FFB24
	mov r2, #0xb2
	strb r2, [r0, #0x1d]
	ldr r0, [r1, #0x34]
	mov r2, #0x91
	lsl r2, r2, #2
	add r0, r0, r2
	ldr r1, _021E6834 ; =0x0000BA44
	add r2, #0xb8
	bl ov112_021E5EC4
	b _021E6778
_021E674E:
	ldr r0, _021E682C ; =ov112_021FFB24
	mov r2, #0xb4
	strb r2, [r0, #0x1d]
	mov r0, #0x15
	ldr r1, [r1, #0x34]
	lsl r0, r0, #6
	add r0, r1, r0
	ldr r1, _021E6838 ; =0x0000BD40
	add r2, #0xd4
	bl ov112_021E5EC4
	b _021E6778
_021E6766:
	ldr r0, _021E682C ; =ov112_021FFB24
	mov r2, #0xb6
	strb r2, [r0, #0x1d]
	ldr r0, [r1, #0xc]
	mov r1, #0xbf
	ldr r2, _021E6824 ; =0x00000CBC
	lsl r1, r1, #8
	bl ov112_021E5EC4
_021E6778:
	bl ov112_021E5EEC
	pop {r3, r4, r5, r6, r7, pc}
_021E677E:
	mov r1, #2
	tst r1, r3
	beq _021E6792
	mov r0, #0
	add r1, r0, #0
	mov r2, #0x9e
	mov r3, #1
	bl ov112_021E5A68
	pop {r3, r4, r5, r6, r7, pc}
_021E6792:
	ldr r1, _021E682C ; =ov112_021FFB24
	mov r2, #0x24
	strb r0, [r1, #0x1d]
	mov r0, #0
	add r1, r0, #0
	mov r3, #1
	bl ov112_021E5A68
	pop {r3, r4, r5, r6, r7, pc}
_021E67A4:
	ldr r2, _021E6828 ; =ov112_021FFAA4
	ldr r1, [r2, #4]
	cmp r1, #3
	bhi _021E6820
	add r1, r1, r1
	add r1, pc
	ldrh r1, [r1, #6]
	lsl r1, r1, #0x10
	asr r1, r1, #0x10
	add pc, r1
_021E67B8: ; jump table
	.short _021E67C0 - _021E67B8 - 2 ; case 0
	.short _021E67D8 - _021E67B8 - 2 ; case 1
	.short _021E67F0 - _021E67B8 - 2 ; case 2
	.short _021E6808 - _021E67B8 - 2 ; case 3
_021E67C0:
	ldr r1, [r2, #0x34]
	ldrb r1, [r1]
	lsl r1, r1, #0x1b
	lsr r1, r1, #0x1f
	beq _021E6820
	mov r0, #0
	add r1, r0, #0
	mov r2, #0x9c
	mov r3, #1
	bl ov112_021E5A68
	pop {r3, r4, r5, r6, r7, pc}
_021E67D8:
	ldr r1, [r2, #0x34]
	ldrb r1, [r1]
	lsl r1, r1, #0x1a
	lsr r1, r1, #0x1f
	beq _021E6820
	mov r0, #0
	add r1, r0, #0
	mov r2, #0x9c
	mov r3, #1
	bl ov112_021E5A68
	pop {r3, r4, r5, r6, r7, pc}
_021E67F0:
	ldr r1, [r2, #0x34]
	ldrb r1, [r1]
	lsl r1, r1, #0x19
	lsr r1, r1, #0x1f
	beq _021E6820
	mov r0, #0
	add r1, r0, #0
	mov r2, #0x9c
	mov r3, #1
	bl ov112_021E5A68
	pop {r3, r4, r5, r6, r7, pc}
_021E6808:
	ldr r1, [r2, #0x34]
	ldrb r1, [r1]
	lsl r1, r1, #0x18
	lsr r1, r1, #0x1f
	beq _021E6820
	mov r0, #0
	add r1, r0, #0
	mov r2, #0x9c
	mov r3, #1
	bl ov112_021E5A68
	pop {r3, r4, r5, r6, r7, pc}
_021E6820:
	ldr r1, _021E6828 ; =ov112_021FFAA4
	b _021E683C
	.balign 4, 0
_021E6824: .word 0x00000CBC
_021E6828: .word ov112_021FFAA4
_021E682C: .word ov112_021FFB24
_021E6830: .word 0x0000B804
_021E6834: .word 0x0000BA44
_021E6838: .word 0x0000BD40
_021E683C:
	mov r5, #0xb
	ldr r4, [r1, #0x34]
	ldrb r2, [r4]
	lsl r3, r2, #0x1c
	lsr r3, r3, #0x1c
	and r5, r3
	cmp r5, #0xb
	bne _021E68D8
	mov r0, #0xf
	bic r2, r0
	mov r0, #4
	orr r0, r3
	lsl r0, r0, #0x18
	lsr r3, r0, #0x18
	mov r0, #0xf
	and r0, r3
	orr r0, r2
	strb r0, [r4]
	ldr r0, [r1, #4]
	cmp r0, #3
	bhi _021E68D2
	add r0, r0, r0
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_021E6872: ; jump table
	.short _021E687A - _021E6872 - 2 ; case 0
	.short _021E6890 - _021E6872 - 2 ; case 1
	.short _021E68A8 - _021E6872 - 2 ; case 2
	.short _021E68C0 - _021E6872 - 2 ; case 3
_021E687A:
	ldr r0, _021E6A54 ; =ov112_021FFB24
	mov r2, #0xb0
	strb r2, [r0, #0x1d]
	ldr r0, [r1, #0x34]
	mov r2, #9
	ldr r1, _021E6A58 ; =0x0000B804
	add r0, r0, #4
	lsl r2, r2, #6
	bl ov112_021E5EC4
	b _021E68D2
_021E6890:
	ldr r0, _021E6A54 ; =ov112_021FFB24
	mov r2, #0xb2
	strb r2, [r0, #0x1d]
	ldr r0, [r1, #0x34]
	mov r2, #0x91
	lsl r2, r2, #2
	add r0, r0, r2
	ldr r1, _021E6A5C ; =0x0000BA44
	add r2, #0xb8
	bl ov112_021E5EC4
	b _021E68D2
_021E68A8:
	ldr r0, _021E6A54 ; =ov112_021FFB24
	mov r2, #0xb4
	strb r2, [r0, #0x1d]
	mov r0, #0x15
	ldr r1, [r1, #0x34]
	lsl r0, r0, #6
	add r0, r1, r0
	ldr r1, _021E6A60 ; =0x0000BD40
	add r2, #0xd4
	bl ov112_021E5EC4
	b _021E68D2
_021E68C0:
	ldr r0, _021E6A54 ; =ov112_021FFB24
	mov r2, #0xb6
	strb r2, [r0, #0x1d]
	ldr r0, [r1, #0xc]
	mov r1, #0xbf
	ldr r2, _021E6A64 ; =0x00000CBC
	lsl r1, r1, #8
	bl ov112_021E5EC4
_021E68D2:
	bl ov112_021E5EEC
	pop {r3, r4, r5, r6, r7, pc}
_021E68D8:
	mov r1, #4
	tst r1, r3
	beq _021E68EC
	mov r0, #0
	add r1, r0, #0
	mov r2, #0x9e
	mov r3, #1
	bl ov112_021E5A68
	pop {r3, r4, r5, r6, r7, pc}
_021E68EC:
	ldr r1, _021E6A54 ; =ov112_021FFB24
	mov r2, #0x24
	strb r0, [r1, #0x1d]
	mov r0, #0
	add r1, r0, #0
	mov r3, #1
	bl ov112_021E5A68
	pop {r3, r4, r5, r6, r7, pc}
_021E68FE:
	ldr r2, _021E6A68 ; =ov112_021FFAA4
	ldr r1, [r2, #4]
	cmp r1, #3
	bhi _021E697A
	add r1, r1, r1
	add r1, pc
	ldrh r1, [r1, #6]
	lsl r1, r1, #0x10
	asr r1, r1, #0x10
	add pc, r1
_021E6912: ; jump table
	.short _021E691A - _021E6912 - 2 ; case 0
	.short _021E6932 - _021E6912 - 2 ; case 1
	.short _021E694A - _021E6912 - 2 ; case 2
	.short _021E6962 - _021E6912 - 2 ; case 3
_021E691A:
	ldr r1, [r2, #0x34]
	ldrb r1, [r1]
	lsl r1, r1, #0x1b
	lsr r1, r1, #0x1f
	beq _021E697A
	mov r0, #0
	add r1, r0, #0
	mov r2, #0x9c
	mov r3, #1
	bl ov112_021E5A68
	pop {r3, r4, r5, r6, r7, pc}
_021E6932:
	ldr r1, [r2, #0x34]
	ldrb r1, [r1]
	lsl r1, r1, #0x1a
	lsr r1, r1, #0x1f
	beq _021E697A
	mov r0, #0
	add r1, r0, #0
	mov r2, #0x9c
	mov r3, #1
	bl ov112_021E5A68
	pop {r3, r4, r5, r6, r7, pc}
_021E694A:
	ldr r1, [r2, #0x34]
	ldrb r1, [r1]
	lsl r1, r1, #0x19
	lsr r1, r1, #0x1f
	beq _021E697A
	mov r0, #0
	add r1, r0, #0
	mov r2, #0x9c
	mov r3, #1
	bl ov112_021E5A68
	pop {r3, r4, r5, r6, r7, pc}
_021E6962:
	ldr r1, [r2, #0x34]
	ldrb r1, [r1]
	lsl r1, r1, #0x18
	lsr r1, r1, #0x1f
	beq _021E697A
	mov r0, #0
	add r1, r0, #0
	mov r2, #0x9c
	mov r3, #1
	bl ov112_021E5A68
	pop {r3, r4, r5, r6, r7, pc}
_021E697A:
	ldr r1, _021E6A68 ; =ov112_021FFAA4
	mov r5, #7
	ldr r4, [r1, #0x34]
	ldrb r2, [r4]
	lsl r3, r2, #0x1c
	lsr r3, r3, #0x1c
	and r5, r3
	cmp r5, #7
	bne _021E6A18
	mov r0, #0xf
	bic r2, r0
	mov r0, #8
	orr r0, r3
	lsl r0, r0, #0x18
	lsr r3, r0, #0x18
	mov r0, #0xf
	and r0, r3
	orr r0, r2
	strb r0, [r4]
	ldr r0, [r1, #4]
	cmp r0, #3
	bhi _021E6A12
	add r0, r0, r0
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_021E69B2: ; jump table
	.short _021E69BA - _021E69B2 - 2 ; case 0
	.short _021E69D0 - _021E69B2 - 2 ; case 1
	.short _021E69E8 - _021E69B2 - 2 ; case 2
	.short _021E6A00 - _021E69B2 - 2 ; case 3
_021E69BA:
	ldr r0, _021E6A54 ; =ov112_021FFB24
	mov r2, #0xb0
	strb r2, [r0, #0x1d]
	ldr r0, [r1, #0x34]
	mov r2, #9
	ldr r1, _021E6A58 ; =0x0000B804
	add r0, r0, #4
	lsl r2, r2, #6
	bl ov112_021E5EC4
	b _021E6A12
_021E69D0:
	ldr r0, _021E6A54 ; =ov112_021FFB24
	mov r2, #0xb2
	strb r2, [r0, #0x1d]
	ldr r0, [r1, #0x34]
	mov r2, #0x91
	lsl r2, r2, #2
	add r0, r0, r2
	ldr r1, _021E6A5C ; =0x0000BA44
	add r2, #0xb8
	bl ov112_021E5EC4
	b _021E6A12
_021E69E8:
	ldr r0, _021E6A54 ; =ov112_021FFB24
	mov r2, #0xb4
	strb r2, [r0, #0x1d]
	mov r0, #0x15
	ldr r1, [r1, #0x34]
	lsl r0, r0, #6
	add r0, r1, r0
	ldr r1, _021E6A60 ; =0x0000BD40
	add r2, #0xd4
	bl ov112_021E5EC4
	b _021E6A12
_021E6A00:
	ldr r0, _021E6A54 ; =ov112_021FFB24
	mov r2, #0xb6
	strb r2, [r0, #0x1d]
	ldr r0, [r1, #0xc]
	mov r1, #0xbf
	ldr r2, _021E6A64 ; =0x00000CBC
	lsl r1, r1, #8
	bl ov112_021E5EC4
_021E6A12:
	bl ov112_021E5EEC
	pop {r3, r4, r5, r6, r7, pc}
_021E6A18:
	mov r1, #8
	tst r1, r3
	beq _021E6A2C
	mov r0, #0
	add r1, r0, #0
	mov r2, #0x9e
	mov r3, #1
	bl ov112_021E5A68
	pop {r3, r4, r5, r6, r7, pc}
_021E6A2C:
	ldr r1, _021E6A54 ; =ov112_021FFB24
	mov r2, #0x24
	strb r0, [r1, #0x1d]
	mov r0, #0
	add r1, r0, #0
	mov r3, #1
	bl ov112_021E5A68
	pop {r3, r4, r5, r6, r7, pc}
_021E6A3E:
	bl OS_Halt
	pop {r3, r4, r5, r6, r7, pc}
_021E6A44:
	mov r0, #0
	add r1, r0, #0
	mov r2, #0x24
	mov r3, #1
	bl ov112_021E5A68
_021E6A50:
	pop {r3, r4, r5, r6, r7, pc}
	nop
_021E6A54: .word ov112_021FFB24
_021E6A58: .word 0x0000B804
_021E6A5C: .word 0x0000BA44
_021E6A60: .word 0x0000BD40
_021E6A64: .word 0x00000CBC
_021E6A68: .word ov112_021FFAA4
	thumb_func_end ov112_021E6164

	thumb_func_start ov112_021E6A6C
ov112_021E6A6C: ; 0x021E6A6C
	push {r3, lr}
	ldr r1, _021E6BD0 ; =ov112_021FFAA4
	mov r2, #0
	str r2, [r1, #0x14]
	cmp r0, #0
	bne _021E6A84
	mov r0, #0xc
	bl ov112_021E5E18
	bl ov112_021E5A5C
	pop {r3, pc}
_021E6A84:
	cmp r0, #2
	bls _021E6A94
	mov r0, #0xc
	bl ov112_021E5E18
	bl ov112_021E5A5C
	pop {r3, pc}
_021E6A94:
	bl OS_GetTick
	ldr r2, _021E6BD0 ; =ov112_021FFAA4
	str r0, [r2, #0x3c]
	ldr r0, _021E6BD4 ; =ov112_021FFB24
	str r1, [r2, #0x40]
	ldrb r0, [r0, #0x1c]
	cmp r0, #0x60
	bgt _021E6AC4
	blt _021E6AAA
	b _021E6BBC
_021E6AAA:
	cmp r0, #0x32
	bgt _021E6AB6
	bge _021E6BAA
	cmp r0, #0x2a
	beq _021E6B88
	pop {r3, pc}
_021E6AB6:
	cmp r0, #0x40
	bgt _021E6ABE
	beq _021E6BAA
	pop {r3, pc}
_021E6ABE:
	cmp r0, #0x52
	beq _021E6BAA
	pop {r3, pc}
_021E6AC4:
	cmp r0, #0xe2
	bgt _021E6B04
	bge _021E6B4A
	cmp r0, #0xbe
	bgt _021E6AFE
	add r1, r0, #0
	sub r1, #0xb0
	bmi _021E6BCE
	add r1, r1, r1
	add r1, pc
	ldrh r1, [r1, #6]
	lsl r1, r1, #0x10
	asr r1, r1, #0x10
	add pc, r1
_021E6AE0: ; jump table
	.short _021E6BBC - _021E6AE0 - 2 ; case 0
	.short _021E6BCE - _021E6AE0 - 2 ; case 1
	.short _021E6BBC - _021E6AE0 - 2 ; case 2
	.short _021E6BCE - _021E6AE0 - 2 ; case 3
	.short _021E6BBC - _021E6AE0 - 2 ; case 4
	.short _021E6BCE - _021E6AE0 - 2 ; case 5
	.short _021E6BBC - _021E6AE0 - 2 ; case 6
	.short _021E6BCE - _021E6AE0 - 2 ; case 7
	.short _021E6BBC - _021E6AE0 - 2 ; case 8
	.short _021E6BCE - _021E6AE0 - 2 ; case 9
	.short _021E6BBC - _021E6AE0 - 2 ; case 10
	.short _021E6BCE - _021E6AE0 - 2 ; case 11
	.short _021E6BBC - _021E6AE0 - 2 ; case 12
	.short _021E6BCE - _021E6AE0 - 2 ; case 13
	.short _021E6BBC - _021E6AE0 - 2 ; case 14
_021E6AFE:
	cmp r0, #0xe0
	beq _021E6B70
	pop {r3, pc}
_021E6B04:
	cmp r0, #0xf0
	bgt _021E6B0C
	beq _021E6B12
	pop {r3, pc}
_021E6B0C:
	cmp r0, #0xfe
	beq _021E6B34
	pop {r3, pc}
_021E6B12:
	bl ov112_021E5E28
	ldr r0, _021E6BD0 ; =ov112_021FFAA4
	ldr r1, _021E6BD8 ; =ov112_021FFB64
	ldr r0, [r0, #0x20]
	mov r2, #0x28
	add r0, #0x10
	bl MI_CpuCopy8
	ldr r2, _021E6BD4 ; =ov112_021FFB24
	ldr r0, _021E6BD8 ; =ov112_021FFB64
	ldrb r2, [r2, #0x1c]
	mov r1, #0x74
	mov r3, #1
	bl ov112_021E5A68
	pop {r3, pc}
_021E6B34:
	bl ov112_021E5E28
	ldr r2, _021E6BD4 ; =ov112_021FFB24
	ldr r0, _021E6BD0 ; =ov112_021FFAA4
	ldrb r2, [r2, #0x1c]
	ldr r0, [r0, #0x28]
	mov r1, #8
	mov r3, #1
	bl ov112_021E5A68
	pop {r3, pc}
_021E6B4A:
	mov r1, #0x80
	add r0, sp, #0
	strb r1, [r0]
	ldrh r2, [r2]
	mov r3, #0
	asr r1, r2, #8
	strb r1, [r0, #1]
	strb r2, [r0, #2]
	ldrb r1, [r0, #1]
	add r2, r1, #1
	ldrb r1, [r0, #2]
	add r1, r2, r1
	strb r1, [r0, #3]
	add r0, sp, #0
	mov r1, #4
	mov r2, #0xa
	bl ov112_021E5A68
	pop {r3, pc}
_021E6B70:
	bl ov112_021E5E28
	ldr r0, _021E6BD0 ; =ov112_021FFAA4
	ldr r2, _021E6BD4 ; =ov112_021FFB24
	ldr r0, [r0, #0x20]
	ldrb r2, [r2, #0x1c]
	add r0, #0x10
	mov r1, #0x28
	mov r3, #1
	bl ov112_021E5A68
	pop {r3, pc}
_021E6B88:
	bl ov112_021E5E28
	ldr r0, _021E6BD0 ; =ov112_021FFAA4
	ldr r2, _021E6BD4 ; =ov112_021FFB24
	ldr r0, [r0, #0x20]
	ldrb r2, [r2, #0x1d]
	add r0, #0x10
	mov r1, #0x28
	mov r3, #1
	bl ov112_021E5A68
	mov r0, #0xf
	bl ov112_021E5E18
	bl ov112_021E5A5C
	pop {r3, pc}
_021E6BAA:
	bl ov112_021E5E48
	mov r0, #0
	add r1, r0, #0
	mov r2, #0x20
	mov r3, #1
	bl ov112_021E5A68
	pop {r3, pc}
_021E6BBC:
	ldr r1, _021E6BD0 ; =ov112_021FFAA4
	mov r0, #0
	ldr r1, [r1, #0x20]
	mov r2, #0x20
	str r0, [r1, #0x60]
	add r1, r0, #0
	mov r3, #1
	bl ov112_021E5A68
_021E6BCE:
	pop {r3, pc}
	.balign 4, 0
_021E6BD0: .word ov112_021FFAA4
_021E6BD4: .word ov112_021FFB24
_021E6BD8: .word ov112_021FFB64
	thumb_func_end ov112_021E6A6C

	thumb_func_start ov112_021E6BDC
ov112_021E6BDC: ; 0x021E6BDC
	ldr r0, _021E6BF4 ; =ov112_021FFB24
	ldr r1, [r0, #0x24]
	str r1, [r0, #0x20]
	ldr r1, [r0, #0x2c]
	str r1, [r0, #0x28]
	ldrh r1, [r0, #0x32]
	strh r1, [r0, #0x30]
	mov r1, #0
	strh r1, [r0, #0x36]
	strh r1, [r0, #0x3a]
	bx lr
	nop
_021E6BF4: .word ov112_021FFB24
	thumb_func_end ov112_021E6BDC

	thumb_func_start ov112_021E6BF8
ov112_021E6BF8: ; 0x021E6BF8
	push {r4, lr}
	ldr r0, _021E6EA8 ; =ov112_021FFB24
	ldrb r2, [r0, #0x1c]
	cmp r2, #0x52
	bgt _021E6C14
	blt _021E6C06
	b _021E6D6C
_021E6C06:
	cmp r2, #0x32
	bgt _021E6C0E
	beq _021E6C52
	b _021E6F52
_021E6C0E:
	cmp r2, #0x40
	beq _021E6C8E
	b _021E6F52
_021E6C14:
	cmp r2, #0x60
	bgt _021E6C1E
	bne _021E6C1C
	b _021E6E4C
_021E6C1C:
	b _021E6F52
_021E6C1E:
	add r0, r2, #0
	sub r0, #0xb0
	cmp r0, #0xe
	bls _021E6C28
	b _021E6F52
_021E6C28:
	add r0, r0, r0
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_021E6C34: ; jump table
	.short _021E6EA4 - _021E6C34 - 2 ; case 0
	.short _021E6F52 - _021E6C34 - 2 ; case 1
	.short _021E6EA4 - _021E6C34 - 2 ; case 2
	.short _021E6F52 - _021E6C34 - 2 ; case 3
	.short _021E6EA4 - _021E6C34 - 2 ; case 4
	.short _021E6F52 - _021E6C34 - 2 ; case 5
	.short _021E6EA4 - _021E6C34 - 2 ; case 6
	.short _021E6F52 - _021E6C34 - 2 ; case 7
	.short _021E6EA4 - _021E6C34 - 2 ; case 8
	.short _021E6F52 - _021E6C34 - 2 ; case 9
	.short _021E6EA4 - _021E6C34 - 2 ; case 10
	.short _021E6F52 - _021E6C34 - 2 ; case 11
	.short _021E6EA4 - _021E6C34 - 2 ; case 12
	.short _021E6F52 - _021E6C34 - 2 ; case 13
	.short _021E6EA4 - _021E6C34 - 2 ; case 14
_021E6C52:
	ldr r0, _021E6EAC ; =ov112_021FFAA4
	ldr r1, [r0, #0x18]
	cmp r1, #0
	beq _021E6C7E
	ldr r0, [r0, #0x24]
	add r0, #0x5b
	ldrb r0, [r0]
	lsl r0, r0, #0x1f
	lsr r0, r0, #0x1f
	beq _021E6C7E
	mov r0, #0
	add r1, r0, #0
	mov r2, #0x36
	mov r3, #1
	bl ov112_021E5A68
	mov r0, #4
	bl ov112_021E5E18
	bl ov112_021E5A5C
	pop {r4, pc}
_021E6C7E:
	ldr r0, _021E6EAC ; =ov112_021FFAA4
	mov r1, #0x68
	ldr r0, [r0, #0x20]
	mov r2, #0x32
	mov r3, #1
	bl ov112_021E5A68
	pop {r4, pc}
_021E6C8E:
	ldr r2, _021E6EAC ; =ov112_021FFAA4
	ldr r1, [r2, #0x24]
	add r0, r1, #0
	add r0, #0x5b
	ldrb r0, [r0]
	lsl r3, r0, #0x1f
	lsr r3, r3, #0x1f
	bne _021E6CB6
	mov r0, #0
	add r1, r0, #0
	mov r2, #0x44
	mov r3, #1
	bl ov112_021E5A68
	mov r0, #5
	bl ov112_021E5E18
	bl ov112_021E5A5C
	pop {r4, pc}
_021E6CB6:
	lsl r0, r0, #0x1e
	lsr r0, r0, #0x1f
	bne _021E6CD4
	mov r0, #0
	add r1, r0, #0
	mov r2, #0x44
	mov r3, #1
	bl ov112_021E5A68
	mov r0, #6
	bl ov112_021E5E18
	bl ov112_021E5A5C
	pop {r4, pc}
_021E6CD4:
	ldr r0, [r2, #0x20]
	ldr r3, [r1, #0xc]
	ldr r2, [r0, #0xc]
	cmp r3, r2
	beq _021E6CF6
	mov r0, #0
	add r1, r0, #0
	mov r2, #0x44
	mov r3, #1
	bl ov112_021E5A68
	mov r0, #0xb
	bl ov112_021E5E18
	bl ov112_021E5A5C
	pop {r4, pc}
_021E6CF6:
	ldr r3, [r1, #4]
	ldr r2, [r0, #4]
	cmp r3, r2
	beq _021E6D16
	mov r0, #0
	add r1, r0, #0
	mov r2, #0x44
	mov r3, #1
	bl ov112_021E5A68
	mov r0, #8
	bl ov112_021E5E18
	bl ov112_021E5A5C
	pop {r4, pc}
_021E6D16:
	ldrh r2, [r1, #0xa]
	lsr r3, r2, #2
	ldrh r2, [r0, #0xa]
	lsr r2, r2, #2
	cmp r3, r2
	beq _021E6D3A
	mov r0, #0
	add r1, r0, #0
	mov r2, #0x44
	mov r3, #1
	bl ov112_021E5A68
	mov r0, #9
	bl ov112_021E5E18
	bl ov112_021E5A5C
	pop {r4, pc}
_021E6D3A:
	add r1, #0x5d
	ldrb r2, [r1]
	add r1, r0, #0
	add r1, #0x5d
	ldrb r1, [r1]
	cmp r2, r1
	bls _021E6D60
	mov r0, #0
	add r1, r0, #0
	mov r2, #0x44
	mov r3, #1
	bl ov112_021E5A68
	mov r0, #0xa
	bl ov112_021E5E18
	bl ov112_021E5A5C
	pop {r4, pc}
_021E6D60:
	mov r1, #0x68
	mov r2, #0x40
	mov r3, #1
	bl ov112_021E5A68
	pop {r4, pc}
_021E6D6C:
	ldr r2, _021E6EAC ; =ov112_021FFAA4
	ldr r1, [r2, #0x24]
	add r0, r1, #0
	add r0, #0x5b
	ldrb r0, [r0]
	lsl r3, r0, #0x1f
	lsr r3, r3, #0x1f
	bne _021E6D94
	mov r0, #0
	add r1, r0, #0
	mov r2, #0x56
	mov r3, #1
	bl ov112_021E5A68
	mov r0, #5
	bl ov112_021E5E18
	bl ov112_021E5A5C
	pop {r4, pc}
_021E6D94:
	lsl r0, r0, #0x1e
	lsr r0, r0, #0x1f
	cmp r0, #1
	bne _021E6DB4
	mov r0, #0
	add r1, r0, #0
	mov r2, #0x56
	mov r3, #1
	bl ov112_021E5A68
	mov r0, #7
	bl ov112_021E5E18
	bl ov112_021E5A5C
	pop {r4, pc}
_021E6DB4:
	ldr r0, [r2, #0x20]
	ldr r3, [r1]
	ldr r2, [r0]
	cmp r3, r2
	beq _021E6DD6
	mov r0, #0
	add r1, r0, #0
	mov r2, #0x56
	mov r3, #1
	bl ov112_021E5A68
	mov r0, #8
	bl ov112_021E5E18
	bl ov112_021E5A5C
	pop {r4, pc}
_021E6DD6:
	ldrh r2, [r1, #8]
	lsr r3, r2, #2
	ldrh r2, [r0, #8]
	lsr r2, r2, #2
	cmp r3, r2
	beq _021E6DFA
	mov r0, #0
	add r1, r0, #0
	mov r2, #0x56
	mov r3, #1
	bl ov112_021E5A68
	mov r0, #9
	bl ov112_021E5E18
	bl ov112_021E5A5C
	pop {r4, pc}
_021E6DFA:
	ldr r3, [r1, #0xc]
	ldr r2, [r0, #0xc]
	cmp r3, r2
	beq _021E6E1A
	mov r0, #0
	add r1, r0, #0
	mov r2, #0x56
	mov r3, #1
	bl ov112_021E5A68
	mov r0, #0xb
	bl ov112_021E5E18
	bl ov112_021E5A5C
	pop {r4, pc}
_021E6E1A:
	add r1, #0x5d
	ldrb r2, [r1]
	add r1, r0, #0
	add r1, #0x5d
	ldrb r1, [r1]
	cmp r2, r1
	bls _021E6E40
	mov r0, #0
	add r1, r0, #0
	mov r2, #0x56
	mov r3, #1
	bl ov112_021E5A68
	mov r0, #0xa
	bl ov112_021E5E18
	bl ov112_021E5A5C
	pop {r4, pc}
_021E6E40:
	mov r1, #0x68
	mov r2, #0x52
	mov r3, #1
	bl ov112_021E5A68
	pop {r4, pc}
_021E6E4C:
	ldr r1, _021E6EAC ; =ov112_021FFAA4
	ldr r2, [r1, #0x24]
	add r0, r2, #0
	add r0, #0x5b
	ldrb r0, [r0]
	lsl r0, r0, #0x1f
	lsr r0, r0, #0x1f
	ldr r0, [r1, #0x20]
	bne _021E6E74
	mov r1, #0x68
	mov r2, #0x64
	mov r3, #1
	bl ov112_021E5A68
	mov r0, #5
	bl ov112_021E5E18
	bl ov112_021E5A5C
	pop {r4, pc}
_021E6E74:
	add r1, r0, #0
	add r2, #0x5d
	add r1, #0x5d
	ldrb r2, [r2]
	ldrb r1, [r1]
	cmp r2, r1
	bls _021E6E98
	mov r1, #0x68
	mov r2, #0x64
	mov r3, #1
	bl ov112_021E5A68
	mov r0, #0xa
	bl ov112_021E5E18
	bl ov112_021E5A5C
	pop {r4, pc}
_021E6E98:
	mov r1, #0x68
	mov r2, #0x60
	mov r3, #1
	bl ov112_021E5A68
	pop {r4, pc}
_021E6EA4:
	ldr r3, _021E6EAC ; =ov112_021FFAA4
	b _021E6EB0
	.balign 4, 0
_021E6EA8: .word ov112_021FFB24
_021E6EAC: .word ov112_021FFAA4
_021E6EB0:
	ldr r1, [r3, #0x24]
	add r0, r1, #0
	add r0, #0x5b
	ldrb r0, [r0]
	lsl r0, r0, #0x1f
	lsr r0, r0, #0x1f
	bne _021E6ED6
	mov r0, #0
	add r1, r0, #0
	mov r2, #0xd8
	mov r3, #1
	bl ov112_021E5A68
	mov r0, #5
	bl ov112_021E5E18
	bl ov112_021E5A5C
	pop {r4, pc}
_021E6ED6:
	ldr r0, [r3, #0x20]
	ldr r4, [r1]
	ldr r3, [r0]
	cmp r4, r3
	beq _021E6EF8
	mov r0, #0
	add r1, r0, #0
	mov r2, #0xd8
	mov r3, #1
	bl ov112_021E5A68
	mov r0, #8
	bl ov112_021E5E18
	bl ov112_021E5A5C
	pop {r4, pc}
_021E6EF8:
	ldrh r3, [r1, #8]
	lsr r4, r3, #2
	ldrh r3, [r0, #8]
	lsr r3, r3, #2
	cmp r4, r3
	beq _021E6F1C
	mov r0, #0
	add r1, r0, #0
	mov r2, #0xd8
	mov r3, #1
	bl ov112_021E5A68
	mov r0, #9
	bl ov112_021E5E18
	bl ov112_021E5A5C
	pop {r4, pc}
_021E6F1C:
	add r1, #0x5d
	ldrb r3, [r1]
	add r1, r0, #0
	add r1, #0x5d
	ldrb r1, [r1]
	cmp r3, r1
	bls _021E6F42
	mov r0, #0
	add r1, r0, #0
	mov r2, #0xd8
	mov r3, #1
	bl ov112_021E5A68
	mov r0, #0xa
	bl ov112_021E5E18
	bl ov112_021E5A5C
	pop {r4, pc}
_021E6F42:
	sub r2, #0x10
	lsl r2, r2, #0x18
	mov r1, #0x68
	lsr r2, r2, #0x18
	mov r3, #1
	bl ov112_021E5A68
	pop {r4, pc}
_021E6F52:
	mov r0, #0xc
	bl ov112_021E5E18
	bl ov112_021E5A5C
	pop {r4, pc}
	.balign 4, 0
	thumb_func_end ov112_021E6BF8

	thumb_func_start ov112_021E6F60
ov112_021E6F60: ; 0x021E6F60
	push {r4, r5, r6, lr}
	add r4, r1, #0
	cmp r2, #0x9e
	bgt _021E6FE8
	blt _021E6F6C
	b _021E71F4
_021E6F6C:
	cmp r2, #0x44
	bgt _021E6FB6
	blt _021E6F74
	b _021E7268
_021E6F74:
	cmp r2, #0x28
	bgt _021E6F9E
	cmp r2, #0x22
	blt _021E6F8C
	beq _021E7076
	cmp r2, #0x26
	bne _021E6F84
	b _021E70B0
_021E6F84:
	cmp r2, #0x28
	bne _021E6F8A
	b _021E720C
_021E6F8A:
	pop {r4, r5, r6, pc}
_021E6F8C:
	cmp r2, #4
	bgt _021E6F96
	bne _021E6F94
	b _021E732E
_021E6F94:
	pop {r4, r5, r6, pc}
_021E6F96:
	cmp r2, #0xe
	bne _021E6F9C
	b _021E7342
_021E6F9C:
	pop {r4, r5, r6, pc}
_021E6F9E:
	cmp r2, #0x38
	bgt _021E6FAE
	blt _021E6FA6
	b _021E7242
_021E6FA6:
	cmp r2, #0x34
	bne _021E6FAC
	b _021E7218
_021E6FAC:
	pop {r4, r5, r6, pc}
_021E6FAE:
	cmp r2, #0x42
	bne _021E6FB4
	b _021E724E
_021E6FB4:
	pop {r4, r5, r6, pc}
_021E6FB6:
	cmp r2, #0x5e
	bgt _021E6FD0
	blt _021E6FBE
	b _021E72AA
_021E6FBE:
	cmp r2, #0x54
	bgt _021E6FC8
	bne _021E6FC6
	b _021E7274
_021E6FC6:
	pop {r4, r5, r6, pc}
_021E6FC8:
	cmp r2, #0x5a
	bne _021E6FCE
	b _021E729E
_021E6FCE:
	pop {r4, r5, r6, pc}
_021E6FD0:
	cmp r2, #0x64
	bgt _021E6FE0
	blt _021E6FD8
	b _021E72CE
_021E6FD8:
	cmp r2, #0x62
	bne _021E6FDE
	b _021E72B6
_021E6FDE:
	pop {r4, r5, r6, pc}
_021E6FE0:
	cmp r2, #0x9c
	bne _021E6FE6
	b _021E7200
_021E6FE6:
	pop {r4, r5, r6, pc}
_021E6FE8:
	cmp r2, #0xca
	bgt _021E7040
	bge _021E7052
	cmp r2, #0xc2
	bgt _021E702E
	bge _021E7052
	cmp r2, #0xae
	bgt _021E7028
	add r0, r2, #0
	sub r0, #0xa0
	bmi _021E7064
	add r0, r0, r0
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_021E700A: ; jump table
	.short _021E72DA - _021E700A - 2 ; case 0
	.short _021E737C - _021E700A - 2 ; case 1
	.short _021E72DA - _021E700A - 2 ; case 2
	.short _021E737C - _021E700A - 2 ; case 3
	.short _021E72DA - _021E700A - 2 ; case 4
	.short _021E737C - _021E700A - 2 ; case 5
	.short _021E72DA - _021E700A - 2 ; case 6
	.short _021E737C - _021E700A - 2 ; case 7
	.short _021E72DA - _021E700A - 2 ; case 8
	.short _021E737C - _021E700A - 2 ; case 9
	.short _021E72DA - _021E700A - 2 ; case 10
	.short _021E737C - _021E700A - 2 ; case 11
	.short _021E72DA - _021E700A - 2 ; case 12
	.short _021E737C - _021E700A - 2 ; case 13
	.short _021E72DA - _021E700A - 2 ; case 14
_021E7028:
	cmp r2, #0xc0
	beq _021E7052
	pop {r4, r5, r6, pc}
_021E702E:
	cmp r2, #0xc6
	bgt _021E703A
	bge _021E7052
	cmp r2, #0xc4
	beq _021E7052
	pop {r4, r5, r6, pc}
_021E703A:
	cmp r2, #0xc8
	beq _021E7052
	pop {r4, r5, r6, pc}
_021E7040:
	cmp r2, #0xe2
	bgt _021E705E
	blt _021E7048
	b _021E7316
_021E7048:
	cmp r2, #0xce
	bgt _021E7056
	bge _021E7052
	cmp r2, #0xcc
	bne _021E7054
_021E7052:
	b _021E72F2
_021E7054:
	pop {r4, r5, r6, pc}
_021E7056:
	cmp r2, #0xe0
	bne _021E705C
	b _021E7322
_021E705C:
	pop {r4, r5, r6, pc}
_021E705E:
	cmp r2, #0xf4
	bgt _021E706E
	blt _021E7066
_021E7064:
	b _021E737C
_021E7066:
	cmp r2, #0xf0
	bne _021E706C
	b _021E72FE
_021E706C:
	pop {r4, r5, r6, pc}
_021E706E:
	cmp r2, #0xfe
	bne _021E7074
	b _021E730A
_021E7074:
	pop {r4, r5, r6, pc}
_021E7076:
	ldr r1, _021E7380 ; =ov112_021FFAA4
	mov r2, #0x68
	ldr r1, [r1, #0x24]
	bl MI_CpuCopy8
	ldr r0, _021E7380 ; =ov112_021FFAA4
	mov r5, #0xff
	ldr r4, [r0, #0x24]
	lsl r5, r5, #0x18
	ldr r3, [r4, #0x64]
	lsl r0, r3, #0x18
	add r2, r0, #0
	lsl r1, r3, #8
	lsr r0, r5, #8
	and r1, r0
	lsr r0, r3, #0x18
	lsl r0, r0, #0x18
	lsr r6, r3, #8
	lsr r3, r5, #0x10
	lsr r0, r0, #0x18
	and r3, r6
	orr r0, r3
	and r2, r5
	orr r0, r1
	orr r0, r2
	str r0, [r4, #0x64]
	bl ov112_021E6BF8
	pop {r4, r5, r6, pc}
_021E70B0:
	ldr r0, _021E7384 ; =ov112_021FFB24
	ldrb r2, [r0, #0x1c]
	cmp r2, #0x52
	bgt _021E70C8
	bge _021E7128
	cmp r2, #0x32
	bgt _021E70C2
	beq _021E7102
	pop {r4, r5, r6, pc}
_021E70C2:
	cmp r2, #0x40
	beq _021E7110
	pop {r4, r5, r6, pc}
_021E70C8:
	cmp r2, #0x60
	bgt _021E70D0
	beq _021E7136
	pop {r4, r5, r6, pc}
_021E70D0:
	add r0, r2, #0
	sub r0, #0xb0
	cmp r0, #0xe
	bhi _021E719A
	add r0, r0, r0
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_021E70E4: ; jump table
	.short _021E714E - _021E70E4 - 2 ; case 0
	.short _021E737C - _021E70E4 - 2 ; case 1
	.short _021E715C - _021E70E4 - 2 ; case 2
	.short _021E737C - _021E70E4 - 2 ; case 3
	.short _021E716A - _021E70E4 - 2 ; case 4
	.short _021E737C - _021E70E4 - 2 ; case 5
	.short _021E7178 - _021E70E4 - 2 ; case 6
	.short _021E737C - _021E70E4 - 2 ; case 7
	.short _021E7186 - _021E70E4 - 2 ; case 8
	.short _021E737C - _021E70E4 - 2 ; case 9
	.short _021E7186 - _021E70E4 - 2 ; case 10
	.short _021E737C - _021E70E4 - 2 ; case 11
	.short _021E7186 - _021E70E4 - 2 ; case 12
	.short _021E737C - _021E70E4 - 2 ; case 13
	.short _021E7186 - _021E70E4 - 2 ; case 14
_021E7102:
	mov r0, #0
	add r1, r0, #0
	mov r2, #0x38
	mov r3, #1
	bl ov112_021E5A68
	pop {r4, r5, r6, pc}
_021E7110:
	mov r0, #0
	add r1, r0, #0
	mov r2, #0x4e
	mov r3, #1
	bl ov112_021E5A68
	mov r0, #0xf
	bl ov112_021E5E18
	bl ov112_021E5A5C
	pop {r4, r5, r6, pc}
_021E7128:
	mov r0, #0
	add r1, r0, #0
	mov r2, #0x5a
	mov r3, #1
	bl ov112_021E5A68
	pop {r4, r5, r6, pc}
_021E7136:
	mov r0, #0
	add r1, r0, #0
	mov r2, #0x66
	mov r3, #1
	bl ov112_021E5A68
	mov r0, #0xf
	bl ov112_021E5E18
	bl ov112_021E5A5C
	pop {r4, r5, r6, pc}
_021E714E:
	mov r0, #0
	add r1, r0, #0
	mov r2, #0xc0
	mov r3, #1
	bl ov112_021E5A68
	pop {r4, r5, r6, pc}
_021E715C:
	mov r0, #0
	add r1, r0, #0
	mov r2, #0xc2
	mov r3, #1
	bl ov112_021E5A68
	pop {r4, r5, r6, pc}
_021E716A:
	mov r0, #0
	add r1, r0, #0
	mov r2, #0xc4
	mov r3, #1
	bl ov112_021E5A68
	pop {r4, r5, r6, pc}
_021E7178:
	mov r0, #0
	add r1, r0, #0
	mov r2, #0xc6
	mov r3, #1
	bl ov112_021E5A68
	pop {r4, r5, r6, pc}
_021E7186:
	ldr r0, _021E7380 ; =ov112_021FFAA4
	ldr r1, [r0, #0x34]
	ldrb r1, [r1]
	lsl r1, r1, #0x1c
	lsr r1, r1, #0x1c
	cmp r1, #0xf
	bne _021E71E8
	ldr r0, [r0, #4]
	cmp r0, #3
	bls _021E719C
_021E719A:
	b _021E737C
_021E719C:
	add r0, r0, r0
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_021E71A8: ; jump table
	.short _021E71B0 - _021E71A8 - 2 ; case 0
	.short _021E71BE - _021E71A8 - 2 ; case 1
	.short _021E71CC - _021E71A8 - 2 ; case 2
	.short _021E71DA - _021E71A8 - 2 ; case 3
_021E71B0:
	mov r0, #0
	add r1, r0, #0
	mov r2, #0xd0
	mov r3, #1
	bl ov112_021E5A68
	pop {r4, r5, r6, pc}
_021E71BE:
	mov r0, #0
	add r1, r0, #0
	mov r2, #0xd2
	mov r3, #1
	bl ov112_021E5A68
	pop {r4, r5, r6, pc}
_021E71CC:
	mov r0, #0
	add r1, r0, #0
	mov r2, #0xd4
	mov r3, #1
	bl ov112_021E5A68
	pop {r4, r5, r6, pc}
_021E71DA:
	mov r0, #0
	add r1, r0, #0
	mov r2, #0xd6
	mov r3, #1
	bl ov112_021E5A68
	pop {r4, r5, r6, pc}
_021E71E8:
	mov r0, #0
	add r1, r0, #0
	mov r3, #1
	bl ov112_021E5A68
	pop {r4, r5, r6, pc}
_021E71F4:
	mov r0, #0xc
	bl ov112_021E5E18
	bl ov112_021E5A5C
	pop {r4, r5, r6, pc}
_021E7200:
	mov r0, #0xc
	bl ov112_021E5E18
	bl ov112_021E5A5C
	pop {r4, r5, r6, pc}
_021E720C:
	mov r0, #3
	bl ov112_021E5E18
	bl ov112_021E5A5C
	pop {r4, r5, r6, pc}
_021E7218:
	ldr r0, _021E7384 ; =ov112_021FFB24
	mov r1, #0x3a
	strb r1, [r0, #0x1d]
	ldr r1, _021E7380 ; =ov112_021FFAA4
	ldr r2, _021E7388 ; =0x00008C50
	ldr r0, [r1, #0x50]
	cmp r0, #0
	beq _021E7232
	mov r1, #0xa
	lsl r1, r1, #6
	bl ov112_021E5EC4
	b _021E723C
_021E7232:
	ldr r0, [r1, #0x10]
	mov r1, #0xa
	lsl r1, r1, #6
	bl ov112_021E5EC4
_021E723C:
	bl ov112_021E5EEC
	pop {r4, r5, r6, pc}
_021E7242:
	mov r0, #0xf
	bl ov112_021E5E18
	bl ov112_021E5A5C
	pop {r4, r5, r6, pc}
_021E724E:
	ldr r0, _021E7384 ; =ov112_021FFB24
	mov r1, #0x46
	strb r1, [r0, #0x1d]
	ldr r1, _021E7380 ; =ov112_021FFAA4
	mov r0, #0x8f
	ldr r1, [r1, #8]
	lsl r0, r0, #8
	mov r2, #0x2a
	bl ov112_021E5EC4
	bl ov112_021E6134
	pop {r4, r5, r6, pc}
_021E7268:
	mov r0, #0xe
	bl ov112_021E5E18
	bl ov112_021E5A5C
	pop {r4, r5, r6, pc}
_021E7274:
	ldr r0, _021E7384 ; =ov112_021FFB24
	mov r1, #0x3c
	strb r1, [r0, #0x1d]
	ldr r1, _021E7380 ; =ov112_021FFAA4
	ldr r2, _021E738C ; =0x000028BE
	ldr r0, [r1, #0x58]
	cmp r0, #0
	beq _021E728E
	mov r1, #0xd7
	lsl r1, r1, #8
	bl ov112_021E5EC4
	b _021E7298
_021E728E:
	ldr r0, [r1, #0x2c]
	mov r1, #0xd7
	lsl r1, r1, #8
	bl ov112_021E5EC4
_021E7298:
	bl ov112_021E5EEC
	pop {r4, r5, r6, pc}
_021E729E:
	mov r0, #0xf
	bl ov112_021E5E18
	bl ov112_021E5A5C
	pop {r4, r5, r6, pc}
_021E72AA:
	mov r0, #0xf
	bl ov112_021E5E18
	bl ov112_021E5A5C
	pop {r4, r5, r6, pc}
_021E72B6:
	ldr r0, _021E7384 ; =ov112_021FFB24
	mov r1, #0x48
	strb r1, [r0, #0x1d]
	ldr r1, _021E7380 ; =ov112_021FFAA4
	ldr r0, _021E7390 ; =0x0000CE80
	ldr r1, [r1, #0x38]
	ldr r2, _021E7394 ; =0x00000D4C
	bl ov112_021E5EC4
	bl ov112_021E6134
	pop {r4, r5, r6, pc}
_021E72CE:
	mov r0, #0xe
	bl ov112_021E5E18
	bl ov112_021E5A5C
	pop {r4, r5, r6, pc}
_021E72DA:
	ldr r0, _021E7384 ; =ov112_021FFB24
	mov r1, #0x48
	strb r1, [r0, #0x1d]
	ldr r1, _021E7380 ; =ov112_021FFAA4
	ldr r0, _021E7390 ; =0x0000CE80
	ldr r1, [r1, #0x38]
	ldr r2, _021E7394 ; =0x00000D4C
	bl ov112_021E5EC4
	bl ov112_021E6134
	pop {r4, r5, r6, pc}
_021E72F2:
	mov r0, #0xf
	bl ov112_021E5E18
	bl ov112_021E5A5C
	pop {r4, r5, r6, pc}
_021E72FE:
	mov r0, #0xf
	bl ov112_021E5E18
	bl ov112_021E5A5C
	pop {r4, r5, r6, pc}
_021E730A:
	mov r0, #0xf
	bl ov112_021E5E18
	bl ov112_021E5A5C
	pop {r4, r5, r6, pc}
_021E7316:
	mov r0, #0xf
	bl ov112_021E5E18
	bl ov112_021E5A5C
	pop {r4, r5, r6, pc}
_021E7322:
	mov r0, #0xf
	bl ov112_021E5E18
	bl ov112_021E5A5C
	pop {r4, r5, r6, pc}
_021E732E:
	ldr r0, _021E7384 ; =ov112_021FFB24
	ldrh r0, [r0, #0x30]
	cmp r0, #0
	bne _021E733C
	bl ov112_021E6004
	pop {r4, r5, r6, pc}
_021E733C:
	bl ov112_021E5EEC
	pop {r4, r5, r6, pc}
_021E7342:
	ldr r1, _021E7384 ; =ov112_021FFB24
	add r2, r4, #0
	ldr r1, [r1, #0x28]
	bl MI_CpuCopy8
	ldr r0, _021E7384 ; =ov112_021FFB24
	ldr r1, [r0, #0x20]
	add r1, r1, r4
	str r1, [r0, #0x20]
	ldr r1, [r0, #0x28]
	add r1, r1, r4
	str r1, [r0, #0x28]
	ldrh r1, [r0, #0x30]
	sub r1, r1, r4
	strh r1, [r0, #0x30]
	ldrh r1, [r0, #0x36]
	add r1, r1, #1
	strh r1, [r0, #0x36]
	ldrh r1, [r0, #0x3a]
	add r1, r1, #1
	strh r1, [r0, #0x3a]
	ldrh r0, [r0, #0x30]
	cmp r0, #0
	bne _021E7378
	bl ov112_021E6164
	pop {r4, r5, r6, pc}
_021E7378:
	bl ov112_021E6134
_021E737C:
	pop {r4, r5, r6, pc}
	nop
_021E7380: .word ov112_021FFAA4
_021E7384: .word ov112_021FFB24
_021E7388: .word 0x00008C50
_021E738C: .word 0x000028BE
_021E7390: .word 0x0000CE80
_021E7394: .word 0x00000D4C
	thumb_func_end ov112_021E6F60

	thumb_func_start ov112_021E7398
ov112_021E7398: ; 0x021E7398
	push {r4, r5, r6, lr}
	ldr r5, _021E73C4 ; =ov112_021FFB08
	mov r4, #2
	mov r6, #4
_021E73A0:
	add r0, r5, #0
	bl OS_LockMutex
	bl ov112_021E5B98
	cmp r0, #1
	bne _021E73B4
	add r0, r4, #0
	bl ov112_021E5E18
_021E73B4:
	add r0, r5, #0
	bl OS_UnlockMutex
	add r0, r6, #0
	bl OS_Sleep
	b _021E73A0
	nop
_021E73C4: .word ov112_021FFB08
	thumb_func_end ov112_021E7398

	thumb_func_start ov112_021E73C8
ov112_021E73C8: ; 0x021E73C8
	push {r4, r5, r6, lr}
	sub sp, #8
	add r5, r0, #0
	add r4, r1, #0
	ldr r0, _021E7440 ; =ov112_021FFAA4
	mov r1, #1
	str r1, [r0, #0x18]
	str r1, [r0, #0x14]
	mov r1, #0
	str r1, [r0, #0x44]
	add r6, r2, #0
	str r1, [r0, #0x48]
	bl ov112_021E5964
	mov r0, #1
	bl ov112_021E5D5C
	ldr r0, _021E7444 ; =ov112_021E6A6C
	bl ov112_021E5D38
	ldr r0, _021E7448 ; =ov112_021E6F60
	bl ov112_021E5D44
	ldr r0, _021E744C ; =ov112_021E6BDC
	bl ov112_021E5D50
	bl CARD_SpiWaitGetStatus
	bl CARD_SpiWaitGetStatus
	cmp r0, #0xaa
	bne _021E7414
	mov r0, #0
	bl CARD_SetSpiWriteWaitCycles
	mov r0, #0
	bl CARD_SetSpiReadWaitCycles
_021E7414:
	ldr r0, _021E7450 ; =ov112_021FFB08
	bl OS_InitMutex
	ldr r0, _021E7454 ; =ov112_021FFB20
	ldr r1, _021E7458 ; =ov112_021FFAC0
	mov r2, #1
	bl OS_InitMessageQueue
	str r4, [sp]
	ldr r0, _021E745C ; =ov112_021FFBD8
	ldr r1, _021E7460 ; =ov112_021E7398
	mov r2, #0
	add r3, r5, r4
	str r6, [sp, #4]
	bl OS_CreateThread
	ldr r0, _021E745C ; =ov112_021FFBD8
	bl OS_WakeupThreadDirect
	add sp, #8
	pop {r4, r5, r6, pc}
	nop
_021E7440: .word ov112_021FFAA4
_021E7444: .word ov112_021E6A6C
_021E7448: .word ov112_021E6F60
_021E744C: .word ov112_021E6BDC
_021E7450: .word ov112_021FFB08
_021E7454: .word ov112_021FFB20
_021E7458: .word ov112_021FFAC0
_021E745C: .word ov112_021FFBD8
_021E7460: .word ov112_021E7398
	thumb_func_end ov112_021E73C8
