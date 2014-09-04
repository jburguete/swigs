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

#include "channel_transport.h"

#if !INLINE_CHANNEL_TRANSPORT_PRINT
void channel_transport_print(ChannelTransport *ct,FILE *file)
{_channel_transport_print(ct,file);}
#endif

#if !INLINE_CHANNEL_TRANSPORT_ERROR
void channel_transport_error(ChannelTransport *ct,char *m)
{_channel_transport_error(ct,m);}
#endif

#if !INLINE_CHANNEL_TRANSPORT_DELETE_BT
void channel_transport_delete_bt(ChannelTransport *ct,int i)
{_channel_transport_delete_bt(ct,i);}
#endif

#if !INLINE_CHANNEL_TRANSPORT_INSERT_BT
int channel_transport_insert_bt
	(ChannelTransport *ct,BoundaryTransport *bt,int i)
{return _channel_transport_insert_bt(ct,bt,i);}
#endif

#if !INLINE_CHANNEL_TRANSPORT_DELETE
void channel_transport_delete(ChannelTransport *ct)
{_channel_transport_delete(ct);}
#endif

#if !INLINE_CHANNEL_TRANSPORT_COPY
int channel_transport_copy(ChannelTransport *ct,ChannelTransport *ct_copy)
{return _channel_transport_copy(ct,ct_copy);}
#endif

#if !INLINE_CHANNEL_TRANSPORT_PARSE_NODE
int channel_transport_parse_node(ChannelTransport *ct,xmlNode *node)
{return _channel_transport_parse_node(ct,node);}
#endif

#if !INLINE_CHANNEL_TRANSPORT_OPEN_XML
int channel_transport_open_xml(ChannelTransport *ct,xmlNode *node)
{return _channel_transport_open_xml(ct,node);}
#endif

#if !INLINE_CHANNEL_TRANSPORT_SAVE_XML
void channel_transport_save_xml(ChannelTransport *ct,xmlNode *node)
{_channel_transport_save_xml(ct,node);}
#endif
