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

#include "initial_transport.h"

#if !INLINE_INITIAL_TRANSPORT_PRINT
void initial_transport_print(InitialTransport *it,FILE *file)
{_initial_transport_print(it,file);}
#endif

#if !INLINE_INITIAL_TRANSPORT_ERROR
void initial_transport_error(char *m) {_initial_transport_error(m);}
#endif

#if !INLINE_INITIAL_TRANSPORT_DELETE
void initial_transport_delete(InitialTransport *it)
{_initial_transport_delete(it);}
#endif

#if !INLINE_INITIAL_TRANSPORT_COPY
int initial_transport_copy(InitialTransport *it,InitialTransport *it_copy)
{return _initial_transport_copy(it,it_copy);}
#endif

#if !INLINE_INITIAL_TRANSPORT_ORDER
int initial_transport_order(InitialTransport *it)
{return _initial_transport_order(it);}
#endif

#if !INLINE_INITIAL_TRANSPORT_OPEN_XML
int initial_transport_open_xml(InitialTransport *it,xmlNode *node)
{return _initial_transport_open_xml(it,node);}
#endif

#if !INLINE_INITIAL_TRANSPORT_SAVE_XML
void initial_transport_save_xml(InitialTransport *it,xmlNode *node)
{_initial_transport_save_xml(it,node);}
#endif
