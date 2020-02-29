@ Tremolo library
@-----------------------------------------------------------------------
@ Copyright (C) 2002-2009, Xiph.org Foundation
@ Copyright (C) 2010, Robin Watts for Pinknoise Productions Ltd
@ All rights reserved.

@ Redistribution and use in source and binary forms, with or without
@ modification, are permitted provided that the following conditions
@ are met:

@     * Redistributions of source code must retain the above copyright
@ notice, this list of conditions and the following disclaimer.
@     * Redistributions in binary form must reproduce the above
@ copyright notice, this list of conditions and the following disclaimer
@ in the documentation and/or other materials provided with the
@ distribution.
@     * Neither the names of the Xiph.org Foundation nor Pinknoise
@ Productions Ltd nor the names of its contributors may be used to
@ endorse or promote products derived from this software without
@ specific prior written permission.
@
@ THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
@ "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
@ LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
@ A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
@ OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
@ SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
@ LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
@ DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
@ THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
@ (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
@ OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
@ ----------------------------------------------------------------------

    .text

	@ full accuracy version

	.global mdct_backwardARM
	.global mdct_shift_right
	.global mdct_unroll_prelap
	.global mdct_unroll_part2
	.global mdct_unroll_part3
	.global mdct_unroll_postlap

	.extern	sincos_lookup0
	.extern	sincos_lookup1
	.hidden	sincos_lookup0
	.hidden	sincos_lookup1

mdct_unroll_prelap:
	@ r0 = out
	@ r1 = post
	@ r2 = r
	@ r3 = step
	STMFD	r13!,{r4-r7,r14}
	MVN	r4, #0x8000
	MOV	r3, r3, LSL #1
	SUB	r1, r2, r1		@ r1 = r - post
	SUBS	r1, r1, #16		@ r1 = r - post - 16
	BLT	unroll_over
unroll_loop:
	LDMDB	r2!,{r5,r6,r7,r12}

	MOV	r5, r5, ASR #9		@ r5 = (*--r)>>9
	MOV	r6, r6, ASR #9		@ r6 = (*--r)>>9
	MOV	r7, r7, ASR #9		@ r7 = (*--r)>>9
	MOV	r12,r12,ASR #9		@ r12= (*--r)>>9

	MOV	r14,r12,ASR #15
	TEQ	r14,r14,ASR #31		@ if r14==0 || r14==-1 then in range
	EORNE	r12,r4, r14,ASR #31
	STRH	r12,[r0], r3

	MOV	r14,r7, ASR #15
	TEQ	r14,r14,ASR #31		@ if r14==0 || r14==-1 then in range
	EORNE	r7, r4, r14,ASR #31
	STRH	r7, [r0], r3

	MOV	r14,r6, ASR #15
	TEQ	r14,r14,ASR #31		@ if r14==0 || r14==-1 then in range
	EORNE	r6, r4, r14,ASR #31
	STRH	r6, [r0], r3

	MOV	r14,r5, ASR #15
	TEQ	r14,r14,ASR #31		@ if r14==0 || r14==-1 then in range
	EORNE	r5, r4, r14,ASR #31
	STRH	r5, [r0], r3

	SUBS	r1, r1, #16
	BGE	unroll_loop

unroll_over:
	ADDS	r1, r1, #16
	BLE	unroll_end
unroll_loop2:
	LDR	r5,[r2,#-4]!
	@ stall
	@ stall (Xscale)
	MOV	r5, r5, ASR #9		@ r5 = (*--r)>>9
	MOV	r14,r5, ASR #15
	TEQ	r14,r14,ASR #31		@ if r14==0 || r14==-1 then in range
	EORNE	r5, r4, r14,ASR #31
	STRH	r5, [r0], r3
	SUBS	r1, r1, #4
	BGT	unroll_loop2
unroll_end:
	LDMFD	r13!,{r4-r7,PC}

mdct_unroll_postlap:
	@ r0 = out
	@ r1 = post
	@ r2 = l
	@ r3 = step
	STMFD	r13!,{r4-r7,r14}
	MVN	r4, #0x8000
	MOV	r3, r3, LSL #1
	SUB	r1, r1, r2		@ r1 = post - l
	MOV	r1, r1, ASR #1		@ r1 = (post - l)>>1
	SUBS	r1, r1, #16		@ r1 = ((post - l)>>1) - 4
	BLT	unroll_over3
unroll_loop3:
	LDR	r12,[r2],#8
	LDR	r7, [r2],#8
	LDR	r6, [r2],#8
	LDR	r5, [r2],#8

	RSB	r12,r12,#0
	RSB	r5, r5, #0
	RSB	r6, r6, #0
	RSB	r7, r7, #0

	MOV	r12, r12,ASR #9		@ r12= (-*l)>>9
	MOV	r5,  r5, ASR #9		@ r5 = (-*l)>>9
	MOV	r6,  r6, ASR #9		@ r6 = (-*l)>>9
	MOV	r7,  r7, ASR #9		@ r7 = (-*l)>>9

	MOV	r14,r12,ASR #15
	TEQ	r14,r14,ASR #31		@ if r14==0 || r14==-1 then in range
	EORNE	r12,r4, r14,ASR #31
	STRH	r12,[r0], r3

	MOV	r14,r7, ASR #15
	TEQ	r14,r14,ASR #31		@ if r14==0 || r14==-1 then in range
	EORNE	r7, r4, r14,ASR #31
	STRH	r7, [r0], r3

	MOV	r14,r6, ASR #15
	TEQ	r14,r14,ASR #31		@ if r14==0 || r14==-1 then in range
	EORNE	r6, r4, r14,ASR #31
	STRH	r6, [r0], r3

	MOV	r14,r5, ASR #15
	TEQ	r14,r14,ASR #31		@ if r14==0 || r14==-1 then in range
	EORNE	r5, r4, r14,ASR #31
	STRH	r5, [r0], r3

	SUBS	r1, r1, #16
	BGE	unroll_loop3

unroll_over3:
	ADDS	r1, r1, #16
	BLE	unroll_over4
unroll_loop4:
	LDR	r5,[r2], #8
	@ stall
	@ stall (Xscale)
	RSB	r5, r5, #0
	MOV	r5, r5, ASR #9		@ r5 = (-*l)>>9
	MOV	r14,r5, ASR #15
	TEQ	r14,r14,ASR #31		@ if r14==0 || r14==-1 then in range
	EORNE	r5, r4, r14,ASR #31
	STRH	r5, [r0], r3
	SUBS	r1, r1, #4
	BGT	unroll_loop4
unroll_over4:
	LDMFD	r13!,{r4-r7,PC}

mdct_unroll_part2:
	@ r0 = out
	@ r1 = post
	@ r2 = l
	@ r3 = r
	@ <> = step
	@ <> = wL
	@ <> = wR
	MOV	r12,r13
	STMFD	r13!,{r4,r6-r11,r14}
	LDMFD	r12,{r8,r9,r10}		@ r8 = step
					@ r9 = wL
					@ r10= wR
	MVN	r4, #0x8000
	MOV	r8, r8, LSL #1
	SUBS	r1, r3, r1		@ r1 = (r - post)
	BLE	unroll_over5
unroll_loop5:
	LDR	r12,[r2, #-8]!		@ r12= *l       (but l -= 2 first)
	LDR	r11,[r9],#4		@ r11= *wL++
	LDR	r7, [r3, #-4]!		@ r7 = *--r
	LDR	r6, [r10,#-4]!		@ r6 = *--wR

	@ Can save a cycle here, at the cost of 1bit errors in rounding
	SMULL	r14,r11,r12,r11		@ (r14,r11)  = *l   * *wL++
	SMULL	r14,r6, r7, r6		@ (r14,r6)   = *--r * *--wR
	ADD	r6, r6, r11
	MOV	r6, r6, ASR #8
	MOV	r14,r6, ASR #15
	TEQ	r14,r14,ASR #31		@ if r14==0 || r14==-1 then in range
	EORNE	r6, r4, r14,ASR #31
	STRH	r6, [r0], r8

	SUBS	r1, r1, #4
	BGT	unroll_loop5

unroll_over5:
	LDMFD	r13!,{r4,r6-r11,PC}

mdct_unroll_part3:
	@ r0 = out
	@ r1 = post
	@ r2 = l
	@ r3 = r
	@ <> = step
	@ <> = wL
	@ <> = wR
	MOV	r12,r13
	STMFD	r13!,{r4,r6-r11,r14}
	LDMFD	r12,{r8,r9,r10}		@ r8 = step
					@ r9 = wL
					@ r10= wR
	MVN	r4, #0x8000
	MOV	r8, r8, LSL #1
	SUBS	r1, r1, r3		@ r1 = (post - r)
	BLE	unroll_over6
unroll_loop6:
	LDR	r12,[r2],#8		@ r12= *l       (but l += 2 first)
	LDR	r11,[r9],#4		@ r11= *wL++
	LDR	r7, [r3],#4		@ r7 = *r++
	LDR	r6, [r10,#-4]!		@ r6 = *--wR

	@ Can save a cycle here, at the cost of 1bit errors in rounding
	SMULL	r14,r11,r12,r11		@ (r14,r11)  = *l   * *wL++
	SMULL	r14,r6, r7, r6		@ (r14,r6)   = *--r * *--wR
	SUB	r6, r6, r11
	MOV	r6, r6, ASR #8
	MOV	r14,r6, ASR #15
	TEQ	r14,r14,ASR #31		@ if r14==0 || r14==-1 then in range
	EORNE	r6, r4, r14,ASR #31
	STRH	r6, [r0], r8

	SUBS	r1, r1, #4
	BGT	unroll_loop6

unroll_over6:
	LDMFD	r13!,{r4,r6-r11,PC}

mdct_shift_right:
	@ r0 = n
	@ r1 = in
	@ r2 = right
	STMFD	r13!,{r4-r11,r14}

	MOV	r0, r0, LSR #2		@ n >>= 2
	ADD	r1, r1, #4

	SUBS	r0, r0,	#8
	BLT	sr_less_than_8
sr_loop:
	LDR	r3, [r1], #8
	LDR	r4, [r1], #8
	LDR	r5, [r1], #8
	LDR	r6, [r1], #8
	LDR	r7, [r1], #8
	LDR	r8, [r1], #8
	LDR	r12,[r1], #8
	LDR	r14,[r1], #8
	SUBS	r0, r0, #8
	STMIA	r2!,{r3,r4,r5,r6,r7,r8,r12,r14}
	BGE	sr_loop
sr_less_than_8:
	ADDS	r0, r0, #8
	BEQ	sr_end
sr_loop2:
	LDR	r3, [r1], #8
	SUBS	r0, r0, #1
	STR	r3, [r2], #4
	BGT	sr_loop2
sr_end:
	LDMFD	r13!,{r4-r11,PC}

mdct_backwardARM:
	@ r0 = n
	@ r1 = in
	STMFD	r13!,{r4-r11,r14}

	MOV	r2,#1<<4	@ r2 = 1<<shift
	MOV	r3,#13-4	@ r3 = 13-shift
find_shift_loop:
	TST	r0,r2		@ if (n & (1<<shift)) == 0
	MOV	r2,r2,LSL #1
	SUBEQ	r3,r3,#1	@ shift--
	BEQ	find_shift_loop
	MOV	r2,#2
	MOV	r2,r2,LSL r3	@ r2 = step = 2<<shift

	@ presymmetry
	@ r0 = n (a multiple of 4)
	@ r1 = in
	@ r2 = step
	@ r3 = shift

	ADD	r4, r1, r0, LSL #1	@ r4 = aX = in+(n>>1)
	ADD	r14,r1, r0		@ r14= in+(n>>2)
	SUB	r4, r4, #3*4		@ r4 = aX = in+n2-3
	ADRL	r7, .Lsincos_lookup
	LDR	r5, [r7]		@ r5 = T=sincos_lookup0
	ADD	r5, r7

presymmetry_loop1:
	LDR	r7, [r4,#8]		@ r6 = s2 = aX[2]
	LDR	r11,[r5,#4]		@ r11= T[1]
	LDR	r6, [r4]		@ r6 = s0 = aX[0]
	LDR	r10,[r5],r2,LSL #2	@ r10= T[0]   T += step

	@ XPROD31(s0, s2, T[0], T[1], 0xaX[0], &ax[2])
	SMULL	r8, r9, r7, r11		@ (r8, r9)   = s2*T[1]
	@ stall
	@ stall ?
	SMLAL	r8, r9, r6, r10		@ (r8, r9)  += s0*T[0]
	RSB	r6, r6, #0
	@ stall ?
	SMULL	r8, r12,r7, r10		@ (r8, r12)  = s2*T[0]
	MOV	r9, r9, LSL #1
	@ stall ?
	SMLAL	r8, r12,r6, r11		@ (r8, r12) -= s0*T[1]
	STR	r9, [r4],#-16		@ aX[0] = r9
	CMP	r4,r14
	MOV	r12,r12,LSL #1
	STR	r12,[r4,#8+16]		@ aX[2] = r12

	BGE	presymmetry_loop1	@ while (aX >= in+n4)

presymmetry_loop2:
	LDR	r6,[r4]			@ r6 = s0 = aX[0]
	LDR	r10,[r5,#4]		@ r10= T[1]
	LDR	r7,[r4,#8]		@ r6 = s2 = aX[2]
	LDR	r11,[r5],-r2,LSL #2	@ r11= T[0]   T -= step

	@ XPROD31(s0, s2, T[1], T[0], 0xaX[0], &ax[2])
	SMULL	r8, r9, r6, r10		@ (r8, r9)   = s0*T[1]
	@ stall
	@ stall ?
	SMLAL	r8, r9, r7, r11		@ (r8, r9)  += s2*T[0]
	RSB	r6, r6, #0
	@ stall ?
	SMULL	r8, r12,r7, r10		@ (r8, r12)  = s2*T[1]
	MOV	r9, r9, LSL #1
	@ stall ?
	SMLAL	r8, r12,r6, r11		@ (r8, r12) -= s0*T[0]
	STR	r9, [r4],#-16		@ aX[0] = r9
	CMP	r4,r1
	MOV	r12,r12,LSL #1
	STR	r12,[r4,#8+16]		@ aX[2] = r12

	BGE	presymmetry_loop2	@ while (aX >= in)

	@ r0 = n
	@ r1 = in
	@ r2 = step
	@ r3 = shift
	STMFD	r13!,{r3}
	ADRL	r4, .Lsincos_lookup
	LDR	r5, [r4]		@ r5 = T=sincos_lookup0
	ADD	r5, r4
	ADD	r4, r1, r0, LSL #1	@ r4 = aX = in+(n>>1)
	SUB	r4, r4, #4*4		@ r4 = aX = in+(n>>1)-4
	LDR	r11,[r5,#4]		@ r11= T[1]
	LDR	r10,[r5],r2, LSL #2	@ r10= T[0]    T += step
presymmetry_loop3:
	LDR	r8,[r1],#16 		@ r8 = ro0 = bX[0]
	LDR	r9,[r1,#8-16]		@ r9 = ro2 = bX[2]
	LDR	r6,[r4]			@ r6 = ri0 = aX[0]

	@ XNPROD31( ro2, ro0, T[1], T[0], 0xaX[0], &aX[2] )
	@ aX[0] = (ro2*T[1] - ro0*T[0])>>31 aX[2] = (ro0*T[1] + ro2*T[0])>>31
	SMULL	r14,r12,r8, r11		@ (r14,r12)  = ro0*T[1]
	RSB	r8,r8,#0		@ r8 = -ro0
	@ Stall ?
	SMLAL	r14,r12,r9, r10		@ (r14,r12) += ro2*T[0]
	LDR	r7,[r4,#8]		@ r7 = ri2 = aX[2]
	@ Stall ?
	SMULL	r14,r3, r9, r11		@ (r14,r3)   = ro2*T[1]
	MOV	r12,r12,LSL #1
	LDR	r11,[r5,#4]		@ r11= T[1]
	SMLAL	r14,r3, r8, r10		@ (r14,r3)  -= ro0*T[0]
	LDR	r10,[r5],r2, LSL #2	@ r10= T[0]    T += step
	STR	r12,[r4,#8]
	MOV	r3, r3, LSL #1
	STR	r3, [r4],#-16

	@ XNPROD31( ri2, ri0, T[0], T[1], 0xbX[0], &bX[2] )
	@ bX[0] = (ri2*T[0] - ri0*T[1])>>31 bX[2] = (ri0*T[0] + ri2*T[1])>>31
	SMULL	r14,r12,r6, r10		@ (r14,r12)  = ri0*T[0]
	RSB	r6,r6,#0		@ r6 = -ri0
	@ stall ?
	SMLAL	r14,r12,r7, r11		@ (r14,r12) += ri2*T[1]
	@ stall ?
	@ stall ?
	SMULL	r14,r3, r7, r10		@ (r14,r3)   = ri2*T[0]
	MOV	r12,r12,LSL #1
	@ stall ?
	SMLAL	r14,r3, r6, r11		@ (r14,r3)  -= ri0*T[1]
	CMP	r4,r1
	STR	r12,[r1,#8-16]
	MOV	r3, r3, LSL #1
	STR	r3, [r1,#-16]

	BGE	presymmetry_loop3

	SUB	r1,r1,r0		@ r1 = in -= n>>2 (i.e. restore in)

	LDR	r3,[r13]
	STR	r2,[r13,#-4]!

	@ mdct_butterflies
	@ r0 = n  = (points * 2)
	@ r1 = in = x
	@ r2 = i
	@ r3 = shift
	STMFD	r13!,{r0-r1}
	ADRL	r4, .Lsincos_lookup
	LDR	r5, [r4]
	ADD	r5, r4
	RSBS	r4,r3,#6		@ r4 = stages = 7-shift then --stages
	BLE	no_generics
	MOV	r14,#4			@ r14= 4               (i=0)
	MOV	r6, r14,LSL r3		@ r6 = (4<<i)<<shift
mdct_butterflies_loop1:
	MOV	r0, r0, LSR #1		@ r0 = points>>i = POINTS
	MOV	r2, r14,LSR #2		@ r2 = (1<<i)-j        (j=0)
	STMFD	r13!,{r4,r14}
mdct_butterflies_loop2:

	@ mdct_butterfly_generic(x+POINTS*j, POINTS, 4<<(i+shift))
	@ mdct_butterfly_generic(r1, r0, r6)
	@ r0 = points
	@ r1 = x
	@ preserve r2 (external loop counter)
	@ preserve r3
	@ preserve r4 (external loop counter)
	@ r5 = T = sincos_lookup0
	@ r6 = step
	@ preserve r14

	STR	r2,[r13,#-4]!		@ stack r2
	ADD	r1,r1,r0,LSL #1		@ r1 = x2+4 = x + (POINTS>>1)
	ADD	r7,r1,r0,LSL #1		@ r7 = x1+4 = x + POINTS
	ADD	r12,r5,#1024*4		@ r12= sincos_lookup0+1024

mdct_bufferfly_generic_loop1:
	LDMDB	r7!,{r2,r3,r8,r11}	@ r2 = x1[0]
					@ r3 = x1[1]
					@ r8 = x1[2]
					@ r11= x1[3]    x1 -= 4
	LDMDB	r1!,{r4,r9,r10,r14}	@ r4 = x2[0]
					@ r9 = x2[1]
					@ r10= x2[2]
					@ r14= x2[3]    x2 -= 4

	SUB	r2, r2, r3		@ r2 = s0 = x1[0] - x1[1]
	ADD	r3, r2, r3, LSL #1	@ r3 =      x1[0] + x1[1] (-> x1[0])
	SUB	r11,r11,r8		@ r11= s1 = x1[3] - x1[2]
	ADD	r8, r11,r8, LSL #1	@ r8 =      x1[3] + x1[2] (-> x1[2])
	SUB	r9, r9, r4		@ r9 = s2 = x2[1] - x2[0]
	ADD	r4, r9, r4, LSL #1	@ r4 =      x2[1] + x2[0] (-> x1[1])
	SUB	r14,r14,r10		@ r14= s3 = x2[3] - x2[2]
	ADD	r10,r14,r10,LSL #1	@ r10=      x2[3] + x2[2] (-> x1[3])
	STMIA	r7,{r3,r4,r8,r10}

	@ r0 = points
	@ r1 = x2
	@ r2 = s0
	@ r3 free
	@ r4 free
	@ r5 = T
	@ r6 = step
	@ r7 = x1
	@ r8 free
	@ r9 = s2
	@ r10 free
	@ r11= s1
	@ r12= limit
	@ r14= s3

	LDR	r8, [r5,#4]		@ r8 = T[1]
	LDR	r10,[r5],r6,LSL #2	@ r10= T[0]		T += step

	@ XPROD31(s1, s0, T[0], T[1], &x2[0], &x2[2])
	@ x2[0] = (s1*T[0] + s0*T[1])>>31     x2[2] = (s0*T[0] - s1*T[1])>>31
	@ stall Xscale
	SMULL	r4, r3, r2, r8		@ (r4, r3)   = s0*T[1]
	SMLAL	r4, r3, r11,r10		@ (r4, r3)  += s1*T[0]
	RSB	r11,r11,#0
	SMULL	r11,r4, r8, r11		@ (r11,r4)   = -s1*T[1]
	SMLAL	r11,r4, r2, r10		@ (r11,r4)  += s0*T[0]
	MOV	r2, r3, LSL #1		@ r2 = r3<<1 = Value for x2[0]

	@ XPROD31(s2, s3, T[0], T[1], &x2[1], &x2[3])
	@ x2[1] = (s2*T[0] + s3*T[1])>>31     x2[3] = (s3*T[0] - s2*T[1])>>31
	SMULL	r11,r3, r9, r10		@ (r11,r3)   = s2*T[0]
	MOV	r4, r4, LSL #1		@ r4 = r4<<1 = Value for x2[2]
	SMLAL	r11,r3, r14,r8		@ (r11,r3)  += s3*T[1]
	RSB	r9, r9, #0
	SMULL	r10,r11,r14,r10		@ (r10,r11)  = s3*T[0]
	MOV	r3, r3, LSL #1		@ r3 = r3<<1 = Value for x2[1]
	SMLAL	r10,r11,r9,r8		@ (r10,r11) -= s2*T[1]
	CMP	r5, r12
	MOV	r11,r11,LSL #1		@ r11= r11<<1 = Value for x2[3]

	STMIA	r1,{r2,r3,r4,r11}

	BLT	mdct_bufferfly_generic_loop1

	SUB	r12,r12,#1024*4
mdct_bufferfly_generic_loop2:
	LDMDB	r7!,{r2,r3,r9,r10}	@ r2 = x1[0]
					@ r3 = x1[1]
					@ r9 = x1[2]
					@ r10= x1[3]    x1 -= 4
	LDMDB	r1!,{r4,r8,r11,r14}	@ r4 = x2[0]
					@ r8 = x2[1]
					@ r11= x2[2]
					@ r14= x2[3]    x2 -= 4

	SUB	r2, r2, r3		@ r2 = s0 = x1[0] - x1[1]
	ADD	r3, r2, r3, LSL #1	@ r3 =      x1[0] + x1[1] (-> x1[0])
	SUB	r9, r9,r10		@ r9 = s1 = x1[2] - x1[3]
	ADD	r10,r9,r10, LSL #1	@ r10=      x1[2] + x1[3] (-> x1[2])
	SUB	r4, r4, r8		@ r4 = s2 = x2[0] - x2[1]
	ADD	r8, r4, r8, LSL #1	@ r8 =      x2[0] + x2[1] (-> x1[1])
	SUB	r14,r14,r11		@ r14= s3 = x2[3] - x2[2]
	ADD	r11,r14,r11,LSL #1	@ r11=      x2[3] + x2[2] (-> x1[3])
	STMIA	r7,{r3,r8,r10,r11}

	@ r0 = points
	@ r1 = x2
	@ r2 = s0
	@ r3 free
	@ r4 = s2
	@ r5 = T
	@ r6 = step
	@ r7 = x1
	@ r8 free
	@ r9 = s1
	@ r10 free
	@ r11 free
	@ r12= limit
	@ r14= s3

	LDR	r8, [r5,#4]		@ r8 = T[1]
	LDR	r10,[r5],-r6,LSL #2	@ r10= T[0]		T -= step

	@ XNPROD31(s0, s1, T[0], T[1], &x2[0], &x2[2])
	@ x2[0] = (s0*T[0] - s1*T[1])>>31     x2[2] = (s1*T[0] + s0*T[1])>>31
	@ stall Xscale
	SMULL	r3, r11,r2, r8		@ (r3, r11)  = s0*T[1]
	SMLAL	r3, r11,r9, r10		@ (r3, r11) += s1*T[0]
	RSB	r9, r9, #0
	SMULL	r3, r2, r10,r2		@ (r3, r2)   = s0*T[0]
	SMLAL	r3, r2, r9, r8		@ (r3, r2)  += -s1*T[1]
	MOV	r9, r11,LSL #1		@ r9 = r11<<1 = Value for x2[2]

	@ XNPROD31(s3, s2, T[0], T[1], &x2[1], &x2[3])
	@ x2[1] = (s3*T[0] - s2*T[1])>>31     x2[3] = (s2*T[0] + s3*T[1])>>31
	SMULL	r3, r11,r4, r10		@ (r3,r11)   = s2*T[0]
	MOV	r2, r2, LSL #1		@ r2 = r2<<1  = Value for x2[0]
	SMLAL	r3, r11,r14,r8		@ (r3,r11)  += s3*T[1]
	RSB	r4, r4, #0
	SMULL	r10,r3,r14,r10		@ (r10,r3)   = s3*T[0]
	MOV	r11,r11,LSL #1		@ r11= r11<<1 = Value for x2[3]
	SMLAL	r10,r3, r4, r8		@ (r10,r3)  -= s2*T[1]
	CMP	r5, r12
	MOV	r3, r3, LSL #1		@ r3 = r3<<1  = Value for x2[1]

	STMIA	r1,{r2,r3,r9,r11}

	BGT	mdct_bufferfly_generic_loop2

	LDR	r2,[r13],#4		@ unstack r2
	ADD	r1, r1, r0, LSL #2	@ r1 = x+POINTS*j
	@ stall Xscale
	SUBS	r2, r2, #1		@ r2--                 (j++)
	BGT	mdct_butterflies_loop2

	LDMFD	r13!,{r4,r14}

	LDR	r1,[r13,#4]

	SUBS	r4, r4, #1		@ stages--
	MOV	r14,r14,LSL #1		@ r14= 4<<i            (i++)
	MOV	r6, r6, LSL #1		@ r6 = step <<= 1      (i++)
	BGE	mdct_butterflies_loop1
	LDMFD	r13,{r0-r1}
no_generics:
	@ mdct_butterflies part2 (loop around mdct_bufferfly_32)
	@ r0 = points
	@ r1 = in
	@ r2 = step
	@ r3 = shift

mdct_bufferflies_loop3:
	@ mdct_bufferfly_32

	@ block1
	ADD	r4, r1, #16*4		@ r4 = &in[16]
	LDMIA	r4,{r5,r6,r9,r10}	@ r5 = x[16]
					@ r6 = x[17]
					@ r9 = x[18]
					@ r10= x[19]
	LDMIA	r1,{r7,r8,r11,r12}	@ r7 = x[0]
					@ r8 = x[1]
					@ r11= x[2]
					@ r12= x[3]
	SUB	r5, r5, r6		@ r5 = s0 = x[16] - x[17]
	ADD	r6, r5, r6, LSL #1	@ r6 =      x[16] + x[17]  -> x[16]
	SUB	r9, r9, r10		@ r9 = s1 = x[18] - x[19]
	ADD	r10,r9, r10,LSL #1	@ r10=      x[18] + x[19]  -> x[18]
	SUB	r8, r8, r7		@ r8 = s2 = x[ 1] - x[ 0]
	ADD	r7, r8, r7, LSL #1	@ r7 =      x[ 1] + x[ 0]  -> x[17]
	SUB	r12,r12,r11		@ r12= s3 = x[ 3] - x[ 2]
	ADD	r11,r12,r11, LSL #1	@ r11=      x[ 3] + x[ 2]  -> x[19]
	STMIA	r4!,{r6,r7,r10,r11}

	LDR	r6,cPI1_8
	LDR	r7,cPI3_8

	@ XNPROD31( s0, s1, cPI3_8, cPI1_8, &x[ 0], &x[ 2] )
	@ x[0] = s0*cPI3_8 - s1*cPI1_8     x[2] = s1*cPI3_8 + s0*cPI1_8
	@ stall Xscale
	SMULL	r14,r11,r5, r6		@ (r14,r11)  = s0*cPI1_8
	SMLAL	r14,r11,r9, r7		@ (r14,r11) += s1*cPI3_8
	RSB	r9, r9, #0
	SMULL	r14,r5, r7, r5		@ (r14,r5)   = s0*cPI3_8
	SMLAL	r14,r5, r9, r6		@ (r14,r5)  -= s1*cPI1_8
	MOV	r11,r11,LSL #1
	MOV	r5, r5, LSL #1

	@ XPROD31 ( s2, s3, cPI1_8, cPI3_8, &x[ 1], &x[ 3] )
	@ x[1] = s2*cPI1_8 + s3*cPI3_8     x[3] = s3*cPI1_8 - s2*cPI3_8
	SMULL	r14,r9, r8, r6		@ (r14,r9)   = s2*cPI1_8
	SMLAL	r14,r9, r12,r7		@ (r14,r9)  += s3*cPI3_8
	RSB	r8,r8,#0
	SMULL	r14,r12,r6, r12		@ (r14,r12)  = s3*cPI1_8
	SMLAL	r14,r12,r8, r7		@ (r14,r12) -= s2*cPI3_8
	MOV	r9, r9, LSL #1
	MOV	r12,r12,LSL #1
	STMIA	r1!,{r5,r9,r11,r12}

	@ block2
	LDMIA	r4,{r5,r6,r9,r10}	@ r5 = x[20]
					@ r6 = x[21]
					@ r9 = x[22]
					@ r10= x[23]
	LDMIA	r1,{r7,r8,r11,r12}	@ r7 = x[4]
					@ r8 = x[5]
					@ r11= x[6]
					@ r12= x[7]
	SUB	r5, r5, r6		@ r5 = s0 = x[20] - x[21]
	ADD	r6, r5, r6, LSL #1	@ r6 =      x[20] + x[21]  -> x[20]
	SUB	r9, r9, r10		@ r9 = s1 = x[22] - x[23]
	ADD	r10,r9, r10,LSL #1	@ r10=      x[22] + x[23]  -> x[22]
	SUB	r8, r8, r7		@ r8 = s2 = x[ 5] - x[ 4]
	ADD	r7, r8, r7, LSL #1	@ r7 =      x[ 5] + x[ 4]  -> x[21]
	SUB	r12,r12,r11		@ r12= s3 = x[ 7] - x[ 6]
	ADD	r11,r12,r11, LSL #1	@ r11=      x[ 7] + x[ 6]  -> x[23]
	LDR	r14,cPI2_8
	STMIA	r4!,{r6,r7,r10,r11}

	SUB	r5, r5, r9		@ r5 = s0 - s1
	ADD	r9, r5, r9, LSL #1	@ r9 = s0 + s1
	SMULL	r6, r5, r14,r5		@ (r6,r5)  = (s0-s1)*cPI2_8
	SUB	r12,r12,r8		@ r12= s3 - s2
	ADD	r8, r12,r8, LSL #1	@ r8 = s3 + s2

	SMULL	r6, r8, r14,r8		@ (r6,r8)  = (s3+s2)*cPI2_8
	MOV	r5, r5, LSL #1
	SMULL	r6, r9, r14,r9		@ (r6,r9)  = (s0+s1)*cPI2_8
	MOV	r8, r8, LSL #1
	SMULL	r6, r12,r14,r12		@ (r6,r12) = (s3-s2)*cPI2_8
	MOV	r9, r9, LSL #1
	MOV	r12,r12,LSL #1
	STMIA	r1!,{r5,r8,r9,r12}

	@ block3
	LDMIA	r4,{r5,r6,r9,r10}	@ r5 = x[24]
					@ r6 = x[25]
					@ r9 = x[25]
					@ r10= x[26]
	LDMIA	r1,{r7,r8,r11,r12}	@ r7 = x[8]
					@ r8 = x[9]
					@ r11= x[10]
					@ r12= x[11]
	SUB	r5, r5, r6		@ r5 = s0 = x[24] - x[25]
	ADD	r6, r5, r6, LSL #1	@ r6 =      x[24] + x[25]  -> x[25]
	SUB	r9, r9, r10		@ r9 = s1 = x[26] - x[27]
	ADD	r10,r9, r10,LSL #1	@ r10=      x[26] + x[27]  -> x[26]
	SUB	r8, r8, r7		@ r8 = s2 = x[ 9] - x[ 8]
	ADD	r7, r8, r7, LSL #1	@ r7 =      x[ 9] + x[ 8]  -> x[25]
	SUB	r12,r12,r11		@ r12= s3 = x[11] - x[10]
	ADD	r11,r12,r11, LSL #1	@ r11=      x[11] + x[10]  -> x[27]
	STMIA	r4!,{r6,r7,r10,r11}

	LDR	r6,cPI3_8
	LDR	r7,cPI1_8

	@ XNPROD31( s0, s1, cPI1_8, cPI3_8, &x[ 8], &x[10] )
	@ x[8] = s0*cPI1_8 - s1*cPI3_8     x[10] = s1*cPI1_8 + s0*cPI3_8
	@ stall Xscale
	SMULL	r14,r11,r5, r6		@ (r14,r11)  = s0*cPI3_8
	SMLAL	r14,r11,r9, r7		@ (r14,r11) += s1*cPI1_8
	RSB	r9, r9, #0
	SMULL	r14,r5, r7, r5		@ (r14,r5)   = s0*cPI1_8
	SMLAL	r14,r5, r9, r6		@ (r14,r5)  -= s1*cPI3_8
	MOV	r11,r11,LSL #1
	MOV	r5, r5, LSL #1

	@ XPROD31 ( s2, s3, cPI3_8, cPI1_8, &x[ 9], &x[11] )
	@ x[9] = s2*cPI3_8 + s3*cPI1_8     x[11] = s3*cPI3_8 - s2*cPI1_8
	SMULL	r14,r9, r8, r6		@ (r14,r9)   = s2*cPI3_8
	SMLAL	r14,r9, r12,r7		@ (r14,r9)  += s3*cPI1_8
	RSB	r8,r8,#0
	SMULL	r14,r12,r6, r12		@ (r14,r12)  = s3*cPI3_8
	SMLAL	r14,r12,r8, r7		@ (r14,r12) -= s2*cPI1_8
	MOV	r9, r9, LSL #1
	MOV	r12,r12,LSL #1
	STMIA	r1!,{r5,r9,r11,r12}

	@ block4
	LDMIA	r4,{r5,r6,r10,r11}	@ r5 = x[28]
					@ r6 = x[29]
					@ r10= x[30]
					@ r11= x[31]
	LDMIA	r1,{r8,r9,r12,r14}	@ r8 = x[12]
					@ r9 = x[13]
					@ r12= x[14]
					@ r14= x[15]
	SUB	r5, r5, r6		@ r5 = s0 = x[28] - x[29]
	ADD	r6, r5, r6, LSL #1	@ r6 =      x[28] + x[29]  -> x[28]
	SUB	r7, r14,r12		@ r7 = s3 = x[15] - x[14]
	ADD	r12,r7, r12, LSL #1	@ r12=      x[15] + x[14]  -> x[31]
	SUB	r10,r10,r11		@ r10= s1 = x[30] - x[31]
	ADD	r11,r10,r11,LSL #1	@ r11=      x[30] + x[31]  -> x[30]
	SUB	r14, r8, r9		@ r14= s2 = x[12] - x[13]
	ADD	r9, r14, r9, LSL #1	@ r9 =      x[12] + x[13]  -> x[29]
	STMIA	r4!,{r6,r9,r11,r12}
	STMIA	r1!,{r5,r7,r10,r14}

	@ mdct_butterfly16 (1st version)
	@ block 1
	SUB	r1,r1,#16*4
	ADD	r4,r1,#8*4
	LDMIA	r4,{r5,r6,r9,r10}	@ r5 = x[ 8]
					@ r6 = x[ 9]
					@ r9 = x[10]
					@ r10= x[11]
	LDMIA	r1,{r7,r8,r11,r12}	@ r7 = x[0]
					@ r8 = x[1]
					@ r11= x[2]
					@ r12= x[3]
	SUB	r5, r5, r6		@ r5 = s0 = x[ 8] - x[ 9]
	ADD	r6, r5, r6, LSL #1	@ r6 =      x[ 8] + x[ 9]  -> x[ 8]
	SUB	r9, r9, r10		@ r9 = s1 = x[10] - x[11]
	ADD	r10,r9, r10,LSL #1	@ r10=      x[10] + x[11]  -> x[10]
	SUB	r8, r8, r7		@ r8 = s2 = x[ 1] - x[ 0]
	ADD	r7, r8, r7, LSL #1	@ r7 =      x[ 1] + x[ 0]  -> x[ 9]
	SUB	r12,r12,r11		@ r12= s3 = x[ 3] - x[ 2]
	ADD	r11,r12,r11, LSL #1	@ r11=      x[ 3] + x[ 2]  -> x[11]
	LDR	r14,cPI2_8
	STMIA	r4!,{r6,r7,r10,r11}

	SUB	r5, r5, r9		@ r5 = s0 - s1
	ADD	r9, r5, r9, LSL #1	@ r9 = s0 + s1
	SMULL	r6, r5, r14,r5		@ (r6,r5)  = (s0-s1)*cPI2_8
	SUB	r12,r12,r8		@ r12= s3 - s2
	ADD	r8, r12,r8, LSL #1	@ r8 = s3 + s2

	SMULL	r6, r8, r14,r8		@ (r6,r8)  = (s3+s2)*cPI2_8
	MOV	r5, r5, LSL #1
	SMULL	r6, r9, r14,r9		@ (r6,r9)  = (s0+s1)*cPI2_8
	MOV	r8, r8, LSL #1
	SMULL	r6, r12,r14,r12		@ (r6,r12) = (s3-s2)*cPI2_8
	MOV	r9, r9, LSL #1
	MOV	r12,r12,LSL #1
	STMIA	r1!,{r5,r8,r9,r12}

	@ block4
	LDMIA	r4,{r5,r6,r9,r10}	@ r5 = x[12]
					@ r6 = x[13]
					@ r9 = x[14]
					@ r10= x[15]
	LDMIA	r1,{r7,r8,r11,r12}	@ r7 = x[ 4]
					@ r8 = x[ 5]
					@ r11= x[ 6]
					@ r12= x[ 7]
	SUB	r14,r7, r8		@ r14= s0 = x[ 4] - x[ 5]
	ADD	r8, r14,r8, LSL #1	@ r8 =      x[ 4] + x[ 5]  -> x[13]
	SUB	r7, r12,r11		@ r7 = s1 = x[ 7] - x[ 6]
	ADD	r11,r7, r11, LSL #1	@ r11=      x[ 7] + x[ 6]  -> x[15]
	SUB	r5, r5, r6		@ r5 = s2 = x[12] - x[13]
	ADD	r6, r5, r6, LSL #1	@ r6 =      x[12] + x[13]  -> x[12]
	SUB	r12,r9, r10		@ r12= s3 = x[14] - x[15]
	ADD	r10,r12,r10,LSL #1	@ r10=      x[14] + x[15]  -> x[14]
	STMIA	r4!,{r6,r8,r10,r11}
	STMIA	r1!,{r5,r7,r12,r14}

	@ mdct_butterfly_8
	LDMDB	r1,{r6,r7,r8,r9,r10,r11,r12,r14}
					@ r6 = x[0]
					@ r7 = x[1]
					@ r8 = x[2]
					@ r9 = x[3]
					@ r10= x[4]
					@ r11= x[5]
					@ r12= x[6]
					@ r14= x[7]
	ADD	r6, r6, r7		@ r6 = s0 = x[0] + x[1]
	SUB	r7, r6, r7, LSL #1	@ r7 = s1 = x[0] - x[1]
	ADD	r8, r8, r9		@ r8 = s2 = x[2] + x[3]
	SUB	r9, r8, r9, LSL #1	@ r9 = s3 = x[2] - x[3]
	ADD	r10,r10,r11		@ r10= s4 = x[4] + x[5]
	SUB	r11,r10,r11,LSL #1	@ r11= s5 = x[4] - x[5]
	ADD	r12,r12,r14		@ r12= s6 = x[6] + x[7]
	SUB	r14,r12,r14,LSL #1	@ r14= s7 = x[6] - x[7]

	ADD	r2, r11,r9		@ r2 = x[0] = s5 + s3
	SUB	r4, r2, r9, LSL #1	@ r4 = x[2] = s5 - s3
	SUB	r3, r14,r7		@ r3 = x[1] = s7 - s1
	ADD	r5, r3, r7, LSL #1	@ r5 = x[3] = s7 + s1
	SUB	r10,r10,r6		@ r10= x[4] = s4 - s0
	SUB	r11,r12,r8		@ r11= x[5] = s6 - s2
	ADD	r12,r10,r6, LSL #1	@ r12= x[6] = s4 + s0
	ADD	r14,r11,r8, LSL #1	@ r14= x[7] = s6 + s2
	STMDB	r1,{r2,r3,r4,r5,r10,r11,r12,r14}

	@ mdct_butterfly_8
	LDMIA	r1,{r6,r7,r8,r9,r10,r11,r12,r14}
					@ r6 = x[0]
					@ r7 = x[1]
					@ r8 = x[2]
					@ r9 = x[3]
					@ r10= x[4]
					@ r11= x[5]
					@ r12= x[6]
					@ r14= x[7]
	ADD	r6, r6, r7		@ r6 = s0 = x[0] + x[1]
	SUB	r7, r6, r7, LSL #1	@ r7 = s1 = x[0] - x[1]
	ADD	r8, r8, r9		@ r8 = s2 = x[2] + x[3]
	SUB	r9, r8, r9, LSL #1	@ r9 = s3 = x[2] - x[3]
	ADD	r10,r10,r11		@ r10= s4 = x[4] + x[5]
	SUB	r11,r10,r11,LSL #1	@ r11= s5 = x[4] - x[5]
	ADD	r12,r12,r14		@ r12= s6 = x[6] + x[7]
	SUB	r14,r12,r14,LSL #1	@ r14= s7 = x[6] - x[7]

	ADD	r2, r11,r9		@ r2 = x[0] = s5 + s3
	SUB	r4, r2, r9, LSL #1	@ r4 = x[2] = s5 - s3
	SUB	r3, r14,r7		@ r3 = x[1] = s7 - s1
	ADD	r5, r3, r7, LSL #1	@ r5 = x[3] = s7 + s1
	SUB	r10,r10,r6		@ r10= x[4] = s4 - s0
	SUB	r11,r12,r8		@ r11= x[5] = s6 - s2
	ADD	r12,r10,r6, LSL #1	@ r12= x[6] = s4 + s0
	ADD	r14,r11,r8, LSL #1	@ r14= x[7] = s6 + s2
	STMIA	r1,{r2,r3,r4,r5,r10,r11,r12,r14}

	@ block 2
	ADD	r1,r1,#16*4-8*4
	ADD	r4,r1,#8*4
	LDMIA	r4,{r5,r6,r9,r10}	@ r5 = x[ 8]
					@ r6 = x[ 9]
					@ r9 = x[10]
					@ r10= x[11]
	LDMIA	r1,{r7,r8,r11,r12}	@ r7 = x[0]
					@ r8 = x[1]
					@ r11= x[2]
					@ r12= x[3]
	SUB	r5, r5, r6		@ r5 = s0 = x[ 8] - x[ 9]
	ADD	r6, r5, r6, LSL #1	@ r6 =      x[ 8] + x[ 9]  -> x[ 8]
	SUB	r9, r9, r10		@ r9 = s1 = x[10] - x[11]
	ADD	r10,r9, r10,LSL #1	@ r10=      x[10] + x[11]  -> x[10]
	SUB	r8, r8, r7		@ r8 = s2 = x[ 1] - x[ 0]
	ADD	r7, r8, r7, LSL #1	@ r7 =      x[ 1] + x[ 0]  -> x[ 9]
	SUB	r12,r12,r11		@ r12= s3 = x[ 3] - x[ 2]
	ADD	r11,r12,r11, LSL #1	@ r11=      x[ 3] + x[ 2]  -> x[11]
	LDR	r14,cPI2_8
	STMIA	r4!,{r6,r7,r10,r11}

	SUB	r5, r5, r9		@ r5 = s0 - s1
	ADD	r9, r5, r9, LSL #1	@ r9 = s0 + s1
	SMULL	r6, r5, r14,r5		@ (r6,r5)  = (s0-s1)*cPI2_8
	SUB	r12,r12,r8		@ r12= s3 - s2
	ADD	r8, r12,r8, LSL #1	@ r8 = s3 + s2

	SMULL	r6, r8, r14,r8		@ (r6,r8)  = (s3+s2)*cPI2_8
	MOV	r5, r5, LSL #1
	SMULL	r6, r9, r14,r9		@ (r6,r9)  = (s0+s1)*cPI2_8
	MOV	r8, r8, LSL #1
	SMULL	r6, r12,r14,r12		@ (r6,r12) = (s3-s2)*cPI2_8
	MOV	r9, r9, LSL #1
	MOV	r12,r12,LSL #1
	STMIA	r1!,{r5,r8,r9,r12}

	@ block4
	LDMIA	r4,{r5,r6,r9,r10}	@ r5 = x[12]
					@ r6 = x[13]
					@ r9 = x[14]
					@ r10= x[15]
	LDMIA	r1,{r7,r8,r11,r12}	@ r7 = x[ 4]
					@ r8 = x[ 5]
					@ r11= x[ 6]
					@ r12= x[ 7]
	SUB	r5, r5, r6		@ r5 = s2 = x[12] - x[13]
	ADD	r6, r5, r6, LSL #1	@ r6 =      x[12] + x[13]  -> x[12]
	SUB	r9, r9, r10		@ r9 = s3 = x[14] - x[15]
	ADD	r10,r9, r10,LSL #1	@ r10=      x[14] + x[15]  -> x[14]
	SUB	r14,r7, r8		@ r14= s0 = x[ 4] - x[ 5]
	ADD	r8, r14,r8, LSL #1	@ r8 =      x[ 4] + x[ 5]  -> x[13]
	SUB	r7, r12,r11		@ r7 = s1 = x[ 7] - x[ 6]
	ADD	r11,r7, r11, LSL #1	@ r11=      x[ 7] + x[ 6]  -> x[15]
	STMIA	r4!,{r6,r8,r10,r11}
	STMIA	r1!,{r5,r7,r9,r14}

	@ mdct_butterfly_8
	LDMDB	r1,{r6,r7,r8,r9,r10,r11,r12,r14}
					@ r6 = x[0]
					@ r7 = x[1]
					@ r8 = x[2]
					@ r9 = x[3]
					@ r10= x[4]
					@ r11= x[5]
					@ r12= x[6]
					@ r14= x[7]
	ADD	r6, r6, r7		@ r6 = s0 = x[0] + x[1]
	SUB	r7, r6, r7, LSL #1	@ r7 = s1 = x[0] - x[1]
	ADD	r8, r8, r9		@ r8 = s2 = x[2] + x[3]
	SUB	r9, r8, r9, LSL #1	@ r9 = s3 = x[2] - x[3]
	ADD	r10,r10,r11		@ r10= s4 = x[4] + x[5]
	SUB	r11,r10,r11,LSL #1	@ r11= s5 = x[4] - x[5]
	ADD	r12,r12,r14		@ r12= s6 = x[6] + x[7]
	SUB	r14,r12,r14,LSL #1	@ r14= s7 = x[6] - x[7]

	ADD	r2, r11,r9		@ r2 = x[0] = s5 + s3
	SUB	r4, r2, r9, LSL #1	@ r4 = x[2] = s5 - s3
	SUB	r3, r14,r7		@ r3 = x[1] = s7 - s1
	ADD	r5, r3, r7, LSL #1	@ r5 = x[3] = s7 + s1
	SUB	r10,r10,r6		@ r10= x[4] = s4 - s0
	SUB	r11,r12,r8		@ r11= x[5] = s6 - s2
	ADD	r12,r10,r6, LSL #1	@ r12= x[6] = s4 + s0
	ADD	r14,r11,r8, LSL #1	@ r14= x[7] = s6 + s2
	STMDB	r1,{r2,r3,r4,r5,r10,r11,r12,r14}

	@ mdct_butterfly_8
	LDMIA	r1,{r6,r7,r8,r9,r10,r11,r12,r14}
					@ r6 = x[0]
					@ r7 = x[1]
					@ r8 = x[2]
					@ r9 = x[3]
					@ r10= x[4]
					@ r11= x[5]
					@ r12= x[6]
					@ r14= x[7]
	ADD	r6, r6, r7		@ r6 = s0 = x[0] + x[1]
	SUB	r7, r6, r7, LSL #1	@ r7 = s1 = x[0] - x[1]
	ADD	r8, r8, r9		@ r8 = s2 = x[2] + x[3]
	SUB	r9, r8, r9, LSL #1	@ r9 = s3 = x[2] - x[3]
	ADD	r10,r10,r11		@ r10= s4 = x[4] + x[5]
	SUB	r11,r10,r11,LSL #1	@ r11= s5 = x[4] - x[5]
	ADD	r12,r12,r14		@ r12= s6 = x[6] + x[7]
	SUB	r14,r12,r14,LSL #1	@ r14= s7 = x[6] - x[7]

	ADD	r2, r11,r9		@ r2 = x[0] = s5 + s3
	SUB	r4, r2, r9, LSL #1	@ r4 = x[2] = s5 - s3
	SUB	r3, r14,r7		@ r3 = x[1] = s7 - s1
	ADD	r5, r3, r7, LSL #1	@ r5 = x[3] = s7 + s1
	SUB	r10,r10,r6		@ r10= x[4] = s4 - s0
	SUB	r11,r12,r8		@ r11= x[5] = s6 - s2
	ADD	r12,r10,r6, LSL #1	@ r12= x[6] = s4 + s0
	ADD	r14,r11,r8, LSL #1	@ r14= x[7] = s6 + s2
	STMIA	r1,{r2,r3,r4,r5,r10,r11,r12,r14}

	ADD	r1,r1,#8*4
	SUBS	r0,r0,#64
	BGT	mdct_bufferflies_loop3

	LDMFD	r13,{r0-r3}

mdct_bitreverseARM:
	@ r0 = points = n
	@ r1 = in
	@ r2 = step
	@ r3 = shift

	MOV	r4, #0			@ r4 = bit = 0
	ADD	r5, r1, r0, LSL #1	@ r5 = w = x + (n>>1)
	ADR	r6, bitrev
	SUB	r5, r5, #8
brev_lp:
	LDRB	r7, [r6, r4, LSR #6]
	AND	r8, r4, #0x3f
	LDRB	r8, [r6, r8]
	ADD	r4, r4, #1		@ bit++
	@ stall XScale
	ORR	r7, r7, r8, LSL #6	@ r7 = bitrev[bit]
	MOV	r7, r7, LSR r3
	ADD	r9, r1, r7, LSL #2	@ r9 = xx = x + (b>>shift)
	CMP	r5, r9			@ if (w > xx)
	LDR	r10,[r5],#-8		@   r10 = w[0]		w -= 2
	LDRGT	r11,[r5,#12]		@   r11 = w[1]
	LDRGT	r12,[r9]		@   r12 = xx[0]
	LDRGT	r14,[r9,#4]		@   r14 = xx[1]
	STRGT	r10,[r9]		@   xx[0]= w[0]
	STRGT	r11,[r9,#4]		@   xx[1]= w[1]
	STRGT	r12,[r5,#8]		@   w[0] = xx[0]
	STRGT	r14,[r5,#12]		@   w[1] = xx[1]
	CMP	r5,r1
	BGT	brev_lp

	@ mdct_step7
	@ r0 = points
	@ r1 = in
	@ r2 = step
	@ r3 = shift

	CMP	r2, #4			@ r5 = T = (step>=4) ?
	ADR	r7, .Lsincos_lookup	@          sincos_lookup0 +
	ADDLT	r7, #4			@          sincos_lookup1
	LDR	r5, [r7]
	ADD	r5, r7
	ADD	r7, r1, r0, LSL #1	@ r7 = w1 = x + (n>>1)
	ADDGE	r5, r5, r2, LSL #1	@		            (step>>1)
	ADD	r8, r5, #1024*4		@ r8 = Ttop
step7_loop1:
	LDR	r6, [r1]		@ r6 = w0[0]
	LDR	r9, [r1,#4]		@ r9 = w0[1]
	LDR	r10,[r7,#-8]!		@ r10= w1[0]	w1 -= 2
	LDR	r11,[r7,#4]		@ r11= w1[1]
	LDR	r14,[r5,#4]		@ r14= T[1]
	LDR	r12,[r5],r2,LSL #2	@ r12= T[0]	T += step

	ADD	r6, r6, r10		@ r6 = s0 = w0[0] + w1[0]
	SUB	r10,r6, r10,LSL #1	@ r10= s1b= w0[0] - w1[0]
	SUB	r11,r11,r9		@ r11= s1 = w1[1] - w0[1]
	ADD	r9, r11,r9, LSL #1	@ r9 = s0b= w1[1] + w0[1]

	@ Can save 1 cycle by using SMULL SMLAL - at the cost of being
	@ 1 off.
	SMULL	r0, r3, r6, r14		@ (r0,r3)   = s0*T[1]
	SMULL	r0, r4, r11,r12		@ (r0,r4)  += s1*T[0] = s2
	ADD	r3, r3, r4
	SMULL	r0, r14,r11,r14		@ (r0,r14)  = s1*T[1]
	SMULL	r0, r12,r6, r12		@ (r0,r12) += s0*T[0] = s3
	SUB	r14,r14,r12

	@ r9 = s0b<<1
	@ r10= s1b<<1
	ADD	r9, r3, r9, ASR #1	@ r9 = s0b + s2
	SUB	r3, r9, r3, LSL #1	@ r3 = s0b - s2

	SUB	r12,r14,r10,ASR #1	@ r12= s3  - s1b
	ADD	r10,r14,r10,ASR #1	@ r10= s3  + s1b
	STR	r9, [r1],#4
	STR	r10,[r1],#4		@ w0 += 2
	STR	r3, [r7]
	STR	r12,[r7,#4]

	CMP	r5,r8
	BLT	step7_loop1

step7_loop2:
	LDR	r6, [r1]		@ r6 = w0[0]
	LDR	r9, [r1,#4]		@ r9 = w0[1]
	LDR	r10,[r7,#-8]!		@ r10= w1[0]	w1 -= 2
	LDR	r11,[r7,#4]		@ r11= w1[1]
	LDR	r14,[r5,-r2,LSL #2]!	@ r12= T[1]	T -= step
	LDR	r12,[r5,#4]		@ r14= T[0]

	ADD	r6, r6, r10		@ r6 = s0 = w0[0] + w1[0]
	SUB	r10,r6, r10,LSL #1	@ r10= s1b= w0[0] - w1[0]
	SUB	r11,r11,r9		@ r11= s1 = w1[1] - w0[1]
	ADD	r9, r11,r9, LSL #1	@ r9 = s0b= w1[1] + w0[1]

	@ Can save 1 cycle by using SMULL SMLAL - at the cost of being
	@ 1 off.
	SMULL	r0, r3, r6, r14		@ (r0,r3)   = s0*T[0]
	SMULL	r0, r4, r11,r12		@ (r0,r4)  += s1*T[1] = s2
	ADD	r3, r3, r4
	SMULL	r0, r14,r11,r14		@ (r0,r14)  = s1*T[0]
	SMULL	r0, r12,r6, r12		@ (r0,r12) += s0*T[1] = s3
	SUB	r14,r14,r12

	@ r9 = s0b<<1
	@ r10= s1b<<1
	ADD	r9, r3, r9, ASR #1	@ r9 = s0b + s2
	SUB	r3, r9, r3, LSL #1	@ r3 = s0b - s2

	SUB	r12,r14,r10,ASR #1	@ r12= s3  - s1b
	ADD	r10,r14,r10,ASR #1	@ r10= s3  + s1b
	STR	r9, [r1],#4
	STR	r10,[r1],#4		@ w0 += 2
	STR	r3, [r7]
	STR	r12,[r7,#4]

	CMP	r1,r7
	BLT	step7_loop2

	LDMFD	r13!,{r0-r3}

	@ r0 = points
	@ r1 = in
	@ r2 = step
	@ r3 = shift
	MOV	r2, r2, ASR #2		@ r2 = step >>= 2
	CMP	r2, #0
	CMPNE	r2, #1
	BEQ	mdct_end

	@ step > 1 (default case)
	CMP	r2, #4			@ r5 = T = (step>=4) ?
	ADR	r7, .Lsincos_lookup	@          sincos_lookup0 +
	ADDLT	r7, #4			@          sincos_lookup1
	LDR	r5, [r7]
	ADD	r5, r7
	ADD	r7, r1, r0, LSL #1	@ r7 = iX = x + (n>>1)
	ADDGE	r5, r5, r2, LSL #1	@		            (step>>1)
mdct_step8_default:
	LDR	r6, [r1],#4		@ r6 =  s0 = x[0]
	LDR	r8, [r1],#4		@ r8 = -s1 = x[1]
	LDR	r12,[r5,#4]       	@ r12= T[1]
	LDR	r14,[r5],r2,LSL #2	@ r14= T[0]	T += step
	RSB	r8, r8, #0		@ r8 = s1

	@ XPROD31(s0, s1, T[0], T[1], x, x+1)
	@ x[0] = s0 * T[0] + s1 * T[1]      x[1] = s1 * T[0] - s0 * T[1]
	SMULL	r9, r10, r8, r12	@ (r9,r10)  = s1 * T[1]
	CMP	r1, r7
	SMLAL	r9, r10, r6, r14	@ (r9,r10) += s0 * T[0]
	RSB	r6, r6, #0		@ r6 = -s0
	SMULL	r9, r11, r8, r14	@ (r9,r11)  = s1 * T[0]
	MOV	r10,r10,LSL #1
	SMLAL	r9, r11, r6, r12	@ (r9,r11) -= s0 * T[1]
	STR	r10,[r1,#-8]
	MOV	r11,r11,LSL #1
	STR	r11,[r1,#-4]
	BLT	mdct_step8_default

mdct_end:
	MOV	r0, r2
	LDMFD	r13!,{r4-r11,PC}

cPI1_8:
	.word	0x7641af3d
cPI2_8:
	.word	0x5a82799a
cPI3_8:
	.word	0x30fbc54d
bitrev:
	.byte	0
	.byte	32
	.byte	16
	.byte	48
	.byte	8
	.byte	40
	.byte	24
	.byte	56
	.byte	4
	.byte	36
	.byte	20
	.byte	52
	.byte	12
	.byte	44
	.byte	28
	.byte	60
	.byte	2
	.byte	34
	.byte	18
	.byte	50
	.byte	10
	.byte	42
	.byte	26
	.byte	58
	.byte	6
	.byte	38
	.byte	22
	.byte	54
	.byte	14
	.byte	46
	.byte	30
	.byte	62
	.byte	1
	.byte	33
	.byte	17
	.byte	49
	.byte	9
	.byte	41
	.byte	25
	.byte	57
	.byte	5
	.byte	37
	.byte	21
	.byte	53
	.byte	13
	.byte	45
	.byte	29
	.byte	61
	.byte	3
	.byte	35
	.byte	19
	.byte	51
	.byte	11
	.byte	43
	.byte	27
	.byte	59
	.byte	7
	.byte	39
	.byte	23
	.byte	55
	.byte	15
	.byte	47
	.byte	31
	.byte	63

.Lsincos_lookup:
	.word	sincos_lookup0-.Lsincos_lookup
	.word	sincos_lookup1-(.Lsincos_lookup+4)

	@ END
