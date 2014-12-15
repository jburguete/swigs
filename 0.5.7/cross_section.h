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

/**
 * \file cross_section.h
 * \brief Header file to define cross section structures and methods.
 * \authors Javier Burguete Tolosa.
 * \copyright Copyright 2005-2014 Javier Burguete Tolosa.
 */
#ifndef CROSS_SECTION__H
#define CROSS_SECTION__H 1

#include "transient_section.h"

/**
 * \enum CrossSectionType
 * \brief Enumeration to define cross section types.
 * \var CROSS_SECTION_TYPE_TIME
 * \brief Variable in time cross section.
 * \var CROSS_SECTION_TYPE_CONTROL_Q
 * \brief Controled by discharge cross section.
 * \var CROSS_SECTION_TYPE_CONTROL_H
 * \brief Controled by depth cross section.
 * \var CROSS_SECTION_TYPE_CONTROL_Z
 * \brief Controled by surface level cross section.
 */
enum CrossSectionType
{
	CROSS_SECTION_TYPE_TIME = 0,
	CROSS_SECTION_TYPE_CONTROL_Q = 1,
	CROSS_SECTION_TYPE_CONTROL_H = 2,
	CROSS_SECTION_TYPE_CONTROL_Z = 3,
};

/**
 * \struct CrossSection
 * \brief Structure to define a cross section.
 */
typedef struct
{
/**
 * \var n
 * \brief transient sections number.
 * \var type
 * \brief type.
 * \var control
 * \brief number of system cross section to measure the variable to control.
 * \var control_channel
 * \brief number of channel where to measure the variable to control.
 * \var control_section
 * \brief number of system cross section to measure the variable to control.
 * \var i
 * \brief number of mesh cross section.
 * \var j
 * \brief number of current transient section.
 * \var nt
 * \brief number of times that the control parameter has been obtained.
 * \var x
 * \brief position x-coordinate.
 * \var y
 * \brief position y-coordinate.
 * \var angle
 * \brief angle of transient sections.
 * \var parameter
 * \brief objetive parameter of the control.
 * \var time
 * \brief time of integration of control variable.
 * \var tolerance
 * \brief tolerance to control requirements.
 * \var tmax
 * \brief time when the control will actuate.
 * \var pt
 * \brief integrated parameter at the control.
 * \var t
 * \brief times array associated to every transient section.
 * \var ts
 * \brief transient sections array.
 * \var name
 * \brief name.
 */
	int n, type, control, control_channel, control_section, i, j, nt;
	JBFLOAT x, y, angle, parameter, time, tolerance, tmax, pt;
	JBFLOAT *t;
	TransientSection *ts;
	char *name;
} CrossSection;

#define NEW_GEOMETRY_SIZE 2

extern CrossSection csnew[NEW_GEOMETRY_SIZE];

static inline void _cross_section_print(CrossSection *cs, FILE *file)
{
	int i;
	fprintf(file, "cross_section_print: start\n");
	fprintf(file, "%s\n", cs->name);
	fprintf(file, "type=%d\n", cs->type);
	fprintf(file, "x="FWF" y="FWF" angle="FWF"\n", cs->x, cs->y, cs->angle);
	if (cs->type != CROSS_SECTION_TYPE_TIME)
		fprintf(file, "control=%d parameter="FWF" time="FWF" tolerance="FWF"\n",
			cs->control, cs->parameter, cs->time, cs->tolerance);
	for (i = 0; i <= cs->n; ++i) transient_section_print(cs->ts+i, file);
	fprintf(file, "cross_section_print: end\n");
}

#if INLINE_CROSS_SECTION_PRINT
	#define cross_section_print _cross_section_print
#else
	void cross_section_print(CrossSection*, FILE*);
#endif

static inline void _cross_section_error(CrossSection *cs, char *m)
{
	char *buffer;
	#if DEBUG_CROSS_SECTION_ERROR
		fprintf(stderr, "cross_section_error: start\n");
	#endif
	buffer = message;
	message
		= g_strconcat(gettext("Cross section"), ": ", cs->name, "\n", m, NULL);
	g_free(buffer);
	#if DEBUG_CROSS_SECTION_ERROR
		fprintf(stderr, "cross_section_error: end\n");
	#endif
}

#if INLINE_CROSS_SECTION_ERROR
	#define cross_section_error _cross_section_error
#else
	void cross_section_error(CrossSection*, char*);
#endif

static inline void _cross_section_delete(CrossSection *cs)
{
	register int i;
	#if DEBUG_CROSS_SECTION_DELETE
		fprintf(stderr, "cross_section_delete: start\n");
	#endif
	for (i = 0; i <= cs->n; ++i) transient_section_delete(cs->ts+i); 
	jb_free_null((void**)&cs->ts);
	jb_free_null((void**)&cs->t);
	jb_free_null((void**)&cs->name);
	cs->n = -1;
	#if DEBUG_CROSS_SECTION_DELETE
		fprintf(stderr, "cross_section_delete: end\n");
	#endif
}

#if INLINE_CROSS_SECTION_DELETE
	#define cross_section_delete _cross_section_delete
#else
	void cross_section_delete(CrossSection*);
#endif

static inline int _cross_section_create(CrossSection *cs, int n, char *name)
{
	#if DEBUG_CROSS_SECTION_CREATE
		fprintf(stderr, "cross_section_create: start\n");
	#endif
	cs->n = -1;
	cs->t = NULL;
	cs->ts = NULL;
	cs->name = jb_strdup(name);
	if (!cs->name) goto exit2;
	cs->ts
		= (TransientSection*)g_try_malloc((n + 1) * sizeof(TransientSection));
	if (!cs->ts) goto exit1;
	cs->t = (JBFLOAT*)g_try_malloc((n + 1) * sizeof(JBFLOAT));
	if (cs->t)
	{
		#if DEBUG_CROSS_SECTION_CREATE
			fprintf(stderr, "cross_section_create: end\n");
		#endif
		return 1;
	}

exit1:
	cross_section_delete(cs);
exit2:
	message = g_strconcat(gettext("Cross section"), ": ", name, "\n",
		gettext("Not enough memory"), NULL);
	#if DEBUG_CROSS_SECTION_CREATE
		fprintf(stderr, "cross_section_create: end\n");
	#endif
	return 0;
}

#if INLINE_CROSS_SECTION_CREATE
	#define cross_section_create _cross_section_create
#else
	int cross_section_create(CrossSection*, int, char*);
#endif

static inline int _cross_section_insert_transient
	(CrossSection *cs, int position)
{
	int i;
	TransientSection *ts;
	#if DEBUG_CROSS_SECTION_INSERT_TRANSIENT
		fprintf(stderr, "cross_section_insert_transient: start\n");
	#endif
	cs->t = (JBFLOAT*)jb_realloc(cs->t, (cs->n + 2) * sizeof(JBFLOAT));
	if (!cs->t) goto error_insert;
	cs->ts = (TransientSection*)
		jb_realloc(cs->ts, (cs->n + 2) * sizeof(TransientSection));
	if (!cs->ts) goto error_insert;
	++cs->n;
	#if DEBUG_CROSS_SECTION_INSERT_TRANSIENT
		fprintf(stderr, "cross_section_insert_transient: n=%d position=%d\n",
			cs->n, position);
	#endif
	ts = cs->ts + cs->n;
	ts->data = NULL;
	ts->name = NULL;
	for (i = cs->n; i >= position; --i)
	{
		cs->t[i] = cs->t[i - 1];
		ts = cs->ts + i;
		transient_section_delete(ts);
		if (!transient_section_copy(ts, ts - 1))
			goto error_insert;
	}
	#if DEBUG_CROSS_SECTION_INSERT_TRANSIENT
		fprintf(stderr, "cross_section_insert_transient: end\n");
	#endif
	return 1;

error_insert:
	cross_section_error(cs, gettext("Not enough memory"));
	cross_section_delete(cs);
	#if DEBUG_CROSS_SECTION_INSERT_TRANSIENT
		fprintf(stderr, "cross_section_insert_transient: end\n");
	#endif
	return 0;
}

#if INLINE_CROSS_SECTION_INSERT_TRANSIENT
	#define cross_section_insert_transient _cross_section_insert_transient
#else
	int cross_section_insert_transient(CrossSection*, int);
#endif

static inline void _cross_section_remove_transient
	(CrossSection *cs, int position)
{
	int i;
	TransientSection *ts;
	#if DEBUG_CROSS_SECTION_REMOVE_TRANSIENT
		fprintf(stderr, "cross_section_remove_transient: start\n");
	#endif
	transient_section_delete(cs->ts + position);
	for (i = position; i < cs->n; ++i)
	{
		cs->t[i] = cs->t[i + 1];
		ts = cs->ts + i;
		transient_section_delete(ts);
		transeint_section_copy(ts, ts + 1);
	}
	cs->t = (JBFLOAT*)g_realloc(cs->t, cs->n * sizeof(JBFLOAT));
	cs->ts =
		(TransientSection*)g_realloc(cs->ts, cs->n * sizeof(TransientSection));
	--cs->n;
	#if DEBUG_CROSS_SECTION_REMOVE_TRANSIENT
		fprintf(stderr, "cross_section_remove_transient: end\n");
	#endif
}

#if INLINE_CROSS_SECTION_REMOVE_TRANSIENT
	#define cross_section_remove_transient _cross_section_remove_transient
#else
	void cross_section_remove_transient(CrossSection*, int);
#endif

static inline int _cross_section_copy(CrossSection *cs, CrossSection *cs_copy)
{
	register int i;

	#if DEBUG_CROSS_SECTION_COPY
		fprintf(stderr, "cross_section_copy: start\n");
		cross_section_print(cs_copy, stderr);
	#endif
	
	if (cs == cs_copy) goto exit0;
	memcpy(cs, cs_copy, (size_t)&cs->t - (size_t)cs);
	if (!cross_section_create(cs, cs_copy->n, cs_copy->name)) goto exit3;

	for (i = 0; i <= cs_copy->n; ++i)
	{
		if (!transient_section_copy(cs->ts + i, cs_copy->ts + i)) goto exit2;
		cs->t[i] = cs_copy->t[i];
		++cs->n;
	}

exit0:
	#if DEBUG_CROSS_SECTION_COPY
		cross_section_print(cs, stderr);
		fprintf(stderr, "cross_section_copy: end\n");
	#endif
	return 1;

exit2:
	cross_section_error(cs, message);
	cross_section_delete(cs);

exit3:
	#if DEBUG_CROSS_SECTION_COPY
		fprintf(stderr, "cross_section_copy: end\n");
	#endif
	return 0;
}

#if INLINE_CROSS_SECTION_COPY
	#define cross_section_copy _cross_section_copy
#else
	int cross_section_copy(CrossSection*, CrossSection*);
#endif

static inline int _cross_section_order(CrossSection *cs)
{
	register int i;
	register JBFLOAT *t;
	#if DEBUG_CROSS_SECTION_ORDER
		fprintf(stderr, "cross_section_order: start\n");
	#endif
	for (i = 0, t = cs->t; ++i <= cs->n;) if (t[i] <= t[i - 1])
	{
		cross_section_error(cs, gettext("Bad order"));
		#if DEBUG_CROSS_SECTION_ORDER
			fprintf(stderr, "cross_section_order: end\n");
		#endif
		return 0;
	}
	#if DEBUG_CROSS_SECTION_ORDER
		fprintf(stderr, "cross_section_order: end\n");
	#endif
	return 1;
}

#if INLINE_CROSS_SECTION_ORDER
	#define cross_section_order _cross_section_order
#else
	int cross_section_order(CrossSection*);
#endif

static inline int _cross_section_open_xml(CrossSection *cs, xmlNode *node)
{
	int j;
	char *buffer;

	#if DEBUG_CROSS_SECTION_OPEN_XML
		fprintf(stderr, "cross_section_open_xml: start\n");
	#endif

	j = 0;
	cs->n = -1;
	cs->ts = 0;
	cs->t = 0;
	cs->name = 0;

	if (xmlStrcmp(node->name, XML_CROSS_SECTION)) goto exit0;

	if (!xmlHasProp(node, XML_NAME))
	{
		message = g_strconcat(gettext("Cross section"), "\n",
			gettext("Not name"), NULL);
		goto exit2;
	}
	buffer = (char*)xmlGetProp(node, XML_NAME);
	#if DEBUG_CROSS_SECTION_OPEN_XML
		fprintf(stderr, "CSOX name=%s\n", buffer);
	#endif
	cs->name = jb_strdup(buffer);
	if (!cs->name)
	{

		message = g_strconcat(gettext("Cross section"), ": ", buffer, "\n",
			gettext("Not enough memory"), NULL);
		goto exit2;
	}
	xmlFree(buffer);

	if (!xmlHasProp(node, XML_X))
	{
		cross_section_error(cs, gettext("Bad defined"));
		goto exit0;
	}
	cs->x = jb_xml_node_get_float(node, XML_X, &j);
	if (j != 1)
	{
		cross_section_error(cs, gettext("Bad defined"));
		goto exit0;
	}
	cs->y = jb_xml_node_get_float_with_default(node, XML_Y, &j, 0.);
	if (j != 1)
	{
		cross_section_error(cs, gettext("Bad defined"));
		goto exit0;
	}
	cs->angle = jb_xml_node_get_float_with_default(node, XML_ANGLE, &j, 90.);
	if (j != 1)
	{
		cross_section_error(cs, gettext("Bad defined"));
		goto exit0;
	}
	if (xmlHasProp(node, XML_TYPE))
	{
		buffer = (char*)xmlGetProp(node, XML_TYPE);
		if (!g_ascii_strcasecmp(buffer, (char*)XML_TIME))
			cs->type = CROSS_SECTION_TYPE_TIME;
		else if (!g_ascii_strcasecmp(buffer, (char*)XML_DISCHARGE))
		{
			cs->type = CROSS_SECTION_TYPE_CONTROL_Q;
			if (xmlHasProp(node, XML_DISCHARGE))
			{
				cs->parameter = jb_xml_node_get_float(node, XML_DISCHARGE, &j);
				if (j != 1)
				{
					cross_section_error(cs, gettext("Bad defined"));
					goto exit0;
				}
			}
			else cs->parameter = 0.;
		}
		else if (!g_ascii_strcasecmp(buffer, (char*)XML_DEPTH))
		{
			cs->type = CROSS_SECTION_TYPE_CONTROL_H;
			if (xmlHasProp(node, XML_DEPTH))
			{
				cs->parameter = jb_xml_node_get_float(node, XML_DEPTH, &j);
				if (j != 1)
				{
					cross_section_error(cs, gettext("Bad defined"));
					goto exit0;
				}
			}
			else cs->parameter = 0.;
		}
		else if (!g_ascii_strcasecmp(buffer, (char*)XML_LEVEL))
		{
			cs->type = CROSS_SECTION_TYPE_CONTROL_Z;
			if (xmlHasProp(node, XML_LEVEL))
			{
				cs->parameter = jb_xml_node_get_float(node, XML_LEVEL, &j);
				if (j != 1)
				{
					cross_section_error(cs, gettext("Bad defined"));
					goto exit0;
				}
			}
			else cs->parameter = 0.;
		}
		else
		{
			cross_section_error(cs, gettext("Unknow type"));
			goto exit0;
		}
	}
	else cs->type = CROSS_SECTION_TYPE_TIME;
	cs->control = jb_xml_node_get_int_with_default(node, XML_CONTROL, &j, 0);
	if (j != 1)
	{
		cross_section_error(cs, gettext("Bad defined"));
		goto exit0;
	}
	cs->time = jb_xml_node_get_float_with_default(node, XML_TIME, &j, 0.);
	if (j != 1)
	{
		cross_section_error(cs, gettext("Bad defined"));
		goto exit0;
	}
	cs->tolerance =
		jb_xml_node_get_float_with_default(node, XML_TOLERANCE, &j, 0.);
	if (j != 1)
	{
		cross_section_error(cs, gettext("Bad defined"));
		goto exit0;
	}

	for (j = 0, node = node->children; node; ++j, node = node->next)
	{
		cs->ts = (TransientSection*)
			jb_try_realloc(cs->ts, (j + 1) * sizeof(TransientSection));
		cs->t = (JBFLOAT*)jb_try_realloc(cs->t, (j + 1) * sizeof(JBFLOAT));
		if (!cs->ts || !cs->t)
		{
			cross_section_error(cs, gettext("Not enough memory"));
			goto exit0;
		}
		if (!transient_section_open_xml
			(cs->ts+j, node, cs->x, cs->y, cs->angle))
		{
			cross_section_error(cs, message);
			goto exit0;
		}
		cs->t[j] = cs->ts[j].t;
		++cs->n;
	}

	if (j <= 0)
	{
		cross_section_error(cs, gettext("Bad defined"));
		goto exit0;
	}

	if (cs->type==CROSS_SECTION_TYPE_TIME && !cross_section_order(cs))
		goto exit0;

	#if DEBUG_CROSS_SECTION_OPEN_XML
		cross_section_print(cs, stderr);
		fprintf(stderr, "cross_section_open_xml: end\n");
	#endif
	return 1;

exit0:
	cross_section_delete(cs);

exit2:
	#if DEBUG_CROSS_SECTION_OPEN_XML
		fprintf(stderr, "cross_section_open_xml: end\n");
	#endif
	return 0;
}

#if INLINE_CROSS_SECTION_OPEN_XML
	#define cross_section_open_xml _cross_section_open_xml
#else
	int cross_section_open_xml(CrossSection*, xmlNode*);
#endif

static inline void _cross_section_save_xml(CrossSection *cs, xmlNode *node)
{
	int i;
	xmlNode *child;
	const xmlChar *str[] = {XML_DISCHARGE, XML_DEPTH, XML_LEVEL};

	#if DEBUG_CROSS_SECTION_SAVE_XML
		fprintf(stderr, "cross_section_save_xml: start\n");
		fprintf(stderr, "CSSX %s\n", cs->name);
	#endif

	xmlSetProp(node, XML_NAME, (const xmlChar*)cs->name);
	jb_xml_node_set_float(node, XML_X, cs->x);
	jb_xml_node_set_float_with_default(node, XML_Y, cs->y, 0.);
	jb_xml_node_set_float_with_default(node, XML_ANGLE, cs->angle, 90.);
	if (cs->type != CROSS_SECTION_TYPE_TIME)
	{
		xmlSetProp(node, XML_TYPE, str[cs->type-1]);
		jb_xml_node_set_int(node, XML_CONTROL, cs->control);
		jb_xml_node_set_float_with_default(node, XML_TIME, cs->time, 0.);
		jb_xml_node_set_float_with_default
			(node, XML_TOLERANCE, cs->tolerance, 0.);
		if (cs->parameter != 0.) switch (cs->type)
		{
		case CROSS_SECTION_TYPE_CONTROL_Q:
			jb_xml_node_set_float(node, XML_DISCHARGE, cs->parameter);
			break;
		case CROSS_SECTION_TYPE_CONTROL_H:
			jb_xml_node_set_float(node, XML_DEPTH, cs->parameter);
			break;
		case CROSS_SECTION_TYPE_CONTROL_Z:
			jb_xml_node_set_float(node, XML_LEVEL, cs->parameter);
		}
	} 
	for (i = 0; i <= cs->n; ++i)
	{
		child = xmlNewChild(node, 0, XML_TRANSIENT_SECTION, 0);
		transient_section_save_xml(cs->ts+i, child);
	}
	#if DEBUG_CROSS_SECTION_SAVE_XML
		fprintf(stderr, "cross_section_save_xml: end\n");
	#endif
}

#if INLINE_CROSS_SECTION_SAVE_XML
	#define cross_section_save_xml _cross_section_save_xml
#else
	void cross_section_save_xml(CrossSection*, xmlNode*);
#endif

static inline TransientSection*
	_cross_section_get_transient(CrossSection *cs, JBDOUBLE t)
{
	register int i;
	register JBDOUBLE k;
	#if DEBUG_CROSS_SECTION_GET_TRANSIENT
		fprintf(stderr, "cross_section_get_transient: start\n");
	#endif
	if (cs->type == CROSS_SECTION_TYPE_TIME)
	{
		i = jbm_farray_search_extended(t, cs->t, cs->n);
		if (i<0) i = 0;
		#if DEBUG_CROSS_SECTION_GET_TRANSIENT
			fprintf(stderr, "CSGT i=%d\n", i);
		#endif
	}
	else
	{
		if (t >= cs->tmax)
		{
			cs->tmax = t + cs->time;
			k = cs->pt / cs->nt - cs->parameter;
			#if DEBUG_CROSS_SECTION_GET_TRANSIENT
				fprintf(stderr, "CSGT j=%d\n", cs->j);
			#endif
			if (k > cs->tolerance)
			{
				if (cs->j < cs->n) ++cs->j;
			}
			else if (-k > cs->tolerance)
			{
				if (cs->j > 0) --cs->j;
			}
			#if DEBUG_CROSS_SECTION_GET_TRANSIENT
				fprintf(stderr, "CSGT j=%d pt="FWF" p="FWF" tol="FWF"\n",
					cs->j, cs->pt/cs->nt, cs->parameter, cs->tolerance);
			#endif
			cs->nt = 0;
			cs->pt = 0.;
		}
		i = cs->j;
	}
	#if DEBUG_CROSS_SECTION_GET_TRANSIENT
		fprintf(stderr, "CSGT i=%d\n", i);
		transient_section_print(cs->ts+i, stderr);
		fprintf(stderr, "cross_section_get_transient: end\n");
	#endif
	return cs->ts + i;
}

#if INLINE_CROSS_SECTION_GET_TRANSIENT
	#define cross_section_get_transient _cross_section_get_transient
#else
	TransientSection* cross_section_get_transient(CrossSection*, JBDOUBLE);
#endif

#endif
