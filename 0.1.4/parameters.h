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

#ifndef PARAMETERS__H
#define PARAMETERS__H 1

#include "model.h"

extern int channel_overflow;
extern JBDOUBLE dtmax,fdtmax;

static inline void _transport_parameters_saturation
	(TransportParameters *tp,Parameters *p,JBDOUBLE cmax)
{
	register JBDOUBLE m;
	#if DEBUG_TRANSPORT_PARAMETERS_SATURATION
		if (t>=DEBUG_TIME)
		{
			fprintf(stderr,"transport_parameters_saturation: start\n");
			fprintf(stderr,"TPS c="FWF" b="FWF" cmax="FWL"\n",tp->c,tp->b,cmax);
			fprintf(stderr,"TPS V="FWF" A="FWF"\n",p->V,p->A);
		}
	#endif
	if (tp->c > cmax)
	{
		tp->b += p->V * (tp->c - cmax);
		tp->c = cmax;
	}
	if (tp->b > 0.)
	{
		m = fmin(tp->b / p->V, cmax - tp->c);
		tp->b -= m * p->V;
		tp->c += m;
	}
	#if DEBUG_TRANSPORT_PARAMETERS_SATURATION
		if (t>=DEBUG_TIME)
		{
			fprintf(stderr,"TPS c="FWF" b="FWF"\n",tp->c,tp->b);
			fprintf(stderr,"transport_parameters_saturation: end\n");
		}
	#endif
}

#if INLINE_TRANSPORT_PARAMETERS_SATURATION
	#define transport_parameters_saturation _transport_parameters_saturation
#else
	void transport_parameters_saturation
		(TransportParameters*,Parameters*,JBDOUBLE);
#endif

static inline void _transport_parameters_node
	(TransportParameters *tp,Parameters *p,JBDOUBLE cmax)
{
	#if DEBUG_TRANSPORT_PARAMETERS_NODE
		if (t>=DEBUG_TIME) fprintf(stderr,"transport_parameters_node: start\n");
	#endif
	tp->ic = 0.;
	transport_parameters_saturation(tp,p,cmax);
	tp->m = p->V * tp->c;
	tp->T = p->Q * tp->c;
	#if (TRANSPORT_DISCRETIZATION!=TRANSPORT_DISCRETIZATION_SPLIT) && \
		(FLOW_LINEARIZATION==FLOW_LINEARIZATION_ROE)
		tp->csA = p->sA * tp->c;
		#if DEBUG_TRANSPORT_PARAMETERS_NODE
			if (t>=DEBUG_TIME)
				fprintf(stderr,"TPN csA="FWF" sA="FWF" c="FWF"\n",
					tp->csA,p->sA,tp->c);
		#endif
	#endif
	#if DEBUG_TRANSPORT_PARAMETERS_NODE
		if (t>=DEBUG_TIME)
		{
			fprintf(stderr,"TPN m="FWF" T="FWF"\n",tp->m,tp->T);
			fprintf(stderr,"transport_parameters_node: end\n");
		}
	#endif
}

#if INLINE_TRANSPORT_PARAMETERS_NODE
	#define transport_parameters_node _transport_parameters_node
#else
	void transport_parameters_node(TransportParameters*,Parameters*,JBDOUBLE);
#endif

static inline void _transport_parameters_cell(TransportParameters *tp)
{
	#if DEBUG_TRANSPORT_PARAMETERS_CELL
		if (t>=DEBUG_TIME) fprintf(stderr,"transport_parameters_cell: start\n");
	#endif
	tp->dT = (tp+1)->T - tp->T;
	tp->dc = (tp+1)->c - tp->c;
	#if DEBUG_TRANSPORT_PARAMETERS_CELL
		if (t>=DEBUG_TIME)
		{
			fprintf(stderr,"TPC dT="FWF" dc="FWF"\n",tp->dT,tp->dc);
			fprintf(stderr,"transport_parameters_cell: end\n");
		}
	#endif
}

#if INLINE_TRANSPORT_PARAMETERS_CELL
	#define transport_parameters_cell _transport_parameters_cell
#else
	void transport_parameters_cell(TransportParameters*);
#endif

static inline void _parameters_wave_velocity(Parameters *p)
{
	register JBDOUBLE k;
	#if DEBUG_PARAMETERS_WAVE_VELOCITY
		if (t>=DEBUG_TIME)
		{
			fprintf(stderr,"parameters_wave_velocity: start\n");
			fprintf(stderr,"PWV A="FWF" B="FWF" Amin="FWF"\n",
				p->A,p->B,p->Amin);
		}
	#endif
	if (p->A < p->Amin) k = p->hmin; else k = p->A / p->B;
	p->uc = sqrt(p->g * k);
	#if DEBUG_PARAMETERS_WAVE_VELOCITY
		if (t>=DEBUG_TIME)
		{
			fprintf(stderr,"PWV uc="FWF"\n",p->uc);
			fprintf(stderr,"parameters_wave_velocity: end\n");
		}
	#endif
}

#if INLINE_PARAMETERS_WAVE_VELOCITY
	#define parameters_wave_velocity _parameters_wave_velocity
#else
	void parameters_wave_velocity(Parameters*);
#endif

static inline void _parameters_node(Parameters *p)
{
	unsigned int j;
	JBDOUBLE k,dz;
	SectionParameters *sp;
	FrictionParameters *fp;
	#if DEBUG_PARAMETERS_NODE
		if (t>=DEBUG_TIME)
		{
			fprintf(stderr,"parameters_node: start\n");
			fprintf(stderr, "PN V="FWF"\n", p->V);
		}
	#endif
	#if INTERFACE == INTERFACE_MARKET
		if (p->V < 0. || isnan(p->V)) p->V = 0.;
	#endif
	p->A = p->V / p->dx;
	sp = p->s->sp;
	#if DEBUG_PARAMETERS_NODE
		if (t>=DEBUG_TIME)
			fprintf(stderr, "PN A="FWF" An="FWF"\n", p->A, (sp + p->s->n)->A);
	#endif
	if (p->A < (sp + p->s->n)->A)
	{
		p->j = jbm_varray_search
			(p->A, &sp->A, sizeof(SectionParameters), p->s->n);
		sp += p->j;
		p->zs = section_trapezoidal_level(sp, p->A);
		dz = p->zs - sp->z;
		p->P = sp->P + dz * sp->dPz;
		p->B = sp->B + dz * sp->dBz;
		k = p->zs + sp->z;
		k = sp->I + 1./6. * dz * (p->B * (k + p->zs) + sp->B * (k + sp->z));
	}
	else
	{
		p->zs = section_level(p->s, p->A);
		p->j = p->s->n;
		sp += p->s->n;
		dz = p->zs - sp->z;
		p->P = sp->P;
		p->B = sp->B;
		k = sp->I - 0.5 * dz * p->B * (p->zs + sp->z);
	}
	p->I = p->zs * p->A - k;
	if (p->A > 0.)
	{
		p->zm = k / p->A;
		p->tr = t;
	}
	else
	{
		p->zm = p->s->sp->z;
		if (p->tr == 0.) p->ta = t;
	}
	p->h = p->zs - p->s->sp->z;
	#if FLOW_LINEARIZATION == FLOW_LINEARIZATION_ROE
		p->sA = sqrt(p->A);
	#endif
	#if MODEL_PRESSURE == MODEL_PRESSURE_HIGH_ORDER
		p->izm = (p->zm - p->zmv) / dt;
	#endif
	
	if (p->A <= p->Amin)
	{
		p->R = p->hmin;
		p->K = p->nu = p->dbetaA = 0.;
		p->beta = 1.;
	}
	else
	{
		j = jbm_min(p->s->nf, (int)ceill((p->zs - p->s->sp->z) / p->s->dz));
		fp = p->s->fp + j;
		p->R = p->A / p->P;
		p->K = model_friction(fp->r, p->R, p->A);
		#if MODEL_FRICTION == MODEL_FRICTION_SIMPLE
			p->beta = 1.;
			p->dbetaA = 0.;
		#else
			p->beta = fp->beta;
			p->dbetaA = fp->dbetaA;
		#endif
		p->nu = model_diffusion(p);
		#if DEBUG_PARAMETERS_NODE
			if (t>=DEBUG_TIME)
			{
				fprintf(stderr,"PN R="FWF" K="FWF"\n",p->R,p->K);
				fprintf(stderr,"PN zs="FWF" zb="FWF" dz="FWF"\n",
					p->zs,p->s->sp->z,p->s->dz);
				fprintf(stderr,"PN i=%d r="FWF"\n",
					(int)rint((p->zs - p->s->sp->z) / p->s->dz), fp->r);
			}
		#endif
	}
	p->Anu = p->A * p->nu;
	
	#if DEBUG_PARAMETERS_NODE
		if (t>=DEBUG_TIME)
		{
			fprintf(stderr,"PN A="FWF" z="FWF" h="FWF"\n"
				"PN B="FWF" I="FWF" K="FWF"\n"
				"PN Amin="FWF" zm="FWF" beta="FWF"\n",
				p->A,p->zs,p->h,p->B,p->I,p->K,p->Amin,p->zm,p->beta);
			#if FLOW_LINEARIZATION == FLOW_LINEARIZATION_ROE
				fprintf(stderr,"PN sA="FWF"\n",p->sA);
			#endif
			fprintf(stderr,"parameters_node: end\n");
		}
	#endif
}

#if INLINE_PARAMETERS_NODE
	#define parameters_node _parameters_node
#else
	void parameters_node(Parameters*);
#endif

static inline void _parameters_cell(Parameters *p)
{
	#if DEBUG_PARAMETERS_CELL
		if (t>=DEBUG_TIME) fprintf(stderr,"parameters_cell: start\n");
	#endif
	p->A2 = (p+1)->A + p->A;
	#if FLOW_LINEARIZATION == FLOW_LINEARIZATION_ROE
		p->sA2 = (p+1)->sA + p->sA;
	#endif
	p->dzs = (p+1)->zs - p->zs;
	#if MODEL_PRESSURE == MODEL_PRESSURE_HYDROSTATIC
		p->gm = JBM_G;
		p->dg = 0.;
	#elif MODEL_PRESSURE == MODEL_PRESSURE_STRONG_SLOPE
		#if DEBUG_PARAMETERS_CELL
			if (t>=DEBUG_TIME)
			{
//				fprintf(stderr,"PC Sf="FWF" Sf2="FWF"\n",p->Sf,(p+1)->Sf);
				fprintf(stderr, "PC dzs="FWF" dzm="FWF" dzb="FWF" ix="FWF"\n",
					p->dzs, (p+1)->zm - p->zm, p->dzb, p->ix);
			}
		#endif
//		p->gm = JBM_G / (1. +
//			jbm_fsqr(jbm_fmodmin(0.5 * ((p+1)->Sf + p->Sf), jbm_fmodmin
//			(jbm_fmodmin(p->dzs, (p+1)->zm - p->zm), p->dzb) / p->ix)));
		p->gm = JBM_G / (1. + jbm_fsqr(jbm_fmodmin(p->dzb,
			jbm_fmodmin(p->dzs, (p+1)->zm - p->zm)) / p->ix));
		p->dg = 0.;
	#endif
	p->Kx = fmin((p+1)->nu, p->nu);
	p->D = fmin((p+1)->Anu, p->Anu) / p->ix;
	#if DEBUG_PARAMETERS_CELL
		if (t>=DEBUG_TIME)
		{
			fprintf(stderr,"PC A2="FWF" dzs="FWF" gm="FWF"\n",
				p->A2, p->dzs, p->gm);
			fprintf(stderr,"parameters_cell: end\n");
		}
	#endif
}

#if INLINE_PARAMETERS_CELL
	#define parameters_cell _parameters_cell
#else
	void parameters_cell(Parameters*);
#endif

static inline void _flow_dry_correction(Parameters *p)
{
	register JBDOUBLE k;
	#if DEBUG_FLOW_DRY_CORRECTION
		if (t>=DEBUG_TIME) fprintf(stderr,"flow_dry_correction: start\n");
	#endif
	p->Q = p->u = p->F = p->Ff = 0.;
//	p->Sf = 0.;
	k = p->lmax = p->la = p->uc;
	p->lb = -k;
	p->dt = INFINITY;
	#if TMAX == TMAX_POINT
		k = p->lmax / p->dx;
		if (k>dtmax)
		{
			#if DEBUG_FLOW_DRY_CORRECTION
				if (t>=DEBUG_TIME) fprintf(stderr, "FDC lmax="FWF" dx="FWF"\n",
					p->lmax, p->dx);
			#endif
			g_mutex_lock(mutex);
			dtmax=k;
			g_mutex_unlock(mutex);
		}
	#endif
	#if DEBUG_FLOW_DRY_CORRECTION
		if (t>=DEBUG_TIME)
		{
			fprintf(stderr,"FDC v 0 F 0 H 0 la "FWF" lb "FWF" Q 0\n",
				p->la,p->lb);
			fprintf(stderr,"flow_dry_correction: end\n");
		}
	#endif
}

#if INLINE_FLOW_DRY_CORRECTION
	#define flow_dry_correction _flow_dry_correction
#else
	void flow_dry_correction(Parameters*);
#endif

static inline void _flows(Parameters *p)
{
	register JBDOUBLE u,c;
	#if DEBUG_FLOWS
		if (t>=DEBUG_TIME) fprintf(stderr,"flows: start\n");
	#endif
	c = p->K * fabs(p->Q);
	p->dt = 1. / c;
	if (p->dt < fdtmax)
	{
		g_mutex_lock(mutex);
		fdtmax = p->dt;
		g_mutex_unlock(mutex);
	}
	p->Ff = c * p->Q;
//	p->Sf = p->Ff / (JBM_G * p->A);
	p->u = p->Q / p->A;
	p->F = p->beta * p->u * p->Q;
	u = p->beta * p->u;
	c = sqrt(p->uc * p->uc + (p->beta * p->beta - p->beta + p->A * p->dbetaA) *
		p->u * p->u);
	p->la = u + c;
	p->lb = u - c;
	p->lmax = fmax(fabs(p->la), fabs(p->lb));
	#if TMAX == TMAX_POINT
		register double k = p->lmax / p->dx;
		if (k>dtmax)
		{
			#if DEBUG_FLOW_DRY_CORRECTION
				if (t>=DEBUG_TIME) fprintf(stderr, "F lmax="FWF" dx="FWF"\n",
					p->lmax, p->dx);
			#endif
			g_mutex_lock(mutex);
			dtmax=k;
			g_mutex_unlock(mutex);
		}
	#endif
	#if DEBUG_FLOWS
		if (t>=DEBUG_TIME)
		{
			fprintf(stderr,
				"F v="FWF" F="FWF"\nla="FWF" lb="FWF"\nQ="FWF" Ff="FWF
				" beta="FWF" dbetaA="FWF"\n",
				p->u,p->F,p->la,p->lb,p->Q,p->Ff,p->beta,p->dbetaA);
			fprintf(stderr,"flows: end\n");
		}
	#endif
}

#if INLINE_FLOWS
	#define flows _flows
#else
	void flows(Parameters*);
#endif

static inline void _channel_dry_correction(Channel *channel)
{
	register int i,j;
	register JBDOUBLE m;
	#if DEBUG_CHANNEL_DRY_CORRECTION
		if (t>=DEBUG_TIME)
		{
			fprintf(stderr,"channel_dry_correction: start\n");
			fprintf(stderr,"CDC mass="FWL"\n",flow_mass(p,n));
			for (i=channel->i; i<=channel->i2; ++i)
				fprintf(stderr,"CDC i=%d V="FWF"\n",i,p[i].V);
		}			
	#endif
	for (i=channel->i2+1; --i>=channel->i;) if (p[i].V<0.)
	{
		m=p[i].V;
		p[i].V=0.;
		j=i+1;
		for (; --i>=channel->i && p[i].V<0.; p[i].V=0.) m+=p[i].V;
		#if DEBUG_CHANNEL_DRY_CORRECTION
			if (t>=DEBUG_TIME)
				fprintf(stderr,"CDC i=%d j=%d m="FWL"\n",i,j,m);
		#endif
		do
		{
			if (j>channel->i2)
			{
				for (; i>=channel->i; --i)
				{
					m+=p[i].V;
					#if DEBUG_CHANNEL_DRY_CORRECTION
						if (t>=DEBUG_TIME)
							fprintf(stderr,"CDC i=%d m="FWL"\n",i,m);
					#endif
					if (m>=0.)
					{
						p[i].V=m;
						break;
					}
					else p[i].V=0.;
				}
			}
			else if (i<channel->i)
			{
				for (; j<=channel->i2; ++j)	
				{
					m+=p[j].V;
					#if DEBUG_CHANNEL_DRY_CORRECTION
						if (t>=DEBUG_TIME)
							fprintf(stderr,"CDC j=%d m="FWL"\n",j,m);
					#endif
					if (m>=0.)
					{
						p[j].V=m;
						break;
					}
					else p[j].V=0.;
				}
			}
			else if (p[i].V<p[j].V)
			{
				m+=p[j].V;
				#if DEBUG_CHANNEL_DRY_CORRECTION
					if (t>=DEBUG_TIME)
						fprintf(stderr,"CDC j=%d m="FWL"\n",j,m);
				#endif
				if (m>=0.)
				{
					p[j].V=m;
					break;
				}
				else p[j].V=0.;
				++j;
			}
			else
			{
				m+=p[i].V;
				#if DEBUG_CHANNEL_DRY_CORRECTION
					if (t>=DEBUG_TIME)
						fprintf(stderr,"CDC i=%d m="FWL"\n",i,m);
				#endif
				if (m>=0.)
				{
					p[i].V=m;
					break;
				}
				else p[i].V=0.;
				--i;
			}			
			#if DEBUG_CHANNEL_DRY_CORRECTION
				if (t>=DEBUG_TIME)
					fprintf(stderr, "CDC i=%d j=%d i1=%d i2=%d\n",
						i, j, channel->i, channel->i2);
			#endif
		}
		while (m<0. && (i>=channel->i || j<=channel->i2));
		if (p[i].V<0.) ++i;
	}
	#if DEBUG_CHANNEL_DRY_CORRECTION
		if (t>=DEBUG_TIME)
		{
			for (i=channel->i; i<=channel->i2; ++i)
				fprintf(stderr,"CDC i=%d V="FWF"\n",i,p[i].V);
			fprintf(stderr,"CDC mass="FWL"\n",flow_mass(p,n));
			fprintf(stderr,"channel_dry_correction: end\n");
		}
	#endif
}

#if INLINE_CHANNEL_DRY_CORRECTION
	#define channel_dry_correction _channel_dry_correction
#else
	void channel_dry_correction(Channel*);
#endif

static inline void _channel_dry_correction_exit(Channel *channel)
{
	#if DEBUG_CHANNEL_DRY_CORRECTION_EXIT
		if (t>=DEBUG_TIME)
			fprintf(stderr,"channel_dry_correction_exit: start\n");
	#endif
	channel_dry_correction(channel);
	#if DEBUG_CHANNEL_DRY_CORRECTION_EXIT
		if (t>=DEBUG_TIME) fprintf(stderr,"channel_dry_correction_exit: end\n");
	#endif
	g_thread_exit(NULL);
}

#if INLINE_CHANNEL_DRY_CORRECTION_EXIT
	#define channel_dry_correction_exit _channel_dry_correction_exit
#else
	void channel_dry_correction_exit(Channel*);
#endif

static inline void _system_dry_correction_parallel()
{
	int i;
	GThread *thread[sys->n];
	Channel *channel;
	#if DEBUG_SYSTEM_DRY_CORRECTION_PARALLEL
		if (t>=DEBUG_TIME)
		{
			fprintf(stderr,"system_dry_correction_parallel: start\n");
			fprintf(stderr,"SDCP mass="FWL"\n",flow_mass(p,n));
		}
	#endif
	for (i=sys->n, channel=sys->channel; i>=0; --i, ++channel) if (channel->dry)
		thread[i] = g_thread_new
			(NULL, (void(*))channel_dry_correction_exit, (void*)channel);
	for (i=sys->n, channel=sys->channel; i>=0; --i, ++channel) if (channel->dry)
		g_thread_join(thread[i]);
	#if DEBUG_SYSTEM_DRY_CORRECTION_PARALLEL
		if (t>=DEBUG_TIME)
		{
			fprintf(stderr,"SDCP mass="FWL"\n",flow_mass(p,n));
			fprintf(stderr,"system_dry_correction_parallel: end\n");
		}
	#endif
}

#if INLINE_SYSTEM_DRY_CORRECTION_PARALLEL
	#define system_dry_correction_parallel _system_dry_correction_parallel
#else
	void system_dry_correction_parallel();
#endif

static inline void _system_dry_correction_simple()
{
	int i;
	Channel *channel;
	#if DEBUG_SYSTEM_DRY_CORRECTION_SIMPLE
		if (t>=DEBUG_TIME)
		{
			fprintf(stderr,"system_dry_correction_simple: start\n");
			fprintf(stderr,"SDCS mass="FWL"\n",flow_mass(p,n));
		}
	#endif
	for (i=sys->n, channel=sys->channel; i>=0; --i, ++channel) if (channel->dry)
		channel_dry_correction(channel);
	#if DEBUG_SYSTEM_DRY_CORRECTION_SIMPLE
		if (t>=DEBUG_TIME)
		{
			fprintf(stderr,"SDCS mass="FWL"\n",flow_mass(p,n));
			fprintf(stderr,"system_dry_correction_simple: end\n");
		}
	#endif
}

#if INLINE_SYSTEM_DRY_CORRECTION_SIMPLE
	#define system_dry_correction_simple _system_dry_correction_simple
#else
	void system_dry_correction_simple();
#endif

static inline int _change_sections(Parameters *p,GSList *list_variable_segments,
	JBDOUBLE t,JBDOUBLE tmax,JBFLOAT hmin)
{
	int i,k,k2;
	GSList *list;
	CrossSection *cs;
	TransientSection *ts1,*ts2,*tsv1,*tsv2;
	ChannelSegment seg[1];
	SectionVariable *sv;
	Parameters *pv;
	#if DEBUG_CHANGE_SECTIONS
		fprintf(stderr,"Change sections: start\n");
	#endif
	for (list=list_variable_segments, k2=1; list; list=list->next, k2=k)
	{
		sv = (SectionVariable*)list->data;
		cs = sv->cs;
		tsv1=cross_section_get_transient(cs,t);
		tsv2=cross_section_get_transient(cs+1,t);
		ts1=cross_section_get_transient(cs,tmax);
		ts2=cross_section_get_transient(cs+1,tmax);
		if (ts2==tsv2) k=1; else k=0;
		if (k && k2 && ts1==tsv1) continue;
		if (!channel_segment_open(seg,ts1,ts2,sv->x[0],sv->x[1])) goto exit2;
		i = cs->i;
		#if DEBUG_CHANGE_SECTIONS
			fprintf(stderr,"CS i1=%d i2=%d\n",i,(cs+1)->i);
		#endif
		pv = p + i;
		if (k2)
		{
			section_delete(pv->s);
			if (!section_copy(pv->s,seg->s1)) goto exit1;
			parameters_node(pv);
		}
		while (++i < (cs+1)->i)
		{
			++pv;
			section_delete(pv->s);
			if (!channel_segment_open_section(seg,pv->s,pv->x)) goto exit1;
			parameters_node(pv);
		}
		if (!k)
		{
			++pv;
			if (!section_copy(pv->s,seg->s2)) goto exit1;
			parameters_node(pv);
		}
		mesh_actualize(p + cs->i, (cs+1)->i - cs->i);
		channel_segment_delete(seg);
	}
	#if DEBUG_CHANGE_SECTIONS
		fprintf(stderr,"Change sections: end\n");
	#endif
	return 1;

exit1:
	channel_segment_delete(seg);

exit2:
	#if DEBUG_CHANGE_SECTIONS
		fprintf(stderr,"Change sections: end\n");
	#endif
	return 0;
}

#if INLINE_CHANGE_SECTIONS
	#define change_sections _change_sections
#else
	int change_sections(Parameters*,GSList*,JBDOUBLE,JBDOUBLE,JBFLOAT);
#endif

static inline void _list_section_gauge_step(GSList *list,Parameters *p)
{
	register JBDOUBLE x;
	CrossSection *cs,*csc;
	#if DEBUG_LIST_SECTION_GAUGE_STEP
		fprintf(stderr,"List section gauge step: start\n");
	#endif
	for (; list; list=list->next)
	{
		cs = ((SectionGauge*)list->data)->cs;
		csc = ((SectionGauge*)list->data)->csc;
		p += csc->i;
		++cs->nt;
		switch (cs->type)
		{
		case CROSS_SECTION_TYPE_CONTROL_Q:
			x = p->Q;
			break;
		case CROSS_SECTION_TYPE_CONTROL_H:
			x = p->h;
			break;
		default:
			x = p->zs;
		}
		cs->pt += x;
	}
	#if DEBUG_LIST_SECTION_GAUGE_STEP
		fprintf(stderr,"List section gauge step: end\n");
	#endif
}

#if INLINE_LIST_SECTION_GAUGE_STEP
	#define list_section_gauge_step _list_section_gauge_step
#else
	void list_section_gauge_step(GSList*,Parameters*);
#endif

#endif
