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

#include "channel_geometry.h"

#if !INLINE_CHANNEL_GEOMETRY_PRINT
void channel_geometry_print(ChannelGeometry *cg,FILE *file)
{_channel_geometry_print(cg,file);}
#endif

#if !INLINE_CHANNEL_GEOMETRY_ERROR
void channel_geometry_error(char *m) {_channel_geometry_error(m);}
#endif

#if !INLINE_CHANNEL_GEOMETRY_ACTUALIZE_X
void channel_geometry_actualize_x(ChannelGeometry *cg,int i)
{_channel_geometry_actualize_x(cg,i);}
#endif

#if !INLINE_CHANNEL_GEOMETRY_DELETE_SECTION
void channel_geometry_delete_section(ChannelGeometry *cg,int i)
{_channel_geometry_delete_section(cg,i);}
#endif

#if !INLINE_CHANNEL_GEOMETRY_INSERT_SECTION
int channel_geometry_insert_section
	(ChannelGeometry *cg,CrossSection *cs,int i)
{return _channel_geometry_insert_section(cg,cs,i);}
#endif

#if !INLINE_CHANNEL_GEOMETRY_DELETE
void channel_geometry_delete(ChannelGeometry *cg)
{_channel_geometry_delete(cg);}
#endif

#if !INLINE_CHANNEL_GEOMETRY_COPY
int channel_geometry_copy(ChannelGeometry *cg,ChannelGeometry *cg_copy)
{return _channel_geometry_copy(cg,cg_copy);}
#endif

#if !INLINE_CHANNEL_GEOMETRY_OPEN_XML
int channel_geometry_open_xml(ChannelGeometry *cg,xmlNode *node)
{return _channel_geometry_open_xml(cg,node);}
#endif

#if !INLINE_CHANNEL_GEOMETRY_SAVE_XML
void channel_geometry_save_xml(ChannelGeometry *cg,xmlNode *node)
{_channel_geometry_save_xml(cg,node);}
#endif
