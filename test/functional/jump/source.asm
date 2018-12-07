	.org 0x0600

	lda #0x03
	jmp there
	brk
	brk
	brk

there:
	sta 0x0200
