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
transport_simulate.c

Source file defining the main functions of the numerical methods of solute
transport

Author: Javier Burguete Tolosa
Copyright 2005-2014 Javier Burguete Tolosa. All rights reserved
*/

#include "transport_simulate.h"

#if !INLINE_TRANSPORT_STEP_IMPLICIT
void transport_step_implicit(TransportParameters *tp,Parameters *p,int n)
{_transport_step_implicit(tp,p,n);}
#endif

#if !INLINE_TRANSPORT_SIMULATE_STEP
void transport_simulate_step
	(TransportParameters *tp,Parameters *p,int n,Mass *m)
{_transport_simulate_step(tp,p,n,m);}
#endif

#if !INLINE_JUNCTION_TRANSPORT_SCHEME
void junction_transport_scheme(Junction *junction)
{_junction_transport_scheme(junction);}
#endif

#if !INLINE_PART_TRANSPORT_STEP
void part_transport_step(int thread) {_part_transport_step(thread);}
#endif

#if !INLINE_SYSTEM_TRANSPORT_STEP_PARALLEL
void system_transport_step_parallel() {_system_transport_step_parallel();}
#endif

#if !INLINE_SYSTEM_TRANSPORT_STEP_SIMPLE
void system_transport_step_simple() {_system_transport_step_simple();}
#endif
