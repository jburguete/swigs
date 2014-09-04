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

#include "channel_segment.h"

#if !INLINE_CHANNEL_SEGMENT_PRINT
void channel_segment_print(ChannelSegment *seg,FILE *file)
{_channel_segment_print(seg,file);}
#endif

#if !INLINE_CHANNEL_SEGMENT_DELETE
void channel_segment_delete(ChannelSegment *seg) {_channel_segment_delete(seg);}
#endif

#if !INLINE_CHANNEL_SEGMENT_OPEN_SECTION
int channel_segment_open_section(ChannelSegment *seg,Section *s,JBDOUBLE x)
{return _channel_segment_open_section(seg,s,x);}
#endif

#if !INLINE_CHANNEL_SEGMENT_OPEN
int channel_segment_open(ChannelSegment *seg,TransientSection *ts1,
	TransientSection *ts2,JBDOUBLE x1,JBDOUBLE x2)
{return _channel_segment_open(seg,ts1,ts2,x1,x2);}
#endif
