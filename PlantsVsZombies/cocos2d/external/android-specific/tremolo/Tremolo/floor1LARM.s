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

	.global	render_lineARM

render_lineARM:
	@ r0 = n
	@ r1 = d
	@ r2 = floor
	@ r3 = base
	@ <> = err
	@ <> = adx
	@ <> = ady
	MOV	r12,r13
	STMFD	r13!,{r4-r6,r11,r14}
	LDMFD	r12,{r11,r12,r14}	@ r11 = err
					@ r12 = adx
					@ r14 = ady
rl_loop:
	LDR	r4, [r1]		@ r4 = *d
	LDR	r5, [r2], r3,LSL #2	@ r5 = *floor    r2 = floor+base
	SUBS	r11,r11,r14		@ err -= ady
	MOV	r4, r4, ASR #6
	MUL	r5, r4, r5		@ r5 = MULT31_SHIFT15
	ADDLT	r11,r11,r12		@ if (err < 0) err+=adx
	ADDLT	r2, r2, #4		@              floor+=1
	SUBS	r0, r0, #1
	STR	r5, [r1], #4
	BGT	rl_loop

	LDMFD	r13!,{r4-r6,r11,PC}

	@ END
