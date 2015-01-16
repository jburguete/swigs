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

/*
	BOUNDARY SPECIFICATIONS
	0: Pointwise source
		p1[0]=mass
	1: Large source
		p1[i]=time, p2[i]=discharge
*/

/**
 * \file boundary_transport.h
 * \brief Header file to define transported solute boundary conditions
 *   structures and functions.
 * \authors Javier Burguete Tolosa.
 * \copyright Copyright 2005-2014 Javier Burguete Tolosa.
 */
#ifndef BOUNDARY_TRANSPORT__H
#define BOUNDARY_TRANSPORT__H 1

#include "points.h"

/**
 * \def N_BOUNDARY_TRANSPORT_TYPES
 * \brief Macro to define the number of transported solute boundary condition
 *   types.
 */
#define N_BOUNDARY_TRANSPORT_TYPES (BOUNDARY_TRANSPORT_TYPE_Q + 1)

/**
 * \enum BoundaryTransportType
 * \brief Enumeration to define the transported solute boundary condition types.
 * \var BOUNDARY_TRANSPORT_TYPE_M
 * \brief Sudden mass solute contribution.
 * \var BOUNDARY_TRANSPORT_TYPE_Q
 * \brief Solute discharge hydrogram.
 */
enum BoundaryTransportType
{
	BOUNDARY_TRANSPORT_TYPE_M = 0,
	BOUNDARY_TRANSPORT_TYPE_Q = 1
};

/**
 * \struct BoundaryTransport
 * \brief Structure to define a transported solute boundary condition.
 */
typedef struct
{
/**
 * \var type
 * \brief type.
 * \var pos
 * \brief first channel cross section to apply the boundary condition.
 * \var pos2
 * \brief last channel cross section to apply the boundary condition.
 * \var n
 * \brief number of data.
 * \var i
 * \brief index of the first channel cross section to apply the boundary
 *   condition.
 * \var i2
 * \brief index of the last channel cross section to apply the boundary
 *   condition.
 * \var contribution
 * \brief solute mass contribution.
 * \var length
 * \brief length to apply the boundary condition.
 * \var delay
 * \brief delay to apply the boundary condition.
 * \var t
 * \brief array of dates.
 * \var p
 * \brief array of parameters data.
 * \var name
 * \brief name.
 * \var section
 * \brief name of the first cross section to apply the boundary condition.
 * \var section2
 * \brief name of the last cross section to apply the boundary condition.
 */
	int type, pos, pos2, n, i, i2;
	JBFLOAT contribution, length, delay, *p;
	JBDOUBLE *t;
	char *name, *section, *section2;
} BoundaryTransport;

extern int simulating;

static inline void _boundary_transport_print(BoundaryTransport *bt, FILE *file)
{
	int i;
	fprintf(file, "boundary_transport_print: start\n");
	fprintf(file, "BTP name=%s\n", bt->name);
	fprintf(file, "BTP name=%s\n", bt->section);
	if (bt->section2) fprintf(file, "BTP name=%s\n", bt->section2);
	fprintf(file, "BTP type=%d\n", bt->type);
	fprintf(file, "BTP length="FWF" delay="FWF"\n", bt->length, bt->delay);
	switch (bt->type)
	{
	case BOUNDARY_TRANSPORT_TYPE_M:
		fprintf(file, "BTP t="FWL" p="FWF"\n", bt->t[0], bt->p[0]);
		break;
	case BOUNDARY_TRANSPORT_TYPE_Q:
		for (i = 0; i <= bt->n; ++i)
			fprintf(file, "BTP i=%d t="FWL" p="FWF"\n", i, bt->t[i], bt->p[i]);
	}
	fprintf(file, "boundary_transport_print: end\n");
}

#if INLINE_BOUNDARY_TRANSPORT_PRINT
	#define boundary_transport_print _boundary_transport_print
#else
	void boundary_transport_print(BoundaryTransport*, FILE*);
#endif

static inline void _boundary_transport_error(BoundaryTransport *bt, char *m)
{
	char *buffer;
	#if DEBUG_BOUNDARY_TRANSPORT_ERROR
		fprintf(stderr, "boundary_transport_error: start\n");
	#endif
	buffer = message;
	message = g_strconcat(gettext("Transport boundary condition"), ": ",
		bt->name, "\n", m, NULL);
	g_free(buffer);
	#if DEBUG_BOUNDARY_TRANSPORT_ERROR
		fprintf(stderr, "boundary_transport_error: end\n");
	#endif
}

#if INLINE_BOUNDARY_TRANSPORT_ERROR
	#define boundary_transport_error _boundary_transport_error
#else
	void boundary_transport_error(BoundaryTransport*, char*);
#endif

static inline void _boundary_transport_delete(BoundaryTransport *bt)
{
	#if DEBUG_BOUNDARY_TRANSPORT_DELETE
		fprintf(stderr, "boundary_transport_delete: start\n");
	#endif
	jb_free_null((void**)&bt->t);
	jb_free_null((void**)&bt->p);
	jb_free_null((void**)&bt->name);
	jb_free_null((void**)&bt->section);
	jb_free_null((void**)&bt->section2);
	#if DEBUG_BOUNDARY_TRANSPORT_DELETE
		fprintf(stderr, "boundary_transport_delete: end\n");
	#endif
}

#if INLINE_BOUNDARY_TRANSPORT_DELETE
	#define boundary_transport_delete _boundary_transport_delete
#else
	void boundary_transport_delete(BoundaryTransport*);
#endif

static inline void _boundary_transport_init_empty(BoundaryTransport *bt)
{
	#if DEBUG_BOUNDARY_TRANSPORT_INIT_EMPTY
		fprintf(stderr, "boundary_transport_init_empty: start\n");
	#endif
	bt->name = bt->section = bt->section2 = NULL;
	bt->t = NULL;
	bt->p = NULL;
	#if DEBUG_BOUNDARY_TRANSPORT_INIT_EMPTY
		fprintf(stderr, "boundary_transport_init_empty: end\n");
	#endif
}

#if INLINE_BOUNDARY_TRANSPORT_INIT_EMPTY
	#define boundary_transport_init_empty _boundary_transport_init_empty
#else
	void boundary_transport_init_empty(BoundaryTransport*);
#endif

static inline int _boundary_transport_copy
	(BoundaryTransport *bt, BoundaryTransport *bt_copy)
{
	int i, j;
	#if DEBUG_BOUNDARY_TRANSPORT_COPY
		fprintf(stderr, "boundary_transport_copy: start\n");
		boundary_transport_print(bt_copy, stderr);
	#endif

	if (bt == bt_copy) goto exit0;

	boundary_transport_init_empty(bt);

	memcpy(bt, bt_copy, JB_POINTER_SIZE(bt->p, bt->type));

	bt->name = jb_strdup(bt_copy->name);
	if (!bt->name) goto exit1;
	bt->section = jb_strdup(bt_copy->section);
	if (!bt->section) goto exit1;
	if (bt_copy->section2)
	{
		bt->section2 = jb_strdup(bt_copy->section2);
		if (!bt->section2) goto exit1;
	}

	j = bt->n + 1;
	i = j * sizeof(JBDOUBLE);
	bt->t = (JBDOUBLE*)g_try_malloc(i);
	if (!bt->t) goto exit1;
	memcpy(bt->t, bt_copy->t, i);
	i = j * sizeof(JBFLOAT);
	bt->p = (JBFLOAT*)g_try_malloc(i);
	if (!bt->p) goto exit1;
	memcpy(bt->p, bt_copy->p, i);

exit0:
	#if DEBUG_BOUNDARY_TRANSPORT_COPY
		boundary_transport_print(bt, stderr);
		fprintf(stderr, "boundary_transport_copy: end\n");
	#endif
	return 1;

exit1:
	boundary_transport_error(bt_copy, gettext("Not enough memory"));
	boundary_transport_delete(bt);
	#if DEBUG_BOUNDARY_TRANSPORT_COPY
		fprintf(stderr, "boundary_transport_copy: end\n");
	#endif
	return 0;
}

#if INLINE_BOUNDARY_TRANSPORT_COPY
	#define boundary_transport_copy _boundary_transport_copy
#else
	int boundary_transport_copy(BoundaryTransport*, BoundaryTransport*);
#endif

static inline int _boundary_transport_open_xml
	(BoundaryTransport *bt, xmlNode *node)
{
	int i, j;
	char *buffer;
	JBDOUBLE t;
	JBFLOAT p;
	FILE *file;
	#if DEBUG_BOUNDARY_TRANSPORT_OPEN_XML
		fprintf(stderr, "boundary_transport_open_xml: start\n");
	#endif

	boundary_transport_init_empty(bt);

	if (!xmlHasProp(node, XML_NAME))
	{
		message = g_strconcat(gettext("Transport boundary condition"), "\n",
			gettext("Not name"), NULL);
		goto exit1;
	}
	buffer = (char*)xmlGetProp(node, XML_NAME);
	bt->name = jb_strdup(buffer);
	if (!bt->name)
	{
		message = g_strconcat(gettext("Transport boundary condition"), ": ",
			buffer, "\n", gettext("Not enough memory"), NULL);
		xmlFree(buffer);
		goto exit2;
	}
	xmlFree(buffer);
	#if DEBUG_BOUNDARY_TRANSPORT_OPEN_XML
		fprintf(stderr, "BTOX name=%s\n", bt->name);
	#endif


	if (!xmlHasProp(node, XML_INITIAL))
	{
		boundary_transport_error(bt, gettext("Bad position"));
		goto exit1;
	}
	buffer = (char*)xmlGetProp(node, XML_INITIAL);
	bt->section = jb_strdup(buffer);
	if (!bt->section)
	{
		boundary_transport_error(bt, gettext("Bad position"));
		goto exit2;
	}
	xmlFree(buffer);
	#if DEBUG_BOUNDARY_TRANSPORT_OPEN_XML
		fprintf(stderr, "BTOX section=%s\n", bt->section);
	#endif

	if (xmlHasProp(node, XML_FINAL))
	{
		buffer = (char*)xmlGetProp(node, XML_FINAL);
		bt->section2 = jb_strdup(buffer);
		if (!bt->section2)
		{
			boundary_transport_error(bt, gettext("Bad position"));
			goto exit2;
		}
		xmlFree(buffer);
		#if DEBUG_BOUNDARY_TRANSPORT_OPEN_XML
			fprintf(stderr, "BTOX section2=%s\n", bt->section2);
		#endif
	}

	if (!xmlHasProp(node, XML_TYPE))
	{
		boundary_transport_error(bt, gettext("Unknow type"));
		goto exit1;
	}
	buffer = (char*)xmlGetProp(node, XML_TYPE);
	#if DEBUG_BOUNDARY_TRANSPORT_OPEN_XML
		fprintf(stderr, "BTOX type=%s\n", buffer);
	#endif

	bt->delay = jb_xml_node_get_float_with_default(node, XML_DELAY, &i, 0.);
	if (!i)
	{
		boundary_transport_error(bt, gettext("Bad delay"));
		goto exit1;
	}
	#if DEBUG_BOUNDARY_TRANSPORT_OPEN_XML
		fprintf(stderr, "BTOX delay="FWF"\n", bt->delay);
	#endif

	if (!xmlStrcmp((const xmlChar*)buffer, XML_M))
	{
		bt->type = 0;
		if (!xmlHasProp(node, XML_TIME) || !xmlHasProp(node, XML_MASS))
		{
			boundary_transport_error(bt, gettext("Bad defined"));
			goto exit2;
		}
		t = jb_xml_node_get_time(node, XML_TIME, &i);
		p = jb_xml_node_get_float(node, XML_MASS, &j);
		if (i != 1 || j != 1)
		{
			boundary_transport_error(bt, gettext("Bad defined"));
			goto exit2;
		}
		bt->t = (JBDOUBLE*)g_try_malloc(sizeof(JBDOUBLE));
		if (!bt->t)
		{
			boundary_transport_error(bt, gettext("Not enough memory"));
			goto exit2;
		}
		bt->t[0] = t;
		bt->p = (JBFLOAT*)g_try_malloc(sizeof(JBFLOAT));
		if (!bt->p)
		{
			boundary_transport_error(bt, gettext("Not enough memory"));
			goto exit2;
		}
		bt->p[0] = p;
		bt->n = 0;
	}
	else if (!xmlStrcmp((const xmlChar*)buffer, XML_Q))
	{
		bt->type = 1;
		xmlFree(buffer);
		file = jb_xml_node_get_content_file(node, &buffer);
		if (!file)
		{
			boundary_transport_error(bt, gettext("Not enough memory"));
			goto exit1;
		}
		j = -1;
		do
		{
			t = jb_get_time_file(file, &i);
			if (i != 6) break;
			if (fscanf(file, FRF, &p) < 1) break;
			++j;
			bt->t = (JBDOUBLE*)jb_try_realloc
				(bt->t, (j + 1) * sizeof(JBDOUBLE));
			bt->p = (JBFLOAT*)jb_try_realloc(bt->p, (j + 1) * sizeof(JBFLOAT));
			if (!bt->t || !bt->p)
			{
				boundary_transport_error(bt, gettext("Not enough memory"));
				goto exit4;
			}
			bt->t[j] = t;
			bt->p[j] = p;
		}
		while (1);
		bt->n = j;
		fclose(file);
	}
	else
	{
		boundary_transport_error(bt, gettext("Unknow type"));
		goto exit2;
	}

	#if DEBUG_BOUNDARY_TRANSPORT_OPEN_XML
		boundary_transport_print(bt, stderr);
		fprintf(stderr, "boundary_transport_open_xml: end\n");
	#endif
	xmlFree(buffer);
	return 1;

exit4:
	fclose(file);

exit2:
	xmlFree(buffer);

exit1:
	boundary_transport_delete(bt);
	#if DEBUG_BOUNDARY_TRANSPORT_OPEN_XML
		fprintf(stderr, "boundary_transport_open_xml: end\n");
	#endif
	return 0;
}

#if INLINE_BOUNDARY_TRANSPORT_OPEN_XML
	#define boundary_transport_open_xml _boundary_transport_open_xml
#else
	int boundary_transport_open_xml(BoundaryTransport*, xmlNode*);
#endif

static inline void _boundary_transport_save_xml
	(BoundaryTransport *bt, xmlNode *node)
{
	int i;
	char *buffer, *buffer2, str[JB_BUFFER_SIZE];
	const xmlChar *bttype[] = {XML_M, XML_Q};

	#if DEBUG_BOUNDARY_TRANSPORT_SAVE_XML
		fprintf(stderr, "boundary_transport_save_xml: start\n");
		boundary_transport_print(bt, stderr);
	#endif

	xmlSetProp(node, XML_NAME, (const xmlChar*)bt->name);
	xmlSetProp(node, XML_INITIAL, (const xmlChar*)bt->section);
	if (bt->section2) xmlSetProp(node, XML_FINAL, (const xmlChar*)bt->section2);
	jb_xml_node_set_float_with_default(node, XML_DELAY, bt->delay, 0.);
	xmlSetProp(node, XML_TYPE, bttype[bt->type]);

	switch (bt->type)
	{
	case BOUNDARY_TRANSPORT_TYPE_M:
		jb_xml_node_set_time(node, XML_TIME, bt->t[0]);
		jb_xml_node_set_float(node, XML_MASS, bt->p[0]);
		break;
	case BOUNDARY_TRANSPORT_TYPE_Q:
		buffer = NULL;
		for (i = 0; i <= bt->n; ++i)
		{
			buffer2 = jb_set_time(bt->t[i]);
			snprintf(str, JB_BUFFER_SIZE, "\n      %s " FWF, buffer2, bt->p[i]);
			g_free(buffer2);
			if (!i) buffer2 = g_strdup(str);
			else buffer2 = g_strconcat(buffer, str, NULL);
			g_free(buffer);
			buffer = buffer2;
		}
		buffer2 = g_strconcat(buffer, "\n  ", NULL);
		xmlNodeSetContent(node, (const xmlChar*)buffer2);
		g_free(buffer);
		g_free(buffer2);
		break;
	}

	#if DEBUG_BOUNDARY_TRANSPORT_SAVE_XML
		fprintf(stderr, "boundary_transport_save_xml: end\n");
	#endif
}

#if INLINE_BOUNDARY_TRANSPORT_SAVE_XML
	#define boundary_transport_save_xml _boundary_transport_save_xml
#else
	void boundary_transport_save_xml(BoundaryTransport*, xmlNode*);
#endif

static inline JBDOUBLE _boundary_transport_parameter
	(BoundaryTransport *bt, JBDOUBLE t)
{
	if (!simulating) return bt->p[0];
	return jbm_darray_farray_interpolate(t - bt->delay, bt->t, bt->p, bt->n);
}

#if INLINE_BOUNDARY_TRANSPORT_PARAMETER
	#define boundary_transport_parameter _boundary_transport_parameter
#else
	JBDOUBLE boundary_transport_parameter(BoundaryTransport*, JBDOUBLE);
#endif

#endif
