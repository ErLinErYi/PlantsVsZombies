/************************************************************************
 * Copyright (C) 2002-2009, Xiph.org Foundation
 * Copyright (C) 2010, Robin Watts for Pinknoise Productions Ltd
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the names of the Xiph.org Foundation nor Pinknoise
 * Productions Ltd nor the names of its contributors may be used to
 * endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 ************************************************************************

 function: arm7 and later wide math functions

 ************************************************************************/

#ifdef _ARM_ASSEM_

#if !defined(_V_WIDE_MATH) && !defined(_LOW_ACCURACY_)
#define _V_WIDE_MATH

static inline ogg_int32_t MULT32(ogg_int32_t x, ogg_int32_t y) {
  int lo,hi;
  asm volatile("smull\t%0, %1, %2, %3"
               : "=&r"(lo),"=&r"(hi)
               : "%r"(x),"r"(y)
	       : "cc");
  return(hi);
}

static inline ogg_int32_t MULT31(ogg_int32_t x, ogg_int32_t y) {
  return MULT32(x,y)<<1;
}

static inline ogg_int32_t MULT31_SHIFT15(ogg_int32_t x, ogg_int32_t y) {
  int lo,hi;
  asm volatile("smull	%0, %1, %2, %3\n\t"
	       "movs	%0, %0, lsr #15\n\t"
	       "adc	%1, %0, %1, lsl #17\n\t"
               : "=&r"(lo),"=&r"(hi)
               : "%r"(x),"r"(y)
	       : "cc");
  return(hi);
}

#define MB() asm volatile ("" : : : "memory")

static inline void XPROD32(ogg_int32_t  a, ogg_int32_t  b,
			   ogg_int32_t  t, ogg_int32_t  v,
			   ogg_int32_t *x, ogg_int32_t *y)
{
  int x1, y1, l;
  asm(	"smull	%0, %1, %4, %6\n\t"
	"smlal	%0, %1, %5, %7\n\t"
	"rsb	%3, %4, #0\n\t"
	"smull	%0, %2, %5, %6\n\t"
	"smlal	%0, %2, %3, %7"
	: "=&r" (l), "=&r" (x1), "=&r" (y1), "=r" (a)
	: "3" (a), "r" (b), "r" (t), "r" (v)
	: "cc" );
  *x = x1;
  MB();
  *y = y1;
}

/* x = (a*t + b*v)>>31,    y = (b*t - a*v)>>31 */
static inline void XPROD31(ogg_int32_t  a, ogg_int32_t  b,
			   ogg_int32_t  t, ogg_int32_t  v,
			   ogg_int32_t *x, ogg_int32_t *y)
{
  int x1, y1, l;
  asm(	"smull	%0, %1, %4, %6\n\t"
	"smlal	%0, %1, %5, %7\n\t"
	"rsb	%3, %4, #0\n\t"
	"smull	%0, %2, %5, %6\n\t"
	"smlal	%0, %2, %3, %7"
	: "=&r" (l), "=&r" (x1), "=&r" (y1), "=r" (a)
	: "3" (a), "r" (b), "r" (t), "r" (v)
	: "cc" );
  *x = x1 << 1;
  MB();
  *y = y1 << 1;
}

/* x = (a*t - b*v)>>31,     y = (b*t + a*v)>>31 */
static inline void XNPROD31(ogg_int32_t  a, ogg_int32_t  b,
			    ogg_int32_t  t, ogg_int32_t  v,
			    ogg_int32_t *x, ogg_int32_t *y)
{
  int x1, y1, l;
  asm(	"rsb	%2, %4, #0\n\t"
	"smull	%0, %1, %3, %5\n\t"
	"smlal	%0, %1, %2, %6\n\t"
	"smull	%0, %2, %4, %5\n\t"
	"smlal	%0, %2, %3, %6"
	: "=&r" (l), "=&r" (x1), "=&r" (y1)
	: "r" (a), "r" (b), "r" (t), "r" (v)
	: "cc" );
  *x = x1 << 1;
  MB();
  *y = y1 << 1;
}

#endif

#ifndef _V_CLIP_MATH
#define _V_CLIP_MATH

static inline ogg_int32_t CLIP_TO_15(ogg_int32_t x) {
  int tmp;
  asm volatile("subs	%1, %0, #32768\n\t"
	       "movpl	%0, #0x7f00\n\t"
	       "orrpl	%0, %0, #0xff\n"
	       "adds	%1, %0, #32768\n\t"
	       "movmi	%0, #0x8000"
	       : "+r"(x),"=r"(tmp)
	       :
	       : "cc");
  return(x);
}

#endif

#ifndef _V_LSP_MATH_ASM
#define _V_LSP_MATH_ASM

static inline void lsp_loop_asm(ogg_uint32_t *qip,ogg_uint32_t *pip,
				ogg_int32_t *qexpp,
				ogg_int32_t *ilsp,ogg_int32_t wi,
				ogg_int32_t m){

  ogg_uint32_t qi=*qip,pi=*pip;
  ogg_int32_t qexp=*qexpp;

  asm("mov     r0,%3;"
      "mov     r1,%5,asr#1;"
      "add     r0,r0,r1,lsl#3;"
      "1:"

      "ldmdb   r0!,{r1,r3};"
      "subs    r1,r1,%4;"          //ilsp[j]-wi
      "rsbmi   r1,r1,#0;"          //labs(ilsp[j]-wi)
      "umull   %0,r2,r1,%0;"       //qi*=labs(ilsp[j]-wi)

      "subs    r1,r3,%4;"          //ilsp[j+1]-wi
      "rsbmi   r1,r1,#0;"          //labs(ilsp[j+1]-wi)
      "umull   %1,r3,r1,%1;"       //pi*=labs(ilsp[j+1]-wi)

      "cmn     r2,r3;"             // shift down 16?
      "beq     0f;"
      "add     %2,%2,#16;"
      "mov     %0,%0,lsr #16;"
      "orr     %0,%0,r2,lsl #16;"
      "mov     %1,%1,lsr #16;"
      "orr     %1,%1,r3,lsl #16;"
      "0:"
      "cmp     r0,%3;\n"
      "bhi     1b;\n"

      // odd filter assymetry
      "ands    r0,%5,#1;\n"
      "beq     2f;\n"
      "add     r0,%3,%5,lsl#2;\n"

      "ldr     r1,[r0,#-4];\n"
      "mov     r0,#0x4000;\n"

      "subs    r1,r1,%4;\n"          //ilsp[j]-wi
      "rsbmi   r1,r1,#0;\n"          //labs(ilsp[j]-wi)
      "umull   %0,r2,r1,%0;\n"       //qi*=labs(ilsp[j]-wi)
      "umull   %1,r3,r0,%1;\n"       //pi*=labs(ilsp[j+1]-wi)

      "cmn     r2,r3;\n"             // shift down 16?
      "beq     2f;\n"
      "add     %2,%2,#16;\n"
      "mov     %0,%0,lsr #16;\n"
      "orr     %0,%0,r2,lsl #16;\n"
      "mov     %1,%1,lsr #16;\n"
      "orr     %1,%1,r3,lsl #16;\n"

      //qi=(pi>>shift)*labs(ilsp[j]-wi);
      //pi=(qi>>shift)*labs(ilsp[j+1]-wi);
      //qexp+=shift;

      //}

      /* normalize to max 16 sig figs */
      "2:"
      "mov     r2,#0;"
      "orr     r1,%0,%1;"
      "tst     r1,#0xff000000;"
      "addne   r2,r2,#8;"
      "movne   r1,r1,lsr #8;"
      "tst     r1,#0x00f00000;"
      "addne   r2,r2,#4;"
      "movne   r1,r1,lsr #4;"
      "tst     r1,#0x000c0000;"
      "addne   r2,r2,#2;"
      "movne   r1,r1,lsr #2;"
      "tst     r1,#0x00020000;"
      "addne   r2,r2,#1;"
      "movne   r1,r1,lsr #1;"
      "tst     r1,#0x00010000;"
      "addne   r2,r2,#1;"
      "mov     %0,%0,lsr r2;"
      "mov     %1,%1,lsr r2;"
      "add     %2,%2,r2;"

      : "+r"(qi),"+r"(pi),"+r"(qexp)
      : "r"(ilsp),"r"(wi),"r"(m)
      : "r0","r1","r2","r3","cc");

  *qip=qi;
  *pip=pi;
  *qexpp=qexp;
}

static inline void lsp_norm_asm(ogg_uint32_t *qip,ogg_int32_t *qexpp){

  ogg_uint32_t qi=*qip;
  ogg_int32_t qexp=*qexpp;

  asm("tst     %0,#0x0000ff00;"
      "moveq   %0,%0,lsl #8;"
      "subeq   %1,%1,#8;"
      "tst     %0,#0x0000f000;"
      "moveq   %0,%0,lsl #4;"
      "subeq   %1,%1,#4;"
      "tst     %0,#0x0000c000;"
      "moveq   %0,%0,lsl #2;"
      "subeq   %1,%1,#2;"
      "tst     %0,#0x00008000;"
      "moveq   %0,%0,lsl #1;"
      "subeq   %1,%1,#1;"
      : "+r"(qi),"+r"(qexp)
      :
      : "cc");
  *qip=qi;
  *qexpp=qexp;
}

#endif
#endif

