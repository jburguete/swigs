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

#ifndef SECTION_CHANGES__H
#define SECTION_CHANGES__H 1

#include "channel_geometry.h"

extern GMutex *mutex;

extern JBDOUBLE t,tmax;

typedef struct
{
	CrossSection *cs;
} SectionChanges;

typedef struct
{
	CrossSection *cs,*csc;
} SectionGauge;

typedef struct
{
	JBFLOAT *x;
	CrossSection *cs;
} SectionVariable;

static inline void _section_changes_print(SectionChanges *sc,FILE *file)
{
	fprintf(file,"Section changes print: start\n");
	cross_section_print(sc->cs,file);
	fprintf(file,"Section changes print: end\n");
}

#if INLINE_SECTION_CHANGES_PRINT
	#define section_changes_print _section_changes_print
#else
	void section_changes_print(SectionChanges*,FILE*);
#endif

static inline void _section_changes_tmax(SectionChanges *sc)
{
	register int i;
	register CrossSection *cs = sc->cs;
	#if DEBUG_SECTION_CHANGES_TMAX
		fprintf(stderr,"Section changes tmax: start\n");
		fprintf(stderr,"SCT t="FWL" tmax="FWL"\n",t,tmax);
	#endif
	if (t >= cs->t[cs->n]) goto exit;
	i = jbm_farray_search(t,cs->t,cs->n);
	#if DEBUG_SECTION_CHANGES_TMAX
		fprintf(stderr,"SCT i=%d n=%d\n",i,cs->n);
	#endif
	if (i<cs->n)
	{
		if (cs->t[i+1] < tmax)
		{
			g_mutex_lock(mutex);
			tmax = cs->t[i+1];
			g_mutex_unlock(mutex);
		}
		#if DEBUG_SECTION_CHANGES_TMAX
			fprintf(stderr,"SCT i=%d ti="FWL" tmax="FWL"\n",i,cs->t[i+1],tmax);
		#endif
	}
exit:
	#if DEBUG_SECTION_CHANGES_TMAX
		fprintf(stderr,"SCT tmax="FWL"\n",tmax);
		fprintf(stderr,"Section changes tmax: end\n");
	#endif
	return;
}

#if INLINE_SECTION_CHANGES_TMAX
	#define section_changes_tmax _section_changes_tmax
#else
	void section_changes_tmax(SectionChanges*);
#endif

static inline void _list_section_changes_print(GSList *list,FILE *file)
{
	fprintf(file,"List section changes print: start\n");
	for (; list; list=list->next)
		section_changes_print((SectionChanges*)list->data,file);
	fprintf(file,"List section changes print: end\n");
}

#if INLINE_LIST_SECTION_CHANGES_PRINT
	#define list_section_changes_print _list_section_changes_print
#else
	void list_section_changes_print(GSList*,FILE*);
#endif

static inline void _list_section_changes_tmax(GSList *list)
{
	#if DEBUG_LIST_SECTION_CHANGES_TMAX
		fprintf(stderr,"List section changes tmax: start\n");
		fprintf(stderr,"LSCT tmax="FWL"\n",tmax);
	#endif
	for (; list; list=list->next)
		section_changes_tmax((SectionChanges*)list->data);
	#if DEBUG_LIST_SECTION_CHANGES_TMAX
		fprintf(stderr,"LSCT tmax="FWL"\n",tmax);
		fprintf(stderr,"List section changes tmax: end\n");
	#endif
}

#if INLINE_LIST_SECTION_CHANGES_TMAX
	#define list_section_changes_tmax _list_section_changes_tmax
#else
	void list_section_changes_tmax(GSList*);
#endif

static inline int _list_section_changes_open(GSList **list,ChannelGeometry *cg)
{
	int i;
	CrossSection *cs;
	SectionChanges *sc;
	#if DEBUG_LIST_SECTION_CHANGES_OPEN
		fprintf(stderr,"List section changes open: start\n");
	#endif
	jb_slist_free(list);
	for (i=cg->n+1, cs=cg->cs+cg->n; --i>=0; --cs)
		if (cs->n && cs->type==CROSS_SECTION_TYPE_TIME)
		{
			sc=(SectionChanges*)g_try_malloc(sizeof(SectionChanges));
			if (!sc)
			{
				jbw_show_error2(gettext("Section changes list"),
					gettext("Not enough memory"));
				#if DEBUG_LIST_SECTION_CHANGES_OPEN
					fprintf(stderr,"List section changes open: end\n");
				#endif
				return 0;
			}
			sc->cs = cs;
			*list=g_slist_prepend(*list,sc);
		}
	#if DEBUG_LIST_SECTION_CHANGES_OPEN
		fprintf(stderr,"List section changes open: end\n");
	#endif
	return 1;
}

#if INLINE_LIST_SECTION_CHANGES_OPEN
	#define list_section_changes_open _list_section_changes_open
#else
	int list_section_changes_open(GSList**,ChannelGeometry*);
#endif

static inline void _section_gauge_print(SectionGauge *sg,FILE *file)
{
	fprintf(file,"Section gauge print: start\n");
	cross_section_print(sg->cs,file);
	cross_section_print(sg->csc,file);
	fprintf(file,"Section gauge print: end\n");
}

#if INLINE_SECTION_GAUGE_PRINT
	#define section_gauge_print _section_gauge_print
#else
	void section_gauge_print(SectionGauge*,FILE*);
#endif

static inline void _section_gauge_tmax(SectionGauge *sg)
{
	#if DEBUG_SECTION_GAUGE_TMAX
		fprintf(stderr,"Section gauge tmax: start\n");
		fprintf(stderr,"SGT t="FWL" tmax="FWF"\n",t,tmax);
		fprintf(stderr,"SGT list tmax="FWF"\n",sg->cs->tmax);
	#endif
	if (sg->cs->tmax < tmax)
	{
		g_mutex_lock(mutex);
		tmax = sg->cs->tmax;
		g_mutex_unlock(mutex);
	}
	#if DEBUG_SECTION_GAUGE_TMAX
		fprintf(stderr,"SGT tmax="FWF"\n",tmax);
		fprintf(stderr,"Section gauge tmax: end\n");
	#endif
	return;
}

#if INLINE_SECTION_GAUGE_TMAX
	#define section_gauge_tmax _section_gauge_tmax
#else
	void section_gauge_tmax(SectionGauge*);
#endif

static inline void _list_section_gauge_print(GSList *list,FILE *file)
{
	fprintf(file,"List section gauge print: start\n");
	for (; list; list=list->next)
		section_gauge_print((SectionGauge*)list->data,file);
	fprintf(file,"List section gauge print: end\n");
}

#if INLINE_LIST_SECTION_GAUGE_PRINT
	#define list_section_gauge_print _list_section_gauge_print
#else
	void list_section_gauge_print(GSList*,FILE*);
#endif

static inline void _list_section_gauge_tmax(GSList *list)
{
	#if DEBUG_LIST_SECTION_GAUGE_TMAX
		fprintf(stderr,"List section gauge tmax: start\n");
		fprintf(stderr,"LSGT tmax="FWL"\n",tmax);
	#endif
	for (; list; list=list->next)
		section_gauge_tmax((SectionGauge*)list->data);
	#if DEBUG_LIST_SECTION_GAUGE_TMAX
		fprintf(stderr,"LSGT tmax="FWL"\n",tmax);
		fprintf(stderr,"List section gauge tmax: end\n");
	#endif
}

#if INLINE_LIST_SECTION_GAUGE_TMAX
	#define list_section_gauge_tmax _list_section_gauge_tmax
#else
	void list_section_gauge_tmax(GSList*);
#endif

static inline int _list_section_gauge_open(GSList **list,ChannelGeometry *cg)
{
	int i;
	CrossSection *cs;
	SectionGauge *sg;
	#if DEBUG_LIST_SECTION_GAUGE_OPEN
		fprintf(stderr,"List section gauge open: start\n");
	#endif
	jb_slist_free(list);
	for (i=cg->n+1, cs=cg->cs+cg->n; --i>=0; --cs)
		if (cs->type!=CROSS_SECTION_TYPE_TIME)
		{
			sg=(SectionGauge*)g_try_malloc(sizeof(SectionGauge));
			if (!sg)
			{
				jbw_show_error2(gettext("Gauge sections list"),
					gettext("Not enough memory"));
				#if DEBUG_LIST_SECTION_GAUGE_OPEN
					fprintf(stderr,"List section gauge open: end\n");
				#endif
				return 0;
			}
			sg->csc = cg->cs + cs->control - 1;
			sg->cs = cs;
			*list=g_slist_prepend(*list,sg);
			cs->j = 0;
			cs->nt = 0;
			cs->pt = 0.;
			cs->tmax = cs->time;
		}
	#if DEBUG_LIST_SECTION_GAUGE_OPEN
		fprintf(stderr,"List section gauge open: end\n");
	#endif
	return 1;
}

#if INLINE_LIST_SECTION_GAUGE_OPEN
	#define list_section_gauge_open _list_section_gauge_open
#else
	int list_section_gauge_open(GSList**,ChannelGeometry*);
#endif

static inline int _list_variable_segments_open(GSList **list,ChannelGeometry *cg)
{
	int i;
	SectionVariable *sv;
	CrossSection *cs;
	#if DEBUG_LIST_VARIABLE_SEGMENTS_OPEN
		fprintf(stderr,"List variable segments open: start\n");
	#endif
	jb_slist_free(list);
	for (i=cg->n, cs=cg->cs+cg->n-1; --i>=0; --cs) if (cs->n || (cs+1)->n)
	{
		sv=(SectionVariable*)g_try_malloc(sizeof(SectionVariable));
		if (!sv)
		{
			jbw_show_error2(gettext("Variable sections list"),
				gettext("Not enough memory"));
			#if DEBUG_LIST_SECTION_GAUGE_OPEN
				fprintf(stderr,"List section gauge open: end\n");
			#endif
			return 0;
		}
		sv->cs = cs;
		sv->x = cg->x + i;
		*list=g_slist_prepend(*list,sv);
	}
	#if DEBUG_LIST_VARIABLE_SEGMENTS_OPEN
		fprintf(stderr,"List variable segments open: end\n");
	#endif
	return 1;
}

#if INLINE_LIST_VARIABLE_SEGMENTS_OPEN
	#define list_variable_segments_open _list_variable_segments_open
#else
	int list_variable_segments_open(GSList**,ChannelGeometry*);
#endif

#endif

