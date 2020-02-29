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

 function: modified discrete cosine transform prototypes

 ************************************************************************/

#ifndef _OGG_mdct_H_
#define _OGG_mdct_H_

#include "ivorbiscodec.h"
#include "misc.h"

#define DATA_TYPE ogg_int32_t
#define REG_TYPE  register ogg_int32_t

#ifdef _LOW_ACCURACY_
#define cPI3_8 (0x0062)
#define cPI2_8 (0x00b5)
#define cPI1_8 (0x00ed)
#else
#define cPI3_8 (0x30fbc54d)
#define cPI2_8 (0x5a82799a)
#define cPI1_8 (0x7641af3d)
#endif

extern void mdct_backward(int n, DATA_TYPE *in);
extern void mdct_shift_right(int n, DATA_TYPE *in, DATA_TYPE *right);
extern void mdct_unroll_lap(int n0,int n1,
			    int lW,int W,
			    DATA_TYPE *in,DATA_TYPE *right,
			    LOOKUP_T *w0,LOOKUP_T *w1,
			    ogg_int16_t *out,
			    int step,
			    int start,int end /* samples, this frame */);

#endif












