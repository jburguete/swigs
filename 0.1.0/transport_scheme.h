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

#ifndef TRANSPORT_SCHEME__H
#define TRANSPORT_SCHEME__H 1

#include "diffusion_scheme.h"

extern JBDOUBLE dt2;

static inline void
	_transport_scheme_parameters_upwind(TransportParameters *tp,Parameters *p)
{
	#if DEBUG_TRANSPORT_SCHEME_PARAMETERS_UPWIND
		if (t>=DEBUG_TIME)
			fprintf(stderr,"transport_scheme_parameters_upwind: start\n");
	#endif
	#if TRANSPORT_DISCRETIZATION == TRANSPORT_DISCRETIZATION_SPLIT
		if (p->U > 0.) tp->dTp = tp->dT, tp->dTm = 0.;
		else tp->dTm = tp->dT, tp->dTp = 0.;
	#elif FLUX_LIMITER_DISCRETIZATION == \
				FLUX_LIMITER_DISCRETIZATION_CONSERVATIVE
		#if FLOW_LINEARIZATION == FLOW_LINEARIZATION_ROE
			if (p->sA2 > 0.) tp->S = (tp->csA + (tp+1)->csA) / p->sA2;
			else tp->S = 0.;
		#else
			tp->S = 0.5 * (tp->c + (tp+1)->c);
		#endif
		if (fmax((p+1)->zs,p->zs) <= p->zsm) tp->dTm = tp->dTp = 0.;
		else
		{
			if (p->U > 0.)
			{
				tp->dTm = p->dQm * tp->S;
				tp->dTp = tp->dT - tp->dTm;
			}
			else
			{
				tp->dTp = p->dQp * tp->S;
				tp->dTm = tp->dT - tp->dTp;
			}
		}
	#else
		register JBDOUBLE dW;
		#if FLOW_LINEARIZATION == FLOW_LINEARIZATION_ROE
			if (p->sA2 > 0.) tp->S = (tp->csA + (tp+1)->csA) / p->sA2;
			else tp->S = 0.;
			#if DEBUG_TRANSPORT_SCHEME_PARAMETERS_UPWIND
				if (t>=DEBUG_TIME)
					fprintf(stderr,"TSPU csA="FWF" csA2="FWF" sA2="FWF"\n",
						tp->csA,(tp)->csA,p->sA2);
			#endif
		#else
			tp->S = 0.5 * (tp->c + (tp+1)->c);
		#endif
		if (fmax((p+1)->zs,p->zs) <= p->zsm)
			tp->dTm = tp->dTp = tp->dWCm = tp->dWCp = 0.;
		else
		{
			dW = tp->dT - tp->S * p->dQ;
			if (p->U > 0.)
			{
				tp->dWCm = 0.;
				tp->dWCp = dW;
				tp->dTm = p->dQm * tp->S;
				tp->dTp = tp->dT - tp->dTm;
			}
			else
			{
				tp->dWCp = 0.;
				tp->dWCm = dW;
				tp->dTp = p->dQp * tp->S;
				tp->dTm = tp->dT - tp->dTp;
			}
		}
		#if DEBUG_TRANSPORT_SCHEME_PARAMETERS_UPWIND
			if (t>=DEBUG_TIME)
				fprintf(stderr,"TSPU S="FWF" dWCp="FWF" dWCm="FWF"\n",
					tp->S,tp->dWCp,tp->dWCm);
		#endif
	#endif
	#if DEBUG_TRANSPORT_SCHEME_PARAMETERS_UPWIND
		if (t>=DEBUG_TIME)
		{
			fprintf(stderr,"TSPU dTp="FWF" dTm="FWF"\n",tp->dTp,tp->dTm);
			fprintf(stderr,"transport_scheme_parameters_upwind: end\n");
		}
	#endif
}

#if INLINE_TRANSPORT_SCHEME_PARAMETERS_UPWIND
	#define transport_scheme_parameters_upwind \
		_transport_scheme_parameters_upwind
#else
	void transport_scheme_parameters_upwind(TransportParameters*,Parameters*);
#endif

static inline void _transport_scheme_parameters_upwind2
	(TransportParameters *tp,Parameters *p)
{
	register JBDOUBLE dtx;
	#if DEBUG_TRANSPORT_SCHEME_PARAMETERS_UPWIND2
		if (t>=DEBUG_TIME)
			fprintf(stderr,"transport_scheme_parameters_upwind2: start\n");
	#endif
	dtx = dt / p->ix;
	#if TRANSPORT_DISCRETIZATION == TRANSPORT_DISCRETIZATION_SPLIT
		if (p->U > 0.)
		{
			tp->dTm = tp->dTTm = 0.;
			tp->dTp = tp->dT;
			tp->dTTp = dt2 * (1. - dtx * p->U) * tp->dT;
		}
		else
		{
			tp->dTp = tp->dTTp = 0.;
			tp->dTm = tp->dT;
			tp->dTTm = dt2 * (1. + dtx * p->U) * tp->dT;
		}
	#elif FLUX_LIMITER_DISCRETIZATION == \
				FLUX_LIMITER_DISCRETIZATION_CONSERVATIVE
		#if FLOW_LINEARIZATION == FLOW_LINEARIZATION_ROE
			if (p->sA2 > 0.) tp->S = (tp->csA + (tp+1)->csA) / p->sA2;
			else tp->S = 0.;
		#else
			tp->S = 0.5 * (tp->c + (tp+1)->c);
		#endif
		if (fmax((p+1)->zs,p->zs) <= p->zsm)
			tp->dTm = tp->dTp = tp->dTTm = tp->dTTp = 0.;
		else
		{
			if (p->U > 0.)
			{
				tp->dTm = p->dQm * tp->S;
				tp->dTp = tp->dT - tp->dTm;
			}
			else
			{
				tp->dTp = p->dQp * tp->S;
				tp->dTm = tp->dT - tp->dTp;
			}
			tp->dTTm = dt2 * (tp->dTm +
				dtx * (tp->S * (p->dFm - p->U * p->dQm) + p->U * tp->dTm));
			tp->dTTp = dt2 * (tp->dTp -
				dtx * (tp->S * (p->dFp - p->U * p->dQp) + p->U * tp->dTp));
		}
		#if DEBUG_TRANSPORT_SCHEME_PARAMETERS_UPWIND2
			if (t>=DEBUG_TIME)
				fprintf(stderr,
					"TSPU2 dTp="FWF" dTm="FWF" dTTp="FWF" dTTm="FWF"\n",
					tp->dTp,tp->dTm,tp->dTTp,tp->dTTm);
		#endif
	#else
		register JBDOUBLE dW;
		#if FLOW_LINEARIZATION == FLOW_LINEARIZATION_ROE
			if (p->sA2 > 0.) tp->S = (tp->csA + (tp+1)->csA) / p->sA2;
			else tp->S = 0.;
		#else
			tp->S = 0.5 * (tp->c + (tp+1)->c);
		#endif
		if (fmax((p+1)->zs,p->zs) <= p->zsm)
			tp->dTm = tp->dTp = tp->dWCm = tp->dWCp = 0.;
		else
		{
			dW = tp->dT - tp->S * p->dQ;
			if (p->U > 0.)
			{
				tp->dWCm = 0.;
				tp->dWCp = dt2 * (1. - p->U * dtx) * dW;
				tp->dTm = p->dQm * tp->S;
				tp->dTp = tp->dT - tp->dTm;
			}
			else
			{
				tp->dWCp = 0.;
				tp->dWCm = dt2 * (1. + p->U * dtx) * dW;
				tp->dTp = p->dQp * tp->S;
				tp->dTm = tp->dT - tp->dTp;
			}
		}
		#if DEBUG_TRANSPORT_SCHEME_PARAMETERS_UPWIND2
			if (t>=DEBUG_TIME)
				fprintf(stderr,
					"TSPU2 dTp="FWF" dTm="FWF" dWCp="FWF" dWCm="FWF"\n",
					tp->dTp,tp->dTm,tp->dWCp,tp->dWCm);
		#endif
	#endif
	#if DEBUG_TRANSPORT_SCHEME_PARAMETERS_UPWIND2
		if (t>=DEBUG_TIME)
			fprintf(stderr,"transport_scheme_parameters_upwind2: end\n");
	#endif
}

#if INLINE_TRANSPORT_SCHEME_PARAMETERS_UPWIND2
	#define transport_scheme_parameters_upwind2 \
		_transport_scheme_parameters_upwind2
#else
	void transport_scheme_parameters_upwind2
		(TransportParameters*,Parameters*);
#endif

#if TRANSPORT_SCHEME == TRANSPORT_SCHEME_TVD2
	#define transport_scheme_parameters transport_scheme_parameters_upwind2
#else
	#define transport_scheme_parameters transport_scheme_parameters_upwind
#endif

static inline void
	_transport_scheme_diffusion(TransportParameters *tp,Parameters *p)
{
	register JBDOUBLE xi;
	#if DEBUG_TRANSPORT_SCHEME_DIFFUSION
		if (t>=DEBUG_TIME)
			fprintf(stderr,"transport_scheme_diffusion: start\n");
	#endif
	if (p->LA * p->LB >= 0.)
	{
		#if DEBUG_TRANSPORT_SCHEME_DIFFUSION
			if (t>=DEBUG_TIME)
				fprintf(stderr,"transport_scheme_diffusion: end\n");
		#endif
		return;
	}
	if (tp->dc > 0.)
	{
		if (p->U > 0.)
		{
			xi = dt * fmax(0., fmax(p->dQm * (tp->S - tp->c),
				tp->S * p->dQm + (tp+1)->c * p->dQp - tp->dT));
			tp->ic += xi;
			(tp+1)->ic -= xi;
		}
		else
		{
			xi = dt * fmax(0., fmax(p->dQp * ((tp+1)->c - tp->S),
				tp->dT - tp->S * p->dQp - tp->c * p->dQm));
			tp->ic += xi;
			(tp+1)->ic -= xi;
		}
	}
	else if (tp->dc < 0.)
	{
		if (p->U > 0.)
		{
			xi = dt * fmin(0., fmin(p->dQm * (tp->S - tp->c),
				tp->S * p->dQm + (tp+1)->c * p->dQp - tp->dT));
			tp->ic += xi;
			(tp+1)->ic -= xi;
		}
		else
		{
			xi = dt * fmin(0., fmin(p->dQp * ((tp+1)->c - tp->S),
				tp->dT - tp->S * p->dQp - tp->c * p->dQm));
			tp->ic += xi;
			(tp+1)->ic -= xi;
		}
	}
	#if DEBUG_TRANSPORT_SCHEME_DIFFUSION
		if (t>=DEBUG_TIME)
		{
			fprintf(stderr,"TSD ic="FWF" ic2="FWF"\n",tp->ic,(tp+1)->ic);
			fprintf(stderr,"transport_scheme_diffusion: end\n");
		}
	#endif
}

#if INLINE_TRANSPORT_SCHEME_DIFFUSION
	#define transport_scheme_diffusion \
		_transport_scheme_diffusion
#else
	void transport_scheme_diffusion(TransportParameters*,Parameters*);
#endif

static inline void
	_transport_scheme_upwind(TransportParameters *tp,Parameters *p)
{
	#if DEBUG_TRANSPORT_SCHEME_UPWIND
		if (t>=DEBUG_TIME)
		   fprintf(stderr,"transport_scheme_upwind: start\n");	
	#endif
	tp->ic -= dt * tp->dTm;
	(tp+1)->ic -= dt * tp->dTp;
	#if TRANSPORT_SCHEME_DIFFUSION
		transport_scheme_diffusion(tp,p);
	#endif
	#if DEBUG_TRANSPORT_SCHEME_UPWIND
		if (t>=DEBUG_TIME)
		   fprintf(stderr,"transport_scheme_upwind: end\n");	
	#endif
}

#if INLINE_TRANSPORT_SCHEME_UPWIND
	#define transport_scheme_upwind _transport_scheme_upwind
#else
	void transport_scheme_upwind(TransportParameters*,Parameters*);
#endif

#if TRANSPORT_SCHEME == TRANSPORT_SCHEME_TVD

static inline void
	_transport_scheme_tvd(TransportParameters *tp,Parameters *p)
{
	register JBDOUBLE k;
	#if DEBUG_TRANSPORT_SCHEME_TVD
		if (t>=DEBUG_TIME)
		   fprintf(stderr,"transport_scheme_tvd: start\n");	
	#endif
#if FLUX_LIMITER_DISCRETIZATION == FLUX_LIMITER_DISCRETIZATION_CONSERVATIVE
	k = dt2 * tp->dTm * transport_flux_limiter((tp-1)->dTm, tp->dTm);
	tp->ic -= k;
	(tp-1)->ic += k;
	k = dt2 * tp->dTp * transport_flux_limiter((tp+1)->dTp, tp->dTp);
	(tp+1)->ic -= k;
	(tp+2)->ic += k;
#else
	k = tp->S * p->dQQm +
		dt2 * tp->dWCm * transport_flux_limiter((tp-1)->dWCm, tp->dWCm);
	tp->ic -= k;
	(tp-1)->ic += k;
	k = tp->S * p->dQQp +
		dt2 * tp->dWCp * transport_flux_limiter((tp+1)->dWCp, tp->dWCp);
	(tp+1)->ic -= k;
	(tp+2)->ic += k;
#endif
	#if DEBUG_TRANSPORT_SCHEME_TVD
		if (t>=DEBUG_TIME)
		   fprintf(stderr,"transport_scheme_tvd: end\n");	
	#endif
}

#if INLINE_TRANSPORT_SCHEME_TVD
	#define transport_scheme_tvd _transport_scheme_tvd
#else
	void transport_scheme_tvd(TransportParameters*,Parameters*);
#endif

#elif TRANSPORT_SCHEME == TRANSPORT_SCHEME_TVD2

static inline void
	_transport_scheme_tvd2(TransportParameters *tp,Parameters *p)
{
	register JBDOUBLE k;
	#if DEBUG_TRANSPORT_SCHEME_TVD2
		if (t>=DEBUG_TIME)
		   fprintf(stderr,"transport_scheme_tvd2: start\n");	
	#endif
#if FLUX_LIMITER_DISCRETIZATION == FLUX_LIMITER_DISCRETIZATION_CONSERVATIVE
	k = tp->dTTm * transport_flux_limiter((tp-1)->dTTm, tp->dTTm);
	tp->ic -= k;
	(tp-1)->ic += k;
	k = tp->dTTp * transport_flux_limiter((tp+1)->dTTp, tp->dTTp);
	(tp+1)->ic -= k;
	(tp+2)->ic += k;
#else
	k = tp->S * p->dQQm +
		tp->dWCm * transport_flux_limiter((tp-1)->dWCm, tp->dWCm);
	tp->ic -= k;
	(tp-1)->ic += k;
	k = tp->S * p->dQQp +
		tp->dWCp * transport_flux_limiter((tp+1)->dWCp, tp->dWCp);
	(tp+1)->ic -= k;
	(tp+2)->ic += k;
#endif
	#if DEBUG_TRANSPORT_SCHEME_TVD2
		if (t>=DEBUG_TIME)
		   fprintf(stderr,"transport_scheme_tvd2: end\n");	
	#endif
}

#if INLINE_TRANSPORT_SCHEME_TVD2
	#define transport_scheme_tvd2 _transport_scheme_tvd2
#else
	void transport_scheme_tvd2(TransportParameters*,Parameters*);
#endif

#endif

static inline void _transport_scheme_contributions_upwind
	(TransportParameters *tp,Parameters *p,int n,JBDOUBLE *inlet,JBDOUBLE *outlet)
{
	#if DEBUG_TRANSPORT_SCHEME_CONTRIBUTIONS_UPWIND
		if (t>=DEBUG_TIME)
			fprintf(stderr,"transport_scheme_contributions_upwind: start\n");
	#endif
	*inlet = dt * tp[0].T;
	*outlet = dt * tp[n].T;
	#if DEBUG_TRANSPORT_SCHEME_CONTRIBUTIONS_UPWIND
		if (t>=DEBUG_TIME)
			fprintf(stderr,"transport_scheme_contributions_upwind: end\n");
	#endif
}

#if INLINE_TRANSPORT_SCHEME_CONTRIBUTIONS_UPWIND
	#define transport_scheme_contributions_upwind \
		_transport_scheme_contributions_upwind
#else
	void transport_scheme_contributions_upwind
		(TransportParameters*,Parameters*,int,JBDOUBLE*,JBDOUBLE*);
#endif

static inline void _transport_scheme_inlet_upwind
	(TransportParameters *tp,Parameters *p,int n,Mass *m,JBDOUBLE inlet)
{
	#if DEBUG_TRANSPORT_SCHEME_INLET_UPWIND
		if (t>=DEBUG_TIME)
		{
			fprintf(stderr,"transport_scheme_inlet_upwind: start\n");
			fprintf(stderr,"TSIU inlet="FWL"\n",m->inlet);
		}
	#endif
	if (p->Q >= 0.)
	{
		if (p->V > 0.) tp->c -= inlet / p->V; else tp->c = 0.;
	}
	else m->inlet += inlet;
	#if DEBUG_TRANSPORT_SCHEME_INLET_UPWIND
		if (t>=DEBUG_TIME)
		{
			fprintf(stderr,"TSIU inlet="FWL"\n",m->inlet);
			fprintf(stderr,"transport_scheme_inlet_upwind: end\n");
		}
	#endif
}

#if INLINE_TRANSPORT_SCHEME_INLET_UPWIND
	#define transport_scheme_inlet_upwind _transport_scheme_inlet_upwind
#else
	void transport_scheme_inlet_upwind
		(TransportParameters*,Parameters*,int,Mass*,JBDOUBLE);
#endif

static inline void _transport_scheme_outlet_upwind
	(TransportParameters *tp,Parameters *p,int n,Mass *m,JBDOUBLE outlet)
{
	#if DEBUG_TRANSPORT_SCHEME_OUTLET_UPWIND
		if (t>=DEBUG_TIME)
		{
			fprintf(stderr,"transport_scheme_outlet_upwind: start\n");
			fprintf(stderr,"TSOU outlet="FWL"\n",outlet);
		}
	#endif
	if (p->Q <= 0.)
	{
		if (p->V > 0.) tp->c += outlet / p->V; else tp->c = 0.;
	}
	else m->outlet -= outlet;
	#if DEBUG_TRANSPORT_SCHEME_OUTLET_UPWIND
		if (t>=DEBUG_TIME)
		{
			fprintf(stderr,"TSOU outlet="FWL"\n",m->outlet);
			fprintf(stderr,"transport_scheme_outlet_upwind: end\n");
		}
	#endif
}

#if INLINE_TRANSPORT_SCHEME_OUTLET_UPWIND
	#define transport_scheme_outlet_upwind _transport_scheme_outlet_upwind
#else
	void transport_scheme_outlet_upwind
		(TransportParameters*,Parameters*,int,Mass*,JBDOUBLE);
#endif

static inline void _transport_boundary
	(TransportParameters *tp,Parameters *p,Mass *m,BoundaryTransport *bt)
{
	int i;
	register JBDOUBLE mass;
	#if DEBUG_TRANSPORT_BOUNDARY
		if (t>=DEBUG_TIME)
		{
			fprintf(stderr,"transport_boundary: start\n");
			fprintf(stderr,"TB position=%d type=%d i=%d i2=%d\n",
				bt->pos,bt->type,bt->i,bt->i2);
			fprintf(stderr,"TB im="FWL" t="FWL" tmax="FWL"\n",m->inner,t,tmax);
		}
	#endif
	switch (bt->type)
	{
	case BOUNDARY_TRANSPORT_M:
		#if DEBUG_TRANSPORT_BOUNDARY
			if (t>=DEBUG_TIME)
				fprintf(stderr,"TB t0="FWF" Q0="FWF"\n",bt->p1[0],bt->p2[0]);
		#endif
		if (t == bt->p1[0]) mass = bt->p2[0]; else mass = 0.;
		break;
	default:
		mass = jbm_farray_integral(bt->p1, bt->p2, bt->n, t, tmax);
		#if DEBUG_TRANSPORT_BOUNDARY
			if (t>=DEBUG_TIME)
				fprintf(stderr,"TB mass="FWL" length="FWF"\n",mass,bt->length);
		#endif
	}
	g_mutex_lock(mutex);
	m->inner += mass;
	mass /= bt->length;
	for (i=bt->i; i<=bt->i2; ++i) tp[i].ic += mass * p[i].dx;
	g_mutex_unlock(mutex);
	#if DEBUG_TRANSPORT_BOUNDARY
		if (t>=DEBUG_TIME)
		{
			fprintf(stderr,"TB im="FWL" dm="FWL" c="FWF"\n",
				m->inner,mass,(tp+bt->i)->c);
			fprintf(stderr,"transport_boundary: end\n");
		}
	#endif
}

#if INLINE_TRANSPORT_BOUNDARY
	#define transport_boundary _transport_boundary
#else
	void transport_boundary
		(TransportParameters*,Parameters*,Mass*,BoundaryTransport*);
#endif

#endif
