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
 ************************************************************************/

#define HEAD_ALIGN 64
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#define MISC_C
#include "misc.h"
//#include <sys/time.h>

static void **pointers=NULL;
static long *insertlist=NULL; /* We can't embed this in the pointer list;
			  a pointer can have any value... */

static char **files=NULL;
static long *file_bytes=NULL;
static int  filecount=0;

static int ptop=0;
static int palloced=0;
static int pinsert=0;

typedef struct {
  char *file;
  long line;
  long ptr;
  long bytes;
} head;

long global_bytes=0;
long start_time=-1;

static void *_insert(void *ptr,long bytes,char *file,long line){
  ((head *)ptr)->file=file;
  ((head *)ptr)->line=line;
  ((head *)ptr)->ptr=pinsert;
  ((head *)ptr)->bytes=bytes-HEAD_ALIGN;

  if(pinsert>=palloced){
    palloced+=64;
    if(pointers){
      pointers=(void **)realloc(pointers,sizeof(void **)*palloced);
      insertlist=(long *)realloc(insertlist,sizeof(long *)*palloced);
    }else{
      pointers=(void **)malloc(sizeof(void **)*palloced);
      insertlist=(long *)malloc(sizeof(long *)*palloced);
    }
  }

  pointers[pinsert]=ptr;

  if(pinsert==ptop)
    pinsert=++ptop;
  else
    pinsert=insertlist[pinsert];

#ifdef _VDBG_GRAPHFILE
  {
    FILE *out;
    struct timeval tv;
    static struct timezone tz;
    int i;
    char buffer[80];
    gettimeofday(&tv,&tz);

    for(i=0;i<filecount;i++)
      if(!strcmp(file,files[i]))break;

    if(i==filecount){
      filecount++;
      if(!files){
	files=malloc(filecount*sizeof(*files));
	file_bytes=malloc(filecount*sizeof(*file_bytes));
      }else{
	files=realloc(files,filecount*sizeof(*files));
	file_bytes=realloc(file_bytes,filecount*sizeof(*file_bytes));
      }
      files[i]=strdup(file);
      file_bytes[i]=0;
    }

    file_bytes[i]+=bytes-HEAD_ALIGN;

    if(start_time==-1)start_time=(tv.tv_sec*1000)+(tv.tv_usec/1000);

    snprintf(buffer,80,"%s",file);
    if(strchr(buffer,'.'))strchr(buffer,'.')[0]=0;
    strcat(buffer,_VDBG_GRAPHFILE);
    out=fopen(buffer,"a");
    fprintf(out,"%ld, %ld\n",-start_time+(tv.tv_sec*1000)+(tv.tv_usec/1000),
	    file_bytes[i]-(bytes-HEAD_ALIGN));
    fprintf(out,"%ld, %ld # FILE %s LINE %ld\n",
	    -start_time+(tv.tv_sec*1000)+(tv.tv_usec/1000),
	    file_bytes[i],file,line);
    fclose(out);

    out=fopen("total"_VDBG_GRAPHFILE,"a");
    fprintf(out,"%ld, %ld\n",-start_time+(tv.tv_sec*1000)+(tv.tv_usec/1000),
	    global_bytes);
    fprintf(out,"%ld, %ld\n",-start_time+(tv.tv_sec*1000)+(tv.tv_usec/1000),
	    global_bytes+(bytes-HEAD_ALIGN));
    fclose(out);
  }
#endif

  global_bytes+=(bytes-HEAD_ALIGN);

  return(void *)(((char *)ptr)+HEAD_ALIGN);
}

static void _ripremove(void *ptr){
  int insert;

#ifdef _VDBG_GRAPHFILE
  {
    FILE *out=fopen("total"_VDBG_GRAPHFILE,"a");
    struct timeval tv;
    static struct timezone tz;
    char buffer[80];
    char *file =((head *)ptr)->file;
    long bytes =((head *)ptr)->bytes;
    int i;

    gettimeofday(&tv,&tz);
    fprintf(out,"%ld, %ld\n",-start_time+(tv.tv_sec*1000)+(tv.tv_usec/1000),
	    global_bytes);
    fprintf(out,"%ld, %ld\n",-start_time+(tv.tv_sec*1000)+(tv.tv_usec/1000),
	    global_bytes-((head *)ptr)->bytes);
    fclose(out);

    for(i=0;i<filecount;i++)
      if(!strcmp(file,files[i]))break;

    snprintf(buffer,80,"%s",file);
    if(strchr(buffer,'.'))strchr(buffer,'.')[0]=0;
    strcat(buffer,_VDBG_GRAPHFILE);
    out=fopen(buffer,"a");
    fprintf(out,"%ld, %ld\n",-start_time+(tv.tv_sec*1000)+(tv.tv_usec/1000),
	    file_bytes[i]);
    fprintf(out,"%ld, %ld\n",-start_time+(tv.tv_sec*1000)+(tv.tv_usec/1000),
	    file_bytes[i]-bytes);
    fclose(out);

    file_bytes[i]-=bytes;

  }
#endif

  global_bytes-=((head *)ptr)->bytes;

  insert=((head *)ptr)->ptr;
  insertlist[insert]=pinsert;
  pinsert=insert;

  if(pointers[insert]==NULL){
    fprintf(stderr,"DEBUGGING MALLOC ERROR: freeing previously freed memory\n");
    fprintf(stderr,"\t%s %ld\n",((head *)ptr)->file,((head *)ptr)->line);
  }

  if(global_bytes<0){
    fprintf(stderr,"DEBUGGING MALLOC ERROR: freeing unmalloced memory\n");
  }

  pointers[insert]=NULL;
}

void _VDBG_dump(void){
  int i;
  for(i=0;i<ptop;i++){
    head *ptr=pointers[i];
    if(ptr)
      fprintf(stderr,"unfreed bytes from %s:%ld\n",
	      ptr->file,ptr->line);
  }

}

extern void *_VDBG_malloc(void *ptr,long bytes,char *file,long line){
  bytes+=HEAD_ALIGN;
  if(ptr){
    ptr=(void *)(((char *)ptr)-HEAD_ALIGN);
    _ripremove(ptr);
    ptr=realloc(ptr,bytes);
  }else{
    ptr=malloc(bytes);
    memset(ptr,0,bytes);
  }
  return _insert(ptr,bytes,file,line);
}

extern void _VDBG_free(void *ptr){
  if(ptr){
    ptr=(void *)(((char *)ptr)-HEAD_ALIGN);
    _ripremove(ptr);
    free(ptr);
  }
}

