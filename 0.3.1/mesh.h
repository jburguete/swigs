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

/**
 * \file mesh.h
 * \brief Header file to define the mesh.
 * \author Javier Burguete Tolosa.
 * \copyright Copyright 2005-2014 Javier Burguete Tolosa. All right reserved.
 */
#ifndef MESH__H
#define MESH__H 1

#include "system.h"
#include "channel_segment.h"
#include "section_changes.h"

/**
 * \def N_OF_VARIABLES
 * \brief Macro to define the number of variables to save in the solution file.
 */
#define N_OF_VARIABLES 8

typedef struct
{
	int j,nw,nchannel,nsegment;
	JBFLOAT x,A,Q,zs,h,B,P,R,I,zm,u,uc,F,K,V,dx,ix,dzs,dzb,iA,iQ,Av,Qv,dt,
		C2,LA,LB,U,dQ,dF,dQp,dFp,dQm,dFm,dQQp,dFFp,dQQm,dFFm,Ep,Em,la,lb,lmax,
		zsm,g,dg,gm,nu,Anu,A2,Ff,beta,dbetaA,Kx,D,hmin,zmin,Amin,CC,DD,EE,HH,
		ta,tr;
//	JBFLOAT Sf;
	#if FLUX_LIMITER_DISCRETIZATION == \
		FLUX_LIMITER_DISCRETIZATION_CHARACTERISTIC
		JBFLOAT dWAp,dWBp,dWAm,dWBm,dWWAp,dWWBp,dWWAm,dWWBm;
	#endif
	#if FLOW_LINEARIZATION == FLOW_LINEARIZATION_ROE
		JBFLOAT sA,sA2;
	#endif
	#if MODEL_PRESSURE == MODEL_PRESSURE_HIGH_ORDER
		JBFLOAT zp,izm,izmv,dzm,dzp,dzpv,ddzp,uv,um,uu;
	#endif
	Section s[1];
	Channel *channel;
} Parameters;

#define iAp dQQp
#define iQp dFFp
#define iAm dQQm
#define iQm dFFm

typedef struct
{
	JBDOUBLE m0,m,inlet,inner,outlet,*mi;
	unsigned int n;
} Mass;

typedef struct
{
	JBFLOAT c,b,m,T,ic,dc,dT,dTp,dTm,dTTp,dTTm,S,xi;
	#if (TRANSPORT_DISCRETIZATION!=TRANSPORT_DISCRETIZATION_SPLIT) && \
		(FLOW_LINEARIZATION==FLOW_LINEARIZATION_ROE)
		JBFLOAT csA;
	#endif
	#if TRANSPORT_SEMILAGRANGIAN_CONTROL
		JBFLOAT cmin,cmax;
	#endif
} TransportParameters;

#define dWCp dTTp
#define dWCm dTTm
#define iTp dWCp
#define iTm dWCm

typedef struct
{
	JBFLOAT *x,*Q,*zs,*A,*B,*zb,*zmax,*beta,*p;
} Variables;

typedef struct
{
	JBFLOAT V,A,dAz,z;
} JunctionVolume;

typedef struct
{
	int nparameters,nvolumes,ninlets,noutlets,type;
	Channel **inlet,**outlet;
	Parameters **parameter;
	TransportParameters **transport;
	JBFLOAT *angle,*mass;
	JunctionVolume *volume;
} Junction;

extern int nth,nthreads,*cell_thread;
extern int n_open_sections;
extern int n,njunctions,vsize;
extern JBDOUBLE t,dt;
extern Parameters *p;
extern TransportParameters *tp;
extern Junction *junction;
extern Variables v[1];
extern Mass *m;
extern GSList **list_channels;

#if JBW==JBW_GTK
	extern int n_open_sections;
	extern void dialog_open_parameters_actualize();
#endif

static inline void _parameters_print(Parameters *p,FILE *file)
{
	fprintf(file,"parameters_print: start\n");
	fprintf(file,"P j=%d x="FWF" dx="FWF" ix="FWF"\n",p->j,p->x,p->dx,p->ix);
	fprintf(file,"P A="FWF" Q="FWF" zs="FWF" h="FWF"\n",p->A,p->Q,p->zs,p->h);
	section_print(p->s,file);
	fprintf(file,"parameters_print: end\n");
}

#if INLINE_PARAMETERS_PRINT
	#define parameters_print _parameters_print
#else
	void parameters_print(Parameters*,FILE*);
#endif

static inline void _mass_start(Mass *m)
{
	#if DEBUG_MASS_START
		fprintf(stderr,"mass_start: start\n");
	#endif
	m->inlet = m->outlet = m->inner = 0.;
	#if DEBUG_MASS_START
		fprintf(stderr,"mass_start: end\n");
	#endif
}

#if INLINE_MASS_START
	#define mass_start _mass_start
#else
	void mass_start(Mass*);
#endif

static inline void _mass_copy(Mass *m,Mass *mcopy)
{
	#if DEBUG_MASS_COPY
		fprintf(stderr,"mass_copy: start\n");
	#endif
	m->m = mcopy->m;
	m->inlet = mcopy->inlet;
	m->outlet = mcopy->outlet;
	m->inner = mcopy->inner;
	#if DEBUG_MASS_COPY
		fprintf(stderr,"mass_copy: end\n");
	#endif
}

#if INLINE_MASS_COPY
	#define mass_copy _mass_copy
#else
	void mass_copy(Mass*,Mass*);
#endif

static inline JBDOUBLE _flow_mass(Parameters *p,int n)
{
	register int i;
	register JBDOUBLE k=0.;
	#if DEBUG_FLOW_MASS
		if (t>=DEBUG_TIME) fprintf(stderr,"flow_mass: start\n");
	#endif
	for (i=n+1; --i>=0;) k += p[i].V;
	#if DEBUG_FLOW_MASS
		if (t>=DEBUG_TIME)
		{
			fprintf(stderr,
				"m "FWL" Q0 "FWF" Qn "FWF" dt "FWL"\n",k,p[0].Q,p[n].Q,dt);
			fprintf(stderr,"flow_mass: end\n");
		}
	#endif
	return k;
}

#if INLINE_FLOW_MASS
	#define flow_mass _flow_mass
#else
	JBDOUBLE flow_mass(Parameters*,int);
#endif

static inline JBDOUBLE _flow_mass_error(Mass *m,Parameters *p,int n)
{
	register JBDOUBLE k,mass;
	#if DEBUG_FLOW_MASS_ERROR
		if (t>=DEBUG_TIME)
		{
			fprintf(stderr,"flow_mass_error: start\n");
			fprintf(stderr,"FME m0="FWL" inlet="FWL" inner="FWL" outlet="FWL"\n",
				m->m0,m->inlet,m->inner,m->outlet);
		}
	#endif
	mass = m->m0 + m->inlet + m->inner + m->outlet;
	m->m = flow_mass(p,n);
	k = fmax(m->m, mass);
	if (k>0.) k = 100. * (m->m - mass) / k;
	#if DEBUG_FLOW_MASS_ERROR
		if (t>=DEBUG_TIME)
			fprintf(stderr,"FME "FWL"%%\nflow_mass_error: end\n",k);
	#endif
	return k;
}

#if INLINE_FLOW_MASS_ERROR
	#define flow_mass_error _flow_mass_error
#else
	JBDOUBLE flow_mass_error(Mass*,Parameters*,int);
#endif

static inline JBDOUBLE _transport_mass(TransportParameters *tp,int n)
{
	register int i;
	register TransportParameters *tpv;
	register JBDOUBLE m;
	#if DEBUG_TRANSPORT_MASS
		if (t>=DEBUG_TIME) fprintf(stderr,"transport_mass: start\n");
	#endif
	for (i=0, tpv=tp, m=0.; i<=n; ++i, ++tpv) m += tpv->m + tpv->b;
	#if DEBUG_TRANSPORT_MASS
		if (t>=DEBUG_TIME)
		{
			register JBDOUBLE k;
			for (i=0, tpv=tp, k=0.; i<=n; ++i, ++tpv) k += tpv->m;
			fprintf(stderr,"TM ms="FWL,k);
			for (i=0, tpv=tp, k=0.; i<=n; ++i, ++tpv) k += tpv->b;
			fprintf(stderr," mb="FWL" mt="FWL"\ntransport_mass: end\n",k,m);
		}
	#endif
	return m;
}

#if INLINE_TRANSPORT_MASS
	#define transport_mass _transport_mass
#else
	JBDOUBLE transport_mass(TransportParameters*,int);
#endif

static inline JBDOUBLE _transport_mass_error
	(Mass *m,TransportParameters *tp,int n)
{
	register JBDOUBLE k,mass;
	#if DEBUG_TRANSPORT_MASS_ERROR
		if (t>=DEBUG_TIME) fprintf(stderr,"transport_mass_error: start\n");
	#endif
	mass = m->m0 + m->inlet + m->inner + m->outlet;
	m->m = transport_mass(tp, n);
	#if DEBUG_TRANSPORT_MASS_ERROR
		if (t>=DEBUG_TIME)
			fprintf(stderr,
				"FME m0="FWL" m="FWL"\ninlet="FWL" inner="FWL" outlet="FWL"\n",
				m->m0,m->m,m->inlet,m->inner,m->outlet);
	#endif
	k = fmax(m->m, mass);
	if (k>0.) k = 100. * (m->m - mass) / k;
	#if DEBUG_TRANSPORT_MASS_ERROR
		if (t>=DEBUG_TIME)
			fprintf(stderr,"TME="FWL"\ntransport_mass_error: end\n",mass);
	#endif
	return k;
}

#if INLINE_TRANSPORT_MASS_ERROR
	#define transport_mass_error _transport_mass_error
#else
	JBDOUBLE transport_mass_error(Mass*,TransportParameters*,int);
#endif

static inline void _transport_variables_interpolate
	(Parameters *p,TransportParameters *tp,int n,InitialTransport *it)
{
	register int i,j,k;
	#if DEBUG_TRANSPORT_VARIABLES_INTERPOLATE
		fprintf(stderr,"transport_variables_interpolate: start\n");
		initial_transport_print(it,stderr);
		fprintf(stderr,"TVI n=%d itn=%d\n",n,it->n);
	#endif
	for (i=0; i<=n && p[i].x<=it->p[0].x; ++i) tp[i].c = it->p[0].y;
	#if DEBUG_TRANSPORT_VARIABLES_INTERPOLATE
		fprintf(stderr,"TVI i=%d\n",i);
	#endif
	for (j=n; j>=0 && p[j].x>=it->p[it->n].x; --j) tp[j].c = it->p[it->n].y;
	#if DEBUG_TRANSPORT_VARIABLES_INTERPOLATE
		fprintf(stderr,"TVI j=%d\n",j);
	#endif
	for (k=1; i<=j; ++i)
	{
		while (it->p[k].x < p[i].x) ++k;
		tp[i].c = jbm_extrapolate(p[i].x, it->p[k-1].x, it->p[k].x,
			it->p[k-1].y, it->p[k].y);
	}
	#if DEBUG_TRANSPORT_VARIABLES_INTERPOLATE
		for (i=0; i<=n; ++i)
			fprintf(stderr,"TVI i=%d x="FWF" c="FWF"\n",i,p[i].x,tp[i].c);
		fprintf(stderr,"transport_variables_interpolate: end\n");
	#endif
}

#if INLINE_TRANSPORT_VARIABLES_INTERPOLATE
	#define transport_variables_interpolate _transport_variables_interpolate
#else
	void transport_variables_interpolate
		(Parameters*,TransportParameters*,int,InitialTransport*);
#endif

static inline void _transport_variables_open
	(Parameters *p,TransportParameters *tp,int n,InitialTransport *it)
{
	register int i;
	#if DEBUG_TRANSPORT_VARIABLES_OPEN
		fprintf(stderr,"transport_variables_open: start\n");
		initial_transport_print(it,stderr);
	#endif
	switch (it->type)
	{
	case INITIAL_TRANSPORT_STEADY:
	case INITIAL_TRANSPORT_DRY:
		for (i=n+1; --i>=0;) tp[i].c = 0.;
		break;
	case INITIAL_TRANSPORT_XC:
		transport_variables_interpolate(p,tp,n,it);
	}
	for (i=n+1; --i>=0;)
	{
		tp[i].b = 0.;
		tp[i].m = p[i].V * tp[i].c;
	}
	#if DEBUG_TRANSPORT_VARIABLES_OPEN
		for (i=0; i<=n; ++i)
			fprintf(stderr,"TVO i=%d c="FWF"\n",i,tp[i].c);
		fprintf(stderr,"transport_variables_open: end\n");
	#endif
}

#if INLINE_TRANSPORT_VARIABLES_OPEN
	#define transport_variables_open _transport_variables_open
#else
	void transport_variables_open
		(Parameters*,TransportParameters*,int,InitialTransport*);
#endif

static inline int _variables_create(Variables *v,int n,int *vsize,int nt)
{
	#if DEBUG_VARIABLES_CREATE
		fprintf(stderr,"variables_create: start\n");
		fprintf(stderr,"VC %ld\n",(size_t)v->x);
	#endif
	++n;
	*vsize = (N_OF_VARIABLES + nt + 1) * sizeof(JBFLOAT);
	#if DEBUG_VARIABLES_CREATE
		fprintf(stderr,"VC: n=%d size=%d\n",n,*vsize);
	#endif
	v->x = (JBFLOAT*)jb_try_realloc(v->x, n * *vsize);
	if (!v->x)
	{
		jbw_show_error2(gettext("Variables"),gettext("Not enough memory"));
		#if DEBUG_VARIABLES_CREATE
			fprintf(stderr,"VC: memory error\n");
			fprintf(stderr,"variables_create: end\n");
		#endif
		return 0;
	}
	v->Q = v->x + n;
	v->zs = v->Q + n;
	v->A = v->zs + n;
	v->B = v->A + n;
	v->zb = v->B + n;
	v->zmax = v->zb + n;
	v->beta = v->zmax + n;
	v->p = v->beta + n;
	#if DEBUG_VARIABLES_CREATE
		fprintf(stderr,"VC %ld\n",(size_t)v->x);
		fprintf(stderr,"variables_create: end\n");
	#endif
	return 1;
}

#if INLINE_VARIABLES_CREATE
	#define variables_create _variables_create
#else
	int variables_create(Variables*,int,int*,int);
#endif

static inline JBDOUBLE _mesh_hmin(Parameters *p)
{
	register int i,j,n;
	JBDOUBLE R,h,z,hmin;
	#if DEBUG_MESH_HMIN
		fprintf(stderr,"mesh_hmin: start\n");
	#endif
	for (i=j=0, n=p->s->nf; n-i>1;)
	{
		j=(i+n)>>1;
		z = p->s->sp->z + j * p->s->dz;
		R = section_area(p->s, z) / section_perimeter(p->s, z);
		h = pow(0.5 * p->dx * p->s->fp[j].r, friction_2b1i);
		#if DEBUG_MESH_HMIN
			fprintf(stderr,"MH i=%d j=%d n=%d R="FWL" h="FWL" r="FWF"\n",
				i,j,n,R,h,p->s->fp[j].r);
		#endif
		if (R<h) i=j; else n=j;
	}
	hmin = fmax(sys->depth_min, fmax(p->s->hmin, (++j) * p->s->dz));
	#if DEBUG_MESH_HMIN
		fprintf(stderr,"MH j=%d hmin="FWL"\n",j,hmin);
		fprintf(stderr,"mesh_hmin: end\n");
	#endif
	return hmin;
}

#if INLINE_MESH_HMIN
	#define mesh_hmin _mesh_hmin
#else
	JBDOUBLE mesh_hmin(Parameters*);
#endif

static inline void _mesh_actualize(Parameters *p,int n)
{
	int i;
	Parameters *pv;
	#if DEBUG_MESH_ACTUALIZE
		fprintf(stderr,"mesh_actualize: start\n");
	#endif
	for (i=n+1, pv=p; --i>=0; ++pv)
	{
		#if DEPTH_MIN
			pv->hmin = fmax(sys->depth_min, pv->s->hmin);
		#else
			pv->hmin = mesh_hmin(pv);
		#endif
		pv->zmin = pv->s->sp->z + pv->hmin;
		pv->Amin = section_area(pv->s, pv->zmin);
	}
	for (i=n, pv-=2; --i>=0; --pv)
	{
		pv->zsm = fmax((pv+1)->zmin, pv->zmin);
		pv->dzb = (pv+1)->s->sp->z - pv->s->sp->z;
	}
	#if DEBUG_MESH_ACTUALIZE
		for (i=0; i<=n; ++i)
		{
			fprintf(stderr,"MA i=%d z="FWF" s->hmin="FWF" s->zmin="FWF"\n",
				i,p[i].s->sp->z,p[i].s->hmin,p[i].s->zmin);
			fprintf(stderr,"MA i=%d hmin="FWF" zmin="FWF" Amin="FWF"\n",
				i,p[i].hmin,p[i].zmin,p[i].Amin);
		}
		for (i=0; i<n; ++i)
			fprintf(stderr,"MA i=%d zsm="FWF" dzb="FWF"\n",
				i,p[i].zsm,p[i].dzb);
		fprintf(stderr,"mesh_actualize: end\n");
	#endif
}

#if INLINE_MESH_ACTUALIZE
	#define mesh_actualize _mesh_actualize
#else
	void mesh_actualize(Parameters*,int);
#endif

static inline void _initial_variables_open
(
	Parameters *p,TransportParameters *tp,InitialFlow *ifc,
	ChannelTransport *ct,int nif,int *nit,int nt
)
{
	int i;
	register JBDOUBLE r;
	InitialTransport *it;
	#if DEBUG_INITIAL_VARIABLES_OPEN
		fprintf(stderr,"init_variables_open: start\n");
		fprintf(stderr,"IVO fic type=%d\n",ifc->type);
	#endif
	switch (ifc->type)
	{
	case INITIAL_FLOW_STEADY:
	case INITIAL_FLOW_DRY:
		p->A = p->Q = 0.;
		break;
	default:
		#if DEBUG_INITIAL_VARIABLES_OPEN
			fprintf(stderr,"IVO nif=%d n=%d\n",nif,ifc->n);
		#endif
		if (nif<0)
		{
			p->Q = ifc->p[0].y;
			p->h = ifc->p[0].z;
		}
		else if (nif == ifc->n)
		{
			p->Q = ifc->p[nif].y;
			p->h = ifc->p[nif].z;
		}
		else
		{
			r = (p->x - ifc->p[nif].x) / (ifc->p[nif+1].x - ifc->p[nif].x);
			p->Q = ifc->p[nif].y + r * (ifc->p[nif+1].y - ifc->p[nif].y);
			p->h = ifc->p[nif].z + r * (ifc->p[nif+1].z - ifc->p[nif].z);
		}
		#if DEBUG_INITIAL_VARIABLES_OPEN
			fprintf(stderr,"IVO x="FWF" x1="FWF" x2="FWF"\n",
				p->x, ifc->p[nif].x, ifc->p[nif+1].x);
			fprintf(stderr,"IVO h="FWF"\n",p->h);
		#endif
		if (ifc->type == INITIAL_FLOW_XQH)
			p->A = section_area_with_depth(p->s, p->h);
		else if (ifc->type == INITIAL_FLOW_XQZ)
			p->A = section_area(p->s, p->h);
		else if (ifc->type == INITIAL_FLOW_XUH)
		{
			p->A = section_area_with_depth(p->s, p->h);
			p->Q *= p->A;
		}
		else
		{
			p->A = section_area(p->s, p->h);
			p->Q *= p->A;
		}
		#if DEBUG_INITIAL_VARIABLES_OPEN
			fprintf(stderr,"IVO Q="FWF" A="FWF"\n",p->Q,p->A);
		#endif
	}
	p->V = p->A * p->dx;
	p->ta = p->tr = 0.;
	
	#if DEBUG_INITIAL_VARIABLES_OPEN
		fprintf(stderr,"IVO opening transport initial conditions\n");
		fprintf(stderr,"IVO nt=%d\n",nt);
	#endif
	for (i=0; i<=nt; ++i, ++ct, tp+=n+1)
	{
		it = ct->it;
		#if DEBUG_INITIAL_VARIABLES_OPEN
			fprintf(stderr,"IVO i=%d type=%d\n",i,it->type);
		#endif
		switch (it->type)
		{
		case INITIAL_TRANSPORT_STEADY:
		case INITIAL_TRANSPORT_DRY:
			tp->c = 0.;
			break;
		default:
			#if DEBUG_INITIAL_VARIABLES_OPEN
				fprintf(stderr,"IVO nit=%d n=%d\n",nit[i],it->n);
			#endif
			if (nit[i]<0) tp->c = it->p[0].y;
			else if (nit[i] == it->n) tp->c = it->p[it->n].y;
			else tp->c = it->p[nit[i]].y + (p->x - it->p[nit[i]].x) /
				(it->p[nit[i]+1].x - it->p[nit[i]].x) *
				(it->p[nit[i]+1].y - it->p[nit[i]].y);
		}
		tp->b = 0.;
		tp->m = p->V * tp->c;
	}

	#if DEBUG_INITIAL_VARIABLES_OPEN
		fprintf(stderr,"init_variables_open: end\n");
	#endif
}

#if INLINE_INITIAL_VARIABLES_OPEN
	#define initial_variables_open _initial_variables_open
#else
	void initial_variables_open(Parameters*,TransportParameters*,InitialFlow*,
		ChannelTransport*,int,int*,int);
#endif

static inline int _part_open(int thread)
{
	int i,i2,j,k,nif,*nit;
	JBFLOAT *x;
	CrossSection *cs1,*cs2;
	InitialFlow *ifc;
	InitialTransport *it;
	ChannelTransport *ct;
	Channel *channel;
	ChannelSegment seg[1];
	Parameters *pv;
	TransportParameters *tpv;
	#if DEBUG_PART_OPEN
		fprintf(stderr,"part_open: start\n");
	#endif
	i=cell_thread[thread];
	i2=cell_thread[thread+1];
	#if DEBUG_PART_OPEN
		fprintf(stderr,"PO i=%d i2=%d\n",i,i2);
		fprintf(stderr,"PO channel=%d\n",p[i].nchannel);
	#endif
	for (j=i, pv=p+i; j<i2; ++j, ++pv) pv->s->sp = NULL, pv->s->fp = NULL;
	pv=p+i;
	channel = pv->channel;
	#if DEBUG_PART_OPEN
		fprintf(stderr,"PO channel=%d nt=%d\n",pv->nchannel,channel->nt);
	#endif

	if (channel->nt >= 0)
	{
		nit = (int*)g_try_malloc((channel->nt + 1) * sizeof(int));
		if (!nit)
		{
			jbw_show_error2(gettext("Transport initial conditions"),
				gettext("Not enough memory"));
			goto exit2;
		}
	}

	#if DEBUG_PART_OPEN
		fprintf(stderr,"PO nsegment=%d\n",pv->nsegment);
	#endif
	cs1 = channel->cg->cs + pv->nsegment;
	cs2=cs1+1;
	x = channel->cg->x + pv->nsegment;
	#if DEBUG_PART_OPEN
		fprintf(stderr,"PO opening channel segment\n");
	#endif
	if (!channel_segment_open(seg,cs1->ts,cs2->ts,x[0],x[1])) goto exit2;
	
	ifc = channel->fic;
	pv=p+i;
	tpv=tp+i;
	nif=-1;
	for (k=0; k<=channel->nt; ++k) nit[k]=-1;
	#if DEBUG_PART_OPEN
		fprintf(stderr,"PO opening initial conditions\n");
	#endif
	for (j=i; j<i2; ++j, ++pv, ++tpv)
	{
		#if DEBUG_PART_OPEN
			fprintf(stderr,"PO i=%d j=%d\n",i,j);
			fprintf(stderr,"PO i1=%d i2=%d\n",cs1->i,cs2->i);
		#endif
		if (j > cs2->i)
		{
			#if DEBUG_PART_OPEN
				fprintf(stderr,"PO deleting channel segment\n");
			#endif
			channel_segment_delete(seg);
			#if DEBUG_PART_OPEN
				fprintf(stderr,"PO locating channel segment\n");
			#endif
			if (j > channel->i2)
			{
				++channel;
				cs1 = channel->cg->cs;
				cs2=cs1+1;
				x = channel->cg->x;
				ifc = channel->fic;
				ct = channel->ct;
				nif=-1;
				for (k=0; k<=channel->nt; ++k) nit[k]=-1;
			}
			else
			{
				++cs1, ++cs2;
				++x;
			}
			#if DEBUG_PART_OPEN
				fprintf(stderr,"PO opening channel segment\n");
			#endif
			if (!channel_segment_open(seg,cs1->ts,cs2->ts,x[0],x[1]))
				goto exit2;
			#if DEBUG_PART_OPEN
				fprintf(stderr,"PO copying section\n");
			#endif
		}
		if (j == cs1->i)
		{
			#if DEBUG_PART_OPEN
				fprintf(stderr,"PO copying section\n");
			#endif
			if (!section_copy(pv->s,seg->s1)) goto exit1;
		}
		else if (j == cs2->i)
		{
			#if DEBUG_PART_OPEN
				fprintf(stderr,"PO copying section\n");
			#endif
			if (!section_copy(pv->s,seg->s2)) goto exit1;
		}
		else
		{
			#if DEBUG_PART_OPEN
				fprintf(stderr,"PO channel segment open section i=%d\n",i);
				fprintf(stderr,"PO x1="FWF" x2="FWF"\n",seg->x1,seg->x2);
				fprintf(stderr,"PO x="FWL"\n",pv->x);
			#endif
			if (!channel_segment_open_section(seg,pv->s,pv->x)) goto exit1;
		}
		pv->channel = channel;
		#if DEBUG_PART_OPEN
			fprintf(stderr,"PO locating flow initial conditions\n");
			fprintf(stderr,"PO fic type=%d\n",ifc->type);
		#endif
		switch (ifc->type)
		{
		case INITIAL_FLOW_STEADY:
		case INITIAL_FLOW_DRY:
			break;
		default:
			while (nif<ifc->n && pv->x > ifc->p[nif+1].x) ++nif;
			#if DEBUG_PART_OPEN
				fprintf(stderr,"PO nif=%d n=%d\n",nif,ifc->n);
			#endif
		}

		#if DEBUG_PART_OPEN
			fprintf(stderr,"PO locating transport initial conditions\n");
		#endif
		for (k=0, ct=channel->ct; k<=channel->nt; ++k, ++ct)
		{
			it = ct->it;
			#if DEBUG_PART_OPEN
				fprintf(stderr,"PO channel=%d tic type=%d\n",k,it->type);
			#endif
			switch (it->type)
			{
			case INITIAL_TRANSPORT_STEADY:
			case INITIAL_TRANSPORT_DRY:
				break;
			default:
				while (nit[k]<it->n && pv->x > it->p[nit[k]+1].x) ++nit[k];
				#if DEBUG_PART_OPEN
					fprintf(stderr,"PO nit=%d n=%d\n",nit[k],it->n);
				#endif
			}
		}

		initial_variables_open(pv,tpv,ifc,channel->ct,nif,nit,channel->nt);
		#if DEPTH_MIN
			pv->hmin = fmax(sys->depth_min, pv->s->hmin);
		#else
			pv->hmin = mesh_hmin(pv);
		#endif
		pv->zmin = pv->s->sp->z + pv->hmin;
		pv->Amin = section_area(pv->s, pv->zmin);
		#if DEBUG_PART_OPEN
			fprintf(stderr,"PO j=%d z="FWF" hmin="FWF" zmin="FWF"\n",
				j,pv->s->sp->z,pv->hmin,pv->zmin);
		#endif
		#if JBW==JBW_GTK
			#if DEBUG_PART_OPEN
				fprintf(stderr,"PO actualizing dialog\n");
			#endif
			g_mutex_lock(mutex);
			++n_open_sections;
			dialog_open_parameters_actualize();
			g_mutex_unlock(mutex);
		#endif
	}
	#if DEBUG_PART_OPEN
		fprintf(stderr,"PO destroying channel segment p=%ld\n",(size_t)p);
	#endif
	channel_segment_delete(seg);
	pv=p+i;
	channel = pv->channel;
	for (j=i, --i2; j<i2; ++j, ++pv)
	{
		#if DEBUG_PART_OPEN
			fprintf(stderr,"PO j=%d ci=%d ci2=%d\n",j,channel->i,channel->i2);
		#endif
		if (j < channel->i2) 
		{
			pv->zsm = fmax((pv+1)->zmin, pv->zmin);
			#if DEBUG_PART_OPEN
				fprintf(stderr,"PO zsm="FWF"\n",pv->zsm);
			#endif
			pv->dzb = (pv+1)->s->sp->z - pv->s->sp->z;
			#if DEBUG_PART_OPEN
				fprintf(stderr,"PO dzb="FWF"\n",pv->dzb);
			#endif
		}
		else ++channel;
	}
	#if DEBUG_PART_OPEN
		fprintf(stderr,"part_open: end\n");
	#endif
	return 1;

exit1:
	#if DEBUG_PART_OPEN
		fprintf(stderr,"PO destroying channel segment\n");
	#endif
	channel_segment_delete(seg);
exit2:
	#if DEBUG_PART_OPEN
		fprintf(stderr,"part_open: end\n");
	#endif
	return 0;
}

#if INLINE_PART_OPEN
	#define part_open _part_open
#else
	int part_open(int);
#endif

static inline void _part_open_exit(int thread)
{
	int i;
	#if DEBUG_PART_OPEN_EXIT
		fprintf(stderr,"part_open_exit: start\n");
	#endif
	i=part_open(thread);
	g_thread_exit((void*)(size_t)i);
	#if DEBUG_PART_OPEN_EXIT
		fprintf(stderr,"part_open_exit: end\n");
	#endif
}

#if INLINE_PART_OPEN_EXIT
	#define part_open_exit _part_open_exit
#else
	void part_open_exit(int);
#endif

static inline void _sort_int(int *x,int *ni,int n)
{
	int i,j,k,xk;
	#if DEBUG_SORT_INT
		fprintf(stderr,"sort_int: start\n");
	#endif
	for (i=0; ++i<=n;)
	{
		k=ni[i];
		xk=x[k];
		for (j=i; --j>=0 && x[ni[j]]<xk;) ni[j+1]=ni[j];
		ni[j+1]=k;
	}
	#if DEBUG_SORT_INT
		fprintf(stderr,"sort_int: end\n");
	#endif
}

#if INLINE_SORT_INT
	#define sort_int _sort_int
#else
	void sort_int(int*,int*,int);
#endif

static inline void _sort_inverse_int(int *x,int *ni,int n)
{
	int i,j,k,xk;
	#if DEBUG_SORT_INVERSE_INT
		fprintf(stderr,"sort_inverse_int: start\n");
	#endif
	for (i=0; ++i<=n;)
	{
		k=ni[i];
		xk=x[k];
		for (j=i; --j>=0 && x[ni[j]]>xk;) ni[j+1]=ni[j];
		ni[j+1]=k;
	}
	#if DEBUG_SORT_INVERSE_INT
		fprintf(stderr,"sort_inverse_int: end\n");
	#endif
}

#if INLINE_SORT_INVERSE_INT
	#define sort_inverse_int _sort_inverse_int
#else
	void sort_inverse_int(int*,int*,int);
#endif

static inline void _system_list()
{
	int i, j, n[sys->n + 1], ni[sys->n + 1], n2[nth], nj[nth];
	ChannelGeometry *cg;
	#if DEBUG_SYSTEM_LIST
		GSList *list;
		fprintf(stderr,"system_list: start\n");
	#endif
	for (i=0; i<nth; ++i)
	{
		if (list_channels[i]) g_slist_free(list_channels[i]);
		list_channels[i]=NULL;
	}
	for (i=0; i<=sys->n; ++i)
	{
		ni[i]=i;
		cg = sys->channel[i].cg;
		n[i] = cg->cs[cg->n - 1].i;
	}
	sort_int(n, ni, sys->n);
	for (i=0; i<nth; ++i)
	{
		n2[i]=0;
		nj[i]=i;
	}
	for (j=0; 1;)
	{
		for (i=0; i<nth; ++i)
		{
			#if DEBUG_SYSTEM_LIST
				fprintf(stderr,"SL i=%d j=%d nj=%d channel=%d\n",
					i,j,nj[i],ni[j]);
			#endif
			list_channels[nj[i]] =
				g_slist_prepend(list_channels[nj[i]], sys->channel[ni[j]].cg);
			n2[nj[i]] += n[ni[j]];
			if (++j > sys->n) goto exit_list;
		}
		sort_inverse_int(n2,nj,nth-1);
	}
exit_list:
	#if DEBUG_SYSTEM_LIST
		for (i=0; i<nth; ++i)
		{
			fprintf(stderr,"SL list=%d\n",i);
			for (list=list_channels[i]; list; list=list->next)
				fprintf(stderr,"SL channel=%s\n",
					p[((ChannelGeometry*)list->data)->cs->i].channel->name);
		}
		fprintf(stderr,"system_list: end\n");
	#endif
	return;
}

#if INLINE_SYSTEM_LIST
	#define system_list _system_list
#else
	void system_list();
#endif

static inline void _junction_print(Junction *junction,FILE *file)
{
	fprintf(file,"junction_print: start\n");
	fprintf(file,"JP nparameters=%d nvolumes=%d ninlets=%d noutlets=%d\n",
		junction->nparameters, junction->nvolumes, junction->ninlets,
		junction->noutlets);
	fprintf(file,"junction_print: end\n");
}

#if INLINE_JUNCTION_PRINT
	#define junction_print _junction_print
#else
	void junction_print(Junction*,FILE*);
#endif

static inline void _junction_delete(Junction *junction)
{
	#if DEBUG_JUNCTION_DELETE
		fprintf(stderr,"junction_delete: start\n");
	#endif
	junction->nparameters = junction->nvolumes = junction->ninlets =
		junction->noutlets = -1;
	jb_free_null((void**)&junction->parameter);
	jb_free_null((void**)&junction->transport);
	jb_free_null((void**)&junction->inlet);
	jb_free_null((void**)&junction->outlet);
	jb_free_null((void**)&junction->angle);
	jb_free_null((void**)&junction->mass);
	jb_free_null((void**)&junction->volume);
	#if DEBUG_JUNCTION_DELETE
		fprintf(stderr,"junction_delete: end\n");
	#endif
}

#if INLINE_JUNCTION_DELETE
	#define junction_delete _junction_delete
#else
	void junction_delete(Junction*);
#endif

static inline int _junction_create(Junction *junction)
{
	#if DEBUG_JUNCTION_CREATE
		fprintf(stderr,"junction_create: start\n");
	#endif
	junction->nparameters = junction->nvolumes = junction->ninlets =
		junction->noutlets = -1;
	junction->parameter = NULL;
	junction->transport = NULL;
	junction->inlet = junction->outlet = NULL;
	junction->angle = NULL;
	junction->volume = NULL;
	junction->mass = (JBFLOAT*)g_try_malloc((sys->nt + 2) * sizeof(JBFLOAT));
	if (!junction->mass)
	{
		jbw_show_error2(gettext("Junction"),gettext("Not enough memory"));
		#if DEBUG_JUNCTION_CREATE
			fprintf(stderr,"junction_create: end\n");
		#endif
		return 0;
	}
	#if DEBUG_JUNCTION_CREATE
		fprintf(stderr,"junction_create: end\n");
	#endif
	return 1;
}

#if INLINE_JUNCTION_CREATE
	#define junction_create _junction_create
#else
	int junction_create(Junction*);
#endif

static inline int _junction_add_parameter(Junction *junction,
	Parameters *parameter,TransportParameters *transport,JBFLOAT angle)
{
	#if DEBUG_JUNCTION_ADD_PARAMETER
		fprintf(stderr,"junction_add_parameter: start\n");
	#endif
	++junction->nparameters;
	#if DEBUG_JUNCTION_ADD_PARAMETER
		fprintf(stderr,"JAD nparameters=%d\n",junction->nparameters);
		fprintf(stderr,"JAD allocating parameters\n");
	#endif
	junction->parameter = (Parameters**)jb_realloc(junction->parameter,
		(junction->nparameters + 1) * sizeof(Parameters*));
	junction->transport = (TransportParameters**)jb_realloc(junction->transport,
		(junction->nparameters + 1) * sizeof(TransportParameters*));
	#if DEBUG_JUNCTION_ADD_PARAMETER
		fprintf(stderr,"JAD allocating angles\n");
	#endif
	junction->angle = (JBFLOAT*)jb_realloc
		(junction->angle, (junction->nparameters + 1) * sizeof(JBFLOAT));
	if (!junction->parameter || !junction->transport || !junction->angle)
	{
		jbw_show_error2(gettext("Junctions"),gettext("Not enough memory"));
		#if DEBUG_JUNCTION_ADD_PARAMETER
			fprintf(stderr,"junction_add_parameter: end\n");
		#endif
		return 0;
	}
	#if DEBUG_JUNCTION_ADD_PARAMETER
		fprintf(stderr,"JAD saving parameter\n");
	#endif
	junction->parameter[junction->nparameters] = parameter;
	junction->transport[junction->nparameters] = transport;
	#if DEBUG_JUNCTION_ADD_PARAMETER
		fprintf(stderr,"JAD parameter=%ld %ld\n",(size_t)parameter,
			(size_t)junction->parameter[junction->nparameters]);
		fprintf(stderr,"JAD saving angle\n");
	#endif
	junction->angle[junction->nparameters] = angle;
	#if DEBUG_JUNCTION_ADD_PARAMETER
		fprintf(stderr,"junction_add_parameter: end\n");
	#endif
	return 1;
}

#if INLINE_JUNCTION_ADD_PARAMETER
	#define junction_add_parameter _junction_add_parameter
#else
	int junction_add_parameter
		(Junction*,Parameters*,TransportParameters*,JBFLOAT);
#endif

static inline int _junction_add_inlet(Junction *junction,Channel *channel)
{
	int i;
	CrossSection *cs;
	#if DEBUG_JUNCTION_ADD_INLET
		fprintf(stderr,"junction_add_inlet: start\n");
	#endif
	junction->type = JUNCTION_TYPE_FRONTAL;
	#if DEBUG_JUNCTION_ADD_INLET
		fprintf(stderr,"JAI type=%d\n",junction->type);
	#endif
	++junction->ninlets;
	#if DEBUG_JUNCTION_ADD_INLET
		fprintf(stderr,"JAI ninlets=%d\n",junction->ninlets);
	#endif
	junction->inlet =
		jb_realloc(junction->inlet, (junction->ninlets + 1) * sizeof(Channel*));
	if (!junction->inlet)
	{
		jbw_show_error2(gettext("Junctions"),gettext("Not enough memory"));
		#if DEBUG_JUNCTION_ADD_INLET
			fprintf(stderr,"junction_add_inlet: end\n");
		#endif
		return 0;
	}
	junction->inlet[junction->ninlets] = channel;
	cs = channel->cg->cs;
	i = junction_add_parameter(junction, p + channel->i, tp + channel->i,
		atan(((cs+1)->y - cs->y) / ((cs+1)->x - cs->x)));
	#if DEBUG_JUNCTION_ADD_INLET
		fprintf(stderr,"junction_add_inlet: end\n");
	#endif
	return i;
}

#if INLINE_JUNCTION_ADD_INLET
	#define junction_add_inlet _junction_add_inlet
#else
	int junction_add_inlet(Junction*,Channel*);
#endif

static inline int _junction_add_outlet(Junction *junction,Channel *channel)
{
	int i;
	CrossSection *cs;
	#if DEBUG_JUNCTION_ADD_OUTLET
		fprintf(stderr,"junction_add_outlet: start\n");
	#endif
	junction->type = JUNCTION_TYPE_FRONTAL;
	#if DEBUG_JUNCTION_ADD_OUTLET
		fprintf(stderr,"JAO type=%d\n",junction->type);
	#endif
	++junction->noutlets;
	#if DEBUG_JUNCTION_ADD_OUTLET
		fprintf(stderr,"JAO noutlets=%d\n",junction->noutlets);
	#endif
	junction->outlet = jb_realloc
		(junction->outlet, (junction->noutlets + 1) * sizeof(Channel*));
	if (!junction->outlet)
	{
		jbw_show_error2(gettext("Junctions"),gettext("Not enough memory"));
		#if DEBUG_JUNCTION_ADD_OUTLET
			fprintf(stderr,"junction_add_outlet: end\n");
		#endif
		return 0;
	}
	junction->outlet[junction->noutlets] = channel;
	cs = channel->cg->cs + channel->cg->n;
	i = junction_add_parameter(junction, p + channel->i2, tp + channel->i2,
		atan(((cs-1)->y - cs->y) / ((cs-1)->x - cs->x)));
	#if DEBUG_JUNCTION_ADD_OUTLET
		fprintf(stderr,"junction_add_outlet: end\n");
	#endif
	return i;
}

#if INLINE_JUNCTION_ADD_OUTLET
	#define junction_add_outlet _junction_add_outlet
#else
	int junction_add_outlet(Junction*,Channel*);
#endif

static inline int _junction_add_tributary
	(Junction *junction,Channel *channel,int pos,int pos2)
{
	int i,j;
	JBDOUBLE angle;
	CrossSection *cs;
	ChannelGeometry *cg;
	#if DEBUG_JUNCTION_ADD_TRIBUTARY
		fprintf(stderr,"junction_add_tributary: start\n");
	#endif
	junction->type = JUNCTION_TYPE_FRONTAL;
	cg = channel->cg;
	#if DEBUG_JUNCTION_ADD_TRIBUTARY
		fprintf(stderr,"JAT pos=%d pos2=%d i=%d i2=%d\n",
			pos,pos2,cg->cs[pos].i,cg->cs[pos2].i);
	#endif
	for (j=pos, cs=cg->cs+j, i=cs->i; j<=pos2;)
	{
		#if DEBUG_JUNCTION_ADD_TRIBUTARY
			fprintf(stderr,"JAT i=%d j=%d\n",i,j);
		#endif
		if (i == cg->cs->i)
			angle = atan(((cs+1)->y - cs->y) / ((cs+1)->x - cs->x));
		else if (i == cg->cs[cg->n].i)
			angle = atan(((cs-1)->y - cs->y) / ((cs-1)->x - cs->x));
		else
			angle = atan(((cs+1)->y - (cs-1)->y) / ((cs+1)->x - (cs-1)->x));
		#if DEBUG_JUNCTION_ADD_TRIBUTARY
			fprintf(stderr,"JAT angle="FWF"\n",angle);
		#endif
		if (!junction_add_parameter(junction,p+i,tp+i,angle))
		{
			#if DEBUG_JUNCTION_ADD_TRIBUTARY
				fprintf(stderr,"junction_add_tributary: end\n");
			#endif
			return 0;
		}
		if (j==pos2) break;
		if (++i == (cs+1)->i)
		{
			++j;
			++cs;
		}
	}
	#if DEBUG_JUNCTION_ADD_TRIBUTARY
		fprintf(stderr,"junction_add_tributary: end\n");
	#endif
	return 1;
}

#if INLINE_JUNCTION_ADD_TRIBUTARY
	#define junction_add_tributary _junction_add_tributary
#else
	int junction_add_tributary(Junction*,Channel*,int,int);
#endif

static inline int _junction_set_volumes(Junction *junction)
{
	int i,j,k,ii[junction->nparameters+1];
	JBFLOAT zmin;
	JBDOUBLE B,dBz,dz;
	SectionParameters *sp;
	Parameters *parameter;
	JunctionVolume *jv;
	#if DEBUG_JUNCTION_SET_VOLUMES
		fprintf(stderr,"junction_set_volumes: start\n");
		junction_print(junction,stderr);
		for (i=0; i<=junction->nparameters; ++i)
		{
			fprintf(stderr,"JSV i=%d dx="FWF"\n",i,junction->parameter[i]->dx);
			section_print(junction->parameter[i]->s,stderr);
		}
	#endif
	for (ii[0]=i=j=0; i<=junction->nparameters; ++i)
	{
		j += junction->parameter[i]->s->n + 1;
		ii[i]=0;
	}
	junction->volume =
		(JunctionVolume*)g_try_malloc(j * sizeof(JunctionVolume));
	if (!junction->volume)
	{
		jbw_show_error2
			(gettext("Volume of junctions"),gettext("Not enough memory"));
		#if DEBUG_JUNCTION_SET_VOLUMES
			fprintf(stderr,"junction_set_volumes: end\n");
		#endif
		return 0;
	}
	#if DEBUG_JUNCTION_SET_VOLUMES
		fprintf(stderr,"JSV nvolumes=%d\n",j);
	#endif
	j=0;
	jv = junction->volume;
	do
	{
		#if DEBUG_JUNCTION_SET_VOLUMES
			fprintf(stderr,"JSV j=%d\n",j);
		#endif
		for	(i=k=0; i<=junction->nparameters; ++i)
		{
			parameter = junction->parameter[i];
			sp = parameter->s->sp + ii[i];
			if (ii[i] <= parameter->s->n)
			{
				#if DEBUG_JUNCTION_SET_VOLUMES
					fprintf(stderr, "JSV i=%d ii=%d z="FWF"\n",
						i, ii[i], sp->z);
				#endif
				if (!k) zmin = sp->z; else zmin = fmin(zmin, sp->z);
				k=1;
			}
		}
		if (!k) break;
		#if DEBUG_JUNCTION_SET_VOLUMES
			fprintf(stderr,"JSV zmin="FWF"\n",zmin);
		#endif
		jv->z = zmin;
		jv->V = jv->A = jv->dAz = 0.;
		for (i=0; i<=junction->nparameters; ++i)
		{
			#if DEBUG_JUNCTION_SET_VOLUMES
				fprintf(stderr,"JSV i=%d ii=%d n=%d\n",i,ii[i],parameter->s->n);
				if (ii[i]>0)
					fprintf(stderr,"JSV z1="FWF" z2="FWF"\n",sp->z,(sp-1)->z);
			#endif
			parameter = junction->parameter[i];
			sp = parameter->s->sp + ii[i];
			if (ii[i] == parameter->s->n)
			{
				B = sp->B;
				dBz = 0.;
			}
			else if (ii[i] == 0)
			{
				if (sp->z == zmin)
				{
					B = sp->B;
					dBz = sp->dBz;
				}
			}
			else if (sp->z > zmin)
			{
				B = jbm_extrapolate(zmin, sp->z, (sp-1)->z, sp->B, (sp-1)->B);
				dBz = (sp-1)->dBz;
			}
			else if (sp->z == zmin)
			{
				B = sp->B;
				dBz = sp->dBz;
			}
			#if DEBUG_JUNCTION_SET_VOLUMES
				fprintf(stderr,"JSV B="FWL" dBz="FWL"\n",B,dBz);
				fprintf(stderr, "JSV A="FWF" dx="FWF"\n", sp->A, parameter->dx);
			#endif
			jv->A += B * parameter->dx;
			jv->dAz += dBz * parameter->dx;
			if (sp->z == zmin) ++ii[i];
		}
		if (j>0)
		{
			dz = zmin - (jv-1)->z;
			jv->V = (jv-1)->V + dz * ((jv-1)->A + 0.5 * dz * (jv-1)->dAz);
		}
		#if DEBUG_JUNCTION_SET_VOLUMES
			fprintf(stderr, "JSV j=%d z="FWF" V="FWF"\n", j, jv->z, jv->V);
			fprintf(stderr, "JSV A="FWF" dAz="FWF"\n", jv->A, jv->dAz);
		#endif
		++j; ++jv;
	}
	while (1);
	junction->volume = (JunctionVolume*)
		jb_realloc(junction->volume, j * sizeof(JunctionVolume));
	if (!junction->volume)
	{
		jbw_show_error2
			(gettext("Volume of junctions"),gettext("Not enough memory"));
		#if DEBUG_JUNCTION_SET_VOLUMES
			fprintf(stderr,"junction_set_volumes: end\n");
		#endif
		return 0;
	}
	junction->nvolumes = --j;
	#if DEBUG_JUNCTION_SET_VOLUMES
		for (i=0; i<=junction->nvolumes; ++i)
		{
			jv = junction->volume + i;
			fprintf(stderr, "JSV i=%d z="FWF" V="FWF"\n", i, jv->z, jv->V);
			fprintf(stderr, "JSV A="FWF" dAz="FWF"\n", jv->A, jv->dAz);
		}
	#endif
	#if DEBUG_JUNCTION_SET_VOLUMES
		fprintf(stderr,"junction_set_volumes: end\n");
	#endif
	return 1;
}

#if INLINE_JUNCTION_SET_VOLUMES
	#define junction_set_volumes _junction_set_volumes
#else
	int junction_set_volumes(Junction*);
#endif

static inline void _junctions_delete(Junction **junction,int *n)
{
	int i;
	#if DEBUG_JUNCTIONS_DELETE
		fprintf(stderr,"junctions_delete: start\n");
	#endif
	for (i=0; i<=*n; ++i) junction_delete(*junction + i);
	jb_free_null((void**)junction);
	*n = -1;
	#if DEBUG_JUNCTIONS_DELETE
		fprintf(stderr,"junctions_delete: end\n");
	#endif
}

#if INLINE_JUNCTIONS_DELETE
	#define junctions_delete _junctions_delete
#else
	void junctions_delete(Junction**,int*);
#endif

static inline int _junctions_open(Junction **junction,int *n)
{
	int i,j,k,l;
	JunctionData *jd;
	BoundaryFlow *bf;
	Channel *channel, *channel2;
	Junction *junction2;
	#if DEBUG_JUNCTIONS_OPEN
		fprintf(stderr,"junctions_open: start\n");
	#endif
	for (*n=-1, i=0, channel=sys->channel; i<=sys->n; ++i, ++channel)
	{
		for (j=0, bf=channel->bf; j<=channel->n; ++j, ++bf)
		{
			#if DEBUG_JUNCTIONS_OPEN
				fprintf(stderr,"JO type=%d junction=%d\n",
					bf->type,BOUNDARY_FLOW_JUNCTION);
			#endif
			if (bf->type == BOUNDARY_FLOW_JUNCTION)
			{
				jd=JUNCTION_DATA(bf,0);
				#if DEBUG_JUNCTIONS_OPEN
					fprintf(stderr,"JO j=%d n=%d pos=%d pos2=%d\n",j,channel->n,
						jd->pos,jd->pos2);
				#endif
				channel2 = sys->channel + jd->channel;
				#if DEBUG_JUNCTIONS_OPEN
					fprintf(stderr,"JO channel=%d n=%d\n",
						jd->channel,channel2->cg->n);
				#endif
				if ((!j || j == channel->n))
				{
					if (jd->pos2 > 0 && jd->pos < channel2->cg->n)
					{
						// TRIBUTARY JUNCTION
						#if DEBUG_JUNCTIONS_OPEN
							fprintf(stderr,
								"JO tributary junction njunctions=%d\n",*n);
						#endif
						junction2=(Junction*)
							g_try_realloc(*junction,(*n+2)*sizeof(Junction));
						if (!junction2)
						{
							jbw_show_error2(gettext("Tributary junctions"),
								gettext("Not enough memory"));
							goto exit1;
						}
						*junction=junction2;
						++(*n);
						if (!junction_create(*junction + *n)) goto exit1;
						if (!junction_add_tributary
							(*junction + *n, channel2, jd->pos, jd->pos2))
								goto exit1;
						if (!j)
						{
							if (!junction_add_inlet(*junction + *n, channel))
								goto exit1;
						}
						else if (!junction_add_outlet(*junction + *n, channel))
							goto exit1;
					}
					else
					{
						// FRONTAL JUNCTION
						// checking inlets
						#if DEBUG_JUNCTIONS_OPEN
							fprintf(stderr,
								"JO frontal junction njunctions=%d\n",*n);
						#endif
						if (!j) for (k=0; k<=*n; ++k)
						{
							junction2 = *junction + k;
							for (l=0; l<=junction2->ninlets; ++l)
							{
								#if DEBUG_JUNCTIONS_OPEN
									fprintf(stderr,
										"JO l=%d inlet1=%ld inlet2=%ld\n",
										l,
										(size_t)channel,
										(size_t)junction2->inlet[l]);
								#endif
								if (junction2->inlet[l] == channel) goto done;
							}
						}
						// checking outlets
						else for (k=0; k<=*n; ++k)
						{
							junction2 = *junction + k;
							for (l=0; l<=junction2->noutlets; ++l)
							{
								#if DEBUG_JUNCTIONS_OPEN
									fprintf(stderr,
										"JO l=%d outlet1=%ld outlet2=%ld\n",
										l,
										(size_t)channel,
										(size_t)junction2->outlet[l]);
								#endif
								if (junction2->outlet[l] == channel) goto done;
							}
						}
						#if DEBUG_JUNCTIONS_OPEN
							fprintf(stderr,"JO frontal junctions checked\n");
						#endif
						junction2=(Junction*)
							g_try_realloc(*junction,(*n+2)*sizeof(Junction));
						if (!junction2)
						{
							jbw_show_error2(gettext("Frontal junctions"),
								gettext("Not enough memory"));
							goto exit1;
						}
						*junction=junction2;
						++(*n);
						if (!junction_create(*junction + *n)) goto exit1;
						if (!j)
						{
							if (!junction_add_inlet(*junction + *n, channel))
								goto exit1;
						}
						else if (!junction_add_outlet(*junction + *n, channel))
							goto exit1;
						for (k=0; k<=JUNCTION_N(bf); ++k)
						{
							jd=JUNCTION_DATA(bf,k);
							channel2 = sys->channel + jd->channel;
							#if DEBUG_JUNCTIONS_OPEN
								fprintf(stderr,"JO channel=%d pos=%d\n",
									jd->channel,jd->pos);
							#endif
							if (!jd->pos)
							{
								if (!junction_add_inlet
									(*junction + *n, channel2))
										goto exit1;
							}
							else if (!junction_add_outlet
								(*junction + *n, channel2))
									goto exit1;
						}
						#if DEBUG_JUNCTIONS_OPEN
							junction_print(*junction + *n, stderr);
						#endif
					}
				}
				else
				{
					// PARALLEL JUNCTION (not yet implemented)
					jbw_show_error2(gettext("Parallel junction"),
						gettext("Not yet implemented"));
					goto exit1;
				}
done:
				continue;
			}
		}
	}
	for (i=0; i<=*n; ++i) if (!junction_set_volumes(*junction+i)) goto exit1;
	#if DEBUG_JUNCTIONS_OPEN
		fprintf(stderr,"junctions_open: end\n");
	#endif
	return 1;

exit1:
	junctions_delete(junction,n);
	#if DEBUG_JUNCTIONS_OPEN
		fprintf(stderr,"junctions_open: end\n");
	#endif
	return 0;
}

#if INLINE_JUNCTIONS_OPEN
	#define junctions_open _junctions_open
#else
	int junctions_open(Junction**,int*);
#endif

static inline void _mesh_delete(System *system,Parameters **p,
	TransportParameters **tp,Variables *v,Junction **junction,Mass **m,int *n,
	int *njunctions)
{
	int i;
	Channel *channel;
	#if DEBUG_MESH_DELETE
		fprintf(stderr,"mesh_delete: start\n");
	#endif
	if (*p)
	{
		for (i=*n+1; --i>=0;) section_delete((*p)[i].s);
		jb_free_null((void**)p);
	}
	jb_free_null((void**)tp);
	jb_free_null((void**)&v->x);
	if (*junction) junctions_delete(junction,njunctions);
	jb_free_null((void**)m);
	for (i=0, channel=system->channel; i<=system->n; ++i)
	{
		jb_slist_free(&channel->list_section_changes);
		jb_slist_free(&channel->list_section_gauge);
		jb_slist_free(&channel->list_variable_segments);
	}
	*n = *njunctions = -1;
	#if DEBUG_MESH_DELETE
		fprintf(stderr,"mesh_delete: end\n");
	#endif
}

#if INLINE_MESH_DELETE
	#define mesh_delete _mesh_delete
#else
	void mesh_delete(System*,Parameters**,TransportParameters**,Variables*,
		Junction**,Mass**,int*,int*);
#endif

static inline int _mesh_open
(
	System *system,Parameters **p,TransportParameters **tp,Variables *v,
	Junction **junction,Mass **m,int *n,int *njunctions,int *vsize
)
{
	int i,j,k,l;
	register JBDOUBLE r,dx;
	char *me;
	JBFLOAT *x,*li;
	GThread *thread[nth];
	CrossSection *cs;
	ChannelGeometry *cg;
	ChannelTransport *ct;
	BoundaryFlow *bf;
	BoundaryTransport *bt;
	Channel *channel;
	Parameters *pv;

	#if DEBUG_MESH_OPEN
		fprintf(stderr,"mesh_open: start\n");
		fprintf(stderr,"MO n=%d\n",system->n);
	#endif
	#if JBW==JBW_GTK
		#if DEBUG_MESH_OPEN
			fprintf(stderr,"MO actualizing dialog\n");
		#endif
		n_open_sections=0;
		dialog_open_parameters_actualize();
	#endif

	me=gettext("Not enough memory");

	for (k=0, *n=-1, *p=NULL, channel=system->channel; k<=system->n;
		++k, ++channel)
	{
		dx = channel->cell_size;
		++(*n);
		channel->i = *n;
		cg = channel->cg;
		cg->cs[0].i = channel->i;
		x = cg->x;
		li = (JBFLOAT*)g_try_malloc(cg->n * sizeof(JBFLOAT));
		if (!li) goto error3;
		for (i=cg->n; --i>=0;) li[i]=x[i+1]-x[i];
		#if DEBUG_MESH_OPEN
			fprintf(stderr,"MO dx="FWL"\n",dx);
			for (i=0; i<cg->n; ++i)
				fprintf(stderr,"MO i=%d xx="FWF" li="FWF"\n",i,x[i],li[i]);
			fprintf(stderr,"MO i=%d xx="FWF"\n",i,x[i]);
			fprintf(stderr,"MO nmin=%d cg=%ld cs=%ld\n",
				cg->n,(size_t)cg,(size_t)cg->cs);
		#endif
	#if MESH_BOUNDARY == MESH_BOUNDARY_UNIFORM
		if (cg->n == 1)
		{
			*n += j = jbm_max(2, ceill(li[0] / dx) - 1);
			i=j+1;
			#if DEBUG_MESH_OPEN
				fprintf(stderr,"MO n=%d j=%d\n",*n,j);
			#endif
			*p = (Parameters*)jb_realloc(*p, (*n + 2) * sizeof(Parameters));
			if (!*p) goto error4;
			pv = *p + *n + 1 - j;
			pv->x = x[0];
			pv->nchannel = k;
			pv->channel = channel;
			pv->nsegment = 0;
			pv = *p + *n + 1;
			r=li[0]/i;
			for (; --i>0; --pv)
			{
				pv->x = x[0] + i * r;
				pv->nchannel = k;
				pv->channel = channel;
				pv->nsegment = 0;
			}
		}
		else
		{
			*n += j = jbm_max(1, ceill(li[0] / dx - 0.5));
			#if DEBUG_MESH_OPEN
				fprintf(stderr,"MO n=%d j=%d\n",*n,j);
			#endif
			*p = (Parameters*)jb_realloc(*p, (*n + 1) * sizeof(Parameters));
			if (!*p) goto error4;
			pv = *p + *n + 1 - j;
			pv->x = x[0];
			pv->nchannel = k;
			pv->channel = channel;
			pv->nsegment = 0;
			r=li[0]/(j+0.5);
			for (pv=*p+*n; j>=0; --j, --pv)
			{
				pv->x = x[0] + j * r;
				pv->nchannel = k;
				pv->channel = channel;
				pv->nsegment = 0;
			}
			for (i=0; ++i<cg->n-1;)
			{
				cg->cs[i].i = *n;
				*n += j = jbm_max(1, ceill(li[i] / dx));
				*p = (Parameters*)jb_realloc(*p, (*n + 1) * sizeof(Parameters));
				if (!*p) goto error4;
				r=li[i]/j;
				for (pv=*p+*n; j>0; --j, --pv)
				{
					pv->x = x[i] + (j - 0.5) * r;
					pv->nchannel = k;
					pv->channel = channel;
					pv->nsegment = i;
				}
			}
			cg->cs[i].i = *n;
			*n += j = jbm_max(1, ceill(li[i] / dx - 0.5));
			*p = (Parameters*)jb_realloc(*p, (*n + 2) * sizeof(Parameters));
			if (!*p) goto error4;
			r=li[i]/(j+0.5);
			for (pv=*p+*n; j>0; --j, --pv)
			{
				pv->x = x[i] + (j - 0.5) * r;
				pv->nchannel = k;
				pv->channel = channel;
				pv->nsegment = i;
			}
		}
	#else
		if (cg->n == 1)
		{
			*n += j = jbm_max(2, ceill(li[0] / dx));
			*p = (Parameters*)jb_realloc(*p, (*n + 2) * sizeof(Parameters));
			if (!*p) goto error4;
			#if DEBUG_MESH_OPEN
				fprintf(stderr,"MO n=%d j=%d\n",*n,j);
			#endif
			pv = *p + *n - j;
			pv->x = x[0];
			pv->nchannel = k;
			pv->channel = channel;
			pv->nsegment = 0;
			r=li[0]/j;
			for (pv=*p+*n; j>0; --j, --pv)
			{
				pv->x = x[0] + (j - 0.5) * r;
				pv->nchannel = k;
				pv->channel = channel;
				pv->nsegment = 0;
			}
			#if DEBUG_MESH_OPEN
				for (i=0; i<=*n; ++i)
					fprintf(stderr,"MO i=%d x="FWF"\n",i,(*p)[i].x);
			#endif
		}
		else for (i=0; i<cg->n; ++i)
		{
			cg->cs[i].i = *n;
			*n += j = jbm_max(1, ceill(li[i] / dx));
			*p = (Parameters*)jb_realloc(*p, (*n + 2) * sizeof(Parameters));
			if (!*p) goto error4;
			if (i==0)
			{
				pv = *p + *n - j;
				pv->x = x[0];
				pv->nchannel = k;
				pv->channel = channel;
				pv->nsegment = 0;
			}
			r=li[i]/j;
			for (pv=*p+*n; j>0; --j, --pv)
			{
				pv->x = x[i] + (j - 0.5) * r;
				pv->nchannel = k;
				pv->channel = channel;
				pv->nsegment = i;
			}
		}
	#endif
		cs=cg->cs;
		channel->i2 = cs[cg->n].i = j = *n;
		#if DEBUG_MESH_OPEN
			for (i=0; i<=cg->n; ++i) fprintf(stderr,"MO cs i=%d\n",cs[i].i);
		#endif
		g_free(li);
		(*p)[j+1].x = x[cg->n];
		#if DEBUG_MESH_OPEN
			for (i=0; i<=j+1; ++i)
				fprintf(stderr,"MO i=%d x="FWF" channel=%d\n",
					i,(*p)[i].x,(*p)[i].nchannel);
		#endif
		for (bf=channel->bf, i=channel->n; i>=0; --i, ++bf)
		{
			bf->i = cs[bf->pos-1].i;
			bf->i2 = cs[bf->pos2-1].i;
		}
		#if DEBUG_MESH_OPEN
			bf = channel->bf;
			for (i=0; i<=channel->n; ++i)
				fprintf(stderr,"MO FBC i=%d\n",bf[i].i);
			fprintf(stderr,"MO nt=%d\n",channel->nt);
		#endif

		for (i=*n+1; --i>=channel->i;) (*p)[i].dx=(*p)[i+1].x-(*p)[i].x;
		++i;
		j=1;
		(*p)[i].x=x[0];
		while (++i<*n)
		{
			#if DEBUG_MESH_OPEN
				fprintf(stderr,"MO j=%d xx="FWF"\n",j,x[j]);
			#endif
			if (x[j] < (*p)[i+1].x)
			{
				(*p)[i].x = x[j];
				++j;
			}
			else (*p)[i].x = 0.5 * ((*p)[i+1].x + (*p)[i].x);
			#if DEBUG_MESH_OPEN
				fprintf(stderr,"MO i=%d x="FWF"\n",i,(*p)[i].x);
			#endif
		}
		(*p)[i].x=x[cg->n];

		for (ct=channel->ct, i=channel->nt; i>=0; --i, ++ct)
			for (bt=ct->bt, j=ct->n; j>=0; --j, ++bt)
			{
				bt->i = cs[bt->pos-1].i;
				bt->i2 = cs[bt->pos2-1].i;
				for (l=bt->i, bt->length=(*p)[l].dx; ++l<=bt->i2;)
					bt->length += (*p)[l].dx;
				#if DEBUG_MESH_OPEN
					fprintf(stderr,"MO TBC i=%d i2=%d length="FWF"\n",
						bt->i,bt->i2,bt->length);
				#endif
			}
		#if DEBUG_MESH_OPEN
			fprintf(stderr,"MO opening section changes\n");
		#endif
		if (!list_section_changes_open(&channel->list_section_changes,cg))
			goto error2;
		#if DEBUG_MESH_OPEN
			fprintf(stderr,"MO opening section gauges\n");
		#endif
		if (!list_section_gauge_open(&channel->list_section_gauge,cg))
			goto error2;
		#if DEBUG_MESH_OPEN
			fprintf(stderr,"MO opening variable segments\n");
		#endif
		if (!list_variable_segments_open(&channel->list_variable_segments,cg))
			goto error2;
	}

	if (!variables_create(v,*n,vsize,system->channel->nt)) goto error3;

	#if DEBUG_MESH_OPEN
		for (i=*n+1; --i>=0;)
			fprintf(stderr,"MO i=%d x="FWF" dx="FWF"\n",i,(*p)[i].x,(*p)[i].dx);
	#endif
#if MESH_BOUNDARY == MESH_BOUNDARY_UNIFORM
	for (i=*n; --i>=0;) (*p)[i].ix=0.5*((*p)[i+1].dx+(*p)[i].dx);
#else
	for (i=*n; --i>=0;) (*p)[i].ix=((*p)[i+1].x-(*p)[i].x);
#endif
	(*p)[*n].ix=(*p)[*n-1].ix;
	#if DEBUG_MESH_OPEN
		for (i=*n; --i>=0;) fprintf(stderr,"MO i=%d ix="FWF"\n",i,(*p)[i].ix);
	#endif
	*p = g_realloc(*p, (*n + 1) * sizeof(Parameters));

	#if DEBUG_MESH_OPEN
		fprintf(stderr,"MO allocating transport variables\n");
	#endif
	j = system->channel->nt + 1;
	*m = (Mass*)jb_realloc(*m, (j + 1) * sizeof(Mass));
	if (!*m) goto error3;
	if (j)
	{
		*tp = (TransportParameters*)
			g_try_malloc(j * (*n +  1) * sizeof(TransportParameters));
		if (!*tp) goto error3;
	}

	#if DEBUG_MESH_OPEN
		fprintf(stderr,"MO opening sections and variables\n");
	#endif
	nth = jbm_min(nthreads, *n);
	cell_thread=(int*)jb_realloc(cell_thread,(nth+1)*sizeof(int));
	if (!cell_thread) goto error3;
	for (i=nth+1; --i>=0;) cell_thread[i]=i*(*n+1)/nth;
	#if DEBUG_MESH_OPEN
		fprintf(stderr,"MO nthreads=%d\n",nth);
		for (i=0; i<=nth; ++i)
			fprintf(stderr,"MO i=%d cell_thread=%d\n",i,cell_thread[i]);
	#endif
	if (nth==1)
	{
		#if DEBUG_MESH_OPEN
			fprintf(stderr,"MO nthreads=%d\n",nth);
		#endif
		if (!part_open(0)) goto error2;
		j = *n - 1;
		pv = *p + j;
		pv->zsm = fmax((pv+1)->zmin, pv->zmin);
		#if DEBUG_MESH_OPEN
			fprintf(stderr,"MO zsm="FWF"\n",pv->zsm);
		#endif
		pv->dzb = fmax((pv+1)->s->sp->z, pv->s->sp->z);
		#if DEBUG_MESH_OPEN
			fprintf(stderr,"MO dzb="FWF"\n",pv->dzb);
		#endif
	}
  	else
	{
		for (i=nth; --i>=0;)
			thread[i] = g_thread_new
				(NULL, (void(*))part_open_exit, (void*)(size_t)i);
		for (i=nth, j=1; --i>=0;) j*=(size_t)g_thread_join(thread[i]);
		if (!j) goto error2;
		for (i=0; ++i<=nth;)
		{
			k=cell_thread[i]-1;
			pv = *p + k;
			channel = pv->channel;
			if (k < channel->i2)
			{
				pv->zsm = fmax((pv+1)->zmin, pv->zmin);
				#if DEBUG_MESH_OPEN
					fprintf(stderr,"MO k=%d zsm="FWF"\n",k,pv->zsm);
				#endif
				pv->dzb = fmax((pv+1)->s->sp->z, pv->s->sp->z);
				#if DEBUG_MESH_OPEN
					fprintf(stderr,"MO k=%d dzb="FWF"\n",k,pv->dzb);
				#endif
			}
		}
	}

	#if DEBUG_MESH_OPEN
		fprintf(stderr,"MO system list\n");
	#endif
	system_list();

	#if DEBUG_MESH_OPEN
		fprintf(stderr,"MO opening junctions\n");
	#endif
	if (!junctions_open(junction,njunctions)) goto error3;

	#if GUAD2D
// if (!receive_parameters_2D(...)) goto error3;
	#endif

	#if DEBUG_MESH_OPEN
		TransportParameters *tpv;
		for (i=0; i<=*n; ++i)
			fprintf(stderr,"MO i=%d x="FWF" Q="FWF" A="FWF" zb="FWF"\n",
				i,(*p)[i].x,(*p)[i].Q,(*p)[i].A,(*p)[i].s->sp->z);
		fprintf(stderr,"MO mass="FWL"\n",flow_mass(*p,*n));
		for (i=0, tpv=*tp; i<=system->channel->nt; ++i, tpv+=*n+1)
			fprintf(stderr,"MO solute mass="FWL"\n",transport_mass(tpv,*n));
		fprintf(stderr,"mesh_open: end\n");
	#endif
	return 1;

error4:
	g_free(li);

error3:
	jbw_show_error2(gettext("Mesh"),me);

error2:
	#if DEBUG_MESH_OPEN
		fprintf(stderr,"MO deleting mesh\n");
	#endif
	mesh_delete(system,p,tp,v,junction,m,n,njunctions);
	#if DEBUG_MESH_OPEN
		fprintf(stderr,"mesh_open: end\n");
	#endif
	return 0;
}

#if INLINE_MESH_OPEN
	#define mesh_open _mesh_open
#else
	int mesh_open(System*,Parameters**,TransportParameters**,Variables*,
		Junction**,Mass**,int*,int*,int*);
#endif

#endif
