/*
SWIGS (Shallow Water in Irregular Geometries Simulator): a software to simulate
transient or steady flows with solute transport in channels, channel networks and rivers

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

#include "write.h"

#if !INLINE_WRITE_DATA
int write_data(FILE *file,Variables *v,Parameters *p,TransportParameters *tp,
	int n,int vsize,int nt)
{return _write_data(file,v,p,tp,n,vsize,nt);}
#endif

#if !INLINE_READ_DATA
int read_data(FILE *file,Variables *v,int n,int vsize)
{return _read_data(file,v,n,vsize);}
#endif

#if !INLINE_WRITE_VARIABLES
void write_variables(FILE *file,Variables *v,int i,int n,int nt)
{_write_variables(file,v,i,n,nt);}
#endif

#if !INLINE_READ_PROFILE
void read_profile(FILE *file,Variables *v,int n,int profile,int vsize)
{_read_profile(file,v,n,profile,vsize);}
#endif

#if !INLINE_WRITE_PROFILE
void write_profile(FILE *file_read,FILE *file_write,int i,int i2,int n,
	int profile,int *vsize,int nt)
{_write_profile(file_read,file_write,i,i2,n,profile,vsize,nt);}
#endif

#if !INLINE_WRITE_EVOLUTION
void write_evolution
	(FILE *file_read,FILE *file_write,int n,int section,int *vsize,Channel *c)
{_write_evolution(file_read,file_write,n,section,vsize,c);}
#endif

#if !INLINE_READ_EVOLUTION
void read_evolution(int channel,int section,int type1,int type2,JBFLOAT **x,
	JBFLOAT **y1,JBFLOAT **z1,int *n,int vsize)
{_read_evolution(channel,section,type1,type2,x,y1,z1,n,vsize);}
#endif

/*

Not yet implemented

#if !INLINE_WRITE_ADVANCE
void write_advance(FILE *file,Parameters *p,int n,JBDOUBLE t)
{_write_advance(file,p,n,t);}
#endif

#if !INLINE_WRITE_PLUME
void write_plume(FILE *file,Parameters *p,TransportParameters *tp,int n,
	JBDOUBLE t,JBDOUBLE danger,Channel *c)
{_write_plume(file,p,tp,n,t,danger,c);}
#endif

*/
