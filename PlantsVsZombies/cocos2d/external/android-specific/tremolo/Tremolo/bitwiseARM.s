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

	.global	oggpack_look
	.global	oggpack_adv
	.global	oggpack_readinit
	.global	oggpack_read

oggpack_look:
	@ r0 = oggpack_buffer *b
	@ r1 = int             bits
	STMFD	r13!,{r10,r11,r14}
	LDMIA	r0,{r2,r3,r12}
					@ r2 = bitsLeftInSegment
					@ r3 = ptr
					@ r12= bitsLeftInWord
	SUBS	r2,r2,r1		@ bitsLeftinSegment -= bits
	BLT	look_slow		@ Not enough bits in this segment for
					@ this request. Do it slowly.
	LDR	r10,[r3]		@ r10= ptr[0]
	RSB	r14,r12,#32		@ r14= 32-bitsLeftInWord
	SUBS	r12,r12,r1		@ r12= bitsLeftInWord -= bits
	LDRLT	r11,[r3,#4]!		@ r11= ptr[1]
	MOV	r10,r10,LSR r14		@ r10= ptr[0]>>(32-bitsLeftInWord)
	ADDLE	r12,r12,#32		@ r12= bitsLeftInWord += 32
	RSB	r14,r14,#32		@ r14= 32-bitsLeftInWord
	ORRLT	r10,r10,r11,LSL r14	@ r10= Next 32 bits.
	MOV	r14,#1
	RSB	r14,r14,r14,LSL r1
	AND	r0,r10,r14
	LDMFD	r13!,{r10,r11,PC}

look_slow:
	STMFD	r13!,{r5,r6}
	ADDS	r10,r2,r1		@ r10= bitsLeftInSegment + bits (i.e.
					@ the initial value of bitsLeftInSeg)
	@ r10 = bitsLeftInSegment (initial)
	@ r12 = bitsLeftInWord
	RSB	r14,r12,#32		@ r14= 32-bitsLeftInWord
	MOV	r5,r10			@ r5 = bitsLeftInSegment (initial)
	BLT	look_overrun
	BEQ	look_next_segment	@ r10= r12 = 0, if we branch
	CMP	r12,r10			@ If bitsLeftInWord < bitsLeftInSeg
					@ there must be more in the next word
	LDR	r10,[r3],#4		@ r10= ptr[0]
	LDRLT	r6,[r3]			@ r6 = ptr[1]
	MOV	r11,#1
	MOV	r10,r10,LSR r14		@ r10= first bitsLeftInWord bits
	ORRLT	r10,r10,r6,LSL r12	@ r10= first bitsLeftInSeg bits+crap
	RSB	r11,r11,r11,LSL r5	@ r11= mask
	AND	r10,r10,r11		@ r10= first r5 bits
	@ Load the next segments data
look_next_segment:
	@ At this point, r10 contains the first r5 bits of the result
	LDR	r11,[r0,#12]		@ r11= head = b->head
	@ Stall
	@ Stall
look_next_segment_2:
	LDR	r11,[r11,#12]		@ r11= head = head->next
	@ Stall
	@ Stall
	CMP	r11,#0
	BEQ	look_out_of_data
	LDMIA	r11,{r6,r12,r14}	@ r6 = buffer
					@ r12= begin
					@ r14= length
	LDR	r6,[r6]			@ r6 = buffer->data
	CMP	r14,#0
	BEQ	look_next_segment_2
	ADD	r6,r6,r12		@ r6 = buffer->data+begin
look_slow_loop:
	LDRB	r12,[r6],#1		@ r12= *buffer
	SUBS	r14,r14,#1		@ r14= length
	@ Stall
	ORR	r10,r10,r12,LSL r5	@ r10= first r5+8 bits
	ADD	r5,r5,#8
	BLE	look_really_slow
	CMP	r5,r1
	BLT	look_slow_loop
	MOV	r14,#1
	RSB	r14,r14,r14,LSL r1
	AND	r0,r10,r14
	LDMFD	r13!,{r5,r6,r10,r11,PC}


look_really_slow:
	CMP	r5,r1
	BLT	look_next_segment_2
	MOV	r14,#1
	RSB	r14,r14,r14,LSL r1
	AND	r0,r10,r14
	LDMFD	r13!,{r5,r6,r10,r11,PC}

look_out_of_data:
	@MVN	r0,#0			; return -1
	MOV	r0,#0
	LDMFD	r13!,{r5,r6,r10,r11,PC}

look_overrun:
	@ We had overrun when we started, so we need to skip -r10 bits.
	LDR	r11,[r0,#12]		@ r11 = head = b->head
	@ stall
	@ stall
look_overrun_next_segment:
	LDR	r11,[r11,#12]		@ r11 = head->next
	@ stall
	@ stall
	CMP	r11,#0
	BEQ	look_out_of_data
	LDMIA	r11,{r6,r7,r14}		@ r6 = buffer
					@ r7 = begin
					@ r14= length
	LDR	r6,[r6]			@ r6 = buffer->data
	@ stall
	@ stall
	ADD	r6,r6,r7		@ r6 = buffer->data+begin
	MOV	r14,r14,LSL #3		@ r14= length in bits
	ADDS	r14,r14,r10		@ r14= length in bits-bits to skip
	MOVLE	r10,r14
	BLE	look_overrun_next_segment
	RSB	r10,r10,#0		@ r10= bits to skip
	ADD	r6,r10,r10,LSR #3	@ r6 = pointer to data
	MOV	r10,#0
	B	look_slow_loop

oggpack_adv:
	@ r0 = oggpack_buffer *b
	@ r1 = bits
	LDMIA	r0,{r2,r3,r12}
					@ r2 = bitsLeftInSegment
					@ r3 = ptr
					@ r12= bitsLeftInWord
	SUBS	r2,r2,r1		@ Does this run us out of bits in the
	BLE	adv_slow		@ segment? If so, do it slowly
	SUBS	r12,r12,r1
	ADDLE	r12,r12,#32
	ADDLE	r3,r3,#4
	STMIA	r0,{r2,r3,r12}
	BX      LR
adv_slow:
	STMFD	r13!,{r10,r14}

	LDR	r14,[r0,#12]		@ r14= head
	@ stall
adv_slow_loop:
	LDR	r1,[r0,#20]		@ r1 = count
	LDR	r10,[r14,#8]		@ r10= head->length
	LDR	r14,[r14,#12]		@ r14= head->next
	@ stall
	ADD	r1,r1,r10		@ r1 = count += head->length
	CMP	r14,#0
	BEQ	adv_end
	STR	r1,[r0,#20]		@ b->count = count
	STR	r14,[r0,#12]		@ b->head = head
	LDMIA	r14,{r3,r10,r12}	@ r3 = buffer
					@ r10= begin
					@ r12= length
	LDR	r3,[r3]			@ r3 = buffer->data
	ADD	r3,r3,r10		@ r3 = Pointer to start (byte)
	AND	r10,r3,#3		@ r10= bytes to backtrk to word align
	MOV	r10,r10,LSL #3		@ r10= bits to backtrk to word align
	RSB	r10,r10,#32		@ r10= bits left in word
	ADDS	r10,r10,r2		@ r10= bits left in word after skip
	ADDLE	r10,r10,#32
	ADDLE	r3,r3,#4
	BIC	r3,r3,#3		@ r3 = Pointer to start (word)
	ADDS	r2,r2,r12,LSL #3	@ r2 = length in bits after advance
	BLE	adv_slow_loop
	STMIA	r0,{r2,r3,r10}

	LDMFD	r13!,{r10,PC}
adv_end:
	MOV	r2, #0
	MOV	r12,#0
	STMIA	r0,{r2,r3,r12}

	LDMFD	r13!,{r10,PC}

oggpack_readinit:
	@ r0 = oggpack_buffer *b
	@ r1 = oggreference   *r
	STR	r1,[r0,#12]		@ b->head = r1
	STR	r1,[r0,#16]		@ b->tail = r1
	LDMIA	r1,{r2,r3,r12}		@ r2 = b->head->buffer
					@ r3 = b->head->begin
					@ r12= b->head->length
	LDR	r2,[r2]			@ r2 = b->head->buffer->data
	MOV	r1,r12,LSL #3		@ r1 = BitsInSegment
	MOV	r12,#0
	ADD	r3,r2,r3		@ r3 = r2+b->head->begin
	BIC	r2,r3,#3		@ r2 = b->headptr (word)
	AND	r3,r3,#3
	MOV	r3,r3,LSL #3
	RSB	r3,r3,#32		@ r3 = BitsInWord
	STMIA	r0,{r1,r2,r3}
	STR	r12,[r0,#20]
	BX      LR

oggpack_read:
	@ r0 = oggpack_buffer *b
	@ r1 = int             bits
	STMFD	r13!,{r10,r11,r14}
	LDMIA	r0,{r2,r3,r12}
					@ r2 = bitsLeftInSegment
					@ r3 = ptr
					@ r12= bitsLeftInWord
	SUBS	r2,r2,r1		@ bitsLeftinSegment -= bits
	BLT	read_slow		@ Not enough bits in this segment for
					@ this request. Do it slowly.
	LDR	r10,[r3]		@ r10= ptr[0]
	RSB	r14,r12,#32		@ r14= 32-bitsLeftInWord
	SUBS	r12,r12,r1		@ r12= bitsLeftInWord -= bits
	ADDLE	r3,r3,#4
	LDRLT	r11,[r3]		@ r11= ptr[1]
	MOV	r10,r10,LSR r14		@ r10= ptr[0]>>(32-bitsLeftInWord)
	ADDLE	r12,r12,#32		@ r12= bitsLeftInWord += 32
	RSB	r14,r14,#32		@ r14= 32-bitsLeftInWord
	ORRLT	r10,r10,r11,LSL r14	@ r10= Next 32 bits.
	STMIA	r0,{r2,r3,r12}
	MOV	r14,#1
	RSB	r14,r14,r14,LSL r1
	AND	r0,r10,r14
	LDMFD	r13!,{r10,r11,PC}

read_slow:
	STMFD	r13!,{r5,r6}
	ADDS	r10,r2,r1		@ r10= bitsLeftInSegment + bits (i.e.
					@ the initial value of bitsLeftInSeg)
	@ r10 = bitsLeftInSegment (initial)
	@ r12 = bitsLeftInWord
	RSB	r14,r12,#32		@ r14= 32-bitsLeftInWord
	MOV	r5,r10			@ r5 = bitsLeftInSegment (initial)
	BLT	read_overrun
	BEQ	read_next_segment	@ r10= r12 = 0, if we branch
	CMP	r12,r10			@ If bitsLeftInWord < bitsLeftInSeg
					@ there must be more in the next word
	LDR	r10,[r3],#4		@ r10= ptr[0]
	LDRLT	r6,[r3]			@ r6 = ptr[1]
	MOV	r11,#1
	MOV	r10,r10,LSR r14		@ r10= first bitsLeftInWord bits
	ORRLT	r10,r10,r6,LSL r12	@ r10= first bitsLeftInSeg bits+crap
	RSB	r11,r11,r11,LSL r5	@ r11= mask
	AND	r10,r10,r11		@ r10= first r5 bits
	@ Load the next segments data
read_next_segment:
	@ At this point, r10 contains the first r5 bits of the result
	LDR	r11,[r0,#12]		@ r11= head = b->head
	@ Stall
read_next_segment_2:
	@ r11 = head
	LDR	r6,[r0,#20]		@ r6 = count
	LDR	r12,[r11,#8]		@ r12= length
	LDR	r11,[r11,#12]		@ r11= head = head->next
	@ Stall
	ADD	r6,r6,r12		@ count += length
	CMP	r11,#0
	BEQ	read_out_of_data
	STR	r11,[r0,#12]
	STR	r6,[r0,#20]		@ b->count = count
	LDMIA	r11,{r6,r12,r14}	@ r6 = buffer
					@ r12= begin
					@ r14= length
	LDR	r6,[r6]			@ r6 = buffer->data
	CMP	r14,#0
	BEQ	read_next_segment_2
	ADD	r6,r6,r12		@ r6 = buffer->data+begin
read_slow_loop:
	LDRB	r12,[r6],#1		@ r12= *buffer
	SUBS	r14,r14,#1		@ r14= length
	@ Stall
	ORR	r10,r10,r12,LSL r5	@ r10= first r5+8 bits
	ADD	r5,r5,#8
	BLE	read_really_slow
	CMP	r5,r1
	BLT	read_slow_loop
read_end:
	MOV	r12,#1
	RSB	r12,r12,r12,LSL r1

	@ Store back the new position
	@ r2 = -number of bits to go from this segment
	@ r6 = ptr
	@ r14= bytesLeftInSegment
	@ r11= New head value
	LDMIA	r11,{r3,r6,r14}		@ r3 = buffer
					@ r6 = begin
					@ r14= length
	LDR	r3,[r3]			@ r3 = buffer->data
	ADD	r1,r2,r14,LSL #3	@ r1 = bitsLeftInSegment
	@ stall
	ADD	r6,r3,r6		@ r6 = pointer
	AND	r3,r6,#3		@ r3 = bytes used in first word
	RSB	r3,r2,r3,LSL #3		@ r3 = bits used in first word
	BIC	r2,r6,#3		@ r2 = word ptr
	RSBS	r3,r3,#32		@ r3 = bitsLeftInWord
	ADDLE	r3,r3,#32
	ADDLE	r2,r2,#4
	STMIA	r0,{r1,r2,r3}

	AND	r0,r10,r12
	LDMFD	r13!,{r5,r6,r10,r11,PC}


read_really_slow:
	CMP	r5,r1
	BGE	read_end
	LDR	r14,[r11,#8]		@ r14= length of segment just done
	@ stall
	@ stall
	ADD	r2,r2,r14,LSL #3	@ r2 = -bits to use from next seg
	B	read_next_segment_2

read_out_of_data:
	@ Store back the new position
	@ r2 = -number of bits to go from this segment
	@ r6 = ptr
	@ r14= bytesLeftInSegment
	@ RJW: This may be overkill - we leave the buffer empty, with -1
	@ bits left in it. We might get away with just storing the
	@ bitsLeftInSegment as -1.
	LDR	r11,[r0,#12]		@ r11=head

	LDMIA	r11,{r3,r6,r14}		@ r3 = buffer
					@ r6 = begin
					@ r14= length
	LDR	r3,[r3]			@ r3 = buffer->data
	ADD	r6,r3,r6		@ r6 = pointer
	ADD	r6,r6,r14
	AND	r3,r6,#3		@ r3 = bytes used in first word
	MOV	r3,r3,LSL #3		@ r3 = bits used in first word
	BIC	r2,r6,#3		@ r2 = word ptr
	RSBS	r3,r3,#32		@ r3 = bitsLeftInWord
	MVN	r1,#0			@ r1 = -1 = bitsLeftInSegment
	STMIA	r0,{r1,r2,r3}
	@MVN	r0,#0			; return -1
	MOV	r0,#0
	LDMFD	r13!,{r5,r6,r10,r11,PC}

read_overrun:
	@ We had overrun when we started, so we need to skip -r10 bits.
	LDR	r11,[r0,#12]		@ r11 = head = b->head
	@ stall
	@ stall
read_overrun_next_segment:
	LDR	r11,[r11,#12]		@ r11 = head->next
	@ stall
	@ stall
	CMP	r11,#0
	BEQ	read_out_of_data
	LDMIA	r11,{r6,r7,r14}		@ r6 = buffer
					@ r7 = begin
					@ r14= length
	LDR	r6,[r6]			@ r6 = buffer->data
	@ stall
	@ stall
	ADD	r6,r6,r7		@ r6 = buffer->data+begin
	MOV	r14,r14,LSL #3		@ r14= length in bits
	ADDS	r14,r14,r10		@ r14= length in bits-bits to skip
	MOVLE	r10,r14
	BLE	read_overrun_next_segment
	RSB	r10,r10,#0		@ r10= bits to skip
	ADD	r6,r10,r10,LSR #3	@ r6 = pointer to data
	MOV	r10,#0
	B	read_slow_loop

	@ END
