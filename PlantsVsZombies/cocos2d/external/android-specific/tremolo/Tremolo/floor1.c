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

 function: floor backend 1 implementation

 ************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "ogg.h"
#include "ivorbiscodec.h"
#include "codec_internal.h"
#include "codebook.h"
#include "misc.h"

extern const ogg_int32_t FLOOR_fromdB_LOOKUP[];
#define floor1_rangedB 140 /* floor 1 fixed at -140dB to 0dB range */
#define VIF_POSIT 63

/***********************************************/

void floor1_free_info(vorbis_info_floor *i){
  vorbis_info_floor1 *info=(vorbis_info_floor1 *)i;
  if(info){
    if(info->klass)_ogg_free(info->klass);
    if(info->partitionclass)_ogg_free(info->partitionclass);
    if(info->postlist)_ogg_free(info->postlist);
    if(info->forward_index)_ogg_free(info->forward_index);
    if(info->hineighbor)_ogg_free(info->hineighbor);
    if(info->loneighbor)_ogg_free(info->loneighbor);
    memset(info,0,sizeof(*info));
    _ogg_free(info);
  }
}

static int ilog(unsigned int v){
  int ret=0;
  while(v){
    ret++;
    v>>=1;
  }
  return(ret);
}

static void floor1_mergesort(ogg_uint8_t *index,ogg_uint16_t *vals,ogg_uint16_t n){
  ogg_uint16_t i,j;
  ogg_uint8_t *temp,*A=index,*B=_ogg_malloc(n*sizeof(*B));

  for(i=1;i<n;i<<=1){
    for(j=0;j+i<n;){
      int k1=j;
      int mid=j+i;
      int k2=mid;
      int end=(j+i*2<n?j+i*2:n);
      while(k1<mid && k2<end){
	if(vals[A[k1]]<vals[A[k2]])
	  B[j++]=A[k1++];
	else
	  B[j++]=A[k2++];
      }
      while(k1<mid) B[j++]=A[k1++];
      while(k2<end) B[j++]=A[k2++];
    }
    for(;j<n;j++)B[j]=A[j];
    temp=A;A=B;B=temp;
  }

  if(B==index){
    for(j=0;j<n;j++)B[j]=A[j];
    _ogg_free(A);
  }else
    _ogg_free(B);
}


vorbis_info_floor *floor1_info_unpack (vorbis_info *vi,oggpack_buffer *opb){
  codec_setup_info     *ci=(codec_setup_info *)vi->codec_setup;
  int j,k,count=0,maxclass=-1,rangebits;

  vorbis_info_floor1 *info=(vorbis_info_floor1 *)_ogg_calloc(1,sizeof(*info));
  /* read partitions */
  info->partitions=oggpack_read(opb,5); /* only 0 to 31 legal */
  info->partitionclass=
    (ogg_uint8_t *)_ogg_malloc(info->partitions*sizeof(*info->partitionclass));
  for(j=0;j<info->partitions;j++){
    info->partitionclass[j]=(char)oggpack_read(opb,4); /* only 0 to 15 legal */
    if(maxclass<info->partitionclass[j])maxclass=info->partitionclass[j];
  }

  /* read partition classes */
  info->klass=
    (floor1class *)_ogg_malloc((maxclass+1)*sizeof(*info->klass));
  for(j=0;j<maxclass+1;j++){
    info->klass[j].class_dim=(char)oggpack_read(opb,3)+1; /* 1 to 8 */
    info->klass[j].class_subs=(char)oggpack_read(opb,2); /* 0,1,2,3 bits */
    if(oggpack_eop(opb)<0) goto err_out;
    if(info->klass[j].class_subs)
      info->klass[j].class_book=(unsigned char)oggpack_read(opb,8);
    else
      info->klass[j].class_book=0;
    if(info->klass[j].class_book>=ci->books)goto err_out;
    for(k=0;k<(1<<info->klass[j].class_subs);k++){
      info->klass[j].class_subbook[k]=(unsigned char)(oggpack_read(opb,8)-1);
      if(info->klass[j].class_subbook[k]>=ci->books &&
	 info->klass[j].class_subbook[k]!=0xff)goto err_out;
    }
  }

  /* read the post list */
  info->mult=oggpack_read(opb,2)+1;     /* only 1,2,3,4 legal now */
  rangebits=oggpack_read(opb,4);

  for(j=0,k=0;j<info->partitions;j++)
    count+=info->klass[info->partitionclass[j]].class_dim;
  info->postlist=
    (ogg_uint16_t *)_ogg_malloc((count+2)*sizeof(*info->postlist));
  info->forward_index=
    (ogg_uint8_t *)_ogg_malloc((count+2)*sizeof(*info->forward_index));
  info->loneighbor=
    (ogg_uint8_t *)_ogg_malloc(count*sizeof(*info->loneighbor));
  info->hineighbor=
    (ogg_uint8_t *)_ogg_malloc(count*sizeof(*info->hineighbor));

  count=0;
  for(j=0,k=0;j<info->partitions;j++){
    count+=info->klass[info->partitionclass[j]].class_dim;
    for(;k<count;k++){
      int t=info->postlist[k+2]=(ogg_uint16_t)oggpack_read(opb,rangebits);
      if(t>=(1<<rangebits))goto err_out;
    }
  }
  if(oggpack_eop(opb))goto err_out;
  info->postlist[0]=0;
  info->postlist[1]=1<<rangebits;
  info->posts=count+2;

  /* also store a sorted position index */
  for(j=0;j<info->posts;j++)info->forward_index[j]=j;
  floor1_mergesort(info->forward_index,info->postlist,info->posts);

  /* discover our neighbors for decode where we don't use fit flags
     (that would push the neighbors outward) */
  for(j=0;j<info->posts-2;j++){
    int lo=0;
    int hi=1;
    int lx=0;
    int hx=info->postlist[1];
    int currentx=info->postlist[j+2];
    for(k=0;k<j+2;k++){
      int x=info->postlist[k];
      if(x>lx && x<currentx){
	lo=k;
	lx=x;
      }
      if(x<hx && x>currentx){
	hi=k;
	hx=x;
      }
    }
    info->loneighbor[j]=lo;
    info->hineighbor[j]=hi;
  }

  return(info);

 err_out:
  floor1_free_info(info);
  return(NULL);
}

#ifdef ONLY_C
static
#endif
int render_point(int x0,int x1,int y0,int y1,int x){
  y0&=0x7fff; /* mask off flag */
  y1&=0x7fff;

  {
    int dy=y1-y0;
    int adx=x1-x0;
    int ady=abs(dy);
    int err=ady*(x-x0);

    int off=err/adx;
    if(dy<0)return(y0-off);
    return(y0+off);
  }
}

#ifndef ONLY_C
void render_lineARM(int n, ogg_int32_t *d,const ogg_int32_t *floor, int base, int err, int adx, int ady);
#endif

static void render_line(int n,int x0,int x1,int y0,int y1,ogg_int32_t *d){
  int dy;
  int adx;
  int ady;
  int base;
  int err;
  const ogg_int32_t *floor;

  if(n>x1)n=x1;
  n -= x0;
  if (n <= 0 || y0 < 0 || y0 > 255 || y1 < 0 || y1 > 255) {
    return;
  }
  dy=y1-y0;
  adx=x1-x0;
  ady=abs(dy);
  base=dy/adx;
  err=adx-1;
  floor=&FLOOR_fromdB_LOOKUP[y0];
  d += x0;
  ady-=abs(base*adx);

  /* We should add base each time, and then:
   *   if dy >=0 we occasionally add 1
   *   else         occasionally subtract 1.
   * As an optimisation we say that if dy <0 we make base 1 smaller.
   * Then we need to add 1 occassionally, rather than subtract 1 - but we
   * need to add 1 in all the cases when we wouldn't have done so before.
   * Previously we'd have added 1 (100*ady/adx)% of the time. Now we want
   * to do so (100*(adx-ady)/adx)% of the time.
   */
  if (dy < 0){
    base--;
    ady = adx-ady;
    err = 0;
  }

  //if(x<n)
  //  d[x]= MULT31_SHIFT15(d[x],FLOOR_fromdB_LOOKUP[y]);

#if defined(ONLY_C)
  do{
    *d = MULT31_SHIFT15(*d,*floor);
    d++;
    floor+=base;
    err-=ady;
    if(err<0){
      err+=adx;
      floor+=1;
    }
    n--;
  } while(n>0);
#else
  render_lineARM(n,d,floor,base,err,adx,ady);
#endif
}

int floor1_memosize(vorbis_info_floor *i){
  vorbis_info_floor1 *info=(vorbis_info_floor1 *)i;
  return info->posts;
}

static int quant_look[4]={256,128,86,64};

ogg_int32_t *floor1_inverse1(vorbis_dsp_state *vd,vorbis_info_floor *in,
			     ogg_int32_t *fit_value){
  vorbis_info_floor1 *info=(vorbis_info_floor1 *)in;
  codec_setup_info   *ci=(codec_setup_info *)vd->vi->codec_setup;

  int i,j,k;
  codebook *books=ci->book_param;
  int quant_q=quant_look[info->mult-1];

  /* unpack wrapped/predicted values from stream */
  if(oggpack_read(&vd->opb,1)==1){
    fit_value[0]=oggpack_read(&vd->opb,ilog(quant_q-1));
    fit_value[1]=oggpack_read(&vd->opb,ilog(quant_q-1));

    /* partition by partition */
    /* partition by partition */
    for(i=0,j=2;i<info->partitions;i++){
      int classv=info->partitionclass[i];
      int cdim=info->klass[classv].class_dim;
      int csubbits=info->klass[classv].class_subs;
      int csub=1<<csubbits;
      int cval=0;

      /* decode the partition's first stage cascade value */
      if(csubbits){
	cval=vorbis_book_decode(books+info->klass[classv].class_book,&vd->opb);

	if(cval==-1)goto eop;
      }

      for(k=0;k<cdim;k++){
	int book=info->klass[classv].class_subbook[cval&(csub-1)];
	cval>>=csubbits;
	if(book!=0xff){
	  if((fit_value[j+k]=vorbis_book_decode(books+book,&vd->opb))==-1)
	    goto eop;
	}else{
	  fit_value[j+k]=0;
	}
      }
      j+=cdim;
    }

    /* unwrap positive values and reconsitute via linear interpolation */
    for(i=2;i<info->posts;i++){
      int predicted=render_point(info->postlist[info->loneighbor[i-2]],
				 info->postlist[info->hineighbor[i-2]],
				 fit_value[info->loneighbor[i-2]],
				 fit_value[info->hineighbor[i-2]],
				 info->postlist[i]);
      int hiroom=quant_q-predicted;
      int loroom=predicted;
      int room=(hiroom<loroom?hiroom:loroom)<<1;
      int val=fit_value[i];

      if(val){
	if(val>=room){
	  if(hiroom>loroom){
	    val = val-loroom;
	  }else{
	  val = -1-(val-hiroom);
	  }
	}else{
	  if(val&1){
	    val= -((val+1)>>1);
	  }else{
	    val>>=1;
	  }
	}

	fit_value[i]=val+predicted;
	fit_value[info->loneighbor[i-2]]&=0x7fff;
	fit_value[info->hineighbor[i-2]]&=0x7fff;

      }else{
	fit_value[i]=predicted|0x8000;
      }

    }

    return(fit_value);
  }
 eop:
  return(NULL);
}

int floor1_inverse2(vorbis_dsp_state *vd,vorbis_info_floor *in,
		    ogg_int32_t *fit_value,ogg_int32_t *out){
  vorbis_info_floor1 *info=(vorbis_info_floor1 *)in;

  codec_setup_info   *ci=(codec_setup_info *)vd->vi->codec_setup;
  int                  n=ci->blocksizes[vd->W]/2;
  int j;

  if(fit_value){
    /* render the lines */
    int hx=0;
    int lx=0;
    int ly=fit_value[0]*info->mult;
    for(j=1;j<info->posts;j++){
      int current=info->forward_index[j];
      int hy=fit_value[current]&0x7fff;
      if(hy==fit_value[current]){

	hy*=info->mult;
	hx=info->postlist[current];

	render_line(n,lx,hx,ly,hy,out);

	lx=hx;
	ly=hy;
      }
    }
    for(j=hx;j<n;j++)out[j]*=ly; /* be certain */
    return(1);
  }
  memset(out,0,sizeof(*out)*n);
  return(0);
}
