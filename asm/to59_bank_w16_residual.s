	.include "asm/macros.inc"
	.include "overlay_07.inc"
	.include "global.inc"

	.extern _02234B80
	.extern _02237840
	.extern ov07_02233EB8
	.extern ov07_02233EBC
	.extern ov07_022344B4
	.extern ov07_022344C0
	.extern ov07_02234530
	.extern ov07_0223474C
	.extern ov07_0223475C
	.extern ov07_0223494C
	.extern ov07_02234A20
	.extern ov07_02234B8C
	.extern ov07_02234B98
	.extern ov07_02234BA8
	.extern ov07_02234BC0
	.extern ov07_02234BD8
	.extern ov07_02234BF0
	.extern ov07_02234C08
	.extern ov07_02234C20
	.extern ov07_02234C40
	.extern ov07_02234C64
	.extern ov07_02234CC8
	.extern ov07_02234D58
	.extern ov07_02234DE8
	.extern ov07_02234F48
	.extern ov07_022353E4
	.extern ov07_02235440
	.extern ov07_02235488
	.extern ov07_022354D0
	.extern ov07_022354E0
	.extern ov07_02235510
	.extern ov07_02235540
	.extern ov07_02235578
	.extern ov07_022355B0
	.extern ov07_02235620
	.extern ov07_02235624
	.extern ov07_02235628
	.extern ov07_022357D0
	.extern ov07_02235980
	.extern ov07_02235984
	.extern ov07_02235988
	.extern ov07_02235B30
	.extern ov07_02235B34
	.extern ov07_02235B38
	.extern ov07_02235CE0
	.extern ov07_02235CE4
	.extern ov07_02235CE8
	.extern ov07_02235E90
	.extern ov07_02235E94
	.extern ov07_02235E98
	.extern ov07_02236040
	.extern ov07_02236044
	.extern ov07_02236048
	.extern ov07_022361F0
	.extern ov07_022361F4
	.extern ov07_022361F8
	.extern ov07_022363A0
	.extern ov07_022363D8
	.extern ov07_02236414
	.extern ov07_02236444
	.extern ov07_02236468
	.extern ov07_0223646A
	.extern ov07_0223649C
	.extern ov07_02236520
	.extern ov07_02236670
	.extern ov07_022366C8
	.extern ov07_022366CC
	.extern ov07_022366CD
	.extern ov07_022366CE
	.extern ov07_022366D2
	.extern ov07_022366D8
	.extern ov07_022366D9
	.extern ov07_022366DA
	.extern ov07_022366E1
	.extern ov07_022366EB
	.extern ov07_022366EC
	.extern ov07_022366ED
	.extern ov07_022366EE
	.extern ov07_022366EF
	.extern ov07_022366F5
	.extern ov07_022366F6
	.extern ov07_022366F7
	.extern ov07_022366F8
	.extern ov07_022366F9
	.extern ov07_02236704
	.extern ov07_02236705
	.extern ov07_02236706
	.extern ov07_02236707
	.extern ov07_02236708
	.extern ov07_02236713
	.extern ov07_02236714
	.extern ov07_02236715
	.extern ov07_02236716
	.extern ov07_02236717
	.extern ov07_02236722
	.extern ov07_02236732
	.extern ov07_02236734
	.extern ov07_02236736
	.extern ov07_0223674C
	.extern ov07_02236750
	.extern ov07_02236774
	.extern ov07_02236778
	.extern ov07_0223677E
	.extern ov07_02236780
	.extern ov07_02236796
	.extern ov07_022367AE
	.extern ov07_022367B0
	.extern ov07_022367B2
	.extern ov07_022367B4
	.extern ov07_022367CE
	.extern ov07_02236800
	.extern ov07_0223680C
	.extern ov07_02236810
	.extern ov07_02236814
	.extern ov07_02236858
	.extern ov07_02236864
	.extern ov07_0223687C
	.extern ov07_0223717C
	.extern ov07_02237188
	.extern ov07_022371B8
	.extern ov07_02237200
	.extern ov07_02237228
	.extern ov07_02237254
	.extern ov07_0223729C
	.extern ov07_0223730E
	.extern ov07_02237310
	.extern ov07_0223738C
	.extern ov07_02237390
	.extern ov07_0223746C
	.extern ov07_02237470
	.extern ov07_02237474
	.extern ov07_022375BC
	.extern ov07_02237870
	.extern ov07_0223789C
	.extern ov07_022378B0

	.text

	.extern ov07_0221BE20
	.extern ov07_0221BE44
	.extern ov07_0221BE68
	.extern ov07_0221BEA4
	.extern ov07_0221BEDC
	.extern ov07_0221BFC0
	.extern ov07_0221BFD0
	.extern ov07_0221BFE0
	.extern ov07_0221C01C
	.extern ov07_0221C394
	.extern ov07_0221C3B0
	.extern ov07_0221C3C0
	.extern ov07_0221C3DC
	.extern ov07_0221C3F4
	.extern ov07_0221C410
	.extern ov07_0221C42C
	.extern ov07_0221C448
	.extern ov07_0221C458
	.extern ov07_0221C468
	.extern ov07_0221C470
	.extern ov07_0221C478
	.extern ov07_0221C488
	.extern ov07_0221C494
	.extern ov07_0221C4A0
	.extern ov07_0221C4A8
	.extern ov07_0221C4C0
	.extern ov07_0221C4E8
	.extern ov07_0221C514
	.extern ov07_0221C528
	.extern ov07_0221C530
	.extern ov07_0221C53C
	.extern ov07_0221C56C
	.extern ov07_0221C584
	.extern ov07_0221C5C4
	.extern ov07_0221C5C8
	.extern ov07_0221C624
	.extern ov07_0221C62C
	.extern ov07_0221C634
	.extern ov07_0221C674
	.extern ov07_0221C69C
	.extern ov07_0221C6B4
	.extern ov07_0221C6EC
	.extern ov07_0221C70C
	.extern ov07_0221C72C
	.extern ov07_0221C74C
	.extern ov07_0221C768
	.extern ov07_0221C7B8
	.extern ov07_0221C8CC
	.extern ov07_0221C910
	.extern ov07_0221C934
	.extern ov07_0221C974
	.extern ov07_0221C9C0
	.extern ov07_0221CBC0
	.extern ov07_0221CC54
	.extern ov07_0221CCF4
	.extern ov07_0221CD4C
	.extern ov07_0221CDA4
	.extern ov07_0221CE5C
	.extern ov07_0221CF14
	.extern ov07_0221CF54
	.extern ov07_0221CFA8
	.extern ov07_0221D000
	.extern ov07_0221D02C
	.extern ov07_0221D05C
	.extern ov07_0221D088
	.extern ov07_0221D0C0
	.extern ov07_0221D0E8
	.extern ov07_0221D148
	.extern ov07_0221D174
	.extern ov07_0221D1B8
	.extern ov07_0221D214
	.extern ov07_0221D23C
	.extern ov07_0221D270
	.extern ov07_0221D2B0
	.extern ov07_0221D2E4
	.extern ov07_0221D2F4
	.extern ov07_0221D330
	.extern ov07_0221D374
	.extern ov07_0221D3CC
	.extern ov07_0221D4B0
	.extern ov07_0221D4FC
	.extern ov07_0221D55C
	.extern ov07_0221D5A8
	.extern ov07_0221D5AC
	.extern ov07_0221D5B0
	.extern ov07_0221D718
	.extern ov07_0221D740
	.extern ov07_0221D7B8
	.extern ov07_0221D874
	.extern ov07_0221DA74
	.extern ov07_0221DAA0
	.extern ov07_0221DAD0
	.extern ov07_0221DAF4
	.extern ov07_0221DCA8
	.extern ov07_0221DCC0
	.extern ov07_0221DCD4
	.extern ov07_0221DCF0
	.extern ov07_0221DD0C
	.extern ov07_0221DD10
	.extern ov07_0221DD14
	.extern ov07_0221DD38
	.extern ov07_0221DD8C
	.extern ov07_0221DDB0
	.extern ov07_0221DE04
	.extern ov07_0221DEB0
	.extern ov07_0221DEC0
	.extern ov07_0221DF1C
	.extern ov07_0221DFB4
	.extern ov07_0221E0B4
	.extern ov07_0221E280
	.extern ov07_0221E3B8
	.extern ov07_0221E5E0
	.extern ov07_0221E5EC
	.extern ov07_0221E600
	.extern ov07_0221E664
	.extern ov07_0221E6C8
	.extern ov07_0221E6D0
	.extern ov07_0221E6D4
	.extern ov07_0221E788
	.extern ov07_0221E868
	.extern ov07_0221E87C
	.extern ov07_0221E914
	.extern ov07_0221E9D4
	.extern ov07_0221EB84
	.extern ov07_0221EB98
	.extern ov07_0221EBA4
	.extern ov07_0221EC7C
	.extern ov07_0221ECE8
	.extern ov07_0221ED00
	.extern ov07_0221ED14
	.extern ov07_0221ED44
	.extern ov07_0221ED94
	.extern ov07_0221EE74
	.extern ov07_0221EEC0
	.extern ov07_0221EF14
	.extern ov07_0221EF34
	.extern ov07_0221EF54
	.extern ov07_0221F024
	.extern ov07_0221F084
	.extern ov07_0221F088
	.extern ov07_0221F08C
	.extern ov07_0221F090
	.extern ov07_0221F094
	.extern ov07_0221F098
	.extern ov07_0221F0B0
	.extern ov07_0221F0CC
	.extern ov07_0221F104
	.extern ov07_0221F120
	.extern ov07_0221F1C0
	.extern ov07_0221F238
	.extern ov07_0221F2DC
	.extern ov07_0221F340
	.extern ov07_0221F398
	.extern ov07_0221F39C
	.extern ov07_0221F3F4
	.extern ov07_0221F424
	.extern ov07_0221F42C
	.extern ov07_0221F430
	.extern ov07_0221F4CC
	.extern ov07_0221F514
	.extern ov07_0221F574
	.extern ov07_0221F5BC
	.extern ov07_0221F604
	.extern ov07_0221F710
	.extern ov07_0221F7C4
	.extern ov07_0221F7F0
	.extern ov07_0221F814
	.extern ov07_0221F81C
	.extern ov07_0221F880
	.extern ov07_0221F8AC
	.extern ov07_0221F8B0
	.extern ov07_0221F8C4
	.extern ov07_0221F8C8
	.extern ov07_0221F980
	.extern ov07_0221F9A8
	.extern ov07_0221F9E8
	.extern ov07_0221FA04
	.extern ov07_0221FA10
	.extern ov07_0221FA1C
	.extern ov07_0221FA2C
	.extern ov07_0221FA38
	.extern ov07_0221FA48
	.extern ov07_0221FA78
	.extern ov07_0221FA80
	.extern ov07_0221FA90
	.extern ov07_0221FAA0
	.extern ov07_0221FAB0
	.extern ov07_0221FAC8
	.extern ov07_0221FAE8
	.extern ov07_0221FAEC
	.extern ov07_0221FAF8
	.extern ov07_0221FB04
	.extern ov07_0221FB30
	.extern ov07_0221FB58
	.extern ov07_0221FB78
	.extern ov07_0221FB7C
	.extern ov07_0221FB90
	.extern ov07_0221FDFC
	.extern ov07_0221FE08
	.extern ov07_0221FE3C
	.extern ov07_0221FE70
	.extern ov07_0221FE84
	.extern ov07_0221FE88
	.extern ov07_0221FEB0
	.extern ov07_0221FEDC
	.extern ov07_0221FEE4
	.extern ov07_0221FF18
	.extern ov07_0221FF2C
	.extern ov07_0221FF34
	.extern ov07_0221FF74
	.extern ov07_0221FF78
	.extern ov07_02220030
	.extern ov07_0222008C
	.extern ov07_022200D0
	.extern ov07_02220114
	.extern ov07_02220160
	.extern ov07_022201C8
	.extern ov07_02220230
	.extern ov07_022202D8
	.extern ov07_0222036C
	.extern ov07_0222043C
	.extern ov07_0222050C
	.extern ov07_022207D0
	.extern ov07_02220800
	.extern ov07_0222080C
	.extern ov07_02220818
	.extern ov07_02220824
	.extern ov07_02220830
	.extern ov07_0222083C
	.extern ov07_02220848
	.extern ov07_02220854
	.extern ov07_02220860
	.extern ov07_0222086C
	.extern ov07_02220878
	.extern ov07_02220884
	.extern ov07_022208B4
	.extern ov07_022208F8
	.extern ov07_02220938
	.extern ov07_02220978
	.extern ov07_02220CAC
	.extern ov07_0222150C
	.extern ov07_02221540
	.extern ov07_02221550
	.extern ov07_022215B4
	.extern ov07_02221664
	.extern ov07_022216A8
	.extern ov07_02221734
	.extern ov07_022217A4
	.extern ov07_022217B0
	.extern ov07_022217B4
	.extern ov07_02221898
	.extern ov07_0222189C
	.extern ov07_022218A0
	.extern ov07_02221968
	.extern ov07_022219EC
	.extern ov07_02221A6C
	.extern ov07_02221AD4
	.extern ov07_02221AD8
	.extern ov07_02221ADC
	.extern ov07_02221AE0
	.extern ov07_02221AE4
	.extern ov07_02221BF0
	.extern ov07_02221C74
	.extern ov07_02221C8C
	.extern ov07_02221CB4
	.extern ov07_02221CE0
	.extern ov07_02221CEC
	.extern ov07_02221D10
	.extern ov07_02221D4C
	.extern ov07_02221D88
	.extern ov07_02221DC4
	.extern ov07_02221E24
	.extern ov07_02221E60
	.extern ov07_02221E9C
	.extern ov07_02221EC4
	.extern ov07_02221EEC
	.extern ov07_02221F04
	.extern ov07_02221F38
	.extern ov07_02221F80
	.extern ov07_02221FF0
	.extern ov07_02222004
	.extern ov07_0222202C
	.extern ov07_02222040
	.extern ov07_0222204C
	.extern ov07_0222207C
	.extern ov07_0222209C
	.extern ov07_022220B8
	.extern ov07_022220DC
	.extern ov07_022220FC
	.extern ov07_0222212C
	.extern ov07_02222180
	.extern ov07_02222218
	.extern ov07_02222240
	.extern ov07_02222268
	.extern ov07_022222B4
	.extern ov07_022222F0
	.extern ov07_02222314
	.extern ov07_02222338
	.extern ov07_02222384
	.extern ov07_022223CC
	.extern ov07_022223F0
	.extern ov07_02222418
	.extern ov07_02222440
	.extern ov07_02222468
	.extern ov07_02222494
	.extern ov07_022224AC
	.extern ov07_02222508
	.extern ov07_02222558
	.extern ov07_02222590
	.extern ov07_0222260C
	.extern ov07_02222644
	.extern ov07_02222674
	.extern ov07_022226C4
	.extern ov07_022226FC
	.extern ov07_02222748
	.extern ov07_02222768
	.extern ov07_02222788
	.extern ov07_022227A8
	.extern ov07_022227D8
	.extern ov07_0222283C
	.extern ov07_02222864
	.extern ov07_02222914
	.extern ov07_02222A44
	.extern ov07_02222A7C
	.extern ov07_02222AC4
	.extern ov07_02222AF4
	.extern ov07_02222AF8
	.extern ov07_02222B14
	.extern ov07_02222B30
	.extern ov07_02222B70
	.extern ov07_02222B94
	.extern ov07_02222BA4
	.extern ov07_02222BC8
	.extern ov07_02222BDC
	.extern ov07_02222BE4
	.extern ov07_02222C60
	.extern ov07_02222C84
	.extern ov07_02222C98
	.extern ov07_02222CAC
	.extern ov07_02222CC0
	.extern ov07_02222CCC
	.extern ov07_02222D3C
	.extern ov07_02222D60
	.extern ov07_02222D74
	.extern ov07_02222D88
	.extern ov07_02222D90
	.extern ov07_02222DC8
	.extern ov07_02222DE4
	.extern ov07_02222E0C
	.extern ov07_02222E48
	.extern ov07_02222E74
	.extern ov07_02222EE8
	.extern ov07_02222EF8
	.extern ov07_02222F10
	.extern ov07_02222F7C
	.extern ov07_02222FC4
	.extern ov07_02222FF4
	.extern ov07_02223038
	.extern ov07_0222304C
	.extern ov07_02223060
	.extern ov07_02223064
	.extern ov07_0222308C
	.extern ov07_022230B4
	.extern ov07_022230DC
	.extern ov07_02223104
	.extern ov07_02223128
	.extern ov07_0222314C
	.extern ov07_022231BC
	.extern ov07_02223224
	.extern ov07_02223240
	.extern ov07_02223350
	.extern ov07_022233D8
	.extern ov07_02223550
	.extern ov07_022236F0
	.extern ov07_02223864
	.extern ov07_02223A1C
	.extern ov07_02223A94
	.extern ov07_02223B70
	.extern ov07_02223BC0
	.extern ov07_02223CAC
	.extern ov07_02223CE4
	.extern ov07_02223D28
	.extern ov07_02223DC4
	.extern ov07_02223E08
	.extern ov07_02223E94
	.extern ov07_02223EB4
	.extern ov07_02223F5C
	.extern ov07_02224008
	.extern ov07_022241D8
	.extern ov07_02224348
	.extern ov07_0222446C
	.extern ov07_022245BC
	.extern ov07_022247C8
	.extern ov07_022247F4
	.extern ov07_022248A0
	.extern ov07_022248F4
	.extern ov07_02224948
	.extern ov07_02224998
	.extern ov07_02224A20
	.extern ov07_02224B14
	.extern ov07_02224BAC
	.extern ov07_02224CB8
	.extern ov07_02224E58
	.extern ov07_02224EF4
	.extern ov07_02225098
	.extern ov07_0222519C
	.extern ov07_022251CC
	.extern ov07_0222525C
	.extern ov07_022252F8
	.extern ov07_02225444
	.extern ov07_0222548C
	.extern ov07_02225504
	.extern ov07_0222554C
	.extern ov07_02225640
	.extern ov07_02225768
	.extern ov07_02225810
	.extern ov07_02225838
	.extern ov07_02225904
	.extern ov07_02225990
	.extern ov07_02225AE0
	.extern ov07_02225B38
	.extern ov07_02225BC4
	.extern ov07_02225C50
	.extern ov07_02225CC4
	.extern ov07_02225DC4
	.extern ov07_02225E9C
	.extern ov07_02225F18
	.extern ov07_02225F94
	.extern ov07_0222600C
	.extern ov07_022260DC
	.extern ov07_022260F8
	.extern ov07_022261B4
	.extern ov07_022262E8
	.extern ov07_0222647C
	.extern ov07_022264E0
	.extern ov07_02226628
	.extern ov07_02226698
	.extern ov07_02226804
	.extern ov07_02226838
	.extern ov07_02226880
	.extern ov07_022269D8
	.extern ov07_02226A68
	.extern ov07_02226A9C
	.extern ov07_02226B2C
	.extern ov07_02226C4C
	.extern ov07_02226C90
	.extern ov07_02226CB0
	.extern ov07_02226F04
	.extern ov07_02226F80
	.extern ov07_0222719C
	.extern ov07_022271DC
	.extern ov07_02227248
	.extern ov07_022272D0
	.extern ov07_02227300
	.extern ov07_022274F8
	.extern ov07_02227630
	.extern ov07_02227824
	.extern ov07_022278B0
	.extern ov07_02227AEC
	.extern ov07_02227B58
	.extern ov07_02227D1C
	.extern ov07_02227FA4
	.extern ov07_02228070
	.extern ov07_022280A8
	.extern ov07_02228174
	.extern ov07_022281C0
	.extern ov07_02228384
	.extern ov07_022283D8
	.extern ov07_022284D8
	.extern ov07_02228510
	.extern ov07_02228638
	.extern ov07_02228684
	.extern ov07_02228750
	.extern ov07_02228834
	.extern ov07_02228A50
	.extern ov07_02228B38
	.extern ov07_02228C8C
	.extern ov07_02228D08
	.extern ov07_02228D64
	.extern ov07_02228E70
	.extern ov07_02228F14
	.extern ov07_02228F88
	.extern ov07_02229024
	.extern ov07_022290BC
	.extern ov07_0222915C
	.extern ov07_02229220
	.extern ov07_0222928C
	.extern ov07_022292E4
	.extern ov07_02229480
	.extern ov07_02229630
	.extern ov07_022297B8
	.extern ov07_02229BFC
	.extern ov07_02229D40
	.extern ov07_02229E50
	.extern ov07_02229FC4
	.extern ov07_0222A1F0
	.extern ov07_0222A328
	.extern ov07_0222A4B4
	.extern ov07_0222A58C
	.extern ov07_0222A710
	.extern ov07_0222A838
	.extern ov07_0222A8D8
	.extern ov07_0222A8EC
	.extern ov07_0222A988
	.extern ov07_0222AA20
	.extern ov07_0222ACAC
	.extern ov07_0222AE14
	.extern ov07_0222AEA0
	.extern ov07_0222AFAC
	.extern ov07_0222B130
	.extern ov07_0222B2A8
	.extern ov07_0222B320
	.extern ov07_0222B4E0
	.extern ov07_0222B640
	.extern ov07_0222B898
	.extern ov07_0222B988
	.extern ov07_0222BADC
	.extern ov07_0222BAF4
	.extern ov07_0222BC7C
	.extern ov07_0222BD90
	.extern ov07_0222BE00
	.extern ov07_0222BF18
	.extern ov07_0222C074
	.extern ov07_0222C18C
	.extern ov07_0222C1FC
	.extern ov07_0222C208
	.extern ov07_0222C214
	.extern ov07_0222C2D4
	.extern ov07_0222C4C0
	.extern ov07_0222C610
	.extern ov07_0222C6A8
	.extern ov07_0222C780
	.extern ov07_0222C850
	.extern ov07_0222C870
	.extern ov07_0222C918
	.extern ov07_0222CA8C
	.extern ov07_0222CBFC
	.extern ov07_0222CCBC
	.extern ov07_0222CE10
	.extern ov07_0222CEC0
	.extern ov07_0222CFCC
	.extern ov07_0222D050
	.extern ov07_0222D148
	.extern ov07_0222D2B0
	.extern ov07_0222D3BC
	.extern ov07_0222D4F4
	.extern ov07_0222D56C
	.extern ov07_0222D578
	.extern ov07_0222D590
	.extern ov07_0222D60C
	.extern ov07_0222D6E0
	.extern ov07_0222D7A4
	.extern ov07_0222D974
	.extern ov07_0222DA60
	.extern ov07_0222DB14
	.extern ov07_0222DBD8
	.extern ov07_0222DC54
	.extern ov07_0222DCD8
	.extern ov07_0222DD20
	.extern ov07_0222DE30
	.extern ov07_0222DEEC
	.extern ov07_0222E020
	.extern ov07_0222E07C
	.extern ov07_0222E108
	.extern ov07_0222E2EC
	.extern ov07_0222E3A4
	.extern ov07_0222E3E8
	.extern ov07_0222E4F8
	.extern ov07_0222E54C
	.extern ov07_0222E5E8
	.extern ov07_0222E6A8
	.extern ov07_0222E6F4
	.extern ov07_0222E754
	.extern ov07_0222E7BC
	.extern ov07_0222E894
	.extern ov07_0222E90C
	.extern ov07_0222EA48
	.extern ov07_0222EAB4
	.extern ov07_0222EB00
	.extern ov07_0222EB6C
	.extern ov07_0222EBB0
	.extern ov07_0222ED7C
	.extern ov07_0222EDF0
	.extern ov07_0222EE14
	.extern ov07_0222EF0C
	.extern ov07_0222EF58
	.extern ov07_0222EF88
	.extern ov07_0222F0B0
	.extern ov07_0222F210
	.extern ov07_0222F378
	.extern ov07_0222F408
	.extern ov07_0222F434
	.extern ov07_0222F4D4
	.extern ov07_0222F600
	.extern ov07_0222F6C0
	.extern ov07_0222F764
	.extern ov07_0222F848
	.extern ov07_0222F9B8
	.extern ov07_0222FA08
	.extern ov07_0222FA64
	.extern ov07_0222FB24
	.extern ov07_0222FBEC
	.extern ov07_0222FC44
	.extern ov07_0222FD14
	.extern ov07_0222FE04
	.extern ov07_0222FEB0
	.extern ov07_0222FF04
	.extern ov07_0222FFD0
	.extern ov07_02230058
	.extern ov07_02230094
	.extern ov07_02230170
	.extern ov07_022301C4
	.extern ov07_022302A8
	.extern ov07_022303A4
	.extern ov07_0223049C
	.extern ov07_02230600
	.extern ov07_0223061C
	.extern ov07_022306D8
	.extern ov07_02230714
	.extern ov07_022308E4
	.extern ov07_02230960
	.extern ov07_02230994
	.extern ov07_02230A70
	.extern ov07_02230BA0
	.extern ov07_02230BF8
	.extern ov07_02230D1C
	.extern ov07_02230D74
	.extern ov07_02230E20
	.extern ov07_02230F0C
	.extern ov07_02230F6C
	.extern ov07_02230F98
	.extern ov07_02230FE8
	.extern ov07_022310E0
	.extern ov07_022311B0
	.extern ov07_02231204
	.extern ov07_02231234
	.extern ov07_022312A8
	.extern ov07_02231300
	.extern ov07_02231378
	.extern ov07_02231468
	.extern ov07_02231514
	.extern ov07_0223151C
	.extern ov07_02231524
	.extern ov07_022315C0
	.extern ov07_0223172C
	.extern ov07_0223174C
	.extern ov07_02231874
	.extern ov07_022318A0
	.extern ov07_022318CC
	.extern ov07_022318F8
	.extern ov07_02231924
	.extern ov07_0223192C
	.extern ov07_02231958
	.extern ov07_0223197C
	.extern ov07_022319E0
	.extern ov07_02231A20
	.extern ov07_02231A50
	.extern ov07_02231A70
	.extern ov07_02231AD0
	.extern ov07_02231AE0
	.extern ov07_02231AF0
	.extern ov07_02231B00
	.extern ov07_02231B10
	.extern ov07_02231B20
	.extern ov07_02231B30
	.extern ov07_02231B40
	.extern ov07_02231B50
	.extern ov07_02231B60
	.extern ov07_02231B70
	.extern ov07_02231B80
	.extern ov07_02231B90
	.extern ov07_02231BC0
	.extern ov07_02231BF0
	.extern ov07_02231C20
	.extern ov07_02231C50
	.extern ov07_02231C80
	.extern ov07_02231CB0
	.extern ov07_02231CE0
	.extern ov07_02231D10
	.extern ov07_02231D40
	.extern ov07_02231D70
	.extern ov07_02231DA0
	.extern ov07_02231DD0
	.extern ov07_02231DE0
	.extern ov07_02231DE8
	.extern ov07_02231E08
	.extern ov07_02231E44
	.extern ov07_02231E74
	.extern ov07_02231EC0
	.extern ov07_02231FA0
	.extern ov07_02231FD8
	.extern ov07_02231FE4
	.extern ov07_02232020
	.extern ov07_022323FC
	.extern ov07_022324D8
	.extern ov07_02232508
	.extern ov07_0223251C
	.extern ov07_02232540
	.extern ov07_02232580
	.extern ov07_022325BC
	.extern ov07_022325F4
	.extern ov07_02232608
	.extern ov07_0223261C
	.extern ov07_02232630
	.extern ov07_02232644
	.extern ov07_02232658
	.extern ov07_02232670
	.extern ov07_02232694
	.extern ov07_022326C0
	.extern ov07_02232730
	.extern ov07_022329B0
	.extern ov07_02232A04
	.extern ov07_02232A14
	.extern ov07_02232A44
	.extern ov07_02232A54
	.extern ov07_02232A5C
	.extern ov07_02232AB8
	.extern ov07_02232AC0
	.extern ov07_02232AFC
	.extern ov07_02232BB0
	.extern ov07_02232C28
	.extern ov07_02232C64
	.extern ov07_02232CD8
	.extern ov07_02232D20
	.extern ov07_02232D80
	.extern ov07_02232DF4
	.extern ov07_02232E10
	.extern ov07_02232E18
	.extern ov07_02232E40
	.extern ov07_02232E68
	.extern ov07_02232E90
	.extern ov07_02232EB8
	.extern ov07_02232EE0
	.extern ov07_02232F08
	.extern ov07_02232F30
	.extern UnkBallData_SetBallAnimation
	.extern ov07_02232F60
	.extern ov07_02232F74
	.extern ov07_02232F80
	.extern ov07_02232F84
	.extern ov07_02232F9C
	.extern ov07_02232FA8
	.extern ov07_02233088
	.extern ov07_0223308C
	.extern ov07_022330E0
	.extern ov07_022330E4
	.extern ov07_022330F0
	.extern ov07_022330FC
	.extern ov07_022331A4
	.extern ov07_022331B0
	.extern ov07_02233224
	.extern ov07_02233228
	.extern ov07_022332C4
	.extern ov07_022332C8
	.extern ov07_022332E8
	.extern ov07_022332F4
	.extern ov07_0223338C
	.extern ov07_02233390
	.extern ov07_0223339C
	.extern ov07_022333A8
	.extern ov07_02233498
	.extern ov07_0223349C
	.extern ov07_02233500
	.extern ov07_0223350C
	.extern ov07_02233590
	.extern ov07_0223359C
	.extern ov07_022335A0
	.extern ov07_022335B4
	.extern ov07_02233874
	.extern ov07_02233B04
	.extern ov07_02233C98


	thumb_func_start ov07_02220CAC
ov07_02220CAC: ; 0x02220CAC
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x148
	str r0, [sp]
	str r1, [sp, #4]
	ldr r0, [r1]
	ldr r1, [r1, #0x24]
	bl ov07_02231924
	str r0, [sp, #0x18]
	ldr r0, [sp, #4]
	ldr r1, [sp, #4]
	ldr r0, [r0]
	ldr r1, [r1, #0x28]
	bl ov07_02231924
	str r0, [sp, #0x14]
	ldr r0, [sp, #4]
	ldr r0, [r0, #0x18]
	cmp r0, #0x1a
	bls _02220CD6
	b _022214D0
_02220CD6:
	add r0, r0, r0
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_02220CE2: ; jump table
	.short _02220D18 - _02220CE2 - 2 ; case 0
	.short _02220D24 - _02220CE2 - 2 ; case 1
	.short _02220D24 - _02220CE2 - 2 ; case 2
	.short _0222143A - _02220CE2 - 2 ; case 3
	.short _02220D8C - _02220CE2 - 2 ; case 4
	.short _02220E46 - _02220CE2 - 2 ; case 5
	.short _02221178 - _02220CE2 - 2 ; case 6
	.short _02221178 - _02220CE2 - 2 ; case 7
	.short _02220F00 - _02220CE2 - 2 ; case 8
	.short _02220F00 - _02220CE2 - 2 ; case 9
	.short _02220F68 - _02220CE2 - 2 ; case 10
	.short _02220F68 - _02220CE2 - 2 ; case 11
	.short _02220FB6 - _02220CE2 - 2 ; case 12
	.short _02220FB6 - _02220CE2 - 2 ; case 13
	.short _02220FFE - _02220CE2 - 2 ; case 14
	.short _02220FFE - _02220CE2 - 2 ; case 15
	.short _02221046 - _02220CE2 - 2 ; case 16
	.short _02221046 - _02220CE2 - 2 ; case 17
	.short _022210E8 - _02220CE2 - 2 ; case 18
	.short _022210E8 - _02220CE2 - 2 ; case 19
	.short _02221130 - _02220CE2 - 2 ; case 20
	.short _02221130 - _02220CE2 - 2 ; case 21
	.short _0222144E - _02220CE2 - 2 ; case 22
	.short _022214D0 - _02220CE2 - 2 ; case 23
	.short _022214A2 - _02220CE2 - 2 ; case 24
	.short _0222145E - _02220CE2 - 2 ; case 25
	.short _02221470 - _02220CE2 - 2 ; case 26
_02220D18:
	mov r1, #0
	add r0, sp, #0x20
	strh r1, [r0]
	strh r1, [r0, #2]
	strh r1, [r0, #4]
	b _022214D0
_02220D24:
	ldr r0, [sp, #4]
	ldr r0, [r0]
	bl ov07_0221BFC0
	cmp r0, #1
	bne _02220D5E
	ldr r0, [sp, #4]
	ldr r0, [r0, #0x10]
	cmp r0, #2
	add r0, sp, #0x20
	bne _02220D4C
	ldr r1, _02221080 ; =0xFFFFF2F8
	strh r1, [r0]
	mov r1, #0x73
	lsl r1, r1, #4
	strh r1, [r0, #2]
	mov r1, #0x2e
	lsl r1, r1, #4
	strh r1, [r0, #4]
	b _022214D0
_02220D4C:
	mov r1, #0x92
	lsl r1, r1, #4
	strh r1, [r0]
	ldr r1, _02221084 ; =0xFFFFFA60
	strh r1, [r0, #2]
	mov r1, #0x2e
	lsl r1, r1, #4
	strh r1, [r0, #4]
	b _022214D0
_02220D5E:
	ldr r0, [sp, #0x14]
	mov r1, #0xc
	mul r1, r0
	ldr r2, [sp, #0x18]
	mov r0, #0x48
	mul r0, r2
	ldr r2, _02221088 ; =ov07_02235CE0
	add r2, r2, r0
	ldr r3, [r1, r2]
	add r2, sp, #0x20
	strh r3, [r2]
	ldr r3, _0222108C ; =ov07_02235CE4
	add r3, r3, r0
	ldr r3, [r1, r3]
	strh r3, [r2, #2]
	ldr r3, _02221090 ; =ov07_02235CE8
	add r0, r3, r0
	ldr r1, [r1, r0]
	lsr r0, r1, #0x1f
	add r0, r1, r0
	asr r0, r0, #1
	strh r0, [r2, #4]
	b _022214D0
_02220D8C:
	ldr r0, [sp, #4]
	ldr r0, [r0]
	bl ov07_0221BFC0
	cmp r0, #1
	bne _02220DC8
	ldr r0, [sp, #0x14]
	mov r1, #0xc
	mul r1, r0
	ldr r2, [sp, #0x18]
	mov r0, #0x48
	mul r0, r2
	ldr r2, _02221088 ; =ov07_02235CE0
	add r2, r2, r0
	ldr r2, [r1, r2]
	neg r3, r2
	add r2, sp, #0x20
	strh r3, [r2]
	ldr r3, _0222108C ; =ov07_02235CE4
	add r3, r3, r0
	ldr r3, [r1, r3]
	strh r3, [r2, #2]
	ldr r3, _02221090 ; =ov07_02235CE8
	add r0, r3, r0
	ldr r1, [r1, r0]
	lsr r0, r1, #0x1f
	add r0, r1, r0
	asr r0, r0, #1
	strh r0, [r2, #4]
	b _022214D0
_02220DC8:
	ldr r0, [sp, #4]
	ldr r1, [sp, #4]
	ldr r0, [r0]
	ldr r1, [r1, #0x24]
	bl ov07_0223192C
	add r4, r0, #0
	ldr r0, [sp, #4]
	ldr r1, [sp, #4]
	ldr r0, [r0]
	ldr r1, [r1, #0x28]
	bl ov07_0223192C
	cmp r4, r0
	bne _02220E14
	ldr r0, [sp, #0x14]
	mov r1, #0xc
	mul r1, r0
	ldr r2, [sp, #0x18]
	mov r0, #0x48
	mul r0, r2
	ldr r2, _02221088 ; =ov07_02235CE0
	add r2, r2, r0
	ldr r3, [r1, r2]
	add r2, sp, #0x20
	strh r3, [r2]
	ldr r3, _0222108C ; =ov07_02235CE4
	add r3, r3, r0
	ldr r3, [r1, r3]
	strh r3, [r2, #2]
	ldr r3, _02221090 ; =ov07_02235CE8
	add r0, r3, r0
	ldr r1, [r1, r0]
	lsr r0, r1, #0x1f
	add r0, r1, r0
	asr r0, r0, #1
	strh r0, [r2, #4]
	b _022214D0
_02220E14:
	ldr r0, [sp, #4]
	ldr r1, [sp, #4]
	ldr r0, [r0]
	ldr r1, [r1, #0x24]
	bl ov07_0223192C
	cmp r0, #3
	add r0, sp, #0x20
	bne _02220E38
	mov r1, #0x3b
	lsl r1, r1, #6
	strh r1, [r0]
	mov r1, #0x21
	lsl r1, r1, #6
	strh r1, [r0, #2]
	ldr r1, _02221094 ; =0x000005FC
	strh r1, [r0, #4]
	b _022214D0
_02220E38:
	ldr r1, _02221098 ; =0xFFFFEF7C
	strh r1, [r0]
	ldr r1, _0222109C ; =0xFFFFF558
	strh r1, [r0, #2]
	ldr r1, _02221094 ; =0x000005FC
	strh r1, [r0, #4]
	b _022214D0
_02220E46:
	ldr r0, [sp, #4]
	ldr r0, [r0]
	bl ov07_0221BFC0
	cmp r0, #1
	bne _02220E82
	ldr r0, [sp, #0x14]
	mov r1, #0xc
	mul r1, r0
	ldr r2, [sp, #0x18]
	mov r0, #0x48
	mul r0, r2
	ldr r2, _02221088 ; =ov07_02235CE0
	add r2, r2, r0
	ldr r2, [r1, r2]
	neg r3, r2
	add r2, sp, #0x20
	strh r3, [r2]
	ldr r3, _0222108C ; =ov07_02235CE4
	add r3, r3, r0
	ldr r3, [r1, r3]
	strh r3, [r2, #2]
	ldr r3, _02221090 ; =ov07_02235CE8
	add r0, r3, r0
	ldr r1, [r1, r0]
	lsr r0, r1, #0x1f
	add r0, r1, r0
	asr r0, r0, #1
	strh r0, [r2, #4]
	b _022214D0
_02220E82:
	ldr r0, [sp, #4]
	ldr r1, [sp, #4]
	ldr r0, [r0]
	ldr r1, [r1, #0x24]
	bl ov07_0223192C
	add r4, r0, #0
	ldr r0, [sp, #4]
	ldr r1, [sp, #4]
	ldr r0, [r0]
	ldr r1, [r1, #0x28]
	bl ov07_0223192C
	cmp r4, r0
	bne _02220ECE
	ldr r0, [sp, #0x14]
	mov r1, #0xc
	mul r1, r0
	ldr r2, [sp, #0x18]
	mov r0, #0x48
	mul r0, r2
	ldr r2, _02221088 ; =ov07_02235CE0
	add r2, r2, r0
	ldr r3, [r1, r2]
	add r2, sp, #0x20
	strh r3, [r2]
	ldr r3, _0222108C ; =ov07_02235CE4
	add r3, r3, r0
	ldr r3, [r1, r3]
	strh r3, [r2, #2]
	ldr r3, _02221090 ; =ov07_02235CE8
	add r0, r3, r0
	ldr r1, [r1, r0]
	lsr r0, r1, #0x1f
	add r0, r1, r0
	asr r0, r0, #1
	strh r0, [r2, #4]
	b _022214D0
_02220ECE:
	ldr r0, [sp, #4]
	ldr r1, [sp, #4]
	ldr r0, [r0]
	ldr r1, [r1, #0x24]
	bl ov07_0223192C
	cmp r0, #3
	add r0, sp, #0x20
	bne _02220EF2
	mov r1, #0x3b
	lsl r1, r1, #6
	strh r1, [r0]
	mov r1, #0x21
	lsl r1, r1, #6
	strh r1, [r0, #2]
	ldr r1, _02221094 ; =0x000005FC
	strh r1, [r0, #4]
	b _022214D0
_02220EF2:
	ldr r1, _02221098 ; =0xFFFFEF7C
	strh r1, [r0]
	ldr r1, _0222109C ; =0xFFFFF558
	strh r1, [r0, #2]
	ldr r1, _02221094 ; =0x000005FC
	strh r1, [r0, #4]
	b _022214D0
_02220F00:
	ldr r0, [sp, #4]
	ldr r0, [r0]
	bl ov07_0221BFC0
	cmp r0, #1
	bne _02220F3A
	ldr r0, [sp, #4]
	ldr r0, [r0, #0x10]
	cmp r0, #2
	add r0, sp, #0x20
	bne _02220F28
	ldr r1, _022210A0 ; =0xFFFFF6E0
	strh r1, [r0]
	mov r1, #0x5a
	lsl r1, r1, #4
	strh r1, [r0, #2]
	mov r1, #0x2e
	lsl r1, r1, #4
	strh r1, [r0, #4]
	b _022214D0
_02220F28:
	mov r1, #0x92
	lsl r1, r1, #4
	strh r1, [r0]
	ldr r1, _02221084 ; =0xFFFFFA60
	strh r1, [r0, #2]
	mov r1, #0x2e
	lsl r1, r1, #4
	strh r1, [r0, #4]
	b _022214D0
_02220F3A:
	ldr r0, [sp, #0x14]
	mov r1, #0xc
	mul r1, r0
	ldr r2, [sp, #0x18]
	mov r0, #0x48
	mul r0, r2
	ldr r2, _022210A4 ; =ov07_02235620
	add r2, r2, r0
	ldr r3, [r1, r2]
	add r2, sp, #0x20
	strh r3, [r2]
	ldr r3, _022210A8 ; =ov07_02235624
	add r3, r3, r0
	ldr r3, [r1, r3]
	strh r3, [r2, #2]
	ldr r3, _022210AC ; =ov07_02235628
	add r0, r3, r0
	ldr r1, [r1, r0]
	lsr r0, r1, #0x1f
	add r0, r1, r0
	asr r0, r0, #1
	strh r0, [r2, #4]
	b _022214D0
_02220F68:
	ldr r0, [sp, #4]
	ldr r0, [r0]
	bl ov07_0221BFC0
	cmp r0, #1
	bne _02220F88
	ldr r1, _022210A0 ; =0xFFFFF6E0
	add r0, sp, #0x20
	strh r1, [r0]
	mov r1, #0x5a
	lsl r1, r1, #4
	strh r1, [r0, #2]
	mov r1, #0x2e
	lsl r1, r1, #4
	strh r1, [r0, #4]
	b _022214D0
_02220F88:
	ldr r0, [sp, #0x14]
	mov r1, #0xc
	mul r1, r0
	ldr r2, [sp, #0x18]
	mov r0, #0x48
	mul r0, r2
	ldr r2, _022210B0 ; =ov07_022357D0
	add r2, r2, r0
	ldr r3, [r1, r2]
	add r2, sp, #0x20
	strh r3, [r2]
	ldr r3, _022210A8 ; =ov07_02235624
	add r3, r3, r0
	ldr r3, [r1, r3]
	strh r3, [r2, #2]
	ldr r3, _022210AC ; =ov07_02235628
	add r0, r3, r0
	ldr r1, [r1, r0]
	lsr r0, r1, #0x1f
	add r0, r1, r0
	asr r0, r0, #1
	strh r0, [r2, #4]
	b _022214D0
_02220FB6:
	ldr r0, [sp, #4]
	ldr r0, [r0]
	bl ov07_0221BFC0
	cmp r0, #1
	bne _02220FD6
	ldr r1, _022210B4 ; =0xFFFFF488
	add r0, sp, #0x20
	strh r1, [r0]
	mov r1, #0x5a
	lsl r1, r1, #4
	strh r1, [r0, #2]
	mov r1, #0x2e
	lsl r1, r1, #4
	strh r1, [r0, #4]
	b _022214D0
_02220FD6:
	ldr r0, [sp, #0x14]
	mov r1, #0xc
	mul r1, r0
	ldr r2, [sp, #0x18]
	mov r0, #0x48
	mul r0, r2
	ldr r2, _022210B8 ; =ov07_02235B30
	add r2, r2, r0
	ldr r3, [r1, r2]
	add r2, sp, #0x20
	strh r3, [r2]
	ldr r3, _022210BC ; =ov07_02235B34
	add r3, r3, r0
	ldr r3, [r1, r3]
	strh r3, [r2, #2]
	ldr r3, _022210C0 ; =ov07_02235B38
	add r0, r3, r0
	ldr r0, [r1, r0]
	strh r0, [r2, #4]
	b _022214D0
_02220FFE:
	ldr r0, [sp, #4]
	ldr r0, [r0]
	bl ov07_0221BFC0
	cmp r0, #1
	bne _0222101E
	ldr r1, _022210A0 ; =0xFFFFF6E0
	add r0, sp, #0x20
	strh r1, [r0]
	mov r1, #0x5a
	lsl r1, r1, #4
	strh r1, [r0, #2]
	mov r1, #0x2e
	lsl r1, r1, #4
	strh r1, [r0, #4]
	b _022214D0
_0222101E:
	ldr r0, [sp, #0x14]
	mov r1, #0xc
	mul r1, r0
	ldr r2, [sp, #0x18]
	mov r0, #0x48
	mul r0, r2
	ldr r2, _022210C4 ; =ov07_02235E90
	add r2, r2, r0
	ldr r3, [r1, r2]
	add r2, sp, #0x20
	strh r3, [r2]
	ldr r3, _022210C8 ; =ov07_02235E94
	add r3, r3, r0
	ldr r3, [r1, r3]
	strh r3, [r2, #2]
	ldr r3, _022210CC ; =ov07_02235E98
	add r0, r3, r0
	ldr r0, [r1, r0]
	strh r0, [r2, #4]
	b _022214D0
_02221046:
	ldr r0, [sp, #4]
	ldr r0, [r0]
	bl ov07_0221BFC0
	cmp r0, #1
	bne _02221066
	ldr r1, _022210A0 ; =0xFFFFF6E0
	add r0, sp, #0x20
	strh r1, [r0]
	mov r1, #0x5a
	lsl r1, r1, #4
	strh r1, [r0, #2]
	mov r1, #0x2e
	lsl r1, r1, #4
	strh r1, [r0, #4]
	b _022214D0
_02221066:
	ldr r0, [sp, #0x14]
	mov r1, #0xc
	mul r1, r0
	ldr r2, [sp, #0x18]
	mov r0, #0x48
	mul r0, r2
	ldr r2, _022210D0 ; =ov07_02235980
	add r2, r2, r0
	ldr r3, [r1, r2]
	add r2, sp, #0x20
	strh r3, [r2]
	ldr r3, _022210D4 ; =ov07_02235984
	b _022210D8
	.balign 4, 0
_02221080: .word 0xFFFFF2F8
_02221084: .word 0xFFFFFA60
_02221088: .word ov07_02235CE0
_0222108C: .word ov07_02235CE4
_02221090: .word ov07_02235CE8
_02221094: .word 0x000005FC
_02221098: .word 0xFFFFEF7C
_0222109C: .word 0xFFFFF558
_022210A0: .word 0xFFFFF6E0
_022210A4: .word ov07_02235620
_022210A8: .word ov07_02235624
_022210AC: .word ov07_02235628
_022210B0: .word ov07_022357D0
_022210B4: .word 0xFFFFF488
_022210B8: .word ov07_02235B30
_022210BC: .word ov07_02235B34
_022210C0: .word ov07_02235B38
_022210C4: .word ov07_02235E90
_022210C8: .word ov07_02235E94
_022210CC: .word ov07_02235E98
_022210D0: .word ov07_02235980
_022210D4: .word ov07_02235984
_022210D8:
	add r3, r3, r0
	ldr r3, [r1, r3]
	strh r3, [r2, #2]
	ldr r3, _022213F8 ; =ov07_02235988
	add r0, r3, r0
	ldr r0, [r1, r0]
	strh r0, [r2, #4]
	b _022214D0
_022210E8:
	ldr r0, [sp, #4]
	ldr r0, [r0]
	bl ov07_0221BFC0
	cmp r0, #1
	bne _02221108
	ldr r1, _022213FC ; =0xFFFFF6E0
	add r0, sp, #0x20
	strh r1, [r0]
	mov r1, #0x5a
	lsl r1, r1, #4
	strh r1, [r0, #2]
	mov r1, #0x2e
	lsl r1, r1, #4
	strh r1, [r0, #4]
	b _022214D0
_02221108:
	ldr r0, [sp, #0x14]
	mov r1, #0xc
	mul r1, r0
	ldr r2, [sp, #0x18]
	mov r0, #0x48
	mul r0, r2
	ldr r2, _02221400 ; =ov07_02236040
	add r2, r2, r0
	ldr r3, [r1, r2]
	add r2, sp, #0x20
	strh r3, [r2]
	ldr r3, _02221404 ; =ov07_02236044
	add r3, r3, r0
	ldr r3, [r1, r3]
	strh r3, [r2, #2]
	ldr r3, _02221408 ; =ov07_02236048
	add r0, r3, r0
	ldr r0, [r1, r0]
	strh r0, [r2, #4]
	b _022214D0
_02221130:
	ldr r0, [sp, #4]
	ldr r0, [r0]
	bl ov07_0221BFC0
	cmp r0, #1
	bne _02221150
	ldr r1, _0222140C ; =0xFFFFEF10
	add r0, sp, #0x20
	strh r1, [r0]
	mov r1, #0x5a
	lsl r1, r1, #4
	strh r1, [r0, #2]
	mov r1, #0x2e
	lsl r1, r1, #4
	strh r1, [r0, #4]
	b _022214D0
_02221150:
	ldr r0, [sp, #0x14]
	mov r1, #0xc
	mul r1, r0
	ldr r2, [sp, #0x18]
	mov r0, #0x48
	mul r0, r2
	ldr r2, _02221410 ; =ov07_022361F0
	add r2, r2, r0
	ldr r3, [r1, r2]
	add r2, sp, #0x20
	strh r3, [r2]
	ldr r3, _02221414 ; =ov07_022361F4
	add r3, r3, r0
	ldr r3, [r1, r3]
	strh r3, [r2, #2]
	ldr r3, _02221418 ; =ov07_022361F8
	add r0, r3, r0
	ldr r0, [r1, r0]
	strh r0, [r2, #4]
	b _022214D0
_02221178:
	ldr r4, _0222141C ; =ov07_022355B0
	add r3, sp, #0xd8
	mov r2, #0xe
_0222117E:
	ldmia r4!, {r0, r1}
	stmia r3!, {r0, r1}
	sub r2, r2, #1
	bne _0222117E
	ldr r3, _02221420 ; =ov07_022354D0
	add r2, sp, #0x58
	ldmia r3!, {r0, r1}
	stmia r2!, {r0, r1}
	ldmia r3!, {r0, r1}
	stmia r2!, {r0, r1}
	add r1, sp, #0xc8
	mov r0, #0
	str r1, [sp, #0xc]
	add r1, sp, #0xb8
	mov ip, r0
	str r1, [sp, #0x1c]
	add r0, sp, #0xd8
	add r7, sp, #0xa8
	add r6, sp, #0x98
	add r5, sp, #0x88
	add r1, sp, #0x78
	add r2, sp, #0x68
_022211AA:
	ldr r4, [r0]
	ldr r3, [sp, #0xc]
	str r4, [r3]
	ldr r4, [r0, #0x10]
	ldr r3, [sp, #0x1c]
	str r4, [r3]
	ldr r3, [r0, #0x20]
	stmia r7!, {r3}
	ldr r3, [r0, #0x30]
	stmia r6!, {r3}
	ldr r3, [r0, #0x40]
	stmia r5!, {r3}
	ldr r3, [r0, #0x50]
	stmia r1!, {r3}
	ldr r3, [r0, #0x60]
	add r0, r0, #4
	stmia r2!, {r3}
	ldr r3, [sp, #0xc]
	add r3, r3, #4
	str r3, [sp, #0xc]
	ldr r3, [sp, #0x1c]
	add r3, r3, #4
	str r3, [sp, #0x1c]
	mov r3, ip
	add r3, r3, #1
	mov ip, r3
	cmp r3, #4
	blt _022211AA
	ldr r0, [sp, #4]
	ldr r0, [r0]
	bl ov07_0221BFC0
	cmp r0, #1
	bne _02221226
	mov r0, #0
	str r0, [sp, #8]
	add r0, sp, #0x58
	str r0, [sp, #0x10]
	add r7, sp, #0xc8
	add r6, sp, #0xb8
	add r0, sp, #0xa8
	add r1, sp, #0x98
	add r2, sp, #0x88
	add r3, sp, #0x78
	add r4, sp, #0x68
_02221204:
	ldr r5, [sp, #0x10]
	ldr r5, [r5]
	stmia r7!, {r5}
	stmia r6!, {r5}
	stmia r0!, {r5}
	stmia r1!, {r5}
	stmia r2!, {r5}
	stmia r3!, {r5}
	stmia r4!, {r5}
	ldr r5, [sp, #0x10]
	add r5, r5, #4
	str r5, [sp, #0x10]
	ldr r5, [sp, #8]
	add r5, r5, #1
	str r5, [sp, #8]
	cmp r5, #4
	blt _02221204
_02221226:
	ldr r0, [sp, #0x18]
	cmp r0, #5
	bhi _02221244
	add r0, r0, r0
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_02221238: ; jump table
	.short _02221244 - _02221238 - 2 ; case 0
	.short _02221254 - _02221238 - 2 ; case 1
	.short _02221280 - _02221238 - 2 ; case 2
	.short _022212B8 - _02221238 - 2 ; case 3
	.short _02221346 - _02221238 - 2 ; case 4
	.short _0222137E - _02221238 - 2 ; case 5
_02221244:
	ldr r1, [sp, #0xc8]
	add r0, sp, #0x20
	strh r1, [r0]
	ldr r1, [sp, #0xcc]
	strh r1, [r0, #2]
	ldr r1, [sp, #0xd0]
	strh r1, [r0, #4]
	b _022214D0
_02221254:
	ldr r0, [sp, #0xd4]
	cmp r0, #1
	bne _0222126A
	ldr r1, [sp, #0xc8]
	add r0, sp, #0x20
	strh r1, [r0]
	ldr r1, [sp, #0xcc]
	strh r1, [r0, #2]
	ldr r1, [sp, #0xd0]
	strh r1, [r0, #4]
	b _022214D0
_0222126A:
	ldr r0, [sp, #0xc8]
	neg r1, r0
	add r0, sp, #0x20
	strh r1, [r0]
	ldr r1, [sp, #0xcc]
	neg r1, r1
	strh r1, [r0, #2]
	ldr r1, [sp, #0xd0]
	neg r1, r1
	strh r1, [r0, #4]
	b _022214D0
_02221280:
	ldr r0, [sp, #0x14]
	cmp r0, #3
	bne _02221296
	ldr r1, [sp, #0xb8]
	add r0, sp, #0x20
	strh r1, [r0]
	ldr r1, [sp, #0xbc]
	strh r1, [r0, #2]
	ldr r1, [sp, #0xc0]
	strh r1, [r0, #4]
	b _022214D0
_02221296:
	cmp r0, #5
	add r0, sp, #0x20
	bne _022212AA
	ldr r1, [sp, #0x98]
	strh r1, [r0]
	ldr r1, [sp, #0x9c]
	strh r1, [r0, #2]
	ldr r1, [sp, #0xa0]
	strh r1, [r0, #4]
	b _022214D0
_022212AA:
	ldr r1, [sp, #0xa8]
	strh r1, [r0]
	ldr r1, [sp, #0xac]
	strh r1, [r0, #2]
	ldr r1, [sp, #0xb0]
	strh r1, [r0, #4]
	b _022214D0
_022212B8:
	ldr r0, [sp, #0x14]
	cmp r0, #2
	bne _022212EA
	ldr r0, [sp, #0xc4]
	cmp r0, #1
	bne _022212D4
	ldr r1, [sp, #0xb8]
	add r0, sp, #0x20
	strh r1, [r0]
	ldr r1, [sp, #0xbc]
	strh r1, [r0, #2]
	ldr r1, [sp, #0xc0]
	strh r1, [r0, #4]
	b _022214D0
_022212D4:
	ldr r0, [sp, #0xb8]
	neg r1, r0
	add r0, sp, #0x20
	strh r1, [r0]
	ldr r1, [sp, #0xbc]
	neg r1, r1
	strh r1, [r0, #2]
	ldr r1, [sp, #0xc0]
	neg r1, r1
	strh r1, [r0, #4]
	b _022214D0
_022212EA:
	cmp r0, #5
	bne _0222131A
	ldr r0, [sp, #0xb4]
	cmp r0, #1
	bne _0222130A
	ldr r0, [sp, #0x88]
	neg r1, r0
	add r0, sp, #0x20
	strh r1, [r0]
	ldr r1, [sp, #0x8c]
	neg r1, r1
	strh r1, [r0, #2]
	ldr r1, [sp, #0x90]
	neg r1, r1
	strh r1, [r0, #4]
	b _022214D0
_0222130A:
	ldr r1, [sp, #0x88]
	add r0, sp, #0x20
	strh r1, [r0]
	ldr r1, [sp, #0x8c]
	strh r1, [r0, #2]
	ldr r1, [sp, #0x90]
	strh r1, [r0, #4]
	b _022214D0
_0222131A:
	ldr r0, [sp, #0x84]
	cmp r0, #1
	bne _02221330
	ldr r1, [sp, #0x78]
	add r0, sp, #0x20
	strh r1, [r0]
	ldr r1, [sp, #0x7c]
	strh r1, [r0, #2]
	ldr r1, [sp, #0x80]
	strh r1, [r0, #4]
	b _022214D0
_02221330:
	ldr r0, [sp, #0x78]
	neg r1, r0
	add r0, sp, #0x20
	strh r1, [r0]
	ldr r1, [sp, #0x7c]
	neg r1, r1
	strh r1, [r0, #2]
	ldr r1, [sp, #0x80]
	neg r1, r1
	strh r1, [r0, #4]
	b _022214D0
_02221346:
	ldr r0, [sp, #0x14]
	cmp r0, #3
	bne _0222135C
	ldr r1, [sp, #0x78]
	add r0, sp, #0x20
	strh r1, [r0]
	ldr r1, [sp, #0x7c]
	strh r1, [r0, #2]
	ldr r1, [sp, #0x80]
	strh r1, [r0, #4]
	b _022214D0
_0222135C:
	cmp r0, #5
	add r0, sp, #0x20
	bne _02221370
	ldr r1, [sp, #0x68]
	strh r1, [r0]
	ldr r1, [sp, #0x6c]
	strh r1, [r0, #2]
	ldr r1, [sp, #0x70]
	strh r1, [r0, #4]
	b _022214D0
_02221370:
	ldr r1, [sp, #0x88]
	strh r1, [r0]
	ldr r1, [sp, #0x8c]
	strh r1, [r0, #2]
	ldr r1, [sp, #0x90]
	strh r1, [r0, #4]
	b _022214D0
_0222137E:
	ldr r0, [sp, #0x14]
	cmp r0, #3
	bne _022213B0
	ldr r0, [sp, #0xc4]
	cmp r0, #1
	bne _0222139A
	ldr r1, [sp, #0x88]
	add r0, sp, #0x20
	strh r1, [r0]
	ldr r1, [sp, #0x8c]
	strh r1, [r0, #2]
	ldr r1, [sp, #0x90]
	strh r1, [r0, #4]
	b _022214D0
_0222139A:
	ldr r0, [sp, #0x88]
	neg r1, r0
	add r0, sp, #0x20
	strh r1, [r0]
	ldr r1, [sp, #0x8c]
	neg r1, r1
	strh r1, [r0, #2]
	ldr r1, [sp, #0x90]
	neg r1, r1
	strh r1, [r0, #4]
	b _022214D0
_022213B0:
	cmp r0, #2
	bne _022213E0
	ldr r0, [sp, #0xc4]
	cmp r0, #1
	bne _022213CA
	ldr r1, [sp, #0x98]
	add r0, sp, #0x20
	strh r1, [r0]
	ldr r1, [sp, #0x9c]
	strh r1, [r0, #2]
	ldr r1, [sp, #0xa0]
	strh r1, [r0, #4]
	b _022214D0
_022213CA:
	ldr r0, [sp, #0x98]
	neg r1, r0
	add r0, sp, #0x20
	strh r1, [r0]
	ldr r1, [sp, #0x9c]
	neg r1, r1
	strh r1, [r0, #2]
	ldr r1, [sp, #0xa0]
	neg r1, r1
	strh r1, [r0, #4]
	b _022214D0
_022213E0:
	ldr r0, [sp, #0xc4]
	cmp r0, #1
	bne _02221424
	ldr r1, [sp, #0x68]
	add r0, sp, #0x20
	strh r1, [r0]
	ldr r1, [sp, #0x6c]
	strh r1, [r0, #2]
	ldr r1, [sp, #0x70]
	strh r1, [r0, #4]
	b _022214D0
	nop
_022213F8: .word ov07_02235988
_022213FC: .word 0xFFFFF6E0
_02221400: .word ov07_02236040
_02221404: .word ov07_02236044
_02221408: .word ov07_02236048
_0222140C: .word 0xFFFFEF10
_02221410: .word ov07_022361F0
_02221414: .word ov07_022361F4
_02221418: .word ov07_022361F8
_0222141C: .word ov07_022355B0
_02221420: .word ov07_022354D0
_02221424:
	ldr r0, [sp, #0x68]
	neg r1, r0
	add r0, sp, #0x20
	strh r1, [r0]
	ldr r1, [sp, #0x6c]
	neg r1, r1
	strh r1, [r0, #2]
	ldr r1, [sp, #0x70]
	neg r1, r1
	strh r1, [r0, #4]
	b _022214D0
_0222143A:
	ldr r1, _022214F0 ; =0xFFFFFCE0
	add r0, sp, #0x20
	strh r1, [r0]
	mov r1, #0x4b
	lsl r1, r1, #4
	strh r1, [r0, #2]
	mov r1, #0x7d
	lsl r1, r1, #2
	strh r1, [r0, #4]
	b _022214D0
_0222144E:
	ldr r1, _022214F4 ; =0xFFFFF2AE
	add r0, sp, #0x20
	strh r1, [r0]
	ldr r1, _022214F8 ; =0xFFFFF5AC
	strh r1, [r0, #2]
	mov r1, #0
	strh r1, [r0, #4]
	b _022214D0
_0222145E:
	ldr r1, _022214FC ; =0xFFFFF290
	add r0, sp, #0x20
	strh r1, [r0]
	mov r1, #0x7a
	lsl r1, r1, #4
	strh r1, [r0, #2]
	mov r1, #0
	strh r1, [r0, #4]
	b _022214D0
_02221470:
	ldr r0, [sp, #4]
	ldr r1, [sp, #4]
	ldr r0, [r0]
	ldr r1, [r1, #0x24]
	bl ov07_0223192C
	cmp r0, #3
	add r0, sp, #0x20
	bne _02221494
	mov r1, #0x3b
	lsl r1, r1, #6
	strh r1, [r0]
	mov r1, #0x21
	lsl r1, r1, #6
	strh r1, [r0, #2]
	mov r1, #0
	strh r1, [r0, #4]
	b _022214D0
_02221494:
	ldr r1, _02221500 ; =0xFFFFE890
	strh r1, [r0]
	ldr r1, _02221504 ; =0xFFFFF768
	strh r1, [r0, #2]
	mov r1, #0
	strh r1, [r0, #4]
	b _022214D0
_022214A2:
	ldr r4, _02221508 ; =ov07_02235510
	add r3, sp, #0x28
	mov r2, #6
_022214A8:
	ldmia r4!, {r0, r1}
	stmia r3!, {r0, r1}
	sub r2, r2, #1
	bne _022214A8
	ldr r0, [sp, #4]
	ldr r1, [sp, #4]
	ldr r0, [r0]
	ldr r1, [r1, #0x24]
	bl ov07_02231924
	lsl r2, r0, #3
	add r0, sp, #0x28
	ldr r1, [r0, r2]
	add r0, sp, #0x20
	strh r1, [r0]
	add r1, sp, #0x2c
	ldr r1, [r1, r2]
	strh r1, [r0, #2]
	mov r1, #0
	strh r1, [r0, #4]
_022214D0:
	add r1, sp, #0x20
	ldr r0, [sp]
	ldrh r2, [r1]
	add r0, #0x50
	strh r2, [r0]
	ldr r0, [sp]
	ldrh r2, [r1, #2]
	add r0, #0x52
	strh r2, [r0]
	ldr r0, [sp]
	ldrh r1, [r1, #4]
	add r0, #0x54
	str r0, [sp]
	strh r1, [r0]
	add sp, #0x148
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_022214F0: .word 0xFFFFFCE0
_022214F4: .word 0xFFFFF2AE
_022214F8: .word 0xFFFFF5AC
_022214FC: .word 0xFFFFF290
_02221500: .word 0xFFFFE890
_02221504: .word 0xFFFFF768
_02221508: .word ov07_02235510
	thumb_func_end ov07_02220CAC
