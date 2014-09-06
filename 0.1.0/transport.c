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

#include "transport.h"

#if !INLINE_TRANSPORT_PRINT
void transport_print(Transport *t,FILE *file) {_transport_print(t,file);}
#endif

#if !INLINE_TRANSPORT_ERROR
void transport_error(Transport *t,char *m) {_transport_error(t,m);}
#endif

#if !INLINE_TRANSPORT_DELETE
void transport_delete(Transport *t) {_transport_delete(t);}
#endif

#if !INLINE_TRANSPORT_CREATE
int transport_create(Transport *t,char *name)
{return _transport_create(t,name);}
#endif

#if !INLINE_TRANSPORT_COPY
int transport_copy(Transport *t,Transport *t_copy)
{return _transport_copy(t,t_copy);}
#endif

#if !INLINE_TRANSPORT_OPEN_XML
int transport_open_xml(Transport *t,xmlNode *node)
{return _transport_open_xml(t,node);}
#endif

#if !INLINE_TRANSPORT_SAVE_XML
void transport_save_xml(Transport *t,xmlNode *node)
{_transport_save_xml(t,node);}
#endif
