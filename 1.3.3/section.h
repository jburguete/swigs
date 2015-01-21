/*
SWIGS (Shallow Water in Irregular Geometries Simulator): a software to simulate
transient or steady flows with solute transport in channels, channel networks
and rivers.

Copyright 2005-2015 Javier Burguete Tolosa.

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

#ifndef SECTION__H
#define SECTION__H 1

#include "system.h"

#define friction_b1 (friction_b + 1.)
#define friction_b1b1 (friction_b1 * friction_b1)
#define friction_2b (friction_b + friction_b)
#define friction_2b1 (friction_2b + 1.)
#define friction_2b2 (friction_2b + 2.)
#define friction_3b2 (3. * friction_b + 2.)
#define friction_4b3 (4. * friction_b + 3.)
#define friction_2b1i (1. / friction_2b1)
#define friction_beta (friction_b1b1 / friction_2b1)

#if MODEL_FRICTION == MODEL_FRICTION_MINIMIZATION_STRESS
	#define friction_gi1e (friction_2b2)
	#define friction_gi21e (friction_2b + 3.)
	#define friction_gi22e (friction_b + 2.)
	#define friction_gi21c (1. / friction_gi21e)
	#define friction_gi22c (-2. / friction_gi22e)
	#define friction_xi1e (friction_4b3)
	#define friction_xi21e (4. * friction_b + 4.)
	#define friction_xi22e (3. * friction_b + 3.)
	#define friction_xi23e (friction_2b + 3.)
	#define friction_xi24e (friction_2b2)
	#define friction_xi25e (friction_2b1)
	#define friction_xi21c (1. / friction_xi21e)
	#define friction_xi22c (-2. / friction_xi22e)
	#define friction_xi23c (-1. / friction_xi23e)
	#define friction_xi24c (1. / friction_xi24e)
	#define friction_xi25c (2. / friction_xi25e)
#else
	#define friction_gi1e (friction_b + 1.5)
	#define friction_gi21e (friction_b + 2.5)
	#define friction_gi22e (1.5)
	#define friction_gi21c (1. / friction_gi21e)
	#define friction_gi22c (-2./3.)
	#define friction_xi1e (friction_2b2)
	#define friction_xi21e (friction_2b + 3.)
	#define friction_xi22e (2.)
	#define friction_xi21c (1. / friction_xi21e)
	#define friction_xi22c (-0.5)
#endif

typedef struct
{
	JBFLOAT z,B,P,dBz,dPz,A,I,c;
} SectionParameters;

typedef struct
{
	JBFLOAT r,beta,dbetaA;
} FrictionParameters;

typedef struct
{
	int t;
	JBFLOAT dzy,r2dpy,l,l1,l2;
	#if MODEL_FRICTION == MODEL_FRICTION_MINIMIZATION_STRESS
		JBFLOAT l3,l4,l5;
	#endif
} FrictionCoefficients;

typedef struct
{
	int n,nf,e;
	JBFLOAT dz,zmin,zmax,zleft,zright,hmin,hmax,Amax,u;
	SectionParameters *sp;
	FrictionParameters *fp;
} Section;

static inline JBDOUBLE _wave_velocity(JBDOUBLE g,JBDOUBLE A,JBDOUBLE B)
{
	return sqrt(g*A/B);
}

#if INLINE_WAVE_VELOCITY
	#define wave_velocity _wave_velocity
#else
	JBDOUBLE wave_velocity(JBDOUBLE,JBDOUBLE,JBDOUBLE);
#endif

static inline void _section_parameters_print(SectionParameters *sp,FILE *file)
{
	fprintf(file,"Section parameters: start\n");
	fprintf(file,"SP z="FWF" A="FWF" B="FWF" dBz="FWF"\n""P="FWF" dPz="FWF"\n",
		sp->z,sp->A,sp->B,sp->dBz,sp->P,sp->dPz);
	fprintf(file,"Section parameters: end\n");
}

#if INLINE_SECTION_PARAMETERS_PRINT
	#define section_parameters_print _section_parameters_print
#else
	void section_parameters_print(SectionParameters*,FILE*);
#endif

static inline void _friction_parameters_print(FrictionParameters *fp,FILE *file)
{
	fprintf(file,"Friction parameters: start\n");
	fprintf(file,"FP r="FWF" beta="FWF"\n",fp->r,fp->beta);
	fprintf(file,"Friction parameters: end\n");
}

#if INLINE_FRICTION_PARAMETERS_PRINT
	#define friction_parameters_print _friction_parameters_print
#else
	void friction_parameters_print(FrictionParameters*,FILE*);
#endif

static inline void _section_print(Section *s,FILE *file)
{
	int i;
	fprintf(file,"Section: start\n");
	fprintf(file,"S n=%d nf=%d e=%d\n",s->n,s->nf,s->e);
	fprintf(file,"S dz="FWF" zmin="FWF" zmax="FWF"\nzleft="FWF" zright="FWF"\n"
		"hmin="FWF" hmax="FWF"\nAmax="FWF" u="FWF"\n",
		s->dz,s->zmin,s->zmax,s->zleft,s->zright,s->hmin,s->hmax,s->Amax,s->u);
	for (i=0; i<=s->n; ++i)
	{
		fprintf(file,"S i=%d\n",i);
		section_parameters_print(s->sp + i, file);
	}
	if (s->e) for (i=0; i<=s->n; ++i)
		fprintf(file,"S i=%d c="FWF"\n",i,s->sp[i].c);
	for (i=0; i<=s->nf; ++i)
	{
		fprintf(file,"S i=%d\n",i);
		friction_parameters_print(s->fp + i, file);
	}
	fprintf(file,"Section: end\n");
}

#if INLINE_SECTION_PRINT
	#define section_print _section_print
#else
	void section_print(Section*,FILE*);
#endif
extern JBDOUBLE t;

static inline JBDOUBLE
	_section_trapezoidal_level(SectionParameters *sp,JBDOUBLE A)
{
	if (sp->dBz==0.) return sp->z + (A - sp->A) / sp->B;
	return sp->z + (sqrt(jbm_fsqr(sp->B) + jbm_fdbl(sp->dBz) * (A - sp->A))
		- sp->B) / sp->dBz;
}

#if INLINE_SECTION_TRAPEZOIDAL_LEVEL
	#define section_trapezoidal_level _section_trapezoidal_level
#else
	JBDOUBLE section_trapezoidal_level(SectionParameters*,JBDOUBLE);
#endif

static inline JBDOUBLE _section_level(Section *s,JBDOUBLE A)
{
	SectionParameters *sp = s->sp + s->n;
	if (A<=0.) return s->sp->z;
	else if (A < sp->A)
		return section_trapezoidal_level(s->sp + jbm_varray_search
			(A, &s->sp->A, sizeof(SectionParameters), s->n), A);
	return sp->z + (A - sp->A) / sp->B;
}

#if INLINE_SECTION_LEVEL
	#define section_level _section_level
#else
	JBDOUBLE section_level(Section*,JBDOUBLE);
#endif

static inline JBDOUBLE _section_beta(Section *s,JBDOUBLE z)
{
	unsigned int i;
	i = jbm_min(s->nf, (int)ceill((z - s->sp->z) / s->dz));
	return s->fp[i].beta;
}

#if INLINE_SECTION_BETA
	#define section_beta _section_beta
#else
	JBDOUBLE section_beta(Section*,JBDOUBLE);
#endif

static inline JBDOUBLE _section_width(Section *s,JBDOUBLE z)
{
	register SectionParameters *sp;
	sp = s->sp + s->n;
	if (z < sp->z)
	{
		sp = s->sp +
			jbm_varray_search(z, &s->sp->z, sizeof(SectionParameters), s->n);
		return sp->B + sp->dBz * (z - sp->z);
	}
	return sp->B;
}

#if INLINE_SECTION_WIDTH
	#define section_width _section_width
#else
	JBDOUBLE section_width(Section*,JBDOUBLE);
#endif

static inline JBDOUBLE _section_width_with_area(Section *s,JBDOUBLE A)
{
	#if DEBUG_SECTION_WIDTH_WITH_AREA
    		fprintf(stderr,"A="FWL" z="FWL"\n",A,section_level(s,A));
    	#endif
	return section_width(s,section_level(s,A));
}

#if INLINE_SECTION_WIDTH_WITH_AREA
	#define section_width_with_area _section_width_with_area
#else
	JBDOUBLE section_width_with_area(Section*,JBDOUBLE);
#endif

static inline JBDOUBLE _section_perimeter(Section *s,JBDOUBLE z)
{
	SectionParameters *sp;
	sp = s->sp + s->n;
	if (z < sp->z)
	{
		sp = s->sp +
			jbm_varray_search(z, &s->sp->z, sizeof(SectionParameters), s->n);
		return sp->P + sp->dPz * (z - sp->z);
	}
	return sp->P;
}

#if INLINE_SECTION_PERIMETER
	#define section_perimeter _section_perimeter
#else
	JBDOUBLE section_perimeter(Section*,JBDOUBLE);
#endif

static inline JBDOUBLE _section_perimeter_with_area(Section *s,JBDOUBLE A)
{
	return section_perimeter(s, section_level(s, A));
}

#if INLINE_SECTION_PERIMETER_WITH_AREA
	#define section_perimeter_with_area _section_perimeter_with_area
#else
	JBDOUBLE section_perimeter_with_area(Section*,JBDOUBLE);
#endif

static inline JBDOUBLE _section_perimeter_with_depth(Section *s,JBDOUBLE h)
{
	return section_perimeter(s, h + s->sp->z);
}

#if INLINE_SECTION_PERIMETER_WITH_DEPTH
	#define section_perimeter_with_depth _section_perimeter_with_depth
#else
	JBDOUBLE section_perimeter_with_depth(Section*,JBDOUBLE);
#endif

static inline JBDOUBLE _section_area(Section *s,JBDOUBLE z)
{
	register JBDOUBLE dz;
	register SectionParameters *sp;
	#if DEBUG_SECTION_AREA
		fprintf(stderr,"SA z="FWL" zb="FWF"\n",z,s->sp->z);
	#endif
	if (z <= s->sp->z) return 0.;
	sp = s->sp + s->n;
	if (z < sp->z)
	{
		sp = s->sp +
			jbm_varray_search(z, &s->sp->z, sizeof(SectionParameters), s->n);
		dz = z - sp->z;
		#if DEBUG_SECTION_AREA
			fprintf(stderr,"SA dz="FWL" zb="FWF"\n",dz,sp->z);
		#endif
		return sp->A + (sp->B + 0.5 * dz * sp->dBz) * dz;
	}
	#if DEBUG_SECTION_AREA
		fprintf(stderr,"SA z="FWL" zb="FWF"\n",z,sp->z);
	#endif
	return sp->A + (z - sp->z) * sp->B;
}

#if INLINE_SECTION_AREA
	#define section_area _section_area
#else
	JBDOUBLE section_area(Section*,JBDOUBLE);
#endif

static inline JBDOUBLE _section_area_with_depth(Section *s,JBDOUBLE h)
{
	#if DEBUG_SECTION_AREA_WITH_DEPTH
		fprintf(stderr,"SAWD h="FWL" zb="FWL"\n",h,s->sp->z);
	#endif
	return section_area(s, h + s->sp->z);
}

#if INLINE_SECTION_AREA_WITH_DEPTH
	#define section_area_with_depth _section_area_with_depth
#else
	JBDOUBLE section_area_with_depth(Section*,JBDOUBLE);
#endif

static inline JBDOUBLE _section_wave_velocity(Section *s,JBDOUBLE g,JBDOUBLE A)
{
	#if DEBUG_SECTION_WAVE_VELOCITY
    	fprintf(stderr,"SWV g="FWL" A="FWL" B="FWL"\n",
			g,A,section_width_with_area(s,A));
    #endif
	return wave_velocity(g,A,section_width_with_area(s,A));
}

#if INLINE_SECTION_WAVE_VELOCITY
	#define section_wave_velocity _section_wave_velocity
#else
	JBDOUBLE section_wave_velocity(Section*,JBDOUBLE,JBDOUBLE);
#endif

static inline JBDOUBLE _section_critical_discharge(Section *s,JBDOUBLE A)
{
	register JBDOUBLE z,B;
	z=section_level(s,A);
	B=section_width(s,z);
	return A*sqrt(JBM_G*A/(B*section_beta(s,z)));
}

#if INLINE_SECTION_CRITICAL_DISCHARGE
	#define section_critical_discharge _section_critical_discharge
#else
	JBDOUBLE section_critical_discharge(Section*,JBDOUBLE);
#endif

static inline JBDOUBLE _section_critical_function(Section *s,double A)
{
	register JBDOUBLE z;
	z=section_level(s,A);
	return A*A*A/(section_width(s,z)*section_beta(s,z));
}

#if INLINE_SECTION_CRITICAL_FUNCTION
	#define section_critical_function _section_critical_function
#else
	JBDOUBLE section_critical_function(Section*,JBDOUBLE);
#endif

static inline JBDOUBLE _section_critical_area(Section *s,JBDOUBLE Q)
{
	register JBDOUBLE A1,A2,A3,f;
	if (Q<=0.) return 0.;
	Q*=Q/JBM_G;
	A2 = s->Amax;
	f=section_critical_function(s,A2);
	while (f<Q)
	{
		A2 += A2;
		f *= 8.;
	}
	A1 = s->sp[1].A;
	f=section_critical_function(s,A1);
	while (f>Q)
	{
		A1 *= 0.5;
		f=section_critical_function(s,A1);
	}
	A3=A1;
	while (jbm_fcompare(f,Q))
	{
		A3=0.5*(A1+A2);
		if (A3==A1 || A3==A2) break;
		f=section_critical_function(s,A3);
		if (f>Q) A2=A3; else A1=A3;
	}
	return A3;
}

#if INLINE_SECTION_CRITICAL_AREA
	#define section_critical_area _section_critical_area
#else
	JBDOUBLE section_critical_area(Section*,JBDOUBLE);
#endif

static inline void _section_delete(Section *s)
{
	#if DEBUG_SECTION_DELETE
		fprintf(stderr,"Section delete: start\n");
		fprintf(stderr,"SD deleting section points\n");
	#endif
	jb_free_null((void**)&s->sp);
	#if DEBUG_SECTION_DELETE
		fprintf(stderr,"SD deleting friction points\n");
	#endif
	jb_free_null((void**)&s->fp);
	#if DEBUG_SECTION_DELETE
		fprintf(stderr,"Section delete: end\n");
	#endif
}

#if INLINE_SECTION_DELETE
	#define section_delete _section_delete
#else
	void section_delete(Section*);
#endif

static inline int _section_create(Section *s,int n,int nf)
{
	#if DEBUG_SECTION_CREATE
		fprintf(stderr,"Section create: start\n");
	#endif
	s->n = n;
	s->sp = (SectionParameters*)
		g_try_malloc((n + 1) * sizeof(SectionParameters));
	s->nf = nf;
	s->fp = (FrictionParameters*)
		g_try_malloc((nf + 1) * sizeof(FrictionParameters));
	if (!s->sp || !s->fp)
	{
		section_delete(s);
		jbw_show_error2(gettext("Section"),gettext("Not enough memory"));
		#if DEBUG_SECTION_CREATE
			fprintf(stderr,"Section create: end\n");
		#endif
		return 0;
	}
	#if DEBUG_SECTION_CREATE
		fprintf(stderr,"Section create: end\n");
	#endif
	return 1;
}

#if INLINE_SECTION_CREATE
	#define section_create _section_create
#else
	int section_create(Section*,int,int);
#endif

static inline int _section_copy(Section *s,Section *s_copy)
{
	#if DEBUG_SECTION_COPY
		fprintf(stderr,"Section copy: start\n");
		section_print(s_copy,stderr);
	#endif
	if (!section_create(s, s_copy->n, s_copy->nf)) goto exit2;
	memcpy(s, s_copy, (size_t)&s->sp - (size_t)s);
	memcpy(s->sp, s_copy->sp, (s->n + 1) * sizeof(SectionParameters));
	memcpy(s->fp, s_copy->fp, (s->nf + 1) * sizeof(FrictionParameters));
	#if DEBUG_SECTION_COPY
		section_print(s,stderr);
		fprintf(stderr,"Section copy: end\n");
	#endif
	return 1;
exit2:
	jbw_show_error2(gettext("Section"),gettext("Not enough memory"));
	#if DEBUG_SECTION_COPY
		fprintf(stderr,"Section copy: end\n");
	#endif
	return 0;
}

#if INLINE_SECTION_COPY
	#define section_copy _section_copy
#else
	int section_copy(Section*,Section*);
#endif

static inline JBDOUBLE _section_Bm(TransientSection *ts,JBDOUBLE z,int imax,int *nk)
{
	register int j,k;
	register JBDOUBLE B,y1,y2;

	#if DEBUG_SECTION_BM
		fprintf(stderr,"Section Bm: start\n");
	#endif
	
	B = 0.;
	for (j=0; j<imax;)
	{
		k = nk[j++];
		y1 = jbm_extrapolate
			(z,ts->sp[k].z,ts->sp[k+1].z,ts->sp[k].y,ts->sp[k+1].y);
		k = nk[j++];
		y2 = jbm_extrapolate
			(z,ts->sp[k].z,ts->sp[k+1].z,ts->sp[k].y,ts->sp[k+1].y);
		B += fabs(y2 - y1);
	}
	#if DEBUG_SECTION_BM
		fprintf(stderr,"B="FWL"\n",B);
		fprintf(stderr,"Section Bm: end\n");
	#endif
	return B;
}

#if INLINE_SECTION_BM
	#define section_Bm _section_Bm
#else
	JBDOUBLE section_Bm(TransientSection*,JBDOUBLE,int,int*);
#endif

static inline JBDOUBLE _section_Bl(TransientSection *ts,JBDOUBLE z,int imax,int *nk)
{
	register int j;
	register JBDOUBLE B;
	#if DEBUG_SECTION_BL
		fprintf(stderr,"Section Bl: start\n");
	#endif
	B=section_Bm(ts,z,imax-1,nk);
	j=nk[imax-1];
	B += fabs(ts->sp[ts->n].y -
		jbm_extrapolate(z,ts->sp[j].z,ts->sp[j+1].z,ts->sp[j].y,ts->sp[j+1].y));
	#if DEBUG_SECTION_BL
		fprintf(stderr,"B="FWL"\n",B);
		fprintf(stderr,"Section Bl: end\n");
	#endif
	return B;
}

#if INLINE_SECTION_BL
	#define section_Bl _section_Bl
#else
	JBDOUBLE section_Bl(TransientSection*,JBDOUBLE,int,int*);
#endif

static inline JBDOUBLE _section_Br
	(TransientSection *ts,JBDOUBLE z,int imax,int *nk)
{
	register int j;
	register JBDOUBLE B;
	#if DEBUG_SECTION_BR
		fprintf(stderr,"Section Br: start\n");
		fprintf(stderr,"z="FWL"\n",z);
	#endif
	B=section_Bm(ts,z,imax-1,nk+1);
	j=nk[0];
	#if DEBUG_SECTION_BR
		fprintf(stderr,"j=%d\n",j);
	#endif
	B += fabs(jbm_extrapolate(
		z,ts->sp[j].z,ts->sp[j+1].z,ts->sp[j].y,ts->sp[j+1].y) - ts->sp[0].y);
	#if DEBUG_SECTION_BR
		fprintf(stderr,"B="FWL"\n",B);
		fprintf(stderr,"Section Br: end\n");
	#endif
	return B;
}

#if INLINE_SECTION_BR
	#define section_Br _section_Br
#else
	JBDOUBLE section_Br(TransientSection*,JBDOUBLE,int,int*);
#endif

static inline JBDOUBLE _section_Bc
	(TransientSection *ts,JBDOUBLE z,int imax,int *nk)
{
	register int j;
	register JBDOUBLE B;
	#if DEBUG_SECTION_BC
		fprintf(stderr,"Section Bc: start\n");
	#endif
	B=section_Bm(ts,z,imax-2,nk+1);
	j=nk[imax-1];
	B += fabs(ts->sp[ts->n].y -
		jbm_extrapolate(z,ts->sp[j].z,ts->sp[j+1].z,ts->sp[j].y,ts->sp[j+1].y));
	j=nk[0];
	B += fabs(jbm_extrapolate(
		z,ts->sp[j].z,ts->sp[j+1].z,ts->sp[j].y,ts->sp[j+1].y) - ts->sp[0].y);
	#if DEBUG_SECTION_BC
		fprintf(stderr,"B="FWL"\n",B);
		fprintf(stderr,"Section Bc: end\n");
	#endif
	return B;
}

#if INLINE_SECTION_BC
	#define section_Bc _section_Bc
#else
	JBDOUBLE section_Bc(TransientSection*,JBDOUBLE,int,int*);
#endif

static inline void _section_B(TransientSection *ts,SectionParameters *sp,
	int nj,int *nk,JBDOUBLE (*B)(TransientSection*,JBDOUBLE,int,int*))
{
	#if DEBUG_SECTION_B
		fprintf(stderr,"Section B: start\n");
	#endif
	sp->B = B(ts, sp->z, nj, nk);
	sp->dBz = (B(ts, (sp+1)->z, nj, nk) - sp->B) / ((sp+1)->z - sp->z);
	#if DEBUG_SECTION_B
		fprintf(stderr,"Section B: end\n");
	#endif
}

#if INLINE_SECTION_B
	#define section_B _section_B
#else
	void section_B(TransientSection*,SectionParameters*,int,int*,
		JBDOUBLE (*)(TransientSection*,JBDOUBLE,int,int*));
#endif

static inline void _section_P
	(TransientSection *ts,SectionParameters *sp,int n,int *ni)
{
	register int i,j,k;
	register JBDOUBLE dp;
	register SectionParameters *s;
	#if DEBUG_SECTION_P
		fprintf(stderr,"Section P: start\n");
	#endif
	for (i=0; i<ts->n; ++i, ++ni)
	{
		if (ni[0]>ni[1]) j=ni[1], k=ni[0]; else j=ni[0], k=ni[1];
		s = sp + j;
		if (j==k)
		{
			s->P += fabs(ts->sp[i+1].y - ts->sp[i].y);
			continue;
		}
		dp = sqrt(1. + jbm_fsqr(
			(ts->sp[i+1].y - ts->sp[i].y) / (ts->sp[i+1].z - ts->sp[i].z)));
		#if DEBUG_SECTION_P
			fprintf(stderr,"SP i=%d dp="FWL"\n",i,dp);
		#endif
		for (; ++j<=k;)
		{
			s->dPz += dp;
			++s;
			s->P += (s->z - (s-1)->z) * dp;
			#if DEBUG_SECTION_P
				fprintf(stderr,"SP j=%d P="FWF" z="FWF"\n",j,s->P,s->z);
			#endif
		}
	}
	#if DEBUG_SECTION_P
		for (i=0; i<=n; ++i) fprintf(stderr,"SP i=%d P="FWF"\n",i,sp[i].P);
	#endif
	for (i=0, s=sp; ++i<=n; ++s) (s+1)->P += s->P;
	#if DEBUG_SECTION_P
		for (i=0; i<=n; ++i) fprintf(stderr,"SP i=%d P="FWF" dPz="FWF"\n",
			i,sp[i].P,sp[i].dPz);
		fprintf(stderr,"Section P: end\n");
	#endif
}

#if INLINE_SECTION_P
	#define section_P _section_P
#else
	void section_P(TransientSection*,SectionParameters*,int,int*);
#endif

static inline void _section_rhpa
	(FrictionParameters *fp,JBDOUBLE h1,JBDOUBLE h2,JBDOUBLE r1,JBDOUBLE r2)
{
	#if DEBUG_SECTION_RHPA
		fprintf(stderr,"Section rhpa: start\n");
		fprintf(stderr,"SRHPA h1="FWL" h2="FWL"\n",h1,h2);
		fprintf(stderr,"SRHPA r="FWF" beta="FWF"\n",fp->r,fp->beta);
	#endif
	fp->r+=friction_gi21c*(pow(h1,friction_gi21e)-pow(h2,friction_gi21e))/r1;
	fp->beta+=friction_xi21c*(pow(h1,friction_xi21e)-pow(h2,friction_xi21e))/r2;
	#if DEBUG_SECTION_RHPA
		fprintf(stderr,"SRHPA r="FWF" beta="FWF"\n",fp->r,fp->beta);
		fprintf(stderr,"Section rhpa: end\n");
	#endif
}

#if INLINE_SECTION_RHPA
	#define section_rhpa _section_rhpa
#else
	void section_rhpa(FrictionParameters*,JBDOUBLE,JBDOUBLE,JBDOUBLE,JBDOUBLE);
#endif

static inline void _section_rhpb
	(FrictionParameters *fp,JBDOUBLE h,JBDOUBLE r1,JBDOUBLE r2)
{
	#if DEBUG_SECTION_RHPB
		fprintf(stderr,"Section rhpb: start\n");
		fprintf(stderr,"SRHPB h="FWL"\n",h);
		fprintf(stderr,"SRHPB r="FWF" beta="FWF"\n",fp->r,fp->beta);
	#endif
	fp->r+=pow(h,friction_gi1e)/r1;
	fp->beta+=pow(h,friction_xi1e)/r2;
	#if DEBUG_SECTION_RHPB
		fprintf(stderr,"SRHPB r="FWF" beta="FWF"\n",fp->r,fp->beta);
		fprintf(stderr,"Section rhpb: end\n");
	#endif
}

#if INLINE_SECTION_RHPB
	#define section_rhpb _section_rhpb
#else
	void section_rhpb(FrictionParameters*,JBDOUBLE,JBDOUBLE,JBDOUBLE);
#endif

static inline void _section_rhpc
	(FrictionParameters *fp,JBDOUBLE h,JBDOUBLE r1,JBDOUBLE r2)
{
	#if DEBUG_SECTION_RHPC
		fprintf(stderr,"Section rhpc: start\n");
		fprintf(stderr,"SRHPC h="FWL"\n",h);
		fprintf(stderr,"SRHPC r="FWF" beta="FWF"\n",fp->r,fp->beta);
	#endif
	fp->r+=friction_gi21c*pow(h,friction_gi21e)/r1;
	fp->beta+=friction_xi21c*pow(h,friction_xi21e)/r2;
	#if DEBUG_SECTION_RHPC
		fprintf(stderr,"SRHPC r="FWF" beta="FWF"\n",fp->r,fp->beta);
		fprintf(stderr,"Section rhpc: end\n");
	#endif
}

#if INLINE_SECTION_RHPC
	#define section_rhpc _section_rhpc
#else
	void section_rhpc(FrictionParameters*,JBDOUBLE,JBDOUBLE,JBDOUBLE);
#endif

static inline void _section_rhla(FrictionParameters *fp,JBDOUBLE h1,JBDOUBLE h2,
	JBDOUBLE r1,JBDOUBLE r2,FrictionCoefficients *fc)
{
	#if DEBUG_SECTION_RHLA
		fprintf(stderr,"Section rhla: start\n");
		fprintf(stderr,"SRHLA h1="FWL" h2="FWL"\n",h1,h2);
		fprintf(stderr,"SRHLA r="FWF" beta="FWF"\n",fp->r,fp->beta);
	#endif
#if MODEL_FRICTION == MODEL_FRICTION_MINIMIZATION_STRESS	
	fp->r+=(friction_gi21c*(pow(h1,friction_gi21e)-pow(h2,friction_gi21e))
		+fc->l1*friction_gi22c*(pow(h1,friction_gi22e)-pow(h2,friction_gi22e))
		+fc->l2*(h1-h2))/r1;
	fp->beta+=(friction_xi21c*(pow(h1,friction_xi21e)-pow(h2,friction_xi21e))
		+fc->l1*friction_xi22c*(pow(h1,friction_xi22e)-pow(h2,friction_xi22e))
		+fc->l2*friction_xi23c*(pow(h1,friction_xi23e)-pow(h2,friction_xi23e))
		+fc->l3*friction_xi24c*(pow(h1,friction_xi24e)-pow(h2,friction_xi24e))
		+fc->l4*friction_xi25c*(pow(h1,friction_xi25e)-pow(h2,friction_xi25e))
		-fc->l5*(h1-h2))/r2;
#else
	fp->r+=(friction_gi21c*(pow(h1,friction_gi21e)-pow(h2,friction_gi21e))
		+fc->l1*friction_gi22c*(pow(h1,friction_gi22e)-pow(h2,friction_gi22e)))
		/r1;
	fp->beta+=(friction_xi21c*(pow(h1,friction_xi21e)-pow(h2,friction_xi21e))
		+fc->l2*friction_xi22c*(pow(h1,friction_xi22e)-pow(h2,friction_xi22e)))
		/r2;
#endif
	#if DEBUG_SECTION_RHLA
		fprintf(stderr,"SRHLA r="FWF" beta="FWF"\n",fp->r,fp->beta);
		fprintf(stderr,"Section rhla: end\n");
	#endif
}

#if INLINE_SECTION_RHLA
	#define section_rhla _section_rhla
#else
	void section_rhla(FrictionParameters*,JBDOUBLE,JBDOUBLE,JBDOUBLE,JBDOUBLE,
		FrictionCoefficients*);
#endif

static inline void _section_rhlb(FrictionParameters *fp,JBDOUBLE h,JBDOUBLE r1,
	JBDOUBLE r2,FrictionCoefficients *fc)
{
	JBDOUBLE R;
	#if DEBUG_SECTION_RHLB
		fprintf(stderr,"Section rhlb: start\n");
		fprintf(stderr,"SRHLB h="FWL"\n",h);
		fprintf(stderr,"SRHLB r="FWF" beta="FWF"\n",fp->r,fp->beta);
	#endif
	R = fc->l / h;
#if MODEL_FRICTION == MODEL_FRICTION_MINIMIZATION_STRESS	
	JBDOUBLE R1;
	R1=jbm_fsqr(1.-pow(R,friction_b1));
	fp->r+=pow(h,friction_gi1e)*R1/r1;
	fp->beta+=pow(h,friction_xi1e)*(1.-pow(R,friction_2b1))*R1/r2;
#else
	fp->r+=pow(h,friction_gi1e)*(1.-pow(R,friction_b1))/r1;
	fp->beta+=pow(h,friction_xi1e)*(1.-pow(R,friction_2b1))/r2;
#endif
	#if DEBUG_SECTION_RHLB
		fprintf(stderr,"SRHLB r="FWF" beta="FWF"\n",fp->r,fp->beta);
		fprintf(stderr,"Section rhlb: end\n");
	#endif
}

#if INLINE_SECTION_RHLB
	#define section_rhlb _section_rhlb
#else
	void section_rhlb
		(FrictionParameters*,JBDOUBLE,JBDOUBLE,JBDOUBLE,FrictionCoefficients*);
#endif

static inline void _section_rhlc(FrictionParameters *fp,JBDOUBLE h,JBDOUBLE r1,
	JBDOUBLE r2,FrictionCoefficients *fc)
{
	#if DEBUG_SECTION_RHLC
		fprintf(stderr,"Section rhlc: start\n");
		fprintf(stderr,"SRHLC h="FWL"\n",h);
		fprintf(stderr,"SRHLC r="FWF" beta="FWF"\n",fp->r,fp->beta);
	#endif
#if MODEL_FRICTION == MODEL_FRICTION_MINIMIZATION_STRESS	
	fp->r+=(friction_gi21c*pow(h,friction_gi21e)+
		fc->l1*friction_gi22c*pow(h,friction_gi22e)+fc->l2*h)/r1;
	fp->beta+=(friction_xi21c*pow(h,friction_xi21e)
		+fc->l1*friction_xi22c*pow(h,friction_xi22e)
		+fc->l2*friction_xi23c*pow(h,friction_xi23e)
		+fc->l3*friction_xi24c*pow(h,friction_xi24e)
		+fc->l4*friction_xi25c*pow(h,friction_xi25e)
		-fc->l5*h)/r2;
#else
	fp->r+=(friction_gi21c*pow(h,friction_gi21e)+
		fc->l1*friction_gi22c*pow(h,friction_gi22e))/r1;
	fp->beta+=(friction_xi21c*pow(h,friction_xi21e)
		+fc->l2*friction_xi22c*pow(h,friction_xi22e))/r2;
#endif
	#if DEBUG_SECTION_RHLC
		fprintf(stderr,"SRHLC r="FWF" beta="FWF"\n",fp->r,fp->beta);
		fprintf(stderr,"Section rhlc: end\n");
	#endif
}

#if INLINE_SECTION_RHLC
	#define section_rhlc _section_rhlc
#else
	void section_rhlc
		(FrictionParameters*,JBDOUBLE,JBDOUBLE,JBDOUBLE,FrictionCoefficients*);
#endif

static inline void _section_rha(FrictionParameters *fp,JBDOUBLE h1,JBDOUBLE h2,
	JBDOUBLE r1,JBDOUBLE r2,FrictionCoefficients *fc)
{
	#if DEBUG_SECTION_RHA
		fprintf(stderr,"Section rha: start\n");
		fprintf(stderr,"SRHA r1="FWL" r2="FWL"\n",r1,r2);
		fprintf(stderr,"SRHA h1="FWL" h2="FWL"\n",h1,h2);
	#endif
	if (fc->t)
	{
		if (h2 > fc->l) section_rhla(fp,h1,h2,r1,r2,fc);
		else if (h1 > fc->l) section_rhlc(fp,h1,r1,r2,fc);
	}
	else section_rhpa(fp,h1,h2,r1,r2);
	#if DEBUG_SECTION_RHA
		fprintf(stderr,"SRHA r="FWF" beta="FWF"\n",fp->r,fp->beta);
		fprintf(stderr,"Section rha: end\n");
	#endif
}

#if INLINE_SECTION_RHA
	#define section_rha _section_rha
#else
	void section_rha(FrictionParameters*,JBDOUBLE,JBDOUBLE,JBDOUBLE,JBDOUBLE,
		FrictionCoefficients*);
#endif

static inline void _section_rhb(FrictionParameters *fp,JBDOUBLE h,JBDOUBLE r1,
	JBDOUBLE r2,FrictionCoefficients *fc)
{
	#if DEBUG_SECTION_RHB
		fprintf(stderr,"Section rhb: start\n");
		fprintf(stderr,"SRHB h="FWL" r1="FWL" r2="FWL"\n",h,r1,r2);
	#endif
	if (fc->t)
	{
		if (h > fc->l) section_rhlb(fp,h,r1,r2,fc);
	}
	else section_rhpb(fp,h,r1,r2);
	#if DEBUG_SECTION_RHB
		fprintf(stderr,"SRHB r="FWF" beta="FWF"\n",fp->r,fp->beta);
		fprintf(stderr,"Section rhb: end\n");
	#endif
}

#if INLINE_SECTION_RHB
	#define section_rhb _section_rhb
#else
	void section_rhb
		(FrictionParameters*,JBDOUBLE,JBDOUBLE,JBDOUBLE,FrictionCoefficients*);
#endif

static inline void _section_rhc(FrictionParameters *fp,JBDOUBLE h,JBDOUBLE r1,
	JBDOUBLE r2,FrictionCoefficients *fc)
{
	#if DEBUG_SECTION_RHC
		fprintf(stderr,"Section rhc: start\n");
		fprintf(stderr,"SRHC h="FWL" r1="FWL" r2="FWL"\n",h,r1,r2);
	#endif
	if (fc->t)
	{
		if (h > fc->l) section_rhlc(fp,h,r1,r2,fc);
	}
	else section_rhpc(fp,h,r1,r2);
	#if DEBUG_SECTION_RHC
		fprintf(stderr,"SRHC r="FWF" beta="FWF"\n",fp->r,fp->beta);
		fprintf(stderr,"Section rhc: end\n");
	#endif
}

#if INLINE_SECTION_RHC
	#define section_rhc _section_rhc
#else
	void section_rhc
		(FrictionParameters*,JBDOUBLE,JBDOUBLE,JBDOUBLE,FrictionCoefficients*);
#endif

static inline void _section_ri(Section *s,TransientSection *ts,
	SectionParameters *sp,FrictionParameters *fp,JBDOUBLE y1,JBDOUBLE y2,
	int nj,int *nk,int n,FrictionCoefficients *fci)
{
	register int i,j;
	int ni1,ni2,nj1,nj2;
	JBDOUBLE dy,yi1,yi2,zi1,zi2,zj1,zj2,k2,dh,dh1,dh2,r1,r2,r3,r4,z;
	JBFLOAT z1[nj],z2[nj];
	FrictionCoefficients fcj[nj];
	FrictionParameters fpj[1];

	#if DEBUG_SECTION_RI
		fprintf(stderr,"Section ri: start\n");
	#endif
	
	dy=y2-y1;
	if (dy<=0.) return;
	#if DEBUG_SECTION_RI
		fprintf(stderr,"SRI y1="FWL" y2="FWL"\n",y1,y2);
		fprintf(stderr,"SRI dy="FWL" zmin="FWL"\n",dy,ts->zmin);
	#endif
	--nj;
	for (j=0; j<=nj; ++j)
	{
		i=nk[j];
		memcpy(fcj+j,fci+i,sizeof(FrictionCoefficients));
		#if DEBUG_SECTION_RI
			fprintf(stderr,"SRI j=%d i=%d e="FWF" r="FWF"\n",
				j,i,fcj[j].dzy,fcj[j].r2dpy);
		#endif
		yi1 = ts->sp[i].y;
		zi1 = ts->sp[i].z;
		++i;
		yi2 = ts->sp[i].y;
		zi2 = ts->sp[i].z;
		z1[j]=jbm_extrapolate(y1,yi1,yi2,zi1,zi2);
		z2[j]=jbm_extrapolate(y2,yi1,yi2,zi1,zi2);
	}
	for (j=0; ++j<=nj; ++j)
		fcj[j].r2dpy = pow(pow(fcj[j].r2dpy, friction_2b1i) +
			pow(fcj[j-1].r2dpy, friction_2b1i), friction_2b1);
	#if (MODEL_FRICTION == MODEL_FRICTION_MINIMIZATION_LOSS) || \
		(MODEL_FRICTION == MODEL_FRICTION_CONSTANT_SLOPE)
		for (j=0; j<=nj; ++j) fcj[j].r2dpy = sqrt(fcj[j].r2dpy);
	#endif
	for (j=0; j<nj; ++j)
	{
		#if DEBUG_SECTION_RI
			fprintf(stderr,"SRI j=%d ri="FWF"\n",j,fcj[j+1].r2dpy);
		#endif
		if (z1[j]<z2[j])
		{
			zi1=z1[j];
			zi2=z2[j];
			++j;
			zj1=z1[j];
			zj2=z2[j];
		}
		else if (z1[j]>z2[j])
		{
			zi1=z2[j];
			zi2=z1[j];
			++j;
			zj1=z2[j];
			zj2=z1[j];
		}
		else
		{
			zi1=zi2=z1[j];
			ni1 = jbm_max(0, floorl((zi1 - ts->zmin) / ts->dz));
			if (ni1>=n) continue;
			r2 = fcj[j].r2dpy;
			r1=r2/dy;
			r2*=r1/friction_beta;
			++j;
			if (z1[j]<z2[j])
			{
				zj1=z1[j];
				zj2=z2[j];
			}
			else if (z1[j]>z2[j])
			{
				zj1=z2[j];
				zj2=z1[j];
			}
			else
			{
				zj1=zj2=z1[j];
				nj1 = jbm_max(0,
					jbm_min(n, floorl((zj1 - ts->zmin) / ts->dz)));
				for (i=ni1; ++i<=nj1;)
					section_rhb(fp+i,ts->zmin+i*ts->dz-zi1,r1,r2,fcj+j-1);
				dh1=dh2=zj2-zi2;
				r4 = fcj[j].r2dpy;
				r3=r4/dy;
				r4*=r3/friction_beta;
				goto pressure;
			}
			nj1 = jbm_max(0, jbm_min(n, floorl((zj1 - ts->zmin) / ts->dz)));
			nj2 = jbm_max(0, jbm_min(n, floorl((zj2 - ts->zmin) / ts->dz)));
			for (i=ni1; ++i<=nj1;)
				section_rhb(fp+i,ts->zmin+i*ts->dz-zi1,r1,r2,fcj+j-1);
			r4 = fcj[j].r2dpy;
			r3 = r4 / fcj[j].dzy;
			r4*=r3/friction_beta;
			for (; i<=nj2; ++i)
			{
				z = ts->zmin + i * ts->dz;
				dh=z-zi1;
				r2 = fcj[j-1].r2dpy;
				r1 = r2 * fcj[j].dzy / (zj2-z);
				r2*=r1/friction_beta;
				section_rhb(fp+i,dh,r1,r2,fcj+j-1);
				section_rhpa(fp+i,dh,zj1-zi1,r3,r4);
			}
			dh1=zj1-zi1;
			dh2=zj2-zi2;
			goto pressure;
		}
		ni1 = jbm_max(0, floorl((zi1 - ts->zmin) / ts->dz));
		if (ni1>n) continue;
		r2 = fcj[j-1].r2dpy;
		r1 = r2 * fcj[j-1].dzy;
		r2*=r1/friction_beta;
		ni2 = jbm_max(0, jbm_min(n, floorl((zi2 - ts->zmin) / ts->dz)));
		nj1 = jbm_max(0, jbm_min(n, floorl((zj1 - ts->zmin) / ts->dz)));
		nj2 = jbm_max(0, jbm_min(n, floorl((zj2 - ts->zmin) / ts->dz)));
		#if DEBUG_SECTION_RI
			fprintf(stderr,"SRI ni1=%d ni2=%d nj1=%d nj2=%d\n",ni1,ni2,nj1,nj2);
			fprintf(stderr,"SRI r1="FWL" r2="FWL"\n",r1,r2);
		#endif
		r4 = fcj[j].r2dpy;
		dh1=zj1-zi1;
		dh2=zj2-zi2;
		if (dh1<=0. && dh2<=0.) continue;
		i=ni1;
		if (dh1==dh2)
		{
			r3=r4/dy;
			r4*=r3/friction_beta;
			if (zj1>=zi2)
			{
				for (; ++i<=ni2;)
					section_rhc(fp+i,ts->zmin+i*ts->dz-zi1,r1,r2,fcj+j-1);
				for (; i<=nj1; ++i)
				{
					z = ts->zmin + i * ts->dz;
					section_rha(fp+i,z-zi1,z-zi2,r1,r2,fcj+j-1);
				}
				for (; i<=nj2; ++i)
				{
					z = ts->zmin + i * ts->dz;
					k2=(z-zj1)/(zj2-zj1);
					section_rha(fp+i,dh1,z-zi2,r1,r2,fcj+j-1);
					section_rhpb(fp+i,dh1,r3/k2,r4/k2);
				}
			}
			else
			{
				for (; ++i<=nj1;)
					section_rhc(fp+i,ts->zmin+i*ts->dz-zi1,r1,r2,fcj+j-1);
				for (; i<=ni2; ++i)
				{
					z = ts->zmin + i * ts->dz;
					k2=(z-zj1)/(zj2-zj1);
					section_rhc(fp+i,dh1,r1,r2,fcj+j-1);
					section_rhpb(fp+i,dh1,r3/k2,r4/k2);
				}
				for (; i<=nj2; ++i)
				{
					z = ts->zmin + i * ts->dz;
					k2=(z-zj1)/(zj2-zj1);
					section_rha(fp+i,dh1,z-zi2,r1,r2,fcj+j-1);
					section_rhpb(fp+i,dh1,r3/k2,r4/k2);
				}
			}
		}
		else
		{
			r3=r4*(dh1-dh2)/dy;
			r4*=r3/friction_beta;
			if (zj1>=zj2)
			{
				for (; ++i<=ni2;)
					section_rhc(fp+i,ts->zmin+i*ts->dz-zi1,r1,r2,fcj+j-1);
				for (; i<=nj2; ++i)
				{
					z = ts->zmin + i * ts->dz;
					section_rha(fp+i,z-zi1,z-zi2,r1,r2,fcj+j-1);
				}
				for (; i<=nj1; ++i)
				{
					z = ts->zmin + i * ts->dz;
					k2=(z-zj1)/(zj2-zj1);
					dh=dh1+(dh2-dh1)*k2;
					section_rha(fp+i,z-zi1,dh,r1,r2,fcj+j-1);
					section_rhpa(fp+i,dh,dh2,r3,r4);
				}
			}
			else if (zj1>=zi2)
			{
				for (; ++i<=ni2;)
					section_rhc(fp+i,ts->zmin+i*ts->dz-zi1,r1,r2,fcj+j-1);
				for (; i<=nj1; ++i)
				{
					z = ts->zmin + i * ts->dz;
					section_rha(fp+i,z-zi1,z-zi2,r1,r2,fcj+j-1);
				}
				for (; i<=nj2; ++i)
				{
					z = ts->zmin + i * ts->dz;
					k2=(z-zj1)/(zj2-zj1);
					dh=dh1+(dh2-dh1)*k2;
					section_rha(fp+i,dh,z-zi2,r1,r2,fcj+j-1);
					section_rhpa(fp+i,dh1,dh,r3,r4);
				}
			}
			else
			{
				for (; ++i<=nj1;)
					section_rhc(fp+i,ts->zmin+i*ts->dz-zi1,r1,r2,fcj+j-1);
				for (; i<=ni2; ++i)
				{
					z = ts->zmin + i * ts->dz;
					k2=(z-zj1)/(zj2-zj1);
					dh=dh1+(dh2-dh1)*k2;
					section_rhc(fp+i,dh,r1,r2,fcj+j-1);
					section_rhpa(fp+i,dh1,dh,r3,r4);
				}
				for (; i<=nj2; ++i)
				{
					z = ts->zmin + i * ts->dz;
					k2=(z-zj1)/(zj2-zj1);
					dh=dh1+(dh2-dh1)*k2;
					section_rha(fp+i,dh,z-zi2,r1,r2,fcj+j-1);
					section_rhpa(fp+i,dh1,dh,r3,r4);
				}
			}
		}
pressure:
		fpj->r = fpj->beta = 0.;
		if (dh1>dh2) section_rhpa(fpj,dh1,dh2,r3,r4);
		else if (dh1<dh2) section_rhpa(fpj,dh2,dh1,r3,r4);
		else section_rhpb(fpj,dh1,r3,r4);
		for (; i<=n; ++i) fp[i].r += fpj->r, fp[i].beta += fpj->beta;
	}
	r1 = fcj[j].r2dpy;
	r2=r1*r1/friction_beta;
	if (j==nj)
	{
		if (z1[j]<z2[j])
		{
			zi1=z1[j];
			zi2=z2[j];
		}
		else if (z1[j]>z2[j])
		{
			zi1=z2[j];
			zi2=z1[j];
		}
		else
		{
			r1/=dy;
			r2/=dy;
			zi1=z1[j];
			ni1 = jbm_max(0, floorl((zi1 - ts->zmin) / ts->dz));
			#if DEBUG_SECTION_RI
				fprintf(stderr,"SRI zi1="FWL" zmin="FWF" dz="FWF"\n",
					zi1,ts->zmin,ts->dz);
				fprintf(stderr,"SRI ni1=%d n=%d\n",ni1,n);
			#endif
			for (i=ni1; ++i<=n;)
				section_rhb(fp+i,ts->zmin+i*ts->dz-zi1,r1,r2,fcj+j);
			goto exit;
		}
		r1 *= fcj[j].dzy;
		r2 *= fcj[j].dzy;
		ni1 = jbm_max(0, floorl((zi1 - ts->zmin) / ts->dz));
		if (ni1>=n) goto exit;
		ni2 = jbm_max(0, jbm_min(n, floorl((zi2 - ts->zmin) / ts->dz)));
		#if DEBUG_SECTION_RI
			fprintf(stderr,"SRI z1="FWL" z2="FWL" zmin="FWF" dz="FWF"\n",
				zi1,zi2,ts->zmin,ts->dz);
			fprintf(stderr,"SRI ni1=%d ni2=%d n=%d\n",ni1,ni2,n);
		#endif
		for (i=ni1; ++i<=ni2;)
			section_rhc(fp+i,ts->zmin+i*ts->dz-zi1,r1,r2,fcj+j);
		for (; i<=n; ++i)
		{
			z = ts->zmin + i * ts->dz;
			section_rha(fp+i,z-zi1,z-zi2,r1,r2,fcj+j);
		}
	}
exit:

	#if DEBUG_SECTION_RI
		for (i=0; ++i<=n;)
			fprintf(stderr,"SRI i=%d r="FWF" beta="FWF"\n",i,fp[i].r,fp[i].beta);
		fprintf(stderr,"Section ri: end\n");
	#endif
	return;
}

#if INLINE_SECTION_RI
	#define section_ri _section_ri
#else
	void section_ri(Section*,TransientSection*,SectionParameters*,
		FrictionParameters*,JBDOUBLE,JBDOUBLE,int,int*,int,FrictionCoefficients*);
#endif

static inline void _section_open_parameters(Section *s)
{
	register int i;
	register JBDOUBLE k,B,dz;
	register SectionParameters *sp;
	#if DEBUG_SECTION_OPEN_PARAMETERS
		fprintf(stderr,"Section open parameters: start\n");
	#endif
	s->e = 0;
	sp = s->sp;
	sp->A = sp->I = 0.;
	for (i=0; ++i<=s->n; ++sp)
	{
		dz = (sp+1)->z - sp->z;
		if ((sp+1)->B <  sp->B) s->e = 1;
		B = sp->B + sp->dBz * dz;
		(sp+1)->A = sp->A + 0.5 * (B + sp->B) * dz;
		k = (sp+1)->z + sp->z;
		(sp+1)->I = sp->I + 1./6. * dz * (B * (k + (sp+1)->z)
			+ sp->B * (k + sp->z));
	}
	sp = s->sp;
	if (s->e)
	{
		sp = s->sp;
		sp->c = 0.;
		for (i=0; ++i<=s->n;)
		{
			++sp;
			sp->c = fmax((sp-1)->c, wave_velocity(JBM_G, sp->A, sp->B));
		}
	}
	s->zmax = s->sp->z + s->hmax;
	s->Amax = section_area(s, s->zmax);
	#if DEBUG_SECTION_OPEN_PARAMETERS
		fprintf(stderr,"Section open parameters: end\n");
	#endif
}

#if INLINE_SECTION_OPEN_PARAMETERS
	#define section_open_parameters _section_open_parameters
#else
	void section_open_parameters(Section*);
#endif

static inline int _section_open(Section *s,TransientSection *ts)
{
	int i,j;
	int *ni,*nj,*nij,*nk,nmin,n,nr;
	JBFLOAT *py,*pz,*y,*z;
	JBDOUBLE k1,k2,A,R,Av;
	FrictionCoefficients *fc;
	SectionParameters *sp;
	FrictionParameters *fp;
	FrictionCoefficients *rfc;

	#if DEBUG_SECTION_OPEN
		fprintf(stderr,"section_open: start\n");
		transient_section_print(ts,stderr);
	#endif

	i = ts->n + 1;
	py = (JBFLOAT*)g_try_malloc(i * 2 * sizeof(JBFLOAT));
	if (!py)
	{
		i=0;
		jbw_show_error2(gettext("Section"),gettext("Not enough memory"));
		goto exit4;
	}
	pz=py+i;
	while (--i>=0) py[i] = ts->sp[i].y, pz[i] = ts->sp[i].z;

	n = jbm_transversal_section_regions(py,pz,ts->n,&z,&ni,&nj,&nij,&nmin);
	if (!n)
	{
		i=0;
		jbw_show_error2(gettext("Section"),gettext("Not enough memory"));
		goto exit4;
	}
	nr = ceill(ts->hmax / ts->dz);
	if (!section_create(s,n,nr)) goto exit2;

	for (i=0, sp=s->sp; i<=n; ++i, ++sp)
	{
		sp->P = sp->dPz = 0.;
		sp->z = z[i];
	}
	g_free(z);

	#if DEBUG_SECTION_OPEN
		for (i=0; i<=n; ++i) fprintf(stderr,"SO i=%d z="FWF"\n",i,s->sp[i].z);
	#endif

	for (i=j=0, nk=nij, sp=s->sp; i<nmin; ++i, ++sp, nk+=ts->n)
		section_B(ts,sp,nj[i],nk,section_Bm);
	for (; i<ni[0]; ++i, ++sp, nk+=ts->n)
		section_B(ts,sp,nj[i],nk,section_Bl);
	for (; i<ni[ts->n]; ++i, ++sp, nk+=ts->n)
		section_B(ts,sp,nj[i],nk,section_Br);
	for (; i<n; ++i, ++sp, nk+=ts->n)
		section_B(ts,sp,nj[i],nk,section_Bc);
	sp->B = fabs(ts->sp[ts->n].y - ts->sp[0].y);
	sp->dBz = 0.;
	section_P(ts,s->sp,n,ni);
	g_free(ni);
	g_free(nj);
	#if DEBUG_SECTION_OPEN
		for (i=0; i<=n; ++i)
			fprintf(stderr,"SO i=%d B="FWF" dBz="FWF" z="FWF"\n"
				"SO i=%d P="FWF" dPz="FWF"\n",
				i,s->sp[i].B,s->sp[i].dBz,s->sp[i].z,
				i,s->sp[i].P,s->sp[i].dPz);
	#endif

	fc = (FrictionCoefficients*)
		g_try_malloc(sizeof(FrictionCoefficients) * ts->n);
	if (!fc) goto exit1;

	k2 = ts->sp->z;
	if (ts->sp->t) k2 += ts->sp->r;
	for (i=0, rfc=fc; i<ts->n; ++i, ++rfc)
	{
		k1 = fabs((ts->sp[i+1].z - ts->sp[i].z) /
			(ts->sp[i+1].y - ts->sp[i].y));
		rfc->dzy = k1;
		rfc->t = ts->sp[i].t;
		switch (rfc->t)
		{
		case 0:
			rfc->r2dpy = JBM_G * jbm_fsqr(ts->sp[i].r);
			#if MODEL_FRICTION == MODEL_FRICTION_MINIMIZATION_STRESS
				rfc->l3 = rfc->l4 = rfc->l5 =
			#endif
			rfc->l = rfc->l1 = rfc->l2 = 0.;
			k2 = fmin(k2, fmin(ts->sp[i].z, ts->sp[i+1].z));
			break;
		default:
			rfc->l = ts->sp[i].r;
			k2 = fmin(k2, fmin(ts->sp[i].z, ts->sp[i+1].z) + rfc->l);
			rfc->r2dpy = friction_b1b1 * sys->granulometric_coefficient *
				pow(ts->sp[i].r, friction_2b);
			rfc->l1 = pow(rfc->l, friction_b1);
			rfc->l2 = pow(rfc->l, friction_2b1);
			#if MODEL_FRICTION == MODEL_FRICTION_MINIMIZATION_STRESS
				rfc->l3 = pow(rfc->l, friction_2b2);
				rfc->l4 = pow(rfc->l, friction_3b2);
				rfc->l5 = pow(rfc->l, friction_4b3);
			#endif
		}
		#if (MODEL_FRICTION == MODEL_FRICTION_MINIMIZATION_STRESS) || \
			(MODEL_FRICTION == MODEL_FRICTION_MINIMIZATION_LOSS) && \
			(MODEL_FRICTION_STRESS == MODEL_FRICTION_STRESS_P)
			rfc->r2dpy*=sqrt(1.+k1*k1);
		#endif
		#if DEBUG_SECTION_OPEN
			fprintf(stderr,"SO i=%d t=%d dzy="FWF" r2dpy="FWF"\n",
				i,rfc->t,rfc->dzy,rfc->r2dpy);
			fprintf(stderr,"SO l="FWF" l1="FWF" l2="FWF"\n",
				rfc->l,rfc->l1,rfc->l2);
			#if MODEL_FRICTION == MODEL_FRICTION_MINIMIZATION_STRESS
				fprintf(stderr,"SO l3="FWF" l4="FWF" l5="FWF"\n",
					rfc->l3,rfc->l4,rfc->l5);
			#endif
		#endif
	}
	#if DEBUG_SECTION_OPEN
		fprintf(stderr,"SO calculating extremes\n");
	#endif
	s->dz = ts->dz;
	s->zmin = k2;
	s->hmin = k2 - ts->zmin;
	if (s->hmin < ts->dz)
	{
		s->zmin += ts->dz - s->hmin;
		s->hmin = ts->dz;
	}
	s->hmax = ts->hmax;

	#if DEBUG_SECTION_OPEN
		fprintf(stderr,"SO starting coefficients\n");
	#endif
	for (i=0, fp=s->fp; i<=s->nf; ++i, ++fp) fp->r = fp->beta = 0.;

	#if DEBUG_SECTION_OPEN
		fprintf(stderr,"SO transversal regions\n");
	#endif
	nr = jbm_transversal_section_regions(pz,py,ts->n,&y,&ni,&nj,&nij,&nmin);
	#if DEBUG_SECTION_OPEN
		fprintf(stderr,"SO freeing friction coefficients\n");
	#endif
	if (!nr)
	{
		g_free(fc);
		goto exit1;
	}

	#if DEBUG_SECTION_OPEN
		fprintf(stderr,"SO calculating friction\n");
	#endif
	for (i=0, nk=nij; i<nr; ++i, nk+=ts->n)
		section_ri(s,ts,s->sp,s->fp,y[i],y[i+1],nj[i],nk,s->nf,fc);
	#if DEBUG_SECTION_OPEN
		fprintf(stderr,"SO freeing memory\n");
	#endif
	g_free(fc);
	g_free(y);
	g_free(ni);
	g_free(nj);

	#if DEBUG_SECTION_OPEN
		fprintf(stderr,"SO calculating width\n");
	#endif
	sp = s->sp;
	k1 = sp->B;
	k2 = sys->section_width_min * k1;
	for (i=j=0; ++i<=s->n;)
	{
		++sp;
		if (sp->B > k1) k1 = sp->B, k2 = sys->section_width_min * k1;
		else if (sp->B < k2) sp->B = k2;
	}

	s->zleft = ts->sp[0].z;
	s->zright = ts->sp[ts->n].z;
	s->u = ts->u;

	#if DEBUG_SECTION_OPEN
		fprintf(stderr,"SO opening parameters\n");
	#endif
	section_open_parameters(s);

	for (i=0, A=0., fp=s->fp; ++i<=s->nf;)
	{
		++fp;
		k1 = ts->zmin + i * ts->dz;
		Av=A;
		A = section_area(s, k1);
		R = A / section_perimeter(s, k1);
		#if DEBUG_SECTION_OPEN
			fprintf(stderr,"SO i=%d z="FWL" A="FWL" R="FWL"\n",i,k1,A,R);
			fprintf(stderr,"SO Av="FWL" r="FWF" contraction="FWF"\n",
				Av,fp->r,ts->contraction);
		#endif
		if (fp->r > 0. && isfinite(fp->r)) fp->beta *= A / jbm_fsqr(fp->r);
		else fp->beta = 1.;
		fp->beta += ts->contraction;
		fp->dbetaA = (fp->beta - (fp-1)->beta) / (A - Av);
		#if MODEL_FRICTION == MODEL_FRICTION_MINIMIZATION_STRESS
			fp->r = 1. / fp->r;
		#elif (MODEL_FRICTION == MODEL_FRICTION_MINIMIZATION_LOSS) || \
			(MODEL_FRICTION == MODEL_FRICTION_CONSTANT_SLOPE)
			fp->r = A / jbm_fsqr(fp->r);
		#endif
		fp->r *= A * pow(R, friction_2b1);
		#if DEBUG_SECTION_OPEN
			fprintf(stderr,"SO r="FWF" beta="FWF" dbetaA="FWF"\n",
				fp->r,fp->beta,fp->dbetaA);
		#endif
	}
	fp = s->fp;
	fp[0].r = fp[1].r;
	fp[0].beta = fp[1].beta;	
	fp[0].dbetaA = fp[1].dbetaA = 0.;	

	#if DEBUG_SECTION_OPEN
		section_print(s,stderr);
	#endif
	goto exit3;

exit1:
	jbw_show_error2(gettext("Section"),gettext("Not enough memory"));
exit2:
	i=0;
exit3:
	g_free(py);
exit4:
	#if DEBUG_SECTION_OPEN
		fprintf(stderr,"section_open: end\n");
	#endif
	return i;
}

#if INLINE_SECTION_OPEN
	#define section_open _section_open
#else
	int section_open(Section*,TransientSection*);
#endif

#endif

