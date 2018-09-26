; The WOZ Monitor for the Apple 1
; Written by Steve Wozniak in 1976

; page 0 variables

	.def xaml		0x24		; last "opened" location low
	.def xamh		0x25		; last "opened" location high
	.def stl		0x26		; store address low
	.def sth		0x27		; store address high
	.def l			0x28		; hex value parsing low
	.def h			0x29		; hex value parsing high
	.def ysav		0x2a		; used to see if hex value is given
	.def mode		0x2b		; $00=xam, $7f=stor, $ae=block xam

; other variables

	.def in			0x0200		; input buffer to $027f
	.def kbd		0xd010		; pia.a keyboard input
	.def kbdcr		0xd011		; pia.a keyboard control register
	.def dsp		0xd012		; pia.b display output register
	.def dspcr		0xd013		; pia.b display control register

	.org			0xff00

reset:
	cld					; clear decimal arithmetic mode.
	cli
	ldy #0x7f				; mask for dsp data direction register.
	sty dsp					; set it up.
	lda #0xa7				; kbd and dsp control register mask.
	sta kbdcr				; enable interrupts, set ca1, cb1, for
	sta dspcr				; positive edge sense/output mode.
notcr:
	cmp #'_' + 0x80				; "_"?
	beq backspace				; yes.
	cmp #0x9b				; esc?
	beq escape				; yes.
	iny					; advance text index.
	bpl nextchar				; auto esc if > 127.
escape:
	lda #'\\' + 0x80				; "\".
	jsr echo				; output it.
getline:
	lda #0x8d				; cr.
	jsr echo				; output it.
	ldy #0x01				; initialize text index.
backspace:
	dey					; back up text index.
	bmi getline				; beyond start of line, reinitialize.
nextchar:
	lda kbdcr				; key ready?
	bpl nextchar				; loop until ready.
	lda kbd					; load character. b7 should be ‘1’.
	sta in, y				; add to text buffer.
	jsr echo				; display character.
	cmp #0x8d				; cr?
	bne notcr				; no.
	ldy #0xff				; reset text index.
	lda #0x00				; for xam mode.
	tax					; 0->x.
setstor:
	asl					; leaves $7b if setting stor mode.
setmode:
	sta mode				; $00=xam $7b=stor $ae=blok xam
blskip:
	iny					; advance text index.
nextitem:
	lda in, y				; get character.
	cmp #0x8d				; cr?
	beq getline				; yes, done this line.
	cmp #'.' + 0x80				; "."?
	bcc blskip				; skip delimiter.
	beq setmode				; yes. set stor mode.
	cmp #':' + 0x80				; ":"?
	beq setstor				; yes. set stor mode.
	cmp #'r' + 0x80				; "r"?
	beq run						; yes. run user program.
	stx l					; $00-> l.
	stx h					; and h.
	sty ysav				; save y for comparison.
nexthex:
	lda in, y				; get character for hex test.
	eor #0xb0				; map digits to $0-9.
	cmp #0x0a				; digit?
	bcc dig					; yes.
	adc #0x88				; map letter "a"-"f" to $fa-ff.
	cmp #0xfa				; hex letter?
	bcc nothex				; no, character not hex.
dig:
	asl
	asl					; hex digit to msd of a.
	asl
	asl
	ldx #0x04				; shift count.
hexshift:
	asl					; hex digit left, msb to carry.
	rol l					; rotate into lsd.
	rol h					;  rotate into msd’s.
	dex					; done 4 shifts?
	bne hexshift				; no, loop.
	iny					; advance text index.
	bne nexthex				; always taken. check next char for hex.
nothex:
	cpy ysav				; check if l, h empty (no hex digits).
	beq escape				; yes, generate esc sequence.
	bit mode				; test mode byte.
	bvc notstor				;  b6=0 stor 1 for xam & block xam
	lda l					; lsd’s of hex data.
	sta [stl, x]				; store at current ‘store index’.
	inc stl					; increment store index.
	bne nextitem				; get next item. (no carry).
	inc sth					; add carry to ‘store index’ high order.
tonextitem:
	jmp nextitem				; get next command item.
run:
	jmp [xaml]				; run at current xam index.
notstor:
	bmi xamnext				; b7=0 for xam, 1 for block xam.
	ldx #0x02				; byte count.
setadr:
	lda l - 1, x				; copy hex data to
	sta stl - 1, x				; ‘store index’.
	sta xaml - 1, x				; and to ‘xam index’.
	dex					; next of 2 bytes.
	bne setadr				; loop unless x=0.
nxtprnt:
	bne prdata				; ne means no address to print.
	lda #0x8d				; cr.
	jsr echo				; output it.
	lda xamh				; ‘examine index’ high-order byte.
	jsr prbyte				; output it in hex format.
	lda xaml				; low-order ‘examine index’ byte.
	jsr prbyte				; output it in hex format.
	lda #':' + 0x80				; ":".
	jsr echo				; output it.
prdata:
	lda #0xa0				; blank.
	jsr echo				; output it.
	lda [xaml, x]				; get data byte at ‘examine index’.
	jsr prbyte				; output it in hex format.
xamnext:
	stx mode				; 0->mode (xam mode).
	lda xaml
	cmp l					; compare ‘examine index’ to hex data.
	lda xamh
	sbc h
	bcs tonextitem				; not less, so no more data to output.
	inc xaml
	bne mod8chk				; increment ‘examine index’.
	inc xamh
mod8chk:
	lda xaml				; check low-order ‘examine index’ byte
	and #0x07				; for mod 8=0
	bpl nxtprnt				; always taken.
prbyte:
	pha					; save a for lsd.
	lsr
	lsr
	lsr					; msd to lsd position.
	lsr
	jsr prhex				; output hex digit.
	pla					; restore a.
prhex:
	and #0x0f				; mask lsd for hex print.
	ora #'0' + 0x80				; add "0".
	cmp #0xba				; digit?
	bcc echo				; yes, output it.
	adc #0x06				; add offset for letter.
echo:
	bit dsp					; bit (b7) cleared yet?
	bmi echo				; no, wait for display.
	sta dsp					; output character. sets da.
	rts					; return.

	brk					; unused
	brk					; unused

; interrupt vectors

	.dw			0x0f00		; nmi
	.dw			reset		; reset
	.dw			0x0000		; brk/irq
