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
	.extern ov112_021E6164
	.extern ov112_021E6A6C
	.extern ov112_021E6BDC
	.extern ov112_021E6BF8
	.extern ov112_021E6F60
	.extern ov112_021E7398
	.extern ov112_021E73C8
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



	.extern ov112_021E7DFC
	.extern ov112_021E8B74
	.extern ov112_021E8C5C
	.extern ov112_021E8CB8



	thumb_func_start ov112_021E7DFC
ov112_021E7DFC: ; 0x021E7DFC
	push {r3, r4, r5, r6, lr}
	sub sp, #0x24
	add r5, r0, #0
	mov r0, #0x9a
	add r4, r1, #0
	add r1, r0, #0
	add r1, #0xe6
	bl Heap_Alloc
	mov r3, #0
	add r6, r0, #0
	str r3, [sp]
	mov r0, #0xa
	str r0, [sp, #4]
	mov r0, #2
	str r0, [sp, #8]
	str r3, [sp, #0xc]
	str r3, [sp, #0x10]
	ldr r0, [r5, #0x18]
	add r1, sp, #0x14
	mov r2, #1
	bl AddWindowParameterized
	add r0, sp, #0x14
	mov r1, #0
	bl FillWindowPixelBuffer
	ldr r0, _021E81D4 ; =ov112_021F5758
	add r1, r4, #0
	mov r2, #0x20
	bl MI_CpuCopy8
	add r1, r4, #0
	ldr r0, _021E81D8 ; =ov112_021F5858
	add r1, #0x20
	mov r2, #0x20
	bl MI_CpuCopy8
	add r1, r4, #0
	ldr r0, _021E81DC ; =ov112_021F5778
	add r1, #0x40
	mov r2, #0x20
	bl MI_CpuCopy8
	add r1, r4, #0
	ldr r0, _021E81E0 ; =ov112_021F5918
	add r1, #0x60
	mov r2, #0x20
	bl MI_CpuCopy8
	add r1, r4, #0
	ldr r0, _021E81E4 ; =ov112_021F5838
	add r1, #0x80
	mov r2, #0x20
	bl MI_CpuCopy8
	add r1, r4, #0
	ldr r0, _021E81E8 ; =ov112_021F5978
	add r1, #0xa0
	mov r2, #0x20
	bl MI_CpuCopy8
	add r1, r4, #0
	ldr r0, _021E81EC ; =ov112_021F5798
	add r1, #0xc0
	mov r2, #0x20
	bl MI_CpuCopy8
	add r1, r4, #0
	ldr r0, _021E81F0 ; =ov112_021F5878
	add r1, #0xe0
	mov r2, #0x20
	bl MI_CpuCopy8
	mov r1, #1
	lsl r1, r1, #8
	ldr r0, _021E81F4 ; =ov112_021F57B8
	add r1, r4, r1
	mov r2, #0x20
	bl MI_CpuCopy8
	mov r1, #0x12
	lsl r1, r1, #4
	ldr r0, _021E81F8 ; =ov112_021F58B8
	add r1, r4, r1
	mov r2, #0x20
	bl MI_CpuCopy8
	mov r1, #5
	lsl r1, r1, #6
	ldr r0, _021E81FC ; =ov112_021F5818
	add r1, r4, r1
	mov r2, #0x20
	bl MI_CpuCopy8
	mov r1, #0x16
	lsl r1, r1, #4
	ldr r0, _021E8200 ; =ov112_021F5938
	add r1, r4, r1
	mov r2, #0x20
	bl MI_CpuCopy8
	mov r1, #6
	lsl r1, r1, #6
	ldr r0, _021E8204 ; =ov112_021F57F8
	add r1, r4, r1
	mov r2, #0x20
	bl MI_CpuCopy8
	mov r1, #0x1a
	lsl r1, r1, #4
	ldr r0, _021E8208 ; =ov112_021F5998
	add r1, r4, r1
	mov r2, #0x40
	bl MI_CpuCopy8
	mov r1, #0x9e
	lsl r1, r1, #2
	ldr r0, _021E820C ; =ov112_021F56C8
	add r1, r4, r1
	mov r2, #0x10
	bl MI_CpuCopy8
	mov r1, #0xa2
	lsl r1, r1, #2
	ldr r0, _021E8210 ; =ov112_021F5718
	add r1, r4, r1
	mov r2, #0x10
	bl MI_CpuCopy8
	mov r1, #0xa6
	lsl r1, r1, #2
	ldr r0, _021E8214 ; =ov112_021F55F8
	add r1, r4, r1
	mov r2, #0x10
	bl MI_CpuCopy8
	mov r1, #0xaa
	lsl r1, r1, #2
	ldr r0, _021E8218 ; =ov112_021F5698
	add r1, r4, r1
	mov r2, #0x10
	bl MI_CpuCopy8
	mov r1, #0xae
	lsl r1, r1, #2
	ldr r0, _021E821C ; =ov112_021F5598
	add r1, r4, r1
	mov r2, #0x10
	bl MI_CpuCopy8
	mov r1, #0xb2
	lsl r1, r1, #2
	ldr r0, _021E8220 ; =ov112_021F55E8
	add r1, r4, r1
	mov r2, #0x10
	bl MI_CpuCopy8
	mov r1, #0xb6
	lsl r1, r1, #2
	ldr r0, _021E8224 ; =ov112_021F55D8
	add r1, r4, r1
	mov r2, #0x10
	bl MI_CpuCopy8
	mov r1, #0xba
	lsl r1, r1, #2
	ldr r0, _021E8228 ; =ov112_021F5688
	add r1, r4, r1
	mov r2, #0x10
	bl MI_CpuCopy8
	mov r1, #0xbe
	lsl r1, r1, #2
	ldr r0, _021E822C ; =ov112_021F55C8
	add r1, r4, r1
	mov r2, #0x10
	bl MI_CpuCopy8
	mov r1, #0xc2
	lsl r1, r1, #2
	ldr r0, _021E8230 ; =ov112_021F56B8
	add r1, r4, r1
	mov r2, #0x10
	bl MI_CpuCopy8
	mov r1, #0xc6
	lsl r1, r1, #2
	ldr r0, _021E8234 ; =ov112_021F5588
	add r1, r4, r1
	mov r2, #0x10
	bl MI_CpuCopy8
	mov r1, #0xca
	lsl r1, r1, #2
	ldr r0, _021E8238 ; =ov112_021F55A8
	add r1, r4, r1
	mov r2, #0x10
	bl MI_CpuCopy8
	mov r1, #0xce
	lsl r1, r1, #2
	ldr r0, _021E823C ; =ov112_021F5898
	add r1, r4, r1
	mov r2, #0x20
	bl MI_CpuCopy8
	mov r1, #0xd6
	lsl r1, r1, #2
	ldr r0, _021E8240 ; =ov112_021F58D8
	add r1, r4, r1
	mov r2, #0x20
	bl MI_CpuCopy8
	mov r1, #0xde
	lsl r1, r1, #2
	ldr r0, _021E8244 ; =ov112_021F58F8
	add r1, r4, r1
	mov r2, #0x20
	bl MI_CpuCopy8
	mov r1, #0xee
	lsl r1, r1, #2
	ldr r0, _021E8248 ; =ov112_021F5668
	add r1, r4, r1
	mov r2, #0x10
	bl MI_CpuCopy8
	mov r1, #0xf2
	lsl r1, r1, #2
	ldr r0, _021E824C ; =ov112_021F5638
	add r1, r4, r1
	mov r2, #8
	bl MI_CpuCopy8
	mov r1, #0x1e
	lsl r1, r1, #4
	ldr r0, _021E8250 ; =ov112_021F56F8
	add r1, r4, r1
	mov r2, #0x10
	bl MI_CpuCopy8
	mov r1, #0x1f
	lsl r1, r1, #4
	ldr r0, _021E8254 ; =ov112_021F5628
	add r1, r4, r1
	mov r2, #0x10
	bl MI_CpuCopy8
	mov r1, #0x82
	lsl r1, r1, #2
	ldr r0, _021E8258 ; =ov112_021F5648
	add r1, r4, r1
	mov r2, #0x10
	bl MI_CpuCopy8
	mov r1, #0x86
	lsl r1, r1, #2
	ldr r0, _021E825C ; =ov112_021F5748
	add r1, r4, r1
	mov r2, #0x10
	bl MI_CpuCopy8
	mov r1, #0x8a
	lsl r1, r1, #2
	ldr r0, _021E8260 ; =ov112_021F5678
	add r1, r4, r1
	mov r2, #0x10
	bl MI_CpuCopy8
	mov r1, #0x8e
	lsl r1, r1, #2
	ldr r0, _021E8264 ; =ov112_021F56D8
	add r1, r4, r1
	mov r2, #0x10
	bl MI_CpuCopy8
	mov r1, #0x92
	lsl r1, r1, #2
	ldr r0, _021E8268 ; =ov112_021F55B8
	add r1, r4, r1
	mov r2, #0x10
	bl MI_CpuCopy8
	mov r1, #0x96
	lsl r1, r1, #2
	ldr r0, _021E826C ; =ov112_021F5608
	add r1, r4, r1
	mov r2, #0x10
	bl MI_CpuCopy8
	mov r1, #0x9a
	lsl r1, r1, #2
	ldr r0, _021E8270 ; =ov112_021F56E8
	add r1, r4, r1
	mov r2, #0x10
	bl MI_CpuCopy8
	mov r1, #0x3d
	lsl r1, r1, #4
	ldr r0, _021E8274 ; =ov112_021F5618
	add r1, r4, r1
	mov r2, #0x10
	bl MI_CpuCopy8
	mov r1, #0x3e
	lsl r1, r1, #4
	ldr r0, _021E8278 ; =ov112_021F56A8
	add r1, r4, r1
	mov r2, #0x10
	bl MI_CpuCopy8
	mov r1, #0xe1
	lsl r1, r1, #4
	ldr r0, _021E827C ; =ov112_021F5B98
	add r1, r4, r1
	mov r2, #0x40
	bl MI_CpuCopy8
	mov r1, #0xe5
	lsl r1, r1, #4
	ldr r0, _021E8280 ; =ov112_021F5CD8
	add r1, r4, r1
	mov r2, #0x40
	bl MI_CpuCopy8
	mov r1, #0xe9
	lsl r1, r1, #4
	ldr r0, _021E8284 ; =ov112_021F5BD8
	add r1, r4, r1
	mov r2, #0x40
	bl MI_CpuCopy8
	mov r1, #0xed
	lsl r1, r1, #4
	ldr r0, _021E8288 ; =ov112_021F5A18
	add r1, r4, r1
	mov r2, #0x40
	bl MI_CpuCopy8
	mov r1, #0xf1
	lsl r1, r1, #4
	ldr r0, _021E828C ; =ov112_021F5C98
	add r1, r4, r1
	mov r2, #0x40
	bl MI_CpuCopy8
	mov r1, #0xf5
	lsl r1, r1, #4
	ldr r0, _021E8290 ; =ov112_021F5AD8
	add r1, r4, r1
	mov r2, #0x40
	bl MI_CpuCopy8
	mov r1, #0x69
	lsl r1, r1, #4
	mov r2, #5
	ldr r0, _021E8294 ; =ov112_021F7418
	add r1, r4, r1
	lsl r2, r2, #6
	bl MI_CpuCopy8
	mov r1, #0x7d
	lsl r1, r1, #4
	mov r2, #5
	ldr r0, _021E8298 ; =ov112_021F7558
	add r1, r4, r1
	lsl r2, r2, #6
	bl MI_CpuCopy8
	mov r1, #0x91
	lsl r1, r1, #4
	mov r2, #5
	ldr r0, _021E829C ; =ov112_021F7698
	add r1, r4, r1
	lsl r2, r2, #6
	bl MI_CpuCopy8
	mov r1, #0xa5
	lsl r1, r1, #4
	mov r2, #5
	ldr r0, _021E82A0 ; =ov112_021F77D8
	add r1, r4, r1
	lsl r2, r2, #6
	bl MI_CpuCopy8
	mov r1, #0xb9
	lsl r1, r1, #4
	mov r2, #5
	ldr r0, _021E82A4 ; =ov112_021F7918
	add r1, r4, r1
	lsl r2, r2, #6
	bl MI_CpuCopy8
	mov r1, #0xcd
	lsl r1, r1, #4
	mov r2, #5
	ldr r0, _021E82A8 ; =ov112_021F7A58
	add r1, r4, r1
	lsl r2, r2, #6
	bl MI_CpuCopy8
	ldr r1, _021E82AC ; =0x000018D0
	ldr r0, _021E82B0 ; =ov112_021F5C58
	add r1, r4, r1
	mov r2, #0x40
	bl MI_CpuCopy8
	ldr r1, _021E82B4 ; =0x00001910
	ldr r0, _021E82B8 ; =ov112_021F59D8
	add r1, r4, r1
	mov r2, #0x40
	bl MI_CpuCopy8
	ldr r1, _021E82BC ; =0x00001950
	ldr r0, _021E82C0 ; =ov112_021F6178
	add r1, r4, r1
	mov r2, #0x80
	bl MI_CpuCopy8
	ldr r1, _021E82C4 ; =0x000019D0
	ldr r0, _021E82C8 ; =ov112_021F5E18
	add r1, r4, r1
	mov r2, #0x60
	bl MI_CpuCopy8
	ldr r1, _021E82CC ; =0x00001A30
	ldr r0, _021E82D0 ; =ov112_021F6DD8
	add r1, r4, r1
	mov r2, #0xc0
	bl MI_CpuCopy8
	ldr r1, _021E82D4 ; =0x00001AF0
	ldr r0, _021E82D8 ; =ov112_021F5A58
	add r1, r4, r1
	mov r2, #0x40
	bl MI_CpuCopy8
	ldr r1, _021E82DC ; =0x00001B30
	ldr r0, _021E82E0 ; =ov112_021F5A98
	add r1, r4, r1
	mov r2, #0x40
	bl MI_CpuCopy8
	ldr r1, _021E82E4 ; =0x00001B70
	ldr r0, _021E82E8 ; =ov112_021F5B18
	add r1, r4, r1
	mov r2, #0x40
	bl MI_CpuCopy8
	ldr r1, _021E82EC ; =0x00001BB0
	ldr r0, _021E82F0 ; =ov112_021F5B58
	add r1, r4, r1
	mov r2, #0x40
	bl MI_CpuCopy8
	ldr r1, _021E82F4 ; =0x00001BF0
	ldr r0, _021E82F8 ; =ov112_021F62F8
	add r1, r4, r1
	mov r2, #0x80
	bl MI_CpuCopy8
	ldr r1, _021E82FC ; =0x00001C70
	ldr r0, _021E8300 ; =ov112_021F6378
	add r1, r4, r1
	mov r2, #0x80
	bl MI_CpuCopy8
	ldr r1, _021E8304 ; =0x00001CF0
	ldr r0, _021E8308 ; =ov112_021F6718
	add r1, r4, r1
	mov r2, #0xc0
	bl MI_CpuCopy8
	ldr r1, _021E830C ; =0x00001DB0
	ldr r0, _021E8310 ; =ov112_021F5738
	add r1, r4, r1
	mov r2, #0x10
	bl MI_CpuCopy8
	mov r1, #0x77
	lsl r1, r1, #6
	ldr r0, _021E8314 ; =ov112_021F5708
	add r1, r4, r1
	mov r2, #0x10
	bl MI_CpuCopy8
	ldr r1, _021E8318 ; =0x00001DD0
	mov r2, #3
	ldr r0, _021E831C ; =ov112_021FE198
	b _021E8320
	nop
_021E81D4: .word ov112_021F5758
_021E81D8: .word ov112_021F5858
_021E81DC: .word ov112_021F5778
_021E81E0: .word ov112_021F5918
_021E81E4: .word ov112_021F5838
_021E81E8: .word ov112_021F5978
_021E81EC: .word ov112_021F5798
_021E81F0: .word ov112_021F5878
_021E81F4: .word ov112_021F57B8
_021E81F8: .word ov112_021F58B8
_021E81FC: .word ov112_021F5818
_021E8200: .word ov112_021F5938
_021E8204: .word ov112_021F57F8
_021E8208: .word ov112_021F5998
_021E820C: .word ov112_021F56C8
_021E8210: .word ov112_021F5718
_021E8214: .word ov112_021F55F8
_021E8218: .word ov112_021F5698
_021E821C: .word ov112_021F5598
_021E8220: .word ov112_021F55E8
_021E8224: .word ov112_021F55D8
_021E8228: .word ov112_021F5688
_021E822C: .word ov112_021F55C8
_021E8230: .word ov112_021F56B8
_021E8234: .word ov112_021F5588
_021E8238: .word ov112_021F55A8
_021E823C: .word ov112_021F5898
_021E8240: .word ov112_021F58D8
_021E8244: .word ov112_021F58F8
_021E8248: .word ov112_021F5668
_021E824C: .word ov112_021F5638
_021E8250: .word ov112_021F56F8
_021E8254: .word ov112_021F5628
_021E8258: .word ov112_021F5648
_021E825C: .word ov112_021F5748
_021E8260: .word ov112_021F5678
_021E8264: .word ov112_021F56D8
_021E8268: .word ov112_021F55B8
_021E826C: .word ov112_021F5608
_021E8270: .word ov112_021F56E8
_021E8274: .word ov112_021F5618
_021E8278: .word ov112_021F56A8
_021E827C: .word ov112_021F5B98
_021E8280: .word ov112_021F5CD8
_021E8284: .word ov112_021F5BD8
_021E8288: .word ov112_021F5A18
_021E828C: .word ov112_021F5C98
_021E8290: .word ov112_021F5AD8
_021E8294: .word ov112_021F7418
_021E8298: .word ov112_021F7558
_021E829C: .word ov112_021F7698
_021E82A0: .word ov112_021F77D8
_021E82A4: .word ov112_021F7918
_021E82A8: .word ov112_021F7A58
_021E82AC: .word 0x000018D0
_021E82B0: .word ov112_021F5C58
_021E82B4: .word 0x00001910
_021E82B8: .word ov112_021F59D8
_021E82BC: .word 0x00001950
_021E82C0: .word ov112_021F6178
_021E82C4: .word 0x000019D0
_021E82C8: .word ov112_021F5E18
_021E82CC: .word 0x00001A30
_021E82D0: .word ov112_021F6DD8
_021E82D4: .word 0x00001AF0
_021E82D8: .word ov112_021F5A58
_021E82DC: .word 0x00001B30
_021E82E0: .word ov112_021F5A98
_021E82E4: .word 0x00001B70
_021E82E8: .word ov112_021F5B18
_021E82EC: .word 0x00001BB0
_021E82F0: .word ov112_021F5B58
_021E82F4: .word 0x00001BF0
_021E82F8: .word ov112_021F62F8
_021E82FC: .word 0x00001C70
_021E8300: .word ov112_021F6378
_021E8304: .word 0x00001CF0
_021E8308: .word ov112_021F6718
_021E830C: .word 0x00001DB0
_021E8310: .word ov112_021F5738
_021E8314: .word ov112_021F5708
_021E8318: .word 0x00001DD0
_021E831C: .word ov112_021FE198
_021E8320:
	add r1, r4, r1
	lsl r2, r2, #8
	bl MI_CpuCopy8
	mov r1, #0xf9
	lsl r1, r1, #4
	ldr r0, _021E86D0 ; =ov112_021F5D18
	add r1, r4, r1
	mov r2, #0x40
	bl MI_CpuCopy8
	ldr r1, _021E86D4 ; =0x00001110
	ldr r0, _021E86D8 ; =ov112_021F5C18
	add r1, r4, r1
	mov r2, #0x40
	bl MI_CpuCopy8
	ldr r1, _021E86DC ; =0x00001150
	ldr r0, _021E86E0 ; =ov112_021F65D8
	add r1, r4, r1
	mov r2, #0xa0
	bl MI_CpuCopy8
	ldr r1, _021E86E4 ; =0x000011F0
	ldr r0, _021E86E8 ; =ov112_021F61F8
	add r1, r4, r1
	mov r2, #0x80
	bl MI_CpuCopy8
	ldr r1, _021E86EC ; =0x00001270
	ldr r0, _021E86F0 ; =ov112_021F6278
	add r1, r4, r1
	mov r2, #0x80
	bl MI_CpuCopy8
	ldr r1, _021E86EC ; =0x00001270
	ldr r0, _021E86F4 ; =ov112_021F6538
	add r1, r4, r1
	mov r2, #0xa0
	bl MI_CpuCopy8
	ldr r1, _021E86F8 ; =0x00001310
	mov r2, #1
	ldr r0, _021E86FC ; =ov112_021F70D8
	add r1, r4, r1
	lsl r2, r2, #8
	bl MI_CpuCopy8
	add r0, sp, #0x14
	add r1, r5, #0
	bl ov112_021E90FC
	mov r0, #0
	str r0, [sp]
	str r0, [sp, #4]
	mov r0, #0x50
	str r0, [sp, #8]
	mov r0, #0x10
	str r0, [sp, #0xc]
	ldr r0, [sp, #0x20]
	mov r1, #0xa
	mov r2, #2
	add r3, r6, #0
	bl ov112_021E93BC
	mov r1, #0xfd
	lsl r1, r1, #4
	mov r2, #5
	add r0, r6, #0
	add r1, r4, r1
	lsl r2, r2, #6
	bl MI_CpuCopy8
	ldr r1, _021E8700 ; =0x00001690
	ldr r0, _021E8704 ; =ov112_021F67D8
	add r1, r4, r1
	mov r2, #0xc0
	bl MI_CpuCopy8
	ldr r1, _021E8708 ; =0x00001750
	ldr r0, _021E870C ; =ov112_021F6898
	add r1, r4, r1
	mov r2, #0xc0
	bl MI_CpuCopy8
	ldr r1, _021E8710 ; =0x00001810
	ldr r0, _021E8714 ; =ov112_021F7018
	add r1, r4, r1
	mov r2, #0xc0
	bl MI_CpuCopy8
	ldr r1, _021E8718 ; =0x00001410
	ldr r0, _021E871C ; =ov112_021F6498
	add r1, r4, r1
	mov r2, #0xa0
	bl MI_CpuCopy8
	ldr r1, _021E8720 ; =0x000014B0
	ldr r0, _021E8724 ; =ov112_021F63F8
	add r1, r4, r1
	mov r2, #0xa0
	bl MI_CpuCopy8
	ldr r1, _021E8728 ; =0x00001670
	ldr r0, _021E872C ; =ov112_021F57D8
	add r1, r4, r1
	mov r2, #0x20
	bl MI_CpuCopy8
	ldr r1, _021E8730 ; =0x000015B0
	ldr r0, _021E8734 ; =ov112_021F5DB8
	add r1, r4, r1
	mov r2, #0x60
	bl MI_CpuCopy8
	ldr r1, _021E8738 ; =0x00001610
	ldr r0, _021E873C ; =ov112_021F6058
	add r1, r4, r1
	mov r2, #0x60
	bl MI_CpuCopy8
	ldr r1, _021E8740 ; =0x00001550
	ldr r0, _021E8744 ; =ov112_021F5D58
	add r1, r4, r1
	mov r2, #0x60
	bl MI_CpuCopy8
	ldr r1, _021E8748 ; =0x000020D0
	mov r2, #1
	ldr r0, _021E874C ; =ov112_021F71D8
	add r1, r4, r1
	lsl r2, r2, #8
	bl MI_CpuCopy8
	ldr r1, _021E8750 ; =0x000021D0
	ldr r0, _021E8754 ; =ov112_021F5958
	add r1, r4, r1
	mov r2, #0x20
	bl MI_CpuCopy8
	ldr r1, _021E8758 ; =0x000021F0
	ldr r0, _021E875C ; =ov112_021F5658
	add r1, r4, r1
	mov r2, #0x10
	bl MI_CpuCopy8
	ldr r1, _021E8760 ; =0x000022B0
	mov r2, #6
	ldr r0, _021E8764 ; =ov112_021FCF98
	add r1, r4, r1
	lsl r2, r2, #6
	bl MI_CpuCopy8
	ldr r1, _021E8768 ; =0x00002430
	mov r2, #6
	ldr r0, _021E876C ; =ov112_021F8198
	add r1, r4, r1
	lsl r2, r2, #6
	bl MI_CpuCopy8
	ldr r1, _021E8770 ; =0x000025B0
	mov r2, #6
	ldr r0, _021E8774 ; =ov112_021F8318
	add r1, r4, r1
	lsl r2, r2, #6
	bl MI_CpuCopy8
	ldr r1, _021E8778 ; =0x00002730
	mov r2, #6
	ldr r0, _021E877C ; =ov112_021F8498
	add r1, r4, r1
	lsl r2, r2, #6
	bl MI_CpuCopy8
	ldr r1, _021E8780 ; =0x000028B0
	mov r2, #6
	ldr r0, _021E8784 ; =ov112_021FC998
	add r1, r4, r1
	lsl r2, r2, #6
	bl MI_CpuCopy8
	ldr r1, _021E8788 ; =0x00002A30
	mov r2, #6
	ldr r0, _021E878C ; =ov112_021F8618
	add r1, r4, r1
	lsl r2, r2, #6
	bl MI_CpuCopy8
	ldr r1, _021E8790 ; =0x00002BB0
	mov r2, #6
	ldr r0, _021E8794 ; =ov112_021F8798
	add r1, r4, r1
	lsl r2, r2, #6
	bl MI_CpuCopy8
	ldr r1, _021E8798 ; =0x00002D30
	mov r2, #6
	ldr r0, _021E879C ; =ov112_021F8918
	add r1, r4, r1
	lsl r2, r2, #6
	bl MI_CpuCopy8
	ldr r1, _021E87A0 ; =0x00002EB0
	mov r2, #6
	ldr r0, _021E87A4 ; =ov112_021FC398
	add r1, r4, r1
	lsl r2, r2, #6
	bl MI_CpuCopy8
	ldr r1, _021E87A8 ; =0x00003030
	mov r2, #6
	ldr r0, _021E87AC ; =ov112_021F8A98
	add r1, r4, r1
	lsl r2, r2, #6
	bl MI_CpuCopy8
	ldr r1, _021E87B0 ; =0x000031B0
	mov r2, #6
	ldr r0, _021E87B4 ; =ov112_021F8C18
	add r1, r4, r1
	lsl r2, r2, #6
	bl MI_CpuCopy8
	ldr r1, _021E87B8 ; =0x00003330
	mov r2, #6
	ldr r0, _021E87BC ; =ov112_021F8D98
	add r1, r4, r1
	lsl r2, r2, #6
	bl MI_CpuCopy8
	ldr r1, _021E87C0 ; =0x000034B0
	mov r2, #6
	ldr r0, _021E87C4 ; =ov112_021F8F18
	add r1, r4, r1
	lsl r2, r2, #6
	bl MI_CpuCopy8
	ldr r1, _021E87C8 ; =0x00003630
	mov r2, #6
	ldr r0, _021E87CC ; =ov112_021F9098
	add r1, r4, r1
	lsl r2, r2, #6
	bl MI_CpuCopy8
	ldr r1, _021E87D0 ; =0x000037B0
	mov r2, #6
	ldr r0, _021E87D4 ; =ov112_021F9218
	add r1, r4, r1
	lsl r2, r2, #6
	bl MI_CpuCopy8
	ldr r1, _021E87D8 ; =0x00003930
	mov r2, #6
	ldr r0, _021E87DC ; =ov112_021F9398
	add r1, r4, r1
	lsl r2, r2, #6
	bl MI_CpuCopy8
	ldr r1, _021E87E0 ; =0x00003AB0
	mov r2, #6
	ldr r0, _021E87E4 ; =ov112_021FB798
	add r1, r4, r1
	lsl r2, r2, #6
	bl MI_CpuCopy8
	ldr r1, _021E87E8 ; =0x00003C30
	mov r2, #6
	ldr r0, _021E87EC ; =ov112_021F9518
	add r1, r4, r1
	lsl r2, r2, #6
	bl MI_CpuCopy8
	ldr r1, _021E87F0 ; =0x00003DB0
	mov r2, #6
	ldr r0, _021E87F4 ; =ov112_021F9698
	add r1, r4, r1
	lsl r2, r2, #6
	bl MI_CpuCopy8
	ldr r1, _021E87F8 ; =0x00003F30
	mov r2, #6
	ldr r0, _021E87FC ; =ov112_021F9818
	add r1, r4, r1
	lsl r2, r2, #6
	bl MI_CpuCopy8
	ldr r1, _021E8800 ; =0x000040B0
	mov r2, #6
	ldr r0, _021E8804 ; =ov112_021FB198
	add r1, r4, r1
	lsl r2, r2, #6
	bl MI_CpuCopy8
	ldr r1, _021E8808 ; =0x00004230
	mov r2, #6
	ldr r0, _021E880C ; =ov112_021F9998
	add r1, r4, r1
	lsl r2, r2, #6
	bl MI_CpuCopy8
	ldr r1, _021E8810 ; =0x000043B0
	mov r2, #6
	ldr r0, _021E8814 ; =ov112_021F9B18
	add r1, r4, r1
	lsl r2, r2, #6
	bl MI_CpuCopy8
	ldr r1, _021E8818 ; =0x00004530
	mov r2, #6
	ldr r0, _021E881C ; =ov112_021F9C98
	add r1, r4, r1
	lsl r2, r2, #6
	bl MI_CpuCopy8
	ldr r1, _021E8820 ; =0x000046B0
	mov r2, #6
	ldr r0, _021E8824 ; =ov112_021FAB98
	add r1, r4, r1
	lsl r2, r2, #6
	bl MI_CpuCopy8
	ldr r1, _021E8828 ; =0x00004830
	mov r2, #6
	ldr r0, _021E882C ; =ov112_021F9E18
	add r1, r4, r1
	lsl r2, r2, #6
	bl MI_CpuCopy8
	ldr r1, _021E8830 ; =0x000049B0
	mov r2, #6
	ldr r0, _021E8834 ; =ov112_021F9F98
	add r1, r4, r1
	lsl r2, r2, #6
	bl MI_CpuCopy8
	ldr r1, _021E8838 ; =0x00004B30
	mov r2, #6
	ldr r0, _021E883C ; =ov112_021FA118
	add r1, r4, r1
	lsl r2, r2, #6
	bl MI_CpuCopy8
	ldr r1, _021E8840 ; =0x00004CB0
	mov r2, #6
	ldr r0, _021E8844 ; =ov112_021FA298
	add r1, r4, r1
	lsl r2, r2, #6
	bl MI_CpuCopy8
	ldr r1, _021E8848 ; =0x00004E30
	mov r2, #6
	ldr r0, _021E884C ; =ov112_021FA418
	add r1, r4, r1
	lsl r2, r2, #6
	bl MI_CpuCopy8
	ldr r1, _021E8850 ; =0x00004FB0
	mov r2, #6
	ldr r0, _021E8854 ; =ov112_021FA598
	add r1, r4, r1
	lsl r2, r2, #6
	bl MI_CpuCopy8
	ldr r1, _021E8858 ; =0x00005130
	mov r2, #6
	ldr r0, _021E885C ; =ov112_021FA718
	add r1, r4, r1
	lsl r2, r2, #6
	bl MI_CpuCopy8
	ldr r1, _021E8860 ; =0x000052B0
	mov r2, #6
	ldr r0, _021E8864 ; =ov112_021FA898
	add r1, r4, r1
	lsl r2, r2, #6
	bl MI_CpuCopy8
	ldr r1, _021E8868 ; =0x00005430
	mov r2, #6
	ldr r0, _021E886C ; =ov112_021FAA18
	add r1, r4, r1
	lsl r2, r2, #6
	bl MI_CpuCopy8
	ldr r1, _021E8870 ; =0x000055B0
	mov r2, #6
	ldr r0, _021E8874 ; =ov112_021FAD18
	add r1, r4, r1
	lsl r2, r2, #6
	bl MI_CpuCopy8
	ldr r1, _021E8878 ; =0x00005730
	mov r2, #6
	ldr r0, _021E887C ; =ov112_021FAE98
	add r1, r4, r1
	lsl r2, r2, #6
	bl MI_CpuCopy8
	ldr r1, _021E8880 ; =0x000058B0
	mov r2, #6
	ldr r0, _021E8884 ; =ov112_021FB018
	add r1, r4, r1
	lsl r2, r2, #6
	bl MI_CpuCopy8
	ldr r1, _021E8888 ; =0x00005A30
	mov r2, #6
	ldr r0, _021E888C ; =ov112_021FB318
	add r1, r4, r1
	lsl r2, r2, #6
	bl MI_CpuCopy8
	ldr r1, _021E8890 ; =0x00005BB0
	mov r2, #6
	ldr r0, _021E8894 ; =ov112_021FB498
	add r1, r4, r1
	lsl r2, r2, #6
	bl MI_CpuCopy8
	ldr r1, _021E8898 ; =0x00005D30
	mov r2, #6
	ldr r0, _021E889C ; =ov112_021FB618
	add r1, r4, r1
	lsl r2, r2, #6
	bl MI_CpuCopy8
	ldr r1, _021E88A0 ; =0x00005EB0
	mov r2, #6
	ldr r0, _021E88A4 ; =ov112_021FB918
	add r1, r4, r1
	lsl r2, r2, #6
	bl MI_CpuCopy8
	ldr r2, _021E88A8 ; =0x00006030
	ldr r0, _021E88AC ; =ov112_021FBA98
	add r1, r4, r2
	lsr r2, r2, #6
	bl MI_CpuCopy8
	ldr r1, _021E88B0 ; =0x000061B0
	mov r2, #6
	ldr r0, _021E88B4 ; =ov112_021FBC18
	add r1, r4, r1
	lsl r2, r2, #6
	bl MI_CpuCopy8
	ldr r1, _021E88B8 ; =0x00006330
	mov r2, #6
	ldr r0, _021E88BC ; =ov112_021FBD98
	add r1, r4, r1
	lsl r2, r2, #6
	bl MI_CpuCopy8
	ldr r1, _021E88C0 ; =0x000064B0
	mov r2, #6
	ldr r0, _021E88C4 ; =ov112_021FBF18
	add r1, r4, r1
	lsl r2, r2, #6
	bl MI_CpuCopy8
	ldr r1, _021E88C8 ; =0x00006630
	mov r2, #6
	ldr r0, _021E88CC ; =ov112_021FC098
	add r1, r4, r1
	lsl r2, r2, #6
	bl MI_CpuCopy8
	ldr r1, _021E88D0 ; =0x000067B0
	mov r2, #6
	ldr r0, _021E88D4 ; =ov112_021FC218
	b _021E88D8
	nop
_021E86D0: .word ov112_021F5D18
_021E86D4: .word 0x00001110
_021E86D8: .word ov112_021F5C18
_021E86DC: .word 0x00001150
_021E86E0: .word ov112_021F65D8
_021E86E4: .word 0x000011F0
_021E86E8: .word ov112_021F61F8
_021E86EC: .word 0x00001270
_021E86F0: .word ov112_021F6278
_021E86F4: .word ov112_021F6538
_021E86F8: .word 0x00001310
_021E86FC: .word ov112_021F70D8
_021E8700: .word 0x00001690
_021E8704: .word ov112_021F67D8
_021E8708: .word 0x00001750
_021E870C: .word ov112_021F6898
_021E8710: .word 0x00001810
_021E8714: .word ov112_021F7018
_021E8718: .word 0x00001410
_021E871C: .word ov112_021F6498
_021E8720: .word 0x000014B0
_021E8724: .word ov112_021F63F8
_021E8728: .word 0x00001670
_021E872C: .word ov112_021F57D8
_021E8730: .word 0x000015B0
_021E8734: .word ov112_021F5DB8
_021E8738: .word 0x00001610
_021E873C: .word ov112_021F6058
_021E8740: .word 0x00001550
_021E8744: .word ov112_021F5D58
_021E8748: .word 0x000020D0
_021E874C: .word ov112_021F71D8
_021E8750: .word 0x000021D0
_021E8754: .word ov112_021F5958
_021E8758: .word 0x000021F0
_021E875C: .word ov112_021F5658
_021E8760: .word 0x000022B0
_021E8764: .word ov112_021FCF98
_021E8768: .word 0x00002430
_021E876C: .word ov112_021F8198
_021E8770: .word 0x000025B0
_021E8774: .word ov112_021F8318
_021E8778: .word 0x00002730
_021E877C: .word ov112_021F8498
_021E8780: .word 0x000028B0
_021E8784: .word ov112_021FC998
_021E8788: .word 0x00002A30
_021E878C: .word ov112_021F8618
_021E8790: .word 0x00002BB0
_021E8794: .word ov112_021F8798
_021E8798: .word 0x00002D30
_021E879C: .word ov112_021F8918
_021E87A0: .word 0x00002EB0
_021E87A4: .word ov112_021FC398
_021E87A8: .word 0x00003030
_021E87AC: .word ov112_021F8A98
_021E87B0: .word 0x000031B0
_021E87B4: .word ov112_021F8C18
_021E87B8: .word 0x00003330
_021E87BC: .word ov112_021F8D98
_021E87C0: .word 0x000034B0
_021E87C4: .word ov112_021F8F18
_021E87C8: .word 0x00003630
_021E87CC: .word ov112_021F9098
_021E87D0: .word 0x000037B0
_021E87D4: .word ov112_021F9218
_021E87D8: .word 0x00003930
_021E87DC: .word ov112_021F9398
_021E87E0: .word 0x00003AB0
_021E87E4: .word ov112_021FB798
_021E87E8: .word 0x00003C30
_021E87EC: .word ov112_021F9518
_021E87F0: .word 0x00003DB0
_021E87F4: .word ov112_021F9698
_021E87F8: .word 0x00003F30
_021E87FC: .word ov112_021F9818
_021E8800: .word 0x000040B0
_021E8804: .word ov112_021FB198
_021E8808: .word 0x00004230
_021E880C: .word ov112_021F9998
_021E8810: .word 0x000043B0
_021E8814: .word ov112_021F9B18
_021E8818: .word 0x00004530
_021E881C: .word ov112_021F9C98
_021E8820: .word 0x000046B0
_021E8824: .word ov112_021FAB98
_021E8828: .word 0x00004830
_021E882C: .word ov112_021F9E18
_021E8830: .word 0x000049B0
_021E8834: .word ov112_021F9F98
_021E8838: .word 0x00004B30
_021E883C: .word ov112_021FA118
_021E8840: .word 0x00004CB0
_021E8844: .word ov112_021FA298
_021E8848: .word 0x00004E30
_021E884C: .word ov112_021FA418
_021E8850: .word 0x00004FB0
_021E8854: .word ov112_021FA598
_021E8858: .word 0x00005130
_021E885C: .word ov112_021FA718
_021E8860: .word 0x000052B0
_021E8864: .word ov112_021FA898
_021E8868: .word 0x00005430
_021E886C: .word ov112_021FAA18
_021E8870: .word 0x000055B0
_021E8874: .word ov112_021FAD18
_021E8878: .word 0x00005730
_021E887C: .word ov112_021FAE98
_021E8880: .word 0x000058B0
_021E8884: .word ov112_021FB018
_021E8888: .word 0x00005A30
_021E888C: .word ov112_021FB318
_021E8890: .word 0x00005BB0
_021E8894: .word ov112_021FB498
_021E8898: .word 0x00005D30
_021E889C: .word ov112_021FB618
_021E88A0: .word 0x00005EB0
_021E88A4: .word ov112_021FB918
_021E88A8: .word 0x00006030
_021E88AC: .word ov112_021FBA98
_021E88B0: .word 0x000061B0
_021E88B4: .word ov112_021FBC18
_021E88B8: .word 0x00006330
_021E88BC: .word ov112_021FBD98
_021E88C0: .word 0x000064B0
_021E88C4: .word ov112_021FBF18
_021E88C8: .word 0x00006630
_021E88CC: .word ov112_021FC098
_021E88D0: .word 0x000067B0
_021E88D4: .word ov112_021FC218
_021E88D8:
	add r1, r4, r1
	lsl r2, r2, #6
	bl MI_CpuCopy8
	ldr r1, _021E8A9C ; =0x00006930
	mov r2, #6
	ldr r0, _021E8AA0 ; =ov112_021FC518
	add r1, r4, r1
	lsl r2, r2, #6
	bl MI_CpuCopy8
	ldr r1, _021E8AA4 ; =0x00006AB0
	mov r2, #6
	ldr r0, _021E8AA8 ; =ov112_021FC698
	add r1, r4, r1
	lsl r2, r2, #6
	bl MI_CpuCopy8
	ldr r1, _021E8AAC ; =0x00006C30
	mov r2, #6
	ldr r0, _021E8AB0 ; =ov112_021FC818
	add r1, r4, r1
	lsl r2, r2, #6
	bl MI_CpuCopy8
	ldr r1, _021E8AB4 ; =0x00006DB0
	mov r2, #6
	ldr r0, _021E8AB8 ; =ov112_021FCB18
	add r1, r4, r1
	lsl r2, r2, #6
	bl MI_CpuCopy8
	ldr r1, _021E8ABC ; =0x00006F30
	mov r2, #6
	ldr r0, _021E8AC0 ; =ov112_021FCC98
	add r1, r4, r1
	lsl r2, r2, #6
	bl MI_CpuCopy8
	ldr r1, _021E8AC4 ; =0x000070B0
	mov r2, #6
	ldr r0, _021E8AC8 ; =ov112_021FCE18
	add r1, r4, r1
	lsl r2, r2, #6
	bl MI_CpuCopy8
	ldr r1, _021E8ACC ; =0x00007230
	mov r2, #6
	ldr r0, _021E8AD0 ; =ov112_021FD118
	add r1, r4, r1
	lsl r2, r2, #6
	bl MI_CpuCopy8
	ldr r1, _021E8AD4 ; =0x000073B0
	mov r2, #6
	ldr r0, _021E8AD8 ; =ov112_021FD298
	add r1, r4, r1
	lsl r2, r2, #6
	bl MI_CpuCopy8
	ldr r1, _021E8ADC ; =0x00007530
	mov r2, #6
	ldr r0, _021E8AE0 ; =ov112_021FD418
	add r1, r4, r1
	lsl r2, r2, #6
	bl MI_CpuCopy8
	ldr r1, _021E8AE4 ; =0x000076B0
	mov r2, #6
	ldr r0, _021E8AE8 ; =ov112_021FD598
	add r1, r4, r1
	lsl r2, r2, #6
	bl MI_CpuCopy8
	ldr r1, _021E8AEC ; =0x00007830
	mov r2, #6
	ldr r0, _021E8AF0 ; =ov112_021FD718
	add r1, r4, r1
	lsl r2, r2, #6
	bl MI_CpuCopy8
	ldr r1, _021E8AF4 ; =0x000079B0
	mov r2, #6
	ldr r0, _021E8AF8 ; =ov112_021FD898
	add r1, r4, r1
	lsl r2, r2, #6
	bl MI_CpuCopy8
	ldr r1, _021E8AFC ; =0x00007B30
	mov r2, #6
	ldr r0, _021E8B00 ; =ov112_021FDA18
	add r1, r4, r1
	lsl r2, r2, #6
	bl MI_CpuCopy8
	ldr r1, _021E8B04 ; =0x00007CB0
	mov r2, #6
	ldr r0, _021E8B08 ; =ov112_021FDB98
	add r1, r4, r1
	lsl r2, r2, #6
	bl MI_CpuCopy8
	ldr r1, _021E8B0C ; =0x00007E30
	mov r2, #6
	ldr r0, _021E8B10 ; =ov112_021FDD18
	add r1, r4, r1
	lsl r2, r2, #6
	bl MI_CpuCopy8
	ldr r1, _021E8B14 ; =0x00007FB0
	mov r2, #6
	ldr r0, _021E8B18 ; =ov112_021FDE98
	add r1, r4, r1
	lsl r2, r2, #6
	bl MI_CpuCopy8
	ldr r1, _021E8B1C ; =0x00008130
	mov r2, #6
	ldr r0, _021E8B20 ; =ov112_021FE018
	add r1, r4, r1
	lsl r2, r2, #6
	bl MI_CpuCopy8
	ldr r1, _021E8B24 ; =0x000082B0
	mov r2, #6
	ldr r0, _021E8B28 ; =ov112_021F7B98
	add r1, r4, r1
	lsl r2, r2, #6
	bl MI_CpuCopy8
	ldr r1, _021E8B2C ; =0x00008430
	mov r2, #6
	ldr r0, _021E8B30 ; =ov112_021F7D18
	add r1, r4, r1
	lsl r2, r2, #6
	bl MI_CpuCopy8
	ldr r1, _021E8B34 ; =0x000085B0
	mov r2, #6
	ldr r0, _021E8B38 ; =ov112_021F7E98
	add r1, r4, r1
	lsl r2, r2, #6
	bl MI_CpuCopy8
	ldr r1, _021E8B3C ; =0x00008730
	mov r2, #6
	ldr r0, _021E8B40 ; =ov112_021F8018
	add r1, r4, r1
	lsl r2, r2, #6
	bl MI_CpuCopy8
	ldr r1, _021E8B44 ; =0x000088B0
	mov r2, #6
	ldr r0, _021E8B48 ; =ov112_021F72D8
	add r1, r4, r1
	lsl r2, r2, #6
	bl MI_CpuCopy8
	ldr r1, _021E8B4C ; =0x00002210
	ldr r0, _021E8B50 ; =ov112_021F6678
	add r1, r4, r1
	mov r2, #0xa0
	bl MI_CpuCopy8
	mov r1, #0x3f
	lsl r1, r1, #4
	ldr r0, _021E8B54 ; =ov112_021F5E78
	add r1, r4, r1
	mov r2, #0x60
	bl MI_CpuCopy8
	mov r1, #0x45
	lsl r1, r1, #4
	ldr r0, _021E8B58 ; =ov112_021F5ED8
	add r1, r4, r1
	mov r2, #0x60
	bl MI_CpuCopy8
	mov r1, #0x4b
	lsl r1, r1, #4
	ldr r0, _021E8B5C ; =ov112_021F5F38
	add r1, r4, r1
	mov r2, #0x60
	bl MI_CpuCopy8
	mov r1, #0x51
	lsl r1, r1, #4
	ldr r0, _021E8B60 ; =ov112_021F5F98
	add r1, r4, r1
	mov r2, #0x60
	bl MI_CpuCopy8
	mov r1, #0x57
	lsl r1, r1, #4
	ldr r0, _021E8B64 ; =ov112_021F5FF8
	add r1, r4, r1
	mov r2, #0x60
	bl MI_CpuCopy8
	mov r1, #0x5d
	lsl r1, r1, #4
	ldr r0, _021E8B68 ; =ov112_021F6118
	add r1, r4, r1
	mov r2, #0x60
	bl MI_CpuCopy8
	mov r1, #0x63
	lsl r1, r1, #4
	ldr r0, _021E8B6C ; =ov112_021F60B8
	add r1, r4, r1
	mov r2, #0x60
	bl MI_CpuCopy8
	ldr r0, _021E8B70 ; =0x00008A30
	add r0, r4, r0
	bl ov112_021F0D48
	add r0, r6, #0
	bl Heap_Free
	add r0, sp, #0x14
	bl RemoveWindow
	add sp, #0x24
	pop {r3, r4, r5, r6, pc}
	nop
_021E8A9C: .word 0x00006930
_021E8AA0: .word ov112_021FC518
_021E8AA4: .word 0x00006AB0
_021E8AA8: .word ov112_021FC698
_021E8AAC: .word 0x00006C30
_021E8AB0: .word ov112_021FC818
_021E8AB4: .word 0x00006DB0
_021E8AB8: .word ov112_021FCB18
_021E8ABC: .word 0x00006F30
_021E8AC0: .word ov112_021FCC98
_021E8AC4: .word 0x000070B0
_021E8AC8: .word ov112_021FCE18
_021E8ACC: .word 0x00007230
_021E8AD0: .word ov112_021FD118
_021E8AD4: .word 0x000073B0
_021E8AD8: .word ov112_021FD298
_021E8ADC: .word 0x00007530
_021E8AE0: .word ov112_021FD418
_021E8AE4: .word 0x000076B0
_021E8AE8: .word ov112_021FD598
_021E8AEC: .word 0x00007830
_021E8AF0: .word ov112_021FD718
_021E8AF4: .word 0x000079B0
_021E8AF8: .word ov112_021FD898
_021E8AFC: .word 0x00007B30
_021E8B00: .word ov112_021FDA18
_021E8B04: .word 0x00007CB0
_021E8B08: .word ov112_021FDB98
_021E8B0C: .word 0x00007E30
_021E8B10: .word ov112_021FDD18
_021E8B14: .word 0x00007FB0
_021E8B18: .word ov112_021FDE98
_021E8B1C: .word 0x00008130
_021E8B20: .word ov112_021FE018
_021E8B24: .word 0x000082B0
_021E8B28: .word ov112_021F7B98
_021E8B2C: .word 0x00008430
_021E8B30: .word ov112_021F7D18
_021E8B34: .word 0x000085B0
_021E8B38: .word ov112_021F7E98
_021E8B3C: .word 0x00008730
_021E8B40: .word ov112_021F8018
_021E8B44: .word 0x000088B0
_021E8B48: .word ov112_021F72D8
_021E8B4C: .word 0x00002210
_021E8B50: .word ov112_021F6678
_021E8B54: .word ov112_021F5E78
_021E8B58: .word ov112_021F5ED8
_021E8B5C: .word ov112_021F5F38
_021E8B60: .word ov112_021F5F98
_021E8B64: .word ov112_021F5FF8
_021E8B68: .word ov112_021F6118
_021E8B6C: .word ov112_021F60B8
_021E8B70: .word 0x00008A30
	thumb_func_end ov112_021E7DFC
