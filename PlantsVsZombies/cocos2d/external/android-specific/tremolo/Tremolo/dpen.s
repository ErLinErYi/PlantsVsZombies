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

	.global	decode_packed_entry_number
	.global decode_packed_entry_number_REALSTART
	.global decode_map
	.global vorbis_book_decodevv_add
	.global _checksum

	.extern	oggpack_adv
	.extern	oggpack_look
	.extern	oggpack_eop
	.extern	crc_lookup
	.hidden	crc_lookup

decode_packed_entry_number_REALSTART:
dpen_nobits:
	MOV	r0,r5		@ r0 = b
	MOV	r1,#1		@ r1 = 1
	BL	oggpack_adv	@ oggpack_adv(b,1)      /* Force eop */
duff:
	MVN	r0,#0		@ return -1
	LDMFD	r13!,{r4-r8,r10,PC}

dpen_readfailed:
	SUBS	r4,r4,#1	@ r4 = --read
	BEQ	dpen_nobits
	MOV	r0,r5		@ r0 = b
	MOV	r1,r4		@ r1 = read
	ADR	r14,dpen_read_return
	B	oggpack_look

decode_packed_entry_number:
	@ r0 = codebook       *book
	@ r1 = oggpack_buffer *b
	STMFD	r13!,{r4-r8,r10,r14}

	LDMIA	r0,{r4,r6,r7}		@ r4 = read = book->max_length
					@ r6 = book->dec_table
					@ r7 = book->dec_method
	MOV	r5,r1		@ r5 = b

	MOV	r0,r5		@ r0 = b
	MOV	r1,r4		@ r1 = read
	BL	oggpack_look
dpen_read_return:
	CMP	r0,#0
	BLT	dpen_readfailed

	@ r0 = lok
	@ r4 = read
	@ r5 = b
	@ r6 = dec_table
	@ r7 = dec_method

	CMP	r7, #3
	BGT	meth4
	BEQ	meth3
	CMP	r7, #1
	BGT	meth2
	BEQ	meth1
meth0:
	RSB	r1, r4, #0		@ r1 = i-read = 0-read
	MOV	r7, #0			@ r7 = chase
m0_loop:
	MOVS	r0, r0, LSR #1		@ r0 = lok>>1   C = bottom bit
	ADC	r2, r6, r7, LSL #1	@ r8 = &t[chase*2+C]
	LDRB	r7, [r2]
	ADDS	r1, r1, #1		@ r1 = i-read++ (i-read<0 => i<read)
	@ stall Xscale
	CMPLT	r7, #0x80
	BLT	m0_loop
	AND	r7, r7, #0x7F		@ r7 = chase
	CMP	r1, #0			@ if (i-read >= 0) === (i >= read)
	MVNGT	r7, #0			@ if (i >= read) value to return = -1
	ADD	r1, r1, r4		@ r1 = i-read+read+1 = i +1
	MOV	r0, r5			@ r0 = b
	BL	oggpack_adv		@ oggpack_adv(b, i+1);
	MOV	r0, r7			@ return chase
	LDMFD	r13!,{r4-r8,r10,PC}

meth1:
	@ r0 = lok
	@ r4 = read
	@ r5 = b
	@ r6 = dec_table
	RSB	r1, r4, #0		@ r1 = i = -read
	MOV	r10,#0			@ r10= next = 0
m1_loop:
	MOV	r7, r10			@ r7 = chase=next
	MOVS	r0, r0, LSR #1		@ r0 = lok>>1     C = bottom bit
	ADC	r8, r6, r7		@ r8 = t+chase+bit
	LDRB	r10,[r8], -r6		@ r10= next=t[chase+bit] r8=chase+bit
	ADDS	r1, r1, #1		@ r1 = i++
	@ stall Xscale
	CMPLT	r10,#0x80		@ if (next & 0x80) == 0
	BLT	m1_loop

	ADD	r1, r1, r4		@ r1 = i+read
	MOV	r0, r5			@ r0 = b
	BL	oggpack_adv		@ oggpack_adv(b, i)

	CMP	r10,#0x80
	BLT	duff

	CMP	r8, r7			@ if bit==0 (chase+bit==chase) (sets C)
	LDRNEB	r14,[r6, r7]		@ r14= t[chase]
	MOVEQ	r14,#128
	ADC	r12,r8, r6		@ r12= chase+bit+1+t
	LDRB	r14,[r12,r14,LSR #7]	@ r14= t[chase+bit+1+(!bit || t[chase]0x0x80)]
	BIC	r10,r10,#0x80		@ r3 = next &= ~0x80
	@ stall Xscale
	ORR	r0, r14,r10,LSL #8	@ r7 = chase = (next<<8) | r14

	LDMFD	r13!,{r4-r8,r10,PC}


meth2:
	RSB	r1, r4, #0		@ r1 = i-read = 0-read
	MOV	r7, #0			@ r7 = chase
	MOV	r6, r6, LSR #1
m2_loop:
	MOVS	r0, r0, LSR #1		@ r0 = lok>>1   C = bottom bit
	ADC	r2, r6, r7, LSL #1	@ r8 = &t[chase*2+C]
	LDRH	r7, [r2, r2]
	ADDS	r1, r1, #1		@ r1 = i-read++ (i-read<0 => i<read)
	@ stall Xscale
	CMPLT	r7, #0x8000
	BLT	m2_loop
	BIC	r7, r7, #0x8000		@ r7 = chase
	CMP	r1, #0			@ if (i-read >= 0) === (i >= read)
	MVNGT	r7, #0			@ if (i >= read) value to return = -1
	ADD	r1, r1, r4		@ r1 = i-read+read+1 = i +1
	MOV	r0, r5			@ r0 = b
	BL	oggpack_adv		@ oggpack_adv(b, i+1);
	MOV	r0, r7			@ return chase
	LDMFD	r13!,{r4-r8,r10,PC}

meth3:
	@ r0 = lok
	@ r4 = read
	@ r5 = b
	@ r6 = dec_table
	RSB	r1, r4, #0		@ r1 = i = -read
	MOV	r10,#0			@ r10= next = 0
m3_loop:
	MOV	r7, r10			@ r7 = chase=next
	MOVS	r0, r0, LSR #1		@ r0 = lok>>1     C = bottom bit
	ADC	r8, r7, #0		@ r8 = chase+bit
	MOV	r8, r8, LSL #1		@ r8 = (chase+bit)<<1
	LDRH	r10,[r6, r8]		@ r10= next=t[chase+bit]
	ADDS	r1, r1, #1		@ r1 = i++
	@ stall Xscale
	CMPLT	r10,#0x8000		@ if (next & 0x8000) == 0
	BLT	m3_loop

	ADD	r1, r1, r4		@ r1 = i+read
	MOV	r0, r5			@ r0 = b
	BL	oggpack_adv		@ oggpack_adv(b, i)

	CMP	r10,#0x8000
	BLT	duff

	MOV	r7, r7, LSL #1
	CMP	r8, r7			@ if bit==0 (chase+bit==chase) sets C
	LDRNEH	r14,[r6, r7]		@ r14= t[chase]
	MOVEQ	r14,#0x8000
	ADC	r12,r8, r14,LSR #15	@ r12= 1+((chase+bit)<<1)+(!bit || t[chase]0x0x8000)
	ADC	r12,r12,r14,LSR #15	@ r12= t + (1+chase+bit+(!bit || t[chase]0x0x8000))<<1
	LDRH	r14,[r6, r12]		@ r14= t[chase+bit+1
	BIC	r10,r10,#0x8000		@ r3 = next &= ~0x8000
	@ stall Xscale
	ORR	r0, r14,r10,LSL #16	@ r7 = chase = (next<<16) | r14

	LDMFD	r13!,{r4-r8,r10,PC}

meth4:
	RSB	r1, r4, #0		@ r1 = i-read = 0-read
	MOV	r7, #0			@ r7 = chase
m4_loop:
	MOVS	r0, r0, LSR #1		@ r0 = lok>>1   C = bottom bit
	ADC	r2, r7, r7		@ r8 = chase*2+C
	LDR	r7, [r6, r2, LSL #2]
	ADDS	r1, r1, #1		@ r1 = i-read++ (i-read<0 => i<read)
	@ stall Xscale
	CMPLT	r7, #0x80000000
	BLT	m4_loop
	BIC	r7, r7, #0x80000000	@ r7 = chase
	CMP	r1, #0			@ if (i-read >= 0) === (i >= read)
	MVNGT	r7, #0			@ if (i >= read) value to return = -1
	ADD	r1, r1, r4		@ r1 = i-read+read+1 = i +1
	MOV	r0, r5			@ r0 = b
	BL	oggpack_adv		@ oggpack_adv(b, i+1);
	MOV	r0, r7			@ return chase
	LDMFD	r13!,{r4-r8,r10,PC}

decode_map:
	@ r0 = codebook *s
	@ r1 = oggpack_buffer *b
	@ r2 = int v
	@ r3 = int point
	STMFD	r13!,{r4-r11,r14}

	MOV	r4, r0		@ r4 = s
	MOV	r5, r1		@ r5 = b
	MOV	r6, r2		@ r6 = v
	MOV	r7, r3		@ r7 = point
	BL	decode_packed_entry_number
	MOV	r8, r0

	MOV	r0, r5
	BL	oggpack_eop
	CMP	r0, #0
	BNE	dm_duff

	@ r4 = s
	@ r5 = b
	@ r6 = v
	@ r7 = point
	@ r8 = entry

	LDR	r1, [r4,#12]	@ r1 = s->dec_type
	LDR	r2, [r4,#16]	@ r2 = s->q_bits
	LDR	r3, [r4,#20]	@ r3 = s->dim
	LDR	r5, [r4,#24]	@ r5 = s->q_delp
	LDR	r11,[r4,#28]	@ r11= s->q_minp
	LDR	r12,[r4,#32]	@ r12= s->q_del = mul
	LDR	r14,[r4,#36]	@ r14= s->q_min
	SUBS	r11,r7, r11	@ r11= add    = point - s->q_minp

	MOVGT	r14,r14,ASR r11	@ r14= add = s->q_min >> add  (if add >0)
	RSBLT	r11,r11,#0
	MOVLT	r14,r14,LSL r11	@ r14= add = s->q_min << -add (if add < 0)

	SUBS	r5, r7, r5	@ r5 = shiftM = point - s->q_delp
	LDR	r7, [r4,#40]	@ r7 = s->q_seq
	RSBLT	r5, r5, #0	@ if (shiftM<0)  r5 =-shiftM
	MOVLT	r12,r12,LSL r5	@                r12=mul<<-shiftM
	MOVLT	r5, #0		@                r5 =shiftM = 0
	MOVGT	r14,r14,LSL r5	@ add <<= shiftM

	CMP	r7,#0		@ seqMask = (s->q_seq?-1:0)
	MVNNE	r7,#0

	CMP	r1, #2
	BEQ	dm2
	BGT	dm3
	CMP	r1,#0		@ probably never happens
	BLE	dm_duff
dm1:
	@ r1 = s->dec_type
	@ r2 = s->q_bits
	@ r3 = s->dim
	@ r5 = shiftM
	@ r6 = v
	@ r7 = seqMask
	@ r8 = entry
	@ r12= mul
	@ r14= add
	MOV	r0, #1
	RSB	r0, r0, r0, LSL r2	@ r0 = mask = (1<<s->q_bits)-1
	MOV	r11,#0			@ r11= prev = 0
dm1_loop:
	AND	r1, r8, r0		@ r1 = v = entry & mask
	MLA	r1, r12, r1, r14	@ r1 = (add + mul*v)
	MOV	r8, r8, LSR r2		@ r8 = entry>>s->q_bits
	SUBS	r3, r3, #1
	ADD	r1, r11,r1, ASR r5	@ r1 = v = prev+((add+mul*v)>>shiftM)
	AND	r11,r1, r7		@ r11= prev = seqMask & v
	STR	r1, [r6], #4		@ *v++ = v
	BGT	dm1_loop

	MOV	r0, #0
	LDMFD	r13!,{r4-r11,PC}
dm2:
	@ r1 = s->dec_type
	@ r2 = s->q_bits
	@ r3 = s->dim
	@ r4 = s
	@ r5 = shiftM
	@ r6 = v
	@ r7 = seqMask
	@ r8 = entry
	@ r12= mul
	@ r14= add
	LDR	r1, [r4,#44]		@ r1 = s->q_pack
	LDR	r4, [r4,#48]		@ r4 = s->q_val
	MOV	r11,#0			@ r11= prev
	MOV	r0, #1
	RSB	r0, r0, r0, LSL r1	@ r8 = mask = (1<<s->q_pack)-1
	CMP	r2,#8
	BGT	dm2_hword
dm2_loop:
	AND	r2, r8, r0		@ r2 = entry & mask
	LDRB	r2, [r4, r2]		@ r2 = v = q->val[entry & mask]
	MOV	r8, r8, LSR r1		@ r8 = entry>>q_pack
	MLA	r2, r12,r2, r14		@ r2 = (add+mul*v)
	SUBS	r3, r3, #1
	ADD	r2, r11,r2, ASR r5	@ r2 = v = prev+(add+mul*v)>>shiftM
	AND	r11,r2, r7		@ r11= prev = seqMask & v
	STR	r2, [r6], #4		@ *v++ = v
	BGT	dm2_loop
	MOV	r0, #0
	LDMFD	r13!,{r4-r11,PC}

dm2_hword:
	AND	r2, r8, r0		@ r2 = entry & mask
	MOV	r2, r2, LSL #1		@ r2 = 2*r2
	LDRH	r2, [r4, r2]		@ r2 = v = q->val[entry & mask]
	MOV	r8, r8, LSR r1		@ r8 = entry>>q_pack
	MLA	r2, r12,r2, r14		@ r2 = (add+mul*v)
	SUBS	r3, r3, #1
	ADD	r2, r11,r2, ASR r5	@ r2 = v = prev+(add+mul*v)>>shiftM
	AND	r11,r2, r7		@ r11= prev = seqMask & v
	STR	r2, [r6], #4		@ *v++ = v
	BGT	dm2_hword
	MOV	r0, #0
	LDMFD	r13!,{r4-r11,PC}

dm3:
	@ r1 = s->dec_type
	@ r2 = s->q_bits
	@ r3 = s->dim
	@ r4 = s
	@ r5 = shiftM
	@ r6 = v
	@ r7 = seqMask
	@ r8 = entry
	@ r12= mul
	@ r14= add
	LDR	r1, [r4,#44]		@ r1 = s->q_pack
	LDR	r4, [r4,#52]		@ r4 = s->q_val
	CMP	r2,#8
	MOV	r11,#0			@ r11= prev
	MLA	r4,r1,r8,r4		@ r4 = ptr = s->q_val+entry*s->q_pack

	BGT	dm3_hword
dm3_loop:
	LDRB	r2, [r4], #1		@ r2 = v = *ptr++
	SUBS	r3, r3, #1
	MLA	r2, r12,r2, r14		@ r2 = (add+mul*v)
	ADD	r2, r11,r2, ASR r5	@ r2 = v = prev+(add+mul*v)>>shiftM
	AND	r11,r2, r7		@ r11= prev = seqMask & v
	STR	r2, [r6], #4		@ *v++ = v
	BGT	dm3_loop
	MOV	r0, #0
	LDMFD	r13!,{r4-r11,PC}

dm3_hword:
	LDRH	r2, [r4], #2		@ r2 = *ptr++
	SUBS	r3, r3, #1
	MLA	r2, r12,r2, r14		@ r2 = (add+mul*v)
	ADD	r2, r11,r2, ASR r5	@ r2 = v = prev+(add+mul*v)>>shiftM
	AND	r11,r2, r7		@ r11= prev = seqMask & v
	STR	r2, [r6], #4		@ *v++ = v
	BGT	dm3_hword
	MOV	r0, #0
	LDMFD	r13!,{r4-r11,PC}

dm_duff:
	MVN	r0,#0
	LDMFD	r13!,{r4-r11,PC}

vorbis_book_decodevv_add:
	@ r0 = codebook     *book
	@ r1 = ogg_int32_t **a
	@ r2 = long          offset
	@ r3 = int           ch
	@ <> = b
	@ <> = n
	@ <> = point
	STMFD	r13!,{r4-r11,R14}
	LDR	r7, [r0, #13*4]		@ r7 = used_entries
	MOV	r9, r0			@ r9 = book
	MOV	r10,r1			@ r10= 0xa[chptr]      chptr=0
	MOV	r6, r3			@ r6 = ch
	ADD	r8, r10,r3, LSL #2	@ r8 = 0xa[ch]
	MOV	r11,r2			@ r11= offset
	CMP	r7, #0			@ if (used_entries <= 0)
	BLE	vbdvva_exit		@     exit
	LDR	r5, [r13,#10*4]		@ r5 = n
vbdvva_loop1:
	@ r5 = n
	@ r6 = ch
	@ r8 = 0xa[ch]
	@ r9 = book
	@ r10= 0xa[chptr]
	@ r11= offset
	MOV	r0, r9			@ r0 = book
	LDR	r1, [r13,# 9*4]		@ r1 = b
	LDR	r2, [r9, #14*4]		@ r2 = v = dec_buf
	LDR	r3, [r13,#11*4]		@ r3 = point
	BL	decode_map
	CMP	r0, #0
	BNE	vbdvva_fail

	LDR	r0, [r9, # 5*4]		@ r0 = book->dim
	LDR	r1, [r9, #14*4]		@ r1 = v = dec_buf
vbdvva_loop2:
	LDR	r2, [r10],#4		@ r2 = a[chptr++]
	LDR	r12,[r1], #4		@ r1 = v[j++]
	CMP	r10,r8			@ if (chptr == ch)
	SUBEQ	r10,r10,r6, LSL #2	@    chptr = 0
	LDR	r14,[r2, r11,LSL #2]!	@ r2 = 0xa[chptr++][i] r14=[r12]
	ADDEQ	r11,r11,#1		@    i++
	SUBEQ	r5, r5, #1		@    n--
	SUBS	r0, r0, #1		@ r0--
	ADD	r12,r12,r14		@ r12= a[chptr++][i]+ v[j]
	STR	r12,[r2]		@ r12= a[chptr++][i]+=v[j]
	BGT	vbdvva_loop2
	CMP	r5,#0
	BGT	vbdvva_loop1
vbdvva_exit:
	MOV	r0, #0			@ return 0
	LDMFD	r13!,{r4-r11,PC}
vbdvva_fail:
	MVN	r0, #0			@ return -1
	LDMFD	r13!,{r4-r11,PC}

_checksum:
	@ r0 = ogg_reference *or
	@ r1 = bytes
	STMFD	r13!,{r5-r6,r14}

	ADR	r6,.Lcrc_lookup
	LDR	r5,[r6]
	ADD	r5,r6
	MOV	r14,#0			@ r14= crc_reg = 0
	MOVS	r12,r0
	BEQ	_cs_end
_cs_loop1:
	LDMIA	r12,{r0,r2,r3,r12}	@ r0 = or->buffer
					@ r2 = or->begin
					@ r3 = or->length
					@ r12= or->next
	LDR	r0,[r0]			@ r0 = or->buffer->data
	CMP	r1,r3			@ r3 = post = (bytes < or->length ?
	MOVLT	r3,r1			@              bytes : or->length)
	MOVS	r6,r3			@ r6 = j = post
	BEQ	_cs_no_bytes
	ADD	r0,r0,r2		@ r0 = or->buffer->data + or->begin
_cs_loop2:
	LDRB	r2, [r0],#1		@ r2 = data[j]
	@ stall
	@ stall Xscale
	EOR	r2, r2, r14,LSR #24	@ r2 = (crc_reg>>24)^data[j]
	LDR	r2, [r5, r2, LSL #2]	@ r2 = crc_lkp[(crc_reg>>24)^data[j]]
	SUBS	r6, r6, #1		@ j--
	@ stall Xscale
	EOR	r14,r2, r14,LSL #8	@ r14= crc_reg = (crc_reg<<8)^r2
	BGT	_cs_loop2
_cs_no_bytes:
	SUBS	r1, r1, r3
	CMPNE	r12,#0
	BNE	_cs_loop1
_cs_end:
	MOV	r0,r14
	LDMFD	r13!,{r5-r6,PC}

.Lcrc_lookup:
        .WORD   crc_lookup-.Lcrc_lookup

	@ END
