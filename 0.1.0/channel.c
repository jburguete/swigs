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

#include "channel.h"

#if !INLINE_CHANNEL_PRINT
void channel_print(Channel *c,FILE *file) {_channel_print(c,file);}
#endif

#if !INLINE_CHANNEL_ERROR
void channel_error(Channel *c,char *m) {_channel_error(c,m);}
#endif

#if !INLINE_CHANNEL_DELETE_BF
void channel_delete_bf(Channel *channel,int i)
{_channel_delete_bf(channel,i);}
#endif

#if !INLINE_CHANNEL_INSERT_BF
int channel_insert_bf(Channel *channel,BoundaryFlow *bf,int i)
{return _channel_insert_bf(channel,bf,i);}
#endif

#if !INLINE_CHANNEL_DELETE_CT
void channel_delete_ct(Channel *channel,int i)
{_channel_delete_ct(channel,i);}
#endif

#if !INLINE_CHANNEL_INSERT_CT
int channel_insert_ct(Channel *channel,ChannelTransport *ct,int i)
{return _channel_insert_ct(channel,ct,i);}
#endif

#if !INLINE_CHANNEL_DELETE
void channel_delete(Channel *c) {_channel_delete(c);}
#endif

#if !INLINE_CHANNEL_CREATE
int channel_create(Channel *c,char *name)
{return _channel_create(c,name);}
#endif

#if !INLINE_CHANNEL_COPY
int channel_copy(Channel *c,Channel *c_copy) {return _channel_copy(c,c_copy);}
#endif

#if !INLINE_CHANNEL_BOUNDARY_TEST
int channel_boundary_test(Channel *c) {return _channel_boundary_test(c);}
#endif

#if !INLINE_CHANNEL_PARSE_NODE
int channel_parse_node(Channel *c,xmlNode *node,int *hasof)
{return _channel_parse_node(c,node,hasof);}
#endif

#if !INLINE_CHANNEL_OPEN_XML
int channel_open_xml(Channel *c,int nt,xmlNode *node)
{return _channel_open_xml(c,nt,node);}
#endif

#if !INLINE_CHANNEL_SAVE_XML
void channel_save_xml(Channel *c,xmlNode *node) {_channel_save_xml(c,node);}
#endif

