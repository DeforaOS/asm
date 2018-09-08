/* $Id$ */
.section .text
	/* BSWAP */
	bswap	%eax
	bswap	%ecx
	bswap	%edx
	bswap	%ebx
	bswap	%esp
	bswap	%ebp
	bswap	%esi
	bswap	%edi
	cpuid					/* 0f a2		*/
