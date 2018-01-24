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

/*
transport_simulate.h

Header file defining the main functions of the numerical methods of solute
transport

Author: Javier Burguete Tolosa
Copyright 2005-2015 Javier Burguete Tolosa. All rights reserved
*/

#ifndef TRANSPORT_SIMULATE__H
#define TRANSPORT_SIMULATE__H 1

#include "diffusion_scheme.h"
#include "transport_scheme.h"

/*
Pointers to functions selecting the numerical method used in solute transport
*/

extern void
  (*transport_parameters_step) (TransportParameters *, Parameters *, int,
                                JBDOUBLE);
extern void (*transport_scheme_contributions) (TransportParameters *,
                                               Parameters *, int, JBDOUBLE *,
                                               JBDOUBLE *);
extern void (*transport_scheme_inlet) (TransportParameters *, Parameters *, int,
                                       Mass *, JBDOUBLE);
extern void (*transport_scheme_outlet) (TransportParameters *, Parameters *,
                                        int, Mass *, JBDOUBLE);
extern void (*diffusion_parameters_explicit) (TransportParameters *,
                                              Parameters *, int, JBDOUBLE);
extern void (*transport_parameters_implicit) (Parameters *, int, JBDOUBLE,
                                              JBFLOAT *, JBFLOAT *, JBFLOAT *);
extern void (*system_transport_step) ();

/*
void transport_step_implicit(TransportParameters *tp,Parameters *p,int n)

Function to solve the implicit part of a numerical method in solute transport

Inputs:
	tp: array of transport parameters
	p: array of flow parameters
	n: number of cell nodes
*/

static inline void _transport_step_implicit
  (TransportParameters * tp, Parameters * p, int n)
{
  register int i;
  register Parameters *pv;
  register TransportParameters *tpv;
#if DEBUG_TRANSPORT_STEP_IMPLICIT
  if (t >= DEBUG_TIME)
    fprintf (stderr, "transport_step_implicit: start\n");
#endif
  for (i = 0, tpv = tp, pv = p; i <= n; ++i, ++tpv, ++pv)
    pv->HH = tpv->m + tpv->ic;
#if DEBUG_TRANSPORT_STEP_IMPLICIT
  if (t >= DEBUG_TIME)
    {
      for (i = 0; i <= n; ++i)
        fprintf (stderr, "TSI i=%d m=" FWF " ic=" FWF "\n",
                 i, tp[i].m, tp[i].ic);
      for (i = 0; i <= n; ++i)
        fprintf (stderr, "TSI i=%d C=" FWF " D=" FWF " E=" FWF " H=" FWF "\n",
                 i, p[i].CC, p[i].DD, p[i].EE, p[i].HH);
      fprintf (stderr, "TSI solving tridiagonal array\n");
    }
#endif
  jbm_varray_solve_tridiagonal_zero
    (&p[1].CC, &p[0].DD, &p[0].EE, &p[0].HH, sizeof (Parameters), n);
  for (i = 0, tpv = tp, pv = p; i <= n; ++i, ++tpv, ++pv)
    tpv->c = pv->HH;
#if DEBUG_TRANSPORT_STEP_IMPLICIT
  if (t >= DEBUG_TIME)
    {
      for (i = 0; i <= n; ++i)
        fprintf (stderr, "TSI i=%d c=" FWF "\n", i, tp[i].c);
      fprintf (stderr, "transport_step_implicit: end\n");
    }
#endif
}

#if INLINE_TRANSPORT_STEP_IMPLICIT
#define transport_step_implicit _transport_step_implicit
#else
void transport_step_implicit (TransportParameters *, Parameters *, int);
#endif

/*
void transport_simulate_step
	(TransportParameters *tp,Parameters *p,int n,Mass *m)

Function making a step of a numerical method in solute transport

Inputs:
	tp: array of transport parameters
	p: array of flow parameters
	n: number of cell nodes
	m: mass structure of the solute
*/

static inline void _transport_simulate_step
  (TransportParameters * tp, Parameters * p, int n, Mass * m)
{
  JBDOUBLE inlet, outlet;
#if DEBUG_TRANSPORT_SIMULATE_STEP
  if (t >= DEBUG_TIME)
    {
      fprintf (stderr, "transport_simulate_step: start\n");
      fprintf (stderr, "TSS t=" FWL " tmax=" FWL "\n", t, tmax);
    }
#endif
  transport_scheme_contributions (tp, p, n, &inlet, &outlet);
  transport_step_implicit (tp, p, n);
  transport_scheme_inlet (tp, p, n, m, inlet);
  transport_scheme_outlet (tp + n, p + n, n, m, outlet);
#if DEBUG_TRANSPORT_SIMULATE_STEP
  if (t >= DEBUG_TIME)
    fprintf (stderr, "transport_simulate_step: end\n");
#endif
}

#if INLINE_TRANSPORT_SIMULATE_STEP
#define transport_simulate_step _transport_simulate_step
#else
void transport_simulate_step (TransportParameters *, Parameters *, int, Mass *);
#endif

/*
void junction_transport_scheme(Junction *junction)

Function solving a junction in solute transport

Inputs:
	junction: structure of the junction
*/

static inline void
_junction_transport_scheme (Junction * junction)
{
  register int i, j;
  register TransportParameters *tpv;
#if DEBUG_JUNCTIONS_SCHEME
  if (t >= DEBUG_TIME)
    fprintf (stderr, "junctions_scheme: start\n");
#endif
  for (i = 0; i <= junction->nparameters; ++i)
    for (j = 0, tpv = junction->transport[i]; j <= sys->nt; tpv += n)
      tpv->c = junction->mass[++j];
#if DEBUG_JUNCTIONS_SCHEME
  if (t >= DEBUG_TIME)
    fprintf (stderr, "junctions_scheme: end\n");
#endif
}

#if INLINE_JUNCTION_TRANSPORT_SCHEME
#define junction_transport_scheme _junction_transport_scheme
#else
void junction_transport_scheme (Junction * junction);
#endif

/*
void part_transport_step(int thread)

Function solving a step of solute transport in the channels associated to a
thread

Inputs:
	thread: thread number
*/

static inline void
_part_transport_step (int thread)
{
  int i;
  GSList *l;
  ChannelGeometry *cg;
  Parameters *pv;
  TransportParameters *tpv;
#if DEBUG_PART_TRANSPORT_STEP
  if (t >= DEBUG_TIME)
    fprintf (stderr, "part_transport_step: start\n");
#endif
  for (l = list_channels[thread]; l; l = l->next)
    {
      cg = (ChannelGeometry *) l->data;
      pv = p + cg->cs->i;
      tpv = tp + cg->cs->i;
#if DEBUG_PART_TRANSPORT_STEP
      if (t >= DEBUG_TIME)
        fprintf (stderr, "PPS channel=%s\n", pv->channel->name);
#endif
      for (i = 0; i <= sys->nt; ++i, tpv += n + 1)
        {
#if DEBUG_PART_TRANSPORT_STEP
          if (t >= DEBUG_TIME)
            fprintf (stderr, "PPS i=%d\n", i);
#endif
          transport_simulate_step
            (tpv, pv, cg->cs[cg->n].i - cg->cs->i, m + i + 1);
        }
    }
#if DEBUG_PART_TRANSPORT_STEP
  if (t >= DEBUG_TIME)
    fprintf (stderr, "part_transport_step: end\n");
#endif
}

#if INLINE_PART_TRANSPORT_STEP
#define part_transport_step _part_transport_step
#else
void part_transport_step (int);
#endif

/*
void system_transport_step_parallel()

Function solving a step of solute transport (parallelized version)
*/

static inline void
_system_transport_step_parallel ()
{
  int i;
  GThread *thread[nth];
#if DEBUG_SYSTEM_TRANSPORT_STEP_PARALLEL
  if (t >= DEBUG_TIME)
    fprintf (stderr, "system_transport_step_parallel: start\n");
#endif
  for (i = 0; i < nth; ++i)
    thread[i] = g_thread_new
      (NULL, (void (*)) part_transport_step, (void *) (size_t) i);
  for (i = 0; i < nth; ++i)
    g_thread_join (thread[i]);
  for (i = 0; i <= njunctions; ++i)
    junction_transport_scheme (junction + i);
#if DEBUG_SYSTEM_TRANSPORT_STEP_PARALLEL
  if (t >= DEBUG_TIME)
    fprintf (stderr, "system_transport_step_parallel: end\n");
#endif
}

#if INLINE_SYSTEM_TRANSPORT_STEP_PARALLEL
#define system_transport_step_parallel _system_transport_step_parallel
#else
void system_transport_step_parallel ();
#endif

/*
void system_transport_step_simple()

Function solving a step of solute transport (simple version)
*/

static inline void
_system_transport_step_simple ()
{
  int i;
#if DEBUG_SYSTEM_TRANSPORT_STEP_SIMPLE
  if (t >= DEBUG_TIME)
    fprintf (stderr, "system_transport_step_simple: start\n");
#endif
  part_transport_step (0);
  for (i = 0; i <= njunctions; ++i)
    junction_transport_scheme (junction + i);
#if DEBUG_SYSTEM_TRANSPORT_STEP_SIMPLE
  if (t >= DEBUG_TIME)
    fprintf (stderr, "system_transport_step_simple: end\n");
#endif
}

#if INLINE_SYSTEM_TRANSPORT_STEP_SIMPLE
#define system_transport_step_simple _system_transport_step_simple
#else
void system_transport_step_simple ();
#endif

#endif
