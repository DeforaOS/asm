.text
	b	$0xa0
	beq	$0xa0
	bl	$0xa0
	bleq	$0xa0
	bx	%r0
	bxeq	%r0
	bic	%r0, %r0, $0x1f
	mrs	%r0, %cpsr
	msr	%r0, %cpsr
	nop
