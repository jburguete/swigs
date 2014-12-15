/*
SWIGS (Shallow Water in Irregular Geometries Simulator): a software to simulate
transient or steady flows with solute transport in channels, channel networks
and rivers.

Copyright 2005 - 2014 Javier Burguete Tolosa.

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

/**
 * \file channel_geometry.h
 * \brief Header file to define channel geometry structures and methods.
 * \authors Javier Burguete Tolosa.
 * \copyright Copyright 2005 - 2014 Javier Burguete Tolosa.
 */
#ifndef CHANNEL_GEOMETRY__H
#define CHANNEL_GEOMETRY__H 1

#include "cross_section.h"

/**
 * \struct ChannelGeometry
 * \brief Structure to define a channel geometry.
 */
typedef struct
{
/**
 * \var n
 * \brief number of cross sections.
 * \var x
 * \brief array of cross section longitudinal distances to the inlet.
 * \var cs
 * \brief array of cross sections.
 */
	int n;
	JBFLOAT *x;
	CrossSection *cs;
} ChannelGeometry;

static inline void _channel_geometry_print(ChannelGeometry *cg, FILE *file)
{
	int i;
	fprintf(file, "channel_geometry_print: start\n");
	for (i = 0; i <= cg->n; ++i) cross_section_print(cg->cs + i, file);
	fprintf(file, "channel_geometry_print: end\n");
}

#if INLINE_CHANNEL_GEOMETRY_PRINT
	#define channel_geometry_print _channel_geometry_print
#else
	void channel_geometry_print(ChannelGeometry*, FILE*);
#endif

static inline void _channel_geometry_error(char *m)
{
	char *buffer;
	#if DEBUG_CHANNEL_GEOMETRY_ERROR
		fprintf(stderr, "channel_geometry_error: start\n");
	#endif
	buffer = message;
	message = g_strconcat(gettext("Geometry"), "\n", m, NULL);
	g_free(buffer);
	#if DEBUG_CHANNEL_GEOMETRY_ERROR
		fprintf(stderr, "channel_geometry_error: end\n");
	#endif
}

#if INLINE_CHANNEL_GEOMETRY_ERROR
	#define channel_geometry_error _channel_geometry_error
#else
	void channel_geometry_error(char*);
#endif

static inline void _channel_geometry_delete(ChannelGeometry *cg)
{
	int i;
	#if DEBUG_CHANNEL_GEOMETRY_DELETE
		fprintf(stderr, "channel_geometry_delete: start\n");
	#endif
	for (i = 0; i <= cg->n; ++i) cross_section_delete(cg->cs + i); 
	jb_free_null((void**)&cg->cs);
	jb_free_null((void**)&cg->x);
	cg->n = -1;
	#if DEBUG_CHANNEL_GEOMETRY_DELETE
		fprintf(stderr, "channel_geometry_delete: end\n");
	#endif
}

#if INLINE_CHANNEL_GEOMETRY_DELETE
	#define channel_geometry_delete _channel_geometry_delete
#else
	void channel_geometry_delete(ChannelGeometry*);
#endif

static inline void _channel_geometry_update_x(ChannelGeometry *cg, int i)
{
	register int j;
	#if DEBUG_CHANNEL_GEOMETRY_UPDATE_X
		fprintf(stderr, "channel_geometry_update_x: start\n");
	#endif
	if (i == 0) cg->x[0] = 0.; else --i;
	for (j = i; ++j <= cg->n;)
		cg->x[j] = cg->x[j - 1] + jbm_v2_length
			(cg->cs[j].x, cg->cs[j].y, cg->cs[j - 1].x, cg->cs[j - 1].y);
	#if DEBUG_CHANNEL_GEOMETRY_UPDATE_X
		for (j = 0; j <= cg->n; ++j)
			fprintf(stderr, "CGAX i=%d x="FWF"\n", j, cg->x[j]);
		fprintf(stderr, "channel_geometry_update_x: end\n");
	#endif
}

#if INLINE_CHANNEL_GEOMETRY_UPDATE_X
	#define channel_geometry_update_x _channel_geometry_update_x
#else
	void channel_geometry_update_x(ChannelGeometry*, int);
#endif

static inline int _channel_geometry_insert_section(ChannelGeometry *cg, int i)
{
	int j;
	JBFLOAT *x;
	CrossSection *rcs;
	#if DEBUG_CHANNEL_GEOMETRY_INSERT_SECTION
		fprintf(stderr, "channel_geometry_insert_section: start\n");
	#endif
	j = cg->n + 2;
	rcs = (CrossSection*)g_try_realloc(cg->cs, j * sizeof(CrossSection));
	if (!rcs) goto error_insert;
	x = (JBFLOAT*)g_try_realloc(cg->x, j * sizeof(JBFLOAT));
	if (!x) goto error_insert;
	cg->cs = rcs;
	cg->x = x;
	++cg->n;
	--j;
	rcs += j;
	rcs->n = -1;
	rcs->t = NULL;
	rcs->ts = NULL;
	rcs->name = NULL;
	#if DEBUG_CHANNEL_GEOMETRY_INSERT_SECTION
		fprintf(stderr, "CGIS: i=%d j=%d\n", i, j);
	#endif
	for (; j-- >= i; --rcs)
	{
		cross_section_delete(rcs);
		if (!cross_section_copy(rcs, rcs - 1)) goto error_insert;
	}
	channel_geometry_update_x(cg, i);
	#if DEBUG_CHANNEL_GEOMETRY_INSERT_SECTION
		fprintf(stderr, "channel_geometry_insert_section: end\n");
	#endif
	return 1;

error_insert:
	channel_geometry_error(gettext("Not enough memory"));
	#if DEBUG_CHANNEL_GEOMETRY_INSERT_SECTION
		fprintf(stderr, "channel_geometry_insert_section: end\n");
	#endif
	return 0;
}

#if INLINE_CHANNEL_GEOMETRY_INSERT_SECTION
	#define channel_geometry_insert_section _channel_geometry_insert_section
#else
	int channel_geometry_insert_section(ChannelGeometry*, CrossSection*, int);
#endif

static inline void _channel_geometry_remove_section(ChannelGeometry *cg, int i)
{
	register int j;
	register CrossSection *cs = cg->cs + i;
	#if DEBUG_CHANNEL_GEOMETRY_REMOVE_SECTION
		fprintf(stderr, "channel_geometry_remove_section: start\n");
	#endif
	for (j = i; j<cg->n; ++j, ++cs)
	{
		cross_section_delete(cs);
		cross_section_copy(cs, cs + 1);
	}
	cg->cs = g_try_realloc(cg->cs, j * sizeof(CrossSection));
	cg->x = g_try_realloc(cg->x, j * sizeof(JBFLOAT));
	--cg->n;
	channel_geometry_update_x(cg, i);
	#if DEBUG_CHANNEL_GEOMETRY_REMOVE_SECTION
		fprintf(stderr, "channel_geometry_remove_section: end\n");
	#endif
}

#if INLINE_CHANNEL_GEOMETRY_REMOVE_SECTION
	#define channel_geometry_remove_section _channel_geometry_remove_section
#else
	void channel_geometry_remove_section(ChannelGeometry*, int);
#endif

static inline int _channel_geometry_copy
	(ChannelGeometry *cg, ChannelGeometry *cg_copy)
{
	register int i;

	#if DEBUG_CHANNEL_GEOMETRY_COPY
		fprintf(stderr, "channel_geometry_copy: start\n");
		channel_geometry_print(cg_copy, stderr);
	#endif
	
	if (cg == cg_copy) goto exit0;

	cg->n = -1;
	cg->x = 0;
	cg->cs = 0;

	cg->x = (JBFLOAT*)g_try_malloc((cg_copy->n + 1) * sizeof(JBFLOAT));
	cg->cs = (CrossSection*)g_try_malloc
		((cg_copy->n + 1) * sizeof(CrossSection));
	if (!cg->x || !cg->cs) goto exit1;
	for (i = 0; i <= cg_copy->n; ++i)
	{
		if (!cross_section_copy(cg->cs + i, cg_copy->cs + i))
		{
			channel_geometry_error(message);
			goto exit2;
		}
		cg->x[i] = cg_copy->x[i];
		++cg->n;
	}

exit0:
	#if DEBUG_CHANNEL_GEOMETRY_COPY
		channel_geometry_print(cg, stderr);
		fprintf(stderr, "channel_geometry_copy: end\n");
	#endif
	return 1;

exit1:
	channel_geometry_error(gettext("Not enough memory"));

exit2:
	channel_geometry_delete(cg);
	#if DEBUG_CHANNEL_GEOMETRY_COPY
		fprintf(stderr, "channel_geometry_copy: end\n");
	#endif
	return 0;
}

#if INLINE_CHANNEL_GEOMETRY_COPY
	#define channel_geometry_copy _channel_geometry_copy
#else
	int channel_geometry_copy(ChannelGeometry*, ChannelGeometry*);
#endif

static inline int _channel_geometry_open_xml
	(ChannelGeometry *cg, xmlNode *node)
{
	int j;

	#if DEBUG_CHANNEL_GEOMETRY_OPEN_XML
		fprintf(stderr, "channel_geometry_open_xml: start\n");
		fprintf(stderr, "CGOX node = %s\n", node->name);
	#endif
	
	cg->x = 0;
	cg->cs = 0;

	cg->n = -1;

	for (j = 0, node = node->children; node; ++j, node = node->next)
	{
		cg->cs = (CrossSection*)
			jb_try_realloc(cg->cs, (j + 1) * sizeof(CrossSection));
		cg->x = (JBFLOAT*)jb_try_realloc(cg->x, (j + 1) * sizeof(JBFLOAT));
		if (!cg->cs || !cg->x)
		{
			channel_geometry_error(gettext("Not enough memory"));
			goto exit0;
		}
		if (!cross_section_open_xml(cg->cs + j, node))
		{
			channel_geometry_error(message);
			goto exit0;
		}
		++cg->n;
	}
	if (j < 2)
	{
		channel_geometry_error(gettext("Bad defined"));
		goto exit0;
	}
	channel_geometry_update_x(cg, 0);
	#if DEBUG_CHANNEL_GEOMETRY_OPEN_XML
		channel_geometry_print(cg, stderr);
		fprintf(stderr, "channel_geometry_open_xml: end\n");
	#endif
	return j;

exit0:
	channel_geometry_delete(cg);
	#if DEBUG_CHANNEL_GEOMETRY_OPEN_XML
		fprintf(stderr, "channel_geometry_open_xml: end\n");
	#endif
	return 0;
}

#if INLINE_CHANNEL_GEOMETRY_OPEN_XML
	#define channel_geometry_open_xml _channel_geometry_open_xml
#else
	int channel_geometry_open_xml(ChannelGeometry*, xmlNode*);
#endif

static inline void _channel_geometry_save_xml
	(ChannelGeometry *cg, xmlNode *node)
{
	int i;
	xmlNode *child;

	#if DEBUG_CHANNEL_GEOMETRY_SAVE_XML
		fprintf(stderr, "channel_geometry_save_xml: start\n");
	#endif

	for (i = 0; i <= cg->n; ++i)
	{
		child = xmlNewChild(node, 0, XML_CROSS_SECTION, 0);
		cross_section_save_xml(cg->cs + i, child);
	}

	#if DEBUG_CHANNEL_GEOMETRY_SAVE_XML
		fprintf(stderr, "channel_geometry_save_xml: end\n");
	#endif
}

#if INLINE_CHANNEL_GEOMETRY_SAVE_XML
	#define channel_geometry_save_xml _channel_geometry_save_xml
#else
	void channel_geometry_save_xml(ChannelGeometry*, xmlNode*);
#endif

#endif
