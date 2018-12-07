	.org 0x0600

	jsr init
	jsr loop
	jsr end

init:
	ldx #0x00
	rts

loop:
	inx
	cpx #0x05
	bne loop
	rts

end:
	brk
