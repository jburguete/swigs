/*
SWIGS (Shallow Water in Irregular Geometries Simulator): a software to simulate
transient or steady flows with solute transport in channels, channel networks
and rivers.

Copyright 2005-2014 Javier Burguete Tolosa.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

	1. Redistributions of source code must retain the above copyright notice,
		this list of conditions and the following disclaimer.

	2. Redistributions in binary form must reproduce the above copyright notice,
		this list of conditions and the following disclaimer in the
		documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY Javier Burguete Tolosa ``AS IS'' AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
SHALL Javier Burguete Tolosa OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef CHANNEL_SEGMENT__H
#define CHANNEL_SEGMENT__H 1

#include "section.h"

typedef struct
{
	int n,nf;
	JBFLOAT x1,x2,z1,z2;
	SectionParameters *sp1,*sp2;
	Section s1[1],s2[1];
} ChannelSegment;

static inline void _channel_segment_print(ChannelSegment *seg,FILE *file)
{
	register int i;
	fprintf(file,"Channel segment: start\n");
	section_print(seg->s1,file);
	section_print(seg->s2,file);
	for (i=0; i<=seg->n; ++i)
		fprintf(file,"CSP i=%d h="FWF" B1="FWF" B2="FWF"\n",
			i,seg->sp1[i].z,seg->sp1[i].B,seg->sp2[i].B);
	fprintf(file,"CSP x1="FWF" x2="FWF" z1="FWF" z2="FWF"\n",
		seg->x1,seg->x2,seg->z1,seg->z2);
	fprintf(file,"Channel segment: end\n");
}

#if INLINE_CHANNEL_SEGMENT_PRINT
	#define channel_segment_print _channel_segment_print
#else
	void channel_segment_print(ChannelSegment*,FILE*);
#endif

static inline void _channel_segment_delete(ChannelSegment *seg)
{
	#if DEBUG_CHANNEL_SEGMENT_DELETE
		fprintf(stderr,"Channel segment delete: start\n");
		fprintf(stderr,"CSD s1=%ld s2=%ld f1=%ld f2=%ld sp1=%ld\n",
			(size_t)seg->s1->sp,(size_t)seg->s2->sp,
			(size_t)seg->s1->fp,(size_t)seg->s2->fp,(size_t)seg->sp1);
	#endif
	section_delete(seg->s1);
	section_delete(seg->s2);
	jb_free_null((void**)&seg->sp1);
	#if DEBUG_CHANNEL_SEGMENT_DELETE
		fprintf(stderr,"Channel segment delete: end\n");
	#endif
}

#if INLINE_CHANNEL_SEGMENT_DELETE
	#define channel_segment_delete _channel_segment_delete
#else
	void channel_segment_delete(ChannelSegment*);
#endif

static inline int _channel_segment_open_section
	(ChannelSegment *seg,Section *s,JBDOUBLE x)
{
	int i;
	register JBDOUBLE k,k2,z;
	SectionParameters *sp,*sp1,*sp2;
	FrictionParameters *fp,*fp1,*fp2,*fp3;
	#if DEBUG_CHANNEL_SEGMENT_OPEN_SECTION
		fprintf(stderr,"Channel segment open section: start\n");
		fprintf(stderr,"CSOS n=%d nf=%d\n",seg->n,seg->nf);
//		channel_segment_print(seg,stderr);
	#endif
	if (!section_create(s,seg->n,seg->nf))
	{
		i=0;
		goto exit1;
	}
	k = (x - seg->x1) / (seg->x2 - seg->x1);
	#if DEBUG_CHANNEL_SEGMENT_OPEN_SECTION
		fprintf(stderr,"CSOS x="FWL" x1="FWF" x2="FWF"\n",x,seg->x1,seg->x2);
	#endif
	s->u = seg->s1->u + k * (seg->s2->u - seg->s1->u);
	z = seg->z1 + k * (seg->z2 - seg->z1);
	#if DEBUG_CHANNEL_SEGMENT_OPEN_SECTION
		fprintf(stderr,"CSOS z="FWL" z1="FWF" z2="FWF"\n",z,seg->z1,seg->z2);
	#endif
	for (i=0, sp=s->sp, sp1=seg->sp1, sp2=seg->sp2; i<=seg->n;
		++sp, ++i, ++sp1, ++sp2)
	{
		sp->z = z + sp1->z;
		sp->B = sp1->B + k * (sp2->B - sp1->B);
		sp->dBz = sp1->dBz + k * (sp2->dBz - sp1->dBz);
		sp->P = sp1->P + k * (sp2->P - sp1->P);
		sp->dPz = sp1->dPz + k * (sp2->dPz - sp1->dPz);
	}
	#if DEBUG_CHANNEL_SEGMENT_OPEN_SECTION
		fprintf(stderr,"CSOS i=%d\n",i);
	#endif
	s->zmin = seg->s1->zmin + k * (seg->s2->zmin - seg->s1->zmin); 
	s->hmin = seg->s1->hmin + k * (seg->s2->hmin - seg->s1->hmin);
	s->hmax = fmin(seg->s1->hmax, seg->s2->hmax); 
	s->zleft = s->zright = z + seg->sp1[seg->n].z;
	if (seg->s1->dz < seg->s2->dz)
	{
		s->dz = seg->s1->dz;
		fp1 = seg->s1->fp;
		fp2 = seg->s2->fp;
		k2 = seg->s2->dz / s->dz;
	}
	else
	{
		s->dz = seg->s2->dz;
		fp1 = seg->s2->fp;
		fp2 = seg->s1->fp;
		k=1.-k;
		k2 = seg->s1->dz / s->dz;
	}
	#if DEBUG_CHANNEL_SEGMENT_OPEN_SECTION
		fprintf(stderr,"CSOS nf=%d\n",seg->nf);
	#endif
	for (i=0, fp=s->fp; i<=seg->nf; ++fp, ++i, ++fp1)
	{
		fp3=fp2+(int)rint(i*k2);
		if (isfinite(fp1->r) && isfinite(fp3->r))
			fp->r = fp1->r + k * (fp3->r - fp1->r);
		else fp->r = INFINITY;
		fp->beta = fp1->beta + k * (fp3->beta - fp1->beta);
		fp->dbetaA = fp1->dbetaA + k * (fp3->dbetaA - fp1->dbetaA);
	}
	#if DEBUG_CHANNEL_SEGMENT_OPEN_SECTION
		fprintf(stderr,"CSOS opening parameters\n");
	#endif
	section_open_parameters(s);
	#if DEBUG_CHANNEL_SEGMENT_OPEN_SECTION
		//section_print(s,stderr);
	#endif
	goto exit2;
exit1:
	section_delete(s);
exit2:
	#if DEBUG_CHANNEL_SEGMENT_OPEN_SECTION
		fprintf(stderr,"Channel segment open section: end\n");
	#endif
	return i;
}

#if INLINE_CHANNEL_SEGMENT_OPEN_SECTION
	#define channel_segment_open_section _channel_segment_open_section
#else
	int channel_segment_open_section(ChannelSegment*,Section*,JBDOUBLE);
#endif

static inline int _channel_segment_open(ChannelSegment *seg,
	TransientSection *ts1,TransientSection *ts2,JBDOUBLE x1,JBDOUBLE x2)
{
	int i,j,k;
	register JBDOUBLE h1,h2,dz;
	SectionParameters *s1,*s2,*sp1,*sp2;

	#if DEBUG_CHANNEL_SEGMENT_OPEN
		fprintf(stderr,"Channel segment open: start\n");
		fprintf(stderr,"CSO opening section 1\n");
	#endif

	if (!section_open(seg->s1, ts1)) goto exit2;
	#if DEBUG_CHANNEL_SEGMENT_OPEN
		fprintf(stderr,"CSO opening section 2\n");
	#endif
	if (!section_open(seg->s2, ts2)) goto exit1;

	#if DEBUG_CHANNEL_SEGMENT_OPEN
		fprintf(stderr,"CSO checking section points\n");
	#endif
	for (i=j=k=1, sp1=seg->s1->sp, sp2=seg->s2->sp;
		i<=seg->s1->n && j<=seg->s2->n;)
	{
		++k;
		h1 = sp1[i].z - sp1[0].z;
		h2 = sp2[j].z - sp2[0].z;
		if (h1<h2) ++i;
		else if (h1==h2) ++i, ++j;
		else ++j;
	}

	seg->sp1 = (SectionParameters*)g_try_malloc(2*sizeof(SectionParameters)*k);
	if (!seg->sp1)
	{
		jbw_show_error2
			(gettext("Channel segment"),gettext("Not enough memory"));
		goto exit0;
	}
	seg->sp2 = seg->sp1 + k;

	seg->n = --k;
	sp1 = seg->s1->sp;
	sp2 = seg->s2->sp;
	s1 = seg->sp1;
	memcpy(s1,sp1,sizeof(SectionParameters));
	s2 = seg->sp2;
	memcpy(s2,sp2,sizeof(SectionParameters));
	s1->z = 0.;
	for (i=j=k=0; i<seg->s1->n && j<seg->s2->n;)
	{
		++k;
		h1 = sp1[i+1].z - sp1[0].z;
		h2 = sp2[j+1].z - sp2[0].z;
		if (h1==h2)
		{
			++i; ++j;
			memcpy(s1+k,sp1+i,sizeof(SectionParameters));
			memcpy(s2+k,sp2+j,sizeof(SectionParameters));
			s1[k].z = h1;
		}
		else if (h1<h2)
		{
			++i;
			memcpy(s1+k,sp1+i,sizeof(SectionParameters));
			s1[k].z = h1;
			h1 += sp2[0].z;
			dz = h1 - sp2[j].z;
			s2[k].B = sp2[j].B + dz * sp2[j].dBz;
			s2[k].dBz = sp2[j].dBz;
			s2[k].P = sp2[j].P + dz * sp2[j].dPz;
			s2[k].dPz = sp2[j].dPz;
		}
		else
		{
			++j;
			memcpy(s2+k,sp2+j,sizeof(SectionParameters));
			s1[k].z = h2;
			h2 += sp1[0].z;
			dz = h2 - sp1[i].z;
			s1[k].B = sp1[i].B + dz * sp1[i].dBz;
			s1[k].dBz = sp1[i].dBz;
			s1[k].P = sp1[i].P + dz * sp1[i].dPz;
			s1[k].dPz = sp1[i].dPz;
		}
	}

	seg->x1 = x1;
	seg->x2 = x2;
	seg->z1 = sp1[0].z;
	seg->z2 = sp2[0].z;

	seg->nf = jbm_min(seg->s1->nf, seg->s2->nf);

	#if DEBUG_CHANNEL_SEGMENT_OPEN
		channel_segment_print(seg,stderr);
		fprintf(stderr,"CS0 s1=%ld s2=%ld f1=%ld f2=%ld sp1=%ld\n",
			(size_t)seg->s1->sp,(size_t)seg->s2->sp,
			(size_t)seg->s1->fp,(size_t)seg->s2->fp,(size_t)seg->sp1);
		fprintf(stderr,"Channel segment open: end\n");
	#endif
	return k;

exit0:
	section_delete(seg->s2);
exit1:
	section_delete(seg->s1);
exit2:
	#if DEBUG_CHANNEL_SEGMENT_OPEN
		fprintf(stderr,"Channel segment open: end\n");
	#endif
	return 0;
}

#if INLINE_CHANNEL_SEGMENT_OPEN
	#define channel_segment_open _channel_segment_open
#else
	int channel_segment_open
		(ChannelSegment*,TransientSection*,TransientSection*,JBDOUBLE,JBDOUBLE);
#endif

#endif
