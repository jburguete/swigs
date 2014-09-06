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

#include "flow_scheme.h"

#if !INLINE_PART_PARAMETERS
void part_parameters(int thread) {_part_parameters(thread);}
#endif

#if !INLINE_PART_PARAMETERS_EXIT
void part_parameters_exit(int thread) {_part_parameters_exit(thread);}
#endif

#if !INLINE_SYSTEM_PARAMETERS_PARALLEL
void system_parameters_parallel() {_system_parameters_parallel();}
#endif

#if !INLINE_SYSTEM_PARAMETERS_SIMPLE
void system_parameters_simple() {_system_parameters_simple();}
#endif

#if !INLINE_PART_PARAMETERS2
void part_parameters2(int thread) {_part_parameters2(thread);}
#endif

#if !INLINE_PART_PARAMETERS2_EXIT
void part_parameters2_exit(int thread) {_part_parameters2_exit(thread);}
#endif

#if !INLINE_BOUNDARY_FLOW_TMAX
void boundary_flow_tmax(BoundaryFlow *bf) {_boundary_flow_tmax(bf);}
#endif

#if !INLINE_SYSTEM_PARAMETERS2_PARALLEL
void system_parameters2_parallel() {_system_parameters2_parallel();}
#endif

#if !INLINE_SYSTEM_PARAMETERS2_SIMPLE
void system_parameters2_simple() {_system_parameters2_simple();}
#endif

#if !INLINE_FLOW_LINEARIZATION
void flow_linearization(Parameters *p) {_flow_linearization(p);}
#endif

#if !INLINE_VARIABLES_ACTUALIZE
void variables_actualize(Variables *v) {_variables_actualize(v);}
#endif

#if !INLINE_FLOW
void flow(Parameters *p) {_flow(p);}
#endif

#if !INLINE_FLOW_WAVE_UPWIND
void flow_wave_upwind(Parameters *p) {_flow_wave_upwind(p);}
#endif

#if !INLINE_FLOW_WAVE_UPWIND2
void flow_wave_upwind2(Parameters *p) {_flow_wave_upwind2(p);}
#endif

#if !INLINE_FLOW_ENTROPY_CORRECTION
JBDOUBLE flow_entropy_correction(JBDOUBLE l1,JBDOUBLE l2,JBDOUBLE lm)
{return _flow_entropy_correction(l1,l2,lm);}
#endif

#if !INLINE_FLOW_ENTROPY_WAVE
void flow_entropy_wave(Parameters *p,JBDOUBLE k2,JBDOUBLE dA,JBDOUBLE dQ)
{_flow_entropy_wave(p,k2,dA,dQ);}
#endif

#if !INLINE_FLOW_ENTROPY
void flow_entropy(Parameters *p) {_flow_entropy(p);}
#endif

#if !INLINE_FLOW_PARAMETERS_UPWIND
void flow_parameters_upwind(Parameters *p) {_flow_parameters_upwind(p);}
#endif

#if !INLINE_FLOW_PARAMETERS_UPWIND2
void flow_parameters_upwind2(Parameters *p) {_flow_parameters_upwind2(p);}
#endif

#if !INLINE_FLOW_INNER_BOUNDARY
void flow_inner_boundary(BoundaryFlow *bf) {_flow_inner_boundary(bf);}
#endif

#if !INLINE_FLOW_WAVE_DECOMPOSITION_UPWIND
void flow_wave_decomposition_upwind(Parameters *p)
{_flow_wave_decomposition_upwind(p);}
#endif

#if FLOW_WAVE_DECOMPOSITION != FLOW_WAVE_DECOMPOSITION_UPWIND
#if !INLINE_FLOW_WAVE_DECOMPOSITION
void flow_wave_decomposition(Parameters *p) {_flow_wave_decomposition(p);}
#endif
#endif

#if !INLINE_FLOW_FRICTION_EXPLICIT
void flow_friction_explicit(Parameters *p) {_flow_friction_explicit(p);}
#endif

#if !INLINE_FLOW_INLET_EXPLICIT
void flow_inlet_explicit(BoundaryFlow *bf,Parameters *p)
{_flow_inlet_explicit(bf,p);}
#endif

#if !INLINE_FLOW_OUTLET_EXPLICIT
void flow_outlet_explicit(BoundaryFlow *bf,Parameters *p)
{_flow_outlet_explicit(bf,p);}
#endif

#if !INLINE_PART_DECOMPOSITION_EXPLICIT
void part_decomposition_explicit(int thread)
{_part_decomposition_explicit(thread);}
#endif

#if !INLINE_PART_DECOMPOSITION_EXIT
void part_decomposition_exit(int thread) {_part_decomposition_exit(thread);}
#endif

#if !INLINE_SYSTEM_DECOMPOSITION_PARALLEL
void system_decomposition_parallel() {_system_decomposition_parallel();}
#endif

#if !INLINE_SYSTEM_DECOMPOSITION_SIMPLE
void system_decomposition_simple() {_system_decomposition_simple();}
#endif

#if !INLINE_FLOW_IMPLICIT_FRICTION
void flow_implicit_friction(Parameters *p,JBDOUBLE odt)
{_flow_implicit_friction(p,odt);}
#endif

#if !INLINE_PART_SIMULATE_STEP
void part_simulate_step(int thread) {_part_simulate_step(thread);}
#endif

#if !INLINE_PART_SIMULATE_STEP_EXIT
void part_simulate_step_exit(int thread) {_part_simulate_step_exit(thread);}
#endif

#if !INLINE_JUNCTION_FLOW_SCHEME
void junction_flow_scheme(Junction *junction) {_junction_flow_scheme(junction);}
#endif

#if !INLINE_SYSTEM_SIMULATE_STEP_PARALLEL
void system_simulate_step_parallel() {_system_simulate_step_parallel();}
#endif

#if !INLINE_SYSTEM_SIMULATE_STEP_SIMPLE
void system_simulate_step_simple() {_system_simulate_step_simple();}
#endif

#if !INLINE_PART_SIMULATE_STEP2
void part_simulate_step2(int thread) {_part_simulate_step2(thread);}
#endif

#if !INLINE_PART_SIMULATE_STEP2_EXIT
void part_simulate_step2_exit(int thread) {_part_simulate_step2_exit(thread);}
#endif

#if !INLINE_SYSTEM_SIMULATE_STEP2_PARALLEL
void system_simulate_step2_parallel() {_system_simulate_step2_parallel();}
#endif

#if !INLINE_SYSTEM_SIMULATE_STEP2_SIMPLE
void system_simulate_step2_simple() {_system_simulate_step2_simple();}
#endif

#if !INLINE_FLOW_STEADY_ERROR
JBDOUBLE flow_steady_error(Parameters *p,int n) {return _flow_steady_error(p,n);}
#endif

