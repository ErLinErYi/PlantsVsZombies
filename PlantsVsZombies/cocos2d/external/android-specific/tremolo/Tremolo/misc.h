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

 function: miscellaneous math and prototypes

 ************************************************************************/

#ifndef _V_RANDOM_H_
#define _V_RANDOM_H_
#include "ivorbiscodec.h"
#include "os_types.h"

/*#define _VDBG_GRAPHFILE "_0.m"*/


#ifdef _VDBG_GRAPHFILE
extern void *_VDBG_malloc(void *ptr,long bytes,char *file,long line);
extern void _VDBG_free(void *ptr);

#undef _ogg_malloc
#undef _ogg_calloc
#undef _ogg_realloc
#undef _ogg_free

#define _ogg_malloc(x) _VDBG_malloc(NULL,(x),__FILE__,__LINE__)
#define _ogg_calloc(x,y) _VDBG_malloc(NULL,(x)*(y),__FILE__,__LINE__)
#define _ogg_realloc(x,y) _VDBG_malloc((x),(y),__FILE__,__LINE__)
#define _ogg_free(x) _VDBG_free((x))
#endif

#include "asm_arm.h"

#ifndef _V_WIDE_MATH
#define _V_WIDE_MATH

#ifndef  _LOW_ACCURACY_
/* 64 bit multiply */

#include <endian.h>
#include <sys/types.h>

#if BYTE_ORDER==LITTLE_ENDIAN
union magic {
  struct {
    ogg_int32_t lo;
    ogg_int32_t hi;
  } halves;
  ogg_int64_t whole;
};
#endif

#if BYTE_ORDER==BIG_ENDIAN
union magic {
  struct {
    ogg_int32_t hi;
    ogg_int32_t lo;
  } halves;
  ogg_int64_t whole;
};
#endif

static inline ogg_int32_t MULT32(ogg_int32_t x, ogg_int32_t y) {
  union magic magic;
  magic.whole = (ogg_int64_t)x * y;
  return magic.halves.hi;
}

static inline ogg_int32_t MULT31(ogg_int32_t x, ogg_int32_t y) {
  return MULT32(x,y)<<1;
}

static inline ogg_int32_t MULT31_SHIFT15(ogg_int32_t x, ogg_int32_t y) {
  union magic magic;
  magic.whole  = (ogg_int64_t)x * y;
  return ((ogg_uint32_t)(magic.halves.lo)>>15) | ((magic.halves.hi)<<17);
}

#else
/* 32 bit multiply, more portable but less accurate */

/*
 * Note: Precision is biased towards the first argument therefore ordering
 * is important.  Shift values were chosen for the best sound quality after
 * many listening tests.
 */

/*
 * For MULT32 and MULT31: The second argument is always a lookup table
 * value already preshifted from 31 to 8 bits.  We therefore take the
 * opportunity to save on text space and use unsigned char for those
 * tables in this case.
 */

static inline ogg_int32_t MULT32(ogg_int32_t x, ogg_int32_t y) {
  return (x >> 9) * y;  /* y preshifted >>23 */
}

static inline ogg_int32_t MULT31(ogg_int32_t x, ogg_int32_t y) {
  return (x >> 8) * y;  /* y preshifted >>23 */
}

static inline ogg_int32_t MULT31_SHIFT15(ogg_int32_t x, ogg_int32_t y) {
  return (x >> 6) * y;  /* y preshifted >>9 */
}

#endif

/*
 * This should be used as a memory barrier, forcing all cached values in
 * registers to wr writen back to memory.  Might or might not be beneficial
 * depending on the architecture and compiler.
 */
#define MB()

/*
 * The XPROD functions are meant to optimize the cross products found all
 * over the place in mdct.c by forcing memory operation ordering to avoid
 * unnecessary register reloads as soon as memory is being written to.
 * However this is only beneficial on CPUs with a sane number of general
 * purpose registers which exclude the Intel x86.  On Intel, better let the
 * compiler actually reload registers directly from original memory by using
 * macros.
 */

#ifdef __i386__

#define XPROD32(_a, _b, _t, _v, _x, _y)		\
  { *(_x)=MULT32(_a,_t)+MULT32(_b,_v);		\
    *(_y)=MULT32(_b,_t)-MULT32(_a,_v); }
#define XPROD31(_a, _b, _t, _v, _x, _y)		\
  { *(_x)=MULT31(_a,_t)+MULT31(_b,_v);		\
    *(_y)=MULT31(_b,_t)-MULT31(_a,_v); }
#define XNPROD31(_a, _b, _t, _v, _x, _y)	\
  { *(_x)=MULT31(_a,_t)-MULT31(_b,_v);		\
    *(_y)=MULT31(_b,_t)+MULT31(_a,_v); }

#else

static inline void XPROD32(ogg_int32_t  a, ogg_int32_t  b,
			   ogg_int32_t  t, ogg_int32_t  v,
			   ogg_int32_t *x, ogg_int32_t *y)
{
  *x = MULT32(a, t) + MULT32(b, v);
  *y = MULT32(b, t) - MULT32(a, v);
}

static inline void XPROD31(ogg_int32_t  a, ogg_int32_t  b,
			   ogg_int32_t  t, ogg_int32_t  v,
			   ogg_int32_t *x, ogg_int32_t *y)
{
  *x = MULT31(a, t) + MULT31(b, v);
  *y = MULT31(b, t) - MULT31(a, v);
}

static inline void XNPROD31(ogg_int32_t  a, ogg_int32_t  b,
			    ogg_int32_t  t, ogg_int32_t  v,
			    ogg_int32_t *x, ogg_int32_t *y)
{
  *x = MULT31(a, t) - MULT31(b, v);
  *y = MULT31(b, t) + MULT31(a, v);
}

#endif

#endif

#ifndef _V_CLIP_MATH
#define _V_CLIP_MATH

static inline ogg_int32_t CLIP_TO_15(ogg_int32_t x) {
  int ret=x;
  ret-= ((x<=32767)-1)&(x-32767);
  ret-= ((x>=-32768)-1)&(x+32768);
  return(ret);
}

#endif

#endif




