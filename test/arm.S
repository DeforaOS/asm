.text
	bx	%r0
	bxeq	%r0
	bic	%r0, %r0, $0x1f
	mrs	%r0, %cpsr
	msr	%r0, %cpsr
	nop
