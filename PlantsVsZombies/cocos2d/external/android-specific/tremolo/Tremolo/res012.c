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

 function: residue backend 0, 1 and 2 implementation

 ************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "ogg.h"
#include "ivorbiscodec.h"
#include "codec_internal.h"
#include "codebook.h"
#include "misc.h"
#include "os.h"

void res_clear_info(vorbis_info_residue *info){
  if(info){
    if(info->stagemasks)_ogg_free(info->stagemasks);
    if(info->stagebooks)_ogg_free(info->stagebooks);
    memset(info,0,sizeof(*info));
  }
}


/* vorbis_info is for range checking */
int res_unpack(vorbis_info_residue *info,
                vorbis_info *vi,oggpack_buffer *opb){
  int j,k;
  codec_setup_info     *ci=(codec_setup_info *)vi->codec_setup;
  memset(info,0,sizeof(*info));

  info->type=oggpack_read(opb,16);
  if(info->type>2 || info->type<0)goto errout;
  info->begin=oggpack_read(opb,24);
  info->end=oggpack_read(opb,24);
  info->grouping=oggpack_read(opb,24)+1;
  info->partitions=(char)(oggpack_read(opb,6)+1);
  info->groupbook=(unsigned char)oggpack_read(opb,8);
  if(info->groupbook>=ci->books)goto errout;

  info->stagemasks=_ogg_malloc(info->partitions*sizeof(*info->stagemasks));
  info->stagebooks=_ogg_malloc(info->partitions*8*sizeof(*info->stagebooks));

  for(j=0;j<info->partitions;j++){
    int cascade=oggpack_read(opb,3);
    if(oggpack_read(opb,1))
      cascade|=(oggpack_read(opb,5)<<3);
    info->stagemasks[j]=cascade;
  }

  for(j=0;j<info->partitions;j++){
    for(k=0;k<8;k++){
      if((info->stagemasks[j]>>k)&1){
        unsigned char book=(unsigned char)oggpack_read(opb,8);
        if(book>=ci->books)goto errout;
        info->stagebooks[j*8+k]=book;
        if(k+1>info->stages)info->stages=k+1;
      }else
        info->stagebooks[j*8+k]=0xff;
    }
  }

  if(oggpack_eop(opb))goto errout;

  return 0;
 errout:
  res_clear_info(info);
  return 1;
}

int res_inverse(vorbis_dsp_state *vd,vorbis_info_residue *info,
                ogg_int32_t **in,int *nonzero,int ch){

  int i,j,k,s,used=0;
  codec_setup_info     *ci=(codec_setup_info *)vd->vi->codec_setup;
  codebook *phrasebook=ci->book_param+info->groupbook;
  int samples_per_partition=info->grouping;
  int partitions_per_word=phrasebook->dim;
  int pcmend=ci->blocksizes[vd->W];

  if(info->type<2){
    int max=pcmend>>1;
    int end=(info->end<max?info->end:max);
    int n=end-info->begin;

    if(n>0){
      int partvals=n/samples_per_partition;
      int partwords=(partvals+partitions_per_word-1)/partitions_per_word;

      for(i=0;i<ch;i++)
        if(nonzero[i])
          in[used++]=in[i];
      ch=used;

      if(used){

        char **partword=(char **)alloca(ch*sizeof(*partword));
        for(j=0;j<ch;j++)
          partword[j]=(char *)alloca(partwords*partitions_per_word*
                                     sizeof(*partword[j]));

        for(s=0;s<info->stages;s++){

          for(i=0;i<partvals;){
            if(s==0){
              /* fetch the partition word for each channel */

              partword[0][i+partitions_per_word-1]=1;
              for(k=partitions_per_word-2;k>=0;k--)
                partword[0][i+k]=partword[0][i+k+1]*info->partitions;

              for(j=1;j<ch;j++)
                for(k=partitions_per_word-1;k>=0;k--)
                  partword[j][i+k]=partword[j-1][i+k];

              for(j=0;j<ch;j++){
                int temp=vorbis_book_decode(phrasebook,&vd->opb);
                if(temp==-1)goto eopbreak;

                /* this can be done quickly in assembly due to the quotient
                   always being at most six bits */
                for(k=0;k<partitions_per_word;k++){
                  ogg_uint32_t div=partword[j][i+k];
                  partword[j][i+k]=temp/div;
                  temp-=partword[j][i+k]*div;
                }

              }
            }

            /* now we decode residual values for the partitions */
            for(k=0;k<partitions_per_word && i<partvals;k++,i++)
              for(j=0;j<ch;j++){
                long offset=info->begin+i*samples_per_partition;
                int idx = (int)partword[j][i];
                if(idx < info->partitions && info->stagemasks[idx]&(1<<s)){
                  codebook *stagebook=ci->book_param+
                    info->stagebooks[(partword[j][i]<<3)+s];
                  if(info->type){
                    if(vorbis_book_decodev_add(stagebook,in[j]+offset,&vd->opb,
                                               samples_per_partition,-8)==-1)
                      goto eopbreak;
                  }else{
                    if(vorbis_book_decodevs_add(stagebook,in[j]+offset,&vd->opb,
                                                samples_per_partition,-8)==-1)
                      goto eopbreak;
                  }
                }
              }
          }
        }
      }
    }
  }else{
    int max=(pcmend*ch)>>1;
    int end=(info->end<max?info->end:max);
    int n=end-info->begin;

    if(n>0){
      int partvals=n/samples_per_partition;
      int partwords=(partvals+partitions_per_word-1)/partitions_per_word;

      char *partword=
        (char *)alloca(partwords*partitions_per_word*sizeof(*partword));
      int beginoff=info->begin/ch;

      for(i=0;i<ch;i++)if(nonzero[i])break;
      if(i==ch)return(0); /* no nonzero vectors */

      samples_per_partition/=ch;

      for(s=0;s<info->stages;s++){
        for(i=0;i<partvals;){

          if(s==0){
            int temp;
            partword[i+partitions_per_word-1]=1;
            for(k=partitions_per_word-2;k>=0;k--)
              partword[i+k]=partword[i+k+1]*info->partitions;

            /* fetch the partition word */
            temp=vorbis_book_decode(phrasebook,&vd->opb);
            if(temp==-1)goto eopbreak;

            /* this can be done quickly in assembly due to the quotient
               always being at most six bits */
            for(k=0;k<partitions_per_word;k++){
              ogg_uint32_t div=partword[i+k];
              partword[i+k]=temp/div;
              temp-=partword[i+k]*div;
            }
          }

          /* now we decode residual values for the partitions */
          for(k=0;k<partitions_per_word && i<partvals;k++,i++){
              if(partword[i] >= 0 && partword[i] < info->partitions &&
                      (info->stagemasks[(int)partword[i]] & (1 << s))){
                  codebook *stagebook=ci->book_param+
                          info->stagebooks[(partword[i]<<3)+s];
                  if(vorbis_book_decodevv_add(stagebook,in,
                              i*samples_per_partition+beginoff,ch,
                              &vd->opb,
                              samples_per_partition,-8)==-1)
                      goto eopbreak;
              }
          }
        }
      }
    }
  }
 eopbreak:

  return 0;
}

