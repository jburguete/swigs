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
flow_scheme.h

Header file defining the main functions of the numerical methods to solve the
flow

Author: Javier Burguete Tolosa
Copyright 2005-2014 Javier Burguete Tolosa. All rights reserved
*/

#ifndef FLOW_SCHEME__H
#define FLOW_SCHEME__H 1

#include "parameters.h"
#include "transport_scheme.h"

extern JBDOUBLE tmax;
extern JBFLOAT fcfl,fo;

/*
Pointers to functions selecting the numerical method used to solve the flow
*/

extern void (*diffusion_parameters_implicit)(Parameters*);
extern void (*flow_parameters)(Parameters*);
extern void (*part_decomposition)(int);
extern void (*system_parameters)();
extern void (*system_parameters2)();
extern void (*system_decomposition)();
extern void (*system_simulate_step)();
extern void (*system_simulate_step2)();

/*
void part_parameters(int thread)

Function to obtain the geometrical parameters in a subdomain

Inputs:
	thread: thread number
*/

static inline void _part_parameters(int thread)
{
	int i,i2,j,k;
	Channel *channel;
	Parameters *pv;
	#if DEBUG_PART_PARAMETERS
		if (t>=DEBUG_TIME) fprintf(stderr,"part_parameters: start\n");
	#endif
	i=cell_thread[thread];
	i2=cell_thread[thread+1]-1;
	#if DEBUG_PART_PARAMETERS
		if (t>=DEBUG_TIME) fprintf(stderr,"PP i=%d i2=%d\n",i,i2);
	#endif
	for (j=i, pv=p+i; j<=i2; ++j, ++pv)
	{
		#if DEBUG_PART_PARAMETERS
			if (t>=DEBUG_TIME) fprintf(stderr,"PP node=%d\n",j);
		#endif
		parameters_node(pv);
	}
	channel = p[i].channel;
	do
	{
		j = jbm_min(i2, channel->i2);
		#if DEBUG_PART_PARAMETERS
			if (t>=DEBUG_TIME) fprintf(stderr,"PP i=%d i2=%d j=%d\n",i,i2,j);
		#endif
		for (k=i, pv=p+i; k<j; ++k, ++pv) parameters_cell(pv);
		pv=p+i;
		pv->g = pv->gm;
		parameters_wave_velocity(pv);
		#if DEBUG_PART_PARAMETERS
			if (t>=DEBUG_TIME)
				fprintf(stderr,"PP i=%d g="FWF" A="FWF" B="FWF" c="FWF"\n",
					i,pv->g,pv->A,pv->B,pv->uc);
		#endif
		while (++i<j)
		{
			++pv;
			pv->g = 0.5 * (pv->gm + (pv-1)->gm);
			parameters_wave_velocity(pv);
			#if DEBUG_PART_PARAMETERS
				if (t>=DEBUG_TIME)
					fprintf(stderr,"PP i=%d g="FWF" A="FWF" B="FWF" c="FWF"\n",
						i,pv->g,pv->A,pv->B,pv->uc);
			#endif
		}
		++pv;
		pv->g = (pv-1)->gm;
		parameters_wave_velocity(pv);
		#if DEBUG_PART_PARAMETERS
			if (t>=DEBUG_TIME)
				fprintf(stderr,"PP i=%d g="FWF" A="FWF" B="FWF" c="FWF"\n",
					i,pv->g,pv->A,pv->B,pv->uc);
		#endif
		if (i2 <= channel->i2) break;
		++channel;
		++i;
	}
	while (1);
	#if DEBUG_PART_PARAMETERS
		if (t>=DEBUG_TIME)
		{
			for (i=cell_thread[thread]; i<cell_thread[thread+1]; ++i)
				fprintf(stderr,"PP i=%d g="FWF" c="FWF"\n",i,p[i].g,p[i].uc);
			fprintf(stderr,"part_parameters: end\n");
		}
	#endif
}

#if INLINE_PART_PARAMETERS
	#define part_parameters _part_parameters
#else
	void part_parameters(int);
#endif

/*
void part_parameters_exit(int thread)

Function to obtain the geometrical parameters in a subdomain and closing the
thread in parallel execution

Inputs:
	thread: thread number
*/

static inline void _part_parameters_exit(int thread)
{
	#if DEBUG_PART_PARAMETERS_EXIT
		if (t>=DEBUG_TIME)
			fprintf(stderr,"part_parameters_exit: start\n");
	#endif
	part_parameters(thread);
	#if DEBUG_PART_PARAMETERS_EXIT
		if (t>=DEBUG_TIME)
			fprintf(stderr,"part_parameters_exit: end\n");
	#endif
	g_thread_exit(NULL);
}

#if INLINE_PART_PARAMETERS_EXIT
	#define part_parameters_exit _part_parameters_exit
#else
	void part_parameters_exit(int);
#endif

/*
void system_parameters_parallel()

Function to obtain the geometrical parameters in the domain with parallel
execution
*/

static inline void _system_parameters_parallel()
{
	int i,j;
	GThread *thread[nth];
	Channel *channel;
	Parameters *pv;
	#if DEBUG_SYSTEM_PARAMETERS_PARALLEL
		if (t>=DEBUG_TIME)
			fprintf(stderr,"system_parameters_parallel: start\n");
	#endif
	for (i=0; i<nth; ++i)
		thread[i] = g_thread_new
			(NULL, (void(*))part_parameters_exit, (void*)(size_t)i);
	for (i=0; i<nth; ++i) g_thread_join(thread[i]);
	#if DEBUG_SYSTEM_PARAMETERS_PARALLEL
		if (t>=DEBUG_TIME)
		{
			for (i=0; i<n; ++i)
				fprintf(stderr,"SPP i=%d gm="FWF"\n",i,p[i].gm);
			fprintf(stderr,"SPP correcting parameters\n");
		}
	#endif
	for (i=0; ++i<nth;)
	{
		j=cell_thread[i]-1;
		#if DEBUG_SYSTEM_PARAMETERS_PARALLEL
			if (t>=DEBUG_TIME) fprintf(stderr,"SPP i=%d j=%d\n",i,j);
		#endif
		pv=p+j;
		channel = pv->channel;
		#if DEBUG_SYSTEM_PARAMETERS_PARALLEL
			if (t>=DEBUG_TIME) fprintf(stderr,"SPP i2=%d\n",channel->i2);
		#endif
		if (j < channel->i2)
		{
			parameters_cell(pv);
			if (j > channel->i) pv->g = 0.5 * ((pv-1)->gm + pv->gm);
			else pv->g = pv->gm;
			parameters_wave_velocity(pv);
			if (++j < channel->i2)
			{
				++pv;
				pv->g = 0.5 * ((pv-1)->gm + pv->gm);
				parameters_wave_velocity(pv);
			}
			else
			{
				++pv;
				pv->g = (pv-1)->gm;
				parameters_wave_velocity(pv);
			}
		}
	}
	#if DEBUG_SYSTEM_PARAMETERS_PARALLEL
		if (t>=DEBUG_TIME)
		{
			for (i=0; i<=n; ++i)
			{
				fprintf(stderr,"SPP i=%d g="FWF" c="FWF"\n",i,p[i].g,p[i].uc);
				fprintf(stderr,"SPP A2="FWF"\n",p[i].A2);
			}
			fprintf(stderr,"system_parameters_parallel: end\n");
		}
	#endif
}

#if INLINE_SYSTEM_PARAMETERS_PARALLEL
	#define system_parameters_parallel _system_parameters_parallel
#else
	void system_parameters_parallel();
#endif

/*
void system_parameters_simple()

Function to obtain the geometrical parameters in the domain without parallel
execution
*/

static inline void _system_parameters_simple()
{
	#if DEBUG_SYSTEM_PARAMETERS_SIMPLE
		int i;
		if (t>=DEBUG_TIME)
			fprintf(stderr,"system_parameters_simple: start\n");
	#endif
	part_parameters(0);
	#if DEBUG_SYSTEM_PARAMETERS_SIMPLE
		if (t>=DEBUG_TIME)
		{
			for (i=0; i<=n; ++i)
			{
				fprintf(stderr,"SPS i=%d g="FWF" c="FWF"\n",i,p[i].g,p[i].uc);
				fprintf(stderr,"SPS A2="FWF"\n",p[i].A2);
			}
			fprintf(stderr,"system_parameters_simple: end\n");
		}
	#endif
}

#if INLINE_SYSTEM_PARAMETERS_SIMPLE
	#define system_parameters_simple _system_parameters_simple
#else
	void system_parameters_simple();
#endif

/*
void part_parameters2(int thread)

Function to obtain the flow parameters in a subdomain

Inputs:
	thread: thread number
*/

static inline void _part_parameters2(int thread)
{
	int i,i2,j,l;
	#if TMAX == TMAX_CELL
		JBDOUBLE k;
	#endif
	Channel *channel;
	Parameters *pv;
	TransportParameters *tpv;
	#if DEBUG_PART_PARAMETERS2
		if (t>=DEBUG_TIME) fprintf(stderr,"part_parameters2: start\n");
	#endif
	i=cell_thread[thread];
	i2=cell_thread[thread+1]-1;
	pv=p+i;
	channel = pv->channel;
	#if TMAX == TMAX_CELL
		k = dtmax;
	#endif
	do
	{
		#if DEBUG_PART_PARAMETERS2
			if (t>=DEBUG_TIME)
				fprintf(stderr, "PP2 i=%d A="FWF" Q="FWF" Amin="FWF"\n",
					i, pv->A, pv->Q, pv->Amin);
		#endif
		if (i == channel->i)
		{
			pv->iA = pv->iQ = 0.;
			if (pv->A <= pv->Amin) flow_dry_correction(pv); else flows(pv);
			++i;
			++pv;
		}
		j = jbm_min(i2, channel->i2);
		for (; i<j; ++i, ++pv)
		{
			pv->iA = pv->iQ = 0.;
			if (pv->zs <= pv->zmin) flow_dry_correction(pv); else flows(pv);
			#if TMAX == TMAX_CELL
				k = fmax(k, fmax(pv->lmax, (pv-1)->lmax) / (pv-1)->ix);
				#if DEBUG_PART_PARAMETERS2
					if (t>=DEBUG_TIME) fprintf(stderr,
						"PP2 i=%d lmax="FWF" lmax2="FWF "ix="FWF"\n",
						i, (pv-1)->lmax, pv->lmax, (pv-1)->ix);
				#endif
			#endif
		}
		pv->iA = pv->iQ = 0.;
		if (i == channel->i2)
		{
			if (pv->A <= pv->Amin) flow_dry_correction(pv); else flows(pv);
			#if TMAX == TMAX_CELL
				k = fmax(k, fmax(pv->lmax, (pv-1)->lmax) / (pv-1)->ix);
				#if DEBUG_PART_PARAMETERS2
					if (t>=DEBUG_TIME) fprintf(stderr,
						"PP2 i=%d lmax="FWF" lmax2="FWF "ix="FWF"\n",
						i, (pv-1)->lmax, pv->lmax, (pv-1)->ix);
				#endif
			#endif
			if (i==i2) break;
		}
		else
		{
			if (pv->zs <= pv->zmin) flow_dry_correction(pv); else flows(pv);
			#if TMAX == TMAX_CELL
				k = fmax(k, fmax(pv->lmax, (pv-1)->lmax) / (pv-1)->ix);
				#if DEBUG_PART_PARAMETERS2
					if (t>=DEBUG_TIME) fprintf(stderr,
						"PP2 i=%d lmax="FWF" lmax2="FWF "ix="FWF"\n",
						i, (pv-1)->lmax, pv->lmax, (pv-1)->ix);
				#endif
			#endif
			break;
		}
		++channel;
		++i;
		++pv;
	}
	while (1);
	#if TMAX == TMAX_CELL
		if (k>dtmax)
		{
			g_mutex_lock(mutex);
			dtmax=k;
			g_mutex_unlock(mutex);
		}
	#endif
	i=cell_thread[thread];
	for (l=0; l<=sys->nt; ++l)
		for (j=i, pv=p+j, tpv=tp+j+l*(n+1); j<=i2; ++j, ++pv, ++tpv)
				transport_parameters_node
					(tpv, pv, sys->transport[l].solubility);
	if (!channel_overflow)
	{
		for (pv=p+i; i<=i2; ++i, ++pv) if (pv->A > pv->s->Amax)
		{
			g_mutex_lock(mutex);
			channel_overflow=1;
			g_mutex_unlock(mutex);
			break;
		}
	}
	#if DEBUG_PART_PARAMETERS2
		if (t>=DEBUG_TIME) fprintf(stderr,"part_parameters2: end\n");
	#endif
}

#if INLINE_PART_PARAMETERS2
	#define part_parameters2 _part_parameters2
#else
	void part_parameters2(int);
#endif

/*
void part_parameters2_exit(int thread)

Function to obtain the flow parameters in a subdomain and closing the thread in
parallel execution

Inputs:
	thread: thread number
*/

static inline void _part_parameters2_exit(int thread)
{
	#if DEBUG_PART_PARAMETERS2_EXIT
		if (t>=DEBUG_TIME)
			fprintf(stderr,"part_parameters2_exit: start\n");
	#endif
	part_parameters2(thread);
	#if DEBUG_PART_PARAMETERS2_EXIT
		if (t>=DEBUG_TIME)
			fprintf(stderr,"part_parameters2_exit: end\n");
	#endif
	g_thread_exit(NULL);
}

#if INLINE_PART_PARAMETERS2_EXIT
	#define part_parameters2_exit _part_parameters2_exit
#else
	void part_parameters2_exit(int);
#endif

/*
void boundary_flow_tmax(BoundaryFlow *bf)

Function to obtain the maximum time step allowed by a boundary condition

Inputs:
	bf: pointer to the struct of the boundary condition
*/

static inline void _boundary_flow_tmax(BoundaryFlow *bf)
{
	JBDOUBLE k,AA,QQ,dx;
	Parameters *pv;
	#if DEBUG_BOUNDARY_FLOW_TMAX
		if (t>=DEBUG_TIME)
		{
			fprintf(stderr,"boundary_flow_tmax: start\n");
			fprintf(stderr,"bf tmax="FWL" dtmax="FWL" type=%d i=%d\n",
				tmax,dtmax,bf->type,bf->i);
		}
	#endif
	pv = p + bf->i;
	switch (bf->type)
	{
	case BOUNDARY_FLOW_TYPE_H:
	case BOUNDARY_FLOW_TYPE_HT:
		#if DEBUG_BOUNDARY_FLOW_TMAX
			if (t>=DEBUG_TIME)
				fprintf(stderr,"h="FWL" A="FWL"\n",
					boundary_flow_parameter(bf, tmax),
					section_area_with_depth(pv->s,
						boundary_flow_parameter(bf, tmax)));
		#endif
		AA = section_area_with_depth(pv->s, boundary_flow_parameter(bf, tmax));
		goto area;
	case BOUNDARY_FLOW_TYPE_Z:
	case BOUNDARY_FLOW_TYPE_ZT:
		AA = section_area(pv->s, boundary_flow_parameter(bf, tmax));
		goto area;
	case BOUNDARY_FLOW_TYPE_Q_H:
	case BOUNDARY_FLOW_TYPE_QT_HT:
		QQ = boundary_flow_parameter(bf, tmax);
		AA = section_area_with_depth(pv->s, boundary_flow_parameter2(bf, tmax));
		goto end;
	case BOUNDARY_FLOW_TYPE_Q_Z:
	case BOUNDARY_FLOW_TYPE_QT_ZT:
		QQ = boundary_flow_parameter(bf, tmax);
		AA = section_area(pv->s, boundary_flow_parameter2(bf, tmax));
		goto end;
	case BOUNDARY_FLOW_TYPE_Q:
	case BOUNDARY_FLOW_TYPE_QT:
		QQ = boundary_flow_parameter(bf, tmax);
		goto discharge;
	case BOUNDARY_FLOW_TYPE_QH:
		QQ = boundary_flow_parameter3(bf, pv->h);
		goto discharge;
	case BOUNDARY_FLOW_TYPE_QZ:
		QQ = boundary_flow_parameter3(bf, pv->zs);
		goto discharge;
	case BOUNDARY_FLOW_TYPE_GATE:
		k = boundary_flow_parameter(bf, t);
		QQ = 0.61 / sqrt(1. + 0.61 * k / p->h) * k * GATE_WIDTH(bf) *
			sqrt(2. * JBM_G * p->h);
		k += GATE_HEIGHT(bf);
		if (p->h > k)
		QQ += GATE_WIDTH(bf) * (p->h - k) * sqrt(JBM_G * (p->h - k));
		goto discharge;
	default:
		#if DEBUG_BOUNDARY_FLOW_TMAX
			if (t>=DEBUG_TIME) fprintf(stderr,"boundary_flow_tmax: end\n");
		#endif
		return;
	}
area:
	QQ = fabs(pv->Q) + AA * pv->dx / (tmax - t);
	goto end;
discharge:
	AA = pv->A + fabs(QQ) * (tmax - t) / pv->dx;
end:
	if (AA > pv->Amin)
	{
//		if (type) dx = pv->ix; else dx = pv->dx;
		dx = pv->dx;
		#if DEBUG_BOUNDARY_FLOW_TMAX
			if (t>=DEBUG_TIME)
			{
				//parameters_print(pv,stderr);
				fprintf(stderr,"t="FWL" dtmax="FWL"\ncfl="FWF" dx="FWL"\n",
					t,dtmax,fcfl,dx);
				fprintf(stderr,"AA="FWL" QQ="FWL" w="FWL"\n",
					AA,QQ,section_wave_velocity(pv->s,JBM_G,AA));
			}
		#endif
		dtmax = fmin(dtmax, fcfl * dx /
			(section_wave_velocity(pv->s, JBM_G, AA) + fabs(QQ / AA)));
	}
	#if DEBUG_BOUNDARY_FLOW_TMAX
		if (t>=DEBUG_TIME)
		{
			fprintf(stderr,"t="FWL" dtmax="FWL" A="FWF" Q="FWF"\n",
				t,dtmax,pv->A,pv->Q);
			fprintf(stderr,"boundary_flow_tmax: end\n");
		}
	#endif
}

#if INLINE_BOUNDARY_FLOW_TMAX
	#define boundary_flow_tmax _boundary_flow_tmax
#else
	void boundary_flow_tmax(BoundaryFlow*);
#endif

/*
void system_parameters2_parallel()

Function to obtain the flow parameters in the domain with parallel execution
*/

static inline void _system_parameters2_parallel()
{
	int i,j;
	GThread *thread[nth];
	Channel *channel;
	BoundaryFlow *bf;
	#if TMAX == TMAX_CELL
		Parameters *pv;
	#endif
	#if DEBUG_SYSTEM_PARAMETERS2_PARALLEL
		if (t>=DEBUG_TIME)
			fprintf(stderr,"system_parameters2_parallel: start\n");
	#endif
	dtmax=0.;
	fdtmax=INFINITY;
	for (i=0; i<nth; ++i)
		thread[i] = g_thread_new
			(NULL, (void(*))part_parameters2_exit, (void*)(size_t)i);
	for (i=0; i<nth; ++i) g_thread_join(thread[i]);
	#if TMAX == TMAX_CELL
		for (i=0; ++i<nth;)
		{
			j=cell_thread[i]-1;
			pv=p+j;
			if (j < pv->channel->i2)
				k = fmax(k, fmax(pv->lmax, (pv+1)->lmax) / pv->ix);
		}
	#endif
	if (dtmax>0.) dtmax=fcfl/dtmax; else dtmax=INFINITY;
	dtmax=fmin(dtmax,FRICTION_CFL*fdtmax);
	tmax = t + dtmax;
	#if DEBUG_SYSTEM_PARAMETERS2_PARALLEL
		if (t>=DEBUG_TIME)
			fprintf(stderr,"SP2P tmax="FWL" dtmax="FWL"\n",tmax,dtmax);
	#endif
	for (i=0, channel=sys->channel; i<=sys->n; ++i, ++channel)
		for (j=0, bf=channel->bf; j<=channel->n; ++j, ++bf)
			boundary_flow_tmax(bf);
	tmax = t + dtmax;
	#if DEBUG_SYSTEM_PARAMETERS2_PARALLEL
		if (t>=DEBUG_TIME)
			fprintf(stderr,"SP2P tmax="FWL" dtmax="FWL"\n",tmax,dtmax);
	#endif
	for (i=0, channel=sys->channel; i<=sys->n; ++i, ++channel)
	{
		list_section_changes_tmax(channel->list_section_changes);
		list_section_gauge_tmax(channel->list_section_gauge);
	}
	dtmax = tmax - t;
	#if DEBUG_SYSTEM_PARAMETERS2_PARALLEL
		if (t>=DEBUG_TIME)
		{
			fprintf(stderr,"SP2P tmax="FWL" dtmax="FWL"\n",tmax,dtmax);
			for (i=0; i<=n; ++i)
				fprintf(stderr,"SP2P i=%d F="FWF" la="FWF" lb="FWF"\n",
					i,p[i].F,p[i].la,p[i].lb);
			fprintf(stderr,"system_parameters2_parallel: end\n");
		}
	#endif
}

#if INLINE_SYSTEM_PARAMETERS2_PARALLEL
	#define system_parameters2_parallel _system_parameters2_parallel
#else
	void system_parameters2_parallel();
#endif

/*
void system_parameters2_simple()

Function to obtain the flow parameters in the domain without parallel execution
*/

static inline void _system_parameters2_simple()
{
	int i,j;
	Channel *channel;
	BoundaryFlow *bf;
	#if DEBUG_SYSTEM_PARAMETERS2_SIMPLE
		if (t>=DEBUG_TIME) fprintf(stderr,"system_parameters2_simple: start\n");
	#endif
	dtmax=0.;
	fdtmax=INFINITY;
	part_parameters2(0);
	if (dtmax>0.) dtmax=fcfl/dtmax; else dtmax=INFINITY;
	dtmax=fmin(dtmax,FRICTION_CFL*fdtmax);
	tmax = t + dtmax;
	#if DEBUG_SYSTEM_PARAMETERS2_SIMPLE
		if (t>=DEBUG_TIME)
			fprintf(stderr,"SP2S tmax="FWL" dtmax="FWL"\n",tmax,dtmax);
	#endif
	for (i=0, channel=sys->channel; i<=sys->n; ++i, ++channel)
		for (j=0, bf=channel->bf; j<=channel->n; ++j, ++bf)
			boundary_flow_tmax(bf);
	tmax = t + dtmax;
	#if DEBUG_SYSTEM_PARAMETERS2_SIMPLE
		if (t>=DEBUG_TIME)
			fprintf(stderr,"SP2S tmax="FWL" dtmax="FWL"\n",tmax,dtmax);
	#endif
	for (i=0, channel=sys->channel; i<=sys->n; ++i, ++channel)
	{
		list_section_changes_tmax(channel->list_section_changes);
		list_section_gauge_tmax(channel->list_section_gauge);
	}
	dtmax = tmax - t;
	#if DEBUG_SYSTEM_PARAMETERS2_SIMPLE
		if (t>=DEBUG_TIME)
		{
			fprintf(stderr,"SP2S tmax="FWL" dtmax="FWL"\n",tmax,dtmax);
			for (i=0; i<=n; ++i)
				fprintf(stderr,"SP2S i=%d F="FWF" la="FWF" lb="FWF"\n",
					i,p[i].F,p[i].la,p[i].lb);
			fprintf(stderr,"system_parameters2_simple: end\n");
		}
	#endif
}

#if INLINE_SYSTEM_PARAMETERS2_SIMPLE
	#define system_parameters2_simple _system_parameters2_simple
#else
	void system_parameters2_simple();
#endif

/*
void flow_linearization(Parameters *p)

Function to obtain the average parameters of a cell

Input:
	p: pointer to the struct of the cell
*/

static inline void _flow_linearization(Parameters *p)
{
	register Parameters *p2=p+1;
#if FLOW_LINEARIZATION == FLOW_LINEARIZATION_ROE
	register JBDOUBLE k1,k2;
	#if DEBUG_FLOW_LINEARIZATION
		if (t>=DEBUG_TIME)
		{
			fprintf(stderr,"flow_linearization: start\n");
			fprintf(stderr,"FL A2="FWF" sA2="FWF"\n",p->A2,p->sA2);
		}
	#endif
	k1 = p->sA / p->sA2;
	k2 = p2->sA / p->sA2;
	p->LA = k1 * p->la + k2 * p2->la;
	p->LB = k1 * p->lb + k2 * p2->lb;
	p->U = k1 * p->u + k2 * p2->u;
#else
	p->LA = 0.5 * (p->la + p2->la);
	p->LB = 0.5 * (p->lb + p2->lb);
	p->U = 0.5 * (p->u + p2->u);
#endif
	p->C2 = p->LA - p->LB;
	#if DEBUG_FLOW_LINEARIZATION
		if (t>=DEBUG_TIME)
		{
			fprintf(stderr,"FL LA="FWF" LB="FWF" C2="FWF"\n",p->LA,p->LB,p->C2);
			fprintf(stderr,"flow_linearization: end\n");
		}
	#endif
}

#if INLINE_FLOW_LINEARIZATION
	#define flow_linearization _flow_linearization
#else
	void flow_linearization(Parameters*);
#endif

static inline void _variables_actualize(Variables *v)
{
	int i,j;
	register JBFLOAT *vp;
	Parameters *pv;
	TransportParameters *tpv;
	#if DEBUG_VARIABLES_ACTUALIZE
		if (t>=DEBUG_TIME) fprintf(stderr,"variables_actualize: start\n");
	#endif
	if (!p) return;
	#if DEBUG_VARIABLES_ACTUALIZE
		if (t>=DEBUG_TIME) fprintf(stderr,"VA: creating\n");
	#endif
	variables_create(v, n, &vsize, sys->channel->nt);
	#if DEBUG_VARIABLES_ACTUALIZE
		if (t>=DEBUG_TIME) fprintf(stderr,"VA: calculating\n");
	#endif
	for (i=0, pv=p; i<=n; ++i, ++pv)
	{
		v->x[i] = pv->x;
		v->B[i] = pv->B;
		v->zs[i] = pv->zs;
		v->zb[i] = pv->s->sp->z;
		v->zmax[i] = pv->s->zmax;
		v->A[i] = pv->A;
		v->Q[i] = pv->Q;
		v->beta[i] = pv->beta;
		#if DEBUG_VARIABLES_ACTUALIZE
			if (t>=DEBUG_TIME)
				fprintf(stderr,"VA i=%d Q="FWF" A="FWF"\n",i,v->Q[i],v->A[i]);
		#endif
	}
	for (j=(sys->channel->nt+1)*(n+1), vp=v->p, tpv=tp; --j>=0; ++vp, ++tpv)
		*vp = tpv->c;
#if INTERFACE != INTERFACE_SCIENTIFIC
	int k,l;
	register JBDOUBLE dx,dQx;
	Channel *channel;
	#if DEBUG_VARIABLES_ACTUALIZE
		if (t>=DEBUG_TIME) fprintf(stderr,"VA: parameters\n");
	#endif
	for (i=n+1, pv=p; --i>=0; ++pv) pv->nw = 0;
	for (l=0, pv=p, channel=sys->channel; l<=sys->n; ++l, ++channel)
	{
		for (i=channel->i; i<channel->i2; ++i)
		{
			if (fmax((pv+1)->zs,pv->zs) <= pv->zsm)
			{
				pv->nw++;
				(++pv)->nw++;
			}
			else if ((pv+1)->la * pv->la < 0. || (pv+1)->lb * pv->lb < 0.)
			{
				 pv->nw += 8;
				 (++pv)->nw += 8;
			}
			else
			{
				if (pv->LB>=0.) (++pv)->nw += 2;
				else if (pv->LA<=0.) (pv++)->nw += 2;
				else
				{
					pv->nw++;
					(++pv)->nw++;
				}
			}
		}
		i=channel->i;
		pv = p + channel->i;
		v->Q[i] = pv->Q;
		for (i=channel->i; ++i<channel->i2;)
		{
			++pv;
			for (j=0; (pv+j)->nw!=2 && i+j<channel->i2; ++j);
			if (j)
			{
				dx = (pv+j)->x - (pv-1)->x;
				dQx = ((pv+j)->Q - (pv-1)->Q) / dx;
				for (k=0; k<j; ++k, ++i)
				{
					dx = (pv+k)->x - (pv-1)->x;
					v->Q[i] = (pv-1)->Q + dQx * dx;
				}
				pv+=j-1;
				--i;
			}
			else v->Q[i] = pv->Q;
			#if DEBUG_VARIABLES_ACTUALIZE
				if (t>=DEBUG_TIME)
					fprintf(stderr,"VA i=%d Q="FWF"\n",i,v->Q[i]);
			#endif
		}
		++pv;
		v->Q[i] = pv->Q;
	}
#endif
	#if DEBUG_VARIABLES_ACTUALIZE
		if (t>=DEBUG_TIME) fprintf(stderr,"variables_actualize: end\n");
	#endif
}

#if INLINE_VARIABLES_ACTUALIZE
	#define variables_actualize _variables_actualize
#else
	void variables_actualize(Variables*);
#endif

static inline void _flow(Parameters *p)
{
	#if DEBUG_FLOW
		if (t>=DEBUG_TIME) fprintf(stderr,"flow: start\n");
	#endif
	#if (MODEL_PRESSURE == MODEL_PRESSURE_STRONG_SLOPE) || \
		(MODEL_PRESSURE == MODEL_PRESSURE_HYDROSTATIC)
		#if FRICTION_EXPLICIT == FRICTION_EXPLICIT_POINTWISE
			p->dF = (p+1)->F - p->F + 0.5 * p->gm * p->A2 * p->dzs;
		#elif FRICTION_EXPLICIT == FRICTION_EXPLICIT_UPWIND
			#if FRICTION_LIMITATION
				register JBDOUBLE k,k2;
				p->dF = (p+1)->F - p->F + 0.5 * p->gm * p->A2 * p->dzs;
				k = 0.5 * ((p+1)->Q + p->Q) * p->ix / dt - p->dF;
				k2 = 0.5 * p->ix * ((p+1)->Ff + p->Ff);
				if (k<0.) k=fmax(k,k2); else k=fmin(k,k2);
				p->dF += k;
			#else
				p->dF = (p+1)->F - p->F + 0.5 * (p->gm * p->A2 * p->dzs +
					p->ix * ((p+1)->Ff + p->Ff));
			#endif
		#endif
	#else
		#if FRICTION_EXPLICIT == FRICTION_EXPLICIT_POINTWISE
			p->dF = (p+1)->F - p->F + 0.5 * (p->gm * p->A2 * p->dzs +
				((p+1)->I + p->I) * p->dg);
		#elif FRICTION_EXPLICIT == FRICTION_EXPLICIT_UPWIND
			#if FRICTION_LIMITATION
				register JBDOUBLE k,k2;
				p->dF = (p+1)->F - p->F + 0.5 * (p->gm * p->A2 * p->dzs +
					((p+1)->I + p->I) * p->dg);
				k = 0.5 * ((p+1)->Q + p->Q) * p->ix / dt - p->dF;
				k2 = 0.5 * p->ix * ((p+1)->Ff + p->Ff);
				if (k<0.) k=fmax(k,k2); else k=fmin(k,k2);
				p->dF += k;
			#else
				p->dF = (p+1)->F - p->F + 0.5 * (p->gm * p->A2 * p->dzs +
					((p+1)->I + p->I) * p->dg + p->ix * ((p+1)->Ff + p->Ff)));
			#endif
		#endif
	#endif
	#if DEBUG_FLOW
		if (t>=DEBUG_TIME)
		{
			fprintf(stderr,"F F1="FWF" F2="FWF"\nF g="FWF" A2="FWF" dzs="FWF
				"\nF ix="FWF" R1="FWF" R2="FWF"\n",
				p->F,(p+1)->F,p->gm,p->A2,p->dzs,p->ix,p->Ff,(p+1)->Ff);
			fprintf(stderr,"F dF="FWF"\n",p->dF);
			fprintf(stderr,"flow: end\n");
		}
	#endif
}

#if INLINE_FLOW
	#define flow _flow
#else
	void flow(Parameters*);
#endif

static inline void _flow_wave_upwind(Parameters *p)
{
	#if DEBUG_FLOW_WAVE_UPWIND
		if (t>=DEBUG_TIME)
		{
			fprintf(stderr,"flow_wave_upwind: start\n");
			fprintf(stderr,
				"FWU dQ="FWF" dF="FWF"\nFWU C2="FWF" LA="FWF" LB="FWF"\n",
				p->dQ,p->dF,p->C2,p->LA,p->LB);
		}
	#endif
#if FLUX_LIMITER_DISCRETIZATION == FLUX_LIMITER_DISCRETIZATION_CONSERVATIVE
	if (p->LB>=0.)
	{
		p->dQm = p->dFm = 0.;
		p->dQp = p->dQ;
		p->dFp = p->dF;
	}
	else if (p->LA<=0.)
	{
		p->dQp = p->dFp = 0.;
		p->dQm = p->dQ;
		p->dFm = p->dF;
	}
	else
	{
		p->dQp = (p->dF - p->LB * p->dQ) / p->C2;
		p->dFp = p->LA * p->dQp; 
		p->dQm = (p->LA * p->dQ - p->dF) / p->C2;
		p->dFm = p->LB * p->dQm;
	}
#else
	register JBDOUBLE dWA,dWB;
	dWA = (p->dF - p->LB * p->dQ) / p->C2;
	dWB = (p->LA * p->dQ - p->dF) / p->C2;
	if (p->LB>=0.)
	{
		p->dQm = p->dFm = p->dWAm = p->dWBm = 0.;
		p->dWAp = dWA;
		p->dWBp = dWB;
		p->dQp = p->dQ;
		p->dFp = p->dF;
	}
	else if (p->LA<=0.)
	{
		p->dQp = p->dFp = p->dWAp = p->dWBp = 0.;
		p->dWAm = dWA;
		p->dWBm = dWB;
		p->dQm = p->dQ;
		p->dFm = p->dF;
	}
	else
	{
		p->dWAm = p->dWBp = 0.;
		p->dQp = p->dWAp = dWA;
		p->dFp = p->LA * dWA; 
		p->dQm = p->dWBm = dWB;
		p->dFm = p->LB * dWB;
	}
	#if DEBUG_FLOW_WAVE_UPWIND
		if (t>=DEBUG_TIME)
			fprintf(stderr,
				"FWU dWAp="FWF" dWAm="FWF"\nFWU dWBp="FWF" dWBm="FWF"\n",
				p->dWAp,p->dWAm,p->dWBp,p->dWBm);
	#endif
#endif
	#if DEBUG_FLOW_WAVE_UPWIND
		if (t>=DEBUG_TIME)
		{
			fprintf(stderr,
				"FWU dQp="FWF" dQm="FWF"\nFWU dFp="FWF" dFm="FWF"\n",
				p->dQp,p->dQm,p->dFp,p->dFm);
			fprintf(stderr,"flow_wave_upwind: end\n");
		}
	#endif
}

#if INLINE_FLOW_WAVE_UPWIND
	#define flow_wave_upwind _flow_wave_upwind
#else
	void flow_wave_upwind(Parameters*);
#endif

static inline void _flow_wave_upwind2(Parameters *p)
{
	register JBDOUBLE k,k2;
	#if DEBUG_FLOW_WAVE_UPWIND2
		if (t>=DEBUG_TIME) fprintf(stderr,"flow_wave_upwind2: start\n");
	#endif
	k = dt / p->ix;
#if FLUX_LIMITER_DISCRETIZATION == FLUX_LIMITER_DISCRETIZATION_CONSERVATIVE
	p->dQQp = dt2 * (p->dQp - k * p->dFp);
	p->dQQm = dt2 * (p->dQm + k * p->dFm);
	k2 = k * p->LA * p->LB;
	k *= p->LA + p->LB;
	p->dFFp = dt2 * ((1. - k) * p->dFp + k2 * p->dQp);
	p->dFFm = dt2 * ((1. + k) * p->dFm - k2 * p->dQm);
	#if DEBUG_FLOW_WAVE_UPWIND2
		if (t>=DEBUG_TIME)
			fprintf(stderr,"dQQp="FWF" dQQm="FWF"\ndFFp="FWF" dFFm="FWF"\n",
				p->dQQp,p->dQQm,p->dFFp,p->dFFm);
	#endif
#else
	k2 = k * p->LA;
	p->dWWAp = dt2 * (1. - k2) * p->dWAp;
	p->dWWAm = dt2 * (1. + k2) * p->dWAm;
	k *= p->LB;
	p->dWWBp = dt2 * (1. - k) * p->dWBp;
	p->dWWBm = dt2 * (1. + k) * p->dWBm;
	#if DEBUG_FLOW_WAVE_UPWIND2
		if (t>=DEBUG_TIME)
		{
			fprintf(stderr,
				"FWU2 dWWAp="FWF" dWWAm="FWF"\nFWU2 dWWBp="FWF" dWWBm="FWF"\n",
				p->dWWAp,p->dWWAm,p->dWWBp,p->dWWBm);
			fprintf(stderr,"flow_wave_upwind2: end\n");
		}
	#endif
#endif
}

#if INLINE_FLOW_WAVE_UPWIND2
	#define flow_wave_upwind2 _flow_wave_upwind2
#else
	void flow_wave_upwind2(Parameters*);
#endif

static inline JBDOUBLE _flow_entropy_correction(JBDOUBLE l1,JBDOUBLE l2,JBDOUBLE lm)
{
	register JBDOUBLE k;
	#if DEBUG_FLOW_ENTROPY_CORRECTION
		if (t>=DEBUG_TIME) fprintf(stderr,"flow_entropy_correction: start\n");
	#endif
	#if FLOW_ENTROPY_CORRECTION == FLOW_ENTROPY_CORRECTION_HARTEN
		register JBDOUBLE km,kp;
		if (lm>0.) kp=lm, km=0.; else kp=0., km=lm;
		k=fmax(l2-kp,km-l1);
	#elif FLOW_ENTROPY_CORRECTION == FLOW_ENTROPY_CORRECTION_MEAN_MAX
		register JBDOUBLE km,kp;
		if (lm>0.) kp=lm, km=0.; else kp=0., km=lm;
		k=fmax(0.5*l2-kp,km-0.5*l1);
	#elif FLOW_ENTROPY_CORRECTION == FLOW_ENTROPY_CORRECTION_MEAN_MEAN
		k=0.25*(l2-l1-fabs(lm+lm));
	#elif FLOW_ENTROPY_CORRECTION == FLOW_ENTROPY_CORRECTION_NULL
		k=0.;
	#endif
	#if DEBUG_FLOW_ENTROPY_CORRECTION
		if (t>=DEBUG_TIME)
		{
			fprintf(stderr,"FEC k="FWL"\n",k);
			fprintf(stderr,"flow_entropy_correction: end\n");
		}
	#endif
	return k;
}

#if INLINE_FLOW_ENTROPY_CORRECTION
	#define flow_entropy_correction _flow_entropy_correction
#else
	JBDOUBLE flow_entropy_correction(JBDOUBLE,JBDOUBLE,JBDOUBLE);
#endif

#if FLOW_ENTROPY == FLOW_ENTROPY_SCALAR

static inline void _flow_entropy_wave
	(Parameters *p,JBDOUBLE k2,JBDOUBLE dA,JBDOUBLE dQ)
{
	register JBDOUBLE k;
	#if DEBUG_FLOW_ENTROPY_WAVE
		if (t>=DEBUG_TIME)
		{
			fprintf(stderr,"flow_entropy_wave: start\n");
			fprintf(stderr,"FEW k1="FWL" k2="FWL"\n",k2*dA,k2*dQ);
		}
	#endif
	k=k2*dA;
	p->dQp += k;
	p->dQm -= k;
	k=k2*dQ;
	p->dFp += k;
	p->dFm -= k;
	#if DEBUG_FLOW_ENTROPY_WAVE
		if (t>=DEBUG_TIME) fprintf(stderr,"flow_entropy_wave: end\n");
	#endif
}

#elif FLOW_ENTROPY == FLOW_ENTROPY_CHARACTERISTIC

static inline void _flow_entropy_wave
	(Parameters *p,JBDOUBLE k2,JBDOUBLE dW,JBDOUBLE L)
{
	register JBDOUBLE k;
	#if DEBUG_FLOW_ENTROPY_WAVE
		if (t>=DEBUG_TIME)
		{
			fprintf(stderr,"flow_entropy_wave: start\n");
			fprintf(stderr,"FEW k1="FWL" k2="FWL"\n",k2*dW,k2*dW*L);
		}
	#endif
	k=k2*dW;
	p->dQp += k;
	p->dQm -= k;
	k*=L;
	p->dFp += k;
	p->dFm -= k;
	#if DEBUG_FLOW_ENTROPY_WAVE
		if (t>=DEBUG_TIME) fprintf(stderr,"flow_entropy_wave: end\n");
	#endif
}

#endif

#if INLINE_FLOW_ENTROPY_WAVE
	#define flow_entropy_wave _flow_entropy_wave
#else
	void flow_entropy_wave(Parameters*,JBDOUBLE,JBDOUBLE,JBDOUBLE);
#endif

static inline void _flow_entropy(Parameters *p)
{
	JBDOUBLE dA;
	#if DEBUG_FLOW_ENTROPY
		if (t>=DEBUG_TIME) fprintf(stderr,"flow_entropy: start\n");
	#endif
	if (p->la<0. && (p+1)->la>0.)
	{
		dA = (p+1)->A - p->A;
		#if FLOW_ENTROPY == FLOW_ENTROPY_SCALAR
			flow_entropy_wave
				(p,flow_entropy_correction(p->la,(p+1)->la,p->LA),dA,p->dQ);
		#else
			flow_entropy_wave(p, flow_entropy_correction(p->la,(p+1)->la,p->LA),
				p->dQ - p->LB * dA, p->LA);
		#endif
		#if DEBUG_FLOW_ENTROPY
			if (t>=DEBUG_TIME)
				fprintf(stderr,
					"FE la="FWF" la2="FWF"\nFE dQp="FWF" dQm="FWF"\n"
					"FE dFp="FWF" dFm="FWF"\nLA="FWF" LB="FWF"\n"
					"FE dA="FWF" dQ="FWF"\n",
					p->la,(p+1)->la,p->dQp,p->dQm,p->dFp,p->dFm,
					p->LA,p->LB,dA,p->dQ);
		#endif
	}
	if (p->lb<0. && (p+1)->lb>0.)
	{
		dA = (p+1)->A - p->A;
		#if FLOW_ENTROPY == FLOW_ENTROPY_SCALAR
			flow_entropy_wave
				(p,flow_entropy_correction(p->lb,(p+1)->lb,p->LB),dA,p->dQ);
		#else
			flow_entropy_wave(p, flow_entropy_correction(p->lb,(p+1)->lb,p->LB),
				p->LA * dA - p->dQ, p->LB);
		#endif
		#if DEBUG_FLOW_ENTROPY
			if (t>=DEBUG_TIME)
				fprintf(stderr,
					"FE lb="FWF" lb2="FWF"\nFE dQp="FWF" dQm="FWF"\n"
					"FE dFp="FWF" dFm="FWF"\nLA="FWF" LB="FWF"\n"
					"FE dA="FWF" dQ="FWF"\n",
					p->lb,(p+1)->lb,p->dQp,p->dQm,p->dFp,p->dFm,
					p->LA,p->LB,dA,p->dQ);
		#endif
	}
	#if DEBUG_FLOW_ENTROPY
		if (t>=DEBUG_TIME) fprintf(stderr,"flow_entropy: end\n");
	#endif
}

#if INLINE_FLOW_ENTROPY
	#define flow_entropy _flow_entropy
#else
	void flow_entropy(Parameters*);
#endif

static inline void _flow_parameters_upwind(Parameters *p)
{
	#if DEBUG_FLOW_PARAMETERS_UPWIND
		if (t>=DEBUG_TIME) fprintf(stderr,"flow_parameters_upwind: start\n");
	#endif
	p->dQ = (p+1)->Q - p->Q;
	if (fmax((p+1)->zs,p->zs) <= p->zsm)
	{
		#if FLUX_LIMITER_DISCRETIZATION == \
			FLUX_LIMITER_DISCRETIZATION_CHARACTERISTIC
			p->LA = 0.5 * (p->la + (p+1)->la);
			p->LB = 0.5 * (p->lb + (p+1)->lb);
			p->dWAp = p->dWAm = p->dWBp = p->dWBm =
		#endif
		p->U = p->dFp = p->dFm = 0.;
		if (p->zs > p->zmin) p->dQm = p->dQ, p->dQp = 0.;
		else p->dQp = p->dQ, p->dQm = 0.;
	}
	else
	{
		flow_linearization(p);
		flow(p);
		flow_wave_upwind(p);
		flow_entropy(p);
	}
	#if DEBUG_FLOW_PARAMETERS_UPWIND
		if (t>=DEBUG_TIME)
		{
			fprintf(stderr,"FPU dQp="FWF" dQm="FWF" dFp="FWF" dFm="FWF"\n",
				p->dQp,p->dQm,p->dFp,p->dFm);
			fprintf(stderr,"flow_parameters_upwind: end\n");
		}
	#endif
}

#if INLINE_FLOW_PARAMETERS_UPWIND
	#define flow_parameters_upwind _flow_parameters_upwind
#else
	void flow_parameters_upwind(Parameters*);
#endif

static inline void _flow_parameters_upwind2(Parameters *p)
{
	#if DEBUG_FLOW_PARAMETERS_UPWIND2
		if (t>=DEBUG_TIME) fprintf(stderr,"flow_parameters_upwind2: start\n");
	#endif
	p->dQ = (p+1)->Q - p->Q;
	if (fmax((p+1)->zs,p->zs) <= p->zsm)
	{
		#if FLUX_LIMITER_DISCRETIZATION == \
			FLUX_LIMITER_DISCRETIZATION_CHARACTERISTIC
			p->LA = 0.5 * (p->la + (p+1)->la);
			p->LB = 0.5 * (p->lb + (p+1)->lb);
			p->dWAp = p->dWAm = p->dWBp = p->dWBm =
			p->dWWAp = p->dWWAm = p->dWWBp = p->dWWBm =
		#endif
		p->U = p->dFp = p->dFm = p->dFFp = p->dFFm = p->dQQp = p->dQQm = 0.;
		if (p->zs > p->zmin) p->dQm = p->dQ, p->dQp = 0.;
		else p->dQp = p->dQ, p->dQm = 0.;
	}
	else
	{
		flow_linearization(p);
		flow(p);
		flow_wave_upwind(p);
		flow_wave_upwind2(p);
		flow_entropy(p);
	}
	#if DEBUG_FLOW_PARAMETERS_UPWIND2
		if (t>=DEBUG_TIME)
		{
			fprintf(stderr,"FPU2 dQp="FWF" dQm="FWF" dFp="FWF" dFm="FWF"\n",
				p->dQp,p->dQm,p->dFp,p->dFm);
			#if FLUX_LIMITER_DISCRETIZATION == \
				FLUX_LIMITER_DISCRETIZATION_CONSERVATIVE
				fprintf(stderr,
					"FPU2 dQQp="FWF" dQQm="FWF" dFFp="FWF" dFFm="FWF"\n",
					p->dQQp,p->dQQm,p->dFFp,p->dFFm);
			#else
				fprintf(stderr,
					"FPU2 dWWAp="FWF" dWWAm="FWF" dWWAp="FWF" dWWBm="FWF"\n",
					p->dWWAp,p->dWWAm,p->dWWBp,p->dWWBm);
			#endif
			fprintf(stderr,"flow_parameters_upwind2: end\n");
		}
	#endif
}

#if INLINE_FLOW_PARAMETERS_UPWIND2
	#define flow_parameters_upwind2 _flow_parameters_upwind2
#else
	void flow_parameters_upwind2(Parameters*);
#endif

static inline void _flow_inner_boundary(BoundaryFlow *bf)
{
	register JBDOUBLE k,k2;
	register Parameters *pv;
	#if DEBUG_FLOW_INNER_BOUNDARY
		if (t>=DEBUG_TIME)
		{
			fprintf(stderr,"flow_inner_boundary: end\n");
			fprintf(stderr,"FIB i=%d type=%d\n",bf->i,bf->type);
		}
	#endif
	pv = p + bf->i;
	switch (bf->type)
	{
	case BOUNDARY_FLOW_TYPE_DAM:
	case BOUNDARY_FLOW_TYPE_PIPE:
		k = fmin(pv->V, boundary_flow_integral(bf, t, tmax));
		pv->V -= k;
		p[bf->i2].V += k;
		goto exit_1;
	case BOUNDARY_FLOW_TYPE_Q:
	case BOUNDARY_FLOW_TYPE_QT:
		k=boundary_flow_integral(bf, t, tmax);
		break;
	case BOUNDARY_FLOW_TYPE_QH:
		k = boundary_flow_parameter3(bf, pv->h) * dt;
		break;
	case BOUNDARY_FLOW_TYPE_QZ:
		k = boundary_flow_parameter3(bf, pv->zs) * dt;
		break;
	default:
		goto exit_1;
	}
	k = fmax(k, -pv->V);
	#if DEBUG_FLOW_INNER_BOUNDARY
		if (t>=DEBUG_TIME)
			fprintf(stderr,"FIB Qm="FWL" t="FWL" tmax="FWL" dt="FWL"\n",
				k,t,tmax,dt);
	#endif
	m->inner += k;
	bf->contribution = k / dt;
	k/=dt;
	if (pv->uc == 0.)
	{
		k*=0.5;
		(pv-1)->dQp -= k;
		pv->dQm -= k;
	}
	else if (pv->lb > 0.) (pv-1)->dQp -= k;
	else if (pv->la < 0.) pv->dQm -= k;
	else
	{
		k /= pv->la - pv->lb;
		(pv-1)->dQp += k2 = k * pv->lb;
		(pv-1)->dFp += k2 * pv->la;
		pv->dQm -= k2 = k * pv->la;
		pv->dFm -= k2 * pv->lb;
	}
exit_1:
	#if DEBUG_FLOW_INNER_BOUNDARY
		if (t>=DEBUG_TIME)
		{
			fprintf(stderr,"FIB inner="FWL" dQp="FWF" dQm="FWF"\nla "
				FWF" lb="FWF" c="FWF"\n",
				m->inner,(pv-1)->dQp,pv->dQm,pv->la,pv->lb,pv->uc);
			fprintf(stderr,"flow_inner_boundary: end\n");
		}
	#endif
	return;
}

#if INLINE_FLOW_INNER_BOUNDARY
	#define flow_inner_boundary _flow_inner_boundary
#else
	void flow_inner_boundary(BoundaryFlow*);
#endif

static inline void _flow_wave_decomposition_upwind(Parameters *p)
{
	#if DEBUG_FLOW_WAVE_DECOMPOSITION_UPWIND
		if (t>=DEBUG_TIME)
			fprintf(stderr,"flow_wave_decomposition_upwind: start\n");
	#endif
	p->iA -= dt * p->dQm;
	p->iQ -= dt * p->dFm;
	(p+1)->iA -= dt * p->dQp;
	(p+1)->iQ -= dt * p->dFp;
	#if DEBUG_FLOW_WAVE_DECOMPOSITION_UPWIND
		if (t>=DEBUG_TIME)
		{
			fprintf(stderr,"FWD iA="FWF" iQ="FWF"\n",p->iA,p->iQ);
			fprintf(stderr,"FWD iA2="FWF" iQ2="FWF"\n",(p+1)->iA,(p+1)->iQ);
			fprintf(stderr,"flow_wave_decomposition_upwind: end\n");
		}
	#endif
}

#if INLINE_FLOW_WAVE_DECOMPOSITION_UPWIND
	#define flow_wave_decomposition_upwind _flow_wave_decomposition_upwind
#else
	void flow_wave_decomposition_upwind(Parameters*);
#endif

#if FLOW_WAVE_DECOMPOSITION == FLOW_WAVE_DECOMPOSITION_UPWIND

	#define _flow_wave_decomposition(p) ()

#elif FLOW_WAVE_DECOMPOSITION == FLOW_WAVE_DECOMPOSITION_TVD

static inline void _flow_wave_decomposition(Parameters *p)
{
	register JBDOUBLE k;
	#if DEBUG_FLOW_WAVE_DECOMPOSITION
		if (t>=DEBUG_TIME)
			fprintf(stderr,"flow_wave_decomposition: start\n");
	#endif
#if FLUX_LIMITER_DISCRETIZATION == FLUX_LIMITER_DISCRETIZATION_CONSERVATIVE
	k = dt2 * p->dQm * flow_flux_limiter((p-1)->dQm, p->dQm);
	p->iA -= k;
	(p-1)->iA += k;
	k = dt2 *p->dFm * flow_flux_limiter((p-1)->dFm, p->dFm);
	p->iQ -= k;
	(p-1)->iQ += k;
	k = dt2 *p->dQp * flow_flux_limiter((p+1)->dQp, p->dQp);
	(p+1)->iA -= k;
	(p+2)->iA += k;
	k = dt2 *p->dFp * flow_flux_limiter((p+1)->dFp, p->dFp);
	(p+1)->iQ -= k;
	(p+2)->iQ += k;
#else
	p->dQQm = k = dt2 * p->dWAm * flow_flux_limiter((p-1)->dWAm, p->dWAm);
	p->iA -= k;
	(p-1)->iA += k;
	k *= p->LA;
	p->iQ -= k;
	(p-1)->iQ += k;
	p->dQQm += k = dt2 * p->dWBm * flow_flux_limiter((p-1)->dWBm, p->dWBm);
	p->iA -= k;
	(p-1)->iA += k;
	k *= p->LB;
	p->iQ -= k;
	(p-1)->iQ += k;
	p->dQQp = k = dt2 * p->dWAp * flow_flux_limiter((p+1)->dWAp, p->dWAp);
	(p+1)->iA -= k;
	(p+2)->iA += k;
	k *= p->LA;
	(p+1)->iQ -= k;
	(p+2)->iQ += k;
	p->dQQp += k = dt2 * p->dWBp * flow_flux_limiter((p+1)->dWBp, p->dWBp);
	(p+1)->iA -= k;
	(p+2)->iA += k;
	k *= p->LB;
	(p+1)->iQ -= k;
	(p+2)->iQ += k;
#endif
	#if DEBUG_FLOW_WAVE_DECOMPOSITION
		if (t>=DEBUG_TIME)
		{
			fprintf(stderr,"FWD iA="FWF" iQ="FWF"\n",p->iA,p->iQ);
			fprintf(stderr,"FWD iA2="FWF" iQ2="FWF"\n",(p+1)->iA,(p+1)->iQ);
			fprintf(stderr,"FWD iA3="FWF" iQ3="FWF"\n",(p-1)->iA,(p-1)->iQ);
			fprintf(stderr,"FWD iA4="FWF" iQ4="FWF"\n",(p+2)->iA,(p+2)->iQ);
			fprintf(stderr,"flow_wave_decomposition: end\n");
		}
	#endif
}

#elif FLOW_WAVE_DECOMPOSITION == FLOW_WAVE_DECOMPOSITION_TVD2

static inline void _flow_wave_decomposition(Parameters *p)
{
	register JBDOUBLE k;
	#if DEBUG_FLOW_WAVE_DECOMPOSITION
		if (t>=DEBUG_TIME)
			fprintf(stderr,"flow_wave_decomposition: start\n");
	#endif
#if FLUX_LIMITER_DISCRETIZATION == FLUX_LIMITER_DISCRETIZATION_CONSERVATIVE
	k = p->dQQm * flow_flux_limiter((p-1)->dQQm, p->dQQm);
	p->iA -= k;
	(p-1)->iA += k;
	k = p->dFFm * flow_flux_limiter((p-1)->dFFm, p->dFFm);
	p->iQ -= k;
	(p-1)->iQ += k;
	k = p->dQQp * flow_flux_limiter((p+1)->dQQp, p->dQQp);
	(p+1)->iA -= k;
	(p+2)->iA += k;
	k = p->dFFp * flow_flux_limiter((p+1)->dFFp, p->dFFp);
	(p+1)->iQ -= k;
	(p+2)->iQ += k;
#else
	p->dQQm = k = p->dWWAm * flow_flux_limiter((p-1)->dWWAm, p->dWWAm);
	p->iA -= k;
	(p-1)->iA += k;
	k *= p->LA;
	p->iQ -= k;
	(p-1)->iQ += k;
	p->dQQm += k = p->dWWBm * flow_flux_limiter((p-1)->dWWBm, p->dWWBm);
	p->iA -= k;
	(p-1)->iA += k;
	k *= p->LB;
	p->iQ -= k;
	(p-1)->iQ += k;
	p->dQQp = k = p->dWWAp * flow_flux_limiter((p+1)->dWWAp, p->dWWAp);
	(p+1)->iA -= k;
	(p+2)->iA += k;
	k *= p->LA;
	(p+1)->iQ -= k;
	(p+2)->iQ += k;
	p->dQQp += k = p->dWWBp * flow_flux_limiter((p+1)->dWWBp, p->dWWBp);
	(p+1)->iA -= k;
	(p+2)->iA += k;
	k *= p->LB;
	(p+1)->iQ -= k;
	(p+2)->iQ += k;
#endif
	#if DEBUG_FLOW_WAVE_DECOMPOSITION
		if (t>=DEBUG_TIME)
		{
			fprintf(stderr,"FWD iA="FWF" iQ="FWF"\n",p->iA,p->iQ);
			fprintf(stderr,"FWD iA2="FWF" iQ2="FWF"\n",(p+1)->iA,(p+1)->iQ);
			fprintf(stderr,"FWD iA3="FWF" iQ3="FWF"\n",(p-1)->iA,(p-1)->iQ);
			fprintf(stderr,"FWD iA4="FWF" iQ4="FWF"\n",(p+2)->iA,(p+2)->iQ);
			fprintf(stderr,"flow_wave_decomposition: end\n");
		}
	#endif
}

#endif

#if INLINE_FLOW_WAVE_DECOMPOSITION
	#define flow_wave_decomposition _flow_wave_decomposition
#else
	#if FLOW_WAVE_DECOMPOSITION == FLOW_WAVE_DECOMPOSITION_UPWIND
		#define flow_wave_decomposition flow_wave_decomposition_upwind
	#else
		void flow_wave_decomposition(Parameters*);
	#endif
#endif

static inline void _flow_friction_explicit(Parameters *p)
{
	#if DEBUG_FLOW_FRICTION_EXPLICIT
		if (t>=DEBUG_TIME) fprintf(stderr,"flow_friction_explicit: start\n");
	#endif
	p->iQ -= dt * p->dx * p->Ff;
	#if DEBUG_FLOW_FRICTION_EXPLICIT
		if (t>=DEBUG_TIME) fprintf(stderr,"flow_friction_explicit: end\n");
	#endif
}

#if INLINE_FLOW_FRICTION_EXPLICIT
	#define flow_friction_explicit _flow_friction_explicit
#else
	void flow_friction_explicit(Parameters *p);
#endif

static inline void _flow_inlet_explicit(BoundaryFlow *bf,Parameters *p)
{
	JBDOUBLE k,k2,k3,k4;
	#if (BOUNDARY_FLOW==BOUNDARY_FLOW_EXTRAPOLATE) ||\
		(BOUNDARY_FLOW==BOUNDARY_FLOW_LOCAL)
		Parameters *q;
	#endif

	#if DEBUG_FLOW_INLET_EXPLICIT
		if (t>=DEBUG_TIME)
		{
			fprintf(stderr,"flow_inlet_explicit: start\n");
			fprintf(stderr,"FIE type=%d\n",bf->type);
			fprintf(stderr,"FIE t="FWL" dt="FWL"\n",t,dt);
			fprintf(stderr,"FIE A="FWF" Q="FWF"\n",p->A,p->Q);
			fprintf(stderr,"FIE iA="FWF" iQ="FWF" dx="FWF"\n",
				p->iA,p->iQ,p->dx);

		}
	#endif

	switch (bf->type)
	{
	case BOUNDARY_FLOW_TYPE_H:
	case BOUNDARY_FLOW_TYPE_HT:
		k = section_area_with_depth(p->s, boundary_flow_parameter(bf, tmax));
		goto area_subcritical;
	case BOUNDARY_FLOW_TYPE_Z:
	case BOUNDARY_FLOW_TYPE_ZT:
		k = section_area(p->s, boundary_flow_parameter(bf, tmax));
		goto area_subcritical;
	case BOUNDARY_FLOW_TYPE_Q_H:
	case BOUNDARY_FLOW_TYPE_QT_HT:
		k3 = boundary_flow_parameter(bf, tmax);
		k = boundary_flow_integral(bf, t, tmax);
		k2 = section_area_with_depth(p->s, boundary_flow_parameter2(bf, tmax));
		goto inlet_fix;
	case BOUNDARY_FLOW_TYPE_Q_Z:
	case BOUNDARY_FLOW_TYPE_QT_ZT:
		k3 = boundary_flow_parameter(bf, tmax);
		k = boundary_flow_integral(bf, t, tmax);
		k2 = section_area(p->s, boundary_flow_parameter2(bf, tmax));
		goto inlet_fix;
	case BOUNDARY_FLOW_TYPE_Q:
	case BOUNDARY_FLOW_TYPE_QT:
		k=boundary_flow_parameter(bf,t);
		k2=boundary_flow_integral(bf,t,tmax);
		goto discharge_subcritical;
	case BOUNDARY_FLOW_TYPE_QH:
		k = -boundary_flow_parameter3(bf, p->h);
		k2 = 0.5 * (k + p->Q) * dt;
		goto discharge_subcritical;
	case BOUNDARY_FLOW_TYPE_QZ:
		k = -boundary_flow_parameter3(bf, p->zs);
		k2 = 0.5 * (k + p->Q) * dt;
		goto discharge_subcritical;
	case BOUNDARY_FLOW_TYPE_GATE:
		k2 = boundary_flow_parameter(bf, t);
		if (k2 <= 0. || p->h<=0.) k=0.; else
			k = 0.61 / sqrt(1. + 0.61 * k2 / p->h) * k2 * GATE_WIDTH(bf) *
				sqrt(2. * JBM_G * p->h);
		k2 += GATE_HEIGHT(bf);
		if (p->h > k2)
		k += GATE_WIDTH(bf) * (p->h - k2) * sqrt(JBM_G * (p->h - k2));
		k = -k;
		k2 = k * dt;
		goto discharge_subcritical;
	case BOUNDARY_FLOW_TYPE_JUNCTION:
		goto inlet_end;
	default:
		k = p->Q + p->iQ / p->dx;
		k2 = 0.5 * (p->Q + k) * dt;
	}

	m->inlet += k2;
	bf->contribution = k2 / dt;
	p->iA += k2 - p->Q * dt;
	k4 = p->A + p->iA / p->dx;
	k2 = -0.99 * section_critical_discharge(p->s, k4);
	if (k4 <= p->Amin)
	{
		k = 0.5 * (p->Q + p->iQ / p->dx) * dt;
		p->iQ = -p->dx * p->Q;
		p->iA -= k;
		m->inlet -= k;
		bf->contribution -= k / dt;
	}
	else if (k>k2)
	{
		k = 0.1 * (k2 - k);
		p->iQ += k * p->dx;
		k*=0.5*dt;
		p->iA += k;
		m->inlet += k;
		bf->contribution += k / dt;
	}
	goto inlet_end;

inlet_fix:
	#if DEBUG_FLOW_INLET_EXPLICIT
		if (t>=DEBUG_TIME)
			fprintf(stderr,"FIE k="FWL" k2="FWL" k3="FWL"\n",k,k2,k3);
	#endif
	m->inlet += k;
	bf->contribution = k / dt;
	p->iA += k - p->Q * dt;
	p->iQ = (k3 / k2 * (p->A + p->iA / p->dx) - p->Q) * p->dx;
	#if DEBUG_FLOW_INLET_EXPLICIT
		if (t>=DEBUG_TIME)
			fprintf(stderr,"FIE iQ="FWF" iA="FWF"\n",p->iQ,p->iA);
	#endif
	goto inlet_end;

area_subcritical:
	#if BOUNDARY_FLOW == BOUNDARY_FLOW_EXTRAPOLATE
		p->iA = (k - p->A) * p->dx;
		q=p+1;
		k2 = jbm_fdbl(q->Q + q->iQ / q->dx);
		k3 = jbm_fdbl(q->A + q->iA / q->dx);
		++q;
		k2 -= q->Q + q->iQ / q->dx + p->Q;
		k3 -= q->A + q->iA / q->dx + p->A + p->iA / p->dx;
		p->iQ = p->dx * (k2 - k3 * p->la);
		#if DEBUG_FLOW_INLET_EXPLICIT
			if (t>=DEBUG_TIME)
				fprintf(stderr,"FIE iQ="FWF" k2="FWL" k3="FWL"\n",p->iQ,k2,k3);
		#endif
		bf->contribution = p->Q + 0.5 * p->iQ / p->dx;
		m->inlet += bf->contribution * dt;
		goto inlet_end;
	#elif BOUNDARY_FLOW == BOUNDARY_FLOW_WAVE
		k = (k - p->A) * p->dx;
		p->iQ += (k - p->iA) * p->la;
		p->iA = k;
		bf->contribution = p->Q + 0.5 * p->iQ / p->dx;
		m->inlet += bf->contribution * dt;
		goto inlet_end;
	#elif BOUNDARY_FLOW == BOUNDARY_FLOW_LOCAL
		p->iA = (k - p->A) * p->dx;
		q=p+1;
		p->iQ = p->dx * (q->iQ / q->dx + jbm_fdbl(q->Q - p->Q) +
			(p->iA + p->dx * q->iA / q->dx) / dt);
		bf->contribution = p->Q + 0.5 * p->iQ / p->dx;
		m->inlet += bf->contribution * dt;
		goto inlet_end;
	#else
		#if DEBUG_FLOW_INLET_EXPLICIT
			if (t>=DEBUG_TIME) fprintf(stderr,"FIE k="FWL"\n",k);
		#endif

		k3 = 0.99 * section_critical_discharge(p->s, k) / sqrt(p->beta);
		k = (k - p->A) * p->dx;
		k2 = k - p->iA;
		k4=k2/dt2;
		#if DEBUG_FLOW_INLET_EXPLICIT
			if (t>=DEBUG_TIME)
				fprintf(stderr,"FIE k2="FWL" k3="FWL" k4="FWL" Q="FWL"\n",
					k2,k3,k4,p->Q);
		#endif
		if (k3 >= p->Q + k4)
		{
			m->inlet += k2 + p->Q * dt;
			bf->contribution = k2 / dt + p->Q;
			p->iA = k;
			p->iQ = k4 * p->dx;
			goto inlet_end;
		}
		k=k3;
		k2=k3*dt;
	#endif

discharge_subcritical:
	#if BOUNDARY_FLOW == BOUNDARY_FLOW_EXTRAPOLATE
		p->iQ = (k - p->Q) * p->dx;
		m->inlet += k2;
		q=p+1;
		k2 = jbm_fdbl(q->Q + q->iQ / q->dx);
		k3 = jbm_fdbl(q->A + q->iA / q->dx);
		++q;
		k2 -= q->Q + q->iQ / q->dx + k;
		k3 -= q->A + q->iA / q->dx + p->A;
		p->iA = p->dx * (k3 - k2 / p->la);
		#if DEBUG_FLOW_INLET_EXPLICIT
			if (t>=DEBUG_TIME)
				fprintf(stderr,"FIE iA="FWF" k2="FWL" k3="FWL"\n",p->iA,k2,k3);
		#endif
		goto inlet_end;
	#elif BOUNDARY_FLOW == BOUNDARY_FLOW_WAVE
		k = (k - p->Q) * p->dx;
		p->iA += (k - p->iQ) / p->la;
		p->iQ = k;
		m->inlet += k2;
		bf->contribution = k2 / dt;
		goto inlet_end;
	#elif BOUNDARY_FLOW == BOUNDARY_FLOW_LOCAL
		k4 = fmax(p->Amin,
			1.01 * section_critical_area(p->s, sqrt(p->beta) * k));
		p->iQ = (k - p->Q) * p->dx;
		k = p->iQ / p->dx;
		q=p+1;
		k3 = q->iQ / q->dx;
		p->iA = fmax((k4 - p->A) * p->dx,
			dt * (jbm_fdbl(p->Q - q->Q) + k - k3) - q->iA * p->dx / q->dx);
		m->inlet += k2;
		bf->contribution = k2 / dt;
		goto inlet_end;
	#else
		#if DEBUG_FLOW_INLET_EXPLICIT
			if (t>=DEBUG_TIME)
				fprintf(stderr,"FIE k="FWL" k2="FWL"\n",k,k2);
		#endif
		p->iQ = (k - p->Q) * p->dx;
		m->inlet += k2;
		bf->contribution = k2 / dt;
		k2 -= p->Q * dt;
		p->iA += k2;
		p->iQ = fmin(p->iQ, p->dx * (0.99 *
			section_critical_discharge(p->s, p->A + p->iA / p->dx) - p->Q));
	#endif

inlet_end:
	#if DEBUG_FLOW_INLET_EXPLICIT
		if (t>=DEBUG_TIME)
		{
			fprintf(stderr,"FIE iA="FWF" iQ="FWF"\n",p->iA,p->iQ);
			fprintf(stderr,"FIE Input:="FWL"\n",m->inlet);
			fprintf(stderr,"flow_inlet_explicit: end\n");
		}
	#endif
	return;
}

#if INLINE_FLOW_INLET_EXPLICIT
	#define flow_inlet_explicit _flow_inlet_explicit
#else
	void flow_inlet_explicit(BoundaryFlow*,Parameters *p);
#endif

static inline void _flow_outlet_explicit(BoundaryFlow *bf,Parameters *p)
{
	JBDOUBLE k,k2,k3,k4;

	#if DEBUG_FLOW_OUTLET_EXPLICIT
		if (t>=DEBUG_TIME)
		{
			fprintf(stderr,"flow_outlet_explicit: start\n");
			fprintf(stderr,"FOE type=%d\n",bf->type);
			fprintf(stderr,"FOE t="FWL" dt="FWL"\n",t,dt);
			fprintf(stderr,"FOE A="FWF" Q="FWF"\n",p->A,p->Q);
			fprintf(stderr,"FOE iA="FWF" iQ="FWF" dx="FWF"\n",
				p->iA,p->iQ,p->dx);
		}
	#endif

	switch (bf->type)
	{
	case BOUNDARY_FLOW_TYPE_H:
	case BOUNDARY_FLOW_TYPE_HT:
		k = section_area_with_depth(p->s, boundary_flow_parameter(bf, tmax));
		goto area_subcritical;
	case BOUNDARY_FLOW_TYPE_Z:
	case BOUNDARY_FLOW_TYPE_ZT:
		k = section_area(p->s, boundary_flow_parameter(bf, tmax));
		goto area_subcritical;
	case BOUNDARY_FLOW_TYPE_Q_H:
	case BOUNDARY_FLOW_TYPE_QT_HT:
		k3 = boundary_flow_parameter(bf, tmax);
		k = -boundary_flow_integral(bf, t, tmax);
		k2 = section_area_with_depth(p->s, boundary_flow_parameter2(bf, tmax));
		goto outlet_fix;
	case BOUNDARY_FLOW_TYPE_Q_Z:
	case BOUNDARY_FLOW_TYPE_QT_ZT:
		k3 = boundary_flow_parameter(bf, tmax);
		k = -boundary_flow_integral(bf, t, tmax);
		k2 = section_area(p->s, boundary_flow_parameter2(bf, tmax));
		goto outlet_fix;
	case BOUNDARY_FLOW_TYPE_Q:
	case BOUNDARY_FLOW_TYPE_QT:
		k=boundary_flow_parameter(bf,tmax);
		k2=-boundary_flow_integral(bf,t,tmax);
		goto discharge_subcritical;
	case BOUNDARY_FLOW_TYPE_QH:
		k = boundary_flow_parameter3(bf, p->h);
		k2 = -0.5 * (k + p->Q) * dt;
		goto discharge_subcritical;
	case BOUNDARY_FLOW_TYPE_QZ:
		k = boundary_flow_parameter3(bf, p->zs);
		k2 = -0.5 * (k + p->Q) * dt;
		goto discharge_subcritical;
	case BOUNDARY_FLOW_TYPE_GATE:
		k2 = boundary_flow_parameter(bf, t);
		if (k2 <= 0. || p->h<=0.) k=0.; else
			k = 0.61 / sqrt(1. + 0.61 * k2 / p->h) * k2 * GATE_WIDTH(bf) *
				sqrt(2. * JBM_G * p->h);
		k2 += GATE_HEIGHT(bf);
		if (p->h > k2)
		k += GATE_WIDTH(bf) * (p->h - k2) * sqrt(JBM_G * (p->h - k2));
		k2 = -k * dt;
		goto discharge_subcritical;
	case BOUNDARY_FLOW_TYPE_JUNCTION:
		goto outlet_end;
	default:
		k = p->Q + p->iQ / p->dx;
		k2 = -0.5 * (p->Q + k) * dt;
	}

	m->outlet += k2;
	bf->contribution = k2 / dt;
	p->iA += k2 + p->Q * dt;
	k4 = p->A + p->iA / p->dx;
	#if DEBUG_FLOW_OUTLET_EXPLICIT
		if (t>=DEBUG_TIME) fprintf(stderr,"FOE section_critical_discharge\n");
	#endif
	k2 = 0.99 * section_critical_discharge(p->s, k4);
	#if DEBUG_FLOW_OUTLET_EXPLICIT
		if (t>=DEBUG_TIME)
		{
			fprintf(stderr,"FOE Output: dx="FWF" Amin="FWF" dt="FWL"\n",
				p->dx,p->Amin,dt);
			fprintf(stderr,"FOE Output: Q="FWF" iQ="FWF" A="FWF" iA="FWF"\n",
				p->Q,p->iQ,p->A,p->iA);
			fprintf(stderr,"FOE Output: k="FWL" k2="FWL" k4="FWL"\n",
				k,k2,k4);
		}
	#endif
	if (k4 <= p->Amin)
	{
		k = 0.5 * (p->Q + p->iQ / p->dx) * dt;
		p->iQ = -p->dx * p->Q;
		p->iA += k;
		m->outlet += k;
		bf->contribution += k / dt;
	}
	else if (k<k2)
	{
		k = 0.1 * (k2 - k);
		#if DEBUG_FLOW_OUTLET_EXPLICIT
			if (t>=DEBUG_TIME) fprintf(stderr,"FOE k="FWL"\n",k);
		#endif
		p->iQ += k * p->dx;
		k*=0.5*dt;
		p->iA -= k ;
		m->outlet -= k;
		bf->contribution -= k / dt;
	}
	#if DEBUG_FLOW_OUTLET_EXPLICIT
		if (t>=DEBUG_TIME)
			fprintf(stderr,"FOE Output: iQ="FWF" iA="FWF"\n",p->iQ,p->iA);
	#endif
	goto outlet_end;

outlet_fix:
	#if DEBUG_FLOW_OUTLET_EXPLICIT
		if (t>=DEBUG_TIME)
			fprintf(stderr,"FOE k="FWL" k2="FWL" k3="FWL"\n",k,k2,k3);
	#endif
	m->outlet += k;
	bf->contribution = k / dt;
	p->iA += k + p->Q * dt;
	p->iQ = (k3 / k2 * (p->A + p->iA / p->dx) - p->Q) * p->dx;
	#if DEBUG_FLOW_OUTLET_EXPLICIT
		if (t>=DEBUG_TIME)
			fprintf(stderr,"FOE iQ="FWF" iA="FWF"\n",p->iQ,p->iA);
	#endif
	goto outlet_end;

area_subcritical:
	#if DEBUG_FLOW_OUTLET_EXPLICIT
		if (t>=DEBUG_TIME) fprintf(stderr,"FOE k="FWL"\n",k);
	#endif
	k3 = -0.99 * section_critical_discharge(p->s, k) / sqrt(p->beta);
	k = (k - p->A) * p->dx;
	k2 = p->iA - k;
	k4=k2/dt2;
	#if DEBUG_FLOW_OUTLET_EXPLICIT
		if (t>=DEBUG_TIME)
			fprintf(stderr,"FOE k2="FWL" k3="FWL" k4="FWL" Q="FWF"\n",
				k2,k3,k4,p->Q);
	#endif
	if (k3 <= p->Q + k4)
	{
		m->outlet -= k2 + p->Q * dt;
		bf->contribution = -k2 / dt - p->Q;
		p->iA = k;
		p->iQ = k4 * p->dx;
		goto outlet_end;
	}
	k=k3;
	k2=-k3*dt;

discharge_subcritical:
	#if DEBUG_FLOW_OUTLET_EXPLICIT
		if (t>=DEBUG_TIME)
			fprintf(stderr,"FOE k="FWL" k2="FWL"\n",k,k2);
	#endif
	p->iQ = (k - p->Q) * p->dx;
	m->outlet += k2;
	bf->contribution = k2 / dt;
	k2 += p->Q * dt;
	p->iA += k2;
	p->iQ = fmax(p->iQ, -p->dx * (p->Q +
		0.99 * section_critical_discharge(p->s, p->A + p->iA / p->dx)));

outlet_end:
	#if DEBUG_FLOW_OUTLET_EXPLICIT
		if (t>=DEBUG_TIME)
		{
			fprintf(stderr,"FOE iA="FWF" iQ="FWF"\n",p->iA,p->iQ);
			fprintf(stderr,"FOE Output:="FWF"\n",m->outlet);
			fprintf(stderr,"flow_outlet_explicit: end\n");
		}
	#endif
	return;
}

#if INLINE_FLOW_OUTLET_EXPLICIT
	#define flow_outlet_explicit _flow_outlet_explicit
#else
	void flow_outlet_explicit(BoundaryFlow*,Parameters *p);
#endif

static inline void _part_decomposition_explicit(int thread)
{
	int i,i2,j,k,l;
	ChannelTransport *ct;
	Channel *channel;
	Parameters *pv;
	TransportParameters *tpv;
	#if DEBUG_PART_DECOMPOSITION_EXPLICIT
		register JBDOUBLE V;
		if (t>=DEBUG_TIME)
			fprintf(stderr,"part_decomposition_explicit: start\n");
	#endif
	i=cell_thread[thread];
	i2=cell_thread[thread+1]-1;
	if (sys->nt >= 0) for (j=i, pv=p+j; j<=i2; ++j, ++pv) pv->DD = 0.;
	#if DEBUG_PART_DECOMPOSITION_EXPLICIT
		if (t>=DEBUG_TIME)
		{
			for (j=i; j<=i2; ++j)
				fprintf(stderr,"PDE i=%d D="FWF"\n",j,p[j].DD);
			for (j=i2, V=0.; --j>=i;) V+=p[j].iA;
			fprintf(stderr,"PDE mass increment="FWL"\n",V);
		}
	#endif
	pv=p+i;
	channel = pv->channel;
	for (; i<i2; ++i, ++pv, ++channel)
	{
		j = jbm_min(i2, channel->i2);
		for (; i<j; ++i, ++pv)
		{
			#if DEBUG_PART_DECOMPOSITION_EXPLICIT
				if (t>=DEBUG_TIME) fprintf(stderr,"PDE i=%d\n",i);
			#endif
			flow_parameters(pv);
			for (k=0, tpv=tp+i; k<=sys->nt; ++k, tpv+=n+1)
			{
				transport_parameters_cell(tpv);
				transport_scheme_parameters(tpv,pv);
			}
		}
	}
	i=cell_thread[thread];
	pv=p+i;
	channel = pv->channel;
	if (i == channel->i2) ++i, ++pv, ++channel;
	#if DEBUG_PART_DECOMPOSITION_EXPLICIT
		if (t>=DEBUG_TIME) fprintf(stderr,"PDE i=%d\n",i);
	#endif
	do
	{
		#if DEBUG_PART_DECOMPOSITION_EXPLICIT
			if (t>=DEBUG_TIME) fprintf(stderr,"PDE i=%d\n",i);
		#endif
		for (j=0; ++j<channel->n;)
		{
			if (channel->bf[j].i < i) continue;
			if (channel->bf[j].i > i2) break;
			flow_inner_boundary(channel->bf + j);
		}
		for (k=0, tpv=tp; k<=sys->nt; ++k, tpv+=n+1)
		{
			ct = channel->ct + k;
			for (j=0; j<=ct->n; ++j)
			{
				l = ct->bt[j].i;
				if (l<i) continue;
				if (l>i2) break;
				transport_boundary(tpv, p, m + k, ct->bt + j);
			}
		}
		#if DEBUG_PART_DECOMPOSITION_EXPLICIT
			if (t>=DEBUG_TIME) fprintf(stderr,"PDE i=%d\n",i);
		#endif
		flow_wave_decomposition_upwind(pv);
		if (sys->nt >= 0)
		{
			diffusion_parameters_implicit(pv);
			for (k=0, tpv=tp+i; k<=sys->nt; ++k, tpv+=n+1)
				transport_scheme_upwind(tpv,pv);
		}
		if (i > channel->i)
		{
			flow_wave_decomposition(pv);
			if (sys->nt >= 0) for (k=0, tpv=tp+i; k<=sys->nt; ++k, tpv+=n+1)
				transport_scheme(tpv,pv);
		}
		j = channel->i2 - 1;
		for (++pv; ++i<j; ++pv)
		{
			if (i==i2) break;
			#if DEBUG_PART_DECOMPOSITION_EXPLICIT
				if (t>=DEBUG_TIME)
					fprintf(stderr,"PDE i=%d j=%d i2=%d\n",i,j,i2);
			#endif
			flow_wave_decomposition_upwind(pv);
			flow_wave_decomposition(pv);
			if (sys->nt >= 0)
			{
				diffusion_parameters_implicit(pv);
				for (k=0, tpv=tp+i; k<=sys->nt; ++k, tpv+=n+1)
				{
					transport_scheme_upwind(tpv,pv);
					transport_scheme(tpv,pv);
				}
			}
		}	
		if (i==i2) break;
		#if DEBUG_PART_DECOMPOSITION_EXPLICIT
			if (t>=DEBUG_TIME)
				fprintf(stderr,"PDE i=%d channeln=%d\n",i,channel->i2);
		#endif
		flow_wave_decomposition_upwind(pv);
		if (sys->nt >= 0)
		{
			diffusion_parameters_implicit(pv);
			for (k=0, tpv=tp+i; k<=sys->nt; ++k, tpv+=n+1)
				transport_scheme_upwind(tpv,pv);
		}
		pv+=2;
		i+=2;
		if (i>=i2) break;
		++channel;
	}
	while (1);
	#if FRICTION_EXPLICIT == FRICTION_EXPLICIT_POINTWISE
		#if DEBUG_PART_DECOMPOSITION_EXPLICIT
			if (t>=DEBUG_TIME) fprintf(stderr,"PDE flow_friction_explicit\n");
		#endif
		i=cell_thread[thread];
		for (pv=p+i, j=i2; --j>=i; ++pv) flow_friction_explicit(pv);
	#endif
	#if DEBUG_PART_DECOMPOSITION_EXPLICIT
		if (t>=DEBUG_TIME)
		{
			for (j=cell_thread[thread]; j<=i2; ++j)
				fprintf(stderr,"PDE i=%d D="FWF"\n",j,p[j].DD);
			fprintf(stderr,"part_decomposition_explicit: end\n");
		}
	#endif
	return;
}

#if INLINE_PART_DECOMPOSITION_EXPLICIT
	#define part_decomposition_explicit _part_decomposition_explicit
#else
	void part_decomposition_explicit(int);
#endif

static inline void _part_decomposition_exit(int thread)
{
	#if DEBUG_PART_DECOMPOSITION_EXIT
		if (t>=DEBUG_TIME)
			fprintf(stderr,"part_decomposition_exit: start\n");
	#endif
	part_decomposition(thread);
	#if DEBUG_PART_DECOMPOSITION_EXIT
		if (t>=DEBUG_TIME)
			fprintf(stderr,"part_decomposition_exit: end\n");
	#endif
	g_thread_exit(NULL);
}

#if INLINE_PART_DECOMPOSITION_EXIT
	#define part_decomposition_exit _part_decomposition_exit
#else
	void part_decomposition_exit(int);
#endif

static inline void _system_decomposition_parallel()
{
	int i,j,k;
	GThread *thread[nth];
	Parameters *pv;
	TransportParameters *tpv;
	#if DEBUG_SYSTEM_DECOMPOSITION_PARALLEL
		if (t>=DEBUG_TIME)
			fprintf(stderr,"system_decomposition_parallel: start\n");
	#endif
	for (i=0; i<nth; ++i)
		thread[i] = g_thread_new
			(NULL, (void(*))part_decomposition_exit, (void*)(size_t)i);
	for (i=0; i<nth; ++i) g_thread_join(thread[i]);
	for (i=0; ++i<nth;)
	{
		j=cell_thread[i]-1;
		pv=p+j;
		if (j != pv->channel->i2)
		{
			flow_parameters(pv);
			flow_wave_decomposition_upwind(pv);
			if (sys->nt >= 0)
			{
				diffusion_parameters_implicit(pv);
				for (k=0, tpv=tp+j; k<=sys->nt; ++k, tpv+=n+1)
				{
					transport_parameters_cell(tpv);
					transport_scheme_parameters(tpv,pv);
				}
			}
			if (j > pv->channel->i + 1 && j < pv->channel->i2 - 1)
			{
				flow_wave_decomposition(pv);
				if (sys->nt >= 0)
					for (k=0, tpv=tp+j; k<=sys->nt; ++k, tpv+=n+1)
						transport_scheme(tpv,pv);
			}
		}
	}
	#if DEBUG_SYSTEM_DECOMPOSITION_PARALLEL
		if (t>=DEBUG_TIME)
		{
			for (i=0; i<n; ++i)
			{
				if (i == p[i].channel->i2) continue;
				fprintf(stderr,"SDP i=%d\n",i);
			   	fprintf(stderr,"SDP LA="FWF" LB="FWF" U="FWF"\n",
					p[i].LA,p[i].LB,p[i].U);
			   	fprintf(stderr,"SDP dQp="FWF" dQm="FWF"\n",
					p[i].dQp,p[i].dQm);
			   	fprintf(stderr,"SDP dFp="FWF" dFm="FWF"\n",
					p[i].dFp,p[i].dFm);
			   	fprintf(stderr,"SDP dWAp="FWF" dWAm="FWF"\n",
					p[i].dWAp,p[i].dWAm);
				fprintf(stderr,"SDP dWBp="FWF" dWBm="FWF"\n",
					p[i].dWBp,p[i].dWBm);
			   	fprintf(stderr,"SDP dWWAp="FWF" dWWAm="FWF"\n",
					p[i].dWWAp,p[i].dWWAm);
				fprintf(stderr,"SDP dWWBp="FWF" dWWBm="FWF"\n",
					p[i].dWWBp,p[i].dWWBm);
			}
			for (i=0; i<=n; ++i)
				fprintf(stderr,"SDP i=%d iQ="FWF" iA="FWF"\n",
					i,p[i].iQ,p[i].iA);
			fprintf(stderr,"system_decomposition_parallel: end\n");
		}
	#endif
}

#if INLINE_SYSTEM_DECOMPOSITION_PARALLEL
	#define system_decomposition_parallel _system_decomposition_parallel
#else
	void system_decomposition_parallel();
#endif

static inline void _system_decomposition_simple()
{
	#if DEBUG_SYSTEM_DECOMPOSITION_SIMPLE
		int i;
		if (t>=DEBUG_TIME)
			fprintf(stderr,"system_decomposition_simple: start\n");
	#endif
	part_decomposition(0);
	#if DEBUG_SYSTEM_DECOMPOSITION_SIMPLE
		if (t>=DEBUG_TIME)
		{
			for (i=0; i<n; ++i)
			{
				if (i == p[i].channel->i2) continue;
				fprintf(stderr,"SDS i=%d\n",i);
			   	fprintf(stderr,"SDP LA="FWF" LB="FWF" U="FWF"\n",
					p[i].LA,p[i].LB,p[i].U);
			   	fprintf(stderr,"SDP dQp="FWF" dQm="FWF"\n",
					p[i].dQp,p[i].dQm);
			   	fprintf(stderr,"SDP dFp="FWF" dFm="FWF"\n",
					p[i].dFp,p[i].dFm);
			   	fprintf(stderr,"SDS dWAp="FWF" dWAm="FWF"\n",
					p[i].dWAp,p[i].dWAm);
				fprintf(stderr,"SDS dWBp="FWF" dWBm="FWF"\n",
					p[i].dWBp,p[i].dWBm);
			   	fprintf(stderr,"SDS dWWAp="FWF" dWWAm="FWF"\n",
					p[i].dWWAp,p[i].dWWAm);
				fprintf(stderr,"SDS dWWBp="FWF" dWWBm="FWF"\n",
					p[i].dWWBp,p[i].dWWBm);
			}
			for (i=0; i<=n; ++i)
				fprintf(stderr,"SDS i=%d iQ="FWF" iA="FWF"\n",
					i,p[i].iQ,p[i].iA);
			fprintf(stderr,"system_decomposition_simple: end\n");
		}
	#endif
}

#if INLINE_SYSTEM_DECOMPOSITION_SIMPLE
	#define system_decomposition_simple _system_decomposition_simple
#else
	void system_decomposition_simple();
#endif

static inline void _flow_implicit_friction(Parameters *p,JBDOUBLE odt)
{
	register JBDOUBLE k,k1,k2,k3;
	#if DEBUG_FLOW_IMPLICIT_FRICTION
		if (t>=DEBUG_TIME)
		{
			fprintf(stderr,"flow_implicit_friction: start\n");
			fprintf(stderr,"FIF K="FWF" Q="FWF"\n",p->K,p->Q);
		}
	#endif
	if (p->A <= p->Amin)
	{
		p->Q = 0.;
		goto exit;
	}

	k1 = fmax(odt, dt - 0.5 * p->dt);
	k = k1 * p->K;
	k2=k+k;
	k3 = (k2 + k2) * (p->Q + k1 * p->Ff);
	if (k3 > 1E-4) k = (sqrt(1. + k3) - 1.) / k2;
	else if (k3 < -1E-4) k = (1. - sqrt(1. - k3)) / k2;
	else goto exit;
	if (k * p->Q < 0.) p->Q = 0.; else p->Q = k;
exit:
	#if DEBUG_FLOW_IMPLICIT_FRICTION
		if (t>=DEBUG_TIME)
		{
			fprintf(stderr,"FIF Q="FWF"\n",p->Q);
			fprintf(stderr,"flow_implicit_friction: end\n");
		}
	#endif
	return;
}

#if INLINE_FLOW_IMPLICIT_FRICTION
	#define flow_implicit_friction _flow_implicit_friction
#else
	void flow_implicit_friction(Parameters *p,JBDOUBLE);
#endif

static inline void _part_simulate_step(int thread)
{
	int i,i2,j;
	Channel *channel;
	Parameters *pv;
	#if DEBUG_PART_SIMULATE_STEP
		if (t>=DEBUG_TIME) fprintf(stderr,"part_simulate_step: start\n");
	#endif
	i=cell_thread[thread];
	i2=cell_thread[thread+1]-1;
	#if DEBUG_PART_SIMULATE_STEP
		if (t>=DEBUG_TIME) fprintf(stderr,"PSS i=%d i2=%d\n",i,i2);
	#endif
	j=i;
	channel=p[j].channel;
	#if DEBUG_PART_SIMULATE_STEP
		if (t>=DEBUG_TIME)
			fprintf(stderr, "PSS j=%d channel->i=%d channel->i2=%d\n",
				j, channel->i, channel->i2);
	#endif
	if (j == channel->i) flow_inlet_explicit(channel->bf, p + j);
	for (j = channel->i2; j<=i2; j = channel->i2)
	{
		#if DEBUG_PART_SIMULATE_STEP
			if (t>=DEBUG_TIME)
				fprintf(stderr, "PSS j=%d channel->i2=%d\n", j, channel->i2);
		#endif
		flow_outlet_explicit(channel->bf + channel->n, p + j);
		if (j==i2) break;
		++channel;
		j = channel->i;
		if (j>i2) break;
		#if DEBUG_PART_SIMULATE_STEP
			if (t>=DEBUG_TIME)
				fprintf(stderr, "PSS j=%d channel->i=%d\n", j, channel->i);
		#endif
		flow_inlet_explicit(channel->bf, p + j);
	}
	pv=p+i;
	for (j=i; j<=i2; ++j, ++pv)
	{
		pv->Qv = pv->Q;
		pv->Av = pv->A;
		pv->V += pv->iA;
		if (pv->V < 0.)
		{
			if (!pv->channel->dry)
			{
				g_mutex_lock(mutex);
				pv->channel->dry = 1;
				g_mutex_unlock(mutex);
			}
		}
		pv->iA /= pv->dx;
		pv->A = pv->V / pv->dx;
		#if DEBUG_PART_SIMULATE_STEP
			if (t>=DEBUG_TIME) fprintf(stderr,
				"PSS i=%d Av="FWF" A="FWF"\nQv="FWF" Q="FWF" iQ="FWF"\n",
					j,pv->Av,pv->A,pv->Qv,pv->Q,pv->iQ);
		#endif
	}
	#if DEBUG_PART_SIMULATE_STEP
		if (t>=DEBUG_TIME) fprintf(stderr,"part_simulate_step: end\n");
	#endif
}

#if INLINE_PART_SIMULATE_STEP
	#define part_simulate_step _part_simulate_step
#else
	void part_simulate_step(int);
#endif

static inline void _part_simulate_step_exit(int thread)
{
	#if DEBUG_PART_SIMULATE_STEP_EXIT
		if (t>=DEBUG_TIME)
			fprintf(stderr,"part_simulate_step_exit: start\n");
	#endif
	part_simulate_step(thread);
	#if DEBUG_PART_SIMULATE_STEP_EXIT
		if (t>=DEBUG_TIME)
			fprintf(stderr,"part_simulate_step_exit: end\n");
	#endif
	g_thread_exit(NULL);
}

#if INLINE_PART_SIMULATE_STEP_EXIT
	#define part_simulate_step_exit _part_simulate_step_exit
#else
	void part_simulate_step_exit(int);
#endif

static inline void _junction_flow_scheme(Junction *junction)
{
	register JBDOUBLE z;
	ChannelGeometry *cg;
	JunctionVolume *jv;
	Parameters *pv;
	TransportParameters *tpv;
	int i,j;
	#if DEBUG_JUNCTION_FLOW_SCHEME
		if (t>=DEBUG_TIME) fprintf(stderr,"junction_flow_scheme: start\n");
	#endif
	for (j=sys->nt+1; j>=0; --j) junction->mass[j]=0.;
	for (i=0; i<=junction->ninlets; ++i)
	{
		cg = junction->inlet[i]->cg;
		junction->mass[0] -= p[cg->cs->i].Q;
		for (j=0, tpv=tp+cg->cs->i; j<=sys->nt; tpv+=n)
			junction->mass[++j] -= tpv->T;
	}
	for (i=0; i<=junction->noutlets; ++i)
	{
		cg = junction->outlet[i]->cg;
		junction->mass[0] += p[cg->cs[cg->n].i].Q;
		for (j=0, tpv=tp+cg->cs[cg->n].i; j<=sys->nt; tpv+=n)
			junction->mass[++j] += tpv->T;
	}
	for (j=sys->nt+1; j>=0; --j) junction->mass[j]*=dt;
	for (i=0; i<=junction->nparameters; ++i)
	{
		pv = junction->parameter[i];
		junction->mass[0] += pv->V;
		for (j=0, tpv=junction->transport[i]; j<=sys->nt; tpv+=n)
			junction->mass[++j] += tpv->c * pv->V;
	}
	junction->mass[0] = fmax(junction->mass[0], 0.);
	#if DEBUG_JUNCTION_FLOW_SCHEME
		if (t>=DEBUG_TIME)
		{
			fprintf(stderr, "JFS mass="FWF"\n", junction->mass[0]);
			for (j=0; j<=sys->nt;)
			{
				++j;
				fprintf(stderr, "JFS j=%d mass="FWF"\n", j, junction->mass[j]);
			}
			for (j=0; j<=junction->nvolumes; ++j)
				fprintf(stderr, "JFS j=%d V="FWF" z="FWF"\n",
					j, junction->volume[j].V, junction->volume[j].z);
		}
	#endif

	jv = junction->volume + junction->nvolumes;
	#if DEBUG_JUNCTION_FLOW_SCHEME
		if (t>=DEBUG_TIME) fprintf(stderr, "JFS nvolumes=%d V="FWF" z="FWF"\n",
			junction->nvolumes, jv->V, jv->z);
	#endif
	if (junction->mass[0] >= jv->V)
		z = jv->z + (junction->mass[0] - jv->V) / jv->A;
	else
	{
		i = jbm_varray_search(junction->mass[0], junction->volume,
			sizeof(JunctionVolume), junction->nvolumes);
		jv = junction->volume + i;
		#if DEBUG_JUNCTION_FLOW_SCHEME
			if (t>=DEBUG_TIME)
				fprintf(stderr, "JFS i=%d V="FWF" z="FWF" A="FWF" dAz="FWF"\n",
					i, jv->V, jv->z, jv->A, jv->dAz);
		#endif
		if (jv->dAz==0.) z = jv->z + (junction->mass[0] - jv->V) / jv->A;
		else z = jv->z + (sqrt(jbm_fsqr(jv->A) + jbm_fdbl(jv->dAz) *
			(junction->mass[0] - jv->V)) - jv->A) / jv->dAz;
	}
	if (junction->mass[0] > 0.) for (j=0; j<=sys->nt;)
	{
		junction->mass[++j] /= junction->mass[0];
		#if DEBUG_JUNCTION_FLOW_SCHEME
			if (t>=DEBUG_TIME)
				fprintf(stderr,"JFS j=%d c="FWF"\n",j,junction->mass[j]);
		#endif
	}
	else for (j=0; j<=sys->nt;) junction->mass[++j] = 0.;
	#if DEBUG_JUNCTION_FLOW_SCHEME
		if (t>=DEBUG_TIME) fprintf(stderr,"JFS z="FWL"\n",z);
	#endif
	for (i=0; i<=junction->nparameters; ++i)
	{
		pv = junction->parameter[i];
		pv->A = section_area(pv->s, z);
		pv->V = pv->A * pv->dx;
		pv->iA = pv->A - pv->Av;
		#if DEBUG_JUNCTION_FLOW_SCHEME
			if (t>=DEBUG_TIME)
				fprintf(stderr, "JFS A="FWF" V="FWF" Av="FWF" zv="FWF"\n",
					pv->A, pv->V, pv->Av, pv->zs);
		#endif
		for (j=0, tpv=junction->transport[i]; j<=sys->nt; tpv+=n)
		{
			tpv->c = junction->mass[++j];
			tpv->m = pv->A * tpv->c;
			#if DEBUG_JUNCTION_FLOW_SCHEME
				if (t>=DEBUG_TIME)
					fprintf(stderr, "JFS j=%d c="FWF" m="FWF"\n",
						j, tpv->c, tpv->m);
			#endif
		}
	}
	#if DEBUG_JUNCTION_FLOW_SCHEME
		if (t>=DEBUG_TIME)
		{
			for (i=0, z=0; i<=junction->nparameters; ++i)
				z += junction->parameter[i]->V;
			fprintf(stderr,"JFS mass="FWL"\n",z);
			fprintf(stderr,"junction_flow_scheme: end\n");
		}
	#endif
}

#if INLINE_JUNCTION_FLOW_SCHEME
	#define junction_flow_scheme _junction_flow_scheme
#else
	void junction_flow_scheme(Junction*);
#endif

static inline void _system_simulate_step_parallel()
{
	int i;
	GThread *thread[nth];
	#if DEBUG_SYSTEM_SIMULATE_STEP_PARALLEL
		if (t>=DEBUG_TIME)
			fprintf(stderr,"system_simulate_step_parallel: start\n");
	#endif
	for (i=0; i<=sys->n; ++i) sys->channel[i].dry = 0;
	for (i=0; i<nth; ++i)
		thread[i] = g_thread_new
			(NULL, (void(*))part_simulate_step_exit, (void*)(size_t)i);
	for (i=0; i<nth; ++i) g_thread_join(thread[i]);
	#if DEBUG_SYSTEM_SIMULATE_STEP_PARALLEL
		if (t>=DEBUG_TIME)
			fprintf(stderr,"SSSP njunctions=%d\n",njunctions);
	#endif
	for (i=0; i<=njunctions; ++i) junction_flow_scheme(junction+i);
	#if DEBUG_SYSTEM_SIMULATE_STEP_PARALLEL
		if (t>=DEBUG_TIME)
		{
			for (i=0; i<=n; ++i)
				fprintf(stderr,"SSSP i=%d Q="FWF" A="FWF"\n",i,p[i].Q,p[i].A);
			fprintf(stderr,"system_simulate_step_parallel: end\n");
		}
	#endif
}

#if INLINE_SYSTEM_SIMULATE_STEP_PARALLEL
	#define system_simulate_step_parallel _system_simulate_step_parallel
#else
	void system_simulate_step_parallel();
#endif

static inline void _system_simulate_step_simple()
{
	int i;
	#if DEBUG_SYSTEM_SIMULATE_STEP_SIMPLE
		if (t>=DEBUG_TIME)
			fprintf(stderr,"system_simulate_step_simple: start\n");
	#endif
	for (i=0; i<=sys->n; ++i) sys->channel[i].dry = 0;
	part_simulate_step(0);
	#if DEBUG_SYSTEM_SIMULATE_STEP_SIMPLE
		if (t>=DEBUG_TIME)
			fprintf(stderr,"SSSS njunctions=%d\n",njunctions);
	#endif
	for (i=0; i<=njunctions; ++i) junction_flow_scheme(junction+i);
	#if DEBUG_SYSTEM_SIMULATE_STEP_SIMPLE
		if (t>=DEBUG_TIME)
		{
			for (i=0; i<=n; ++i)
				fprintf(stderr,"SSSS i=%d Q="FWF" A="FWF"\n",i,p[i].Q,p[i].A);
			fprintf(stderr,"system_simulate_step_simple: end\n");
		}
	#endif
}

#if INLINE_SYSTEM_SIMULATE_STEP_SIMPLE
	#define system_simulate_step_simple _system_simulate_step_simple
#else
	void system_simulate_step_simple();
#endif

static inline void _part_simulate_step2(int thread)
{
	int i,i2,j;
	JBDOUBLE k,k2;
	Channel *channel;
	Parameters *pv;
	#if DEBUG_PART_SIMULATE_STEP2
		if (t>=DEBUG_TIME) fprintf(stderr,"part_simulate_step2: start\n");
	#endif
	i=cell_thread[thread];
	i2=cell_thread[thread+1]-1;
	#if DEBUG_PART_SIMULATE_STEP2
		if (t>=DEBUG_TIME) fprintf(stderr,"PSS2 i=%d i2=%d\n",i,i2);
	#endif
	if (sys->nt >= 0) for (j=i, pv=p+i; j<=i2; ++j, ++pv) pv->DD += pv->V;
	part_parameters(thread);

	k=fo*dt2;
	pv=p+i;
	channel = pv->channel;
	if (i != channel->i)
	{
		k2 = k * pv->gm * pv->dzb;
		pv->iQ -= k2 * pv->iA;
		(pv+1)->iQ -= k2 * (pv+1)->iA;
	}
	do
	{
		#if DEBUG_PART_SIMULATE_STEP2
			if (t>=DEBUG_TIME)
				fprintf(stderr,"PSS2 i=%d channel0=%d\n",i,channel->i);
		#endif
		pv->Q += pv->iQ / pv->dx;
		#if DEBUG_PART_SIMULATE_STEP2
			if (t>=DEBUG_TIME)
				fprintf(stderr,"PSS2 i=%d Q="FWF" iQ="FWF"\n",i,pv->Q,pv->iQ);
		#endif
		++i;
		++pv;
		#if DEBUG_PART_SIMULATE_STEP2
			if (t>=DEBUG_TIME) fprintf(stderr,"PSS2 i=%d i2=%d channeln=%d\n",
				i,i2,channel->i2);
		#endif
		for (j=jbm_min(i2,channel->i2)-1; i<j; ++i)
		{
			k2 = k * pv->gm * pv->dzb;
			pv->iQ -= k2 * pv->iA;
			pv->Q += pv->iQ / pv->dx;
			flow_implicit_friction(pv,k);
			#if DEBUG_PART_SIMULATE_STEP2
				if (t>=DEBUG_TIME)
					fprintf(stderr,"PSS2 i=%d Q="FWF" iQ="FWF"\n",
						i,pv->Q,pv->iQ);
			#endif
			++pv;
			pv->iQ -= k2 * pv->iA;
		}
		if (++i == channel->i2)
		{
			pv->Q += pv->iQ / pv->dx;
			flow_implicit_friction(pv,k);
			++pv;
			pv->Q += pv->iQ / pv->dx;
		}
		else
		{
			k2 = k * pv->gm * pv->dzb;
			pv->iQ -= k2 * pv->iA;
			pv->Q += pv->iQ / pv->dx;
			flow_implicit_friction(pv,k);
			++pv;
			k2 = k * pv->gm * pv->dzb;
			pv->iQ -= k2 * pv->iA;
			pv->Q += pv->iQ / pv->dx;
			flow_implicit_friction(pv,k);
			++pv;
			g_mutex_lock(mutex);
			pv->iQ -= k2 * pv->iA;
			g_mutex_unlock(mutex);
			break;
		}
		#if DEBUG_PART_SIMULATE_STEP2
			if (t>=DEBUG_TIME)
				fprintf(stderr,"PSS2 i=%d Q="FWF" iQ="FWF"\n",i,pv->Q,pv->iQ);
		#endif
		if (++i>=i2) break;
		++pv;
		++channel;
	}
	while (1);
	#if DEBUG_PART_SIMULATE_STEP2
		if (t>=DEBUG_TIME) fprintf(stderr,"part_simulate_step2: end\n");
	#endif
}

#if INLINE_PART_SIMULATE_STEP2
	#define part_simulate_step2 _part_simulate_step2
#else
	void part_simulate_step2(int);
#endif

static inline void _part_simulate_step2_exit(int thread)
{
	#if DEBUG_PART_SIMULATE_STEP2_EXIT
		if (t>=DEBUG_TIME)
			fprintf(stderr,"part_simulate_step2_exit: start\n");
	#endif
	part_simulate_step2(thread);
	#if DEBUG_PART_SIMULATE_STEP2_EXIT
		if (t>=DEBUG_TIME)
			fprintf(stderr,"part_simulate_step2_exit: end\n");
	#endif
	g_thread_exit(NULL);
}

#if INLINE_PART_SIMULATE_STEP2_EXIT
	#define part_simulate_step2_exit _part_simulate_step2_exit
#else
	void part_simulate_step2_exit(int);
#endif

static inline void _system_simulate_step2_parallel()
{
	int i,j;
	GThread *thread[nth];
	Parameters *pv;
	Channel *channel;
	#if DEBUG_SYSTEM_SIMULATE_STEP2_PARALLEL
		if (t>=DEBUG_TIME)
			fprintf(stderr,"system_simulate_step2_parallel: start\n");
	#endif
	system_dry_correction_parallel();
	for (i=0; i<nth; ++i)
		thread[i] = g_thread_new
			(NULL, (void(*))part_simulate_step2_exit, (void*)(size_t)i);
	for (i=0; i<nth; ++i) g_thread_join(thread[i]);
	for (i=0; ++i<=nth;)
	{
		j=cell_thread[i-1];
		pv=p+j;
		#if DEBUG_SYSTEM_SIMULATE_STEP2_PARALLEL
			if (t>=DEBUG_TIME) fprintf(stderr,"SSS2P j=%d\n",j);
		#endif
		if (j != pv->channel->i) flow_implicit_friction(pv, fo * dt2);
	}
	for (i=0; ++i<nth;)
	{
		j=cell_thread[i]-1;
		pv=p+j;
		channel = pv->channel;
		if (j < channel->i2)
		{
			parameters_cell(pv);
			if (j > channel->i) pv->g = 0.5 * ((pv-1)->gm + pv->gm);
			else pv->g = pv->gm;
			parameters_wave_velocity(pv);
			if (++j < channel->i2)
			{
				++pv;
				pv->g = 0.5 * ((pv-1)->gm + pv->gm);
				parameters_wave_velocity(pv);
			}
			else
			{
				++pv;
				pv->g = (pv-1)->gm;
				parameters_wave_velocity(pv);
			}
		}
	}
	#if DEBUG_SYSTEM_SIMULATE_STEP2_PARALLEL
		if (t>=DEBUG_TIME)
		{
			for (i=0; i<=n; ++i)
				fprintf(stderr,"SSS2P i=%d Q="FWF" A="FWF"\n",i,p[i].Q,p[i].A);
			fprintf(stderr,"system_simulate_step2_parallel: end\n");
		}
	#endif
}

#if INLINE_SYSTEM_SIMULATE_STEP2_PARALLEL
	#define system_simulate_step2_parallel _system_simulate_step2_parallel
#else
	void system_simulate_step2_parallel();
#endif

static inline void _system_simulate_step2_simple()
{
	#if DEBUG_SYSTEM_SIMULATE_STEP2_SIMPLE
		if (t>=DEBUG_TIME)
			fprintf(stderr,"system_simulate_step2_simple: start\n");
	#endif
	system_dry_correction_simple();
	part_simulate_step2(0);
	#if DEBUG_SYSTEM_SIMULATE_STEP2_SIMPLE
		if (t>=DEBUG_TIME)
			fprintf(stderr,"system_simulate_step2_simple: end\n");
	#endif
}

#if INLINE_SYSTEM_SIMULATE_STEP2_SIMPLE
	#define system_simulate_step2_simple _system_simulate_step2_simple
#else
	void system_simulate_step2_simple();
#endif

static inline JBDOUBLE _flow_steady_error(Parameters *p,int n)
{
	register int i;
	register JBDOUBLE e;
	#if DEBUG_FLOW_STEADY_ERROR
		fprintf(stderr,"flow_steady_error: start\n");
		fprintf(stderr,"FSE n=%d dt="FWL"\n",n,dt);
	#endif
	for (i=0, e=0.; i<=n; ++i, ++p)
	{
		e += (p->iQ * p->iQ + p->iA * p->iA) / jbm_fsqr(p->dx * dt);
		#if DEBUG_FLOW_STEADY_ERROR
			fprintf(stderr,"FSE i=%d dx="FWF" iQ="FWF" iA="FWF"\n",
				i,p->dx,p->iQ,p->iA);
		#endif
	}
	e/=n;
	e=sqrt(e);
	#if DEBUG_FLOW_STEADY_ERROR
		fprintf(stderr,"FSE e="FWL"\n",e);
		fprintf(stderr,"flow_steady_error: end\n");
	#endif
	return e;
}

#if INLINE_FLOW_STEADY_ERROR
	#define flow_steady_error _flow_steady_error
#else
	JBDOUBLE flow_steady_error(Parameters*,int);
#endif

#endif

