/* $Id$ */
.section .text
	push1	0x01		/* 60 01 */
	push1	0x00		/* 60 00 */
	dup2			/* 81 */
	swap1			/* 90 */
	sstore			/* 55 */
	pop			/* 50 */
