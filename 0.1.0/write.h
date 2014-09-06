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

/*
write.h

Header file to define file inputs and outputs

Author: Javier Burguete Tolosa
Copyright (c) 2005-2014. All rights reserved
*/


#ifndef WRITE_H
#define WRITE_H 1

#include "flow_scheme.h"

extern int nstep;
extern JBDOUBLE t0,ti,tf;

/*
int write_data(FILE *file,Variables *v,Parameters *p,
	TransportParameters *tp,int n,int vsize,int nt)

Function to write the solution in a binary file

Inputs:
	file= binary file of the solution
	v= arrays of variables
	p= array of flow parameters
	tp= array of transport parameters
	n= number of mesh cells
	vsize= size of the variables of a mesh cell in bytes
	nt= number of transported solutes

Output:
	i= number of saved mesh cells
	0= error
*/

static inline int _write_data(FILE *file,Variables *v,Parameters *p,
	TransportParameters *tp,int n,int vsize,int nt)
{
	int i,j;
	#if DEBUG_WRITE_DATA
		if (t>=DEBUG_TIME) fprintf(stderr,"write_data: start\n");
	#endif
	variables_actualize(v);
	j=(n+1)*vsize;
	i=fwrite(v->x,1,j,file);
   	if (i!=j) j=0;
	++nstep;
	#if DEBUG_WRITE_DATA
		char *prueba;
		if (t>=DEBUG_TIME)
		{
			prueba=(char*)v->x;
			fprintf(stderr,"WD v1=%d v2=%d\n",(unsigned char)prueba[i-1],
				(unsigned char)prueba[i]);
			fprintf(stderr,"WD n=%d i=%d j=%d vsize=%d\n",n,i,j,vsize);
			fprintf(stderr,"write_data: end\n");
		}
	#endif
	return j;
}

#if INLINE_WRITE_DATA
	#define write_data _write_data
#else
	int write_data
		(FILE*,Variables*,Parameters*,TransportParameters*,int,int,int);
#endif

/*
int read_data(FILE *file,Variables *v,int n,int vsize)

Function to read the solution in a binary file

Inputs:
	file= binary file of the solution
	v= arrays of variables
	n= number of mesh cells
	vsize= size of the variables of a mesh cell in bytes

Output:
	i= number of read mesh cells
	0= error
*/

static inline int _read_data(FILE *file,Variables *v,int n,int vsize)
{
	register int j;
	#if DEBUG_READ_DATA
		fprintf(stderr,"Read data: start\n");
		fprintf(stderr,"RD n=%d\n",n);
	#endif
	j=fread(v->x,vsize,n+1,file);
	#if DEBUG_READ_DATA
		int i;
		fprintf(stderr,"RD j=%d\n",j);
		for (i=0; i<=n; ++i) fprintf(stderr,"RD i=%d x="FWF" Q="FWF"\n",
			i,v->x[i],v->Q[i]);
		fprintf(stderr,"Read data: end\n");
	#endif
	return j;
}

#if INLINE_READ_DATA
	#define read_data _read_data
#else
	int read_data(FILE*,Variables*,int,int);
#endif

/*
void write_variables(FILE *file,Variables *v,int i,int n,int nt)

Function to write the variables of some mesh cells in an ASCII file

Inputs:
	file= binary file of the solution
	v= arrays of variables
	i= number of the first mesh cell to write
	n= number of mesh cells to write
	nt= number of transported solutes
*/

static inline void _write_variables(FILE *file,Variables *v,int i,int n,int nt)
{
	int j;
	JBFLOAT *p;
	#if DEBUG_WRITE_VARIABLES
		fprintf(stderr,"Write variables: start\n");
	#endif
	fprintf(file,FWF2 FWF2 FWF2 FWF2 FWF2 FWF2 FWF2 FWF,v->x[i],v->Q[i],v->A[i],
		v->B[i],v->zs[i],v->zb[i],v->zmax[i],v->beta[i]);
	for (j=0, p=v->p+i, ++n; j<=nt; ++j, p+=n) fprintf(file," "FWF,*p);
	fprintf(file,"\n");
	#if DEBUG_WRITE_VARIABLES
		fprintf(stderr,"Write variables: end\n");
	#endif
}

#if INLINE_WRITE_VARIABLES
	#define write_variables _write_variables
#else
	void write_variables(FILE*,Variables*,int,int,int);
#endif

/*
void read_profile(FILE *file,Variables *v,int n,int profile,int vsize)

Function to read a longitudinal profile of a system of channels in an ASCII file

Inputs:
	file= binary file of the solution
	v= arrays of variables
	n= number of mesh cells
	profile= number of the saved profile
	vsize= size of the variables of a mesh cell in bytes
*/

static inline void _read_profile
	(FILE *file,Variables *v,int n,int profile,int vsize)
{
	#if DEBUG_READ_PROFILE
		fprintf(stderr,"Read profile: start\n");
		fseek(file,0L,SEEK_END);
		fprintf(stderr,"RP: length=%ld\n",ftell(file));
		fprintf(stderr,"RP: n=%d profile=%d\n",n,profile);
	#endif
	fseek(file,vsize*(n+1)*profile,SEEK_SET);
	read_data(file,v,n,vsize);
	#if DEBUG_READ_PROFILE
		fprintf(stderr,"Read profile: end\n");
	#endif
}

#if INLINE_READ_PROFILE
	#define read_profile _read_profile
#else
	void read_profile(FILE*,Variables*,int,int,int);
#endif

/*
void write_profile(FILE *file_read,FILE *file_write,int i,int i2,
	int n,int profile,int *vsize,int nt)

Function to write a longitudinal profile of a channel in an ASCII file

Inputs:
	file_read= binary file to read the solution
	file_write= ASCII file to write the longitudinal profile
	i= number of the initial mesh cell of the channel
	i2= number of the final mesh cell of the channel
	n= number of mesh cells
	profile= number of the saved profile
	vsize= pointer to the size of the variables of a mesh cell in bytes
	nt= number of transported solutes
*/

static inline void _write_profile(FILE *file_read,FILE *file_write,int i,int i2,
	int n,int profile,int *vsize,int nt)
{
	int j;
	Variables v[1];
	#if DEBUG_WRITE_PROFILE
		fprintf(stderr,"Write profile: start\n");
	#endif
	v->x = NULL;
	if (!variables_create(v,n,vsize,nt))
	{
		jbw_show_error2(gettext("Save solution"),gettext("Not enough memory"));
		#if DEBUG_WRITE_PROFILE
			fprintf(stderr,"Write profile: end\n");
		#endif
		return;
	}
	#if DEBUG_WRITE_PROFILE
		fprintf(stderr,"WP reading profile\n");
	#endif
	read_profile(file_read,v,n,profile,*vsize);
	#if DEBUG_WRITE_PROFILE
		fprintf(stderr,"WP writing variables\n");
	#endif
	for (j=i; j<=i2; ++j) write_variables(file_write,v,j,n,nt);
	#if DEBUG_WRITE_PROFILE
		fprintf(stderr,"WP freeing variables\n");
	#endif
	jb_free_null((void**)&v->x);
	#if DEBUG_WRITE_PROFILE
		fprintf(stderr,"Write profile: end\n");
	#endif
}

#if INLINE_WRITE_PROFILE
	#define write_profile _write_profile
#else
	void write_profile(FILE*,FILE*,int,int,int,int,int*,int);
#endif

/*
void write_evolution(FILE *file_read,FILE *file_write,int n,
	int section,int *vsize,Channel *channel)

Function to write the temporal evolution of a channel cross section in an ASCII
file

Inputs:
	file_read= binary file to read the solution
	file_write= ASCII file to write the temporal evolution of the cross section
	n= number of mesh cells
	section= number of the channel cross section
	vsize= pointer to the size of the variables of a mesh cell in bytes
	channel= pointer to the channel
*/

static inline void _write_evolution(FILE *file_read,FILE *file_write,int n,
	int section,int *vsize,Channel *channel)
{
	long int i,j;
	Variables v[1];
	#if DEBUG_WRITE_EVOLUTION
		fprintf(stderr,"Write evolution: start\n");
	#endif
	v->x = NULL;
	if (!variables_create(v,n,vsize,channel->nt))
	{
		jbw_show_error2(gettext("Time evolution"),gettext("Not enough memory"));
		#if DEBUG_WRITE_EVOLUTION
			fprintf(stderr,"Write evolution: end\n");
		#endif
		return;
	}
	section = channel->cg->cs[section].i;
	j=jb_flength(file_read)/(*vsize*(n+1));
	for (i=0; i<j; ++i)
	{
		read_data(file_read,v,n,*vsize);
		v->x[section] = fmin(i*ti, tf);
		write_variables(file_write,v,section,n,channel->nt);
	}
	jb_free_null((void**)&v->x);
	#if DEBUG_WRITE_EVOLUTION
		fprintf(stderr,"Write evolution: end\n");
	#endif
}

#if INLINE_WRITE_EVOLUTION
	#define write_evolution _write_evolution
#else
	void write_evolution(FILE*,FILE*,int,int,int*,Channel*);
#endif

/*
void read_evolution(int channel,int section,int type1,int type2,
	JBFLOAT **x,JBFLOAT **y1,JBFLOAT **z1,int *nt,int vsize)

Function to read the temporal evolution of some variables of a channel cross
section to make a graphical

Inputs:
	channel= number of the channel
	section= number of the channel cross section
	type1= type of the first variable to read
	type2= type of the second variable to read
	x= pointer to the array of times
	y1= pointer to the array of the first variable
	z1= pointer to the array of the second variable
	nt= pointer to the number of solutes
	vsize= size of the variables of a mesh cell in bytes
*/

static inline void _read_evolution(int channel,int section,int type1,int type2,
	JBFLOAT **x,JBFLOAT **y1,JBFLOAT **z1,int *nt,int vsize)
{
	int ii,nv;
	long int i,j,k;
	FILE *file;
	#if DEBUG_READ_EVOLUTION
		fprintf(stderr,"Read evolution: start\n");
	#endif
	*x=*y1=*z1=0;
	if (t==0.)
	{
		#if DEBUG_READ_EVOLUTION
			fprintf(stderr,"Read evolution: end\n");
		#endif
		return;
	}
	file=g_fopen(sys->solution_path,"rb");
	if (!file)
	{
		jbw_show_error2(gettext("Solution"),gettext("Unable to open the file"));
		#if DEBUG_READ_EVOLUTION
			fprintf(stderr,"Read evolution: end\n");
		#endif
		return;
	}
	nv = vsize / sizeof(JBFLOAT);
	#if DEBUG_READ_EVOLUTION
		fprintf(stderr,"RE nv=%d\n",nv);
	#endif
	register JBDOUBLE kk;
	JBFLOAT p[nv];
	k=vsize*(n+1);
	j=jb_flength(file)/k;
	#if DEBUG_READ_EVOLUTION
		fprintf(stderr,"RE j=%ld\n",j);
	#endif
	*x = (JBFLOAT*)g_try_malloc(j*3*sizeof(JBFLOAT));
	if (!*x)
	{
		fclose(file);
		jbw_show_error2(gettext("Time evolution"),gettext("Not enough memory"));
		#if DEBUG_READ_EVOLUTION
			fprintf(stderr,"Read evolution: end\n");
		#endif
		return;
	}
	*y1 = *x + j;
	*z1 = *y1 + j;
	fseek(file, sys->channel[channel].cg->cs[section].i * sizeof(JBFLOAT),
		SEEK_SET);
	#if DEBUG_READ_EVOLUTION
		fprintf(stderr,"RE t1=%d t2=%d j=%d n=%d\n",
			type1,type2,sys->channel[channel].cg->cs[section].i,n);
	#endif
	for (i=0; i<j; ++i)
	{
		(*x)[i]=i*ti;
		#if DEBUG_READ_EVOLUTION
			fprintf(stderr,"RE pos=%ld\n",ftell(file));
		#endif
		for (ii=0; ii<nv; ++ii)
		{
			fread(p+ii,sizeof(JBFLOAT),1,file);
			fseek(file,n*sizeof(JBFLOAT),SEEK_CUR);
			#if DEBUG_READ_EVOLUTION
				fprintf(stderr,"RE pos=%ld\n",ftell(file));
			#endif
		}
		switch (type1)
		{
		case 0:
			kk=p[1];
			break;
		case 1:
			kk=p[2]-p[5];
			break;
		case 2:
			kk=p[2];
			break;
		case 3:
			if (p[3] > 0.) kk = p[1] / p[3]; else kk = 0.;
			break;
		case 4:
			kk = p[3] * sqrt(JBM_G * p[3] / (p[4] * p[7]));
			if (kk > 0.) kk = p[1] / kk; else kk = 0.;
			break;
		case 5:
			kk=p[3];
			break;
		case 6:
			kk=p[4];
			break;
		default:
			kk=p[type1+1];
		}
		(*y1)[i]=kk;
		switch (type2)
		{
		case 0:
			kk=p[1];
			break;
		case 1:
			kk=p[2]-p[5];
			break;
		case 2:
			kk=p[2];
			break;
		case 3:
			if (p[3] > 0.) kk = p[1] / p[3]; else kk = 0.;
			break;
		case 4:
			kk = p[3] * sqrt(JBM_G * p[3] / (p[4] * p[7]));
			if (kk > 0.) kk = p[1] / kk; else kk = 0.;
			break;
		case 5:
			kk=p[3];
			break;
		case 6:
			kk=p[4];
			break;
		default:
			kk=p[type2+1];
		}
		(*z1)[i]=kk;
	}
	--i;
	(*x)[i]=fmin((*x)[i],tf);
	*nt=i;
	fclose(file);
	#if DEBUG_READ_EVOLUTION
		for (i=0; i<=*nt; ++i)
			fprintf(stderr,"x="FWF" y1="FWF" z1="FWF"\n",
				(*x)[i],(*y1)[i],(*z1)[i]);
		fprintf(stderr,"Read evolution: end\n");
	#endif
}

#if INLINE_READ_EVOLUTION
	#define read_evolution _read_evolution
#else
	void read_evolution(int,int,int,int,JBFLOAT**,JBFLOAT**,JBFLOAT**,int*,int);
#endif

/*

Functions not yet implemented

static inline void _write_advance(FILE *file,Parameters *p,int n,JBDOUBLE t)
{
	register int i,j;
	#if DEBUG_WRITE_ADVANCE
		fprintf(stderr,"Write advance: start\n");
	#endif
	for (i=0; i<=n && p[i].V==0.;) ++i;
	if (i>n) i=0;
	for (j=0; j<=n && p[j].V!=0.;) ++j;
	if (j>n) j=0;
	fprintf(file,FWL" "FWF" "FWF"\n",t,p[j].x,p[i].x);
	#if DEBUG_WRITE_ADVANCE
		fprintf(stderr,"Write advance: end\n");
	#endif
}

#if INLINE_WRITE_ADVANCE
	#define write_advance _write_advance
#else
	void write_advance(FILE*,Parameters*,int,JBDOUBLE);
#endif

static inline void _write_plume(FILE *file,Parameters *p,
	TransportParameters *tp,int n,JBDOUBLE t,JBDOUBLE danger,Channel *c)
{
	register int i,j,k;
	#if DEBUG_WRITE_PLUME
		fprintf(stderr,"Write plume: start\n");
	#endif
	if (c->nt < 0) return;
	fprintf(file,FWL" ",t);
	for (i=0; i<=c->nt; ++i)
	{
		for (j=0; tp[j].c<danger && j<n;) ++j;
		for (k=n; tp[k].c<danger && k>0;) --k;
		fprintf(file,FWF" "FWF" ",p[j].x,p[k].x);
	}
	fprintf(file,"\n");
	#if DEBUG_WRITE_PLUME
		fprintf(stderr,"Write plume: end\n");
	#endif
}

#if INLINE_WRITE_PLUME
	#define write_plume _write_plume
#else
	void write_plume
		(FILE*,Parameters*,TransportParameters*,int,JBDOUBLE,JBDOUBLE,Channel*);
#endif

*/

#endif
