	.org 0x0600

	ldx #0x01
	lda #0x05
	sta 0x01
	lda #0x07
	sta 0x02
	ldy #0x0a
	sty 0x0705
	lda [0x00, x]
