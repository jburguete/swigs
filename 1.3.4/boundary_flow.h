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

/*
	BOUNDARY SPECIFICATIONS
	0: Constant discharge
		p1[0]=discharge
	1: Constant depth
		p1[0]=depth
	2: Constant level
		p1[0]=level
	3: Constant discharge and depth
		p1[0]=discharge, p1[1]=depth
	4: Constant discharge and level
		p1[0]=discharge, p1[1]=level
	5: Discharge hydrogram Q(t)
		p1[i]=time, p2[i]=discharge
	6: Depth hydrogram h(t)
		p1[i]=time, p2[i]=depth
	7: Level hydrogram z(t)
		p1[i]=time, p2[i]=level
	8: Discharge and depth hydrogram Q(t), h(t)
		p1[i]=time, p2[i]=discharge, p3[i]=depth
	9: Discharge and level hydrogram Q(t), z(t)
		p1[i]=time, p2[i]=discharge, p3[i]=level
	10: Q(h)
		p1[i]=depth, p2[i]=discharge
	11: Q(z)
		p1[i]=level, p2[i]=discharge
	12: Supercritical output
	13: Dam
		data[0]=level, data[1]=roughness
		p1[i]=time, p2[i]=maximum discharge
	14: Pipe
		p1[i]=time, p2[i]=maximum discharge
	15: Gate
		data[0]=height, data[1]=width
		p1[i]=time, p2[i]=opening
	16: Junction
*/

/**
 * \file boundary_flow.h
 * \brief Header file to define flow boundary conditions structures and
 *   functions.
 * \authors Javier Burguete Tolosa.
 * \copyright Copyright 2005-2015 Javier Burguete Tolosa.
 */
#ifndef BOUNDARY_FLOW__H
#define BOUNDARY_FLOW__H 1

#include "points.h"
#include "junction_data.h"

/**
 * \def N_BOUNDARY_FLOW_TYPES
 * \brief Number of flow boundary conditions types.
 * \def N_BOUNDARY_FLOW_EXTERN_TYPES
 * \brief Number of flow boundary conditions types at extreme channel points.
 * \def N_BOUNDARY_FLOW_INNER_TYPES
 * \brief Number of flow boundary conditions types at inner channel points.
 */
#define N_BOUNDARY_FLOW_TYPES			(BOUNDARY_FLOW_TYPE_JUNCTION + 1)
#define N_BOUNDARY_FLOW_EXTERN_TYPES	(N_BOUNDARY_FLOW_TYPES - 2)
#define N_BOUNDARY_FLOW_INNER_TYPES		(N_BOUNDARY_FLOW_TYPES - 9)

/**
 * \def DAM_LEVEL
 * \brief Macro to define the overflow level of a dam.
 * \def DAM_ROUGHNESS
 * \brief Macro to define the roughness coeffecient at the dam overflow level.
 */
#define DAM_LEVEL(bf) (((JBFLOAT*)bf->data)[0])
#define DAM_ROUGHNESS(bf) (((JBFLOAT*)bf->data)[1])

/**
 * \def GATE_HEIGHT
 * \brief Macro to define the height of a gate.
 * \def GATE_WIDTH
 * \brief Macro to define the width of a gate.
 */
#define GATE_HEIGHT(bf) (((JBFLOAT*)bf->data)[0])
#define GATE_WIDTH(bf) (((JBFLOAT*)bf->data)[1])

/**
 * \def JUNCTION_TYPE
 * \brief Macro to define the type of a junction.
 * \def JUNCTION_N
 * \brief Macro to define the number of JunctionData structures required to
 *   define a junction.
 * \def JUNCTION_DATA
 * \brief Macro to define the data of a junction.
 */
#define JUNCTION_TYPE(bf) (((int*)bf->data)[0])
#define JUNCTION_N(bf) (((int*)bf->data)[1])
#define JUNCTION_DATA(bf, i) \
	((JunctionData*)(bf->data + 2 * sizeof(int) + i * sizeof(JunctionData)))

/**
 * \enum JunctionType
 * \brief Enumeration to define the junction types.
 * \var JUNCTION_TYPE_FRONTAL
 * \brief Frontal type of junction.
 * \var JUNCTION_TYPE_TRIBUTARY
 * \brief Tributary type of junction.
 * \var JUNCTION_TYPE_PARALLEL
 * \brief Parallel type of junction.
 */
enum JunctionType
{
	JUNCTION_TYPE_FRONTAL = 0,
	JUNCTION_TYPE_TRIBUTARY = 1,
	JUNCTION_TYPE_PARALLEL = 2
};

/**
 * \enum BoundaryFlowType
 * \brief Enumeration to define the flow boundary conditions types.
 * \var BOUNDARY_FLOW_TYPE_Q
 * \brief Constant discharge flow boundary condition type.
 * \var BOUNDARY_FLOW_TYPE_H
 * \brief Constant depth flow boundary condition type.
 * \var BOUNDARY_FLOW_TYPE_Z
 * \brief Constant level flow boundary condition type.
 * \var BOUNDARY_FLOW_TYPE_Q_H
 * \brief Constant discharge and depth flow boundary condition type.
 * \var BOUNDARY_FLOW_TYPE_Q_Z
 * \brief Constant discharge and level flow boundary condition type.
 * \var BOUNDARY_FLOW_TYPE_QT
 * \brief Variable in time discharge flow boundary condition type.
 * \var BOUNDARY_FLOW_TYPE_HT
 * \brief Variable in time depth flow boundary condition type.
 * \var BOUNDARY_FLOW_TYPE_ZT
 * \brief Variable in time level flow boundary condition type.
 * \var BOUNDARY_FLOW_TYPE_QT_HT
 * \brief Variable in time discharge and depth flow boundary condition type.
 * \var BOUNDARY_FLOW_TYPE_QT_ZT
 * \brief Variable in time discharge and level flow boundary condition type.
 * \var BOUNDARY_FLOW_TYPE_QH
 * \brief Discharge depending on depth flow boundary condition type.
 * \var BOUNDARY_FLOW_TYPE_QZ
 * \brief Discharge depending on level flow boundary condition type.
 * \var BOUNDARY_FLOW_TYPE_SUPERCRITICAL
 * \brief Supercritical flow boundary condition type.
 * \var BOUNDARY_FLOW_TYPE_DAM
 * \brief Dam flow boundary condition type.
 * \var BOUNDARY_FLOW_TYPE_PIPE
 * \brief Pipe flow boundary condition type.
 * \var BOUNDARY_FLOW_TYPE_GATE
 * \brief Gate flow boundary condition type.
 * \var BOUNDARY_FLOW_TYPE_JUNCTION
 * \brief Junction flow boundary condition type.
 */
enum BoundaryFlowType
{
	BOUNDARY_FLOW_TYPE_Q = 0,
	BOUNDARY_FLOW_TYPE_H = 1,
	BOUNDARY_FLOW_TYPE_Z = 2,
	BOUNDARY_FLOW_TYPE_Q_H = 3,
	BOUNDARY_FLOW_TYPE_Q_Z = 4,
	BOUNDARY_FLOW_TYPE_QT = 5,
	BOUNDARY_FLOW_TYPE_HT = 6,
	BOUNDARY_FLOW_TYPE_ZT = 7,
	BOUNDARY_FLOW_TYPE_QT_HT = 8,
	BOUNDARY_FLOW_TYPE_QT_ZT = 9,
	BOUNDARY_FLOW_TYPE_QH = 10,
	BOUNDARY_FLOW_TYPE_QZ = 11,
	BOUNDARY_FLOW_TYPE_SUPERCRITICAL = 12,
	BOUNDARY_FLOW_TYPE_DAM = 13,
	BOUNDARY_FLOW_TYPE_PIPE = 14,
	BOUNDARY_FLOW_TYPE_GATE = 15,
	BOUNDARY_FLOW_TYPE_JUNCTION = 16
};

/**
 * \struct BoundaryFlow
 * \brief Structure to define a flow boundary condition.
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
 * \var position
 * \brief -1 on inlet, 0 on inner boundaries, +1 on outlet.
 * \var contribution
 * \brief water mass contribution.
 * \var delay
 * \brief Macro to define the delay to apply the boundary condition.
 * \var t
 * \brief array of times.
 * \var p1
 * \brief array of first parameters data.
 * \var p2
 * \brief array of second parameters data.
 * \var name
 * \brief name.
 * \var section
 * \brief name of the first cross section to apply the boundary condition.
 * \var section2
 * \brief name of the last cross section to apply the boundary condition.
 * \var data
 * \brief additional data.
 */
	int type, pos, pos2, n, i, i2, position;
	JBFLOAT contribution, delay;
	JBDOUBLE *t;
	JBFLOAT *p1, *p2;
	char *name, *section, *section2;
	void *data;
} BoundaryFlow;

extern int simulating;
extern int boundary_flow_extern_type[N_BOUNDARY_FLOW_EXTERN_TYPES];
extern int boundary_flow_inner_type[N_BOUNDARY_FLOW_INNER_TYPES];

static inline void _boundary_flow_print(BoundaryFlow *bf, FILE *file)
{
	int i;
	fprintf(file, "boundary_flow_print: start\n");

	fprintf(file, "BFP name=%s\n", bf->name);
	fprintf(file, "BFP type=%d position=%d pos=%d pos2=%d\n",
		bf->type, bf->position, bf->pos, bf->pos2);

	switch (bf->type)
	{
	case BOUNDARY_FLOW_TYPE_Q_H:
	case BOUNDARY_FLOW_TYPE_Q_Z:
	case BOUNDARY_FLOW_TYPE_QT_HT:
	case BOUNDARY_FLOW_TYPE_QT_ZT:
		for (i = 0; i <= bf->n; ++i)
			fprintf(file, "BFP i=%d p2="FWF"\n", i, bf->p2[i]);
	case BOUNDARY_FLOW_TYPE_Q:
	case BOUNDARY_FLOW_TYPE_H:
	case BOUNDARY_FLOW_TYPE_Z:
	case BOUNDARY_FLOW_TYPE_QT:
	case BOUNDARY_FLOW_TYPE_HT:
	case BOUNDARY_FLOW_TYPE_ZT:
	case BOUNDARY_FLOW_TYPE_DAM:
	case BOUNDARY_FLOW_TYPE_PIPE:
	case BOUNDARY_FLOW_TYPE_GATE:
		for (i = 0; i <= bf->n; ++i)
			fprintf(file, "BFP i=%d t="FWL" p1="FWF"\n",
				i, bf->t[i], bf->p1[i]);
		break;
	case BOUNDARY_FLOW_TYPE_QH:
	case BOUNDARY_FLOW_TYPE_QZ:
		for (i = 0; i <= bf->n; ++i)
			fprintf(file, "BFP i=%d p1="FWL" p2="FWF"\n",
				i, bf->p1[i], bf->p2[i]);
		break;
	case BOUNDARY_FLOW_TYPE_JUNCTION:
		fprintf(file, "BFP n=%d\n", JUNCTION_N(bf));
		for (i = 0; i <= JUNCTION_N(bf); ++i)
			junction_data_print(JUNCTION_DATA(bf, i), file);
	}

	switch (bf->type)
	{
	case BOUNDARY_FLOW_TYPE_QT:
	case BOUNDARY_FLOW_TYPE_HT:
	case BOUNDARY_FLOW_TYPE_ZT:
	case BOUNDARY_FLOW_TYPE_QT_HT:
	case BOUNDARY_FLOW_TYPE_QT_ZT:
		fprintf(file, "BFP delay="FWF"\n", bf->delay);
		break;
	case BOUNDARY_FLOW_TYPE_DAM:
		fprintf(file, "BFP level="FWF" roughness="FWF"\n",
			DAM_LEVEL(bf), DAM_ROUGHNESS(bf));
		break;
	case BOUNDARY_FLOW_TYPE_GATE:
		fprintf(file, "BFP height="FWF" width="FWF"\n",
			GATE_HEIGHT(bf), GATE_WIDTH(bf));
	}

	fprintf(file, "boundary_flow_print: end\n");
}

#if INLINE_BOUNDARY_FLOW_PRINT
	#define boundary_flow_print _boundary_flow_print
#else
	void boundary_flow_print(BoundaryFlow*, FILE*);
#endif

static inline void _boundary_flow_error(BoundaryFlow *bf, char *m)
{
	char *buffer;
	#if DEBUG_BOUNDARY_FLOW_ERROR
		fprintf(stderr, "boundary_flow_error: start\n");
	#endif
	buffer = message;
	message = g_strconcat(gettext("Flow boundary condition"), ": ", bf->name,
		"\n", m, NULL);
	g_free(buffer);
	#if DEBUG_BOUNDARY_FLOW_ERROR
		fprintf(stderr, "boundary_flow_error: end\n");
	#endif
}

#if INLINE_BOUNDARY_FLOW_ERROR
	#define boundary_flow_error _boundary_flow_error
#else
	void boundary_flow_error(BoundaryFlow*, char*);
#endif

static inline void _boundary_flow_delete(BoundaryFlow *bf)
{
	int i;
	#if DEBUG_BOUNDARY_FLOW_DELETE
		fprintf(stderr, "boundary_flow_delete: start\n");
	#endif
	switch (bf->type)
	{
		case BOUNDARY_FLOW_TYPE_JUNCTION:
			for (i = 0; i <= JUNCTION_N(bf); ++i)
				junction_data_delete(JUNCTION_DATA(bf, i));
	}
	jb_free_null((void**)&bf->name);
	jb_free_null((void**)&bf->t);
	jb_free_null((void**)&bf->p1);
	jb_free_null((void**)&bf->p2);
	jb_free_null((void**)&bf->data);
	jb_free_null((void**)&bf->section);
	jb_free_null((void**)&bf->section2);
	#if DEBUG_BOUNDARY_FLOW_DELETE
		fprintf(stderr, "boundary_flow_delete: end\n");
	#endif
}

#if INLINE_BOUNDARY_FLOW_DELETE
	#define boundary_flow_delete _boundary_flow_delete
#else
	void boundary_flow_delete(BoundaryFlow*);
#endif

static inline void _boundary_flow_init_empty(BoundaryFlow *bf)
{
	#if DEBUG_BOUNDARY_FLOW_INIT_EMPTY
		fprintf(stderr, "boundary_flow_init_empty: start\n");
	#endif
	bf->name = bf->section = bf->section2 = NULL;
	bf->t = NULL;
	bf->p1 = bf->p2 = NULL;
	bf->data = NULL;
	#if DEBUG_BOUNDARY_FLOW_INIT_EMPTY
		fprintf(stderr, "boundary_flow_init_empty: end\n");
	#endif
}

#if INLINE_BOUNDARY_FLOW_INIT_EMPTY
	#define boundary_flow_init_empty _boundary_flow_init_empty
#else
	void boundary_flow_init_empty(BoundaryFlow*);
#endif

static inline int _boundary_flow_insert_junction(BoundaryFlow *bf, int position)
{
	int i;
	JunctionData *jd;
	#if DEBUG_BOUNDARY_FLOW_INSERT_JUNCTION
		fprintf(stderr, "boundary_flow_insert_junction: start\n");
	#endif
	bf->data = jb_realloc(bf->data,
		2 * sizeof(int) + (JUNCTION_N(bf) + 2) * sizeof(JunctionData));
	if (!bf->data)
	{
		message = gettext("Not enough memory");
		goto error_insert;
	}
	++JUNCTION_N(bf);
	#if DEBUG_BOUNDARY_FLOW_INSERT_JUNCTION
		fprintf(stderr, "boundary_flow_insert_junction: n=%d position=%d\n",
			JUNCTION_N(bf), position);
	#endif
	jd = JUNCTION_DATA(bf, JUNCTION_N(bf));
	junction_data_init_empty(jd);
	for (i = JUNCTION_N(bf); i >= position; --i)
	{
		jd = JUNCTION_DATA(bf, i);
		junction_data_delete(jd);
		if (!junction_data_copy(jd, jd - 1)) goto error_insert;
	}
	#if DEBUG_BOUNDARY_FLOW_INSERT_JUNCTION
		boundary_flow_print(bf, stderr);
		fprintf(stderr, "boundary_flow_insert_junction: end\n");
	#endif
	return 1;

error_insert:
	boundary_flow_error(bf, message);
	boundary_flow_delete(bf);
	#if DEBUG_BOUNDARY_FLOW_INSERT_JUNCTION
		fprintf(stderr, "boundary_flow_insert_junction: end\n");
	#endif
	return 0;
}

#if INLINE_BOUNDARY_FLOW_INSERT_JUNCTION
	#define boundary_flow_insert_junction _boundary_flow_insert_junction
#else
	int boundary_flow_insert_junction(BoundaryFlow*, int);
#endif

static inline void _boundary_flow_remove_junction
	(BoundaryFlow *bf, int position)
{
	int i;
	JunctionData *jd;
	#if DEBUG_BOUNDARY_FLOW_REMOVE_JUNCTION
		fprintf(stderr, "boundary_flow_remove_junction: start\n");
	#endif
	junction_data_delete(JUNCTION_DATA(bf, position));
	for (i = position; i < JUNCTION_N(bf); ++i)
	{
		jd = JUNCTION_DATA(bf, i);
		junction_data_delete(jd);
		junction_data_copy(jd, jd + 1);
	}
	junction_data_delete(JUNCTION_DATA(bf, i));
	bf->data = jb_realloc
		(bf->data, 2 * sizeof(int) + JUNCTION_N(bf) * sizeof(JunctionData));
	--JUNCTION_N(bf);
	#if DEBUG_BOUNDARY_FLOW_REMOVE_JUNCTION
		fprintf(stderr, "boundary_flow_remove_junction: end\n");
	#endif
}

#if INLINE_BOUNDARY_FLOW_REMOVE_JUNCTION
	#define boundary_flow_remove_junction _boundary_flow_remove_junction
#else
	void boundary_flow_remove_junction(BoundaryFlow*, int);
#endif

static inline int _boundary_flow_copy(BoundaryFlow *bf, BoundaryFlow *bf_copy)
{
	int i;
	#if DEBUG_BOUNDARY_FLOW_COPY
		fprintf(stderr, "boundary_flow_copy: start\n");
		boundary_flow_print(bf_copy, stderr);
	#endif

	if (bf == bf_copy) goto exit0;

	boundary_flow_init_empty(bf);

	bf->position = bf_copy->position;
	bf->pos = bf_copy->pos;
	bf->pos2 = bf_copy->pos2;
	bf->delay = bf_copy->delay;

	bf->name = jb_strdup(bf_copy->name);
	if (!bf->name) goto exit1;
	if (bf_copy->section)
	{
		bf->section = jb_strdup(bf_copy->section);
		if (!bf->section) goto exit1;
	}
	if (bf_copy->section2)
	{
		bf->section2 = jb_strdup(bf_copy->section2);
		if (!bf->section2) goto exit1;
	}
	#if DEBUG_BOUNDARY_FLOW_COPY
		fprintf(stderr, "BFC name=%s\n", bf->name);
	#endif

	bf->type = bf_copy->type;
	if (bf->type == BOUNDARY_FLOW_TYPE_SUPERCRITICAL) goto exit0;
	bf->n = bf_copy->n;
	i = bf->n + 1;
	if (bf_copy->t)
	{
		bf->t = (JBDOUBLE*)g_try_malloc(i * sizeof(JBDOUBLE));
		if (!bf->t) goto exit1;
		memcpy(bf->t, bf_copy->t, i * sizeof(JBDOUBLE));
	}
	if (bf_copy->p1)
	{
		bf->p1 = (JBFLOAT*)g_try_malloc(i * sizeof(JBFLOAT));
		if (!bf->p1) goto exit1;
		memcpy(bf->p1, bf_copy->p1, i * sizeof(JBFLOAT));
	}
	if (bf_copy->p2)
	{
		bf->p2 = (JBFLOAT*)g_try_malloc(i * sizeof(JBFLOAT));
		if (!bf->p2) goto exit1;
		memcpy(bf->p2, bf_copy->p2, i * sizeof(JBFLOAT));
	}
	switch (bf->type)
	{
		case BOUNDARY_FLOW_TYPE_DAM:
		case BOUNDARY_FLOW_TYPE_GATE:
			bf->data = g_try_malloc(2 * sizeof(JBFLOAT));
			if (!bf->data) goto exit1;
			memcpy(bf->data, bf_copy->data, 2 * sizeof(JBFLOAT));
			break;
		case BOUNDARY_FLOW_TYPE_JUNCTION:
			#if DEBUG_BOUNDARY_FLOW_COPY
				fprintf(stderr, "BFC n=%d\n", JUNCTION_N(bf_copy));
			#endif
			i = 2 * sizeof(int)
				+ (1 + JUNCTION_N(bf_copy)) * sizeof(JunctionData);
			bf->data = g_try_malloc(i);
			if (!bf->data) goto exit1;
			memcpy(bf->data, bf_copy->data, i);
			for (i = 0; i <= JUNCTION_N(bf_copy); ++i)
				junction_data_copy
					(JUNCTION_DATA(bf, i), JUNCTION_DATA(bf_copy, i));
	}

exit0:
	#if DEBUG_BOUNDARY_FLOW_COPY
		boundary_flow_print(bf, stderr);
		fprintf(stderr, "boundary_flow_copy: end\n");
	#endif
	return 1;

exit1:
	boundary_flow_error(bf_copy, gettext("Not enough memory"));
	boundary_flow_delete(bf);
	#if DEBUG_BOUNDARY_FLOW_COPY
		fprintf(stderr, "boundary_flow_copy: end\n");
	#endif
	return 0;
}

#if INLINE_BOUNDARY_FLOW_COPY
	#define boundary_flow_copy _boundary_flow_copy
#else
	int boundary_flow_copy(BoundaryFlow*, BoundaryFlow*);
#endif

static inline int _boundary_flow_open_xml
	(BoundaryFlow *bf, xmlNode *node, int position)
{
	int i, j;
	char *buffer;
	JBDOUBLE t;
	JBFLOAT x, y;
	FILE *file;
	xmlNode *child;
	#if DEBUG_BOUNDARY_FLOW_OPEN_XML
		fprintf(stderr, "boundary_flow_open_xml: start\n");
		fprintf(stderr, "BFOX position=%d\n", position);
	#endif

	boundary_flow_init_empty(bf);

	bf->position = position;
	if (position < 0) bf->name = jb_strdup(gettext("Inlet"));
	else if (position > 0) bf->name = jb_strdup(gettext("Outlet"));
	else
	{
		if (!xmlHasProp(node, XML_NAME))
		{
			message = g_strconcat(gettext("Flow boundary condition"), "\n",
				gettext("Not name"), NULL);
			goto exit1;
		}
		buffer = (char*)xmlGetProp(node, XML_NAME);
		bf->name = jb_strdup(buffer);
		xmlFree(buffer);
		if (!bf->name)
		{
			message = g_strconcat(gettext("Flow boundary condition"), "\n",
				gettext("Not enough memory"), NULL);
			goto exit1;
		}
		#if DEBUG_BOUNDARY_FLOW_OPEN_XML
			fprintf(stderr, "BFOX name=%s\n", bf->name);
		#endif
		if (!xmlHasProp(node, XML_INITIAL))
		{
			boundary_flow_error(bf, gettext("Bad position"));
			goto exit1;
		}
		buffer = (char*)xmlGetProp(node, XML_INITIAL);
		bf->section = jb_strdup(buffer);
		xmlFree(buffer);
		if (!bf->section)
		{
			boundary_flow_error(bf, gettext("Bad position"));
			goto exit1;
		}
		#if DEBUG_BOUNDARY_FLOW_OPEN_XML
			fprintf(stderr, "BFOX section=%s\n", bf->section);
		#endif
		if (xmlHasProp(node, XML_FINAL))
		{
			buffer = (char*)xmlGetProp(node, XML_FINAL);
			bf->section2 = jb_strdup(buffer);
			xmlFree(buffer);
			if (!bf->section2)
			{
				boundary_flow_error(bf, gettext("Bad position"));
				goto exit1;
			}
			#if DEBUG_BOUNDARY_FLOW_OPEN_XML
				fprintf(stderr, "BFOX section2=%s\n", bf->section2);
			#endif
		}
	}

	if (!xmlHasProp(node, XML_TYPE))
	{
		boundary_flow_error(bf, gettext("Unknow type"));
		goto exit1;
	}
	buffer = (char*)xmlGetProp(node, XML_TYPE);
	#if DEBUG_BOUNDARY_FLOW_OPEN_XML
		fprintf(stderr, "BFOX type=%s\n", buffer);
	#endif

	if (!xmlStrcmp((const xmlChar*)buffer, XML_Q))
	{
		bf->type = BOUNDARY_FLOW_TYPE_Q;
		if (!xmlHasProp(node, XML_DISCHARGE))
		{
			boundary_flow_error(bf, gettext("Unknow type"));
			goto exit2;
		}
		x = jb_xml_node_get_float(node, XML_DISCHARGE, &i);
		goto type01;
	}
	else if (!xmlStrcmp((const xmlChar*)buffer, XML_H))
	{
		bf->type = BOUNDARY_FLOW_TYPE_H;
		if (!xmlHasProp(node, XML_DEPTH))
		{
			boundary_flow_error(bf, gettext("Unknow type"));
			goto exit2;
		}
		x = jb_xml_node_get_float(node, XML_DEPTH, &i);
		goto type01i;
	}
	else if (!xmlStrcmp((const xmlChar*)buffer, XML_Z))
	{
		bf->type = BOUNDARY_FLOW_TYPE_Z;
		if (!xmlHasProp(node, XML_LEVEL))
		{
			boundary_flow_error(bf, gettext("Unknow type"));
			goto exit2;
		}
		x = jb_xml_node_get_float(node, XML_LEVEL, &i);
		goto type01i;
	}
	else if (!xmlStrcmp((const xmlChar*)buffer, XML_Q_H))
	{
		bf->type = BOUNDARY_FLOW_TYPE_Q_H;
		if (!xmlHasProp(node, XML_DISCHARGE) || !xmlHasProp(node, XML_DEPTH))
		{
			boundary_flow_error(bf, gettext("Unknow type"));
			goto exit2;
		}
		x = jb_xml_node_get_float(node, XML_DISCHARGE, &i);
		y = jb_xml_node_get_float(node, XML_DEPTH, &j);
		goto type02i;
	}
	else if (!xmlStrcmp((const xmlChar*)buffer, XML_Q_Z))
	{
		bf->type = BOUNDARY_FLOW_TYPE_Q_Z;
		if (!xmlHasProp(node, XML_DISCHARGE) || !xmlHasProp(node, XML_LEVEL))
		{
			boundary_flow_error(bf, gettext("Unknow type"));
			goto exit2;
		}
		x = jb_xml_node_get_float(node, XML_DISCHARGE, &i);
		y = jb_xml_node_get_float(node, XML_LEVEL, &j);
		goto type02i;
	}
	else if (!xmlStrcmp((const xmlChar*)buffer, XML_QT))
	{
		bf->type = BOUNDARY_FLOW_TYPE_QT;
		goto type2time;
	}
	else if (!xmlStrcmp((const xmlChar*)buffer, XML_HT))
	{
		bf->type = BOUNDARY_FLOW_TYPE_HT;
		goto type2itime;
	}
	else if (!xmlStrcmp((const xmlChar*)buffer, XML_ZT))
	{
		bf->type = BOUNDARY_FLOW_TYPE_ZT;
		goto type2itime;
	}
	else if (!xmlStrcmp((const xmlChar*)buffer, XML_QT_HT))
	{
		bf->type = BOUNDARY_FLOW_TYPE_QT_HT;
		goto type3itime;
	}
	else if (!xmlStrcmp((const xmlChar*)buffer, XML_QT_ZT))
	{
		bf->type = BOUNDARY_FLOW_TYPE_QT_ZT;
		goto type3itime;
	}
	else if (!xmlStrcmp((const xmlChar*)buffer, XML_QH))
	{
		bf->type = BOUNDARY_FLOW_TYPE_QH;
		goto type2;
	}
	else if (!xmlStrcmp((const xmlChar*)buffer, XML_QZ))
	{
		bf->type = BOUNDARY_FLOW_TYPE_QZ;
		goto type2;
	}
	else if (!xmlStrcmp((const xmlChar*)buffer, XML_SUPERCRITICAL))
	{
		bf->type = BOUNDARY_FLOW_TYPE_SUPERCRITICAL;
		goto type0i;
	}
	else if (!xmlStrcmp((const xmlChar*)buffer, XML_DAM))
	{
		bf->type = BOUNDARY_FLOW_TYPE_DAM;
		if (!xmlHasProp(node, XML_LEVEL) || !xmlHasProp(node, XML_ROUGHNESS))
		{
			boundary_flow_error(bf, gettext("Bad defined"));
			goto exit2;
		}
		bf->data = g_try_malloc(2 * sizeof(JBFLOAT));
		if (!bf->data);
		{
			boundary_flow_error(bf, gettext("Not enough memory"));
			goto exit2;
		}
		DAM_LEVEL(bf) = jb_xml_node_get_float(node, XML_LEVEL, &i);
		DAM_ROUGHNESS(bf) = jb_xml_node_get_float(node, XML_ROUGHNESS, &j);
		if (i != 1 || j != 1);
		{
			boundary_flow_error(bf, gettext("Bad defined"));
			goto exit2;
		}
		goto type2nitime;
	}
	else if (!xmlStrcmp((const xmlChar*)buffer, XML_PIPE))
	{
		bf->type = BOUNDARY_FLOW_TYPE_PIPE;
		goto type2nitime;
	}
	else if (!xmlStrcmp((const xmlChar*)buffer, XML_GATE))
	{
		bf->type = BOUNDARY_FLOW_TYPE_GATE;
		if (!xmlHasProp(node, XML_HEIGHT) || !xmlHasProp(node, XML_WIDTH))
			goto exit2;
		bf->data = g_try_malloc(2 * sizeof(JBFLOAT));
		if (!bf->data) goto exit2;
		GATE_HEIGHT(bf) = jb_xml_node_get_float(node, XML_HEIGHT, &i);
		GATE_WIDTH(bf) = jb_xml_node_get_float(node, XML_WIDTH, &j);
		if (i != 1 || j != 1) goto exit2;
		goto type2itime;
	}
	else if (!xmlStrcmp((const xmlChar*)buffer, XML_JUNCTION))
	{
		bf->type = BOUNDARY_FLOW_TYPE_JUNCTION;
		bf->data = g_try_malloc(2 * sizeof(int));
		if (!bf->data) goto exit1;
		JUNCTION_N(bf) = -1;
		for (child = node->children; child; child = child->next)
		{
			++JUNCTION_N(bf);
			bf->data = jb_realloc(bf->data,
				2 * sizeof(int) + (1 + JUNCTION_N(bf)) * sizeof(JunctionData));
			if (!bf->data) goto exit1;
			junction_data_open_xml(JUNCTION_DATA(bf, JUNCTION_N(bf)), child);
		}
		if (JUNCTION_N(bf) < 0)
		{
			boundary_flow_error(bf, gettext("Bad junction"));
			goto exit2;
		}
		goto exit0;
	}
	else
	{
		boundary_flow_error(bf, gettext("Unknow type"));
		goto exit2;
	}

type3itime:
	if (!position)
	{
		boundary_flow_error(bf, gettext("Unknow type"));
		goto exit2;
	}

/*type3time:*/
	xmlFree(buffer);
	file = jb_xml_node_get_content_file(node, &buffer);
	if (!file)
	{
		boundary_flow_error(bf, gettext("Not enough memory"));
		goto exit2;
	}
	j = -1;
	do
	{
		t = jb_get_time_file(file, &i);
		if (!i) break;
		if (i != 6)
		{
			boundary_flow_error(bf, gettext("Bad time"));
			goto exit4;
		}
		i = fscanf(file, FRF FRF, &x, &y);
		if (i < 2) break;
		++j;
		bf->t = (JBDOUBLE*)g_try_realloc(bf->t, (j + 1) * sizeof(JBDOUBLE));
		bf->p1 = (JBFLOAT*)g_try_realloc(bf->p1, (j + 1) * sizeof(JBFLOAT));
		bf->p2 = (JBFLOAT*)g_try_realloc(bf->p2, (j + 1) * sizeof(JBFLOAT));
		if (!bf->t || !bf->p1 || !bf->p2)
		{
			boundary_flow_error(bf, gettext("Not enough memory"));
			goto exit4;
		}
		bf->t[j] = t;
		bf->p1[j] = x;
		bf->p2[j] = y;
	}
	while (1);
	bf->n = j;
	fclose(file);
	goto exit0;

type2nitime:
	if (position)
	{
		boundary_flow_error(bf, gettext("Unknow type"));
		goto exit2;
	}
	goto type2time;

type2itime:
	if (!position)
	{
		boundary_flow_error(bf, gettext("Unknow type"));
		goto exit1;
	}

type2time:
	xmlFree(buffer);
	file = jb_xml_node_get_content_file(node, &buffer);
	if (!file)
	{
		boundary_flow_error(bf, gettext("Not enough memory"));
		goto exit1;
	}
	j = -1;
	do
	{
		t = jb_get_time_file(file, &i);
		if (!i) break;
		if (i != 6)
		{
			boundary_flow_error(bf, gettext("Bad time"));
			goto exit4;
		}
		i = fscanf(file, FRF, &x);
		if (i < 1) break;
		++j;
		bf->t = (JBDOUBLE*)g_try_realloc(bf->t, (j + 1) * sizeof(JBDOUBLE));
		bf->p1 = (JBFLOAT*)g_try_realloc(bf->p1, (j + 1) * sizeof(JBFLOAT));
		if (!bf->t || !bf->p1)
		{
			boundary_flow_error(bf, gettext("Not enough memory"));
			goto exit4;
		}
		bf->t[j] = t;
		bf->p1[j] = x;
	}
	while (1);
	bf->n = j;
	fclose(file);
	goto exit0;

type2:
	xmlFree(buffer);
	file = jb_xml_node_get_content_file(node, &buffer);
	if (!file)
	{
		boundary_flow_error(bf, gettext("Not enough memory"));
		goto exit1;
	}
	j = -1;
	do
	{
		i = fscanf(file, FRF FRF, &x, &y);
		if (i < 2) break;
		++j;
		bf->p1 = (JBFLOAT*)g_try_realloc(bf->p1, (j + 1) * sizeof(JBFLOAT));
		bf->p2 = (JBFLOAT*)g_try_realloc(bf->p2, (j + 1) * sizeof(JBFLOAT));
		if (!bf->p1 || !bf->p2)
		{
			boundary_flow_error(bf, gettext("Not enough memory"));
			goto exit4;
		}
		bf->p1[j] = x;
		bf->p2[j] = y;
	}
	while (1);
	bf->n = j;
	fclose(file);
	goto exit0;

type02i:
	if (!position)
	{
		boundary_flow_error(bf, gettext("Unknow type"));
		goto exit2;
	}
	if (i != 1 || j != 1)
	{
		boundary_flow_error(bf, gettext("Bad defined"));
		goto exit2;
	}
	bf->t = (JBDOUBLE*)g_try_malloc(sizeof(JBDOUBLE));
	bf->p1 = (JBFLOAT*)g_try_malloc(sizeof(JBFLOAT));
	bf->p2 = (JBFLOAT*)g_try_malloc(sizeof(JBFLOAT));
	if (!bf->t || !bf->p1 || !bf->p2)
	{
		boundary_flow_error(bf, gettext("Not enough memory"));
		goto exit2;
	}
	bf->t[0] = 0.;
	bf->p1[0] = x;
	bf->p2[0] = y;
	bf->n = 0;
	goto exit0;

type01i:
	if (!position)
	{
		boundary_flow_error(bf, gettext("Unknow type"));
		goto exit2;
	}

type01:
	if (i != 1)
	{
		boundary_flow_error(bf, gettext("Bad defined"));
		goto exit2;
	}
	bf->t = (JBDOUBLE*)g_try_malloc(sizeof(JBDOUBLE));
	bf->p1 = (JBFLOAT*)g_try_malloc(sizeof(JBFLOAT));
	if (!bf->t || !bf->p1)
	{
		boundary_flow_error(bf, gettext("Not enough memory"));
		goto exit2;
	}
	bf->t[0] = 0.;
	bf->p1[0] = x;
	bf->n = 0;
	goto exit0;

type0i:
	if (!position)
	{
		boundary_flow_error(bf, gettext("Unknow type"));
		goto exit2;
	}

exit0:
	xmlFree(buffer);
	switch (bf->type)
	{
		case BOUNDARY_FLOW_TYPE_QT:
		case BOUNDARY_FLOW_TYPE_HT:
		case BOUNDARY_FLOW_TYPE_ZT:
		case BOUNDARY_FLOW_TYPE_QT_HT:
		case BOUNDARY_FLOW_TYPE_QT_ZT:
			bf->data = g_try_malloc(sizeof(JBFLOAT));
			if (!bf->data) goto exit1;
			bf->delay
				= jb_xml_node_get_float_with_default(node, XML_DELAY, &i, 0.);
			if (!i)
			{
				boundary_flow_error(bf, gettext("Bad delay"));
				goto exit1;
			}
	}
	#if DEBUG_BOUNDARY_FLOW_OPEN_XML
		boundary_flow_print(bf, stderr);
		fprintf(stderr, "boundary_flow_open_xml: end\n");
	#endif
	return 1;

exit4:
	fclose(file);

exit2:
	xmlFree(buffer);

exit1:
	boundary_flow_delete(bf);
	#if DEBUG_BOUNDARY_FLOW_OPEN_XML
		fprintf(stderr, "boundary_flow_open_xml: end\n");
	#endif
	return 0;
}

#if INLINE_BOUNDARY_FLOW_OPEN_XML
	#define boundary_flow_open_xml _boundary_flow_open_xml
#else
	int boundary_flow_open_xml(BoundaryFlow*, xmlNode*, int);
#endif

static inline void _boundary_flow_save_xml(BoundaryFlow *bf, xmlNode *node)
{
	int i;
	char *buffer, *buffer2, str[JB_BUFFER_SIZE];
	xmlNode *child;
	static const xmlChar *bftype[N_BOUNDARY_FLOW_TYPES] = {XML_Q, XML_H, XML_Z,
		XML_Q_H, XML_Q_Z, XML_QT, XML_HT, XML_ZT, XML_QT_HT, XML_QT_ZT, XML_QH,
		XML_QZ, XML_SUPERCRITICAL, XML_DAM, XML_PIPE, XML_GATE, XML_JUNCTION};

	#if DEBUG_BOUNDARY_FLOW_SAVE_XML
		fprintf(stderr, "boundary_flow_save_xml: start\n");
		boundary_flow_print(bf, stderr);
	#endif

	buffer = NULL;

	if (bf->position == 0)
	{
		xmlSetProp(node, XML_NAME, (const xmlChar*)bf->name);
		xmlSetProp(node, XML_INITIAL, (const xmlChar*)bf->section);
		if (bf->section2)
			xmlSetProp(node, XML_FINAL, (const xmlChar*)bf->section2);
	}
	xmlSetProp(node, XML_TYPE, bftype[bf->type]);
	jb_xml_node_set_float_with_default(node, XML_DELAY, bf->delay, 0.);

	switch (bf->type)
	{
		case BOUNDARY_FLOW_TYPE_Q_H:
			jb_xml_node_set_float(node, XML_DEPTH, bf->p2[0]);
		case BOUNDARY_FLOW_TYPE_Q:
			jb_xml_node_set_float(node, XML_DISCHARGE, bf->p1[0]);
			break;
		case BOUNDARY_FLOW_TYPE_H:
			jb_xml_node_set_float(node, XML_DEPTH, bf->p1[0]);
			break;
		case BOUNDARY_FLOW_TYPE_Z:
			jb_xml_node_set_float(node, XML_LEVEL, bf->p1[0]);
			break;
		case BOUNDARY_FLOW_TYPE_Q_Z:
			jb_xml_node_set_float(node, XML_DISCHARGE, bf->p1[0]);
			jb_xml_node_set_float(node, XML_LEVEL, bf->p2[0]);
			break;
		case BOUNDARY_FLOW_TYPE_DAM:
			jb_xml_node_set_float(node, XML_LEVEL, DAM_LEVEL(bf));
			jb_xml_node_set_float(node, XML_ROUGHNESS, DAM_ROUGHNESS(bf));
		case BOUNDARY_FLOW_TYPE_QT:
		case BOUNDARY_FLOW_TYPE_HT:
		case BOUNDARY_FLOW_TYPE_ZT:
		case BOUNDARY_FLOW_TYPE_PIPE:
		case BOUNDARY_FLOW_TYPE_GATE:
			buffer = NULL;
			for (i = 0; i <= bf->n; ++i)
			{
				buffer2 = jb_set_time(bf->t[i]);
				snprintf
					(str, JB_BUFFER_SIZE, "\n    %s "FWF, buffer2, bf->p1[i]);
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
		case BOUNDARY_FLOW_TYPE_QH:
		case BOUNDARY_FLOW_TYPE_QZ:
			for (i = 0; i <= bf->n; ++i)
			{
				snprintf(str, JB_BUFFER_SIZE, "\n    "FWF2 FWF,
					bf->p1[i], bf->p2[i]);
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
		case BOUNDARY_FLOW_TYPE_QT_HT:
		case BOUNDARY_FLOW_TYPE_QT_ZT:
			for (i = 0; i <= bf->n; ++i)
			{
				buffer2 = jb_set_time(bf->t[i]);
				snprintf(str, JB_BUFFER_SIZE, "\n    %s "FWF2 FWF,
					buffer2, bf->p1[i], bf->p2[i]);
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
		case BOUNDARY_FLOW_TYPE_JUNCTION:
			for (i = 0; i <= JUNCTION_N(bf); ++i)
			{
				child = xmlNewChild(node, 0, XML_JUNCTION, 0);
				junction_data_save_xml(JUNCTION_DATA(bf, i), child);
			}
	}
	if (bf->type == BOUNDARY_FLOW_TYPE_GATE)
	{
		jb_xml_node_set_float(node, XML_HEIGHT, GATE_HEIGHT(bf));
		jb_xml_node_set_float(node, XML_WIDTH, GATE_WIDTH(bf));
	}

	#if DEBUG_BOUNDARY_FLOW_SAVE_XML
		fprintf(stderr, "boundary_flow_save_xml: end\n");
	#endif
}

#if INLINE_BOUNDARY_FLOW_SAVE_XML
	#define boundary_flow_save_xml _boundary_flow_save_xml
#else
	void boundary_flow_save_xml(BoundaryFlow*, xmlNode*);
#endif

static inline int _boundary_flow_type(BoundaryFlow *bf, int type)
{
	int k;
	#if DEBUG_BOUNDARY_FLOW_TYPE
		fprintf(stderr, "boundary_flow_type: start\n");
	#endif
	if (!bf->position) k = boundary_flow_inner_type[type];
	else k = boundary_flow_extern_type[type];
	#if DEBUG_BOUNDARY_FLOW_TYPE
		fprintf(stderr, "boundary_flow_type: end\n");
	#endif
	return k;
}

#if INLINE_BOUNDARY_FLOW_TYPE
	#define boundary_flow_type _boundary_flow_type
#else
	int boundary_flow_type(BoundaryFlow*, int);
#endif

static inline JBDOUBLE _boundary_flow_parameter(BoundaryFlow *bf, JBDOUBLE t)
{
	register JBDOUBLE k;
	#if DEBUG_BOUNDARY_FLOW_PARAMETER
		int i;
		fprintf(stderr, "boundary_flow_parameter: start\n");
		fprintf(stderr, "BFP t="FWL" type=%d n=%d\n", t, bf->type, bf->n);
		for (i = 0; i <= bf->n; ++i)
			fprintf(stderr, "BFP i=%d t="FWL" p1="FWF"\n",
				i, bf->t[i], bf->p1[i]);
	#endif
	if (!simulating) k = bf->p1[0];
	else
	{
		switch (bf->type)
		{
			case BOUNDARY_FLOW_TYPE_QT:
			case BOUNDARY_FLOW_TYPE_HT:
			case BOUNDARY_FLOW_TYPE_ZT:
			case BOUNDARY_FLOW_TYPE_QT_HT:
			case BOUNDARY_FLOW_TYPE_QT_ZT:
				t -= bf->delay;
		}
		k = jbm_darray_farray_interpolate(t, bf->t, bf->p1, bf->n);
	}
	#if DEBUG_BOUNDARY_FLOW_PARAMETER
		fprintf(stderr, "boundary_flow_parameter: end\n");
	#endif
	return k;
}

#if INLINE_BOUNDARY_FLOW_PARAMETER
	#define boundary_flow_parameter _boundary_flow_parameter
#else
	JBDOUBLE boundary_flow_parameter(BoundaryFlow*, JBDOUBLE);
#endif

static inline JBDOUBLE _boundary_flow_parameter2(BoundaryFlow *bf, JBDOUBLE t)
{
	register JBDOUBLE k;
	#if DEBUG_BOUNDARY_FLOW_PARAMETER2
		int i;
		fprintf(stderr, "boundary_flow_parameter2: start\n");
		fprintf(stderr, "BFP2 t="FWL"\n", t);
		for (i = 0; i <= bf->n; ++i)
			fprintf(stderr, "BFP2 i=%d t="FWL" p2="FWF"\n",
				i, bf->t[i], bf->p2[i]);
	#endif
	if (!simulating) k = bf->p2[0];
	else
	{
		switch (bf->type)
		{
			case BOUNDARY_FLOW_TYPE_QT:
			case BOUNDARY_FLOW_TYPE_HT:
			case BOUNDARY_FLOW_TYPE_ZT:
			case BOUNDARY_FLOW_TYPE_QT_HT:
			case BOUNDARY_FLOW_TYPE_QT_ZT:
				t -= bf->delay;
		}
		k = jbm_darray_farray_interpolate(t, bf->t, bf->p2, bf->n);
	}
	#if DEBUG_BOUNDARY_FLOW_PARAMETER2
		fprintf(stderr, "boundary_flow_parameter2: end\n");
	#endif
	return k;
}

#if INLINE_BOUNDARY_FLOW_PARAMETER2
	#define boundary_flow_parameter2 \
		_boundary_flow_parameter2
#else
	JBDOUBLE boundary_flow_parameter2(BoundaryFlow*, JBDOUBLE);
#endif

static inline JBDOUBLE _boundary_flow_parameter3(BoundaryFlow *bf, JBDOUBLE p)
{
	register JBDOUBLE k;
	#if DEBUG_BOUNDARY_FLOW_PARAMETER3
		int i;
		fprintf(stderr, "boundary_flow_parameter3: start\n");
		fprintf(stderr, "BFP3 p="FWF" type=%d n=%d\n", p, bf->type, bf->n);
		for (i = 0; i <= bf->n; ++i)
			fprintf(stderr, "BFP3 i=%d p1="FWF" p2="FWF"\n",
				i, bf->p1[i], bf->p2[i]);
	#endif
	k = jbm_farray_interpolate(p, bf->p1, bf->p2, bf->n);
	#if DEBUG_BOUNDARY_FLOW_PARAMETER3
		fprintf(stderr, "boundary_flow_parameter3: end\n");
	#endif
	return k;
}

#if INLINE_BOUNDARY_FLOW_PARAMETER3
	#define boundary_flow_parameter3 _boundary_flow_parameter3
#else
	JBDOUBLE boundary_flow_parameter3(BoundaryFlow*, JBDOUBLE);
#endif

static inline JBDOUBLE _boundary_flow_integral
	(BoundaryFlow *bf, JBDOUBLE t, JBDOUBLE tmax)
{
	register JBDOUBLE k;
	#if DEBUG_BOUNDARY_FLOW_INTEGRAL
		int i;
		fprintf(stderr, "boundary_flow_integral: start\n");
		fprintf(stderr, "BFI t="FWL" tmax="FWL"\n", t, tmax);
		for (i = 0; i <= bf->n; ++i)
			fprintf(stderr, "BFI i=%d t="FWL" p1="FWF"\n",
				i, bf->t[i], bf->p1[i]);
	#endif
	switch (bf->type)
	{
		case BOUNDARY_FLOW_TYPE_QT:
		case BOUNDARY_FLOW_TYPE_HT:
		case BOUNDARY_FLOW_TYPE_ZT:
		case BOUNDARY_FLOW_TYPE_QT_HT:
		case BOUNDARY_FLOW_TYPE_QT_ZT:
			if (!simulating) return bf->p1[0] * (tmax - t);
			t -= bf->delay;
	}
	k = jbm_darray_farray_integral(bf->t, bf->p1, bf->n, t, tmax);
	#if DEBUG_BOUNDARY_FLOW_INTEGRAL
		fprintf(stderr, "boundary_flow_integral: end\n");
	#endif
	return k;
}

#if INLINE_BOUNDARY_FLOW_INTEGRAL
	#define boundary_flow_integral _boundary_flow_integral
#else
	JBDOUBLE boundary_flow_integral(BoundaryFlow*, JBDOUBLE, JBDOUBLE);
#endif

#endif
