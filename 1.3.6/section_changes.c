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

#include "section_changes.h"

#if !INLINE_SECTION_CHANGES_PRINT
void section_changes_print(SectionChanges *sc,FILE *file)
{_section_changes_print(sc,file);}
#endif

#if !INLINE_SECTION_CHANGES_TMAX
void section_changes_tmax(SectionChanges *sc) {_section_changes_tmax(sc);}
#endif

#if !INLINE_LIST_SECTION_CHANGES_PRINT
void list_section_changes_print(GSList *list,FILE *file)
{_list_section_changes_print(list,file);}
#endif

#if !INLINE_LIST_SECTION_CHANGES_TMAX
void list_section_changes_tmax(GSList *list)
{_list_section_changes_tmax(list);}
#endif

#if !INLINE_LIST_SECTION_CHANGES_OPEN
int list_section_changes_open(GSList **list,ChannelGeometry *cg)
{return _list_section_changes_open(list,cg);}
#endif

#if !INLINE_SECTION_GAUGE_PRINT
void section_gauge_print(SectionGauge *sc,FILE *file)
{_section_gauge_print(sc,file);}
#endif

#if !INLINE_SECTION_GAUGE_TMAX
void section_gauge_tmax(SectionGauge *sc) {_section_gauge_tmax(sc);}
#endif

#if !INLINE_LIST_SECTION_GAUGE_PRINT
void list_section_gauge_print(GSList *list,FILE *file)
{_list_section_gauge_print(list,file);}
#endif

#if !INLINE_LIST_SECTION_GAUGE_TMAX
void list_section_gauge_tmax(GSList *list) {_list_section_gauge_tmax(list);}
#endif

#if !INLINE_LIST_SECTION_GAUGE_OPEN
int list_section_gauge_open(GSList **list,ChannelGeometry *cg)
{return _list_section_gauge_open(list,cg);}
#endif

#if !INLINE_LIST_VARIABLE_SEGMENTS_OPEN
int list_variable_segments_open(GSList **list,ChannelGeometry *cg)
{return _list_variable_segments_open(list,cg);}
#endif

