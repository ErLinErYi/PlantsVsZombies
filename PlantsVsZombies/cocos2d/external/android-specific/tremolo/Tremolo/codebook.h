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

 function: basic shared codebook operations

 ************************************************************************/

#ifndef _V_CODEBOOK_H_
#define _V_CODEBOOK_H_

#include "ogg.h"

typedef struct codebook{
  /* Top 15 used in ARM code */
  int          dec_maxlength;
  void        *dec_table;
  int          dec_method;
  int          dec_type; /* 0 = entry number
			    1 = packed vector of values
			    2 = packed vector of column offsets, maptype 1
			    3 = scalar offset into value array,  maptype 2 */
  int          q_bits;
  long         dim;      /* codebook dimensions (elements per vector) */
  int          q_delp;
  int          q_minp;
  ogg_int32_t  q_del;
  ogg_int32_t  q_min;
  int          q_seq;
  int          q_pack;
  void        *q_val;
  long         used_entries;    /* populated codebook entries */
  ogg_int32_t *dec_buf;

  /* C only */
  int   dec_nodeb;
  int   dec_leafw;

  long  entries;         /* codebook entries */

} codebook;

extern void vorbis_book_clear(codebook *b);
extern int  vorbis_book_unpack(oggpack_buffer *b,codebook *c);

extern long vorbis_book_decode(codebook *book, oggpack_buffer *b);
extern long vorbis_book_decodevs_add(codebook *book, ogg_int32_t *a,
				     oggpack_buffer *b,int n,int point);
extern long vorbis_book_decodev_set(codebook *book, ogg_int32_t *a,
				    oggpack_buffer *b,int n,int point);
extern long vorbis_book_decodev_add(codebook *book, ogg_int32_t *a,
				    oggpack_buffer *b,int n,int point);
extern long vorbis_book_decodevv_add(codebook *book, ogg_int32_t **a,
				     long off,int ch,
				    oggpack_buffer *b,int n,int point);

extern int _ilog(unsigned int v);


#endif
