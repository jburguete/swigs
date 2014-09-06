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

#include "boundary_flow.h"

#if !INLINE_BOUNDARY_FLOW_PRINT
void boundary_flow_print(BoundaryFlow *bf,FILE *file)
{_boundary_flow_print(bf,file);}
#endif

#if !INLINE_BOUNDARY_FLOW_ERROR
void boundary_flow_error(BoundaryFlow *bf,char *m) {_boundary_flow_error(bf,m);}
#endif

#if !INLINE_BOUNDARY_FLOW_DELETE
void boundary_flow_delete(BoundaryFlow *bf) {_boundary_flow_delete(bf);}
#endif

#if !INLINE_BOUNDARY_FLOW_COPY
int boundary_flow_copy(BoundaryFlow *bf,BoundaryFlow *bf_copy)
{return _boundary_flow_copy(bf,bf_copy);}
#endif

#if !INLINE_BOUNDARY_FLOW_OPEN_XML
int boundary_flow_open_xml(BoundaryFlow *bf,xmlNode *node,int position)
{return _boundary_flow_open_xml(bf,node,position);}
#endif

#if !INLINE_BOUNDARY_FLOW_SAVE_XML
void boundary_flow_save_xml(BoundaryFlow *bf,xmlNode *node,int position)
{return _boundary_flow_save_xml(bf,node,position);}
#endif

#if !INLINE_BOUNDARY_FLOW_PARAMETER
JBDOUBLE boundary_flow_parameter(BoundaryFlow *bf,JBDOUBLE t)
{return _boundary_flow_parameter(bf,t);}
#endif

#if !INLINE_BOUNDARY_FLOW_PARAMETER2
JBDOUBLE boundary_flow_parameter2(BoundaryFlow *bf,JBDOUBLE t)
{return _boundary_flow_parameter2(bf,t);}
#endif

#if !INLINE_BOUNDARY_FLOW_PARAMETER_INTEGRAL
JBDOUBLE boundary_flow_parameter_integral(BoundaryFlow *bf,JBDOUBLE t,JBDOUBLE tmax)
{return _boundary_flow_parameter_integral(bf,t,tmax);}
#endif
