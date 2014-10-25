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
 * \file transient_section.h
 * \brief Header file to define transient section structures and methods.
 * \authors Javier Burguete Tolosa.
 * \copyright Copyright 2005-2014 Javier Burguete Tolosa.
 */
#ifndef TRANSIENT_SECTION__H
#define TRANSIENT_SECTION__H 1

#include "points.h"

/**
 * \def N_TRANSIENT_SECTION_TYPES 2
 * \brief Macro to define the number of transient section types.
 */
#define N_TRANSIENT_SECTION_TYPES 2

/**
 * \enum TransientSectionType
 * \brief Enumeration to define transient section types.
 */
enum TransientSectionType
{
/**
 * \var TRANSIENT_SECTION_TYPE_RECT
 * \brief rect.
 * \var TRANSIENT_SECTION_TYPE_POLILYNE
 * \brief polyline.
 */
	TRANSIENT_SECTION_TYPE_RECT = 0,
	TRANSIENT_SECTION_TYPE_POLILYNE = 1
};

/**
 * \struct TransientSection
 * \brief Structure to define a transient section.
 */
typedef struct
{
/**
 * \var n
 * \brief points number.
 * \var type
 * \brief type.
 * \var t
 * \brief time
 * \var u
 * \brief Bed velocity.
 * \var contraction
 * \brief contraction-expansion coefficiet.
 * \var hmax
 * \brief maximum depth without overflow.
 * \var dz
 * \brief z interval defining varable friction coefficients.
 * \var zmin
 * \brief minimum z to allow water movement.
 * \var data
 * \brief data.
 * \var sp
 * \brief section points array.
 * \var name
 * \brief name.
 */
	unsigned int n, type;
	JBDOUBLE t;
	JBFLOAT u, contraction, hmax, dz, zmin;
	void *data;
	SectionPoint2 *sp;
	char *name;
} TransientSection;

/**
 * \def TRANSIENT_SECTION_POINT2
 * \brief Macro to access to 2D section point data of a transient section.
 */
#define TRANSIENT_SECTION_POINT2(ts) ((SectionPoint2*)ts->data)
/**
 * \def TRANSIENT_SECTION_POINT3
 * \brief Macro to access to 3D section point data of a transient section.
 */
#define TRANSIENT_SECTION_POINT3(ts) ((SectionPoint3*)ts->data)

extern JBFLOAT friction_dz;
extern TransientSection tsnew[2];

static inline void _transient_section_print(TransientSection *ts, FILE *file)
{
	register int i;
	fprintf(file, "transient_section_print: start\n");
	fprintf(file, "TSP %s\n", ts->name);
	fprintf(file, "TSP n=%u type=%u\n", ts->n, ts->type);
	fprintf(file, "TSP t="FWL" u="FWF" contraction="FWF"\n",
		ts->t, ts->u, ts->contraction);
	fprintf(file, "TSP hmax="FWF" dz="FWF" zmin="FWF"\n",
		ts->hmax, ts->dz, ts->zmin);
	for (i = 0; i <= ts->n; ++i) section_point2_print(ts->sp + i, file);
	switch (ts->type)
	{
	case TRANSIENT_SECTION_TYPE_POLILYNE:
		for (i = 0; i <= ts->n; ++i)
			section_point3_print(TRANSIENT_SECTION_POINT3(ts) + i, file);
	}
	fprintf(file, "transient_section_print: end\n");
}

#if INLINE_TRANSIENT_SECTION_PRINT
	#define transient_section_print _transient_section_print
#else
	void transient_section_print(TransientSection*, FILE*);
#endif

static inline void _transient_section_delete(TransientSection *ts)
{
	#if DEBUG_TRANSIENT_SECTION_DELETE
		fprintf(stderr, "transient_section_delete: start\n");
	#endif
	jb_free_null((void**)&ts->data);
	jb_free_null((void**)&ts->name);
	#if DEBUG_TRANSIENT_SECTION_DELETE
		fprintf(stderr, "transient_section_delete: end\n");
	#endif
}

#if INLINE_TRANSIENT_SECTION_DELETE
	#define transient_section_delete _transient_section_delete
#else
	void transient_section_delete(TransientSection*);
#endif

static inline void _transient_section_error(TransientSection *ts, char *m)
{
	#if DEBUG_TRANSIENT_SECTION_ERROR
		fprintf(stderr, "transient_section_error: start\n");
	#endif
	char *buffer;
	buffer = message;
	message = g_strconcat(gettext("Transient section"), ": ", ts->name, "\n",
		m, NULL);
	g_free(buffer);
	#if DEBUG_TRANSIENT_SECTION_ERROR
		fprintf(stderr, "transient_section_error: end\n");
	#endif
}

#if INLINE_TRANSIENT_SECTION_ERROR
	#define transient_section_error _transient_section_error
#else
	void transient_section_error(TransientSection*, char*);
#endif

static inline int _transient_section_create
	(TransientSection *ts, int n, int type, char *name)
{
	register int j=n+1;
	#if DEBUG_TRANSIENT_SECTION_CREATE
		fprintf(stderr, "transient_section_create: start\n");
	#endif
	ts->n = n;
	ts->type = type;
	ts->name = jb_strdup(name);
	switch (type)
	{
	case TRANSIENT_SECTION_TYPE_RECT:
		ts->data = g_try_malloc(j*sizeof(SectionPoint2));
		ts->sp = (SectionPoint2*)ts->data;
		break;
	default:
		ts->data =
			g_try_malloc(j * (sizeof(SectionPoint2) + sizeof(SectionPoint3)));
		ts->sp = (SectionPoint2*)(ts->data + j * sizeof(SectionPoint3));
	}
	if (!ts->data || !ts->name)
	{
		transient_section_delete(ts);
		message = g_strconcat(gettext("Transient section"), ": ", name, "\n",
			gettext("Not enough memory"), NULL);
		#if DEBUG_TRANSIENT_SECTION_CREATE
			fprintf(stderr, "transient_section_create: end\n");
		#endif
		return 0;
	}
	#if DEBUG_TRANSIENT_SECTION_CREATE
		fprintf(stderr, "transient_section_create: end\n");
	#endif
	return 1;
}

#if INLINE_TRANSIENT_SECTION_CREATE
	#define transient_section_create _transient_section_create
#else
	int transient_section_create(TransientSection*, int, int, char*);
#endif

static inline int _transient_section_copy
	(TransientSection *ts, TransientSection *ts_copy)
{
	#if DEBUG_TRANSIENT_SECTION_COPY
		fprintf(stderr, "transient_section_copy: start\n");
		transient_section_print(ts_copy, stderr);
	#endif

	if (ts == ts_copy) goto exit0;
	memcpy(ts, ts_copy, JB_POINTER_SIZE(ts->data, ts->n));
	if (!transient_section_create(ts, ts_copy->n, ts_copy->type, ts_copy->name))
	{
		#if DEBUG_TRANSIENT_SECTION_COPY
			fprintf(stderr, "transient_section_copy: end\n");
		#endif
		return 0;
	}
	switch (ts->type)
	{
	case TRANSIENT_SECTION_TYPE_RECT:
		memcpy(ts->data, ts_copy->data, (ts->n + 1) * sizeof(SectionPoint2));
		ts->sp = (SectionPoint2*)ts->data;
		break;
	default:
		memcpy(ts->data, ts_copy->data,
			(ts->n + 1) * (sizeof(SectionPoint3) + sizeof(SectionPoint2)));
		ts->sp =
			(SectionPoint2*)(ts->data + (ts->n + 1) * sizeof(SectionPoint3));
	}

exit0:
	#if DEBUG_TRANSIENT_SECTION_COPY
		transient_section_print(ts, stderr);
		fprintf(stderr, "transient_section_copy: end\n");
	#endif
	return 1;
}

#if INLINE_TRANSIENT_SECTION_COPY
	#define transient_section_copy _transient_section_copy
#else
	int transient_section_copy(TransientSection*, TransientSection*);
#endif

static inline int _transient_section_open_xml
	(TransientSection *ts, xmlNode *node, JBFLOAT x, JBFLOAT y, JBFLOAT angle)
{
	int i, j, block;
	char *buffer;
	SectionPoint2 sp[1];
	SectionPoint3 sp2[1];
	FILE *file;

	#if DEBUG_TRANSIENT_SECTION_OPEN_XML
		fprintf(stderr, "transient_section_open_xml: start\n");
	#endif

	j = block= 0;
	ts->data = NULL;
	ts->name = NULL;

	if (xmlStrcmp(node->name, XML_TRANSIENT_SECTION)) goto exit0;

	if (!xmlHasProp(node, XML_NAME))
	{
		message = g_strconcat(gettext("Transient section"), "\n",
			gettext("Not name"), NULL);
		goto exit2;
	}
	buffer=(char*)xmlGetProp(node, XML_NAME);
	#if DEBUG_TRANSIENT_SECTION_OPEN_XML
		fprintf(stderr, "TSOX name=%s\n", buffer);
	#endif
	ts->name = jb_strdup(buffer);
	if (!ts->name)
	{
		message = g_strconcat(gettext("Transient section"), ": ", buffer, "\n",
			gettext("Not enough memory"), NULL);
		xmlFree(buffer);
		goto exit2;
	}
	xmlFree(buffer);

	if (!xmlHasProp(node, XML_TYPE)) ts->type = TRANSIENT_SECTION_TYPE_RECT;
	else
	{
		buffer=(char*)xmlGetProp(node, XML_TYPE);
		if (!xmlStrcmp((const xmlChar*)buffer, XML_RECT))
			ts->type = TRANSIENT_SECTION_TYPE_RECT;
		else if (!xmlStrcmp((const xmlChar*)buffer, XML_POLILYNE))
			ts->type = TRANSIENT_SECTION_TYPE_POLILYNE;
		else
		{
			xmlFree(buffer);
			transient_section_error(ts, gettext("Unknown type"));
			goto exit1;
		}
	}
	#if DEBUG_TRANSIENT_SECTION_OPEN_XML
		fprintf(stderr, "TSOX reading properties\n");
	#endif
	ts->u = jb_xml_node_get_float_with_default(node, XML_VELOCITY, &i, 0.);
	if (i != 1) goto exit0;
	ts->t = jb_xml_node_get_time_with_default(node, XML_TIME, &i, 0.);
	if (!i) goto exit0;
	ts->contraction =
		jb_xml_node_get_float_with_default(node, XML_CONTRACTION, &i, 0.);
	if (i != 1) goto exit0;
	ts->hmax = jb_xml_node_get_float_with_default(node, XML_HMAX, &i, 0.);
	if (i != 1) goto exit0;
	ts->dz = jb_xml_node_get_float_with_default(node, XML_DZ, &i, friction_dz);
	if (i != 1) goto exit0;

	file = jb_xml_node_get_content_file(node, &buffer);
	if (!file)
	{
		transient_section_error(ts, gettext("Not enough memory"));
		goto exit1;
	}

	#if DEBUG_TRANSIENT_SECTION_OPEN_XML
		fprintf(stderr, "TSOX buffer:\n%s\n", buffer);
		fprintf(stderr, "TSOX opening points\n");
	#endif
	j = -1;
	switch (ts->type)
	{
	case TRANSIENT_SECTION_TYPE_RECT:
		do
		{
			#if DEBUG_TRANSIENT_SECTION_OPEN_XML
				fprintf(stderr, "TSOX reading point\n");
			#endif
			i = fscanf(file, FRF FRF FRF "%u", &sp->y, &sp->z, &sp->r, &sp->t);
			++j;
			#if DEBUG_TRANSIENT_SECTION_OPEN_XML
				fprintf(stderr, "TSOX y="FWF" z="FWF" r="FWF" t=%u\n",
					sp->y, sp->z, sp->r, sp->t);
				fprintf(stderr, "TSOX reallocating point\n");
			#endif
			if (j == block)
			{
				block += BLOCK_REALLOC;
				ts->data =
					jb_try_realloc(ts->data, block * sizeof(SectionPoint2));
				if (!ts->data)
				{
					transient_section_error(ts, gettext("Not enough memory"));
					fclose(file);
					xmlFree(buffer);
					goto exit1;
				}
			}
			#if DEBUG_TRANSIENT_SECTION_OPEN_XML
				fprintf(stderr, "TSOX opening point %d\n", j);
			#endif
			section_point2_copy(TRANSIENT_SECTION_POINT2(ts) + j, sp);
			if (i < 4) break;
		}
		while (1);
		ts->data = jb_try_realloc(ts->data, (j + 1) * sizeof(SectionPoint2));
		#if DEBUG_TRANSIENT_SECTION_OPEN_XML
			fprintf(stderr, "TSOX closing file\n");
		#endif
		fclose(file);
		#if DEBUG_TRANSIENT_SECTION_OPEN_XML
			fprintf(stderr, "TSOX deleting buffer\n");
		#endif
		xmlFree(buffer);
		if (i != 2) goto exit0;
		ts->sp = (SectionPoint2*)ts->data;
		break;
	default:
		do
		{
			#if DEBUG_TRANSIENT_SECTION_OPEN_XML
				fprintf(stderr, "TSOX reading point\n");
			#endif
			i = fscanf(file, FRF FRF FRF FRF "%u",
				&sp2->x, &sp2->y, &sp2->z, &sp2->r, &sp2->t);
			++j;
			#if DEBUG_TRANSIENT_SECTION_OPEN_XML
				fprintf(stderr, "TSOX x="FWF" y="FWF" z="FWF" r="FWF" t=%d\n",
					sp2->x, sp2->y, sp2->z, sp2->r, sp2->t);
				fprintf(stderr, "TSOX reallocating point\n");
			#endif
			if (j == block)
			{
				block += BLOCK_REALLOC;
				ts->data =
					jb_try_realloc(ts->data, block * sizeof(SectionPoint3));
				if (!ts->data)
				{
					transient_section_error(ts, gettext("Not enough memory"));
					fclose(file);
					xmlFree(buffer);
					goto exit1;
				}
			}
			#if DEBUG_TRANSIENT_SECTION_OPEN_XML
				fprintf(stderr, "TSOX opening point %d\n", j);
			#endif
			section_point3_copy(TRANSIENT_SECTION_POINT3(ts) + j, sp2);
			if (i < 5) break;
		}
		while (1);
		#if DEBUG_TRANSIENT_SECTION_OPEN_XML
			fprintf(stderr, "TSOX closing file\n");
		#endif
		fclose(file);
		#if DEBUG_TRANSIENT_SECTION_OPEN_XML
			fprintf(stderr, "TSOX deleting buffer\n");
		#endif
		xmlFree(buffer);
		if (i != 3) goto exit0;
		ts->data = jb_realloc(ts->data,
			(j + 1) * (sizeof(SectionPoint3) + sizeof(SectionPoint2)));
		if (!ts->data)
		{
			transient_section_error(ts, gettext("Not enough memory"));
			goto exit1;
		}
		ts->sp = (SectionPoint2*)(ts->data + (j + 1) * sizeof(SectionPoint3));
		for (i = 0; i <= j; ++i) section_point2_transform
			(ts->sp + i, TRANSIENT_SECTION_POINT3(ts) + i, x, y, angle);
	}
	#if DEBUG_TRANSIENT_SECTION_OPEN_XML
		fprintf(stderr, "TSOX data=%ld sp=%ld\n",
			(size_t)ts->data, (size_t)ts->sp);
		fprintf(stderr, "TSOX calculating hmax, zmin (j=%d)\n", j);
	#endif
	ts->zmin = jbm_varray_min
		(&(ts->sp->z), sizeof(SectionPoint2), j);
	if (ts->hmax == 0.)
		ts->hmax = fmin(ts->sp[0].z, ts->sp[j].z) - ts->zmin;

	if (j) goto exit2;

exit0:
	j=0;
	transient_section_error(ts, gettext("Bad defined"));

exit1:
	#if DEBUG_TRANSIENT_SECTION_OPEN_XML
		fprintf(stderr, "TSOX deleting section\n");
	#endif
	transient_section_delete(ts);

exit2:
	ts->n = j;
	#if DEBUG_TRANSIENT_SECTION_OPEN_XML
		transient_section_print(ts, stderr);
		fprintf(stderr, "transient_section_open_xml: end\n");
	#endif
	return j;
}

#if INLINE_TRANSIENT_SECTION_OPEN_XML
	#define transient_section_open_xml _transient_section_open_xml
#else
	int transient_section_open_xml
		(TransientSection*, xmlNode*, JBFLOAT, JBFLOAT, JBFLOAT);
#endif

static inline void
	_transient_section_save_xml(TransientSection *ts, xmlNode *node)
{
	int i;
	char *buffer, *buffer2, str[JB_BUFFER_SIZE];
	SectionPoint2 *sp;
	SectionPoint3 *sp2;
	#if DEBUG_TRANSIENT_SECTION_SAVE_XML
		fprintf(stderr, "transient_section_save_xml: start\n");
		fprintf(stderr, "TSSX name %s\n", ts->name);
	#endif
	xmlSetProp(node, XML_NAME, (const xmlChar*)ts->name);
	switch (ts->type)
	{
	case TRANSIENT_SECTION_TYPE_POLILYNE:
		xmlSetProp(node, XML_TYPE, XML_POLILYNE);
	}
	if (ts->t != 0.) jb_xml_node_set_time(node, XML_TIME, ts->t);
	if (ts->u != 0.) jb_xml_node_set_float(node, XML_VELOCITY, ts->u);
	if (ts->contraction != 0.)
		jb_xml_node_set_float(node, XML_CONTRACTION, ts->contraction);
	jb_xml_node_set_float(node, XML_DZ, ts->dz);
	switch (ts->type)
	{
	case TRANSIENT_SECTION_TYPE_RECT:
		sp = (SectionPoint2*)ts->data;
		for (i = 0; i < ts->n; ++i, ++sp)
		{
			snprintf(str, JB_BUFFER_SIZE, "\n        "FWF2 FWF2 FWF2 "%d",
				sp->y, sp->z, sp->r, sp->t);
			if (!i) buffer2 = g_strdup(str), buffer=NULL;
			else buffer2 = g_strconcat(buffer, str, NULL);
			g_free(buffer);
			buffer = buffer2;
		}
		snprintf(str, JB_BUFFER_SIZE, "\n        "FWF2 FWF"\n      ",
			sp->y, sp->z);
		break;
	default:
		sp2 = (SectionPoint3*)ts->data;
		for (i = 0; i < ts->n; ++i, ++sp2)
		{
			snprintf(str, JB_BUFFER_SIZE, "\n        "FWF2 FWF2 FWF2 FWF2 "%d",
				sp2->x, sp2->y, sp2->z, sp2->r, sp2->t);
			if (!i) buffer2 = g_strdup(str), buffer=NULL;
			else buffer2 = g_strconcat(buffer, str, NULL);
			g_free(buffer);
			buffer = buffer2;
		}
		snprintf(str, JB_BUFFER_SIZE, "\n        "FWF2 FWF2 FWF"\n      ",
			sp2->x, sp2->y, sp2->z);
	}
	buffer2 = g_strconcat(buffer, str, NULL);
	xmlNodeSetContent(node, (const xmlChar*)buffer2);
	g_free(buffer);
	g_free(buffer2);

	#if DEBUG_TRANSIENT_SECTION_SAVE_XML
		fprintf(stderr, "transient_section_save_xml: end\n");
	#endif
}

#if INLINE_TRANSIENT_SECTION_SAVE_XML
	#define transient_section_save_xml _transient_section_save_xml
#else
	void transient_section_save_xml(TransientSection*, xmlNode*);
#endif

#endif
