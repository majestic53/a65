A65 Syntax
==========

For more detail see the [A65 Backus–Naur Documentation](https://github.com/majestic53/a65/blob/master/doc/a65_bnf.txt)

Primitives
==========

###Character

```
. | \a | \b | \\ | \r | \f | \n | \" | \' | \t | \v | \x[0-9a-fA-F]{1-2} | \[0-9]{1-3}
```

###Constant

```
false | null | true
```

###Identifier

```
[_a-zA-Z][_a-zA-Z0-9]*
```

###Literal

```
"<character>*"{<expression>}? | '<character>'
```

###Scalar

```
-?[0b[0-1]{1-16} | [0-9]{1-5} | 0x[a-fA-F0-9]{1-4} | 0c[0-7]{1-6}]
```

Command
=======

###Absolute

```
[cmd{<scalar>} | adc | and | asl | bit | cmp | cpx | cpy | dec | eor | inc | jmp | jsr | lda | ldx | ldy | lsr
	| ora | rol | ror | sbc | sta | stx | sty | stz | trb | tsb] <expression>
```

###Absolute Index Indirect

```
[cmd{<scalar>} | jmp] [<expression>,x]
```

###Absolute Index-x

```
[cmd{<scalar>} | adc | and | asl | bit | cmp | dec | eor | inc | lda | ldy | lsr | ora | rol | ror
	| sbc | sta | stz] <expression>,x
```

###Absolute Index-y

```
[cmd{<scalar>} | adc | and | cmp | eor | lda | ldx | ora | sbc | sta] <expression>,y
```

###Absolute Indirect

```
[cmd{<scalar>} | jmp] [<expression>]
```

###Accumulator

```
[cmd{<scalar>} | asl | dec | inc | lsr | rol | ror] a
```

###Immediate

```
[cmd{<scalar>} | adc | and | bit | cmp | cpx | cpy | eor | lda | ldx | ldy | ora | sbc] #<expression>
```

###Implied

```
[cmd{<scalar>} | brk | clc | cld | cli | clv | dex | dey | inx | iny | nop | pha | php | phx | phy | pla | plp
	| plx | ply | rti | rts | sec | sed | sei | stp | tax | tay | tsx | txa | txs | tya | wai]
```

###Relative

```
[cmd{<scalar>} | bbr0 | bbr1 | bbr2 | bbr3 | bbr4 | bbr5 | bbr6 | bbr7 | bbs0 | bbs1 | bbs2 | bbs3 | bbs4 | bbs5
	| bbs6 | bbs7 | bcc | bcs | beq | bmi | bne | bpl | bra | bvc | bvs] <expression>
```

###Zeropage

```
[cmd{<scalar>} | adc | and | asl | bit | cmp | cpx | cpy | dec | eor | inc | lda | ldx | ldy | lsr | ora
	| rmb0 | rmb1 | rmb2 | rmb3 | rmb4 | rmb5 | rmb6 | rmb7 | rol | ror | sbc | smb0 | smb1 | smb2 | smb3 | smb4 | smb5 | smb6 | smb7
	| sta | stx | sty | stz | trb | tsb] <expression>
```

###Zeropage Index Indirect

```
[cmd{<scalar>} | adc | and | cmp | eor | lda | ora | sbc | sta] [<expression>,x]
```

###Zeropage Index-x

```
[cmd{<scalar>} | adc | and | asl | bit | cmp | dec | eor | inc | lda | ldy | lsr | ora | rol | ror | sbc
	| sta | sty | stz] <expression>,x
```

###Zeropage Index-y

```
[cmd{<scalar>} | ldx | stx] <expression>,y
```

###Zeropage Indirect

```
[cmd{<scalar>} | adc | and | cmp | eor | lda | ora | sbc | sta] [<expression>]
```

###Zeropage Indirect Index

```
[cmd{<scalar>} | adc | and | cmp | eor | lda | ora | sbc | sta] [<expression>],y
```

Directive
=========

###Define Bytes

```
directive_data_byte ::= .db <expression_list>
```

###Define Words

```
directive_data_word ::= .dw <expression_list>
```

###Define

```
directive_define ::= .def <identifier> <expression>?
```

###Export

```
directive_export ::= .exp <identifier>
```

###If

```
directive_if ::= .if <expression_condition> <statement>* (.elseif <expression_condition> <statement>*)* (.else <statement>*)? .endif
```

###If Defined

```
directive_ifdef ::= [.ifdef | .ifndef] <identifier> <statement>* (.else <statement>*)? .endif
```

###Origin

```
directive_origin ::= .org <expression>
```

###Reserve

```
directive_reserve ::= .res <expression>
```

###Undefine

```
directive_undefine ::= .undef <identifier>
```

Label
=====

```
<identifier>:
```

Pragma
======

###Include Binary

```
@incb <literal>
```

###Include Source

```
@inc <literal>
```
```
