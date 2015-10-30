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

#ifndef DIFFUSION_SCHEME__H
#define DIFFUSION_SCHEME__H 1

#include "mesh.h"

static inline void _diffusion_parameters_explicit_explicit
  (TransportParameters * tp, Parameters * p, int n, JBDOUBLE dt)
{
  register int i;
  register Parameters *pv;
  register TransportParameters *tpv;
  register JBDOUBLE k;
#if DEBUG_DIFFUSION_PARAMETERS_EXPLICIT_EXPLICIT
  if (t >= DEBUG_TIME)
    fprintf (stderr, "diffusion_parameters_explicit_explicit: start\n");
#endif
  for (i = 0, tpv = tp, pv = p; ++i <= n; ++pv)
    {
#if DIFFUSION_SCHEME == DIFFUSION_SCHEME_IMPLICIT
      k = 0.5 * dt * fmin (pv->D,
                           0.5 * pv->A2 * (pv->ix / dt -
                                           fabs (pv->U))) * tpv->dc;
#else
      k = fmin (dt * pv->D,
                0.25 * pv->A2 * (pv->ix - fabs (pv->U) * dt)) * tpv->dc;
#endif
      tpv->ic += k;
      (++tpv)->ic -= k;
    }
#if DEBUG_DIFFUSION_PARAMETERS_EXPLICIT_EXPLICIT
  if (t >= DEBUG_TIME)
    {
      for (i = 0; i <= n; ++i)
        fprintf (stderr, "DPEE ic=" FWF "\n", tp[i].ic);
      fprintf (stderr, "diffusion_parameters_explicit_explicit: end\n");
    }
#endif
}

#if INLINE_DIFFUSION_PARAMETERS_EXPLICIT_EXPLICIT
#define diffusion_parameters_explicit_explicit \
		_diffusion_parameters_explicit_explicit
#else
void diffusion_parameters_explicit_explicit
  (TransportParameters *, Parameters *, int, JBDOUBLE);
#endif

static inline void _diffusion_parameters_explicit_implicit
  (TransportParameters * tp, Parameters * p, int n, JBDOUBLE dt)
{
  register int i;
  register Parameters *pv;
  register TransportParameters *tpv;
  register JBDOUBLE k;
#if DEBUG_DIFFUSION_PARAMETERS_EXPLICIT_IMPLICIT
  if (t >= DEBUG_TIME)
    fprintf (stderr, "diffusion_parameters_explicit_implicit: start\n");
#endif
  for (i = 0, tpv = tp, pv = p; ++i <= n; ++pv)
    {
      k = fmax (0., pv->D - pv->D * pv->ix /
                (dt * (fabs (pv->U) + (pv->Kx + pv->Kx) / pv->ix))) * tpv->dc;
      tpv->ic += k;
      (++tpv)->ic -= k;
    }
#if DEBUG_DIFFUSION_PARAMETERS_EXPLICIT_IMPLICIT
  if (t >= DEBUG_TIME)
    fprintf (stderr, "diffusion_parameters_explicit_implicit: end\n");
#endif
}

#if INLINE_DIFFUSION_PARAMETERS_EXPLICIT_IMPLICIT
#define diffusion_parameters_explicit_implicit \
		_diffusion_parameters_explicit_implicit
#else
void diffusion_parameters_explicit_implicit
  (TransportParameters *, Parameters *, int, JBDOUBLE);
#endif

static inline void
_diffusion_parameters_implicit_explicit (Parameters * p)
{
  register JBDOUBLE k, k2;
#if DEBUG_DIFFUSION_PARAMETERS_IMPLICIT_EXPLICIT
  if (t >= DEBUG_TIME)
    {
      fprintf (stderr, "diffusion_parameters_implicit_explicit: start\n");
      fprintf (stderr,
               "DPIE dt=" FWL " D=" FWF " A2=" FWF " ix=" FWF " U=" FWF "\n",
               dt, p->D, p->A2, p->ix, p->U);
    }
#endif
  k2 = p->D * dt;
  k = k2 - 0.25 * p->A2 * (p->ix - fabs (p->U) * dt);
#if DIFFUSION_SCHEME == DIFFUSION_SCHEME_IMPLICIT
  k = fmax (0.5 * k2, k);
#else
  k = fmax (0., k);
#endif
  p->CC = p->EE = -k;
  p->DD += k;
  (p + 1)->DD += k;
#if DEBUG_DIFFUSION_PARAMETERS_IMPLICIT_EXPLICIT
  if (t >= DEBUG_TIME)
    {
      fprintf (stderr, "DPIE k=" FWL "\n", k);
      fprintf (stderr, "DPIE CC=" FWF " DD=" FWF " EE=" FWF "\n",
               p->CC, p->DD, p->EE);
      fprintf (stderr, "diffusion_parameters_implicit_explicit: end\n");
    }
#endif
}

#if INLINE_DIFFUSION_PARAMETERS_IMPLICIT_EXPLICIT
#define diffusion_parameters_implicit_explicit \
		_diffusion_parameters_implicit_explicit
#else
void diffusion_parameters_implicit_explicit (Parameters *);
#endif

#endif
