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

 function: subsumed libogg includes

 ************************************************************************/
#ifndef _OGG_H
#define _OGG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "os_types.h"

#ifndef ONLY_C
#define ARM_LITTLE_ENDIAN
#endif

typedef struct ogg_buffer_state{
  struct ogg_buffer    *unused_buffers;
  struct ogg_reference *unused_references;
  int                   outstanding;
  int                   shutdown;
} ogg_buffer_state;

typedef struct ogg_buffer {
  unsigned char      *data;
  long                size;
  int                 refcount;

  union {
    ogg_buffer_state  *owner;
    struct ogg_buffer *next;
  } ptr;
} ogg_buffer;

typedef struct ogg_reference {
  ogg_buffer    *buffer;
  long           begin;
  long           length;

  struct ogg_reference *next;
} ogg_reference;

typedef struct oggpack_buffer {
#ifdef ARM_LITTLE_ENDIAN
  int            bitsLeftInSegment;
  ogg_uint32_t  *ptr;
  long           bitsLeftInWord;
#else
  int            headbit;
  unsigned char *headptr;
  long           headend;
#endif /* ARM_LITTLE_ENDIAN */
  /* memory management */
  ogg_reference *head;
  ogg_reference *tail;

  /* render the byte/bit counter API constant time */
  long              count; /* doesn't count the tail */
} oggpack_buffer;

typedef struct oggbyte_buffer {
  ogg_reference *baseref;

  ogg_reference *ref;
  unsigned char *ptr;
  long           pos;
  long           end;
} oggbyte_buffer;

typedef struct ogg_sync_state {
  /* decode memory management pool */
  ogg_buffer_state *bufferpool;

  /* stream buffers */
  ogg_reference    *fifo_head;
  ogg_reference    *fifo_tail;
  long              fifo_fill;

  /* stream sync management */
  int               unsynced;
  int               headerbytes;
  int               bodybytes;

} ogg_sync_state;

typedef struct ogg_stream_state {
  ogg_reference *header_head;
  ogg_reference *header_tail;
  ogg_reference *body_head;
  ogg_reference *body_tail;

  int            e_o_s;    /* set when we have buffered the last
                              packet in the logical bitstream */
  int            b_o_s;    /* set after we've written the initial page
                              of a logical bitstream */
  long           serialno;
  long           pageno;
  ogg_int64_t    packetno; /* sequence number for decode; the framing
                              knows where there's a hole in the data,
                              but we need coupling so that the codec
                              (which is in a seperate abstraction
                              layer) also knows about the gap */
  ogg_int64_t    granulepos;

  int            lacing_fill;
  ogg_uint32_t   body_fill;

  /* decode-side state data */
  int            holeflag;
  int            spanflag;
  int            clearflag;
  int            laceptr;
  ogg_uint32_t   body_fill_next;

} ogg_stream_state;

typedef struct {
  ogg_reference *packet;
  long           bytes;
  long           b_o_s;
  long           e_o_s;
  ogg_int64_t    granulepos;
  ogg_int64_t    packetno;     /* sequence number for decode; the framing
                                  knows where there's a hole in the data,
                                  but we need coupling so that the codec
                                  (which is in a seperate abstraction
                                  layer) also knows about the gap */
} ogg_packet;

typedef struct {
  ogg_reference *header;
  int            header_len;
  ogg_reference *body;
  long           body_len;
} ogg_page;

/* Ogg BITSTREAM PRIMITIVES: bitstream ************************/

extern void  oggpack_readinit(oggpack_buffer *b,ogg_reference *r);
extern long  oggpack_look(oggpack_buffer *b,int bits);
extern void  oggpack_adv(oggpack_buffer *b,int bits);
extern long  oggpack_read(oggpack_buffer *b,int bits);
extern long  oggpack_bytes(oggpack_buffer *b);
extern long  oggpack_bits(oggpack_buffer *b);
extern int   oggpack_eop(oggpack_buffer *b);

// Quick hack
#define oggpack_bytesleft(B)  (((B)->bitsLeftInSegment+7)/8)

/* Ogg BITSTREAM PRIMITIVES: decoding **************************/

extern void     ogg_sync_init(ogg_sync_state *oy);
extern ogg_sync_state *ogg_sync_create(void);
extern int      ogg_sync_clear(ogg_sync_state *oy);
extern int      ogg_sync_destroy(ogg_sync_state *oy);
extern int      ogg_sync_reset(ogg_sync_state *oy);

extern unsigned char *ogg_sync_bufferin(ogg_sync_state *oy, long size);
extern int      ogg_sync_wrote(ogg_sync_state *oy, long bytes);
extern long     ogg_sync_pageseek(ogg_sync_state *oy,ogg_page *og);
extern int      ogg_sync_pageout(ogg_sync_state *oy, ogg_page *og);
extern int      ogg_stream_pagein(ogg_stream_state *os, ogg_page *og);
extern int      ogg_stream_packetout(ogg_stream_state *os,ogg_packet *op);
extern int      ogg_stream_packetpeek(ogg_stream_state *os,ogg_packet *op);

/* Ogg BITSTREAM PRIMITIVES: general ***************************/

extern void     ogg_stream_init(ogg_stream_state *os, int serialno);
extern ogg_stream_state *ogg_stream_create(int serialno);
extern int      ogg_stream_destroy(ogg_stream_state *os);
extern int      ogg_stream_clear(ogg_stream_state *os);
extern int      ogg_stream_reset(ogg_stream_state *os);
extern int      ogg_stream_reset_serialno(ogg_stream_state *os,int serialno);
extern int      ogg_stream_eos(ogg_stream_state *os);

extern int      ogg_page_checksum_set(ogg_page *og);

extern int      ogg_page_version(ogg_page *og);
extern int      ogg_page_continued(ogg_page *og);
extern int      ogg_page_bos(ogg_page *og);
extern int      ogg_page_eos(ogg_page *og);
extern ogg_int64_t  ogg_page_granulepos(ogg_page *og);
extern ogg_uint32_t ogg_page_serialno(ogg_page *og);
extern ogg_uint32_t ogg_page_pageno(ogg_page *og);
extern int      ogg_page_packets(ogg_page *og);
extern int      ogg_page_getbuffer(ogg_page *og, unsigned char **buffer);

extern int      ogg_packet_release(ogg_packet *op);
extern int      ogg_page_release(ogg_page *og);

extern void     ogg_page_dup(ogg_page *d, ogg_page *s);

/* Ogg BITSTREAM PRIMITIVES: return codes ***************************/

#define  OGG_SUCCESS   0

#define  OGG_HOLE     -10
#define  OGG_SPAN     -11
#define  OGG_EVERSION -12
#define  OGG_ESERIAL  -13
#define  OGG_EINVAL   -14
#define  OGG_EEOS     -15


#ifdef __cplusplus
}
#endif

#endif  /* _OGG_H */
