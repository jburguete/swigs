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

#include "section.h"

#if !INLINE_WAVE_VELOCITY
JBDOUBLE wave_velocity(JBDOUBLE g,JBDOUBLE A,JBDOUBLE B) {return _wave_velocity(g,A,B);}
#endif

#if !INLINE_SECTION_PARAMETERS_PRINT
void section_parameters_print(SectionParameters *sp,FILE *file)
{_section_parameters_print(sp,file);}
#endif

#if !INLINE_FRICTION_PARAMETERS_PRINT
void friction_parameters_print(FrictionParameters *fp,FILE *file)
{_friction_parameters_print(fp,file);}
#endif

#if !INLINE_SECTION_PRINT
void section_print(Section *s,FILE *file)
{_section_print(s,file);}
#endif

#if !INLINE_SECTION_TRAPEZOIDAL_LEVEL
JBDOUBLE section_trapezoidal_level(SectionParameters *sp,JBDOUBLE A)
{return _section_trapezoidal_level(sp,A);}
#endif

#if !INLINE_SECTION_LEVEL
JBDOUBLE section_level(Section *s,JBDOUBLE A) {return _section_level(s,A);}
#endif

#if !INLINE_SECTION_BETA
JBDOUBLE section_beta(Section *s,JBDOUBLE z) {return _section_beta(s,z);}
#endif

#if !INLINE_SECTION_WIDTH
JBDOUBLE section_width(Section *s,JBDOUBLE z) {return _section_width(s,z);}
#endif

#if !INLINE_SECTION_WIDTH_WITH_AREA
JBDOUBLE section_width_with_area(Section *s,JBDOUBLE A)
{return _section_width_with_area(s,A);}
#endif

#if !INLINE_SECTION_PERIMETER
JBDOUBLE section_perimeter(Section *s,JBDOUBLE z) {return _section_perimeter(s,z);}
#endif

#if !INLINE_SECTION_PERIMETER_WITH_AREA
JBDOUBLE section_perimeter_with_area(Section *s,JBDOUBLE A)
{return _section_perimeter_with_area(s,A);}
#endif

#if !INLINE_SECTION_PERIMETER_WITH_DEPTH
JBDOUBLE section_perimeter_with_depth(Section *s,JBDOUBLE h)
{return _section_perimeter_with_depth(s,h);}
#endif

#if !INLINE_SECTION_AREA
JBDOUBLE section_area(Section *s,JBDOUBLE z) {return _section_area(s,z);}
#endif

#if !INLINE_SECTION_AREA_WITH_DEPTH
JBDOUBLE section_area_with_depth(Section *s,JBDOUBLE h)
{return _section_area_with_depth(s,h);}
#endif

#if !INLINE_SECTION_WAVE_VELOCITY
JBDOUBLE section_wave_velocity(Section *s,JBDOUBLE g,JBDOUBLE A)
{return _section_wave_velocity(s,g,A);}
#endif

#if !INLINE_SECTION_CRITICAL_DISCHARGE
JBDOUBLE section_critical_discharge(Section *s,JBDOUBLE A)
{return _section_critical_discharge(s,A);}
#endif

#if !INLINE_SECTION_CRITICAL_FUNCTION
JBDOUBLE section_critical_function(Section *s,JBDOUBLE A)
{return _section_critical_function(s,A);}
#endif

#if !INLINE_SECTION_CRITICAL_AREA
JBDOUBLE section_critical_area(Section *s,JBDOUBLE Q)
{return _section_critical_area(s,Q);}
#endif

#if !INLINE_SECTION_DELETE
void section_delete(Section *s) {_section_delete(s);}
#endif

#if !INLINE_SECTION_CREATE
int section_create(Section *s,int n,int nf) {return _section_create(s,n,nf);}
#endif

#if !INLINE_SECTION_COPY
int section_copy(Section *s,Section *s_copy) {return _section_copy(s,s_copy);}
#endif

#if !INLINE_SECTION_BM
JBDOUBLE section_Bm(TransientSection *cs,JBDOUBLE z,int imax,int *nk)
{return _section_Bm(cs,z,imax,nk);}
#endif

#if !INLINE_SECTION_BL
JBDOUBLE section_Bl(TransientSection *cs,JBDOUBLE z,int imax,int *nk)
{return _section_Bl(cs,z,imax,nk);}
#endif

#if !INLINE_SECTION_BR
JBDOUBLE section_Br(TransientSection *cs,JBDOUBLE z,int imax,int *nk)
{return _section_Br(cs,z,imax,nk);}
#endif

#if !INLINE_SECTION_BC
JBDOUBLE section_Bc(TransientSection *cs,JBDOUBLE z,int imax,int *nk)
{return _section_Bc(cs,z,imax,nk);}
#endif

#if !INLINE_SECTION_B
void section_B(TransientSection *ts,SectionParameters *sp,int nj,int *nk,
	JBDOUBLE (*B)(TransientSection*,JBDOUBLE,int,int*))
{_section_B(ts,sp,nj,nk,B);}
#endif

#if !INLINE_SECTION_P
void section_P(TransientSection *cs,SectionParameters *sp,int n,int *ni)
{_section_P(cs,sp,n,ni);}
#endif

#if !INLINE_SECTION_RHPA
void section_rhpa(FrictionParameters *fp,JBDOUBLE h1,JBDOUBLE h2,JBDOUBLE r1,JBDOUBLE r2)
{_section_rhpa(fp,h1,h2,r1,r2);}
#endif

#if !INLINE_SECTION_RHPB
void section_rhpb(FrictionParameters *fp,JBDOUBLE h,JBDOUBLE r1,JBDOUBLE r2)
{_section_rhpb(fp,h,r1,r2);}
#endif

#if !INLINE_SECTION_RHPC
void section_rhpc(FrictionParameters *fp,JBDOUBLE h,JBDOUBLE r1,JBDOUBLE r2)
{_section_rhpc(fp,h,r1,r2);}
#endif

#if !INLINE_SECTION_RHLA
void section_rhla(FrictionParameters *fp,JBDOUBLE h1,JBDOUBLE h2,JBDOUBLE r1,JBDOUBLE r2,
	FrictionCoefficients *fc)
{_section_rhla(fp,h1,h2,r1,r2,fc);}
#endif

#if !INLINE_SECTION_RHLB
void section_rhlb(FrictionParameters *fp,JBDOUBLE h,JBDOUBLE r1,JBDOUBLE r2,
	FrictionCoefficients *fc)
{_section_rhlb(fp,h,r1,r2,fc);}
#endif

#if !INLINE_SECTION_RHLC
void section_rhlc(FrictionParameters *fp,JBDOUBLE h,JBDOUBLE r1,JBDOUBLE r2,
	FrictionCoefficients *fc)
{_section_rhlc(fp,h,r1,r2,fc);}
#endif

#if !INLINE_SECTION_RHA
void section_rha(FrictionParameters *fp,JBDOUBLE h1,JBDOUBLE h2,JBDOUBLE r1,JBDOUBLE r2,
	FrictionCoefficients *fc)
{_section_rha(fp,h1,h2,r1,r2,fc);}
#endif

#if !INLINE_SECTION_RHB
void section_rhb(FrictionParameters *fp,JBDOUBLE h,JBDOUBLE r1,JBDOUBLE r2,
	FrictionCoefficients *fc)
{_section_rhb(fp,h,r1,r2,fc);}
#endif

#if !INLINE_SECTION_RHC
void section_rhc(FrictionParameters *fp,JBDOUBLE h,JBDOUBLE r1,JBDOUBLE r2,
	FrictionCoefficients *fc)
{_section_rhc(fp,h,r1,r2,fc);}
#endif

#if !INLINE_SECTION_RI
void section_ri(Section *s,TransientSection *ts,SectionParameters *sp,
	FrictionParameters *fp,JBDOUBLE y1,JBDOUBLE y2,int nj,int *nk,int n,
	FrictionCoefficients *fc)
{_section_ri(s,ts,sp,fp,y1,y2,nj,nk,n,fc);}
#endif

#if !INLINE_SECTION_OPEN_PARAMETERS
void section_open_parameters(Section *s) {_section_open_parameters(s);}
#endif

#if !INLINE_SECTION_OPEN
int section_open(Section *s,TransientSection *ts) {return _section_open(s,ts);}
#endif
