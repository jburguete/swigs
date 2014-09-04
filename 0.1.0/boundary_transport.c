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

#include "boundary_transport.h"

#if !INLINE_BOUNDARY_TRANSPORT_PRINT
void boundary_transport_print(BoundaryTransport *bt,FILE *file)
{_boundary_transport_print(bt,file);}
#endif

#if !INLINE_BOUNDARY_TRANSPORT_ERROR
void boundary_transport_error(BoundaryTransport *bt,char *m)
{_boundary_transport_error(bt,m);}
#endif

#if !INLINE_BOUNDARY_TRANSPORT_DELETE
void boundary_transport_delete(BoundaryTransport *bt)
{_boundary_transport_delete(bt);}
#endif

#if !INLINE_BOUNDARY_TRANSPORT_COPY
int boundary_transport_copy(BoundaryTransport *bt,BoundaryTransport *bt_copy)
{return _boundary_transport_copy(bt,bt_copy);}
#endif

#if !INLINE_BOUNDARY_TRANSPORT_OPEN_XML
int boundary_transport_open_xml(BoundaryTransport *bt,xmlNode *node)
{return _boundary_transport_open_xml(bt,node);}
#endif

#if !INLINE_BOUNDARY_TRANSPORT_SAVE_XML
void boundary_transport_save_xml(BoundaryTransport *bt,xmlNode *node)
{return _boundary_transport_save_xml(bt,node);}
#endif

#if !INLINE_BOUNDARY_TRANSPORT_PARAMETER
JBDOUBLE boundary_transport_parameter(BoundaryTransport *bt,JBDOUBLE t)
{return _boundary_transport_parameter(bt,t);}
#endif
