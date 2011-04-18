/* $Id$ */
.text
	emms					/* 0f 77		*/
#if 1 /* FIXME doesn't work properly */
	por	%mm1, [%edx]
	por	%mm2, [%edx + $0x56]
	por	%mm3, [%edx + $0x789abc]
	por	%mm4, %mm5
#endif
#if 1 /* FIXME doesn't work properly */
	pxor	%mm1, [%edx]
	pxor	%mm2, [%edx + $0x56]
	pxor	%mm3, [%edx + $0x789abc]
	pxor	%mm4, %mm5
#endif
