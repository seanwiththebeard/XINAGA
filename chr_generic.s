;.incbin "tileset.bin"

.segment "CODEB_0"
;; Export path: CHR ROM BIN -> CharPad -> Export ASM
;;{w:8,h:8,bpp:1,count:512,brev:1,np:2,pofs:8,remap:[0,1,2,4,5,6,7,8,9,10,11,12]};;
charset_data:
.byte $00,$07,$0C,$0A,$0B,$0D,$1B,$10,$00,$07,$0C,$0A,$0B,$0D,$1B,$10
.byte $00,$E0,$30,$10,$10,$B0,$D8,$08,$00,$E0,$30,$10,$10,$B0,$D8,$08
.byte $00,$03,$07,$07,$05,$06,$09,$18,$00,$03,$07,$07,$05,$06,$09,$18
.byte $00,$C0,$E0,$E0,$A0,$60,$90,$18,$00,$C0,$E0,$E0,$A0,$60,$90,$18
.byte $00,$1F,$2F,$08,$0B,$09,$0A,$18,$00,$1F,$2F,$08,$0B,$09,$0A,$18
.byte $00,$E4,$F6,$14,$D6,$92,$52,$1A,$00,$E4,$F6,$14,$D6,$92,$52,$1A
.byte $03,$17,$17,$10,$16,$07,$07,$00,$03,$17,$17,$10,$16,$07,$07,$00
.byte $C0,$E8,$E8,$08,$68,$E0,$E0,$00,$C0,$E8,$E8,$08,$68,$E0,$E0,$00
.byte $00,$07,$0F,$0C,$08,$09,$0A,$1B,$00,$07,$0F,$0C,$08,$09,$0A,$1B
.byte $00,$E0,$F0,$30,$10,$90,$50,$D8,$00,$E0,$F0,$30,$10,$90,$50,$D8
.byte $00,$03,$07,$07,$05,$17,$29,$24,$00,$03,$07,$07,$05,$17,$29,$24
.byte $00,$C0,$E0,$E0,$A0,$E8,$94,$24,$00,$C0,$E0,$E0,$A0,$E8,$94,$24
.byte $00,$03,$04,$03,$05,$07,$03,$18,$00,$03,$04,$03,$05,$07,$03,$18
.byte $00,$C0,$20,$C0,$A0,$E0,$C0,$18,$00,$C0,$20,$C0,$A0,$E0,$C0,$18
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $3E,$36,$06,$30,$37,$04,$04,$00,$3E,$36,$06,$30,$37,$04,$04,$00
.byte $7C,$6C,$60,$0C,$EC,$20,$20,$00,$7C,$6C,$60,$0C,$EC,$20,$20,$00
.byte $3E,$34,$34,$00,$17,$04,$04,$00,$3E,$34,$34,$00,$17,$04,$04,$00
.byte $7C,$2C,$2C,$00,$E8,$20,$20,$00,$7C,$2C,$2C,$00,$E8,$20,$20,$00
.byte $3C,$3E,$3F,$06,$16,$06,$06,$00,$3C,$3E,$3F,$06,$16,$06,$06,$00
.byte $3A,$70,$E6,$66,$60,$62,$62,$00,$3A,$70,$E6,$66,$60,$62,$62,$00
.byte $3E,$36,$06,$30,$37,$04,$04,$00,$3E,$36,$06,$30,$37,$04,$04,$00
.byte $7C,$6C,$60,$0C,$EC,$20,$20,$00,$7C,$6C,$60,$0C,$EC,$20,$20,$00
.byte $3B,$35,$36,$36,$26,$16,$06,$00,$3B,$35,$36,$36,$26,$16,$06,$00
.byte $DC,$AC,$6C,$6C,$64,$68,$60,$00,$DC,$AC,$6C,$6C,$64,$68,$60,$00
.byte $37,$30,$27,$27,$34,$30,$04,$00,$37,$30,$27,$27,$34,$30,$04,$00
.byte $EC,$0C,$E4,$E4,$2C,$0C,$20,$00,$EC,$0C,$E4,$E4,$2C,$0C,$20,$00
.byte $3E,$36,$06,$30,$37,$04,$04,$00,$3E,$36,$06,$30,$37,$04,$04,$00
.byte $7C,$6C,$60,$0C,$EC,$20,$20,$00,$7C,$6C,$60,$0C,$EC,$20,$20,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $18,$18,$18,$18,$00,$00,$18,$00,$18,$18,$18,$18,$00,$00,$18,$00
.byte $66,$66,$66,$00,$00,$00,$00,$00,$66,$66,$66,$00,$00,$00,$00,$00
.byte $66,$66,$FF,$66,$FF,$66,$66,$00,$66,$66,$FF,$66,$FF,$66,$66,$00
.byte $18,$3E,$60,$3C,$06,$7C,$18,$00,$18,$3E,$60,$3C,$06,$7C,$18,$00
.byte $62,$66,$0C,$18,$30,$66,$46,$00,$62,$66,$0C,$18,$30,$66,$46,$00
.byte $3C,$66,$3C,$38,$67,$66,$3F,$00,$3C,$66,$3C,$38,$67,$66,$3F,$00
.byte $06,$0C,$18,$00,$00,$00,$00,$00,$06,$0C,$18,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$0C,$18,$30,$30,$30,$18,$0C,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$30,$18,$0C,$0C,$0C,$18,$30,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$66,$3C,$FF,$3C,$66,$00,$00
.byte $00,$18,$18,$7E,$18,$18,$00,$00,$00,$18,$18,$7E,$18,$18,$00,$00
.byte $00,$00,$00,$00,$00,$18,$18,$30,$00,$00,$00,$00,$00,$18,$18,$30
.byte $00,$00,$00,$7E,$00,$00,$00,$00,$00,$00,$00,$7E,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$18,$18,$00,$00,$00,$00,$00,$00,$18,$18,$00
.byte $00,$03,$06,$0C,$18,$30,$60,$00,$00,$03,$06,$0C,$18,$30,$60,$00
.byte $3C,$66,$6E,$76,$66,$66,$3C,$00,$3C,$66,$6E,$76,$66,$66,$3C,$00
.byte $18,$18,$38,$18,$18,$18,$7E,$00,$18,$18,$38,$18,$18,$18,$7E,$00
.byte $3C,$66,$06,$0C,$30,$60,$7E,$00,$3C,$66,$06,$0C,$30,$60,$7E,$00
.byte $3C,$66,$06,$1C,$06,$66,$3C,$00,$3C,$66,$06,$1C,$06,$66,$3C,$00
.byte $06,$0E,$1E,$66,$7F,$06,$06,$00,$06,$0E,$1E,$66,$7F,$06,$06,$00
.byte $7E,$60,$7C,$06,$06,$66,$3C,$00,$7E,$60,$7C,$06,$06,$66,$3C,$00
.byte $3C,$66,$60,$7C,$66,$66,$3C,$00,$3C,$66,$60,$7C,$66,$66,$3C,$00
.byte $7E,$66,$0C,$18,$18,$18,$18,$00,$7E,$66,$0C,$18,$18,$18,$18,$00
.byte $3C,$66,$66,$3C,$66,$66,$3C,$00,$3C,$66,$66,$3C,$66,$66,$3C,$00
.byte $3C,$66,$66,$3E,$06,$66,$3C,$00,$3C,$66,$66,$3E,$06,$66,$3C,$00
.byte $00,$00,$18,$00,$00,$18,$00,$00,$00,$00,$18,$00,$00,$18,$00,$00
.byte $00,$00,$18,$00,$00,$18,$18,$30,$00,$00,$18,$00,$00,$18,$18,$30
.byte $00,$00,$00,$00,$00,$00,$00,$00,$0E,$18,$30,$60,$30,$18,$0E,$00
.byte $00,$00,$7E,$00,$7E,$00,$00,$00,$00,$00,$7E,$00,$7E,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$70,$18,$0C,$06,$0C,$18,$70,$00
.byte $3C,$66,$06,$0C,$18,$00,$18,$00,$3C,$66,$06,$0C,$18,$00,$18,$00
.byte $3C,$66,$6E,$6E,$60,$62,$3C,$00,$3C,$66,$6E,$6E,$60,$62,$3C,$00
.byte $18,$3C,$66,$7E,$66,$66,$66,$00,$18,$3C,$66,$7E,$66,$66,$66,$00
.byte $7C,$66,$66,$7C,$66,$66,$7C,$00,$7C,$66,$66,$7C,$66,$66,$7C,$00
.byte $3C,$66,$60,$60,$60,$66,$3C,$00,$3C,$66,$60,$60,$60,$66,$3C,$00
.byte $78,$6C,$66,$66,$66,$6C,$78,$00,$78,$6C,$66,$66,$66,$6C,$78,$00
.byte $7E,$60,$60,$78,$60,$60,$7E,$00,$7E,$60,$60,$78,$60,$60,$7E,$00
.byte $7E,$60,$60,$78,$60,$60,$60,$00,$7E,$60,$60,$78,$60,$60,$60,$00
.byte $3C,$66,$60,$6E,$66,$66,$3C,$00,$3C,$66,$60,$6E,$66,$66,$3C,$00
.byte $66,$66,$66,$7E,$66,$66,$66,$00,$66,$66,$66,$7E,$66,$66,$66,$00
.byte $3C,$18,$18,$18,$18,$18,$3C,$00,$3C,$18,$18,$18,$18,$18,$3C,$00
.byte $1E,$0C,$0C,$0C,$0C,$6C,$38,$00,$1E,$0C,$0C,$0C,$0C,$6C,$38,$00
.byte $66,$6C,$78,$70,$78,$6C,$66,$00,$66,$6C,$78,$70,$78,$6C,$66,$00
.byte $60,$60,$60,$60,$60,$60,$7E,$00,$60,$60,$60,$60,$60,$60,$7E,$00
.byte $63,$77,$7F,$6B,$63,$63,$63,$00,$63,$77,$7F,$6B,$63,$63,$63,$00
.byte $66,$76,$7E,$7E,$6E,$66,$66,$00,$66,$76,$7E,$7E,$6E,$66,$66,$00
.byte $3C,$66,$66,$66,$66,$66,$3C,$00,$3C,$66,$66,$66,$66,$66,$3C,$00
.byte $7C,$66,$66,$7C,$60,$60,$60,$00,$7C,$66,$66,$7C,$60,$60,$60,$00
.byte $3C,$66,$66,$66,$66,$3C,$0E,$00,$3C,$66,$66,$66,$66,$3C,$0E,$00
.byte $7C,$66,$66,$7C,$78,$6C,$66,$00,$7C,$66,$66,$7C,$78,$6C,$66,$00
.byte $3C,$66,$60,$3C,$06,$66,$3C,$00,$3C,$66,$60,$3C,$06,$66,$3C,$00
.byte $7E,$18,$18,$18,$18,$18,$18,$00,$7E,$18,$18,$18,$18,$18,$18,$00
.byte $66,$66,$66,$66,$66,$66,$3C,$00,$66,$66,$66,$66,$66,$66,$3C,$00
.byte $66,$66,$66,$66,$66,$3C,$18,$00,$66,$66,$66,$66,$66,$3C,$18,$00
.byte $63,$63,$63,$6B,$7F,$77,$63,$00,$63,$63,$63,$6B,$7F,$77,$63,$00
.byte $66,$66,$3C,$18,$3C,$66,$66,$00,$66,$66,$3C,$18,$3C,$66,$66,$00
.byte $66,$66,$66,$3C,$18,$18,$18,$00,$66,$66,$66,$3C,$18,$18,$18,$00
.byte $7E,$06,$0C,$18,$30,$60,$7E,$00,$7E,$06,$0C,$18,$30,$60,$7E,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $3C,$66,$6E,$6E,$60,$62,$3C,$00,$3C,$66,$6E,$6E,$60,$62,$3C,$00
.byte $00,$00,$3C,$06,$3E,$66,$3E,$00,$00,$00,$3C,$06,$3E,$66,$3E,$00
.byte $00,$60,$60,$7C,$66,$66,$7C,$00,$00,$60,$60,$7C,$66,$66,$7C,$00
.byte $00,$00,$3C,$60,$60,$60,$3C,$00,$00,$00,$3C,$60,$60,$60,$3C,$00
.byte $00,$06,$06,$3E,$66,$66,$3E,$00,$00,$06,$06,$3E,$66,$66,$3E,$00
.byte $00,$00,$3C,$66,$7E,$60,$3C,$00,$00,$00,$3C,$66,$7E,$60,$3C,$00
.byte $00,$0E,$18,$3E,$18,$18,$18,$00,$00,$0E,$18,$3E,$18,$18,$18,$00
.byte $00,$00,$3E,$66,$66,$3E,$06,$7C,$00,$00,$3E,$66,$66,$3E,$06,$7C
.byte $00,$60,$60,$7C,$66,$66,$66,$00,$00,$60,$60,$7C,$66,$66,$66,$00
.byte $00,$18,$00,$38,$18,$18,$3C,$00,$00,$18,$00,$38,$18,$18,$3C,$00
.byte $00,$06,$00,$06,$06,$06,$06,$3C,$00,$06,$00,$06,$06,$06,$06,$3C
.byte $00,$60,$60,$6C,$78,$6C,$66,$00,$00,$60,$60,$6C,$78,$6C,$66,$00
.byte $00,$38,$18,$18,$18,$18,$3C,$00,$00,$38,$18,$18,$18,$18,$3C,$00
.byte $00,$00,$66,$7F,$7F,$6B,$63,$00,$00,$00,$66,$7F,$7F,$6B,$63,$00
.byte $00,$00,$7C,$66,$66,$66,$66,$00,$00,$00,$7C,$66,$66,$66,$66,$00
.byte $00,$00,$3C,$66,$66,$66,$3C,$00,$00,$00,$3C,$66,$66,$66,$3C,$00
.byte $00,$00,$7C,$66,$66,$7C,$60,$60,$00,$00,$7C,$66,$66,$7C,$60,$60
.byte $00,$00,$3E,$66,$66,$3E,$06,$06,$00,$00,$3E,$66,$66,$3E,$06,$06
.byte $00,$00,$7C,$66,$60,$60,$60,$00,$00,$00,$7C,$66,$60,$60,$60,$00
.byte $00,$00,$3E,$60,$3C,$06,$7C,$00,$00,$00,$3E,$60,$3C,$06,$7C,$00
.byte $00,$18,$7E,$18,$18,$18,$0E,$00,$00,$18,$7E,$18,$18,$18,$0E,$00
.byte $00,$00,$66,$66,$66,$66,$3E,$00,$00,$00,$66,$66,$66,$66,$3E,$00
.byte $00,$00,$66,$66,$66,$3C,$18,$00,$00,$00,$66,$66,$66,$3C,$18,$00
.byte $00,$00,$63,$6B,$7F,$3E,$36,$00,$00,$00,$63,$6B,$7F,$3E,$36,$00
.byte $00,$00,$66,$3C,$18,$3C,$66,$00,$00,$00,$66,$3C,$18,$3C,$66,$00
.byte $00,$00,$66,$66,$66,$3E,$0C,$78,$00,$00,$66,$66,$66,$3E,$0C,$78
.byte $00,$00,$7E,$0C,$18,$30,$7E,$00,$00,$00,$7E,$0C,$18,$30,$7E,$00
.byte $3C,$30,$30,$30,$30,$30,$3C,$00,$3C,$30,$30,$30,$30,$30,$3C,$00
.byte $0C,$12,$30,$7C,$30,$62,$FC,$00,$0C,$12,$30,$7C,$30,$62,$FC,$00
.byte $3C,$0C,$0C,$0C,$0C,$0C,$3C,$00,$3C,$0C,$0C,$0C,$0C,$0C,$3C,$00
.byte $00,$18,$3C,$7E,$18,$18,$18,$18,$00,$18,$3C,$7E,$18,$18,$18,$18
.byte $00,$10,$30,$7F,$7F,$30,$10,$00,$00,$10,$30,$7F,$7F,$30,$10,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$6E,$6D,$2B,$47,$00,$47,$2B,$00,$6E,$6D,$2B,$47,$00,$47,$2B
.byte $00,$71,$6A,$5B,$3B,$00,$3B,$5B,$00,$71,$6A,$5B,$3B,$00,$3B,$5B
.byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
.byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
.byte $FF,$FE,$FF,$E0,$E0,$E7,$E0,$E7,$FF,$FF,$FF,$FF,$FF,$F8,$FF,$F8
.byte $FF,$7F,$FF,$07,$07,$E7,$07,$E7,$FF,$FF,$FF,$FF,$FF,$1F,$FF,$1F
.byte $00,$00,$08,$2A,$00,$00,$00,$00,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
.byte $00,$20,$32,$16,$04,$00,$00,$00,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
.byte $00,$7F,$7F,$7F,$40,$5A,$5A,$5A,$00,$7F,$7F,$7F,$40,$5A,$5A,$5A
.byte $00,$FE,$FE,$FE,$02,$2A,$4A,$8A,$00,$FE,$FE,$FE,$02,$2A,$4A,$8A
.byte $00,$6F,$6F,$60,$6D,$6D,$01,$6D,$00,$6F,$6F,$60,$6D,$6D,$01,$6D
.byte $00,$B6,$B6,$30,$B6,$B6,$80,$BE,$00,$B6,$B6,$30,$B6,$B6,$80,$BE
.byte $00,$00,$00,$00,$00,$7E,$7E,$7E,$00,$00,$00,$00,$00,$7E,$7E,$7E
.byte $00,$00,$78,$F8,$58,$F2,$C6,$1E,$00,$00,$78,$F8,$58,$F2,$C6,$1E
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $5B,$3B,$00,$3B,$5B,$6A,$71,$00,$5B,$3B,$00,$3B,$5B,$6A,$71,$00
.byte $6A,$71,$00,$71,$6A,$5B,$3B,$00,$6A,$71,$00,$71,$6A,$5B,$3B,$00
.byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
.byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
.byte $E0,$E0,$FF,$FC,$FC,$E4,$F0,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
.byte $07,$07,$FF,$3F,$3F,$27,$0F,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
.byte $04,$4C,$68,$20,$00,$01,$05,$00,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
.byte $08,$2A,$00,$00,$00,$00,$40,$00,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
.byte $7F,$40,$54,$52,$51,$7F,$00,$00,$7F,$40,$54,$52,$51,$7F,$00,$00
.byte $FE,$02,$5A,$5A,$5A,$FE,$00,$00,$FE,$02,$5A,$5A,$5A,$FE,$00,$00
.byte $6D,$0C,$6D,$6D,$60,$6F,$6F,$00,$6D,$0C,$6D,$6D,$60,$6F,$6F,$00
.byte $BE,$00,$F6,$F6,$06,$B6,$B6,$00,$BE,$00,$F6,$F6,$06,$B6,$B6,$00
.byte $7F,$40,$54,$52,$51,$7F,$00,$00,$7F,$40,$54,$52,$51,$7F,$00,$00
.byte $FE,$02,$5A,$5A,$5A,$FE,$00,$00,$FE,$02,$5A,$5A,$5A,$FE,$00,$00
.byte $00,$3F,$60,$5F,$50,$57,$57,$57,$00,$3F,$60,$5F,$50,$57,$57,$57
.byte $00,$FC,$06,$FA,$0A,$EA,$EA,$EA,$00,$FC,$06,$FA,$0A,$EA,$EA,$EA
.byte $FF,$80,$BF,$BF,$B0,$B0,$B0,$B0,$FF,$80,$BF,$BF,$B0,$B0,$B0,$B0
.byte $FF,$02,$FA,$F2,$12,$12,$12,$12,$FF,$02,$FA,$F2,$12,$12,$12,$12
.byte $00,$00,$00,$01,$03,$03,$37,$37,$00,$00,$00,$01,$03,$03,$37,$37
.byte $00,$00,$00,$F0,$F8,$F8,$FC,$FC,$00,$00,$00,$F0,$F8,$F8,$FC,$FC
.byte $00,$70,$70,$76,$76,$76,$76,$46,$00,$70,$70,$76,$76,$76,$76,$46
.byte $00,$00,$00,$00,$00,$C0,$C0,$D8,$00,$00,$00,$00,$00,$C0,$C0,$D8
.byte $FF,$FF,$FF,$FF,$F7,$F7,$FB,$FB,$FF,$FF,$FF,$FF,$FF,$FF,$E7,$E6
.byte $FF,$DF,$DF,$EF,$EF,$F7,$F7,$FB,$FF,$FF,$FF,$9F,$9F,$0F,$0F,$07
.byte $00,$00,$3F,$20,$2C,$2C,$2D,$2D,$00,$00,$3F,$20,$2C,$2C,$2D,$2D
.byte $00,$00,$FC,$04,$04,$04,$84,$84,$00,$00,$FC,$04,$04,$04,$84,$84
.byte $FF,$80,$BF,$A0,$AA,$A5,$AA,$A5,$FF,$80,$BF,$A0,$AA,$A5,$AA,$A5
.byte $FF,$01,$FD,$05,$A5,$55,$A5,$55,$FF,$01,$FD,$05,$A5,$55,$A5,$55
.byte $00,$78,$4A,$7B,$07,$37,$30,$00,$00,$78,$4A,$7B,$07,$37,$30,$00
.byte $00,$1E,$52,$DE,$E0,$EC,$0C,$00,$00,$1E,$52,$DE,$E0,$EC,$0C,$00
.byte $50,$5F,$50,$57,$57,$50,$5F,$00,$50,$5F,$50,$57,$57,$50,$5F,$00
.byte $0A,$DA,$0A,$EA,$EA,$0A,$FA,$00,$0A,$DA,$0A,$EA,$EA,$0A,$FA,$00
.byte $B0,$B0,$B0,$BF,$A0,$80,$FF,$80,$B0,$B0,$B0,$BF,$A0,$80,$FF,$80
.byte $12,$12,$12,$F2,$02,$02,$FE,$00,$12,$12,$12,$F2,$02,$02,$FE,$00
.byte $03,$00,$4F,$1F,$1F,$0F,$00,$00,$03,$00,$4F,$1F,$1F,$0F,$00,$00
.byte $F8,$00,$9A,$D8,$C0,$80,$00,$00,$F8,$00,$9A,$D8,$C0,$80,$00,$00
.byte $46,$40,$40,$40,$40,$40,$7F,$00,$46,$40,$40,$40,$40,$40,$7F,$00
.byte $D8,$DA,$DA,$1A,$1A,$02,$FE,$00,$D8,$DA,$DA,$1A,$1A,$02,$FE,$00
.byte $FD,$FD,$FE,$FE,$FF,$FF,$FF,$FF,$C3,$C3,$81,$81,$C1,$FF,$F7,$FF
.byte $FB,$FD,$FD,$FF,$FF,$FF,$FF,$FF,$07,$03,$83,$83,$FF,$DF,$FF,$FF
.byte $2D,$2D,$2D,$2D,$20,$3F,$00,$00,$2D,$2D,$2D,$2D,$20,$3F,$00,$00
.byte $B4,$B4,$B4,$B4,$04,$FC,$00,$00,$B4,$B4,$B4,$B4,$04,$FC,$00,$00
.byte $AA,$A5,$AA,$A5,$A0,$BF,$80,$FF,$AA,$A5,$AA,$A5,$A0,$BF,$80,$FF
.byte $A5,$55,$A5,$55,$05,$FD,$01,$FF,$A5,$55,$A5,$55,$05,$FD,$01,$FF
.byte $78,$48,$7A,$07,$37,$36,$36,$00,$78,$48,$7A,$07,$37,$36,$36,$00
.byte $1E,$12,$5E,$E0,$EC,$6C,$6C,$00,$1E,$12,$5E,$E0,$EC,$6C,$6C,$00
.byte $00,$11,$1F,$08,$11,$1F,$11,$0C,$00,$11,$1F,$08,$11,$1F,$11,$0C
.byte $00,$88,$F8,$10,$88,$F8,$88,$30,$00,$88,$F8,$10,$88,$F8,$88,$30
.byte $00,$10,$38,$45,$39,$2B,$3B,$28,$00,$10,$38,$45,$39,$2B,$3B,$28
.byte $00,$08,$1C,$A2,$9C,$D4,$DC,$14,$00,$08,$1C,$A2,$9C,$D4,$DC,$14
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $0F,$0B,$0B,$1F,$0E,$12,$0E,$00,$0F,$0B,$0B,$1F,$0E,$12,$0E,$00
.byte $F0,$D0,$D0,$F8,$70,$48,$70,$00,$F0,$D0,$D0,$F8,$70,$48,$70,$00
.byte $10,$38,$44,$38,$2B,$3A,$3A,$00,$10,$38,$44,$38,$2B,$3A,$3A,$00
.byte $08,$1C,$22,$1C,$D4,$5C,$5C,$00,$08,$1C,$22,$1C,$D4,$5C,$5C,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $FF,$FF,$DF,$AF,$FF,$FD,$FA,$FF,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$FF,$FF,$EF,$FF,$BD,$FF,$F7,$FF
.byte $00,$18,$18,$3C,$3C,$3C,$7C,$00,$FF,$FF,$DB,$FD,$FD,$BD,$FD,$FF
.byte $00,$00,$00,$00,$00,$00,$10,$18,$18,$18,$3C,$3C,$7E,$7E,$18,$18
.byte $33,$99,$33,$FF,$33,$99,$33,$FF,$CC,$66,$CC,$00,$CC,$66,$CC,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $7E,$81,$BD,$A5,$A5,$BD,$81,$7E,$00,$00,$00,$00,$00,$00,$00,$00
.byte $7E,$81,$BD,$A5,$A5,$BD,$81,$7E,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$FF,$22,$44,$FF,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$FF,$22,$44,$FF,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $24,$2C,$34,$24,$24,$2C,$34,$24,$00,$00,$00,$00,$00,$00,$00,$00
.byte $24,$2C,$34,$24,$24,$2C,$34,$24,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$FF,$44,$22,$FF,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$FF,$44,$22,$FF,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $24,$34,$2C,$24,$24,$34,$2C,$24,$00,$00,$00,$00,$00,$00,$00,$00
.byte $24,$34,$2C,$24,$24,$34,$2C,$24,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00

;;

;.segment "CHARS"
;.incbin "tileset.bin"
