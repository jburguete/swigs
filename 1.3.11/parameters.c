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

#include "parameters.h"

#if !INLINE_TRANSPORT_PARAMETERS_SATURATION
void transport_parameters_saturation
	(TransportParameters *tp,Parameters *p,JBDOUBLE cmax)
{_transport_parameters_saturation(tp,p,cmax);}
#endif

#if !INLINE_TRANSPORT_PARAMETERS_NODE
void transport_parameters_node
	(TransportParameters *tp,Parameters *p,JBDOUBLE cmax)
{_transport_parameters_node(tp,p,cmax);}
#endif

#if !INLINE_TRANSPORT_PARAMETERS_CELL
void transport_parameters_cell(TransportParameters *tp)
{_transport_parameters_cell(tp);}
#endif

#if !INLINE_PARAMETERS_WAVE_VELOCITY
void parameters_wave_velocity(Parameters *p) {_parameters_wave_velocity(p);}
#endif

#if !INLINE_PARAMETERS_NODE
void parameters_node(Parameters *p) {_parameters_node(p);}
#endif

#if !INLINE_PARAMETERS_CELL
void parameters_cell(Parameters *p) {_parameters_cell(p);}
#endif

#if !INLINE_FLOW_DRY_CORRECTION
void flow_dry_correction(Parameters *p) {_flow_dry_correction(p);}
#endif

#if !INLINE_FLOWS
void flows(Parameters *p) {_flows(p);}
#endif

#if !INLINE_CHANNEL_DRY_CORRECTION
void channel_dry_correction(Channel *channel)
{_channel_dry_correction(channel);}
#endif

#if !INLINE_CHANNEL_DRY_CORRECTION_EXIT
void channel_dry_correction_exit(Channel *channel)
{_channel_dry_correction_exit(channel);}
#endif

#if !INLINE_SYSTEM_DRY_CORRECTION_PARALLEL
void system_dry_correction_parallel() {_system_dry_correction_parallel();}
#endif

#if !INLINE_SYSTEM_DRY_CORRECTION_SIMPLE
void system_dry_correction_simple() {_system_dry_correction_simple();}
#endif

#if !INLINE_CHANGE_SECTIONS
int change_sections(Parameters *p,GSList *list_variable_segments,JBDOUBLE t,
	JBDOUBLE tmax,JBFLOAT hmin)
{return _change_sections(p,list_variable_segments,t,tmax,hmin);}
#endif

#if !INLINE_LIST_SECTION_GAUGE_STEP
void list_section_gauge_step(GSList *list,Parameters *p)
{_list_section_gauge_step(list,p);}
#endif
