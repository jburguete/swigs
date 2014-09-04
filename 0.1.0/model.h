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

#ifndef MODEL__H
#define MODEL__H 1

#include "mesh.h"

#if MODEL_PRESSURE == MODEL_PRESSURE_HIGH_ORDER
	JBDOUBLE dt2v;
#endif

static inline JBDOUBLE _model_friction(JBDOUBLE r,JBDOUBLE R,JBDOUBLE A)
{
	register JBDOUBLE k;
	#if DEBUG_MODEL_FRICTION
		if (t>=DEBUG_TIME)
		{
			fprintf(stderr,"model_friction: start\n");
			fprintf(stderr,"MF r="FWL" R="FWL" A="FWL"\n",r,R,A);
		}
	#endif
	k=r/(A*pow(R,friction_2b1));
	#if DEBUG_MODEL_FRICTION
		if (t>=DEBUG_TIME)
		{
			fprintf(stderr,"MF "FWL"\n",k);
			fprintf(stderr,"model_friction: end\n");
		}
	#endif
	return k;
}

#if INLINE_MODEL_FRICTION
	#define model_friction _model_friction
#else
	JBDOUBLE model_friction(JBDOUBLE,JBDOUBLE,JBDOUBLE);
#endif

static inline JBDOUBLE _model_diffusion(Parameters *p)
{
	#if DEBUG_MODEL_DIFFUSION
		if (t>=DEBUG_TIME)
			fprintf(stderr,"MD Q="FWF" K="FWF" R="FWF"\n",p->Q,p->K,p->P);
	#endif
	#if MODEL_DIFFUSION == MODEL_DIFFUSION_CONSTANT
		return DIFFUSION_CONSTANT;
	#elif MODEL_DIFFUSION == MODEL_DIFFUSION_B
		return 1e-6 + DIFFUSION_CONSTANT * sqrt(p->K * p->Q * p->Q * p->P);
	#elif MODEL_DIFFUSION == MODEL_DIFFUSION_H
		return
			1e-6 + DIFFUSION_CONSTANT * p->R * sqrt(p->K * p->Q * p->Q / p->P);
	#elif MODEL_DIFFUSION == MODEL_DIFFUSION_NULL
		return 0.;
	#else
		#error "Unknown viscosity model"
	#endif
}

#if INLINE_MODEL_DIFFUSION
	#define model_diffusion _model_diffusion
#else
	JBDOUBLE model_diffusion(Parameters *p);
#endif

#if MODEL_VISCOSITY == MODEL_VISCOSITY_TURBULENT
	#define model_viscosity model_viscosity_turbulent
#elif MODEL_VISCOSITY == MODEL_VISCOSITY_NULL
	#define model_viscosity(a,b) ;
#else
	#error "Unknown viscosity model"
#endif

#endif
