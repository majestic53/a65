	.org 0x0600

	ldx #0x00
	ldy #0x00

firstloop:
	txa
	sta 0x0200, y
	pha
	inx
	iny
	cpy #0x10
	bne firstloop

secondloop:
	pla
	sta 0x0200, y
	iny
	cpy #0x20
	bne secondloop
