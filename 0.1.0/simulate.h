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
simulate.h

Header file defining the main functions of the simulation

Author: Javier Burguete Tolosa
Copyright (c) 2005-2014. All rights reserved
*/

#ifndef SIMULATE_H
#define SIMULATE_H 1

#include "flow_scheme.h"
#include "transport_simulate.h"
#include "write.h"

extern int n,type_animation,started,simulated,istep,steading,file_advance;
extern JBFLOAT flow_steady_error_max;

extern void
	(*transport_parameters_step)(TransportParameters*,Parameters*,int,JBDOUBLE);
extern void (*transport_scheme_contributions)
	(TransportParameters*,Parameters*,int,JBDOUBLE*,JBDOUBLE*);
extern void (*transport_scheme_inlet)
	(TransportParameters*,Parameters*,int,Mass*,JBDOUBLE);
extern void (*transport_scheme_outlet)
	(TransportParameters*,Parameters*,int,Mass*,JBDOUBLE);
extern void (*diffusion_parameters_explicit)
	(TransportParameters*,Parameters*,int,JBDOUBLE);

#if JBW==JBW_GTK

#include "dialog_simulator.h"

extern int opening;

typedef struct
{
	GtkLabel *label;
	GtkProgressBar *progress;
	GtkVBox *box;
	GtkDialog *window;
} DialogOpenParameters;

extern DialogOpenParameters dialog_open_parameters[1];

typedef struct
{
	GtkLabel *label,*label_error;
	JBWFloatEntry *entry;
	GtkVBox *box;
	GtkDialog *window;
} DialogSimulateSteady;

void dialog_open_parameters_destroy();
void dialog_open_parameters_actualize();
void dialog_open_parameters_new();

void dialog_simulate_steady_actualize(DialogSimulateSteady*,JBDOUBLE);
void dialog_simulate_steady_new(DialogSimulateSteady*);

#endif

/*
void scheme_open()

Function to select the numerical scheme
*/

static inline void _scheme_open()
{
	int i;
	#if DEBUG_SCHEME_OPEN
		fprintf(stderr,"Scheme open: start\n");
	#endif
	if (nthreads==1)
	{
		system_parameters=system_parameters_simple;
		system_parameters2=system_parameters2_simple;
		system_decomposition=system_decomposition_simple;
		system_simulate_step=system_simulate_step_simple;
		system_simulate_step2=system_simulate_step2_simple;
		system_transport_step=system_transport_step_simple;
	}
	else
	{
		system_parameters=system_parameters_parallel;
		system_parameters2=system_parameters2_parallel;
		system_decomposition=system_decomposition_parallel;
		system_simulate_step=system_simulate_step_parallel;
		system_simulate_step2=system_simulate_step2_parallel;
		system_transport_step=system_transport_step_parallel;
	}
	list_channels =
		(GSList**)g_realloc(list_channels, nthreads * sizeof(GSList*));
	for (i=0; i<nthreads; ++i) list_channels[i] = NULL;
//	if (fcfl <= FSC)
//	{
		flow_parameters=flow_parameters_upwind2;
		part_decomposition=part_decomposition_explicit;
		transport_scheme_contributions=transport_scheme_contributions_upwind;
		transport_scheme_inlet=transport_scheme_inlet_upwind;
		transport_scheme_outlet=transport_scheme_outlet_upwind;
		diffusion_parameters_explicit=diffusion_parameters_explicit_explicit;
		diffusion_parameters_implicit=diffusion_parameters_implicit_explicit;
//	}
	#if DEBUG_SCHEME_OPEN
		fprintf(stderr,"Scheme open: end\n");
	#endif
}

#if INLINE_SCHEME_OPEN
	#define scheme_open _scheme_open
#else
	void scheme_open();
#endif

void simulate_steady();
int simulate_start();
void simulate();

extern void draw();

#endif
